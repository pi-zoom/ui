#include <iostream>
#include "sequencer.hpp"
#include "lvgl/lvgl.h"
#include "ui/ui.h"
#include "transport/zmq_transport.hpp"

void bpm_changed(lv_event_t *e)
{
    lv_obj_t *slider = (lv_obj_t*)lv_event_get_current_target(e);
    int value = lv_slider_get_value(slider);
    lv_label_set_text(objects.sequencer_bpm_value, std::to_string(value).c_str());
    transport.sendCommand(CmdSequencerBpm{.bpm = value});
}

void volume_changed(lv_event_t *e){
    lv_obj_t *slider = (lv_obj_t*)lv_event_get_current_target(e);
    int value = lv_slider_get_value(slider);
    lv_label_set_text(objects.sequencer_volume_value, std::to_string(value).c_str());
    transport.sendCommand(CmdSequencerVolume{.volume = value});
}

void midi_file_changed(lv_event_t *e){
    lv_obj_t *obj = (lv_obj_t *)lv_event_get_target(e);
    char buf[100];
    lv_dropdown_get_selected_str(objects.sequencer_midi_files_list, buf, sizeof(buf));
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

void sequencer_set_file_list(const EventSequencerMidiFilesList &e){
    lv_dropdown_clear_options(objects.sequencer_midi_files_list);
    for(auto midi: e.midiFiles){
        lv_dropdown_add_option(objects.sequencer_midi_files_list, midi.c_str(), LV_DROPDOWN_POS_LAST);
    }
}

void sequencer_set_position(const EventSequencerPosition &e){
    if(objects.sequencer_bar != nullptr){
        lv_bar_set_value(objects.sequencer_bar, (int)(e.pos * 10.0), LV_ANIM_OFF);
    }
}