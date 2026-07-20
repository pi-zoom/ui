#ifndef TUNER_HPP
#define TUNER_HPP
#include <string>
#include "lvgl/lvgl.h"

void tuner_update(std::string note, float cents);
void tuner_state(lv_event_t *e);
#endif