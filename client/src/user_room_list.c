#include "../inc/header.h"

t_user_rooms *create_node(int id, char *name) {
    t_user_rooms *node = malloc(sizeof(t_user_rooms));
    node->id = id;
    node->name = name;
    node->created = false;
    node->next = NULL;
    node->prev = NULL;
    return node;
}

void node_push_back(t_user_rooms **list, int id, char *name) {
    if (!*list) {
        *list = create_node(id, name);
        return;
    }
    t_user_rooms *temp = *list;
    while (temp->next) {
        temp = temp->next;
    }
    t_user_rooms *prev_node = temp;
    temp->next = create_node(id, name);
    temp = temp->next;
    temp->prev = prev_node;
}

void clear_list_user_rooms(t_user_rooms **list) {
    if (!*list) return; 
    t_user_rooms *temp;
    while (*list) {
        temp = (*list)->next;
        (*list)->next = NULL;
        free(*list);
        *list = temp;
    }
    list = NULL;
}

t_user_rooms *delete_user_room(t_user_rooms **list, int id) {
    if (!*list) return NULL; 
    t_user_rooms *start = (*list);
    t_user_rooms *temp = NULL;
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

