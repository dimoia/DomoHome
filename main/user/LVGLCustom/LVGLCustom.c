#include "LVGLCustom.h"
#include "config.h"
#include "lvgl_port.h"
#include "ui.h"

///////////////////////////////////////////////////////////////////////
// Message Box At Sturtup When Configuration File Is Not Found
///////////////////////////////////////////////////////////////////////
static void event_handler_config(lv_event_t * e) 
{
    lv_obj_t * mbox = lv_event_get_current_target(e);
    const char * txt = lv_msgbox_get_active_btn_text(mbox);

    if(strcmp(txt, "Config") == 0) {
        // Logica per il pulsante Config
        loadScreen(SCREEN_ID_SETTINGS_SCREEN );
    } 
    else if(strcmp(txt, "Cancella") == 0) {
        // Logica per il pulsante Cancella
        loadScreen(SCREEN_ID_TEST);
    }

    lv_msgbox_close(mbox);
}

void MsgConfigBox(char *strMsg) 
{
    static const char * btns[] = {"Config", ""}; // L'ultimo elemento deve essere stringa vuota

      /* 1. Crea un "velo" (overlay) che copre tutto lo schermo */
    lv_obj_t * obj_bg = lv_obj_create(lv_scr_act());
    lv_obj_set_size(obj_bg, LV_PCT(100), LV_PCT(100)); // Copre tutto
    lv_obj_set_style_bg_color(obj_bg, lv_color_black(), 0);
    lv_obj_set_style_bg_opa(obj_bg, LV_OPA_60, 0);    // Trasparenza al 60%
    lv_obj_set_style_border_width(obj_bg, 0, 0);      // Rimuove bordi del velo
    lv_obj_set_style_radius(obj_bg, 0, 0);            // Angoli retti

    lv_obj_t * mboxConfig = lv_msgbox_create(obj_bg, LV_SYMBOL_WARNING "Warning", strMsg, btns, false);
    
    
    // 2. AGGIUNTA DELL'OMBRA
    lv_obj_set_style_shadow_width(mboxConfig, 20, 0);          // Ampiezza dell'ombra
    lv_obj_set_style_shadow_color(mboxConfig, lv_palette_main(LV_PALETTE_GREY), 0); // Colore ombra
    lv_obj_set_style_shadow_ofs_y(mboxConfig, 5, 0);         // Spostamento verso il basso
    lv_obj_set_style_shadow_spread(mboxConfig, 2, 0);         // Diffusione dell'ombra

    // Opzionale: colora il titolo di giallo/arancio
    lv_obj_t * title = lv_msgbox_get_title(mboxConfig);
    lv_obj_set_style_text_color(title, lv_palette_main(LV_PALETTE_AMBER), 0);

    // Aggiunge un evento per gestire la pressione dei pulsanti
    lv_obj_add_event_cb(mboxConfig, event_handler_config, LV_EVENT_VALUE_CHANGED, NULL);
    
    lv_obj_center(mboxConfig);

    lv_obj_t * btnm = lv_msgbox_get_btns(mboxConfig);
    
    // Impostiamo una larghezza generosa alla matrice per occupare tutta la msgbox
    lv_obj_set_width(btnm, lv_pct(100));
    lv_obj_set_style_pad_column(btnm, 40, 0); 

    //lv_obj_set_style_align(btnm, LV_ALIGN_CENTER, 0);
    lv_obj_set_style_text_color(btnm, lv_palette_main(LV_PALETTE_ORANGE), LV_PART_ITEMS);    
}
///////////////////////////////////////////////////////////////////////
/////////  Message Box Factory Reset
///////////////////////////////////////////////////////////////////////

static void event_factory_reset_cb(lv_event_t * e)
{
    lv_obj_t * obj = lv_event_get_current_target(e);
    LV_LOG_USER("Button %s clicked", lv_msgbox_get_active_btn_text(obj));
}

void lv_msgbox_factory_reset(void)
{
    static const char * btns[] = { "Reset", "Cancel" ""};

    lv_obj_t * mbox1 = lv_msgbox_create(NULL, "DomoHome", "DomoHome Reset To Factory", btns, true);
    lv_obj_add_event_cb(mbox1, event_factory_reset_cb, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_center(mbox1);
}

///////////////////////////////////////////////////////////////////////
// Message Box At Sturtup When Configuration File Is Not Found
///////////////////////////////////////////////////////////////////////
static void event_handler_info(lv_event_t * e) 
{
    lv_obj_t * mbox = lv_event_get_current_target(e);
    const char * txt = lv_msgbox_get_active_btn_text(mbox);

    if(strcmp(txt, "OK") == 0) 
    {
        // Logica per il pulsante Config
        esp_restart();
        //loadScreen(SCREEN_ID_SETTINGS_SCREEN );
    } 

    lv_msgbox_close(mbox);
}

void MsgInfoBox(char *strMsg) 
{
    static const char * btns[] = {"OK", ""}; // L'ultimo elemento deve essere stringa vuota

      /* 1. Crea un "velo" (overlay) che copre tutto lo schermo */
    lv_obj_t * obj_bg = lv_obj_create(lv_scr_act());
    lv_obj_set_size(obj_bg, LV_PCT(100), LV_PCT(100)); // Copre tutto
    lv_obj_set_style_bg_color(obj_bg, lv_color_black(), 0);
    lv_obj_set_style_bg_opa(obj_bg, LV_OPA_60, 0);    // Trasparenza al 60%
    lv_obj_set_style_border_width(obj_bg, 0, 0);      // Rimuove bordi del velo
    lv_obj_set_style_radius(obj_bg, 0, 0);            // Angoli retti

    lv_obj_t * mboxConfig = lv_msgbox_create(obj_bg, LV_SYMBOL_WARNING "Warning", strMsg, btns, false);
    
    
    // 2. AGGIUNTA DELL'OMBRA
    lv_obj_set_style_shadow_width(mboxConfig, 20, 0);          // Ampiezza dell'ombra
    lv_obj_set_style_shadow_color(mboxConfig, lv_palette_main(LV_PALETTE_GREY), 0); // Colore ombra
    lv_obj_set_style_shadow_ofs_y(mboxConfig, 5, 0);         // Spostamento verso il basso
    lv_obj_set_style_shadow_spread(mboxConfig, 2, 0);         // Diffusione dell'ombra

    // Opzionale: colora il titolo di giallo/arancio
    lv_obj_t * title = lv_msgbox_get_title(mboxConfig);
    lv_obj_set_style_text_color(title, lv_palette_main(LV_PALETTE_AMBER), 0);

    // Aggiunge un evento per gestire la pressione dei pulsanti
    lv_obj_add_event_cb(mboxConfig, event_handler_info, LV_EVENT_VALUE_CHANGED, NULL);
    
    lv_obj_center(mboxConfig);

    lv_obj_t * btnm = lv_msgbox_get_btns(mboxConfig);
    
    // Impostiamo una larghezza generosa alla matrice per occupare tutta la msgbox
    lv_obj_set_width(btnm, lv_pct(100));
    lv_obj_set_style_pad_column(btnm, 40, 0); 

    //lv_obj_set_style_align(btnm, LV_ALIGN_CENTER, 0);
    lv_obj_set_style_text_color(btnm, lv_palette_main(LV_PALETTE_ORANGE), LV_PART_ITEMS);    
}
///////////////////////////////////////////////////////////////////////
/////////  Set For Heating Screen
///////////////////////////////////////////////////////////////////////

void action_goto_heating_screen(lv_event_t *e) 
{
    // TODO: Implement action goto_heating_screen here
    lv_event_code_t code = lv_event_get_code(e);
   // lv_obj_t * obj = lv_event_get_current_target(e);

    if(code == LV_EVENT_CLICKED) {
        LV_LOG_USER("Clicked");
        loadScreen(SCREEN_ID_HEATING_SCREEN);
        //lv_scr_load(obj);       
    }
}

void action_goto_settings_screen(lv_event_t *e) {
    // TODO: Implement action goto_settings_screen here
    lv_event_code_t code = lv_event_get_code(e);
      if(code == LV_EVENT_CLICKED) 
      {
        LV_LOG_USER("Clicked");
        loadScreen(SCREEN_ID_SETTINGS_SCREEN );               
      }
}
void heating_screen(void)
{
    lvgl_port_lock(-1);
    objects_t objs_goto_heating_screen = objects;
    lv_obj_t *btn_goto_heating_screen  = objs_goto_heating_screen.btn_heating_screen;
    lv_obj_add_event_cb(btn_goto_heating_screen, action_goto_heating_screen, LV_EVENT_CLICKED, NULL); 
        
    //objects_t objs_goto_settings_screen = objects;
    lv_obj_t *btn_goto_settings_screen  = objs_goto_heating_screen.btn_settings_screen;
    lv_obj_add_event_cb(btn_goto_settings_screen, action_goto_settings_screen, LV_EVENT_CLICKED, NULL);
    lvgl_port_unlock();
}


void action_goto_home(lv_event_t *e) 
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) 
    {        
        LV_LOG_USER("Clicked");
        if(getConfigStatus() == CONFIG) 
        {
            loadScreen(SCREEN_ID_MAIN );
        }               
    }
}

 void goto_home_screen(void)
 {
    lvgl_port_lock(-1);
    objects_t objs_btn_goto_home = objects;
    lv_obj_t *btn_goto_home_from_heater_screen = objs_btn_goto_home.btn_goto_home_from_heater_screen;         
    lv_obj_t *btn_goto_home_from_config_screen = objs_btn_goto_home.btn_goto_home_from_config_screen;
    lv_obj_add_event_cb(btn_goto_home_from_heater_screen, action_goto_home, LV_EVENT_CLICKED, NULL);  
    lv_obj_add_event_cb(btn_goto_home_from_config_screen, action_goto_home, LV_EVENT_CLICKED, NULL); 
    lvgl_port_unlock();
 }
