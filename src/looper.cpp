#include <iostream>
#include "looper.hpp"
#include "lvgl/lvgl.h"
#include "ui/ui.h"
#include "transport/zmq_transport.hpp"

static std::vector<Loop> loops = [] {
    std::vector<Loop> v;
    v.reserve(20);
    return v;
}();
int selected_loop = -1;

void add_loop_clicked(lv_event_t *e)
{
    transport.sendCommand(CmdLooperAddLoop{});
}

void remove_loop_clicked(lv_event_t *e)
{
    transport.sendCommand(CmdLooperRemoveLoop{});
}


void loop_clicked(lv_event_t *e)
{
    Loop *loop = (Loop*)lv_event_get_user_data(e);
    transport.sendCommand(CmdLooperSelectLoop{.id = loop->id});
}

void lv_loop_create(Loop &loop){
    loop.container = lv_obj_create(objects.loop_list);
    lv_obj_set_size(loop.container, LV_PCT(100), 50);
    lv_obj_set_style_pad_left(loop.container, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(loop.container, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(loop.container, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(loop.container, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(loop.container, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(loop.container, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(loop.container, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_layout(loop.container, LV_LAYOUT_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_column(loop.container, 11, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_event_cb(loop.container, loop_clicked, LV_EVENT_CLICKED, (void *)&loop);

    // bar
    loop.bar = lv_bar_create(loop.container);
    lv_obj_set_pos(loop.bar, 17, 18);
    lv_obj_set_size(loop.bar, 677, 15);
    lv_obj_add_flag(loop.bar, LV_OBJ_FLAG_EVENT_BUBBLE);
    lv_bar_set_min_value(loop.bar, 0);
    lv_bar_set_max_value(loop.bar, 0);
    lv_bar_set_value(loop.bar, 0, LV_ANIM_OFF);

    // slider
    loop.slider = lv_slider_create(loop.container);
    lv_obj_set_pos(loop.slider, 719, 18);
    lv_obj_set_size(loop.slider, LV_PCT(20), 15);
    lv_slider_set_value(loop.slider, 25, LV_ANIM_OFF);
}

void lv_loop_delete(Loop &loop)
{
    lv_obj_delete(loop.container);
}

void remove_loop(){
    if(loops.size() == 0)
        return;
    lv_loop_delete(loops.back());
    loops.pop_back();
}

void add_loop()
{
    if(loops.size() == 20)
        return;

    Loop &loop = loops.emplace_back();
    loop.id = loops.size() - 1;
    lv_loop_create(loop);
}

void looper_set_loops_count(const EventLooperLoopCount &e)
{
    if(e.count > loops.size()){
        while(e.count > loops.size()){
            add_loop();
        }
    }else{
        while(e.count < loops.size()){
            remove_loop();
        }
    }
}

void looper_set_loop_len(int loop_id, float len){
    if (loops.size() == 0)
    {
        return;
    }
    if (loop_id >= loops.size())
    {
        return;
    }

    Loop &L = loops.at(loop_id);
    if (L.bar == NULL)
    {
        return;
    }
    L.len = len;
    lv_bar_set_max_value(L.bar, int(L.len * 100.0));
}

void looper_set_loop_state(int loop_id, int state){
    if (loops.size() == 0)
    {
        return;
    }
    if (loop_id >= loops.size())
    {
        return;
    }

    Loop &L = loops.at(loop_id);
    if (L.bar == NULL)
    {
        return;
    }

    bool reset = false;
    lv_color_t color;
    switch (state)
    {
    case -1:
        reset = true;
        L.state = false;
        break;
    case 0:
        reset = true;
        L.state = false;
        break;
    case 4:
        reset = true;
        L.state = true;
        break;
    case 2:
        reset = false;
        break;

    case 3:
        color = lv_color_hex(0xd10c0c);
        break;
    default:
        break;
    }

    if (reset)
    {
        lv_obj_remove_local_style_prop(L.bar, LV_STYLE_SHADOW_WIDTH, LV_PART_MAIN);
        lv_obj_remove_local_style_prop(L.bar, LV_STYLE_SHADOW_SPREAD, LV_PART_MAIN);
        lv_obj_remove_local_style_prop(L.bar, LV_STYLE_SHADOW_COLOR, LV_PART_MAIN);
        lv_obj_remove_local_style_prop(L.bar, LV_STYLE_SHADOW_OPA, LV_PART_MAIN);
    }
    else
    {
        lv_obj_set_style_shadow_width(L.bar, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_shadow_spread(L.bar, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_shadow_color(L.bar, color, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_shadow_opa(L.bar, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    }
}

void looper_set_loop_pos(int loop_id, float pos){
    if (loops.size() == 0)
    {
        return;
    }
    if (loop_id >= loops.size())
    {
        return;
    }

    Loop &L = loops.at(loop_id);
    if (L.bar == NULL)
    {
        return;
    }

    if (!L.state)
    {
        return;
    }
    L.pos = pos;
    lv_bar_set_value(L.bar, int(pos * 100.0), LV_ANIM_OFF);
}

void looper_set_loop_selected(const EventLooperLoopSelected &e){
    if (loops.size() == 0)
    {
        return;
    }
    if (e.id >= loops.size())
    {
        return;
    }

    for (size_t i = 0; i < loops.size(); ++i)
    {
        if (i == e.id)
        {
            lv_obj_set_style_border_color(loops.at(i).container, lv_color_hex(0xd10c0c), LV_PART_MAIN);
            lv_obj_set_style_border_width(loops.at(i).container, 5, LV_PART_MAIN);
        }
        else
        {
            lv_obj_remove_local_style_prop(loops.at(i).container, LV_STYLE_BORDER_COLOR, LV_PART_MAIN);
            lv_obj_remove_local_style_prop(loops.at(i).container, LV_STYLE_BORDER_WIDTH, LV_PART_MAIN);
        }
    }
}

void looper_set_loop_list(const EventLooperLoopList &e){
    for(int i; i < loops.size(); i++){
        remove_loop();
    }

    for(const Loop &loop: e.loops){
        add_loop();
        looper_set_loop_len(loop.id, loop.len);
        looper_set_loop_pos(loop.id, loop.pos);
        looper_set_loop_state(loop.id, loop.state);
    }
}