#include "protocol.hpp"
#include <iostream>
using json = nlohmann::json;

std::string encodeCmd(const Cmd &command)
{
    return std::visit([](auto &&c)
                      {
        nlohmann::json j = c;
        return j.dump(); }, command);
}

Event decodeEvent(std::string &data)
{
    try
    {
        // Parse JSON
        nlohmann::json j = nlohmann::json::parse(data);
        EventType eventType = j.at("type").get<EventType>();
        Event event;

        switch (eventType)
        {
        case EventType::EVENT_LOOP_COUNT:
        {
            event = EventLoopCount{.count = j["count"].get<int>()};
            break;
        }

        case EventType::EVENT_LOOP_POS:
        {
            event = EventLoopPos{.id = j["id"].get<int>(), .pos = j["pos"].get<double>()};
            break;
        }

        case EventType::EVENT_LOOP_STATE:
        {
            event = EventLoopState{.id = j["id"].get<int>(), .state = j["state"].get<int>()};
            break;
        }
        case EventType::EVENT_LOOP_LEN:
        {
            event = EventLoopLen{.id = j["id"].get<int>(), .len = j["len"].get<double>()};
            break;
        }
        case EventType::EVENT_LOOP_SELECTED:
        {
            event = EventLoopSelected{.id = j["id"].get<int>()};
            break;
        }
        case EventType::EVENT_PEDALBOARDS_LIST:
        {
            event = EventPedalboardsList{.pedalboards = j["pedalboards"].get<std::vector<std::string>>()};
            break;
        }
        case EventType::EVENT_PEDALBOARD_CHANGED:
        {
            nlohmann::json jsonPedalboard = j["pedalboard"];
            Pedalboard pedalboard;
            jsonPedalboard.at("title").get_to(pedalboard.name);
            jsonPedalboard.at("snapshots").get_to(pedalboard.snapshots);
            jsonPedalboard.at("snapshot_id").get_to(pedalboard.snapshot_index);

            for(const auto& p : jsonPedalboard.value("plugins", nlohmann::json::array())){
                Plugin plugin;
                p.at("name").get_to(plugin.name);
                p.at("instance_id").get_to(plugin.instance_id);
                p.at("category").get_to(plugin.category);
                plugin.bypassed = 0.0; // TODO: use bool

                for(const auto& param : p.value("params", nlohmann::json::array())){
                    PluginParameter parameter;
                    parameter.plugin_instance_id = plugin.instance_id;
                    param.at("name").get_to(parameter.name);
                    param.at("symbol").get_to(parameter.symbol);
                    param.at("properties").get_to(parameter.properties);
                    param.at("ranges").get_to(parameter.ranges);
                    param.at("units").get_to(parameter.units);
                    param.at("scalePoints").get_to(parameter.scalePoints);
                    parameter.currentValue = parameter.ranges.defaultValue;
                    parameter.type = plugin_parameter_get_type(parameter);
                    plugin.parameters.push_back(std::move(parameter));
                }
                pedalboard.plugins.push_back(std::move(plugin));
            }
            event = EventPedalboardChanged{.pedalboard = pedalboard};
            break;
        }
        case EventType::EVENT_EFFECT_PARAM_CHANGED:
        {
            event = EventEffectParamChanged{.instance_id = j["instance_id"].get<std::string>(), .symbol = j["symbol"].get<std::string>(), .value = j["value"].get<float>()};
            break;
        }
        case EventType::EVENT_PEDALBOARD_SNAPSHOT_CHANGED:
        {
            event = EventPedalboardSnapshotChanged{.index = j["index"].get<int>(), .name = j["name"].get<std::string>()};
            break;
        }
        case EventType::EVENT_LOOP_LIST:
        {
            std::vector<Loop> loops;
            j.at("loops").get_to(loops);
            event = EventLoopList{.loops = loops};
            break;
        }
        case EventType::EVENT_SEQUENCER_MIDI_FILES_LIST:
        {
            event = EventSequencerMidiFilesList{.midiFiles = j["files"].get<std::vector<std::string>>()};
            break;
        }
        case EventType::EVENT_SEQUENCER_POSITION:
        {
            event = EventSequencerPosition{.pos = j["pos"].get<double>()};
            break;
        }
        case EventType::EVENT_TUNER:
        {
            event = EventTuner{.note = j["note"].get<std::string>(), .cents = j["cents"].get<float>()};
            break;
        }
        case EventType::EVENT_RECORDED_FILE_LIST:
        {
            event = EventRecordedFileList{.files = j["files"].get<std::vector<std::string>>()};
            break;
        }
        default:
        {
            std::runtime_error("Unknown event type: " + static_cast<int>(eventType));
            break;
        }
        }

        return event;
    }
    catch (const nlohmann::json::parse_error &e)
    {
        throw;
    }
}
