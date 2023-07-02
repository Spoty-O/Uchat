#include "../inc/header.h"

int string_compare(const unsigned char *s1, const char *s2) {
    for (int i = 0; s1[i] != '\0' || s2[i] != '\0'; i++) {
        if (s1[i] != s2[i])
            return s1[i] - s2[i];
    }
    return 0;
}

int registration(sqlite3 *db, const char *username, const char *password) {
    long int ttime = time(NULL);
    char *sql = "";
    int err;
    sqlite3_stmt *stmt;
    if (get_user_id(db, username) > 0) { // find user
        fprintf(stderr, "user exists\n");
        return 1;
    }
    sql = "INSERT INTO users (username, password, data_reg) VALUES (?, ?, ?);";
    err = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (err != SQLITE_OK) {
        printf("prepare failed: %s\n", sqlite3_errmsg(db));
        return -2;
    }
    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_TRANSIENT); /* username */
    sqlite3_bind_text(stmt, 2, password, -1, SQLITE_TRANSIENT); /* password */
    sqlite3_bind_text(stmt, 3, ctime(&ttime), -1, SQLITE_TRANSIENT); /* data_reg */

    err = sqlite3_step(stmt);
    if (err != SQLITE_DONE) {
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
        sqlite3_clear_bindings(stmt);
        sqlite3_finalize(stmt);
        return -2;
    }
    sqlite3_clear_bindings(stmt);
    sqlite3_finalize(stmt);
    return 0;
}

int authorization(sqlite3 *db, const char *username, const char *password) {
    char *sql = "";
    int err;
    sqlite3_stmt *stmt;
    
    // find user
    sql = "SELECT id, username, password FROM users WHERE username = ?;";
    err = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (err == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, username, -1, 0);
    } else {
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
        sqlite3_clear_bindings(stmt);
        sqlite3_finalize(stmt);
        return -2;
    }
    err = sqlite3_step(stmt);
    if (err == SQLITE_ROW) {
        if (string_compare(sqlite3_column_text(stmt, 1), username) == 0) {
            if (string_compare(sqlite3_column_text(stmt, 2), password) == 0) {
                printf("ok\n");
                // sqlite3_clear_bindings(stmt);
                // sqlite3_finalize(stmt);
                return 0;
            }
        }
    }
    fprintf(stderr, "Username or password is not correct\n");
    sqlite3_finalize(stmt);
    return -1;
}

