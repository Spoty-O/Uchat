// Client side C/C++ program to demonstrate Socket programming
#include "./inc/header.h"

#define PORT 8080
int sock;

typedef struct s_user_rooms {
    int id;
    char *name;
    struct s_user_rooms *next;
    struct s_user_rooms *prev;
}              t_user_rooms__t;

typedef struct s_user_info {
    int user_id;
    const char *username;
}              t_user_info;

// user_room list
t_user_rooms__t *create_node__t(int id, char *name);
void node_push_back__t(t_user_rooms__t **list, int id, char *name);
void clear_list_user_rooms__t__t(t_user_rooms__t **list);
t_user_rooms__t *delete_user_room__t(t_user_rooms__t **list, int id);

t_user_rooms__t *auth(t_user_info *user_info, bool is_reg, const char *user, const char *pass);
bool send_message(t_user_info *user_info, int jroom_id, const char *msg);

t_user_rooms__t *create_node__t(int id, char *name) {
    t_user_rooms__t *node = malloc(sizeof(t_user_rooms__t));
    node->id = id;
    node->name = name;
    node->next = NULL;
    node->prev = NULL;
    return node;
}

void node_push_back__t(t_user_rooms__t **list, int id, char *name) {
    // printf("%s ", entry->d_name);
    if (!*list) {
        *list = create_node__t(id, name);
        return;
    }
    t_user_rooms__t *temp = *list;
    while (temp->next) {
        temp = temp->next;
    }
    t_user_rooms__t *prev_node = temp;
    temp->next = create_node__t(id, name);
    temp = temp->next;
    temp->prev = prev_node;
}

void clear_list_user_rooms__t__t(t_user_rooms__t **list) {
    if (!*list) return; 
    t_user_rooms__t *temp;
    while (*list) {
        temp = (*list)->next;
        (*list)->next = NULL;
        free(*list);
        *list = temp;
    }
    list = NULL;
}

t_user_rooms__t *delete_user_room__t(t_user_rooms__t **list, int id) {
    if (!*list) return NULL; 
    t_user_rooms__t *start = (*list);
    t_user_rooms__t *temp = NULL;
    while (*list) {
        if ((*list)->id == id) {
            if (!(*list)->prev) {
                if ((*list)->next) {
                    temp = (*list)->next;
                    temp->prev = NULL;
                    free(*list);
                    return temp;
                }
                else return NULL;
            }
            temp = (*list)->prev;
            if (temp->next->next) {
                temp->next = (*list)->next;
                (*list)->next->prev = temp;
            }
            else {
                temp->next = NULL;
            }
            free(*list);
            return start;
        }
        (*list) = (*list)->next;
    }
    return start;
}

t_user_rooms__t *auth(t_user_info *user_info, bool is_reg, const char *user, const char *pass) {
    // printf("%s\n", crypt("pass", "sol"));
    t_user_rooms__t *user_rooms = NULL;
    char server_res[1024];
    json_object *jobj = json_object_new_object();
    struct json_object *json_str;
    struct json_object *juser_id, *juser;    // auth
    struct json_object *juser_rooms, *jtmp; // rooms
    struct json_object *jerror;

    json_object_object_add(jobj,"type", json_object_new_string("auth"));
    json_object_object_add(jobj,"is_reg", json_object_new_boolean(is_reg));
    json_object_object_add(jobj,"username", json_object_new_string(user));
    json_object_object_add(jobj,"password", json_object_new_string(pass));

    const char *obj_text = json_object_to_json_string(jobj);
    int send_res = send(sock, obj_text, strlen(obj_text), 0);
    if (send_res < 0) {
        fprintf(stderr, "Send message error\n");
    }
    if ((recv(sock, server_res, sizeof(server_res), 0)) > 0) {
        
        json_str = json_tokener_parse(server_res);
        json_object_object_get_ex(json_str, "error", &jerror);
        
        if (json_object_get_string(jerror) == NULL) {
            // printf("server res: %s\n\n",server_res);   // потом убрать

            json_object_object_get_ex(json_str, "user_id", &juser_id);
            json_object_object_get_ex(json_str, "username", &juser);
            json_object_object_get_ex(json_str, "user_rooms", &juser_rooms);

            user_info->user_id = json_object_get_int(juser_id);
            user_info->username = json_object_get_string(juser);
            if (juser_rooms != NULL) {
                for (int i = 0; i < (int)json_object_array_length(juser_rooms); i++) {
                    jtmp = json_object_array_get_idx(juser_rooms, i);
                    node_push_back__t(&user_rooms, json_object_get_int(json_object_object_get(jtmp, "id")),
                                (char *)json_object_get_string(json_object_object_get(jtmp, "name")));
                }
            }
        }
        else {
            fprintf(stderr, "%s\n", server_res);
            return NULL;
        }
    }
    else {
        fprintf(stderr, "recv error\n");
    }
    return user_rooms;
}

bool send_message(t_user_info *user_info, int jroom_id, const char *msg) {

    char server_res[1024];
    json_object *jobj = json_object_new_object();
    struct json_object *json_str;
    struct json_object *jerror;
    
    json_object_object_add(jobj,"type", json_object_new_string("send_message"));
    json_object_object_add(jobj,"room_id", json_object_new_int(jroom_id));
    json_object_object_add(jobj,"user_id", json_object_new_int(user_info->user_id));
    json_object_object_add(jobj,"message", json_object_new_string(msg));
    
    const char *obj_text = json_object_to_json_string(jobj);
    int send_res = send(sock, obj_text, strlen(obj_text), 0);
    if (send_res < 0)
        fprintf(stderr, "Send message error\n");
    if ((recv(sock, server_res, sizeof(server_res), 0)) > 0) {
        json_str = json_tokener_parse(server_res);
        json_object_object_get_ex(json_str, "error", &jerror);
        
        if (json_object_get_string(jerror) == NULL) {
            // printf("server res: %s\n",server_res);
            return false;
        }
        else {
            fprintf(stderr, "%s\n", server_res);
            return true;
        }
    }
    else {
        fprintf(stderr, "recv error\n");
    }
    return true;
}


int main(int argc, char const *argv[]) {
    sock = 0;
    int valread;
    struct sockaddr_in serv_addr;
    char *text = NULL;
    size_t text_size = 0;
    char buffer[1024] = {0};
    char *our_ip = "127.0.0.1";
    if (argc > 1) {
        our_ip = argv[1];
    }
    // char *text = NULL;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    
    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, our_ip, &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
    
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }
    t_user_info *user_info = (t_user_info*)malloc(sizeof(t_user_info));
    t_user_rooms__t *user_rooms = auth(user_info, true, "asd", "qqq");
    user_rooms = auth(user_info, true, "qwe", "qqq");
    user_rooms = auth(user_info, false, "asd", "qqq");
    while(1) {
        printf( "Client: ");
        getline(&text, &text_size, stdin);

        send_message(user_info, 2, text);

    }
    close(sock);
    return 0;
}

