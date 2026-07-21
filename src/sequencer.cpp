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