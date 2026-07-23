#ifndef PLAYER_HPP
#define PLAYER_HPP
#include <vector>
#include "lvgl/lvgl.h"
#include <string>
#include "protocol/protocol.hpp"

struct RecordedFile {
    std::string name;
    float duration;
    bool playing;

    lv_obj_t *container;
    lv_obj_t *label;
    lv_obj_t *play_button;
    lv_obj_t *play_button_label;
};

void player_record_changed(lv_event_t *e);

void player_set_file_list(EventRecorderFileList e);
void player_set_recording(EventRecorderRecording e);
void player_set_playing(EventRecorderPlaying e);
void player_set_stopped(EventRecorderStopped e);

#endif