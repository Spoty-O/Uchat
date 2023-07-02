#ifndef UCHAT_H
#define UCHAT_H

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
#include <gtk/gtk.h>
#include "../../frameworks/json-c/0.15/include/json-c/json.h"
#include "../../frameworks/libmx/inc/libmx.h"
#include <pthread.h>
#include <errno.h>
#include <malloc/malloc.h>
#include <resolv.h>
#include <netdb.h>

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

pthread_t t_id;
int sock;
int tmp_room_id;

bool chat_pos;
char active_room;

struct {
    GtkWidget *box_blur;
    GtkWidget *box_left_bar;
    GtkWidget *window;
    GtkWidget *box;
    GtkWidget *box_main;
    GtkApplication *app;
    GtkCssProvider *provider;
    GtkWidget *info_bar;
}   t_application;

struct {
    GtkWidget *entry_login_start;
    GtkWidget *entry_password_start;
    GtkWidget *entry_login_registration;
    GtkWidget *entry_password_registration;
    GtkWidget *entry_cofirm_password_registration;
    const char *login;
    const char *password;
    const char *confirm_password;
    
}   t_registration_data;

struct {
    GtkWidget *text_field;
    GtkWidget *box_message;
    GtkWidget *box_dialog;
    GtkWidget *entry_search;
    GtkWidget *scroll_window;
    const char *search;
    GtkWidget *box_dialog_manager;
}   t_message;

typedef struct s_user_rooms {
    int id;
    char *name;
    GtkWidget *dialog_button;
    GtkWidget *button_del;
    bool created;
    struct s_user_rooms *next;
    struct s_user_rooms *prev;
}              t_user_rooms;

struct {
    int user_id;
    const char *username;
}   user_info;

struct
{
    int theme;
    bool profile;
    bool themes;
    bool language;
    bool privacy;
}   t_settings;

struct {
    GtkWidget *button_chat;
    GtkWidget *entry_user;
    GtkWidget *entry_room;
    GtkWidget *button_add;
    char *user;
    char *room;
}   add_r_buf;

t_user_rooms *user_rooms;

// user_room list
t_user_rooms *create_node(int id, char *name);
void node_push_back(t_user_rooms **list, int id, char *name);
void clear_list_user_rooms(t_user_rooms **list);
t_user_rooms *delete_user_room(t_user_rooms **list, int id);

void css_chooser ();
void settings_window ();
void window_creation ();
void start_window ();
void chat_window ();
void registration_window ();
void load_css(GtkCssProvider *provider, GtkWidget *widget);
t_user_rooms *auth(bool is_reg, const char *user, const char *pass);
bool send_message(int jroom_id, const char *msg);
int add_room(const char *room_name, char *friend);
void take_sms();
void delete_room(GtkWidget *widget, gpointer data);
int delete_member(int room_id, char *friend);
void print_message(char *str, bool taken_msg, char *username);
void get_messages(int room_id, int start, int count);
void add_member(int room_id, char *friend);
void add_room_window(GtkWidget *widget, gpointer data);
void go_to_settings_from_chat(GtkWidget *widget);
void go_to_chat_from_settings(GtkWidget *widget, gpointer data);
void dialog_loading();
void new_dialog_print();
void dialog_refresh(int id);
void message_window();

#endif

