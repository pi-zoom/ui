#ifndef MOD_HPP
#define MOD_HPP
#include <string>
#include <vector>
#include "lvgl/lvgl.h"

struct Snapshot
{
    int index;
    std::string name;
};

enum class PluginParameterType {
    DEFAULT,
    ENUMERATION,
    INTEGER,
    LOGARITHMIC,
    TAPTEMPO,
    TOGGLED,
};

struct ParamUnits
{
    std::string label;
    std::string render;
    std::string symbol;
    bool _custom{};
};

struct ParamScalePoints
{
    bool valid;
    float value;
    std::string label;
};

struct ParamRange
{
    float minimum;
    float maximum;
    float defaultValue;
};

struct PluginParameter
{
    std::string plugin_instance_id;
    std::string name;
    std::string symbol;
    ParamRange ranges;
    ParamUnits units;
    std::vector<ParamScalePoints> scalePoints;
    std::vector<std::string> properties;
    float currentValue;
    PluginParameterType type;

    lv_obj_t *container = nullptr;
    lv_obj_t *label = nullptr;
    lv_obj_t *slider = nullptr;
    lv_obj_t *display = nullptr;
    lv_obj_t *drop = nullptr;
    lv_obj_t *bypass_button = nullptr;
};

struct Plugin
{
    std::string name;
    std::string instance_id;
    std::vector<PluginParameter> parameters;
    std::vector<std::string> category;
    bool bypassed;

    lv_obj_t *container = nullptr;
    lv_obj_t *label = nullptr;
    lv_obj_t *bypass = nullptr;
};

struct Pedalboard
{
    ~Pedalboard()
    {
        if (effectParamEditOverlay)
        {
            lv_obj_delete_async(effectParamEditOverlay);
            effectParamEditOverlay = nullptr;
        }

        if (plugins_container)
        {
            lv_obj_delete_async(plugins_container);
            plugins_container = nullptr;
        }
    }
    std::string name;
    std::vector<Plugin> plugins;
    int snapshot_index;
    std::vector<Snapshot> snapshots;
    lv_obj_t *plugins_container = nullptr;
    lv_obj_t *effectParamEditOverlay = nullptr;
};

void set_current_pedalboard(Pedalboard pedalboard);
void set_current_snapshot(int index);
void update_plugin_parameter(std::string instance_id, std::string symbol, float value);
PluginParameterType plugin_parameter_get_type(const PluginParameter &param);

void pedalboard_changed(lv_event_t *e);
void pedalboard_snapshot_changed(lv_event_t *e);

#endif