#ifndef PROTOCOL_HPP
#define PROTOCOL_HPP
#include <vector>
#include <string>
#include <variant>
#include <nlohmann/json.hpp>
#include "../mod.hpp"
#include "../looper.hpp"

using json = nlohmann::json;

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(
    Loop,
    id,
    len,
    pos,
    state)

// struct ParamRange
// {
//     float minimum;
//     float maximum;
//     float defaultValue;
// };

inline void from_json(const json& j, ParamRange& r)
{
    j.at("minimum").get_to(r.minimum);
    j.at("maximum").get_to(r.maximum);
    j.at("default").get_to(r.defaultValue);
}

inline void to_json(json& j, const ParamRange& r)
{
    j["minimum"] = r.minimum;
    j["maximum"] = r.maximum;
    j["default"] = r.defaultValue;
}

// struct ParamScalePoints
// {
//     bool valid;
//     float value;
//     std::string label;
// };

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(
    ParamScalePoints,
    valid,
    value,
    label)

// struct ParamUnits
// {
//     std::string label;
//     std::string render;
//     std::string symbol;
//     bool _custom{};
// };

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(
    ParamUnits,
    label,
    render,
    symbol,
    _custom)

// struct Param
// {
//     int index;
//     std::string name;
//     std::string symbol;
//     ParamRange ranges;
//     ParamUnits units;
//     std::vector<ParamScalePoints> scalePoints;
//     std::vector<std::string> properties;
//     std::string shortName;
// };

// NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(
//     Param,
//     index,
//     name,
//     symbol,
//     ranges,
//     units,
//     scalePoints,
//     properties,
//     shortName)

// struct Plugin
// {
//     std::string name;
//     std::string uri;
//     std::string label;
//     std::string instance_id;
//     std::string bundle;
//     std::vector<std::string> category;
//     std::vector<Param> params;
// };

// NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(
//     Plugin,
//     name,
//     uri,
//     label,
//     instance_id,
//     bundle,
//     category,
//     params)

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(
    Snapshot,
    index,
    name)

// struct Pedalboard
// {
//     std::string title;
//     std::string bundle;
//     std::string uri;
//     std::vector<Plugin> plugins;
//     std::vector<Snapshot> snapshots;
//     int snapshot_id;
// };

// NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(
//     Pedalboard,
//     title,
//     bundle,
//     uri,
//     plugins,
//     snapshots,
//     snapshot_id)

enum class EventType {
    EVENT_LOOPER_LOOP_COUNT,
    EVENT_LOOPER_LOOP_POS,
    EVENT_LOOPER_LOOP_LEN,
    EVENT_LOOPER_LOOP_STATE,
    EVENT_LOOPER_LOOP_SELECTED,
    EVENT_LOOPER_LOOP_LIST,

    EVENT_MOD_PEDALBOARDS_LIST,
    EVENT_MOD_PEDALBOARD_CHANGED,
    EVENT_MOD_EFFECT_PARAM_CHANGED,
    EVENT_MOD_PEDALBOARD_SNAPSHOT_CHANGED,

    EVENT_SEQUENCER_MIDI_FILES_LIST,
    EVENT_SEQUENCER_POSITION,

    EVENT_TUNER,

    EVENT_RECORDER_FILE_LIST,
    EVENT_RECORDER_PLAYING,
    EVENT_RECORDER_RECORDING,
    EVENT_RECORDER_STOPPED
};

struct EventLooperLoopCount {
    int count;
};

struct EventLooperLoopPos {
    int id;
    double pos;
};

struct EventLooperLoopState {
    int id;
    int state;
};

struct EventLooperLoopLen {
    int id;
    double len;
};

struct EventLooperLoopList {
    std::vector<Loop> loops;
};

struct EventLooperLoopSelected {
    int id;
};

struct EventModPedalboardsList {
    std::vector<std::string> pedalboards;
};

struct EventModPedalboardChanged {
    Pedalboard pedalboard;
};

struct EventModEffectParamChanged {
    std::string instance_id;
    std::string symbol;
    float value;
};

struct EventModPedalboardSnapshotChanged {
    int index;
    std::string name;
};

struct EventSequencerMidiFilesList {
    std::vector<std::string> midiFiles;
};

struct EventSequencerPosition {
    double pos;
};

struct EventTuner{
    std::string note;
    float cents;
};

struct EventRecorderFileList {
    std::vector<std::string> files;
};

struct EventRecorderPlaying {
    std::string file;
};

struct EventRecorderRecording {
    int start;
};

struct EventRecorderStopped {

};

using Event = std::variant<
    EventLooperLoopCount,
    EventLooperLoopPos,
    EventLooperLoopState,
    EventLooperLoopLen,
    EventLooperLoopList,
    EventLooperLoopSelected,
    EventModPedalboardsList,
    EventModPedalboardChanged,
    EventModEffectParamChanged,
    EventModPedalboardSnapshotChanged,
    EventSequencerMidiFilesList,
    EventSequencerPosition,
    EventTuner,
    EventRecorderFileList,
    EventRecorderPlaying,
    EventRecorderRecording,
    EventRecorderStopped
>;

enum class CommandType {
    CMD_LOOPER_ADD_LOOP,
    CMD_LOOPER_REMOVE_LOOP,
    CMD_LOOPER_SELECT_LOOP,
    CMD_LOOPER_LIST_LOOPS,

    CMD_MOD_SELECT_PEDALBOARD,
    CMD_MOD_EFFECT_PARAM_SET,
    CMD_MOD_LIST_PEDALBOARDS,
    CMD_MOD_SELECT_PEDALBOARD_SNAPSHOT,

    CMD_SEQUENCER_BPM,
    CMD_SEQUENCER_VOLUME,
    CMD_SEQUENCER_SELECT_MIDI_FILE,
    CMD_SEQUENCER_LIST_MIDI_FILES,
    CMD_SEQUENCER_STATE,
    CMD_SEQUENCER_MUTE,

    CMD_TUNER,

    CMD_RECORDER_RECORD,
    CMD_RECORDER_PLAY,
};

struct CmdLooperListLoops {

};

struct CmdLooperAddLoop {

};

struct CmdLooperRemoveLoop {

};

struct CmdLooperSelectLoop {
    int id;
};

struct CmdModSelectPedalboard {
    std::string name;
};

struct CmdModEffectParamSet {
    std::string instande_id;
    std::string symbol;
    float value;
};

struct CmdModListPedalboards {

};

struct CmdModSelectPedalboardSnapshot {
    int index;
};

struct CmdSequencerBpm {
    int bpm;
};

struct CmdSequencerVolume {
    int volume;
};

struct CmdSequencerSelectMidiFile {
    std::string midiFile;
};

struct CmdSequencerListMidiFiles {

};

struct CmdSequencerState {
    bool state;
};

struct CmdSequencerMute {
    bool mute;
};

struct CmdPlayerRecord {
    bool state;
};

struct CmdPlayerPlay {
    bool state;
    std::string file;
};

struct CmdTuner {
    bool state;
};

inline void to_json(json &j, const CmdLooperListLoops &c)
{
    j = json{
        {"type", CommandType::CMD_LOOPER_LIST_LOOPS}};
}

inline void to_json(json &j, const CmdLooperAddLoop &c)
{
    j = json{
        {"type", CommandType::CMD_LOOPER_ADD_LOOP}};
}

inline void to_json(json &j, const CmdLooperRemoveLoop &c)
{
    j = json{
        {"type", CommandType::CMD_LOOPER_REMOVE_LOOP}};
}

inline void to_json(json &j, const CmdLooperSelectLoop &c)
{
    j = json{
        {"type", CommandType::CMD_LOOPER_SELECT_LOOP},
        {"id", c.id}};
}

inline void to_json(json &j, const CmdModSelectPedalboard &c)
{
    j = json{
        {"type", CommandType::CMD_MOD_SELECT_PEDALBOARD},
        {"name", c.name}};
}

inline void to_json(json &j, const CmdModEffectParamSet &c)
{
    j = json{
        {"type", CommandType::CMD_MOD_EFFECT_PARAM_SET},
        {"instance_id", c.instande_id},
        {"symbol", c.symbol},
        {"value", c.value}};
}

inline void to_json(json &j, const CmdModListPedalboards &c)
{
    j = json{
        {"type", CommandType::CMD_MOD_LIST_PEDALBOARDS}};
}

inline void to_json(json &j, const CmdModSelectPedalboardSnapshot &c)
{
    j = json{
        {"type", CommandType::CMD_MOD_SELECT_PEDALBOARD_SNAPSHOT},
        {"index", c.index}};
}

inline void to_json(json &j, const CmdSequencerBpm &c){
    j = json{
        {"type", CommandType::CMD_SEQUENCER_BPM},
        {"bpm", c.bpm}
    };
}

inline void to_json(json &j, const CmdSequencerVolume &c){
    j = json{
        {"type", CommandType::CMD_SEQUENCER_VOLUME},
        {"volume", c.volume}
    };
}

inline void to_json(json &j, const CmdSequencerSelectMidiFile &c){
    j = json{
        {"type", CommandType::CMD_SEQUENCER_SELECT_MIDI_FILE},
        {"file", c.midiFile}
    };
}

inline void to_json(json &j, const CmdSequencerListMidiFiles &c){
    j = json{
        {"type", CommandType::CMD_SEQUENCER_LIST_MIDI_FILES}
    };
}

inline void to_json(json &j, const CmdSequencerState &c){
    j = json{
        {"type", CommandType::CMD_SEQUENCER_STATE},
        {"state", c.state}
    };
}

inline void to_json(json &j, const CmdSequencerMute &c){
    j = json{
        {"type", CommandType::CMD_SEQUENCER_MUTE},
        {"mute", c.mute}
    };
}

inline void to_json(json &j, const CmdPlayerRecord &c){
    j = json{
        {"type", CommandType::CMD_RECORDER_RECORD},
        {"state", c.state}
    };
}

inline void to_json(json &j, const CmdPlayerPlay &c){
    j = json{
        {"type", CommandType::CMD_RECORDER_PLAY},
        {"state", c.state},
        {"file", c.file}
    };
}

inline void to_json(json &j, const CmdTuner &c){
    j = json{
        {"type", CommandType::CMD_TUNER},
        {"state", c.state}
    };
}

using Cmd = std::variant<
    CmdLooperListLoops,
    CmdLooperAddLoop,
    CmdLooperRemoveLoop,
    CmdLooperSelectLoop,
    CmdModSelectPedalboard,
    CmdModEffectParamSet,
    CmdModListPedalboards,
    CmdModSelectPedalboardSnapshot,
    CmdSequencerBpm,
    CmdSequencerVolume,
    CmdSequencerSelectMidiFile,
    CmdSequencerListMidiFiles,
    CmdSequencerState,
    CmdSequencerMute,
    CmdTuner,
    CmdPlayerRecord,
    CmdPlayerPlay
>;

Event decodeEvent(std::string &data);
std::string encodeCmd(const Cmd &command);

#endif