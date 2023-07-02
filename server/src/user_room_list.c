#include "../inc/header.h"

id_sock *create_node(int user_id, int socket) {
    id_sock *node = malloc(sizeof(id_sock));
    node->user_id= user_id;
    node->user_socket = socket;
    node->next = NULL;
    node->prev = NULL;
    return node;
}

void node_push_back_data(id_sock **list, int user_id, int socket) {
    // printf("%s ", entry->d_name);
    if (!*list) {
        *list = create_node(user_id, socket);
        return;
    }
    id_sock *temp = *list;
    while (temp->next) {
        temp = temp->next;
    }
    id_sock *prev_node = temp;
    temp->next = create_node(user_id, socket);
    temp = temp->next;
    temp->prev = prev_node;
}

void clear_list_user_data(id_sock **list) {
    if (!*list) return; 
    id_sock *temp;
    while (*list) {
        temp = (*list)->next;
        (*list)->next = NULL;
        free(*list);
        *list = temp;
    }
    list = NULL;
}

id_sock *delete_user_data(id_sock **list, int socket) {
    if (!*list) return NULL; 
    id_sock *start = (*list);
    id_sock *temp = NULL;
    while (*list) {
        if ((*list)->user_socket == socket) {
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

