#include "../inc/header.h"

void window_creation() {
    t_application.provider = gtk_css_provider_new();
    css_chooser ();


    t_application.window = gtk_application_window_new (t_application.app);
    gtk_window_set_title(GTK_WINDOW(t_application.window), "Golub");
    gtk_window_set_default_size(GTK_WINDOW(t_application.window), WINDOW_WIDTH, WINDOW_HEIGHT);
    gtk_widget_set_name(t_application.window, "window");
    load_css(t_application.provider, t_application.window);

    t_application.box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_halign(t_application.box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(t_application.box, GTK_ALIGN_CENTER);
    gtk_widget_set_size_request (t_application.box, 1920, 1080);
    gtk_widget_set_name(t_application.box, "box_main");
    load_css(t_application.provider, t_application.box);

    
    gtk_window_set_child(GTK_WINDOW(t_application.window), t_application.box);
    gtk_window_set_resizable(GTK_WINDOW(t_application.window), false);// static size window
    gtk_widget_show(t_application.window);

    start_window();
}
