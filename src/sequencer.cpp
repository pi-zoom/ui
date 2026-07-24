#include <iostream>
#include "sequencer.hpp"
#include "lvgl/lvgl.h"
#include "ui/ui.h"
#include "transport/zmq_transport.hpp"

lv_obj_t *midiFilesDropdown = nullptr;
lv_obj_t *bpmValue = nullptr;
lv_obj_t *volumeValue = nullptr;
lv_obj_t *midiPosBar = nullptr;

void lv_sequencer_create(lv_obj_t *parent){
    lv_obj_set_flex_flow(parent, LV_FLEX_FLOW_COLUMN);

    // sequencer_control_panel
    lv_obj_t *controlContainer = lv_obj_create(parent);
    lv_obj_set_size(controlContainer, LV_PCT(100), LV_PCT(20));
    lv_obj_set_style_pad_left(controlContainer, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(controlContainer, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(controlContainer, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(controlContainer, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(controlContainer, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_scrollable(controlContainer, false);
    lv_obj_set_style_border_color(controlContainer, lv_color_hex(0x4b66c4), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(controlContainer, 100, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(controlContainer, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(controlContainer, lv_color_hex(0xafabd6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(controlContainer, 100, LV_PART_MAIN | LV_STATE_DEFAULT);

    // sequencer_midi_files_list
    midiFilesDropdown = lv_dropdown_create(controlContainer);
    lv_obj_set_pos(midiFilesDropdown, 10, 36);
    lv_obj_set_size(midiFilesDropdown, 259, LV_SIZE_CONTENT);
    lv_dropdown_set_options_static(midiFilesDropdown, "");
    lv_dropdown_set_selected(midiFilesDropdown, 0);

    // sequencer_label
    lv_obj_t *midiFilesListLabel = lv_label_create(controlContainer);
    lv_obj_set_pos(midiFilesListLabel, 10, 12);
    lv_obj_set_size(midiFilesListLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_label_set_text_static(midiFilesListLabel, "MIDI Files");

    // sequencer_slider_bpm
    lv_obj_t *bpmSlider = lv_slider_create(controlContainer);
    lv_obj_set_pos(bpmSlider, 327, 46);
    lv_obj_set_size(bpmSlider, 244, 32);
    lv_slider_set_range(bpmSlider, 20, 150);
    lv_slider_set_value(bpmSlider, 120, LV_ANIM_OFF);

    // sequencer_bpm_label
    lv_obj_t *bpmLabel = lv_label_create(controlContainer);
    lv_obj_set_pos(bpmLabel, 327, 12);
    lv_obj_set_size(bpmLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_label_set_text_static(bpmLabel, "BPM");

    // sequencer_bpm_value
    bpmValue = lv_label_create(controlContainer);
    lv_obj_set_pos(bpmValue, 435, 12);
    lv_obj_set_size(bpmValue, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_label_set_text_static(bpmValue, "120");

    // sequencer_slider_volume
    lv_obj_t *volumeSlider = lv_slider_create(controlContainer);
    lv_obj_set_pos(volumeSlider, 646, 47);
    lv_obj_set_size(volumeSlider, 224, 31);
    lv_slider_set_value(volumeSlider, 50, LV_ANIM_OFF);

    // sequencer_volume_label
    lv_obj_t *volumeLabel = lv_label_create(controlContainer);
    lv_obj_set_pos(volumeLabel, 646, 12);
    lv_obj_set_size(volumeLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_label_set_text_static(volumeLabel, "Volume");

    // sequencer_volume_value
    volumeValue = lv_label_create(controlContainer);
    lv_obj_set_pos(volumeValue, 758, 12);
    lv_obj_set_size(volumeValue, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_label_set_text_static(volumeValue, "50");

    // sequencer_display_panel
    lv_obj_t *displayContainer = lv_obj_create(parent);
    lv_obj_set_size(displayContainer, LV_PCT(100), LV_PCT(80));
    lv_obj_set_style_pad_left(displayContainer, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(displayContainer, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(displayContainer, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(displayContainer, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(displayContainer, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_scrollable(displayContainer, false);
    lv_obj_set_style_border_color(displayContainer, lv_color_hex(0x4b66c4), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(displayContainer, 100, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(displayContainer, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(displayContainer, lv_color_hex(0xafabd6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(displayContainer, 100, LV_PART_MAIN | LV_STATE_DEFAULT);

    // sequencer_bar
    midiPosBar = lv_bar_create(displayContainer);
    lv_obj_set_pos(midiPosBar, 33, 37);
    lv_obj_set_size(midiPosBar, 838, 16);
    lv_bar_set_range(midiPosBar, 0, 1000);
    lv_bar_set_value(midiPosBar, 0, LV_ANIM_OFF);

    // sequencer_state_button
    lv_obj_t *stateSwitch = lv_switch_create(displayContainer);
    lv_obj_set_pos(stateSwitch, 116, 156);
    lv_obj_set_size(stateSwitch, 90, 38);

    // sequencer_state_label
    lv_obj_t *stateLabel = lv_label_create(displayContainer);
    lv_obj_set_pos(stateLabel, 142, 128);
    lv_obj_set_size(stateLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_label_set_text_static(stateLabel, "State");

    // sequencer_mute_button
    lv_obj_t *muteSwitch = lv_switch_create(displayContainer);
    lv_obj_set_pos(muteSwitch, 585, 156);
    lv_obj_set_size(muteSwitch, 90, 38);

    // sequencer_mute_label
    lv_obj_t *muteLabel = lv_label_create(displayContainer);
    lv_obj_set_pos(muteLabel, 609, 128);
    lv_obj_set_size(muteLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_label_set_text_static(muteLabel, "Mute");

    lv_obj_add_event_cb(midiFilesDropdown, midi_file_changed, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_event_cb(bpmSlider, bpm_changed, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_event_cb(volumeSlider, volume_changed, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_event_cb(muteSwitch, sequencer_mute_changed, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_event_cb(stateSwitch, sequencer_state_changed, LV_EVENT_VALUE_CHANGED, NULL);
}


void bpm_changed(lv_event_t *e)
{
    lv_obj_t *slider = (lv_obj_t*)lv_event_get_current_target(e);
    int value = lv_slider_get_value(slider);
    lv_label_set_text(bpmValue, std::to_string(value).c_str());
    transport.sendCommand(CmdSequencerBpm{.bpm = value});
}

void volume_changed(lv_event_t *e){
    lv_obj_t *slider = (lv_obj_t*)lv_event_get_current_target(e);
    int value = lv_slider_get_value(slider);
    lv_label_set_text(volumeValue, std::to_string(value).c_str());
    transport.sendCommand(CmdSequencerVolume{.volume = value});
}

void midi_file_changed(lv_event_t *e){
    lv_obj_t *obj = (lv_obj_t *)lv_event_get_target(e);
    char buf[100];
    lv_dropdown_get_selected_str(midiFilesDropdown, buf, sizeof(buf));
    transport.sendCommand(CmdSequencerSelectMidiFile{.midiFile = std::string(buf)});
}

void sequencer_state_changed(lv_event_t *e){
    lv_obj_t *sw = lv_event_get_target_obj(e);
    bool state = lv_obj_has_state(sw, LV_STATE_CHECKED);
    transport.sendCommand(CmdSequencerState{.state = state});
}

void sequencer_mute_changed(lv_event_t *e){
    lv_obj_t *sw = lv_event_get_target_obj(e);
    bool mute = lv_obj_has_state(sw, LV_STATE_CHECKED);
    transport.sendCommand(CmdSequencerMute{.mute = mute});
}

void sequencer_set_file_list(std::vector<std::string> midiFiles){
    if(midiFilesDropdown == nullptr)
        return;

    lv_dropdown_clear_options(midiFilesDropdown);
    for(auto midi: midiFiles){
        lv_dropdown_add_option(midiFilesDropdown, midi.c_str(), LV_DROPDOWN_POS_LAST);
    }
}

void sequencer_set_position(double pos){
    if(midiPosBar == nullptr)
        return;
    lv_bar_set_value(midiPosBar, (int)(pos * 10.0), LV_ANIM_OFF);
}