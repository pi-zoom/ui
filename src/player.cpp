#include <iostream>
#include "player.hpp"
#include "lvgl/lvgl.h"
#include "ui/ui.h"
#include "transport/zmq_transport.hpp"
#include <list>

std::list<RecordedFile> recordedFiles;

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

void lv_create_recorded_file(std::string name){

    recordedFiles.emplace_back();
    RecordedFile& recorded = recordedFiles.back();
    recorded.name = name;
    recorded.playing = false;

    recorded.container = lv_obj_create(objects.player_files);
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

void player_set_file_list(EventRecorderFileList e){

    for (const auto& name : e.files) {
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

void player_set_recording(EventRecorderRecording e){
    for(RecordedFile &file: recordedFiles){
        if(file.playing){
            file.playing = false;
            lv_label_set_text(file.play_button_label, "Play");
        }
    }

    lv_obj_add_state(objects.player_record_button, LV_STATE_CHECKED);
}

void player_set_playing(EventRecorderPlaying e){

    for(RecordedFile &record: recordedFiles){
        if(record.name != e.file && record.playing){
            record.playing = false;
            lv_label_set_text(record.play_button_label, "Play");
        }else if(record.name == e.file){
            record.playing = true;
            lv_label_set_text(record.play_button_label, "Playing");
        }
    }

}

void player_set_stopped(EventRecorderStopped e){
    for(RecordedFile &record: recordedFiles){
        if(record.playing){
            record.playing = false;
            lv_label_set_text(record.play_button_label, "Play");
        }
    }
    lv_obj_remove_state(objects.player_record_button, LV_STATE_CHECKED);
}