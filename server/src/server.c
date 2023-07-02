// Server side C/C++ program to demonstrate Socket programming
#include "../inc/header.h"

void daemonize() {
    pid_t pid, sid;
    pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (pid > 0) {
        printf("The process id is: %d\n", pid);
        exit(EXIT_SUCCESS);
    }
    umask(0);
    if ((sid = setsid()) < 0) {
        perror("setsid");
        exit(EXIT_FAILURE);
    }
    signal(SIGTERM, SIG_DFL);
}

void *handle_clnt(void *arg);
int clnt_cnt = 0;
int clnt_socks[MAX_CLNT];
pthread_mutex_t mutx;

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "usage: ./uchat_server [port]\n");
        return -1;
    }
    // daemonize();
    int PORT = mx_atoi(argv[1]);

    int server_fd = -1; 
    int new_socket = -1;
    struct sockaddr_in address, clnt_adr;
    socklen_t clnt_adr_sz;
    pthread_t t_id;
    int opt = 1;
    id_sock *client = NULL;
    t_pthread_data pthread_data;
    pthread_data.client = &client;

    // databases
    sqlite3 *db;
    int  rc;

    // Creating socket file descriptor
    pthread_mutex_init(&mutx, NULL);
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    memset(&address, 0, sizeof(address));
    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR , &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; //inet_addr("10.11.7.13");
    address.sin_port = htons(PORT);
    
    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    char HostName[64]; 
    gethostname(HostName, 60);
    char *ip=gethostbyname(HostName)->h_addr_list[0]; 
    printf ("Connect on ip address: %u.%u.%u.%u\n",ip[0],ip[1],ip[2],ip[3]);
    
    /* Open database */
    rc = sqlite3_open("test.db", &db);
    if(rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        exit(0);
    } else {
        fprintf(stdout, "Opened database successfully\n");
    }

    if (database_create(db) == 0) {
        if (listen(server_fd, 3) < 0) {
            perror("listen");
            exit(EXIT_FAILURE);
        }
        pthread_data.db = db;
    //******
        // get_room_name(db, 1);
        // json_object *jobj = json_object_new_object();
        
        // get_user_info_obj(db, "ooo", jobj);
        // get_user_rooms_obj(db, jobj, 5);
        // printf ("str: %s\n", json_object_to_json_string(jobj));
        // printf("%s", crypt("password", "sol"));
    //******
        while (1) {
            clnt_adr_sz = sizeof(clnt_adr);
            new_socket = accept (server_fd, (struct sockaddr *)&clnt_adr, &clnt_adr_sz); // Блокировать, отслеживать запрос на подключение к службе поддержки клиентов
            pthread_data.new_socket = new_socket;
            // критическая секция
            pthread_mutex_lock (&mutx); // Блокировка
            clnt_socks[clnt_cnt++] = new_socket; // Недавно подключенный клиент сохраняется в массиве clnt_socks
            pthread_mutex_unlock (&mutx); // снять блокировку

            // Создать тему
            pthread_create(&t_id, NULL, handle_clnt, (void*)&pthread_data);
            pthread_detach (t_id); // Уничтожить поток

            printf("client connected, count: %d \n", clnt_cnt);
        }
    }
    sqlite3_close(db);
    close(server_fd);

    printf("Good bye! ^_^\n");
    return 0;
}

void *handle_clnt(void *arg) {
    t_pthread_data pthread_data = *((t_pthread_data *)arg);
    int i;
    char msg[1024];

    // while ((str_len = read(clnt_sock, msg, sizeof(msg))) != 0)
    //     send_msg(msg, str_len);

         // Удалить текущего клиента из массива
    int recv_res = 0;

    
    struct json_object *json_str;
    struct json_object *jtype, *jis_reg, *juser, *jpass; // auth
    struct json_object *jroom_id, *juser_id, *jmsg, *jstart, *jcount; // message
    struct json_object *jroom_name, *jfriend; // room
    int user_id = -1;
    int room_id = -1;
    char *suser, *spass, *stype;
    char *obj_text = NULL;
    // id_sock *lol = NULL;
    while (1) {
        if ((recv_res = recv(pthread_data.new_socket, msg, sizeof(msg), 0)) > 0) {
            // printf("recv: %s\n\n", msg);
            json_object *jobj = json_object_new_object();
            json_str = json_tokener_parse(msg);
            json_object_object_get_ex(json_str, "type", &jtype);
            stype = strdup(json_object_get_string(jtype));
            if (json_object_get_string(jtype)) {
                if (strcmp(stype, "auth") == 0) {
                    printf("client snd: %s\n", msg);
                    (*pthread_data.client) = delete_user_data(pthread_data.client, pthread_data.new_socket);
                    mx_strdel(&suser);
                    mx_strdel(&spass);
                    json_object_object_get_ex(json_str, "is_reg", &jis_reg);
                    json_object_object_get_ex(json_str, "username", &juser);
                    json_object_object_get_ex(json_str, "password", &jpass);
                    char *key = "sOl";
                    suser = strdup(json_object_get_string(juser));
                    spass = strdup(crypt(json_object_get_string(jpass), key));
                    
                    if (json_object_get_boolean(jis_reg) == true) {
                        printf("reg:\n");
                        if (suser && spass && (registration(pthread_data.db, suser, spass) == 0)) {
                            user_id = get_user_id(pthread_data.db, suser);
                            get_user_info_obj(user_id, suser, jobj);
                            get_user_rooms_obj(pthread_data.db, jobj, get_user_id(pthread_data.db, suser));
                            obj_text = (char *)json_object_to_json_string(jobj);
                            if (send(pthread_data.new_socket, obj_text, strlen(obj_text), 0) < 0)
                                fprintf(stderr, "Send message error\n");
                            printf("serv snd: %s\n", obj_text);
                        }
                        else {
                            mx_strdel(&suser);
                            mx_strdel(&spass);
                            error_obj(jobj, "User exist");
                            obj_text = (char *)json_object_to_json_string(jobj);
                            if (send(pthread_data.new_socket, obj_text, strlen(obj_text), 0) < 0)
                                fprintf(stderr, "Send message error\n");
                            continue;
                        }
                    }
                    else {
                        printf("auth:\n");
                        if (authorization(pthread_data.db, suser, spass) == 0) {
                            user_id = get_user_id(pthread_data.db, suser);
                            get_user_info_obj(user_id, suser, jobj);
                            get_user_rooms_obj(pthread_data.db, jobj, user_id);
                            obj_text = (char *)json_object_to_json_string(jobj);
                            if (send(pthread_data.new_socket, obj_text, strlen(obj_text), 0) < 0)
                                fprintf(stderr, "Send message error\n");
                        }
                        else {
                            mx_strdel(&suser);
                            mx_strdel(&spass);
                            error_obj(jobj, "Username or password is not correct");
                            obj_text = (char *)json_object_to_json_string(jobj);
                            if (send(pthread_data.new_socket, obj_text, strlen(obj_text), 0) < 0)
                                fprintf(stderr, "Send message error\n");
                            continue;
                        }
                    }
                    node_push_back_data(pthread_data.client, user_id, pthread_data.new_socket);
                    printf( "user: %s\npass: %s\n", suser, spass);
                }
                else if (strcmp(stype, "add_room") == 0 && suser) {
                    json_object_object_get_ex(json_str, "room_name", &jroom_name);
                    json_object_object_get_ex(json_str, "friend", &jfriend);
                    if (jroom_name) {
                        int new_room_id = add_room(pthread_data.db, json_object_get_string(jroom_name));
                        if (new_room_id > 0) {
                            if (add_user_to_room(pthread_data.db, new_room_id, user_id) == 0) {
                                if (jfriend) {
                                    int friend_id = get_user_id(pthread_data.db, json_object_get_string(jfriend));
                                    if (friend_id > 0)
                                        add_user_to_room(pthread_data.db, new_room_id, friend_id);
                                }
                                json_object_object_add(jobj,"type", json_object_new_string("room_added"));
                                json_object_object_add(jobj,"new_room_id", json_object_new_int(new_room_id));
                                json_object_object_add(jobj,"room_name", jroom_name);
                                obj_text = (char *)json_object_to_json_string(jobj);
                                printf("send: %s\n",obj_text);
                                if (send(pthread_data.new_socket, obj_text, strlen(obj_text), 0) < 0)
                                    fprintf(stderr, "Send message error\n");
                                resend_message(pthread_data, jobj, new_room_id, user_id);
                                
                                
                                // lol = *pthread_data.client;
                                // while (lol) {
                                //     for (int i = 0; i < count_users_in_room; i++) {
                                //         if ((lol->user_id == users_in_room[i]) && (lol->user_id != user_id)) {
                                //             printf("send to: [ %d ]\n", lol->user_id);
                                //             json_object_object_add(jobj,"type", json_object_new_string("send_message"));
                                //             printf("2[%s]\n", suser);
                                //             get_user_info_obj(user_id, suser, jobj);
                                //             json_object_object_add(jobj,"room_id", json_object_new_int(room_id));
                                //             json_object_object_add(jobj,"message", json_object_new_string(json_object_get_string(jmsg)));
                                //             obj_text = (char *)json_object_to_json_string(jobj);
                                //             printf("%s\n",obj_text);
                                //             if (send(lol->user_socket, obj_text, strlen(obj_text), 0) < 0)
                                //                 fprintf(stderr, "Send message error\n");
                                //         }
                                //     }
                                //     lol = lol->next;
                                // }
                            }
                        }
                        else {
                            error_obj(jobj, "room_error");
                            obj_text = (char *)json_object_to_json_string(jobj);
                            printf("send err: %s\n", obj_text);
                            if (send(pthread_data.new_socket, obj_text, strlen(obj_text), 0) < 0)
                                fprintf(stderr, "Send message error\n");
                        }
                    }
                    else {
                        error_obj(jobj, "user, username is null or user is not login");
                        obj_text = (char *)json_object_to_json_string(jobj);
                        if (send(pthread_data.new_socket, obj_text, strlen(obj_text), 0) < 0)
                            fprintf(stderr, "Send message error\n");
                    }
                }
                else if (strcmp(stype, "add_member") == 0 && suser) {
                    json_object_object_get_ex(json_str, "room_id", &jroom_id);
                    json_object_object_get_ex(json_str, "friend", &jfriend);
                    if (jroom_id && jfriend) {
                        room_id = json_object_get_int(jroom_id);
                        char *room_name = get_room_name(pthread_data.db, room_id);
                        int add_user_id = get_user_id(pthread_data.db, json_object_get_string(jfriend));
                        if (add_member(pthread_data.db, room_id, add_user_id) == 1) {
                            printf("s\n");
                            json_object_object_add(jobj,"type", json_object_new_string("room_added"));
                            json_object_object_add(jobj,"new_room_id", json_object_new_int(room_id));
                            json_object_object_add(jobj,"room_name", json_object_new_string(room_name));
                            // obj_text = (char *)json_object_to_json_string(jobj);
                            // printf("send: %s\n",obj_text);
                            // if (send(pthread_data.new_socket, obj_text, strlen(obj_text), 0) < 0)
                            //     fprintf(stderr, "Send message error\n");
                            // // free(jobj);
                            // json_object_object_add(jobj,"type", json_object_new_string("room_added"));
                            // json_object_object_add(jobj,"new_room_id", json_object_new_int(room_id));
                            // json_object_object_add(jobj,"room_name", json_object_new_string(room_name));
                            resend_to_user_id(pthread_data, jobj, add_user_id);
                        }
                        else {
                            error_obj(jobj, "member_error");
                            obj_text = (char *)json_object_to_json_string(jobj);
                            if (send(pthread_data.new_socket, obj_text, strlen(obj_text), 0) < 0)
                                fprintf(stderr, "Send message error\n");
                        }
                    }
                    else {
                        error_obj(jobj, "user, username is null or user is not login");
                        obj_text = (char *)json_object_to_json_string(jobj);
                        if (send(pthread_data.new_socket, obj_text, strlen(obj_text), 0) < 0)
                            fprintf(stderr, "Send message error\n");
                    }
                }
                else if (strcmp(stype, "delete_room") == 0 && suser) {
                    json_object_object_get_ex(json_str, "room_id", &jroom_id);
                    if (jroom_id) {
                        room_id = json_object_get_int(jroom_id);
                        json_object_object_add(jobj,"type", json_object_new_string("room_deleted"));
                        json_object_object_add(jobj,"room_id", json_object_new_int(room_id));
                        resend_message(pthread_data, jobj, room_id, user_id);
                        if (delete_room(pthread_data.db, room_id)) {
                            obj_text = (char *)json_object_to_json_string(jobj);
                            printf("send: %s\n",obj_text);
                            if (send(pthread_data.new_socket, obj_text, strlen(obj_text), 0) < 0)
                                fprintf(stderr, "Send message error\n");
                        }
                        else {
                            error_obj(jobj, "del_room_error");
                            obj_text = (char *)json_object_to_json_string(jobj);
                            if (send(pthread_data.new_socket, obj_text, strlen(obj_text), 0) < 0)
                                fprintf(stderr, "Send message error\n");
                        }
                    }
                    else {
                        error_obj(jobj, "user, username is null or user is not login");
                        obj_text = (char *)json_object_to_json_string(jobj);
                        if (send(pthread_data.new_socket, obj_text, strlen(obj_text), 0) < 0)
                            fprintf(stderr, "Send message error\n");
                    }
                }
                else if (strcmp(stype, "delete_member") == 0 && suser) {
                    json_object_object_get_ex(json_str, "room_id", &jroom_id);
                    json_object_object_get_ex(json_str, "friend", &jfriend);
                    if (jroom_id && jfriend) {
                        room_id = json_object_get_int(jroom_id);
                        int member_id = get_user_id(pthread_data.db, json_object_get_string(jfriend));
                        if (delete_member(pthread_data.db, room_id, member_id)) {
                            json_object_object_add(jobj,"type", json_object_new_string("member_deleted"));
                            json_object_object_add(jobj,"room_id", json_object_new_int(room_id));
                            json_object_object_add(jobj,"friend", jfriend);
                            obj_text = (char *)json_object_to_json_string(jobj);
                            printf("send: %s\n",obj_text);
                            if (send(pthread_data.new_socket, obj_text, strlen(obj_text), 0) < 0)
                                fprintf(stderr, "Send message error\n");
                        }
                        else {
                            error_obj(jobj, "del_member_error");
                            obj_text = (char *)json_object_to_json_string(jobj);
                            if (send(pthread_data.new_socket, obj_text, strlen(obj_text), 0) < 0)
                                fprintf(stderr, "Send message error\n");
                        }
                    }
                    else {
                        error_obj(jobj, "user, username is null or user is not login");
                        obj_text = (char *)json_object_to_json_string(jobj);
                        if (send(pthread_data.new_socket, obj_text, strlen(obj_text), 0) < 0)
                            fprintf(stderr, "Send message error\n");
                    }
                }
                else if (strcmp(stype, "send_message") == 0 && suser) {
                    printf("1[%s]\n", suser);
                    json_object_object_get_ex(json_str, "room_id", &jroom_id);
                    json_object_object_get_ex(json_str, "user_id", &juser_id);
                    json_object_object_get_ex(json_str, "message", &jmsg);
                    if (juser_id && jroom_id && jmsg && (json_object_get_int(juser_id) == user_id)) {
                        room_id = json_object_get_int(jroom_id);
                        printf( "room_id: %d\nuser_id: %d\nmessage: %s\n", room_id, user_id, json_object_get_string(jmsg));
                        if (add_message(pthread_data.db, user_id, room_id, json_object_get_string(jmsg)) == 0) {
                            if (send(pthread_data.new_socket, "ok", strlen("ok"), 0) < 0)
                                fprintf(stderr, "Send message error\n");
                            
                        //*****  
                            json_object_object_add(jobj,"type", json_object_new_string("send_message"));
                            get_user_info_obj(user_id, suser, jobj);
                            json_object_object_add(jobj,"room_id", json_object_new_int(room_id));
                            json_object_object_add(jobj,"message", json_object_new_string(json_object_get_string(jmsg)));
                            resend_message(pthread_data, jobj, room_id, user_id);
                        //*****

                            // int count_users_in_room = get_count_usr_in_room(pthread_data.db, room_id);
                            // if (count_users_in_room >= 0) {
                            //     int users_in_room[count_users_in_room];
                            //     get_users_in_room(pthread_data.db, 1, users_in_room);
                            //     lol = *pthread_data.client;
                            //     while (lol) {
                            //         for (int i = 0; i < count_users_in_room; i++) {
                            //             if ((lol->user_id == users_in_room[i]) && (lol->user_id != user_id)) {
                            //                 printf("send to: [ %d ]\n", lol->user_id);
                            //                 json_object_object_add(jobj,"type", json_object_new_string("send_message"));
                            //                 printf("2[%s]\n", suser);
                            //                 get_user_info_obj(user_id, suser, jobj);
                            //                 json_object_object_add(jobj,"room_id", json_object_new_int(room_id));
                            //                 json_object_object_add(jobj,"message", json_object_new_string(json_object_get_string(jmsg)));
                            //                 obj_text = (char *)json_object_to_json_string(jobj);
                            //                 printf("%s\n",obj_text);
                            //                 if (send(lol->user_socket, obj_text, strlen(obj_text), 0) < 0)
                            //                     fprintf(stderr, "Send message error\n");
                            //             }
                            //         }
                            //         lol = lol->next;
                            //     }
                            //     printf("\n[ok]\n");
                            // }
                        }
                        else {
                            error_obj(jobj, "user is not login");
                            obj_text = (char *)json_object_to_json_string(jobj);
                            if (send(pthread_data.new_socket, obj_text, strlen(obj_text), 0) < 0)
                                fprintf(stderr, "Send message error\n");
                        }
                    }
                    else {
                        error_obj(jobj, "user is not login");
                        obj_text = (char *)json_object_to_json_string(jobj);
                        if (send(pthread_data.new_socket, obj_text, strlen(obj_text), 0) < 0)
                            fprintf(stderr, "Send message error\n");
                    }
                }
                else if (strcmp(stype, "get_message") == 0 && suser) {
                    json_object_object_get_ex(json_str, "room_id", &jroom_id);
                    json_object_object_get_ex(json_str, "start", &jstart);
                    json_object_object_get_ex(json_str, "count", &jcount);
                    if (jroom_id && jstart && jcount) {
                        int start = json_object_get_int(jstart);
                        int count = json_object_get_int(jcount);
                        room_id = json_object_get_int(jroom_id);

                        json_object_object_add(jobj,"type", jtype);
                        json_object_object_add(jobj,"room_id", json_object_new_int(room_id));

                        get_message(pthread_data.db, jobj, room_id, start, count);
                        obj_text = (char *)json_object_to_json_string(jobj);
                        printf("send: %s\n",obj_text);
                        if (send(pthread_data.new_socket, obj_text, strlen(obj_text), 0) < 0)
                            fprintf(stderr, "Send message error\n");
                    }
                }
                else {
                    error_obj(jobj, "Unknown request!");
                    obj_text = (char *)json_object_to_json_string(jobj);
                    if (send(pthread_data.new_socket, obj_text, strlen(obj_text), 0) < 0)
                        fprintf(stderr, "Send message error\n");
                }
            }
            else {
                error_obj(jobj, "Unknown request!");
                obj_text = (char *)json_object_to_json_string(jobj);
                if (send(pthread_data.new_socket, obj_text, strlen(obj_text), 0) < 0)
                    fprintf(stderr, "Send message error\n");
            }
            mx_strdel(&obj_text);
            mx_strdel(&stype);
            free(jobj);
        }
        else {
            fprintf(stderr, "error recv\n");
            break;
        }
    }
    pthread_mutex_lock(&mutx);
    
    (*pthread_data.client) = delete_user_data(pthread_data.client, pthread_data.new_socket);
    for (i = 0; i < clnt_cnt; i++) {
        if (pthread_data.new_socket == clnt_socks[i]) {
            while (i++ < clnt_cnt - 1)
                clnt_socks[i] = clnt_socks[i + 1];
            break;
        }
    }
    clnt_cnt--;
    pthread_mutex_unlock(&mutx);
    close(pthread_data.new_socket);
    printf("client disconnected, count: %d\n", i - 1);
    // clear data
    // json_object_put(json_str);
    // json_object_put(jtype);
    // json_object_put(jis_reg);
    // json_object_put(juser);
    // json_object_put(jpass);
    // json_object_put(jroom_id);
    // json_object_put(juser_id);
    // json_object_put(jmsg);
    // json_object_put(jroom_name);
    // json_object_put(jfriend);
    mx_strdel(&suser);
    mx_strdel(&spass);
    return NULL;
}

void resend_to_user_id(t_pthread_data pthread_data, json_object *jobj, int user_id) {
    printf("send from:%d\n",user_id);
    id_sock *lol = NULL;
    
    lol = *pthread_data.client;
    while (lol) {
        if (lol->user_id == user_id) {
            printf("send to1: [ %d ]\n", lol->user_id);
            char * obj_text = (char *)json_object_to_json_string(jobj);
            printf("%s\n",obj_text);
            if (send(lol->user_socket, obj_text, strlen(obj_text), 0) < 0)
                fprintf(stderr, "Send message error\n");
        }
        lol = lol->next;
    }
    printf("\n[ok1]\n");
}

void resend_message(t_pthread_data pthread_data, json_object *jobj, int room_id, int user_id) {
    printf("send from:%d\n",user_id);
    id_sock *lol = NULL;
    int count_users_in_room = get_count_usr_in_room(pthread_data.db, room_id);
    if (count_users_in_room >= 0) {
        int users_in_room[count_users_in_room];

        get_users_in_room(pthread_data.db, room_id, users_in_room);
        lol = *pthread_data.client;
        while (lol) {
            // printf("room_id: %d\ncount_usrs_in_r: %d\n",room_id, count_users_in_room);
            for (int i = 0; i < count_users_in_room; i++) {
                // printf("lol->user_id:%d\t users_in_room: %d\t user_id:%d\n",lol->user_id,users_in_room[i], user_id);
                if ((lol->user_id == users_in_room[i]) && (lol->user_id != user_id)) {
                    printf("send to: [ %d ]\n", lol->user_id);
                    char * obj_text = (char *)json_object_to_json_string(jobj);
                    printf("%s\n",obj_text);
                    if (send(lol->user_socket, obj_text, strlen(obj_text), 0) < 0)
                        fprintf(stderr, "Send message error\n");
                }
            }
            lol = lol->next;
        }
        printf("\n[ok]\n");
    }
}

