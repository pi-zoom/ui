#ifndef LOOPER_HPP
#define LOOPER_HPP
#include <vector>
#include "lvgl/lvgl.h"

struct EventLoopCount;
struct EventLoopSelected;
struct EventLoopList;

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
void looper_set_loops_count(const EventLoopCount &e);
void looper_set_loop_pos(int loop_id, float pos);
void looper_set_loop_state(int loop_id, int state);
void looper_set_loop_len(int loop_id, float len);
void looper_set_loop_selected(const EventLoopSelected &e);
void looper_set_loop_list(const EventLoopList &e);

void add_loop_clicked(lv_event_t *e);
void remove_loop_clicked(lv_event_t *e);
#endif