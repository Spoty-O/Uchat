#include "../inc/header.h"

int main (int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "usage: ./uchat [ip] [port]\n");
        return -1;
    }
    char *our_ip = argv[1];
    int PORT = mx_atoi(argv[2]);
    sock = 0;
    struct sockaddr_in serv_addr;
    // char *text = NULL;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    
    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, our_ip, &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
    
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }
    
    //GtkApplication *app;
    int status;

    

    t_settings.profile = true;
    t_settings.themes = false;
    t_settings.language = false;
    t_settings.privacy = false;

    t_application.provider = gtk_css_provider_new();

    t_application.app = gtk_application_new ("Golub.chat", G_APPLICATION_FLAGS_NONE);
    

    g_signal_connect(t_application.app, "activate", G_CALLBACK (window_creation), NULL);
    status = g_application_run(G_APPLICATION(t_application.app), 1, &argv[0]);
    g_object_unref(t_application.app);
    close(sock);
    return status;
}
