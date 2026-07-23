#ifndef PLAYER_HPP
#define PLAYER_HPP
#include <vector>
#include "lvgl/lvgl.h"
#include <string>

struct RecordedFile {
    std::string name;
    float duration;

    lv_obj_t *container;
    lv_obj_t *label;
    lv_obj_t *play_button;
};

void player_record_changed(lv_event_t *e);
void player_set_file_list(std::vector<std::string> files);
#endif