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

void lv_player_create(lv_obj_t *parent);
void player_record_changed(lv_event_t *e);

void player_set_file_list(std::vector<std::string> files);
void player_set_recording(int start);
void player_set_playing(std::string file);
void player_set_stopped(void);

#endif