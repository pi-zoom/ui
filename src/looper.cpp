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

lv_obj_t *listContainer = nullptr;

static lv_style_t styleLoopDefault;
static lv_style_t styleLoopSelected;
static lv_style_t styleLoopWaitingRecord;
static lv_style_t styleLoopWaitingStop;
static lv_style_t styleLoopPlaying;

void init_styles(void){

}

void lv_looper_create(lv_obj_t *parent){

    init_styles();

    lv_obj_set_flex_flow(parent, LV_FLEX_FLOW_COLUMN);

    // loop_control
    lv_obj_t *controlContainer = lv_obj_create(parent);
    lv_obj_set_size(controlContainer, LV_PCT(100), LV_PCT(20));
    lv_obj_set_style_pad_left(controlContainer, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(controlContainer, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(controlContainer, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(controlContainer, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(controlContainer, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_scrollable(controlContainer, false);

    static lv_coord_t dsc[] = {0, LV_GRID_TEMPLATE_LAST};
    lv_obj_set_style_grid_row_dsc_array(controlContainer, dsc, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_bg_color(controlContainer, lv_color_hex(0x9a9fc4), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(controlContainer, 100, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(controlContainer, lv_color_hex(0x3148bb), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(controlContainer, 100, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(controlContainer, 2, LV_PART_MAIN | LV_STATE_DEFAULT);

    // loop_add_button
    lv_obj_t *addButton = lv_button_create(controlContainer);
    lv_obj_set_pos(addButton, 250, 15);
    lv_obj_set_size(addButton, 100, 50);

    lv_obj_t *addButtonLabel = lv_label_create(addButton);
    lv_obj_set_size(addButtonLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_style_align(addButtonLabel, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_text_static(addButtonLabel, "Add");

    // loop_remove_button
    lv_obj_t *removeButton = lv_button_create(controlContainer);
    lv_obj_set_pos(removeButton, 509, 15);
    lv_obj_set_size(removeButton, 100, 50);

    lv_obj_t *removeButtonLabel = lv_label_create(removeButton);
    lv_obj_set_size(removeButtonLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_style_align(removeButtonLabel, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_text_static(removeButtonLabel, "Del");

    // loop_list
    listContainer = lv_obj_create(parent);
    lv_obj_set_size(listContainer, LV_PCT(100), LV_PCT(80));
    lv_obj_set_style_pad_left(listContainer, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(listContainer, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(listContainer, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(listContainer, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(listContainer, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_scrollable(listContainer, false);
    lv_obj_set_style_border_color(listContainer, lv_color_hex(0x4b66c4), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(listContainer, 100, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(listContainer, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(listContainer, lv_color_hex(0xafabd6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(listContainer, 100, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_layout(listContainer, LV_LAYOUT_FLEX, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_flex_flow(listContainer, LV_FLEX_FLOW_COLUMN, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_add_event_cb(addButton, add_loop_clicked, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(removeButton, remove_loop_clicked, LV_EVENT_CLICKED, NULL);
}

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

    if(listContainer == nullptr)
        return;

    loop.container = lv_obj_create(listContainer);
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
    lv_obj_set_event_bubble(loop.bar, true);
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

void looper_set_loops_count(int count)
{
    if(count > loops.size()){
        while(count > loops.size()){
            add_loop();
        }
    }else{
        while(count < loops.size()){
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

void looper_set_loop_pos(int id, float pos){
    if (loops.size() == 0)
    {
        return;
    }
    if (id >= loops.size())
    {
        return;
    }

    Loop &L = loops.at(id);
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

void looper_set_loop_selected(int id){
    if (loops.size() == 0)
    {
        return;
    }
    if (id >= loops.size())
    {
        return;
    }

    for (size_t i = 0; i < loops.size(); ++i)
    {
        if (i == id)
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

void looper_set_loop_list(std::vector<Loop> loops, int selected){
    for(int i; i < loops.size(); i++){
        remove_loop();
    }

    for(Loop &loop: loops){
        add_loop();
        looper_set_loop_len(loop.id, loop.len);
        looper_set_loop_pos(loop.id, loop.pos);
        looper_set_loop_state(loop.id, loop.state);
    }
    looper_set_loop_selected(selected);
}