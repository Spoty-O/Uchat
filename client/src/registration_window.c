#include "../inc/header.h"

int mx_strlen(const char *s) {
    int j = 0;
    char i = s[0];
    while(i != '\0') {
        j++;
        i = s[j];
    }
    return j;
}

void mx_printstr(const char *s) {
    write(1, s, mx_strlen(s));
}

void mx_printchar(char c) {
    write(1, &c, 1);
}

void go_to_start(GtkWidget *widget) {
    gtk_widget_unparent(widget);
    start_window();

}
void go_to_chat_from_registration(GtkWidget *widget) {
    
    t_registration_data.login = gtk_entry_buffer_get_text(gtk_entry_get_buffer(GTK_ENTRY(t_registration_data.entry_login_registration)));
    t_registration_data.password = gtk_entry_buffer_get_text(gtk_entry_get_buffer(GTK_ENTRY(t_registration_data.entry_password_registration)));
    t_registration_data.confirm_password = gtk_entry_buffer_get_text(gtk_entry_get_buffer(GTK_ENTRY(t_registration_data.entry_cofirm_password_registration)));

    if (strcmp(t_registration_data.password, t_registration_data.confirm_password)){
        GtkWidget *label;

        // info_bar = gtk_info_bar_new();
        // gtk_widget_set_size_request (info_bar, 300, 70);
        // gtk_widget_set_halign(box_registration, GTK_ALIGN_CENTER);
        // gtk_widget_set_valign(box_registration, GTK_ALIGN_START);
        // gtk_widget_set_name(info_bar, "info_bar_registration");
        // load_css(t_application.provider, info_bar);
        // gtk_box_append(GTK_BOX(t_application.box_main), info_bar);

        label = gtk_label_new("Passwords don't match");
        gtk_widget_set_halign(label, GTK_ALIGN_CENTER);
        gtk_widget_set_valign(label, GTK_ALIGN_CENTER);
        gtk_widget_set_name(label, "label_registration");
        load_css(t_application.provider, label);
        gtk_info_bar_add_child(GTK_INFO_BAR(t_application.info_bar), label);


        gtk_widget_show (t_application.info_bar);
        //sleep(1);
        //gtk_widget_hide (t_application.info_bar);

        //gtk_widget_unparent(t_application.info_bar);

        
        //mx_printstr("ne sovpadayut paroli(\n");

        return;
    }
    
    user_rooms = auth(true, t_registration_data.login, t_registration_data.password);
    if (user_info.user_id > 0) {
        take_sms();
        gtk_widget_unparent(widget);
        chat_window(user_rooms);
    }
}
// void load_css(GtkCssProvider *provider, GtkWidget *widget){
//     GtkStyleContext *context = gtk_widget_get_style_context(widget);
//     gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER); 
// }

void registration_window() {

    css_chooser ();

    GtkWidget *box_main;
    GtkWidget *box_registration;
    GtkWidget *label_registration;
    GtkWidget *box_buttons_registration;
    GtkWidget *button_back_registration;
    GtkWidget *button_cteate_registration;
    GtkWidget *box_golub_registration;

    box_main = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_halign(box_main, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(box_main, GTK_ALIGN_CENTER);
    gtk_widget_set_size_request (box_main, 1920, 1080);
    gtk_widget_set_name(box_main, "box_main_registration");
    load_css(t_application.provider, box_main);

    t_application.info_bar = gtk_info_bar_new();
    gtk_widget_set_size_request (t_application.info_bar, 300, 70);
    gtk_widget_set_halign(t_application.info_bar, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(t_application.info_bar, GTK_ALIGN_START);
    gtk_widget_set_name(t_application.info_bar, "info_bar_registration");
    load_css(t_application.provider, t_application.info_bar);
    gtk_box_append(GTK_BOX(box_main), t_application.info_bar);    
    gtk_widget_hide (t_application.info_bar);

    box_registration = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_halign(box_registration, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(box_registration, GTK_ALIGN_START);
    gtk_widget_set_size_request (box_registration, 750, 880);
    gtk_widget_set_margin_top (box_registration, 30);
    gtk_widget_set_margin_start (box_registration, 585);
    gtk_widget_set_name(box_registration, "box_registration");
    load_css(t_application.provider, box_registration);
    gtk_box_append(GTK_BOX(box_main), box_registration);

    label_registration = gtk_label_new("Registration");
    gtk_widget_set_halign(label_registration, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(label_registration, GTK_ALIGN_START);
    gtk_widget_set_margin_top (label_registration, 35);
    gtk_widget_set_name(label_registration, "label_registration_registration");
    load_css(t_application.provider, label_registration);
    gtk_box_append(GTK_BOX(box_registration), label_registration);

    t_registration_data.entry_login_registration = gtk_entry_new();
    gtk_widget_set_halign (t_registration_data.entry_login_registration, GTK_ALIGN_CENTER);
    gtk_widget_set_size_request (t_registration_data.entry_login_registration, 482, 74);
    gtk_widget_set_margin_top (t_registration_data.entry_login_registration, 55);
    gtk_entry_set_placeholder_text(GTK_ENTRY(t_registration_data.entry_login_registration),"Username");
    gtk_widget_set_name(t_registration_data.entry_login_registration, "entry_username_registration");
    load_css(t_application.provider, t_registration_data.entry_login_registration);
    gtk_box_append(GTK_BOX(box_registration), t_registration_data.entry_login_registration);

    t_registration_data.entry_password_registration = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(t_registration_data.entry_password_registration), FALSE);
    gtk_widget_set_halign (t_registration_data.entry_password_registration, GTK_ALIGN_CENTER);
    gtk_widget_set_size_request (t_registration_data.entry_password_registration, 482, 74);
    gtk_widget_set_margin_top (t_registration_data.entry_password_registration, 66);
    gtk_entry_set_placeholder_text(GTK_ENTRY(t_registration_data.entry_password_registration),"Password");
    gtk_widget_set_name(t_registration_data.entry_password_registration, "entry_password_registration");
    load_css(t_application.provider, t_registration_data.entry_password_registration);
    gtk_box_append(GTK_BOX(box_registration), t_registration_data.entry_password_registration);

    t_registration_data.entry_cofirm_password_registration = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(t_registration_data.entry_cofirm_password_registration), FALSE);
    gtk_widget_set_halign (t_registration_data.entry_cofirm_password_registration, GTK_ALIGN_CENTER);
    gtk_widget_set_size_request (t_registration_data.entry_cofirm_password_registration, 482, 74);
    gtk_widget_set_margin_top (t_registration_data.entry_cofirm_password_registration, 66);
    gtk_entry_set_placeholder_text(GTK_ENTRY(t_registration_data.entry_cofirm_password_registration),"Confirm password");
    gtk_widget_set_name(t_registration_data.entry_cofirm_password_registration, "entry_cofirm_password_registration");
    load_css(t_application.provider, t_registration_data.entry_cofirm_password_registration);
    gtk_box_append(GTK_BOX(box_registration), t_registration_data.entry_cofirm_password_registration);

    box_buttons_registration = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_halign (box_buttons_registration, GTK_ALIGN_START);
    gtk_widget_set_size_request (box_buttons_registration, 680, 71);
    gtk_widget_set_margin_top (box_buttons_registration, 66);
    gtk_box_append(GTK_BOX(box_registration), box_buttons_registration);

    button_back_registration = gtk_button_new_with_label("Back");
    gtk_widget_set_halign(button_back_registration, GTK_ALIGN_START);
    gtk_widget_set_size_request(button_back_registration, 201, 71);
    gtk_widget_set_margin_start (button_back_registration, 133);
    gtk_widget_set_name(button_back_registration, "button_back_registration");
    load_css(t_application.provider, button_back_registration);
    gtk_box_append(GTK_BOX(box_buttons_registration), button_back_registration);
    // gtk_widget_hide (button_back_registration);
    // sleep(3000);
    // gtk_widget_show (button_back_registration);
    

    button_cteate_registration = gtk_button_new_with_label ("Create");
    gtk_widget_set_halign(button_cteate_registration, GTK_ALIGN_START);
    gtk_widget_set_size_request(button_cteate_registration, 201, 71);
    gtk_widget_set_margin_start (button_cteate_registration, 80);
    gtk_widget_set_name(button_cteate_registration, "button_cteate_registration");
    load_css(t_application.provider, button_cteate_registration);
    gtk_box_append(GTK_BOX(box_buttons_registration), button_cteate_registration);

    box_golub_registration = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_halign(box_golub_registration, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(box_golub_registration, GTK_ALIGN_CENTER);
    gtk_widget_set_size_request (box_golub_registration, 139, 174);
    gtk_widget_set_margin_top (box_golub_registration, 65);
    gtk_widget_set_margin_start (box_golub_registration, 20);
    gtk_widget_set_name(box_golub_registration, "box_golub_registration");
    load_css(t_application.provider, box_golub_registration);
    gtk_box_append(GTK_BOX(box_registration), box_golub_registration);


    gtk_box_append(GTK_BOX(t_application.box), box_main);
    

    g_signal_connect_swapped (button_back_registration, "clicked", G_CALLBACK  (go_to_start), box_main);
    g_signal_connect_swapped (button_cteate_registration, "clicked", G_CALLBACK  (go_to_chat_from_registration), box_main);

    // GtkWidget *button_create;
    // GtkWidget *button_back;
    // GtkWidget *box_registration;
    // GtkWidget *label_login;
    // //GtkWidget *entry_login;
    // //GtkWidget *entry_password_registration;
    // GtkWidget *label_password;
    // GtkWidget *label_registration;

    // //t_application.provider = gtk_css_provider_new();
    // gtk_css_provider_load_from_path(t_application.provider, "client/resources/css/registration_window.css");
    // gtk_widget_set_name(t_application.window, "window_registration");
    // load_css(t_application.provider, t_application.window);

    // box_registration = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    // gtk_widget_set_halign(box_registration, GTK_ALIGN_CENTER);
    // gtk_widget_set_valign(box_registration, GTK_ALIGN_CENTER);
    // gtk_widget_set_name(box_registration, "box_registration");
    // load_css(t_application.provider, box_registration);

    // label_registration = gtk_label_new("REGISTRATION");
    // gtk_widget_set_halign(label_registration, GTK_ALIGN_CENTER);

    // label_login = gtk_label_new("LOGIN");
    // gtk_widget_set_halign(label_login, GTK_ALIGN_START);

    // t_registration_data.entry_login_registration = gtk_entry_new();

    // label_password = gtk_label_new("PASSWORD");
    // gtk_widget_set_halign(label_password, GTK_ALIGN_START);

    // t_registration_data.entry_password_registration = gtk_entry_new();
    // gtk_entry_set_visibility(GTK_ENTRY(t_registration_data.entry_password_registration), FALSE);

    // button_create = gtk_button_new_with_label ("Create");
    // button_back = gtk_button_new_with_label ("Back");

    // gtk_box_append (GTK_BOX (box_registration), label_registration);
    // gtk_box_append (GTK_BOX (box_registration), label_login);
    // gtk_box_append (GTK_BOX (box_registration), t_registration_data.entry_login_registration);
    // gtk_box_append (GTK_BOX (box_registration), label_password);
    // gtk_box_append (GTK_BOX (box_registration), t_registration_data.entry_password_registration);
    
    
    // gtk_box_append (GTK_BOX (box_registration), button_create);
    // gtk_box_append (GTK_BOX (box_registration), button_back);

    // g_signal_connect_swapped (button_back, "clicked", G_CALLBACK  (go_to_start), box_registration);
    // g_signal_connect_swapped (button_create, "clicked", G_CALLBACK  (go_to_chat_from_registration), box_registration);
    
    

    // gtk_box_append(GTK_BOX(t_application.box), box_registration);
    // //gtk_window_set_child (GTK_WINDOW (t_application.window), t_application.box);
    // //gtk_widget_show (t_application.window);
}
