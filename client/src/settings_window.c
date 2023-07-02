#include "../inc/header.h"

void set_theme_light(GtkWidget *widget, gpointer data){
    t_settings.theme = 0;
    gtk_widget_unparent(data);
    gtk_widget_unparent(widget);
    settings_window();
}

void set_theme_neutral(GtkWidget *widget, gpointer data){
    t_settings.theme = 1;
    gtk_widget_unparent(data);
    gtk_widget_unparent(widget);
    settings_window();
}

void set_theme_dark(GtkWidget *widget, gpointer data){
    t_settings.theme = 2;
    gtk_widget_unparent(data);
    gtk_widget_unparent(widget);
    settings_window();
}

void go_to_start_from_settings(GtkWidget *widget, gpointer data) {
    t_settings.profile = true;
    t_settings.themes = false;
    t_settings.language = false;
    t_settings.privacy = false;
    gtk_widget_unparent(data);
    gtk_widget_unparent(widget);
    start_window();
}

void go_to_chat_from_settings(GtkWidget *widget, gpointer data) {
    gtk_widget_unparent(data);
    gtk_widget_unparent(widget);
    chat_window();
}

void choose_profile(GtkWidget *widget, gpointer data){
    t_settings.profile = true;
    t_settings.themes = false;
    t_settings.language = false;
    t_settings.privacy = false;
    gtk_widget_unparent(data);
    gtk_widget_unparent(widget);
    settings_window();
}

void choose_themes(GtkWidget *widget, gpointer data){
    t_settings.profile = false;
    t_settings.themes = true;
    t_settings.language = false;
    t_settings.privacy = false;
    gtk_widget_unparent(data);
    gtk_widget_unparent(widget);
    settings_window();
}

void choose_language(GtkWidget *widget, gpointer data){
    t_settings.profile = false;
    t_settings.themes = false;
    t_settings.language = true;
    t_settings.privacy = false;
    gtk_widget_unparent(data);
    gtk_widget_unparent(widget);
    settings_window();
}

void choose_privacy(GtkWidget *widget, gpointer data){
    t_settings.profile = false;
    t_settings.themes = false;
    t_settings.language = false;
    t_settings.privacy = true;
    gtk_widget_unparent(data);
    gtk_widget_unparent(widget);
    settings_window();
}

void settings_window (){

    GtkWidget *box_blur;
    GtkWidget *box_left_bar;
    GtkWidget *box_right_bar;
    GtkWidget *box_buttons;
    GtkWidget *label_settings;
    GtkWidget *button_profile;
    GtkWidget *button_themes;
    GtkWidget *button_language;
    GtkWidget *button_privacy;
    GtkWidget *button_log_out;
    GtkWidget *button_chat;
    GtkWidget *button_settings;

    css_chooser ();

    t_application.box_main = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_halign(t_application.box_main, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(t_application.box_main, GTK_ALIGN_CENTER);
    gtk_widget_set_size_request (t_application.box_main, 1920, 1080);
    gtk_widget_set_name(t_application.box_main, "box_main_settings");
    load_css(t_application.provider, t_application.box_main);

    box_blur = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_halign(box_blur, GTK_ALIGN_START);
    gtk_widget_set_valign(box_blur, GTK_ALIGN_CENTER);
    gtk_widget_set_size_request (box_blur, 1920, 1080);
    gtk_widget_set_name(box_blur, "box_blur_settings");
    load_css(t_application.provider, box_blur);
    gtk_box_append(GTK_BOX(t_application.box_main), box_blur);

    box_left_bar = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_halign(box_left_bar, GTK_ALIGN_START);
    gtk_widget_set_valign(box_left_bar, GTK_ALIGN_CENTER);
    gtk_widget_set_size_request (box_left_bar, 460, 1080);
    gtk_widget_set_name(box_left_bar, "box_left_bar_settings");
    load_css(t_application.provider, box_left_bar);
    gtk_box_append(GTK_BOX(box_blur), box_left_bar);

    box_right_bar = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_halign(box_right_bar, GTK_ALIGN_END);
    gtk_widget_set_valign(box_right_bar, GTK_ALIGN_CENTER);
    gtk_widget_set_size_request (box_right_bar, 1460, 1080);
    gtk_widget_set_name(box_right_bar, "box_right_bar_settings");
    load_css(t_application.provider, box_right_bar);
    gtk_box_append(GTK_BOX(box_blur), box_right_bar);

    label_settings = gtk_label_new("Settings");
    gtk_widget_set_halign(label_settings, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(label_settings, GTK_ALIGN_START);
    gtk_widget_set_margin_top ((label_settings), 30);
    gtk_widget_set_name(label_settings, "label_settings_settings");
    load_css(t_application.provider, label_settings);
    gtk_box_append(GTK_BOX(box_left_bar), label_settings);

//////////PROFILE///////////////////////////////////////////
    button_profile = gtk_button_new();
    gtk_button_set_label(GTK_BUTTON(button_profile),"Profile");
    gtk_widget_set_halign(button_profile, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(button_profile, GTK_ALIGN_START);
    gtk_widget_set_margin_top ((button_profile), 50);
    gtk_widget_set_size_request (button_profile, 380, 90);
    if(t_settings.profile == true){
        gtk_widget_set_name(button_profile, "button_active_settings");

        GtkWidget *label_comming_soon = gtk_label_new("Coming Soon");
        gtk_widget_set_halign(label_comming_soon, GTK_ALIGN_CENTER);
        gtk_widget_set_valign(label_comming_soon, GTK_ALIGN_CENTER);
        gtk_widget_set_name(label_comming_soon, "label_comming_soon_settings");
        load_css(t_application.provider, label_comming_soon);
        gtk_box_append(GTK_BOX(box_right_bar), label_comming_soon);
    }
    else{
        gtk_widget_set_name(button_profile, "button_passive_settings");
    }
    load_css(t_application.provider, button_profile);
    gtk_box_append(GTK_BOX(box_left_bar), button_profile);

/////////THEMES///////////////////////////////////////////
    button_themes = gtk_button_new();
    gtk_button_set_label(GTK_BUTTON(button_themes),"Themes");
    gtk_widget_set_halign(button_themes, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(button_themes, GTK_ALIGN_START);
    gtk_widget_set_margin_top ((button_themes), 50);
    gtk_widget_set_size_request (button_themes, 380, 90);
    if(t_settings.themes == true){
        gtk_widget_set_name(button_themes, "button_active_settings");

        GtkWidget *box_string_box_theme;
        GtkWidget *button_light_theme;
        GtkWidget *box_light_theme;
        GtkWidget *button_neutral_theme;
        GtkWidget *box_neutral_theme;
        GtkWidget *button_dark_theme;
        GtkWidget *box_dark_theme;

        box_string_box_theme = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
        gtk_widget_set_halign(box_string_box_theme, GTK_ALIGN_CENTER);
        gtk_widget_set_valign(box_string_box_theme, GTK_ALIGN_START);
        gtk_widget_set_margin_top ((box_string_box_theme), 125);
        gtk_widget_set_size_request (box_string_box_theme, 1460, 250);
        gtk_widget_set_name(box_string_box_theme, "box_string_box_theme");
        gtk_box_append(GTK_BOX(box_right_bar), box_string_box_theme);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        button_light_theme = gtk_button_new();
        gtk_widget_set_halign(button_light_theme, GTK_ALIGN_START);
        gtk_widget_set_valign(button_light_theme, GTK_ALIGN_START);
        gtk_widget_set_size_request (button_light_theme, 250, 250);
        gtk_widget_set_margin_start ((button_light_theme), 150);
        gtk_widget_set_name(button_light_theme, "button_light_theme");
        load_css(t_application.provider, button_light_theme);
        gtk_box_append(GTK_BOX(box_string_box_theme), button_light_theme);


        box_light_theme = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
        gtk_widget_set_halign(box_light_theme, GTK_ALIGN_START);
        gtk_widget_set_valign(box_light_theme, GTK_ALIGN_START);
        gtk_widget_set_size_request (box_light_theme, 250, 250);
        gtk_widget_set_name(box_light_theme, "box_light_theme");
        load_css(t_application.provider, box_light_theme);
        gtk_button_set_child (GTK_BUTTON(button_light_theme), box_light_theme);
        


        button_neutral_theme = gtk_button_new();
        gtk_widget_set_halign(button_neutral_theme, GTK_ALIGN_START);
        gtk_widget_set_valign(button_neutral_theme, GTK_ALIGN_CENTER);
        gtk_widget_set_margin_start ((button_neutral_theme), 202);
        gtk_widget_set_size_request (button_neutral_theme, 250, 250);
        gtk_widget_set_name(button_neutral_theme, "button_neutral_theme");
        load_css(t_application.provider, button_neutral_theme);
        gtk_box_append(GTK_BOX(box_string_box_theme), button_neutral_theme);

        box_neutral_theme = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
        gtk_widget_set_halign(box_neutral_theme, GTK_ALIGN_CENTER);
        gtk_widget_set_valign(box_neutral_theme, GTK_ALIGN_START);
        gtk_widget_set_size_request (box_neutral_theme, 250, 250);
        gtk_widget_set_name(box_neutral_theme, "box_neutral_theme");
        load_css(t_application.provider, box_neutral_theme);
        gtk_button_set_child(GTK_BUTTON(button_neutral_theme), box_neutral_theme);        

        button_dark_theme = gtk_button_new();
        gtk_widget_set_halign(button_dark_theme, GTK_ALIGN_START);
        gtk_widget_set_valign(button_dark_theme, GTK_ALIGN_CENTER);
        gtk_widget_set_margin_start ((button_dark_theme), 202);
        gtk_widget_set_size_request (button_dark_theme, 250, 250);
        gtk_widget_set_name(button_dark_theme, "button_dark_theme");
        load_css(t_application.provider, button_dark_theme);
        gtk_box_append(GTK_BOX(box_string_box_theme), button_dark_theme);

        box_dark_theme = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
        gtk_widget_set_halign(box_dark_theme, GTK_ALIGN_CENTER);
        gtk_widget_set_valign(box_dark_theme, GTK_ALIGN_START);
        gtk_widget_set_size_request (box_dark_theme, 250, 250);
        gtk_widget_set_name(box_dark_theme, "box_dark_theme");
        load_css(t_application.provider, box_dark_theme);
        gtk_button_set_child(GTK_BUTTON(button_dark_theme), box_dark_theme);        

        g_signal_connect_swapped (button_light_theme, "clicked", G_CALLBACK (set_theme_light), t_application.box_main);
        g_signal_connect_swapped (button_neutral_theme, "clicked", G_CALLBACK (set_theme_neutral), t_application.box_main);
        g_signal_connect_swapped (button_dark_theme, "clicked", G_CALLBACK (set_theme_dark), t_application.box_main);

    }
    else{
        gtk_widget_set_name(button_themes, "button_passive_settings");
    }
    load_css(t_application.provider, button_themes);
    gtk_box_append(GTK_BOX(box_left_bar), button_themes);


/////////LANGUAGE///////////////////////////////////////////
    button_language = gtk_button_new();
    gtk_button_set_label(GTK_BUTTON(button_language),"Language");
    gtk_widget_set_halign(button_language, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(button_language, GTK_ALIGN_START);
    gtk_widget_set_margin_top ((button_language), 50);
    gtk_widget_set_size_request (button_language, 380, 90);
    if(t_settings.language == true){
        gtk_widget_set_name(button_language, "button_active_settings");

        GtkWidget *label_comming_soon = gtk_label_new("Coming Soon");
        gtk_widget_set_halign(label_comming_soon, GTK_ALIGN_CENTER);
        gtk_widget_set_valign(label_comming_soon, GTK_ALIGN_CENTER);
        gtk_widget_set_name(label_comming_soon, "label_comming_soon_settings");
        load_css(t_application.provider, label_comming_soon);
        gtk_box_append(GTK_BOX(box_right_bar), label_comming_soon);
    }
    else{
        gtk_widget_set_name(button_language, "button_passive_settings");
    }
    load_css(t_application.provider, button_language);
    gtk_box_append(GTK_BOX(box_left_bar), button_language);


/////////////PRIVACY///////////////////////////////////////////
    button_privacy = gtk_button_new();
    gtk_button_set_label(GTK_BUTTON(button_privacy),"Privacy");
    gtk_widget_set_halign(button_privacy, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(button_privacy, GTK_ALIGN_START);
    gtk_widget_set_margin_top ((button_privacy), 50);
    gtk_widget_set_size_request (button_privacy, 380, 90);
    if(t_settings.privacy == true){
        gtk_widget_set_name(button_privacy, "button_active_settings");

        GtkWidget *label_comming_soon = gtk_label_new("Coming Soon");
        gtk_widget_set_halign(label_comming_soon, GTK_ALIGN_CENTER);
        gtk_widget_set_valign(label_comming_soon, GTK_ALIGN_CENTER);
        gtk_widget_set_name(label_comming_soon, "label_comming_soon_settings");
        load_css(t_application.provider, label_comming_soon);
        gtk_box_append(GTK_BOX(box_right_bar), label_comming_soon);
    }
    else{
        gtk_widget_set_name(button_privacy, "button_passive_settings");
    }
    load_css(t_application.provider, button_privacy);
    gtk_box_append(GTK_BOX(box_left_bar), button_privacy);

////////////LOG OUT///////////////////////////////////////////
    button_log_out = gtk_button_new();
    gtk_button_set_label(GTK_BUTTON(button_log_out),"Log out");
    gtk_widget_set_halign(button_log_out, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(button_log_out, GTK_ALIGN_START);
    gtk_widget_set_margin_top ((button_log_out), 50);
    gtk_widget_set_size_request (button_log_out, 380, 90);
    gtk_widget_set_name(button_log_out, "button_passive_settings");
    load_css(t_application.provider, button_log_out);
    gtk_box_append(GTK_BOX(box_left_bar), button_log_out);

    box_buttons = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_halign(box_buttons, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(box_buttons, GTK_ALIGN_END);
    gtk_widget_set_size_request (box_buttons, 460, 95);
    gtk_widget_set_margin_top (box_buttons, 112);
    gtk_widget_set_name(box_buttons, "box_buttos_settings");
    load_css(t_application.provider, box_buttons);
    gtk_box_append(GTK_BOX(box_left_bar), box_buttons);

    button_chat = gtk_button_new();
    gtk_widget_set_halign(button_chat, GTK_ALIGN_END);
    gtk_widget_set_valign(button_chat, GTK_ALIGN_END);
    gtk_widget_set_margin_start ((button_chat), 30);
    gtk_widget_set_size_request (button_chat, 99, 87);
    gtk_widget_set_name(button_chat, "button_chat_settings");
    load_css(t_application.provider, button_chat);
    gtk_box_append(GTK_BOX(box_buttons), button_chat);

    button_settings = gtk_button_new();
    gtk_widget_set_halign(button_settings, GTK_ALIGN_END);
    gtk_widget_set_valign(button_settings, GTK_ALIGN_END);
    gtk_widget_set_margin_start ((button_settings), 198);
    gtk_widget_set_size_request (button_settings, 99, 87);
    gtk_widget_set_name(button_settings, "button_settings_settings");
    load_css(t_application.provider, button_settings);
    gtk_box_append(GTK_BOX(box_buttons), button_settings);
    
    gtk_box_append(GTK_BOX(t_application.box), t_application.box_main);
   
    g_signal_connect_swapped (button_profile, "clicked", G_CALLBACK (choose_profile), t_application.box_main);
    g_signal_connect_swapped (button_themes, "clicked", G_CALLBACK (choose_themes), t_application.box_main);
    g_signal_connect_swapped (button_language, "clicked", G_CALLBACK (choose_language), t_application.box_main);
    g_signal_connect_swapped (button_privacy, "clicked", G_CALLBACK (choose_privacy), t_application.box_main);
    g_signal_connect_swapped (button_chat, "clicked", G_CALLBACK (go_to_chat_from_settings), t_application.box_main);
    g_signal_connect_swapped (button_log_out, "clicked", G_CALLBACK (go_to_start_from_settings), t_application.box_main);

}
