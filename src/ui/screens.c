#include <string.h>

#include "screens.h"
#include "images.h"
#include "fonts.h"
#include "actions.h"
#include "vars.h"
#include "styles.h"
#include "ui.h"

#include <string.h>

objects_t objects;

//
// Event handlers
//

lv_obj_t *tick_value_change_obj;

//
// Screens
//

void create_screen_main() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.main = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 1024, 600);
    {
        lv_obj_t *parent_obj = obj;
        {
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.obj0 = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 1024, 52);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x6f86d5), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 100, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // tabview
            lv_obj_t *obj = lv_tabview_create(parent_obj);
            objects.tabview = obj;
            lv_obj_set_pos(obj, 0, 52);
            lv_obj_set_size(obj, 1024, 548);
            lv_tabview_set_tab_bar_position(obj, LV_DIR_LEFT);
            lv_tabview_set_tab_bar_size(obj, 60);
            lv_tabview_set_active(obj, 3, LV_ANIM_OFF);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // sequencer
                    lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "Seq");
                    objects.sequencer = obj;
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // sequencer_control_panel
                            lv_obj_t *obj = lv_obj_create(parent_obj);
                            objects.sequencer_control_panel = obj;
                            lv_obj_set_pos(obj, 2, 3);
                            lv_obj_set_size(obj, 919, 93);
                            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_border_color(obj, lv_color_hex(0x4b66c4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_border_opa(obj, 100, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xafabd6), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_opa(obj, 100, LV_PART_MAIN | LV_STATE_DEFAULT);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    // sequencer_midi_files_list
                                    lv_obj_t *obj = lv_dropdown_create(parent_obj);
                                    objects.sequencer_midi_files_list = obj;
                                    lv_obj_set_pos(obj, 10, 36);
                                    lv_obj_set_size(obj, 259, LV_SIZE_CONTENT);
                                    lv_dropdown_set_options_static(obj, "");
                                    lv_dropdown_set_selected(obj, 0);
                                }
                                {
                                    // sequencer_label
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.sequencer_label = obj;
                                    lv_obj_set_pos(obj, 10, 12);
                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                    lv_label_set_text_static(obj, "MIDI Files");
                                }
                                {
                                    // sequencer_slider_bpm
                                    lv_obj_t *obj = lv_slider_create(parent_obj);
                                    objects.sequencer_slider_bpm = obj;
                                    lv_obj_set_pos(obj, 327, 46);
                                    lv_obj_set_size(obj, 244, 11);
                                    lv_slider_set_range(obj, 20, 150);
                                    lv_slider_set_value(obj, 120, LV_ANIM_OFF);
                                }
                                {
                                    // sequencer_bpm_label
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.sequencer_bpm_label = obj;
                                    lv_obj_set_pos(obj, 327, 12);
                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                    lv_label_set_text_static(obj, "BPM");
                                }
                                {
                                    // sequencer_bpm_value
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.sequencer_bpm_value = obj;
                                    lv_obj_set_pos(obj, 435, 12);
                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                    lv_label_set_text_static(obj, "120");
                                }
                                {
                                    // sequencer_slider_volume
                                    lv_obj_t *obj = lv_slider_create(parent_obj);
                                    objects.sequencer_slider_volume = obj;
                                    lv_obj_set_pos(obj, 646, 47);
                                    lv_obj_set_size(obj, 224, 11);
                                    lv_slider_set_value(obj, 50, LV_ANIM_OFF);
                                }
                                {
                                    // sequencer_volume_label
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.sequencer_volume_label = obj;
                                    lv_obj_set_pos(obj, 646, 12);
                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                    lv_label_set_text_static(obj, "Volume");
                                }
                                {
                                    // sequencer_volume_value
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.sequencer_volume_value = obj;
                                    lv_obj_set_pos(obj, 758, 12);
                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                    lv_label_set_text_static(obj, "50");
                                }
                            }
                        }
                        {
                            // sequencer_display_panel
                            lv_obj_t *obj = lv_obj_create(parent_obj);
                            objects.sequencer_display_panel = obj;
                            lv_obj_set_pos(obj, 1, 116);
                            lv_obj_set_size(obj, 920, 391);
                            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_border_color(obj, lv_color_hex(0x4b66c4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_border_opa(obj, 100, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xafabd6), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_opa(obj, 100, LV_PART_MAIN | LV_STATE_DEFAULT);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    // sequencer_bar
                                    lv_obj_t *obj = lv_bar_create(parent_obj);
                                    objects.sequencer_bar = obj;
                                    lv_obj_set_pos(obj, 33, 37);
                                    lv_obj_set_size(obj, 838, 16);
                                    lv_bar_set_range(obj, 0, 1000);
                                }
                            }
                        }
                    }
                }
                {
                    // looper
                    lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "Looper");
                    objects.looper = obj;
                    lv_obj_set_style_border_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // loop_control
                            lv_obj_t *obj = lv_obj_create(parent_obj);
                            objects.loop_control = obj;
                            lv_obj_set_pos(obj, 1, 0);
                            lv_obj_set_size(obj, 923, 84);
                            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            {
                                static lv_coord_t dsc[] = {0, LV_GRID_TEMPLATE_LAST};
                                lv_obj_set_style_grid_row_dsc_array(obj, dsc, LV_PART_MAIN | LV_STATE_DEFAULT);
                            }
                            {
                                static lv_coord_t dsc[] = {0, LV_GRID_TEMPLATE_LAST};
                                lv_obj_set_style_grid_column_dsc_array(obj, dsc, LV_PART_MAIN | LV_STATE_DEFAULT);
                            }
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x9a9fc4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_opa(obj, 100, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_border_color(obj, lv_color_hex(0x3148bb), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_border_opa(obj, 100, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    // loop_add_button
                                    lv_obj_t *obj = lv_button_create(parent_obj);
                                    objects.loop_add_button = obj;
                                    lv_obj_set_pos(obj, 250, 15);
                                    lv_obj_set_size(obj, 100, 50);
                                    lv_obj_add_event_cb(obj, action_add_loop, LV_EVENT_CLICKED, (void *)0);
                                    {
                                        lv_obj_t *parent_obj = obj;
                                        {
                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                            lv_obj_set_pos(obj, 0, 0);
                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_label_set_text_static(obj, "Add");
                                        }
                                    }
                                }
                                {
                                    // loop_remove_button
                                    lv_obj_t *obj = lv_button_create(parent_obj);
                                    objects.loop_remove_button = obj;
                                    lv_obj_set_pos(obj, 509, 15);
                                    lv_obj_set_size(obj, 100, 50);
                                    lv_obj_add_event_cb(obj, action_del_loop, LV_EVENT_CLICKED, (void *)0);
                                    {
                                        lv_obj_t *parent_obj = obj;
                                        {
                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                            lv_obj_set_pos(obj, 0, 0);
                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_label_set_text_static(obj, "Del");
                                        }
                                    }
                                }
                            }
                        }
                        {
                            // loop_list
                            lv_obj_t *obj = lv_obj_create(parent_obj);
                            objects.loop_list = obj;
                            lv_obj_set_pos(obj, 1, 97);
                            lv_obj_set_size(obj, 920, 410);
                            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_border_color(obj, lv_color_hex(0x4b66c4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_border_opa(obj, 100, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xafabd6), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_opa(obj, 100, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_layout(obj, LV_LAYOUT_FLEX, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_flow(obj, LV_FLEX_FLOW_ROW_WRAP, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                    }
                }
                {
                    // effects
                    lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "Effects");
                    objects.effects = obj;
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // mod_control
                            lv_obj_t *obj = lv_obj_create(parent_obj);
                            objects.mod_control = obj;
                            lv_obj_set_pos(obj, 1, 0);
                            lv_obj_set_size(obj, 923, 107);
                            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            {
                                static lv_coord_t dsc[] = {0, LV_GRID_TEMPLATE_LAST};
                                lv_obj_set_style_grid_row_dsc_array(obj, dsc, LV_PART_MAIN | LV_STATE_DEFAULT);
                            }
                            {
                                static lv_coord_t dsc[] = {0, LV_GRID_TEMPLATE_LAST};
                                lv_obj_set_style_grid_column_dsc_array(obj, dsc, LV_PART_MAIN | LV_STATE_DEFAULT);
                            }
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x9a9fc4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_opa(obj, 100, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_border_color(obj, lv_color_hex(0x3148bb), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_border_opa(obj, 100, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    // banks_label
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.banks_label = obj;
                                    lv_obj_set_pos(obj, 9, 19);
                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                    lv_label_set_text_static(obj, "Banks");
                                }
                                {
                                    // pedalboards
                                    lv_obj_t *obj = lv_dropdown_create(parent_obj);
                                    objects.pedalboards = obj;
                                    lv_obj_set_pos(obj, 224, 52);
                                    lv_obj_set_size(obj, 367, LV_SIZE_CONTENT);
                                    lv_dropdown_set_options_static(obj, "");
                                    lv_dropdown_set_selected(obj, 0);
                                }
                                {
                                    // pedalboards_label
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.pedalboards_label = obj;
                                    lv_obj_set_pos(obj, 224, 19);
                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                    lv_label_set_text_static(obj, "Pedalboards");
                                }
                                {
                                    // snapshots_label
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.snapshots_label = obj;
                                    lv_obj_set_pos(obj, 609, 19);
                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                    lv_label_set_text_static(obj, "Snapshots");
                                }
                                {
                                    // banks
                                    lv_obj_t *obj = lv_dropdown_create(parent_obj);
                                    objects.banks = obj;
                                    lv_obj_set_pos(obj, 9, 52);
                                    lv_obj_set_size(obj, 197, LV_SIZE_CONTENT);
                                    lv_dropdown_set_options_static(obj, "");
                                    lv_dropdown_set_selected(obj, 0);
                                }
                                {
                                    // snapshots
                                    lv_obj_t *obj = lv_dropdown_create(parent_obj);
                                    objects.snapshots = obj;
                                    lv_obj_set_pos(obj, 609, 52);
                                    lv_obj_set_size(obj, 298, LV_SIZE_CONTENT);
                                    lv_dropdown_set_options_static(obj, "");
                                    lv_dropdown_set_selected(obj, 0);
                                }
                            }
                        }
                        {
                            // pedalboard_container
                            lv_obj_t *obj = lv_obj_create(parent_obj);
                            objects.pedalboard_container = obj;
                            lv_obj_set_pos(obj, 2, 121);
                            lv_obj_set_size(obj, 920, 386);
                            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_border_color(obj, lv_color_hex(0x4b66c4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_border_opa(obj, 100, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xafabd6), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_opa(obj, 100, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_flow(obj, LV_FLEX_FLOW_ROW_WRAP, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_layout(obj, LV_LAYOUT_FLEX, LV_PART_MAIN | LV_STATE_DEFAULT);
                            {
                                static lv_coord_t dsc[] = {0, LV_GRID_TEMPLATE_LAST};
                                lv_obj_set_style_grid_row_dsc_array(obj, dsc, LV_PART_MAIN | LV_STATE_DEFAULT);
                            }
                            {
                                static lv_coord_t dsc[] = {0, LV_GRID_TEMPLATE_LAST};
                                lv_obj_set_style_grid_column_dsc_array(obj, dsc, LV_PART_MAIN | LV_STATE_DEFAULT);
                            }
                            lv_obj_set_style_flex_main_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_track_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                    }
                }
                {
                    lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "Tab");
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // tuner_panel
                            lv_obj_t *obj = lv_obj_create(parent_obj);
                            objects.tuner_panel = obj;
                            lv_obj_set_pos(obj, 1, 1);
                            lv_obj_set_size(obj, 920, 506);
                            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_border_color(obj, lv_color_hex(0x4b66c4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_border_opa(obj, 100, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xafabd6), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_opa(obj, 100, LV_PART_MAIN | LV_STATE_DEFAULT);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    // tuner_note_label
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.tuner_note_label = obj;
                                    lv_obj_set_pos(obj, 414, 104);
                                    lv_obj_set_size(obj, LV_PCT(10), LV_PCT(16));
                                    lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
                                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_label_set_text_static(obj, "");
                                }
                                {
                                    // tuner_slider
                                    lv_obj_t *obj = lv_slider_create(parent_obj);
                                    objects.tuner_slider = obj;
                                    lv_obj_set_pos(obj, 248, 241);
                                    lv_obj_set_size(obj, 421, 10);
                                    lv_slider_set_range(obj, -500, 500);
                                    lv_slider_set_mode(obj, LV_SLIDER_MODE_SYMMETRICAL);
                                    lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
                                }
                                {
                                    // tuner_cents_label
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.tuner_cents_label = obj;
                                    lv_obj_set_pos(obj, 414, 265);
                                    lv_obj_set_size(obj, LV_PCT(10), LV_PCT(10));
                                    lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
                                    lv_obj_set_style_bg_opa(obj, 100, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_label_set_text_static(obj, "");
                                }
                                {
                                    // tuner_state
                                    lv_obj_t *obj = lv_switch_create(parent_obj);
                                    objects.tuner_state = obj;
                                    lv_obj_set_pos(obj, 108, 119);
                                    lv_obj_set_size(obj, 50, 25);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    
    tick_screen_main();
}

void tick_screen_main() {
}

void create_user_widget_loop(lv_obj_t *parent_obj, int startWidgetIndex) {
    (void)startWidgetIndex;
    lv_obj_t *obj = parent_obj;
    {
        lv_obj_t *parent_obj = obj;
        {
            lv_obj_t *obj = lv_obj_create(parent_obj);
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, LV_PCT(100), LV_PCT(100));
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_WITH_ARROW);
            lv_obj_set_style_layout(obj, LV_LAYOUT_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_column(obj, 11, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // bar
                    lv_obj_t *obj = lv_bar_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 0] = obj;
                    lv_obj_set_pos(obj, 17, 18);
                    lv_obj_set_size(obj, 677, 15);
                    lv_obj_add_flag(obj, LV_OBJ_FLAG_EVENT_BUBBLE);
                }
                {
                    // slider
                    lv_obj_t *obj = lv_slider_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 1] = obj;
                    lv_obj_set_pos(obj, 719, 18);
                    lv_obj_set_size(obj, LV_PCT(20), 15);
                    lv_slider_set_value(obj, 25, LV_ANIM_OFF);
                }
            }
        }
    }
}

void tick_user_widget_loop(int startWidgetIndex) {
    (void)startWidgetIndex;
}

typedef void (*tick_screen_func_t)();
tick_screen_func_t tick_screen_funcs[] = {
    tick_screen_main,
};
void tick_screen(int screen_index) {
    if (screen_index >= 0 && screen_index < 1) {
        tick_screen_funcs[screen_index]();
    }
}
void tick_screen_by_id(enum ScreensEnum screenId) {
    tick_screen(screenId - 1);
}

//
// Fonts
//

ext_font_desc_t fonts[] = {
#if LV_FONT_MONTSERRAT_8
    { "MONTSERRAT_8", &lv_font_montserrat_8 },
#endif
#if LV_FONT_MONTSERRAT_10
    { "MONTSERRAT_10", &lv_font_montserrat_10 },
#endif
#if LV_FONT_MONTSERRAT_12
    { "MONTSERRAT_12", &lv_font_montserrat_12 },
#endif
#if LV_FONT_MONTSERRAT_14
    { "MONTSERRAT_14", &lv_font_montserrat_14 },
#endif
#if LV_FONT_MONTSERRAT_16
    { "MONTSERRAT_16", &lv_font_montserrat_16 },
#endif
#if LV_FONT_MONTSERRAT_18
    { "MONTSERRAT_18", &lv_font_montserrat_18 },
#endif
#if LV_FONT_MONTSERRAT_20
    { "MONTSERRAT_20", &lv_font_montserrat_20 },
#endif
#if LV_FONT_MONTSERRAT_22
    { "MONTSERRAT_22", &lv_font_montserrat_22 },
#endif
#if LV_FONT_MONTSERRAT_24
    { "MONTSERRAT_24", &lv_font_montserrat_24 },
#endif
#if LV_FONT_MONTSERRAT_26
    { "MONTSERRAT_26", &lv_font_montserrat_26 },
#endif
#if LV_FONT_MONTSERRAT_28
    { "MONTSERRAT_28", &lv_font_montserrat_28 },
#endif
#if LV_FONT_MONTSERRAT_30
    { "MONTSERRAT_30", &lv_font_montserrat_30 },
#endif
#if LV_FONT_MONTSERRAT_32
    { "MONTSERRAT_32", &lv_font_montserrat_32 },
#endif
#if LV_FONT_MONTSERRAT_34
    { "MONTSERRAT_34", &lv_font_montserrat_34 },
#endif
#if LV_FONT_MONTSERRAT_36
    { "MONTSERRAT_36", &lv_font_montserrat_36 },
#endif
#if LV_FONT_MONTSERRAT_38
    { "MONTSERRAT_38", &lv_font_montserrat_38 },
#endif
#if LV_FONT_MONTSERRAT_40
    { "MONTSERRAT_40", &lv_font_montserrat_40 },
#endif
#if LV_FONT_MONTSERRAT_42
    { "MONTSERRAT_42", &lv_font_montserrat_42 },
#endif
#if LV_FONT_MONTSERRAT_44
    { "MONTSERRAT_44", &lv_font_montserrat_44 },
#endif
#if LV_FONT_MONTSERRAT_46
    { "MONTSERRAT_46", &lv_font_montserrat_46 },
#endif
#if LV_FONT_MONTSERRAT_48
    { "MONTSERRAT_48", &lv_font_montserrat_48 },
#endif
};

//
// Color themes
//

uint32_t active_theme_index = 0;

//
//
//

void create_screens() {

// Set default LVGL theme
    lv_display_t *dispp = lv_display_get_default();
    lv_theme_t *theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), true, LV_FONT_DEFAULT);
    lv_display_set_theme(dispp, theme);
    
    // Initialize screens
    // Create screens
    create_screen_main();
}