#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <math.h>
#include <sys/types.h>
#include <ctype.h>
#include <dirent.h>
#include "../../frameworks/libmx/inc/libmx.h"
#include "../../frameworks/json-c/0.15/include/json-c/json.h"
#include "../../frameworks/openssl@3/3.0.1/include/openssl/ssl.h"
#include "../../frameworks/openssl@3/3.0.1/include/openssl/bio.h"
#include "../../frameworks/openssl@3/3.0.1/include/openssl/err.h"
#include <pthread.h>
#include <errno.h>
#include <malloc/malloc.h>
#include <resolv.h>
#include <netdb.h>
#include <signal.h>
#include <sqlite3.h>
#include <ifaddrs.h>
// #define PORT 8080

#define BUF_SIZE 100
#define MAX_CLNT 256

typedef struct data_client {
    int user_id;
    int user_socket;
    struct data_client *next;
    struct data_client *prev;
}              id_sock;

id_sock *create_node(int user_id, int socket);
void node_push_back_data(id_sock **list, int user_id, int socket);
void clear_list_user_data(id_sock **list);
id_sock *delete_user_data(id_sock **list, int user_id);

typedef struct s_pthread_data {
    int new_socket;
    sqlite3 *db;
    id_sock **client;
}              t_pthread_data;

// database_query
int database_create(sqlite3 *db);
int get_user_id(sqlite3 *db, const char *username);
char *get_username(sqlite3 *db, int user_id);
int get_count_rooms_by_usr(sqlite3 *db, int user_id);
int get_rooms_by_usr(sqlite3 *db, int user_id, int *users);
int get_count_usr_in_room(sqlite3 *db, int room_id);
int get_users_in_room(sqlite3 *db, int room_id, int *rooms);
char *get_room_name(sqlite3 *db, int room_id);
int add_message(sqlite3 *db, int user_id, int room_id, const char *message);
int add_room(sqlite3 *db, const char *name);
int add_user_to_room(sqlite3 *db, int room_id, int user_id);
int add_member(sqlite3 *db, int room_id, int friend_id);
int delete_room(sqlite3 *db, int room_id);
int delete_member(sqlite3 *db, int room_id, int member_id);
int find_room(sqlite3 *db, const char *name);
// auth
int registration(sqlite3 *db, const char *username, const char *password);
int authorization(sqlite3 *db, const char *username, const char *password);

// create_json_obj
void get_user_rooms_obj(sqlite3 *db, json_object *jobj, int user_id);
void error_obj(json_object *jobj, char *error);
void get_user_info_obj(int user_id, const char *username, json_object *jobj);
void get_message(sqlite3 *db, json_object *jobj, int room_id, int start, int count);

void resend_message(t_pthread_data pthread_data, json_object *jobj, int room_id, int user_id);
void resend_to_user_id(t_pthread_data pthread_data, json_object *jobj, int user_id);

#endif

