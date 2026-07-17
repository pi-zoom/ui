#ifndef EEZ_LVGL_UI_SCREENS_H
#define EEZ_LVGL_UI_SCREENS_H

#include <lvgl/lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

// Screens

enum ScreensEnum {
    _SCREEN_ID_FIRST = 1,
    SCREEN_ID_MAIN = 1,
    _SCREEN_ID_LAST = 1
};

typedef struct _objects_t {
    lv_obj_t *main;
    lv_obj_t *obj0;
    lv_obj_t *sequencer;
    lv_obj_t *sequencer_control_panel;
    lv_obj_t *sequencer_midi_files_list;
    lv_obj_t *sequencer_label;
    lv_obj_t *sequencer_slider_bpm;
    lv_obj_t *sequencer_bpm_label;
    lv_obj_t *sequencer_bpm_value;
    lv_obj_t *sequencer_slider_volume;
    lv_obj_t *sequencer_volume_label;
    lv_obj_t *sequencer_volume_value;
    lv_obj_t *sequencer_display_panel;
    lv_obj_t *sequencer_bar;
    lv_obj_t *looper;
    lv_obj_t *loop_control;
    lv_obj_t *loop_add_button;
    lv_obj_t *loop_remove_button;
    lv_obj_t *loop_list;
    lv_obj_t *effects;
    lv_obj_t *mod_control;
    lv_obj_t *banks_label;
    lv_obj_t *pedalboards;
    lv_obj_t *pedalboards_label;
    lv_obj_t *snapshots_label;
    lv_obj_t *banks;
    lv_obj_t *snapshots;
    lv_obj_t *pedalboard_container;
} objects_t;

extern objects_t objects;

void create_screen_main();
void tick_screen_main();

void create_user_widget_loop(lv_obj_t *parent_obj, int startWidgetIndex);
void tick_user_widget_loop(int startWidgetIndex);

void tick_screen_by_id(enum ScreensEnum screenId);
void tick_screen(int screen_index);

void create_screens();

#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_SCREENS_H*/