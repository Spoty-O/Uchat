#include "../inc/header.h"

void error_obj(json_object *jobj, char *error) {
    json_object_object_add(jobj, "error", json_object_new_string(error));
}

void get_user_info_obj(int user_id, const char *username, json_object *jobj) {
    json_object_object_add(jobj,"user_id", json_object_new_int(user_id));
    json_object_object_add(jobj,"username", json_object_new_string(username));
}

void get_user_rooms_obj(sqlite3 *db, json_object *jobj, int user_id) {
    json_object *jarray = json_object_new_array();
    json_object *jroom;

    int count_rooms = get_count_rooms_by_usr(db, user_id);
    if (count_rooms >= 0) {
        int users[count_rooms];
        get_rooms_by_usr(db, user_id, users);
        for (int i = 0; i < count_rooms; i++) {
            printf("%d  ", users[i]);
            char *room_name = get_room_name(db, users[i]);
            if (room_name) {
                jroom = json_object_new_object();
                json_object_object_add(jroom,"id", json_object_new_int(users[i]));
                json_object_object_add(jroom,"name", json_object_new_string(room_name));
                json_object_array_add(jarray, jroom);
            }
        }
        json_object_object_add(jobj,"user_rooms", jarray);
    }
}


void get_message(sqlite3 *db, json_object *jobj, int room_id, int start, int count) {
    if (room_id <= 0 && count < 0)
        return;
        
    json_object *jarray = json_object_new_array();
    json_object *jmessage;
    char *sql = "SELECT user_id, message, message_time FROM message WHERE room_id = ? ORDER BY id DESC LIMIT ?, ?;";
    sqlite3_stmt *stmt;
    int err;
    int user_id;
    char *message, *message_time, *username;
    err = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (err == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, room_id);
        sqlite3_bind_int(stmt, 2, start);
        sqlite3_bind_int(stmt, 3, count);
        while (sqlite3_step(stmt) != SQLITE_DONE) {
            user_id = sqlite3_column_int(stmt, 0);
            message = (char *)sqlite3_column_text(stmt, 1);
            message_time = (char *)sqlite3_column_text(stmt, 2);
            username = get_username(db, user_id);
            jmessage = json_object_new_object();
            json_object_object_add(jmessage,"user_id", json_object_new_int(user_id));
            json_object_object_add(jmessage,"username", json_object_new_string(username));
            json_object_object_add(jmessage,"message", json_object_new_string(message));
            json_object_object_add(jmessage,"message_time", json_object_new_string(message_time));
            json_object_array_add(jarray, jmessage);
            mx_strdel(&username);
        }
        json_object_object_add(jobj,"room_messages", jarray);
    }
    else
        fprintf(stderr, "Database error: %s\n", sqlite3_errmsg(db));
    sqlite3_clear_bindings(stmt);
    sqlite3_finalize(stmt);
}
