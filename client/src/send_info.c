#include "../inc/header.h"

t_user_rooms *auth(bool is_reg, const char *user, const char *pass) {
    // printf("%s\n", crypt("pass", "sol"));
    t_user_rooms *user_rooms = NULL;
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
            printf("server res: %s\n\n",server_res);   // потом убрать

            json_object_object_get_ex(json_str, "user_id", &juser_id);
            json_object_object_get_ex(json_str, "username", &juser);
            json_object_object_get_ex(json_str, "user_rooms", &juser_rooms);

            user_info.user_id = json_object_get_int(juser_id);
            user_info.username = json_object_get_string(juser);
            if (juser_rooms != NULL) {
                for (int i = 0; i < (int)json_object_array_length(juser_rooms); i++) {
                    jtmp = json_object_array_get_idx(juser_rooms, i);
                    node_push_back(&user_rooms, json_object_get_int(json_object_object_get(jtmp, "id")),
                                (char *)json_object_get_string(json_object_object_get(jtmp, "name")));
                }
            }
        }
        else {
            fprintf(stderr, "%s\n", server_res);
            return NULL;
        }
        for (int i = 0; i < 1024; i++) {
            server_res[i] = '\0';
        }
    }
    else {
        fprintf(stderr, "recv error\n");
    }
    //clear data
    free(jobj);
    // json_object_put(json_str);
    // json_object_put(juser_id);
    // json_object_put(juser);
    // json_object_put(jtmp);
    // json_object_put(jerror);
    return user_rooms;
}

int add_room(const char *room_name, char *friend) {
    printf("func_add_room\n");
    json_object *jobj = json_object_new_object();
    // char server_res[1024];
    // struct json_object *json_str, *jtype;
    // struct json_object *jnew_room_id, *jroom_name;

    json_object_object_add(jobj,"type", json_object_new_string("add_room"));
    json_object_object_add(jobj,"room_name", json_object_new_string(room_name));
    if (friend)
        json_object_object_add(jobj,"friend", json_object_new_string(friend));
    
    const char *obj_text = json_object_to_json_string(jobj);
    printf("%s\n", obj_text);
    int send_res = send(sock, obj_text, strlen(obj_text), 0);
    if (send_res < 0)
        fprintf(stderr, "Send message error\n");
    
    // if ((recv(sock, server_res, sizeof(server_res), 0)) > 0) {
        
    //     json_str = json_tokener_parse(server_res);
    //     json_object_object_get_ex(json_str, "type", &jtype);
        
    //     if (jtype && mx_strcmp(json_object_get_string(jtype), "room_added") == 0) {
    //         printf("add_room|\n");
    //         json_object_object_get_ex(json_str, "room_name", &jroom_name);
    //         json_object_object_get_ex(json_str, "new_room_id", &jnew_room_id);
    //         int new_room_id = json_object_get_int(jnew_room_id);
    //         if (new_room_id > 0 && jroom_name) {
    //             node_push_back(&user_rooms, new_room_id, (char *)json_object_get_string(jroom_name));
    //         }
    //     }
    //     else {
    //         fprintf(stderr, "%s\n", server_res);
    //         // return NULL;
    //     }
    // }
    // else {
    //     fprintf(stderr, "recv error\n");
    // }
    free(jobj);
    return 0;
}

void add_member(int room_id, char *friend) {
    printf("func_add_member\n");
    json_object *jobj = json_object_new_object();
    // char server_res[1024];
    // struct json_object *json_str, *jtype;
    // struct json_object *jfriend, *jroom_id;

    json_object_object_add(jobj,"type", json_object_new_string("add_member"));
    json_object_object_add(jobj,"room_id", json_object_new_int(room_id));
    json_object_object_add(jobj,"friend", json_object_new_string(friend));
    
    const char *obj_text = json_object_to_json_string(jobj);
    printf("%s\n", obj_text);
    int send_res = send(sock, obj_text, strlen(obj_text), 0);
    if (send_res < 0)
        fprintf(stderr, "Send message error\n");
    // if ((recv(sock, server_res, sizeof(server_res), 0)) > 0) {
    //     printf("add_member|\n");
    //     json_str = json_tokener_parse(server_res);
    //     json_object_object_get_ex(json_str, "type", &jtype);
    //     json_object_object_get_ex(json_str, "room_id", &jroom_id);
    //     json_object_object_get_ex(json_str, "friend", &jfriend);
    //     int res_room_id = json_object_get_int(jroom_id);
    //     if (res_room_id > 0 && jfriend && jtype && !strcmp(json_object_get_string(jtype), "member_added")) {
    //         printf("member: %s added in room_id: %d\n", json_object_get_string(jfriend), res_room_id);
    //     }
    //     else {
    //         fprintf(stderr, "%s\n", server_res);
    //     }
    // }
    // else {
    //     fprintf(stderr, "recv error\n");
    // }
    free(jobj);
}

void delete_room(GtkWidget *widget, gpointer data) {
    mx_printstr("minus\n");
    (void)widget;
    t_user_rooms *temp = ((t_user_rooms*)data);
    int room_id = temp->id;
    gtk_widget_unparent(temp->button_del);
    gtk_widget_unparent(temp->dialog_button);
    printf("func_del_room\n");
    json_object *jobj = json_object_new_object();
    char server_res[1024];
    struct json_object *json_str, *jtype, *jroom_id;

    json_object_object_add(jobj,"type", json_object_new_string("delete_room"));
    json_object_object_add(jobj,"room_id", json_object_new_int(room_id));
    
    const char *obj_text = json_object_to_json_string(jobj);
    printf("%s\n", obj_text);
    int send_res = send(sock, obj_text, strlen(obj_text), 0);
    if (send_res < 0)
        fprintf(stderr, "Send message error\n");
    
    if ((recv(sock, server_res, sizeof(server_res), 0)) > 0) {
        printf("del_room|\n");
        json_str = json_tokener_parse(server_res);
        json_object_object_get_ex(json_str, "type", &jtype);
        json_object_object_get_ex(json_str, "room_id", &jroom_id);
        int res_room_id = json_object_get_int(jroom_id);
        if (jtype && res_room_id > 0 && !strcmp(json_object_get_string(jtype), "room_deleted")) {
            user_rooms = delete_user_room(&user_rooms, room_id); // я бы закинул в ответ сервера room_deleted (на сервере сделано)
            printf("room id: %d deleted\n", res_room_id);
        }
        else {
            fprintf(stderr, "%s\n", server_res);
        }
    }
    else {
        fprintf(stderr, "recv error\n");
    }
    free(jobj);
}

int delete_member(int room_id, char *friend) {
    printf("func_del_member\n");
    json_object *jobj = json_object_new_object();
    char server_res[1024];
    struct json_object *json_str, *jtype, *jroom_id, *jfriend;

    json_object_object_add(jobj,"type", json_object_new_string("delete_member"));
    json_object_object_add(jobj,"room_id", json_object_new_int(room_id));
    json_object_object_add(jobj,"friend", json_object_new_string(friend));
    
    const char *obj_text = json_object_to_json_string(jobj);
    printf("%s\n", obj_text);
    int send_res = send(sock, obj_text, strlen(obj_text), 0);
    if (send_res < 0)
        fprintf(stderr, "Send message error\n");
    
    if ((recv(sock, server_res, sizeof(server_res), 0)) > 0) {
        printf("del_member|\n");
        json_str = json_tokener_parse(server_res);
        json_object_object_get_ex(json_str, "type", &jtype);
        json_object_object_get_ex(json_str, "room_id", &jroom_id);
        json_object_object_get_ex(json_str, "friend", &jfriend);
        int res_room_id = json_object_get_int(jroom_id);
        if (jfriend && jtype && res_room_id > 0 && !strcmp(json_object_get_string(jtype), "member_deleted")) {
            printf("member: %s deleted in room_id: %d\n", json_object_get_string(jfriend), res_room_id);
        }
        else {
            fprintf(stderr, "%s\n", server_res);
        }
    }
    else {
        fprintf(stderr, "recv error\n");
    }
    free(jobj);
    return -1;
}

void get_messages(int room_id, int start, int count) {
    printf("func_get_msg\n");
    json_object *jobj = json_object_new_object();

    json_object_object_add(jobj,"type", json_object_new_string("get_message"));
    json_object_object_add(jobj,"room_id", json_object_new_int(room_id));
    json_object_object_add(jobj,"start", json_object_new_int(start));
    json_object_object_add(jobj,"count", json_object_new_int(count));
    
    const char *obj_text = json_object_to_json_string(jobj);
    printf("%s\n", obj_text);
    int send_res = send(sock, obj_text, strlen(obj_text), 0);
    if (send_res < 0)
        fprintf(stderr, "Send message error\n");

    free(jobj);
    // return -1;
}

bool send_message(int jroom_id, const char *msg) {

    // char server_res[1024];
    json_object *jobj = json_object_new_object();
    // struct json_object *json_str;
    // struct json_object *jerror;
    
    json_object_object_add(jobj,"type", json_object_new_string("send_message"));
    json_object_object_add(jobj,"room_id", json_object_new_int(jroom_id));
    json_object_object_add(jobj,"user_id", json_object_new_int(user_info.user_id));
    json_object_object_add(jobj,"message", json_object_new_string(msg));
    
    const char *obj_text = json_object_to_json_string(jobj);
    int send_res = send(sock, obj_text, strlen(obj_text), 0);
    if (send_res < 0)
        fprintf(stderr, "Send message error\n");
    // if ((recv(sock, server_res, sizeof(server_res), 0)) > 0) {
    //     json_str = json_tokener_parse(server_res);
    //     json_object_object_get_ex(json_str, "error", &jerror);
        
    //     if (json_object_get_string(jerror) == NULL) {
    //         printf("server res: %s\n",server_res);
    //         return false;
    //     }
    //     else {
    //         fprintf(stderr, "%s\n", server_res);
    //         return true;
    //     }
    // }
    // else {
    //     fprintf(stderr, "recv error\n");
    // }
    free(jobj);
    return true;
}

void print_message(char *str, bool taken_msg, char *username) {
    GtkWidget *my_msg_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    
    if (taken_msg) {
        gtk_widget_set_halign (my_msg_box, GTK_ALIGN_START);
    }
    else {
        gtk_widget_set_halign (my_msg_box, GTK_ALIGN_END);
    }
    gtk_widget_set_valign (my_msg_box, GTK_ALIGN_END);
    //gtk_widget_set_size_request(my_msg_box, 10, 10);
    gtk_widget_set_margin_end(my_msg_box, 66);
    gtk_widget_set_margin_bottom(my_msg_box, 55);
    
    GtkWidget *nickname = gtk_label_new(username);
    gtk_widget_set_valign (nickname, GTK_ALIGN_START);
    gtk_widget_set_halign (nickname, GTK_ALIGN_START);
    gtk_widget_set_name(nickname, "nickname_chat");
    load_css(t_application.provider, nickname);
    
    GtkWidget *msg = gtk_label_new(str);
    if (taken_msg) {
        gtk_widget_set_name(msg, "msg_chat_recv");
        //gtk_widget_set_margin_end(nickname, 66);
        gtk_widget_set_margin_start(nickname, 66);
    }
    else {
        gtk_widget_set_name(msg, "msg_chat");
        gtk_widget_set_margin_start(nickname, mx_strlen(str)*30);
    }
    load_css(t_application.provider, msg);
    gtk_label_set_wrap(GTK_LABEL(msg), TRUE);
    gtk_label_set_wrap_mode(GTK_LABEL(msg), PANGO_WRAP_WORD_CHAR);
    gtk_label_set_max_width_chars(GTK_LABEL(msg), 50);

    gtk_box_append (GTK_BOX (my_msg_box), msg);
    gtk_box_append(GTK_BOX(t_message.box_message), my_msg_box);
    
    

    gtk_box_append (GTK_BOX (my_msg_box), nickname);
    
}

void *yes(void *socket) {
    int sock = *((int *)socket);
    char msg[1024];
    struct json_object *json_str;
    struct json_object *jtype;
    struct json_object *jroom_id, *juser_id, *jusername, *jmsg; // message
    struct json_object *jroom_name, *jnew_room_id, *jroom_messages, *jtmp;
    while (1) {
        usleep(100);
        if (recv(sock, msg, sizeof(msg), 0) <= 0)
            break;
        if (user_info.user_id <= 0)
            break;
        json_str = json_tokener_parse(msg);
        json_object_object_get_ex(json_str, "type", &jtype);
        if (json_object_get_string(jtype)) {
            const char *stype = json_object_get_string(jtype);
            
            if (strcmp(stype, "send_message") == 0) {
                        
                json_object_object_get_ex(json_str, "user_id", &juser_id);
                json_object_object_get_ex(json_str, "username", &jusername);
                json_object_object_get_ex(json_str, "room_id", &jroom_id);
                json_object_object_get_ex(json_str, "message", &jmsg);
                int room_id = json_object_get_int(jroom_id);
                printf( "\nres: %s\n", json_object_get_string(json_str));
                if (juser_id && jroom_id && jmsg && jusername) {
                    // add to form message;
                    // printf( "from: %s\troom_id: %d\n", json_object_get_string(jusername), room_id);
                    printf( "message: %s\n", json_object_get_string(jmsg));
                    if (tmp_room_id == room_id)
                        print_message((char*)json_object_get_string(jmsg), true, (char*)json_object_get_string(jusername));
                }
                else {
                    fprintf(stderr, "recv data problem\n");
                }
            }
            else if (strcmp(stype, "room_added") == 0) {
                printf("Potok_add_room|\n");
                json_object_object_get_ex(json_str, "room_name", &jroom_name);
                json_object_object_get_ex(json_str, "new_room_id", &jnew_room_id);
                int new_room_id = json_object_get_int(jnew_room_id);
                if (new_room_id > 0 && jroom_name) {
                    node_push_back(&user_rooms, new_room_id, (char *)json_object_get_string(jroom_name));
                    if (chat_pos) {
                        new_dialog_print();
                    }
                }
            }
            else if (strcmp(stype, "room_deleted") == 0) {
                printf("Potok_del_room|\n");
                json_object_object_get_ex(json_str, "room_id", &jroom_id);
                int res_room_id = json_object_get_int(jroom_id);
                if (res_room_id > 0) {
                    dialog_refresh(res_room_id);
                    user_rooms = delete_user_room(&user_rooms, res_room_id); // я бы закинул в ответ сервера room_deleted (на сервере сделано)
                    printf("room id: %d deleted\n", res_room_id);
                }
            }
            else if (strcmp(stype, "get_message") == 0) {
                json_object_object_get_ex(json_str, "room_id", &jroom_id);
                json_object_object_get_ex(json_str, "room_messages", &jroom_messages);

                if (jroom_id && jroom_messages) {
                    for (int i = (int)json_object_array_length(jroom_messages) - 1; i >= 0 ; i--) {
                        jtmp = json_object_array_get_idx(jroom_messages, i);
                        int res_user_id = json_object_get_int(json_object_object_get(jtmp, "user_id"));
                        char *username = (char*)json_object_get_string(json_object_object_get(jtmp, "username"));  // потом дополнить остальными данными
                        char *message = (char *)json_object_get_string(json_object_object_get(jtmp, "message"));
                        printf("\n[%s]\n",message);
                        // char *message_time = json_object_get_text(json_object_object_get(jtmp, "message_time")); // потом дополнить остальными данными
                        if (res_user_id == user_info.user_id)
                            print_message(message, false, username);
                        else
                            print_message(message, true, username);
                    }
                }
            }
            else {
                fprintf(stderr, "Unknown request! %s\n", msg);
            }
        }
        
    }
    return NULL;
}

void take_sms() {
    pthread_create(&t_id, NULL, yes, (void*)&sock);
    pthread_detach (t_id);
}

