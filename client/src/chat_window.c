#include "../inc/header.h"



char *mx_strncpy(char *dst, const char *src, int len) {
	for (int i = 0; i < len; i++) {
		dst[i] = src[i];
	}
	
	return dst;
}

char *mx_strnew(const int size) {
	if (size < 0) {
		return NULL;
	}

	char *str = (char *)malloc((size + 1) * sizeof(char));

	if (str == NULL) {
		return NULL;
	}

	for (int i = 0; i <= size; i++) {
		str[i] = '\0';
	}

	return str;
}

void dialog_activating(GtkWidget *widget, gpointer data){
    t_user_rooms *temp = user_rooms;
    message_window();
    while(temp){
        gtk_widget_set_name(temp->dialog_button, "passive_dialog_chat");
        load_css(t_application.provider, temp->dialog_button);
        temp = temp->next;
    }
    gtk_widget_set_name(widget, "active_dialog_chat");
    load_css(t_application.provider, widget);
    gtk_widget_show(t_message.text_field);
    tmp_room_id = ((t_user_rooms*)data)->id;
    get_messages(tmp_room_id, 0, 7);
}


void go_to_start_from_chat(GtkWidget *widget, gpointer data) {
    (void)widget;
    printf("%s\n", ((t_user_rooms*)data)->name);
    //gtk_widget_unparent(t_message.box_message);
    gtk_widget_unparent(t_application.box_main);
    start_window();
}

void go_to_settings_from_chat(GtkWidget *widget) {
    chat_pos = false;
    (void)widget;
    //gtk_widget_unparent(t_message.box_message);
    gtk_widget_unparent(t_application.box_main);
    settings_window();
}

void room_adding(GtkWidget *widget){
    (void)widget;
    t_message.search = gtk_entry_buffer_get_text(gtk_entry_get_buffer(GTK_ENTRY(t_message.entry_search)));

    if (t_message.search[0] == '\n' || t_message.search[0] == '\0'){
        t_message.search = "room";
        add_room(t_message.search, NULL);
        
    }
    
    gtk_widget_unparent(t_application.box_main);
    chat_window();
}

void dialog_refresh(int id) {
    printf("yes");
    t_user_rooms *temp = user_rooms;
    while(temp) {
        if (id == temp->id) {
            gtk_widget_unparent(temp->button_del);
            gtk_widget_unparent(temp->dialog_button);
        }
        temp = temp->next;
    }
}

void new_dialog_print() {
    t_user_rooms *temp = user_rooms;
    while(temp->next) {
        temp = temp->next;
    }
    temp->dialog_button = gtk_button_new();
    
    gtk_button_set_label(GTK_BUTTON(temp->dialog_button), temp->name);
    gtk_widget_set_halign(temp->dialog_button, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(temp->dialog_button, GTK_ALIGN_START);
    gtk_widget_set_size_request (temp->dialog_button, 378, 87);
    gtk_widget_set_margin_top (temp->dialog_button, 44);
    gtk_widget_set_name(temp->dialog_button, "passive_dialog_chat");
    load_css(t_application.provider, temp->dialog_button);
    gtk_box_append(GTK_BOX(t_message.box_dialog), temp->dialog_button);
    printf ("%d\n", temp->id);
    temp->button_del = gtk_button_new();
    gtk_widget_set_halign(temp->button_del, GTK_ALIGN_START);
    gtk_widget_set_valign(temp->button_del, GTK_ALIGN_START);
    gtk_widget_set_size_request (temp->button_del, 50, 50);
    gtk_widget_set_name(temp->button_del, "button_del_chat");
    load_css(t_application.provider, temp->button_del);
    gtk_box_append(GTK_BOX(t_message.box_dialog), temp->button_del);
    // gtk_widget_set_name(temp->button_del, "button_del_chat");
    // load_css(t_application.provider, temp->button_del);
    

    g_signal_connect(temp->button_del, "clicked", G_CALLBACK (delete_room), temp);
    g_signal_connect(temp->dialog_button, "clicked", G_CALLBACK (dialog_activating), temp);
}

void dialog_loading() {
    t_user_rooms *temp = user_rooms;
    while(temp){
        
        temp->dialog_button = gtk_button_new();
        
        gtk_button_set_label(GTK_BUTTON(temp->dialog_button), temp->name);
        gtk_widget_set_halign(temp->dialog_button, GTK_ALIGN_CENTER);
        gtk_widget_set_valign(temp->dialog_button, GTK_ALIGN_START);
        gtk_widget_set_size_request (temp->dialog_button, 378, 87);
        gtk_widget_set_margin_top (temp->dialog_button, 44);
        gtk_widget_set_name(temp->dialog_button, "passive_dialog_chat");
        load_css(t_application.provider, temp->dialog_button);
        gtk_box_append(GTK_BOX(t_message.box_dialog), temp->dialog_button);
        printf ("%d\n", temp->id);
        temp->button_del = gtk_button_new();
        gtk_widget_set_halign(temp->button_del, GTK_ALIGN_START);
        gtk_widget_set_valign(temp->button_del, GTK_ALIGN_START);
        gtk_widget_set_size_request (temp->button_del, 50, 50);
        gtk_widget_set_name(temp->button_del, "button_del_chat");
        load_css(t_application.provider, temp->button_del);
        gtk_box_append(GTK_BOX(t_message.box_dialog), temp->button_del);

        g_signal_connect(temp->button_del, "clicked", G_CALLBACK (delete_room), temp);
        g_signal_connect(temp->dialog_button, "clicked", G_CALLBACK (dialog_activating), temp);

        temp = temp->next;
    }
}
static void message_creation(GtkEventControllerKey *controller, guint keyval, guint keycode, GdkModifierType state, gpointer user_data){
    (void)controller;
    (void)keycode;
    (void)state;
    (void)user_data;

    if(keyval == GDK_KEY_Return){
        GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(t_message.text_field));
        GtkTextIter start, end;
        gtk_text_buffer_get_start_iter(buffer, &start);
        gtk_text_buffer_get_end_iter(buffer, &end);

        const char *buf_str = gtk_text_buffer_get_text(buffer, &start, &end, true);

        if (!buf_str || buf_str[0] == '\n' || buf_str[0] == '\0'){
            gtk_text_buffer_set_text (buffer, "", 0);
            return;
        }
        char *str = mx_strnew(1024);
        str = mx_strncpy(str, buf_str, mx_strlen(buf_str) - 1);
        print_message(str, false, (char*)user_info.username);

        send_message(tmp_room_id, str);
        gtk_text_buffer_set_text (buffer, "", 0);
    }
}

void message_window() {
    gtk_widget_unparent(t_message.box_message);

    t_message.box_message = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_valign(GTK_WIDGET(t_message.box_message), GTK_ALIGN_END);
    gtk_widget_set_size_request (t_message.box_message, 1460, 0);
    gtk_widget_set_name(GTK_WIDGET(t_message.box_message), "t_message.box_message");
    load_css(t_application.provider, t_message.box_message);

    gtk_scrolled_window_set_child (GTK_SCROLLED_WINDOW(t_message.scroll_window), t_message.box_message);
}

void chat_window() {
    chat_pos = true;
    tmp_room_id = 0;
    gtk_widget_unparent(t_application.box_main);
    printf("user_id: %d\tuser: %s\n", user_info.user_id, user_info.username);

    GtkWidget *box_right_bar;
    GtkWidget *box_buttons;
    GtkWidget *button_chat;
    GtkWidget *button_add_room;
    GtkWidget *button_settings;
    //GtkWidget *entry_search;
    // GtkWidget *scroll_window_chat;
    GtkWidget *scroll_window_dialogs;
    GtkEventController *return_controller;
    GtkWidget *scroll_text_chat;
    GtkWidget *box_text_field;

    css_chooser ();

    t_application.box_main = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_halign(t_application.box_main, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(t_application.box_main, GTK_ALIGN_CENTER);
    gtk_widget_set_size_request (t_application.box_main, 1920, 1080);
    gtk_widget_set_name(t_application.box_main, "box_main_chat");
    load_css(t_application.provider, t_application.box_main);

    t_application.box_blur = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_halign(t_application.box_blur, GTK_ALIGN_START);
    gtk_widget_set_valign(t_application.box_blur, GTK_ALIGN_CENTER);
    gtk_widget_set_size_request (t_application.box_blur, 1920, 1080);
    gtk_widget_set_name(t_application.box_blur, "box_blur_chat");
    load_css(t_application.provider, t_application.box_blur);
    gtk_box_append(GTK_BOX(t_application.box_main), t_application.box_blur);

    t_application.box_left_bar = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_halign(t_application.box_left_bar, GTK_ALIGN_START);
    gtk_widget_set_valign(t_application.box_left_bar, GTK_ALIGN_CENTER);
    gtk_widget_set_size_request (t_application.box_left_bar, 460, 1080);
    gtk_widget_set_name(t_application.box_left_bar, "box_left_bar_chat");
    load_css(t_application.provider, t_application.box_left_bar);
    gtk_box_append(GTK_BOX(t_application.box_blur), t_application.box_left_bar);

    box_right_bar = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_halign(box_right_bar, GTK_ALIGN_END);
    gtk_widget_set_valign(box_right_bar, GTK_ALIGN_CENTER);
    gtk_widget_set_size_request (box_right_bar, 1460, 1080);
    gtk_widget_set_name(box_right_bar, "box_right_bar_chat");
    load_css(t_application.provider, box_right_bar);
    gtk_box_append(GTK_BOX(t_application.box_blur), box_right_bar);

    t_message.box_message = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_valign(GTK_WIDGET(t_message.box_message), GTK_ALIGN_END);
    gtk_widget_set_size_request (t_message.box_message, 1460, 0);
    gtk_widget_set_name(GTK_WIDGET(t_message.box_message), "t_message.box_message");
    load_css(t_application.provider, t_message.box_message);

    t_message.scroll_window = gtk_scrolled_window_new ();
    gtk_widget_set_halign(t_message.scroll_window, GTK_ALIGN_START);
    gtk_widget_set_valign(t_message.scroll_window, GTK_ALIGN_START);
    gtk_widget_set_size_request (t_message.scroll_window, 1460, 928);
    gtk_widget_set_name(t_message.scroll_window, "scroll_window_chat");
    load_css(t_application.provider, t_message.scroll_window);
    gtk_scrolled_window_set_child (GTK_SCROLLED_WINDOW(t_message.scroll_window), t_message.box_message);
    gtk_box_append(GTK_BOX(box_right_bar), t_message.scroll_window);



    box_text_field = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_valign(GTK_WIDGET(box_text_field), GTK_ALIGN_START);
    gtk_widget_set_size_request (box_text_field, 1335, 88);
    gtk_widget_set_name(GTK_WIDGET(box_text_field), "t_message.box_message");
    //load_css(t_application.provider, t_message.box_message);
    

    scroll_text_chat = gtk_scrolled_window_new ();
    gtk_widget_set_halign(scroll_text_chat, GTK_ALIGN_START);
    gtk_widget_set_valign(scroll_text_chat, GTK_ALIGN_START);
    gtk_widget_set_size_request (scroll_text_chat, 1335, 88);
    gtk_widget_set_margin_start(scroll_text_chat, 63);
    gtk_widget_set_margin_top(scroll_text_chat, 40);
    //gtk_widget_set_name(scroll_text_chat, "scroll_window_chat");
    //load_css(t_application.provider, scroll_window_chat);
    gtk_scrolled_window_set_child (GTK_SCROLLED_WINDOW(scroll_text_chat), box_text_field);
    gtk_box_append(GTK_BOX(box_right_bar), scroll_text_chat);


    
    t_message.text_field = gtk_text_view_new();
    gtk_widget_set_halign (t_message.text_field, GTK_ALIGN_START);
    gtk_widget_set_valign (t_message.text_field, GTK_ALIGN_END);
    //gtk_widget_set_margin_start(t_message.text_field, 63);
    //gtk_widget_set_margin_top(t_message.text_field, 40);
    gtk_text_view_set_top_margin (GTK_TEXT_VIEW(t_message.text_field), 13);
    gtk_text_view_set_left_margin (GTK_TEXT_VIEW(t_message.text_field), 63);
    gtk_widget_set_size_request (t_message.text_field, 1335, 88);
    gtk_widget_set_name(t_message.text_field, "text_field_chat");
    load_css(t_application.provider, t_message.text_field);
    gtk_box_append(GTK_BOX(box_text_field), t_message.text_field);
    gtk_widget_hide(t_message.text_field);
    

    return_controller = gtk_event_controller_key_new();
    g_signal_connect_after(return_controller, "key-released", G_CALLBACK(message_creation), NULL);
    gtk_widget_add_controller(t_message.text_field,  GTK_EVENT_CONTROLLER(return_controller));

    t_message.entry_search = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(t_message.entry_search)," Add user");
    gtk_widget_set_halign (t_message.entry_search, GTK_ALIGN_START);
    gtk_widget_set_valign (t_message.entry_search, GTK_ALIGN_START);
    gtk_widget_set_size_request (t_message.entry_search, 407, 66);
    gtk_widget_set_margin_top(t_message.entry_search, 27);
    gtk_widget_set_margin_start(t_message.entry_search, 26);
    gtk_widget_set_name(t_message.entry_search, "entry_search_chat");
    load_css(t_application.provider, t_message.entry_search);
    gtk_box_append(GTK_BOX(t_application.box_left_bar), t_message.entry_search);

    t_message.box_dialog = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_halign(t_message.box_dialog, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(GTK_WIDGET(t_message.box_dialog), GTK_ALIGN_START);
    gtk_widget_set_size_request (t_message.box_dialog, 381, 800);
    gtk_widget_set_name(GTK_WIDGET(t_message.box_dialog), "box_dialog_chat");
    load_css(t_application.provider, t_message.box_dialog);

    scroll_window_dialogs = gtk_scrolled_window_new ();
    gtk_widget_set_halign(scroll_window_dialogs, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(scroll_window_dialogs, GTK_ALIGN_START);
    gtk_widget_set_margin_top(scroll_window_dialogs, 44);
    gtk_widget_set_size_request (scroll_window_dialogs, 381, 800);
    gtk_widget_set_name(scroll_window_dialogs, "scroll_window_chat");
    load_css(t_application.provider, scroll_window_dialogs);
    gtk_scrolled_window_set_child (GTK_SCROLLED_WINDOW(scroll_window_dialogs), t_message.box_dialog);
    gtk_box_append(GTK_BOX(t_application.box_left_bar), scroll_window_dialogs);

    dialog_loading();
    // gtk_widget_set_name(temp->button_del, "button_del_chat");
    // load_css(t_application.provider, temp->button_del);

    box_buttons = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_halign(box_buttons, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(box_buttons, GTK_ALIGN_END);
    gtk_widget_set_size_request (box_buttons, 460, 95);
    gtk_widget_set_name(box_buttons, "box_buttos_chat");
    load_css(t_application.provider, box_buttons);
    gtk_box_append(GTK_BOX(t_application.box_left_bar), box_buttons);

    button_chat = gtk_button_new();
    gtk_widget_set_halign(button_chat, GTK_ALIGN_END);
    gtk_widget_set_valign(button_chat, GTK_ALIGN_END);
    gtk_widget_set_margin_start ((button_chat), 30);
    gtk_widget_set_size_request (button_chat, 99, 87);
    gtk_widget_set_name(button_chat, "button_chat_chat");
    load_css(t_application.provider, button_chat);
    gtk_box_append(GTK_BOX(box_buttons), button_chat);

    button_add_room = gtk_button_new();
    gtk_widget_set_halign(button_add_room, GTK_ALIGN_END);
    gtk_widget_set_valign(button_add_room, GTK_ALIGN_END);
    gtk_widget_set_margin_start ((button_add_room), 60);
    gtk_widget_set_size_request (button_add_room, 99, 87);
    gtk_widget_set_name(button_add_room, "add_user_chat");
    load_css(t_application.provider, button_add_room);
    gtk_box_append(GTK_BOX(box_buttons), button_add_room);

    button_settings = gtk_button_new();
    gtk_widget_set_halign(button_settings, GTK_ALIGN_END);
    gtk_widget_set_valign(button_settings, GTK_ALIGN_END);
    gtk_widget_set_margin_start ((button_settings), 40);
    gtk_widget_set_size_request (button_settings, 99, 87);
    gtk_widget_set_name(button_settings, "button_settings_chat");
    load_css(t_application.provider, button_settings);
    gtk_box_append(GTK_BOX(box_buttons), button_settings);

    gtk_box_append(GTK_BOX(t_application.box), t_application.box_main);

    add_r_buf.button_chat = button_chat;
    g_signal_connect(button_settings, "clicked", G_CALLBACK (go_to_settings_from_chat), t_application.box_main);
    g_signal_connect(button_add_room, "clicked", G_CALLBACK (add_room_window), scroll_window_dialogs);
    g_signal_connect_after(return_controller, "key-released", G_CALLBACK(message_creation), NULL);
}
