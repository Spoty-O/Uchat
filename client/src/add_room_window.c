#include "../inc/header.h"


void buffer_adding(){
    add_r_buf.user = (char*)gtk_entry_buffer_get_text(gtk_entry_get_buffer(GTK_ENTRY(add_r_buf.entry_user)));
    add_r_buf.room = (char*)gtk_entry_buffer_get_text(gtk_entry_get_buffer(GTK_ENTRY(add_r_buf.entry_room)));
    t_user_rooms *temp = user_rooms;
    if (!add_r_buf.user || add_r_buf.user[0] == '\n' || add_r_buf.user[0] == '\0'){
        add_r_buf.user =  NULL;
    }
    while(temp) {
        if(strcmp(add_r_buf.room, temp->name) == 0) {
            add_member(temp->id, add_r_buf.user);
            return;
        }
        temp = temp->next;
    }
    add_room(add_r_buf.room, add_r_buf.user);
}


void add_room_window(GtkWidget *widget, gpointer data) {
    chat_pos = false;
    (void)widget;
    gtk_widget_hide(t_message.entry_search);
    css_chooser ();
    

    t_message.box_dialog = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_halign(t_message.box_dialog, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(GTK_WIDGET(t_message.box_dialog), GTK_ALIGN_START);
    gtk_widget_set_size_request (t_message.box_dialog, 381, 893);
    gtk_widget_set_name(GTK_WIDGET(t_message.box_dialog), "box_dialog_chat");
    load_css(t_application.provider, t_message.box_dialog);
    gtk_scrolled_window_set_child (GTK_SCROLLED_WINDOW(data), t_message.box_dialog);

    gtk_widget_set_name(add_r_buf.button_chat, "button_chat_settings");
    load_css(t_application.provider, add_r_buf.button_chat);

    gtk_widget_set_size_request (GTK_WIDGET(data), 381, 893);

    add_r_buf.entry_user = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(add_r_buf.entry_user)," Add user");
    gtk_widget_set_halign (add_r_buf.entry_user, GTK_ALIGN_START);
    gtk_widget_set_valign (add_r_buf.entry_user, GTK_ALIGN_START);
    gtk_widget_set_size_request (add_r_buf.entry_user, 320, 66);
    gtk_widget_set_margin_top(add_r_buf.entry_user, 40);
    gtk_widget_set_margin_start(add_r_buf.entry_user, 26);
    //gtk_text_view_set_left_margin (GTK_TEXT_VIEW(entry_search), 10);
    gtk_widget_set_name(add_r_buf.entry_user, "entry_search_chat");
    load_css(t_application.provider, add_r_buf.entry_user);
    gtk_box_append(GTK_BOX(t_message.box_dialog), add_r_buf.entry_user);



    add_r_buf.entry_room = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(add_r_buf.entry_room)," Add room");
    gtk_widget_set_halign (add_r_buf.entry_room, GTK_ALIGN_START);
    gtk_widget_set_valign (add_r_buf.entry_room, GTK_ALIGN_START);
    gtk_widget_set_size_request (add_r_buf.entry_room, 320, 66);
    gtk_widget_set_margin_top(add_r_buf.entry_room, 40);
    gtk_widget_set_margin_start(add_r_buf.entry_room, 26);
    //gtk_text_view_set_left_margin (GTK_TEXT_VIEW(entry_search), 10);
    gtk_widget_set_name(add_r_buf.entry_room, "entry_search_chat");
    load_css(t_application.provider, add_r_buf.entry_room);
    gtk_box_append(GTK_BOX(t_message.box_dialog), add_r_buf.entry_room);




    add_r_buf.button_add = gtk_button_new();
    gtk_widget_set_halign(add_r_buf.button_add, GTK_ALIGN_START);
    gtk_widget_set_valign(add_r_buf.button_add, GTK_ALIGN_START);
    gtk_widget_set_margin_start ((add_r_buf.button_add), 150);
    gtk_widget_set_margin_top(add_r_buf.button_add, 40);
    gtk_widget_set_size_request (add_r_buf.button_add, 99, 87);
    gtk_widget_set_name(add_r_buf.button_add, "add_user_chat");
    load_css(t_application.provider, add_r_buf.button_add);
    gtk_box_append(GTK_BOX(t_message.box_dialog), add_r_buf.button_add);


    g_signal_connect_swapped (add_r_buf.button_chat, "clicked", G_CALLBACK (go_to_chat_from_settings), t_application.box_main);

    g_signal_connect_swapped (add_r_buf.button_add, "clicked", G_CALLBACK (buffer_adding), t_application.box_main);



    return;

}

