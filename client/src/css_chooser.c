#include "../inc/header.h"


void css_chooser (){
    switch(t_settings.theme){
        case(0):
            gtk_css_provider_load_from_path(t_application.provider, "client/resources/css/light_theme.css");
            break;
        case(1):
            gtk_css_provider_load_from_path(t_application.provider, "client/resources/css/neutral_theme.css");
            break;
        case(2):
            gtk_css_provider_load_from_path(t_application.provider, "client/resources/css/dark_theme.css");
            break;
        default:
            gtk_css_provider_load_from_path(t_application.provider, "client/resources/css/light_theme.css");
            break;
    }

}
