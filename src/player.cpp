#include <iostream>
#include "player.hpp"
#include "lvgl/lvgl.h"
#include "ui/ui.h"
#include "transport/zmq_transport.hpp"
#include <list>

std::list<RecordedFile> recordedFiles;

lv_obj_t *filesContainer = nullptr;
lv_obj_t *recordButton = nullptr;

void play_clicked(lv_event_t *e){

    RecordedFile *recordedFile = (RecordedFile*)lv_event_get_user_data(e);
    if(!recordedFile->playing){
        recordedFile->playing = true;
        lv_label_set_text(recordedFile->play_button_label, "Playing");
        transport.sendCommand(CmdPlayerPlay{.state = recordedFile->playing, .file = recordedFile->name});
    }else{
        recordedFile->playing = false;
        lv_label_set_text(recordedFile->play_button_label, "Play");
        transport.sendCommand(CmdPlayerPlay{.state = recordedFile->playing, .file = ""});
    }
}

void lv_player_create(lv_obj_t *parent){
    lv_obj_set_flex_flow(parent, LV_FLEX_FLOW_COLUMN);

    // play_control
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

    // player_record_button
    recordButton = lv_switch_create(controlContainer);
    lv_obj_set_pos(recordButton, 158, 23);
    lv_obj_set_size(recordButton, 103, 40);

    // player_recording_duration_label
    lv_obj_t *recordDurationLabel = lv_label_create(controlContainer);
    lv_obj_set_pos(recordDurationLabel, 394, 20);
    lv_obj_set_size(recordDurationLabel, LV_PCT(30), LV_PCT(50));
    lv_obj_set_style_bg_color(recordDurationLabel, lv_color_hex(0x1dbccb), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(recordDurationLabel, 100, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_text_static(recordDurationLabel, "");

    // player_files
    filesContainer = lv_obj_create(parent);
    lv_obj_set_size(filesContainer, LV_PCT(100), LV_PCT(80));
    lv_obj_set_style_pad_left(filesContainer, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(filesContainer, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(filesContainer, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(filesContainer, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(filesContainer, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(filesContainer, lv_color_hex(0x4b66c4), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(filesContainer, 100, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(filesContainer, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(filesContainer, lv_color_hex(0xafabd6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(filesContainer, 100, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_layout(filesContainer, LV_LAYOUT_FLEX, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_flex_flow(filesContainer, LV_FLEX_FLOW_ROW_WRAP, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_add_event_cb(recordButton, player_record_changed, LV_EVENT_VALUE_CHANGED, NULL);
}

void lv_create_recorded_file(std::string name){

    recordedFiles.emplace_back();
    RecordedFile& recorded = recordedFiles.back();
    recorded.name = name;
    recorded.playing = false;

    recorded.container = lv_obj_create(filesContainer);
    lv_obj_set_size(recorded.container, LV_PCT(100), 50);
    lv_obj_set_style_pad_left(recorded.container, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(recorded.container, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(recorded.container, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(recorded.container, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(recorded.container, lv_color_hex(0x739ce1), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(recorded.container, 100, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_margin_top(recorded.container, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_margin_left(recorded.container, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_margin_right(recorded.container, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_layout(recorded.container, LV_LAYOUT_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);

    recorded.label = lv_label_create(recorded.container);
    lv_obj_set_pos(recorded.label, 10, 9);
    lv_obj_set_size(recorded.label, LV_PCT(70), LV_PCT(70));
    lv_label_set_text(recorded.label, recorded.name.c_str());

    recorded.play_button = lv_button_create(recorded.container);
    lv_obj_set_pos(recorded.play_button, 788, 8);
    lv_obj_set_size(recorded.play_button, LV_PCT(10), LV_PCT(70));
    lv_obj_add_event_cb(recorded.play_button, play_clicked, LV_EVENT_CLICKED, (void*)&recorded);

    recorded.play_button_label = lv_label_create(recorded.play_button);
    lv_obj_set_size(recorded.play_button_label, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_style_align(recorded.play_button_label, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_text(recorded.play_button_label, "Play");
}

void player_record_changed(lv_event_t *e){
    lv_obj_t *sw = lv_event_get_target_obj(e);
    bool state = lv_obj_has_state(sw, LV_STATE_CHECKED);
    transport.sendCommand(CmdPlayerRecord{.state = state});
}

void player_set_file_list(std::vector<std::string> files){

    for (const auto& name : files) {
        bool found = false;
        for (auto it = recordedFiles.begin(); it != recordedFiles.end(); ++it) {
            if((*it).name == name){
                found = true;
                break;
            }
        }
        if (!found){
            lv_create_recorded_file(name);
        }
    }
}

void player_set_recording(int start){
    for(RecordedFile &file: recordedFiles){
        if(file.playing){
            file.playing = false;
            lv_label_set_text(file.play_button_label, "Play");
        }
    }

    lv_obj_add_state(recordButton, LV_STATE_CHECKED);
}

void player_set_playing(std::string file){

    for(RecordedFile &record: recordedFiles){
        if(record.name != file && record.playing){
            record.playing = false;
            lv_label_set_text(record.play_button_label, "Play");
        }else if(record.name == file){
            record.playing = true;
            lv_label_set_text(record.play_button_label, "Playing");
        }
    }

}

void player_set_stopped(void){
    for(RecordedFile &record: recordedFiles){
        if(record.playing){
            record.playing = false;
            lv_label_set_text(record.play_button_label, "Play");
        }
    }
    lv_obj_remove_state(recordButton, LV_STATE_CHECKED);
}