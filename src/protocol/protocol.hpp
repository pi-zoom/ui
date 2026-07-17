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
    EVENT_LOOP_COUNT,
    EVENT_LOOP_POS,
    EVENT_LOOP_LEN,
    EVENT_LOOP_STATE,
    EVENT_LOOP_SELECTED,
    EVENT_PEDALBOARDS_LIST,
    EVENT_PEDALBOARD_CHANGED,
    EVENT_EFFECT_PARAM_CHANGED,
    EVENT_PEDALBOARD_SNAPSHOT_CHANGED,
    EVENT_LOOP_LIST,
    EVENT_SEQUENCER_MIDI_FILES_LIST,
    EVENT_SEQUENCER_POSITION
};

struct EventLoopCount {
    int count;
};

struct EventLoopPos {
    int id;
    double pos;
};

struct EventLoopState {
    int id;
    int state;
};

struct EventLoopLen {
    int id;
    double len;
};

struct EventLoopList {
    std::vector<Loop> loops;
};

struct EventLoopSelected {
    int id;
};

struct EventPedalboardsList {
    std::vector<std::string> pedalboards;
};

struct EventPedalboardChanged {
    Pedalboard pedalboard;
};

struct EventEffectParamChanged {
    std::string instance_id;
    std::string symbol;
    float value;
};

struct EventPedalboardSnapshotChanged {
    int index;
    std::string name;
};

struct EventSequencerMidiFilesList {
    std::vector<std::string> midiFiles;
};

struct EventSequencerPosition {
    double pos;
};

using Event = std::variant<
    EventLoopCount,
    EventLoopPos,
    EventLoopState,
    EventLoopLen,
    EventLoopList,
    EventLoopSelected,
    EventPedalboardsList,
    EventPedalboardChanged,
    EventEffectParamChanged,
    EventPedalboardSnapshotChanged,
    EventSequencerMidiFilesList,
    EventSequencerPosition
>;

enum class CommandType {
    CMD_ADD_LOOP,
    CMD_REMOVE_LOOP,
    CMD_SELECT_LOOP,
    CMD_SELECT_PEDALBOARD,
    CMD_EFFECT_PARAM_SET,
    CMD_LIST_PEDALBOARDS,
    CMD_SELECT_PEDALBOARD_SNAPSHOT,
    CMD_LIST_LOOPS,
    CMD_SEQUENCER_BPM,
    CMD_SEQUENCER_VOLUME,
    CMD_SEQUENCER_SELECT_MIDI_FILE,
    CMD_SEQUENCER_LIST_MIDI_FILES
};

struct CmdListLoops {

};

struct CmdAddLoop {

};

struct CmdRemoveLoop {

};

struct CmdSelectLoop {
    int id;
};

struct CmdSelectPedalboard {
    std::string name;
};

struct CmdSetEffectParam {
    std::string instande_id;
    std::string symbol;
    float value;
};

struct CmdListPedalboards {

};

struct CmdSelectPedalboardSnapshot {
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

inline void to_json(json &j, const CmdListLoops &c)
{
    j = json{
        {"type", CommandType::CMD_LIST_LOOPS}};
}

inline void to_json(json &j, const CmdAddLoop &c)
{
    j = json{
        {"type", CommandType::CMD_ADD_LOOP}};
}

inline void to_json(json &j, const CmdRemoveLoop &c)
{
    j = json{
        {"type", CommandType::CMD_REMOVE_LOOP}};
}

inline void to_json(json &j, const CmdSelectLoop &c)
{
    j = json{
        {"type", CommandType::CMD_SELECT_LOOP},
        {"id", c.id}};
}

inline void to_json(json &j, const CmdSelectPedalboard &c)
{
    j = json{
        {"type", CommandType::CMD_SELECT_PEDALBOARD},
        {"name", c.name}};
}

inline void to_json(json &j, const CmdSetEffectParam &c)
{
    j = json{
        {"type", CommandType::CMD_EFFECT_PARAM_SET},
        {"instance_id", c.instande_id},
        {"symbol", c.symbol},
        {"value", c.value}};
}

inline void to_json(json &j, const CmdListPedalboards &c)
{
    j = json{
        {"type", CommandType::CMD_LIST_PEDALBOARDS}};
}

inline void to_json(json &j, const CmdSelectPedalboardSnapshot &c)
{
    j = json{
        {"type", CommandType::CMD_SELECT_PEDALBOARD_SNAPSHOT},
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
using Cmd = std::variant<
    CmdListLoops,
    CmdAddLoop,
    CmdRemoveLoop,
    CmdSelectLoop,
    CmdSelectPedalboard,
    CmdSetEffectParam,
    CmdListPedalboards,
    CmdSelectPedalboardSnapshot,
    CmdSequencerBpm,
    CmdSequencerVolume,
    CmdSequencerSelectMidiFile,
    CmdSequencerListMidiFiles
>;

Event decodeEvent(std::string &data);
std::string encodeCmd(const Cmd &command);

#endif