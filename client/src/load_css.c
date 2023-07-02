#include "../inc/header.h"

void load_css(GtkCssProvider *provider, GtkWidget *widget) {
    GtkStyleContext *context = gtk_widget_get_style_context(widget);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER); 
}

