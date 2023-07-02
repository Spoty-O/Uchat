#include "../inc/header.h"

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
   for (int i = 0; i < argc; i++)
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   printf("\n");
   NotUsed = "Zero";
   return 0;
}

int database_create(sqlite3 *db) {

    char *ErrorMsg = NULL;
    char *sql = "CREATE TABLE IF NOT EXISTS users (" \
            "id	INTEGER NOT NULL UNIQUE," \
            "username	VARCHAR(50) NOT NULL UNIQUE," \
            "password	VARCHAR(50) NOT NULL," \
            "data_reg datetime NOT NULL," \
            "PRIMARY KEY(id AUTOINCREMENT)" \
        ");" \
        "CREATE TABLE IF NOT EXISTS room (" \
            "id	INTEGER NOT NULL UNIQUE," \
            "name	VARCHAR(50) NOT NULL," \
            "PRIMARY KEY(id AUTOINCREMENT)" \
        ");" \
        "CREATE TABLE IF NOT EXISTS message (" \
            "id	INTEGER NOT NULL UNIQUE," \
            "room_id	INTEGER NOT NULL," \
            "user_id	INTEGER NOT NULL," \
            "message	TEXT NOT NULL," \
            "message_time datetime NOT NULL," \
            "PRIMARY KEY(id AUTOINCREMENT)" \
        ");" \
        "CREATE TABLE IF NOT EXISTS members (" \
            "id	INTEGER NOT NULL UNIQUE," \
            "room_id	INTEGER NOT NULL," \
            "user_id	INTEGER NOT NULL," \
            "PRIMARY KEY(id AUTOINCREMENT)" \
        ");";
    
    /* create tables */
    int rc = sqlite3_exec(db, sql, callback, 0, &ErrorMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", ErrorMsg);
        sqlite3_free(ErrorMsg);
        return -1;
    }
    return 0;
}

int get_user_id(sqlite3 *db, const char *username) {
    char *sql = "";
    sqlite3_stmt *stmt;
    int err;

    sql = "SELECT id FROM users WHERE username = ?;";
    err = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (err == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, username, -1, 0);
    } 
    else {
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return -1;
    }
    err = sqlite3_step(stmt);
    if (err == SQLITE_ROW) {
        int user_id = sqlite3_column_int(stmt, 0);
        sqlite3_clear_bindings(stmt);
        sqlite3_finalize(stmt);
        return user_id;
    }
    sqlite3_finalize(stmt);
    return -1;
}

char *get_username(sqlite3 *db, int user_id) {
    char *sql = "";
    sqlite3_stmt *stmt;
    int err;

    sql = "SELECT username FROM users WHERE id = ?;";
    err = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (err == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, user_id);
        err = sqlite3_step(stmt);
        if (err == SQLITE_ROW) {
            char *username = strdup((char *)sqlite3_column_text(stmt, 0));
            sqlite3_clear_bindings(stmt);
            sqlite3_finalize(stmt);
            return username;
        }
    }
    fprintf(stderr, "Database error: %s\n", sqlite3_errmsg(db));
    sqlite3_clear_bindings(stmt);
    sqlite3_finalize(stmt);
    return NULL;
}

int add_user_to_room(sqlite3 *db, int room_id, int user_id) {
    char *sql = "INSERT INTO members (room_id, user_id) VALUES (?, ?);";
    sqlite3_stmt *stmt;
    int err = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (err == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, room_id);
        sqlite3_bind_int(stmt, 2, user_id);
    } 
    else {
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return -1;
    }

    err = sqlite3_step(stmt);
    if (err != SQLITE_DONE) {
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
        sqlite3_clear_bindings(stmt);
        sqlite3_finalize(stmt);
        return -1;
    }
    sqlite3_clear_bindings(stmt);
    sqlite3_finalize(stmt);
    return 0;
}

int find_room(sqlite3 *db, const char *name) {
    char *sql;
    sqlite3_stmt *stmt;
    int err;

    sql = "SELECT id, name FROM room WHERE name = ?;";
    err = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (err == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, name, -1, 0);
        err = sqlite3_step(stmt);
        if (err == SQLITE_ROW) {
            if (mx_strcmp(name, (char *)sqlite3_column_text(stmt, 1)) == 0) {
                sqlite3_clear_bindings(stmt);
                sqlite3_finalize(stmt);
                return 1;
            }
            else {
                return 0;
            }
        }
    }
    else {
        fprintf(stderr, "err: %s", sqlite3_errmsg(db));
        sqlite3_clear_bindings(stmt);
        sqlite3_finalize(stmt);
        return -1;
    }
    sqlite3_clear_bindings(stmt);
    sqlite3_finalize(stmt);
    return 0;
}

int add_room(sqlite3 *db, const char *name) {
    sqlite3_stmt *stmt;
    if (find_room(db, name) == 0) {
        char *sql = "INSERT INTO room (name) VALUES (?);";
        int err = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
        if (err == SQLITE_OK) {
            sqlite3_bind_text(stmt, 1, name, -1, 0);
        } 
        else {
            fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
            sqlite3_finalize(stmt);
            return -1;
        }
        err = sqlite3_step(stmt);
        if (err != SQLITE_DONE) {
            fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
            sqlite3_clear_bindings(stmt);
            sqlite3_finalize(stmt);
            return -2;
        }

        sql = "SELECT MAX(id) FROM room;";
        err = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
        if (err != SQLITE_OK) {
            fprintf(stderr, "prepare failed: %s\n", sqlite3_errmsg(db));
            return -1;
        }
        err = sqlite3_step(stmt);
        if (err == SQLITE_ROW) {
            int room_id = sqlite3_column_int(stmt, 0);
            sqlite3_clear_bindings(stmt);
            sqlite3_finalize(stmt);
            return room_id;
        }
        sqlite3_clear_bindings(stmt);
        sqlite3_finalize(stmt);
    }
    return -1;
}

int get_count_rooms_by_usr(sqlite3 *db, int user_id) {
    char *sql = "SELECT COUNT(*) FROM members WHERE user_id = ?;";
    int count_rows = 0;
    sqlite3_stmt *stmt;
    int err;
    
    err = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (err == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, user_id);
    }
    else {
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return -1;
    }
    err = sqlite3_step(stmt);
    if (err == SQLITE_ROW)
        count_rows = sqlite3_column_int(stmt, 0);

    sqlite3_clear_bindings(stmt);
    sqlite3_finalize(stmt);

    if (count_rows <= 0) {
        fprintf(stderr, "User is not a member of the rooms\n");
        return -1;
    }
    return count_rows;
}
int get_rooms_by_usr(sqlite3 *db, int user_id, int *users) {
    char *sql = "SELECT room_id FROM members WHERE user_id = ?;";
    sqlite3_stmt *stmt;
    int err;

    err = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (err == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, user_id);
    } 
    else {
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return -1;
    }
    for (int i = 0; sqlite3_step(stmt) != SQLITE_DONE; i++) {
        users[i] = sqlite3_column_int(stmt, 0);
    }
    sqlite3_finalize(stmt);
    return 0;
}

int get_count_usr_in_room(sqlite3 *db, int room_id) {
    char *sql = "SELECT COUNT(*) FROM members WHERE room_id = ?;";
    int count_rows = 0;
    sqlite3_stmt *stmt;
    int err;
    
    err = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (err == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, room_id);
    }
    else {
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return -1;
    }
    err = sqlite3_step(stmt);
    if (err == SQLITE_ROW)
        count_rows = sqlite3_column_int(stmt, 0);

    sqlite3_clear_bindings(stmt);
    sqlite3_finalize(stmt);

    if (count_rows <= 0) {
        fprintf(stderr, "Room is empty\n");
        return -1;
    }
    return count_rows;
}

int get_users_in_room(sqlite3 *db, int room_id, int *users_in_room) {
    char *sql = "SELECT user_id FROM members WHERE room_id = ?;";
    sqlite3_stmt *stmt;
    int err;

    err = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (err == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, room_id);
    } 
    else {
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return -1;
    }
    for (int i = 0; sqlite3_step(stmt) != SQLITE_DONE; i++) {
        users_in_room[i] = sqlite3_column_int(stmt, 0);
    }
    sqlite3_finalize(stmt);
    return 0;
}

char *get_room_name(sqlite3 *db, int room_id) {
    char *sql = "SELECT name FROM room WHERE id = ?;";
    sqlite3_stmt *stmt;
    int err;
    
    err = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (err == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, room_id);
    } 
    else {
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return NULL;
    }
    err = sqlite3_step(stmt);
    if (err == SQLITE_ROW) {
        char *res = mx_strdup((char *)sqlite3_column_text(stmt, 0));
        sqlite3_finalize(stmt);
        return res;
    }
    sqlite3_clear_bindings(stmt);
    sqlite3_finalize(stmt);
    return NULL;
}

bool check_member_in_room(sqlite3 *db, int room_id, int user_id) {
    char *sql = "SELECT user_id FROM members WHERE room_id = ? AND user_id = ?;";
    sqlite3_stmt *stmt;
    int err;
    err = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (err == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, room_id);
        sqlite3_bind_int(stmt, 2, user_id);
        err = sqlite3_step(stmt);
        if (err == SQLITE_ROW) {
            printf("yest\n");
            sqlite3_clear_bindings(stmt);
            sqlite3_finalize(stmt);
            return true;
        }
    } 
    else {
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return false;
    }
    sqlite3_clear_bindings(stmt);
    sqlite3_finalize(stmt);
    return false;
}

int add_member(sqlite3 *db, int room_id, int friend_id) {
    sqlite3_stmt *stmt;
    char *sql;
    int err;
    if (friend_id <= 0 || room_id <= 0)
        return -2;
    printf("b: %d\n", check_member_in_room(db, room_id, friend_id));
    if (check_member_in_room(db, room_id, friend_id) == true)
        return -2;
    sql = "INSERT INTO members (room_id, user_id) VALUES (?, ?);";
    err = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (err == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, room_id);
        sqlite3_bind_int(stmt, 2, friend_id);
        err = sqlite3_step(stmt);
        if (err == SQLITE_DONE) {
            sqlite3_clear_bindings(stmt);
            sqlite3_finalize(stmt);
            return 1;
        }
    } 
    sqlite3_clear_bindings(stmt);
    sqlite3_finalize(stmt);
    return -1;
}

int add_message(sqlite3 *db, int user_id, int room_id, const char *message) {
    char *sql = "INSERT INTO message (room_id, user_id, message, message_time) VALUES (?, ?, ?, ?);";
    long int ttime = time(NULL);
    sqlite3_stmt *stmt;

    int err = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (err == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, room_id);
        sqlite3_bind_int(stmt, 2, user_id);
        sqlite3_bind_text(stmt, 3, message, -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 4, ctime(&ttime), -1, SQLITE_TRANSIENT);
    } 
    else {
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return -1;
    }

    err = sqlite3_step(stmt);
    if (err != SQLITE_DONE) {
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
        sqlite3_clear_bindings(stmt);
        sqlite3_finalize(stmt);
        return -1;
    }
    sqlite3_clear_bindings(stmt);
    sqlite3_finalize(stmt);
    return 0;
}

int delete_room(sqlite3 *db, int room_id) {
    if (room_id <= 0)
        return -2;
    // char *sql = "DELETE FROM room WHERE id = ?;";
    char *sql[3];
    int err;

    sql[0] = "DELETE FROM room WHERE id = ?;";
    sql[1] = "DELETE FROM members WHERE room_id = ?;";
    sql[2] = "DELETE FROM message WHERE room_id = ?;";

    sqlite3_stmt *stmt;
    for (int i = 0; i < 3; i++) {
        err = sqlite3_prepare_v2(db, sql[i], -1, &stmt, NULL);
        if (err == SQLITE_OK) {
            sqlite3_bind_int(stmt, 1, room_id);
            err = sqlite3_step(stmt);
            sqlite3_clear_bindings(stmt);
            if (err == SQLITE_DONE) {
                continue;
            }
        }
        else {
            break;
        }
    }
    if (err != SQLITE_DONE)
        fprintf(stderr, "Database error: %s\n", sqlite3_errmsg(db));
    sqlite3_finalize(stmt);
    if (err == SQLITE_DONE)
        return 1;
    return -1;
}

int delete_member(sqlite3 *db, int room_id, int member_id) {
    if (member_id <= 0 || room_id <= 0)
        return -2;
    char *sql = "DELETE FROM members WHERE room_id = ? AND user_id = ?;";
    sqlite3_stmt *stmt;
    int err = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (err == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, room_id);
        sqlite3_bind_int(stmt, 2, member_id);
        err = sqlite3_step(stmt);
        sqlite3_clear_bindings(stmt);
        sqlite3_finalize(stmt);
        if (err == SQLITE_DONE) {
            return 1;
        }
    }
    fprintf(stderr, "Database error: %s\n", sqlite3_errmsg(db));
    return -1;
}

