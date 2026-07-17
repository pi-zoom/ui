#ifndef EEZ_LVGL_UI_EVENTS_H
#define EEZ_LVGL_UI_EVENTS_H

#include <lvgl/lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

extern void action_loop_clicked(lv_event_t * e);
extern void action_add_loop(lv_event_t * e);
extern void action_loop_bar_clicked(lv_event_t * e);
extern void action_del_loop(lv_event_t * e);

#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_EVENTS_H*/