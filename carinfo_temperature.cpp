/*
 * Copyright (c) 2019
 * All Rights Reserved by Thundersoft Software Technology Co., Ltd and its affiliates.
 * You may not use, copy, distribute, modify, transimit in any form of this file
 * except in compliance with THUNDERSOFT in writing byfg applicable law.
 */

#include "carinfo_temperature.h"
#include <math.h>
#include <string>
#include <iomanip>
#include <sstream>
#include "cluster_kanzi_app_def.h"
#include "ui_node_manager.h"
#include "cluster_config.h"



/**
 * @brief get Temperature singleton
 */
CarInfoTemperature &CarInfoTemperature::Singleton() {
    static CarInfoTemperature instance;
    return instance;
}

/**
 * @brief Temperature constructor
 */
CarInfoTemperature::CarInfoTemperature() :
    Component(MODULE_CARINFO_OAT) {}

/**
 * @brief init Temperature with ComponentManager
 *
 * @param[in] cm ComponentManager
 */
void CarInfoTemperature::Init(ComponentManager* cm ) {
    cm->registerComponent(MESSAGE_OAT, this);
}
/**
 * @brief process temperature message
 *
 * @param[in] categoryId category id
 *
 * @param[in]
 */
void CarInfoTemperature::process(Message_Group_t message_id, const void* msg) {
    switch (message_id) {
        case MESSAGE_OAT:
            TempHandler(msg);
            break;

        default:
            break;
    }
}

void CarInfoTemperature::TempHandler(const void* msg) {
    if (!msg) {
        return;
    }
    MsgCarInfoOutsideTemp_t* temp = static_cast<MsgCarInfoOutsideTemp_t*> (const_cast<void*>(msg));
    uint8_t temperatureType = temp->temperatureType;
    ui::UiNodeManager::singleton().setProperty(KANZI_MODULE_BASICINFO,
                                               KANZI_PROPERTY_TOP_TEMPERATURESTYLE,
                                               std::to_string(temperatureType));
    temperature_Info_ = temp->nOutsideTemp;
    if (0 == temperatureType) {
        if (temperature_Info_ <-40) {
            ui::UiNodeManager::singleton().setProperty(KANZI_MODULE_BASICINFO,
                                               KANZI_PROPERTY_TOP_TEMPERATURE,
                                               std::to_string(-40));
        } else if (temperature_Info_ >= -40 && temperature_Info_ <= 87) {
            std::string temperature = std::to_string(static_cast<int32_t>(temperature_Info_));
            ui::UiNodeManager::singleton().setProperty(KANZI_MODULE_BASICINFO,
                                               KANZI_PROPERTY_TOP_TEMPERATURE,
                                               temperature);
        } else if (temperature_Info_ > 87) {
            ui::UiNodeManager::singleton().setProperty(KANZI_MODULE_BASICINFO,
                                               KANZI_PROPERTY_TOP_TEMPERATURE,
                                               std::to_string(87));
        } else {
            // data exception
        }
    }
    if (1 == temperatureType) {
        if (temperature_Info_ <-40) {
            ui::UiNodeManager::singleton().setProperty(KANZI_MODULE_BASICINFO,
                                               KANZI_PROPERTY_TOP_TEMPERATURE,
                                               std::to_string(-40));
        } else if (temperature_Info_ >= -40 && temperature_Info_ <= 189) {
            std::string temperature = std::to_string(static_cast<int32_t>(temperature_Info_));
            ui::UiNodeManager::singleton().setProperty(KANZI_MODULE_BASICINFO,
                                               KANZI_PROPERTY_TOP_TEMPERATURE,
                                               temperature);
        } else if (temperature_Info_ > 189) {
            ui::UiNodeManager::singleton().setProperty(KANZI_MODULE_BASICINFO,
                                               KANZI_PROPERTY_TOP_TEMPERATURE,
                                               std::to_string(189));
        } else {
            // data exception
        }
    }
}
