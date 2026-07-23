#ifndef SEQUENCER_HPP
#define SEQUENCER_HPP
#include <vector>
#include "lvgl/lvgl.h"

struct EventSequencerMidiFilesList;
struct EventSequencerPosition;

void bpm_changed(lv_event_t *e);
void volume_changed(lv_event_t *e);
void midi_file_changed(lv_event_t *e);
void sequencer_state_changed(lv_event_t *e);
void sequencer_mute_changed(lv_event_t *e);

void sequencer_set_file_list(const EventSequencerMidiFilesList &e);
void sequencer_set_position(const EventSequencerPosition &e);

#endif