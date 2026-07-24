#include <memory>
#include <iostream>
#include "mod.hpp"
#include "ui/ui.h"
#include "transport/zmq_transport.hpp"
#include "protocol/protocol.hpp"

static std::unique_ptr<Pedalboard> currentPedalboard = nullptr;

static lv_obj_t *pedalboardsDropdown = nullptr;
static lv_obj_t *bankDropdown = nullptr;
static lv_obj_t *snapshotsDropdown = nullptr;
static lv_obj_t *effectsContainer = nullptr;

std::string formatFloatParameter(float value, std::string unit)
{
    char buf[32];
    std::snprintf(buf, sizeof(buf), "%.2f", value);
    return std::string(buf) + " " + unit;
}

void plugin_bypass_changed(lv_event_t *e)
{
    lv_obj_t *button = (lv_obj_t *)lv_event_get_target_obj(e);
    PluginParameter *param = (PluginParameter *)lv_event_get_user_data(e);
    float value = param->currentValue;

    if (value == 0.0)
    {
        value = 1.0;
        lv_obj_set_style_bg_color(button, lv_color_hex(0xbd0b1d), LV_PART_MAIN);
    }
    else
    {
        value = 0.0;
        lv_obj_set_style_bg_color(button, lv_color_hex(0x0edc27), LV_PART_MAIN);
    }
    param->currentValue = value;

    transport.sendCommand(CmdModEffectParamSet{.instande_id = param->plugin_instance_id, .symbol = param->symbol, .value = param->currentValue});
}

void plugin_parameter_changed(lv_event_t *e)
{
    PluginParameter *param = (PluginParameter *)lv_event_get_user_data(e);
    float value = 0.0;
    switch (param->type)
    {
    case PluginParameterType::DEFAULT:
        value = lv_slider_get_value(param->slider) / 100.0;
        param->currentValue = value;
        lv_label_set_text(param->display, formatFloatParameter(value, param->units.symbol).c_str());
        break;

    case PluginParameterType::ENUMERATION:
        value = lv_dropdown_get_selected(param->drop);
        param->currentValue = value;
    default:
        break;
    }

    transport.sendCommand(CmdModEffectParamSet{.instande_id = param->plugin_instance_id, .symbol = param->symbol, .value = param->currentValue});
}

void pedalboard_changed(lv_event_t *e)
{
    lv_obj_t *obj = (lv_obj_t *)lv_event_get_target(e);
    char buf[100];
    lv_dropdown_get_selected_str(pedalboardsDropdown, buf, sizeof(buf));

    if(currentPedalboard == nullptr)
        return;

    if (currentPedalboard->name != std::string(buf))
    {
        transport.sendCommand(CmdModSelectPedalboard{.name = std::string(buf)});
    }
}

void pedalboard_snapshot_changed(lv_event_t *e)
{
    lv_obj_t *obj = (lv_obj_t *)lv_event_get_target(e);
    uint32_t selected = lv_dropdown_get_selected(snapshotsDropdown);
    if(selected != currentPedalboard->snapshot_index){
        currentPedalboard->snapshot_index = selected;
        transport.sendCommand(CmdModSelectPedalboardSnapshot{.index=(int)selected});
    }
}

PluginParameterType plugin_parameter_get_type(const PluginParameter &param){

    if(std::find(param.properties.begin(), param.properties.end(), std::string("enumeration")) != param.properties.end())
        return PluginParameterType::ENUMERATION;
    else if(std::find(param.properties.begin(), param.properties.end(), std::string("integer")) != param.properties.end())
        return PluginParameterType::INTEGER;
    else if(std::find(param.properties.begin(), param.properties.end(), std::string("logarithmic")) != param.properties.end())
        return PluginParameterType::LOGARITHMIC;
    else if(std::find(param.properties.begin(), param.properties.end(), std::string("tapTempo")) != param.properties.end())
        return PluginParameterType::TAPTEMPO;
    else if(std::find(param.properties.begin(), param.properties.end(), std::string("toggled")) != param.properties.end())
        return PluginParameterType::TOGGLED;
    else
        return PluginParameterType::DEFAULT;
}

void lv_plugin_parameter_delete(Plugin *plugin){
    for(PluginParameter &param: plugin->parameters){
        param.container = nullptr;
        param.slider = nullptr;
        param.drop = nullptr;
        param.display = nullptr;
        param.label = nullptr;
        param.bypass_button = nullptr;
    }
}

void lv_plugin_parameter_create(Plugin &plugin, lv_obj_t *overlay)
{
    for (PluginParameter &param : plugin.parameters)
    {
        // parameter_container
        param.container = lv_obj_create(overlay);
        lv_obj_set_style_pad_left(param.container, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_top(param.container, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_right(param.container, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_bottom(param.container, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(param.container, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_width(param.container, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_radius(param.container, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_layout(param.container, LV_LAYOUT_FLEX, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_flex_flow(param.container, LV_FLEX_FLOW_COLUMN, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_flex_main_place(param.container, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_flex_cross_place(param.container, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_flex_track_place(param.container, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_height(param.container, LV_PCT(100), LV_PART_MAIN);

        if (param.symbol == ":bypass")
        {
            param.bypass_button = lv_button_create(param.container);
            lv_obj_t *obj = lv_label_create(param.bypass_button);
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "Bypass");
            lv_obj_add_event_cb(param.bypass_button, plugin_bypass_changed, LV_EVENT_CLICKED, (void*)&param);
            if(param.currentValue == 0.0){
                lv_obj_set_style_bg_color(param.bypass_button, lv_color_hex(0x0edc27), LV_PART_MAIN);
            }else{
                lv_obj_set_style_bg_color(param.bypass_button, lv_color_hex(0xbd0b1d), LV_PART_MAIN);
            }
            continue;
        }

        switch(param.type){
            case PluginParameterType::DEFAULT:
                // parameter_label
                param.label = lv_label_create(param.container);
                lv_obj_set_size(param.label, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                lv_label_set_text_static(param.label, param.name.c_str());
                lv_obj_set_style_text_align(param.label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);

                // parameter_slider
                param.slider = lv_slider_create(param.container);
                lv_obj_set_size(param.slider, 19, 163);
                lv_slider_set_range(param.slider, (int32_t)(param.ranges.minimum * 100.0), (int32_t)(param.ranges.maximum * 100.0));
                lv_slider_set_value(param.slider, (int32_t)(param.currentValue * 100.0), LV_ANIM_OFF);
                lv_obj_add_event_cb(param.slider, plugin_parameter_changed, LV_EVENT_VALUE_CHANGED, (void *)&param);
                lv_obj_set_style_margin_bottom(param.slider, 20, LV_PART_MAIN);
                lv_obj_set_style_margin_top(param.slider, 20, LV_PART_MAIN);

                // display label
                param.display = lv_label_create(param.container);
                lv_obj_set_size(param.display, LV_PCT(100), LV_PCT(10));
                lv_obj_set_style_text_align(param.display, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_label_set_text(param.display, formatFloatParameter(param.currentValue, param.units.symbol).c_str());
                break;

            case PluginParameterType::ENUMERATION:
                param.label = lv_label_create(param.container);
                lv_obj_set_size(param.label, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                lv_label_set_text_static(param.label, param.name.c_str());
                lv_obj_set_style_text_align(param.label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
                param.drop = lv_dropdown_create(param.container);
                lv_dropdown_clear_options(param.drop);

                for(const ParamScalePoints &scalePoint: param.scalePoints){
                    lv_dropdown_add_option(param.drop, scalePoint.label.c_str(), LV_DROPDOWN_POS_LAST);
                }
                lv_obj_add_event_cb(param.drop, plugin_parameter_changed, LV_EVENT_VALUE_CHANGED, (void *)&param);
                break;
            default:
                break;
        }
    }
}

void lv_plugin_overlay_create(lv_event_t *e)
{
    Plugin *plugin = (Plugin *)lv_event_get_user_data(e);
    if(currentPedalboard == nullptr)
        return;

    if(plugin == nullptr)
        return;

    try
    {
        /* -----------------------------
         * Modal overlay (background)
         * ----------------------------- */
        if (currentPedalboard == nullptr)
        {
            return;
        }

        currentPedalboard->effectParamEditOverlay = lv_obj_create(lv_screen_active());
        lv_obj_set_size(currentPedalboard->effectParamEditOverlay, LV_PCT(100), LV_PCT(100));
        lv_obj_set_style_bg_color(currentPedalboard->effectParamEditOverlay, lv_color_black(), 0);
        lv_obj_set_style_bg_opa(currentPedalboard->effectParamEditOverlay, LV_OPA_60, 0);
        lv_obj_set_scrollable(currentPedalboard->effectParamEditOverlay, false);

        /* -----------------------------
         * Main window panel
         * ----------------------------- */
        lv_obj_t *panel = lv_obj_create(currentPedalboard->effectParamEditOverlay);
        lv_obj_set_size(panel, LV_PCT(90), LV_PCT(90));
        lv_obj_center(panel);

        lv_obj_set_style_bg_color(panel, lv_color_hex(0x1e1e1e), 0);
        lv_obj_set_style_radius(panel, 10, 0);
        lv_obj_set_style_pad_all(panel, 10, 0);
        lv_obj_set_scroll_dir(panel, LV_DIR_VER);

        /* -----------------------------
         * Title
         * ----------------------------- */
        lv_obj_t *title = lv_label_create(panel);

        lv_label_set_text(title, lv_label_get_text(plugin->label));

        lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 5);

        /* -----------------------------
         * Content container (scrollable)
         * ----------------------------- */
        lv_obj_t *content = lv_obj_create(panel);
        lv_obj_set_size(content, LV_PCT(100), LV_PCT(80));
        lv_obj_align_to(content, title, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);

        lv_obj_set_flex_flow(content, LV_FLEX_FLOW_ROW);
        lv_obj_set_scroll_dir(content, LV_DIR_VER);
        lv_obj_set_style_pad_all(content, 5, 0);
        lv_obj_set_scroll_dir(content, LV_DIR_HOR);

        /* -----------------------------
         * Parameters UI
         * ----------------------------- */
        lv_plugin_parameter_create(*plugin, content);

        /* -----------------------------
         * Close button
         * ----------------------------- */
        lv_obj_t *close_btn = lv_btn_create(panel);
        lv_obj_align(close_btn, LV_ALIGN_BOTTOM_RIGHT, -10, -10);

        lv_obj_t *close_lbl = lv_label_create(close_btn);
        lv_label_set_text(close_lbl, "Close");

        lv_obj_add_event_cb(close_btn, [](lv_event_t *e)
            {
                lv_obj_delete(currentPedalboard->effectParamEditOverlay);
                currentPedalboard->effectParamEditOverlay = nullptr;
                Plugin *plugin = (Plugin*)lv_event_get_user_data(e);
                lv_plugin_parameter_delete(plugin);

            }, LV_EVENT_CLICKED, (void*)plugin);

        /* -----------------------------
         * Optional: click outside closes
         * ----------------------------- */
        lv_obj_add_event_cb(currentPedalboard->effectParamEditOverlay, [](lv_event_t *e)
            {
                lv_obj_delete_async(currentPedalboard->effectParamEditOverlay);
                currentPedalboard->effectParamEditOverlay = nullptr;
                Plugin *plugin = (Plugin*)lv_event_get_user_data(e);
                lv_plugin_parameter_delete(plugin);
            }, LV_EVENT_CLICKED, (void*)plugin);
    }
    catch (const std::exception &e)
    {
        std::cerr << "plugin_edit exception: " << e.what() << "\n";
    }
}

void lv_plugin_create(Plugin &plugin)
{
    plugin.container = lv_obj_create(effectsContainer);
    lv_obj_set_size(plugin.container, 300, 100);

    // use someting else to store
    lv_color_t color;
    for(const std::string &category: plugin.category){
        if(category == "Delay")
            color = lv_color_hex(0x32a852);
        else if(category == "Distortion")
            color = lv_color_hex(0xa84432);
        else if(category == "Dynamics")
            color = lv_color_hex(0xccc129);
        else if(category == "Filter")
            color = lv_color_hex(0x29cc70);
        else if(category == "Generator")
            color = lv_color_hex(0x18dbce);
        else if(category == "Midiutility")
            color = lv_color_hex(0x10a7e3);
        else if(category == "Modulator")
            color = lv_color_hex(0x0b3fb0);
        else if(category == "Reverb")
            color = lv_color_hex(0x670fd9);
        else if(category == "Simulator")
            color = lv_color_hex(0xbe09de);
        else if(category == "Spacial")
            color = lv_color_hex(0xed0c88);
        else if(category == "Spectral")
            color = lv_color_hex(0xd90939);
        else if(category == "Utility")
            color = lv_color_hex(0xf7020f);
        else
            color = lv_color_hex(0x0d5bdd);

    }
    lv_obj_set_style_bg_color(plugin.container, color, LV_PART_MAIN | LV_STATE_DEFAULT);

    //bypass
    if(!plugin.bypassed){
        lv_obj_set_style_bg_opa(plugin.container, 255, LV_PART_MAIN);
    }else{
        lv_obj_set_style_bg_opa(plugin.container, 10, LV_PART_MAIN);
    }

    // plugin_label
    plugin.label = lv_label_create(plugin.container);
    lv_label_set_text(plugin.label, plugin.name.c_str());
    lv_obj_center(plugin.label);

    lv_obj_add_event_cb(plugin.container, lv_plugin_overlay_create, LV_EVENT_DOUBLE_CLICKED, (void *)&plugin);
}

void lv_pedalboard_create(){
    if(currentPedalboard == nullptr)
        return;

    // set pedalboard dropdown
    int32_t index = lv_dropdown_get_option_index(pedalboardsDropdown, currentPedalboard->name.c_str());
    if (index < 0)
    {
        std::cout << "Unable to get index for pedalboard: " << currentPedalboard->name << std::endl;
        return;
    }
    lv_dropdown_set_selected(pedalboardsDropdown, index);

    // set snapshots dropdown
    lv_dropdown_clear_options(snapshotsDropdown);
    for (const Snapshot &snapshot : currentPedalboard->snapshots)
    {
        lv_dropdown_add_option(snapshotsDropdown, snapshot.name.c_str(), LV_DROPDOWN_POS_LAST);
    }
    lv_dropdown_set_selected(snapshotsDropdown, (uint32_t)(currentPedalboard->snapshot_index));

    for (Plugin &plugin : currentPedalboard->plugins)
    {
        lv_plugin_create(plugin);
    }

    return;
}

void lv_update_plugin_parameter(Plugin &plugin, PluginParameter &param){
    if(param.symbol == ":bypass"){
        plugin.bypassed = param.currentValue;
        if(param.bypass_button != nullptr){
            if(!plugin.bypassed){
                lv_obj_set_style_bg_color(param.bypass_button, lv_color_hex(0x0edc27), LV_PART_MAIN);
            }else{
                lv_obj_set_style_bg_color(param.bypass_button, lv_color_hex(0xbd0b1d), LV_PART_MAIN);
            }
        }

        if(plugin.container != nullptr){
            if(!plugin.bypassed){
                lv_obj_set_style_bg_opa(plugin.container, 255, LV_PART_MAIN);
            }else{
                lv_obj_set_style_bg_opa(plugin.container, 10, LV_PART_MAIN);
            }
        }
    }else{
        if(param.slider != nullptr){
            lv_slider_set_value(param.slider, (int32_t)(param.currentValue * 100.0), LV_ANIM_OFF);
            lv_label_set_text(param.display, formatFloatParameter(param.currentValue, param.units.symbol).c_str());
        }else if(param.drop != nullptr){
            lv_dropdown_set_selected(param.drop, (uint32_t)(param.currentValue));
        }
    }
}

void lv_mod_create(lv_obj_t *parent){

    lv_obj_set_flex_flow(parent, LV_FLEX_FLOW_COLUMN);

    // mod_control
    lv_obj_t *controlContainer = lv_obj_create(parent);
    lv_obj_set_size(controlContainer, LV_PCT(100), LV_PCT(20));
    lv_obj_set_style_pad_left(controlContainer, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(controlContainer, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(controlContainer, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(controlContainer, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(controlContainer, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_scrollable(controlContainer, false);

    static lv_coord_t controlContainerDsc[] = {0, LV_GRID_TEMPLATE_LAST};
    lv_obj_set_style_grid_row_dsc_array(controlContainer, controlContainerDsc, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_bg_color(controlContainer, lv_color_hex(0x9a9fc4), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(controlContainer, 100, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(controlContainer, lv_color_hex(0x3148bb), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(controlContainer, 100, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(controlContainer, 2, LV_PART_MAIN | LV_STATE_DEFAULT);

    // banks
    bankDropdown = lv_dropdown_create(controlContainer);
    lv_obj_set_pos(bankDropdown, 9, 52);
    lv_obj_set_size(bankDropdown, 197, LV_SIZE_CONTENT);
    lv_dropdown_set_options_static(bankDropdown, "");
    lv_dropdown_set_selected(bankDropdown, 0);

    // banks_label
    lv_obj_t *bankLabel = lv_label_create(controlContainer);
    lv_obj_set_pos(bankLabel, 9, 19);
    lv_obj_set_size(bankLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_label_set_text_static(bankLabel, "Banks");

    // pedalboards
    pedalboardsDropdown = lv_dropdown_create(controlContainer);
    lv_obj_set_pos(pedalboardsDropdown, 224, 52);
    lv_obj_set_size(pedalboardsDropdown, 367, LV_SIZE_CONTENT);
    lv_dropdown_set_options_static(pedalboardsDropdown, "");
    lv_dropdown_set_selected(pedalboardsDropdown, 0);

    // pedalboards_label
    lv_obj_t *pedalboardsLabel = lv_label_create(controlContainer);
    lv_obj_set_pos(pedalboardsLabel, 224, 19);
    lv_obj_set_size(pedalboardsLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_label_set_text_static(pedalboardsLabel, "Pedalboards");

    // snapshots
    snapshotsDropdown = lv_dropdown_create(controlContainer);
    lv_obj_set_pos(snapshotsDropdown, 609, 52);
    lv_obj_set_size(snapshotsDropdown, 298, LV_SIZE_CONTENT);
    lv_dropdown_set_options_static(snapshotsDropdown, "");
    lv_dropdown_set_selected(snapshotsDropdown, 0);

    // snapshots_label
    lv_obj_t *snapshotsLabel = lv_label_create(controlContainer);
    lv_obj_set_pos(snapshotsLabel, 609, 19);
    lv_obj_set_size(snapshotsLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_label_set_text_static(snapshotsLabel, "Snapshots");

    // effects_container
    effectsContainer = lv_obj_create(parent);
    lv_obj_set_size(effectsContainer, LV_PCT(100), LV_PCT(80));
    lv_obj_set_style_pad_left(effectsContainer, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(effectsContainer, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(effectsContainer, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(effectsContainer, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(effectsContainer, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_scrollable(effectsContainer, false);
    lv_obj_set_style_border_color(effectsContainer, lv_color_hex(0x4b66c4), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(effectsContainer, 100, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(effectsContainer, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(effectsContainer, lv_color_hex(0xafabd6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(effectsContainer, 100, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_flex_flow(effectsContainer, LV_FLEX_FLOW_ROW_WRAP, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_layout(effectsContainer, LV_LAYOUT_FLEX, LV_PART_MAIN | LV_STATE_DEFAULT);

    static lv_coord_t effectsContainerDsc[] = {0, LV_GRID_TEMPLATE_LAST};
    lv_obj_set_style_grid_row_dsc_array(effectsContainer, effectsContainerDsc, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_flex_main_place(effectsContainer, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_flex_track_place(effectsContainer, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_add_event_cb(pedalboardsDropdown, pedalboard_changed, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_event_cb(snapshotsDropdown, pedalboard_snapshot_changed, LV_EVENT_VALUE_CHANGED, NULL);
}

void mod_set_current_pedalboard(Pedalboard pedalboard){
    if(currentPedalboard)
        currentPedalboard.reset();

    if(effectsContainer)
        lv_obj_clean(effectsContainer);

    currentPedalboard = std::make_unique<Pedalboard>(pedalboard);
    lv_pedalboard_create();
}

void mod_set_plugin_parameter(std::string instance_id, std::string symbol, float value){
    if (currentPedalboard == nullptr)
        return;

    for(Plugin &plugin: currentPedalboard->plugins){
        if(plugin.instance_id == instance_id){
            for(PluginParameter &param: plugin.parameters){
                if(param.symbol == symbol){
                    param.currentValue = value;

                    lv_update_plugin_parameter(plugin, param);
                    break;
                }
            }
        }
    }
}

void mod_set_current_snapshot(int index, std::string name){
    if (currentPedalboard == nullptr)
        return;
    if(index >= currentPedalboard->snapshots.size())
        return;

    currentPedalboard->snapshot_index = index;
    lv_dropdown_set_selected(snapshotsDropdown, (uint32_t)(currentPedalboard->snapshot_index));
}

void mod_set_pedalboard_list(std::vector<std::string> pedalboards){
    for (auto pedal : pedalboards)
    {
        lv_dropdown_add_option(pedalboardsDropdown, pedal.c_str(), LV_DROPDOWN_POS_LAST);
    }
}