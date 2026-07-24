#ifndef LOOPER_HPP
#define LOOPER_HPP
#include <vector>
#include "lvgl/lvgl.h"

/**
 * Looper part
 */
struct Loop
{
    int id = 0;
    float state = 0.0;
    float pos = 0.0;
    float len = 0.0;

    lv_obj_t *container = nullptr;
    lv_obj_t *bar = nullptr;
    lv_obj_t *slider = nullptr;
};

//****/

// void add_loop();
void looper_set_loops_count(int count);
void looper_set_loop_pos(int id, float pos);
void looper_set_loop_state(int id, int state);
void looper_set_loop_len(int id, float len);
void looper_set_loop_selected(int id);
void looper_set_loop_list(std::vector<Loop> list, int selected);

void lv_looper_create(lv_obj_t *parent);
void add_loop_clicked(lv_event_t *e);
void remove_loop_clicked(lv_event_t *e);

void add_loop();
#endif