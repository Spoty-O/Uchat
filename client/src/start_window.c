#include "../inc/header.h"

void go_to_registration(GtkWidget *widget) {
    gtk_widget_unparent(widget);
    registration_window();

}

void go_to_chat_from_start(GtkWidget *widget) {
    t_registration_data.login = gtk_entry_buffer_get_text(gtk_entry_get_buffer(GTK_ENTRY(t_registration_data.entry_login_start)));
    t_registration_data.password = gtk_entry_buffer_get_text(gtk_entry_get_buffer(GTK_ENTRY(t_registration_data.entry_password_start)));
    // t_user_info *user_info = (t_user_info*)malloc(sizeof(t_user_info));
    user_rooms = auth(false, t_registration_data.login, t_registration_data.password);
    if (user_info.user_id > 0) {
        take_sms();
        
        gtk_widget_unparent(widget);
        chat_window(user_rooms);
    }
}

void start_window() {
    pthread_detach (t_id);
    user_info.user_id = -1;
    user_info.username = NULL;
    GtkWidget *box_login;
    GtkWidget *box_buttons;
    GtkWidget *box_labels;
    GtkWidget *button_log_in;
    GtkWidget *button_exit;
    GtkWidget *label_welcome;
    GtkWidget *label_new_here;
    GtkWidget *label_registration;
    GtkGesture *click;
    
    css_chooser ();

    gtk_widget_set_name(t_application.window, "window");  
    load_css(t_application.provider, t_application.window);

    box_login = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_halign (box_login, GTK_ALIGN_END);
    gtk_widget_set_valign (box_login, GTK_ALIGN_START);
    gtk_widget_set_size_request (box_login, 680, 580);
    gtk_widget_set_margin_top (box_login, 270);
    gtk_widget_set_margin_end (box_login, 80);
    gtk_widget_set_name(box_login, "box_login_start");
    load_css(t_application.provider, box_login);

    label_welcome = gtk_label_new("Welcome");
    gtk_widget_set_halign (label_welcome, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_top (label_welcome, 45);
    gtk_widget_set_name(label_welcome, "label_welcome_start");
    load_css(t_application.provider, label_welcome);

    t_registration_data.entry_login_start = gtk_entry_new();
    gtk_widget_set_halign (t_registration_data.entry_login_start, GTK_ALIGN_CENTER);
    gtk_widget_set_size_request (t_registration_data.entry_login_start, 482, 74);
    gtk_widget_set_margin_top (t_registration_data.entry_login_start, 40);
    gtk_entry_set_placeholder_text(GTK_ENTRY(t_registration_data.entry_login_start),"Username");
    gtk_widget_set_name(t_registration_data.entry_login_start, "entry_username_start");
    load_css(t_application.provider, t_registration_data.entry_login_start);

    t_registration_data.entry_password_start = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(t_registration_data.entry_password_start), FALSE);
    gtk_widget_set_halign (t_registration_data.entry_password_start, GTK_ALIGN_CENTER);
    gtk_widget_set_size_request (t_registration_data.entry_password_start, 482, 74);
    gtk_widget_set_margin_top (t_registration_data.entry_password_start, 45);
    gtk_entry_set_placeholder_text(GTK_ENTRY(t_registration_data.entry_password_start),"Password");
    gtk_widget_set_name(t_registration_data.entry_password_start, "entry_password_start");
    load_css(t_application.provider, t_registration_data.entry_password_start);

    box_buttons = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_halign (box_buttons, GTK_ALIGN_START);
    gtk_widget_set_size_request (box_buttons, 680, 71);
    gtk_widget_set_margin_top (box_buttons, 45);
    
    button_exit = gtk_button_new_with_label("Exit");
    gtk_widget_set_halign(button_exit, GTK_ALIGN_START);
    gtk_widget_set_size_request(button_exit, 201, 71);
    gtk_widget_set_margin_start (button_exit, 98);
    gtk_widget_set_name(button_exit, "button_exit_start");
    load_css(t_application.provider, button_exit);
    
    button_log_in = gtk_button_new_with_label ("Login");
    gtk_widget_set_halign(button_log_in, GTK_ALIGN_START);
    gtk_widget_set_size_request(button_log_in, 201, 71);
    gtk_widget_set_margin_start (button_log_in, 80);
    gtk_widget_set_name(button_log_in, "button_log_in_start");
    load_css(t_application.provider, button_log_in);

    box_labels = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_halign (box_labels, GTK_ALIGN_START);
    gtk_widget_set_margin_top (box_labels, 40);

    label_new_here = gtk_label_new("New here? ");
    gtk_widget_set_halign (label_new_here, GTK_ALIGN_START);
    gtk_widget_set_name(label_new_here, "label_new_here_registartion_start");
    load_css(t_application.provider, label_new_here);
    gtk_widget_set_margin_start (label_new_here, 98);

    label_registration = gtk_label_new("Registration");
    gtk_widget_set_halign (label_registration, GTK_ALIGN_START);
    gtk_widget_set_name(label_registration, "label_new_here_registartion_start");
    gtk_widget_set_margin_start (label_registration, 5);
    load_css(t_application.provider, label_registration);
    
    click = gtk_gesture_click_new();
    gtk_gesture_set_state(click, GTK_EVENT_SEQUENCE_CLAIMED);
    gtk_widget_add_controller(label_registration, GTK_EVENT_CONTROLLER(click));

    g_signal_connect_swapped(click, "pressed", G_CALLBACK(go_to_registration), box_login);
    g_signal_connect_swapped(button_log_in, "clicked", G_CALLBACK  (go_to_chat_from_start), box_login); 
    g_signal_connect_swapped(button_exit, "clicked", G_CALLBACK (gtk_window_destroy), t_application.window);

    gtk_box_append(GTK_BOX(t_application.box), box_login);
    gtk_box_append(GTK_BOX (box_login), label_welcome);
    gtk_box_append(GTK_BOX (box_login), t_registration_data.entry_login_start);
    gtk_box_append(GTK_BOX (box_login), t_registration_data.entry_password_start);
    gtk_box_append(GTK_BOX (box_login), box_buttons);
    gtk_box_append(GTK_BOX (box_buttons), button_exit);
    gtk_box_append(GTK_BOX (box_buttons), button_log_in);
    gtk_box_append(GTK_BOX (box_login), box_labels);
    gtk_box_append(GTK_BOX (box_labels), label_new_here);
    gtk_box_append(GTK_BOX (box_labels), label_registration);












///////////////////////////////////////////////////////////////////////////////////////////////////////
    // GtkWidget *button_log_in;
    // GtkWidget *button_exit;
    // GtkWidget *box_login;
    // GtkWidget *box_buttons;

    // gtk_widget_set_name(t_application.window, "window");
    // gtk_css_provider_load_from_path(t_application.provider, "client/resources/css/start_window.css");
    // load_css(t_application.provider, t_application.window);
    
    // box_login = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
    // gtk_widget_set_halign (box_login, GTK_ALIGN_CENTER);
    // gtk_widget_set_valign (box_login, GTK_ALIGN_CENTER);
    // gtk_widget_set_size_request (box_login, 600, 50);
    // gtk_widget_set_margin_top (box_login, 200);
    // gtk_widget_set_name(box_login, "box_login");
    // load_css(t_application.provider, box_login);

    // box_buttons = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
    // gtk_widget_set_halign (box_buttons, GTK_ALIGN_START);
    // gtk_widget_set_valign (box_buttons, GTK_ALIGN_START);
    // gtk_widget_set_size_request (box_login, 600, 600);
    // gtk_widget_set_margin_top (box_buttons, 200);
    // gtk_widget_set_name(box_buttons, "box_buttons");
    // load_css(t_application.provider, box_buttons);

    // t_registration_data.entry_login_start = gtk_entry_new();
    // gtk_widget_set_halign (t_registration_data.entry_login_start, GTK_ALIGN_START);
    // gtk_widget_set_valign (t_registration_data.entry_login_start, GTK_ALIGN_START);
    // gtk_widget_set_size_request (t_registration_data.entry_login_start, 500, 75);
    // gtk_widget_set_margin_start (t_registration_data.entry_login_start, 50);
    // gtk_widget_set_margin_top (t_registration_data.entry_login_start, 80);
    // gtk_entry_set_placeholder_text(GTK_ENTRY(t_registration_data.entry_login_start),"Username");
    // gtk_widget_set_name(t_registration_data.entry_login_start, "entry_login");
    // load_css(t_application.provider, t_registration_data.entry_login_start);

    // t_registration_data.entry_password_start = gtk_entry_new();
    // gtk_entry_set_visibility(GTK_ENTRY(t_registration_data.entry_password_start), FALSE);
    // gtk_widget_set_halign (t_registration_data.entry_password_start, GTK_ALIGN_START);
    // gtk_widget_set_valign (t_registration_data.entry_password_start, GTK_ALIGN_START);
    // gtk_widget_set_size_request (t_registration_data.entry_password_start, 500, 75);
    // gtk_widget_set_margin_start (t_registration_data.entry_password_start, 50);
    // gtk_widget_set_margin_top (t_registration_data.entry_password_start, 50);
    // gtk_entry_set_placeholder_text(GTK_ENTRY(t_registration_data.entry_password_start),"Password");
    // gtk_widget_set_name(t_registration_data.entry_password_start, "entry_password");
    // load_css(t_application.provider, t_registration_data.entry_password_start);
    
    // // label_create_account = gtk_label_new("Create account");
    // // gtk_widget_set_halign(label_create_account, GTK_ALIGN_START);
    // // gtk_gesture_set_state(click_create, GTK_EVENT_SEQUENCE_CLAIMED);
    // // g_signal_connect_swapped(click_create, "pressed", G_CALLBACK(go_to_registration), box_login);
    // // gtk_widget_add_controller(label_create_account, GTK_EVENT_CONTROLLER(click_create));

    // button_log_in = gtk_button_new_with_label ("Login");
    // gtk_widget_set_halign(button_log_in, GTK_ALIGN_START);
    // gtk_widget_set_size_request(button_log_in, 180, 55);
    // gtk_widget_set_margin_start (button_log_in, 50);
    // gtk_widget_set_name(button_log_in, "button_log_in");
    // load_css(t_application.provider, button_log_in);


    // button_exit = gtk_button_new_with_label("Register");
    // gtk_widget_set_halign(button_exit, GTK_ALIGN_END);
    // gtk_widget_set_size_request(button_exit, 180, 55);
    // gtk_widget_set_margin_start (button_exit, 135);
    // gtk_widget_set_name(button_exit, "button_exit");
    // load_css(t_application.provider, button_exit);

    // g_signal_connect_swapped(button_log_in, "clicked", G_CALLBACK  (go_to_chat_from_start), box_login); 
    // g_signal_connect_swapped(button_exit, "clicked", G_CALLBACK (go_to_registration), box_login);

    // gtk_box_append(GTK_BOX (box_login), t_registration_data.entry_login_start);
    // gtk_box_append(GTK_BOX (box_login), t_registration_data.entry_password_start);
    // gtk_box_append(GTK_BOX (box_login), box_buttons);
    // gtk_box_append(GTK_BOX (box_buttons), button_log_in);
    // gtk_box_append(GTK_BOX (box_buttons), button_exit);
    // gtk_box_append(GTK_BOX(t_application.box), box_login);

}
