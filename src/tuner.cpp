#include <iostream>
#include "tuner.hpp"
#include "ui/ui.h"
#include "transport/zmq_transport.hpp"

void tuner_update(std::string note, float cents){
    lv_label_set_text(objects.tuner_note_label, note.c_str());
    lv_slider_set_value(objects.tuner_slider, (int)(cents * 10), LV_ANIM_OFF);
    lv_label_set_text(objects.tuner_cents_label, std::to_string(cents).c_str());
}

void tuner_state(lv_event_t *e){
    lv_obj_t *sw = (lv_obj_t*)lv_event_get_target(e);
    bool state = lv_obj_has_state(sw, LV_STATE_CHECKED);

    if (!state){
        lv_obj_set_hidden(objects.tuner_cents_label, true);
        lv_obj_set_hidden(objects.tuner_note_label, true);
        lv_obj_set_hidden(objects.tuner_slider, true);
    }else{
        lv_obj_set_hidden(objects.tuner_cents_label, false);
        lv_obj_set_hidden(objects.tuner_note_label, false);
        lv_obj_set_hidden(objects.tuner_slider, false);
    }
    transport.sendCommand(CmdTuner{.state = state});
}