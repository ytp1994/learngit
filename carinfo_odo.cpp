/*
 * Copyright (c) 2019
 * All Rights Reserved by Thundersoft Software Technology Co., Ltd and its affiliates.
 * You may not use, copy, distribute, modify, transimit in any form of this file
 * except in compliance with THUNDERSOFT in writing by applicable law.
 */

#include <string.h>

#include "carinfo_odo.h"
#include "cluster_config.h"
#include "cluster_kanzi_app_def.h"
#include "ui_node_manager.h"

/**
 * @brief get Basic singleton
 */
CarInfoOdo& CarInfoOdo::Singleton() {
    static CarInfoOdo ins;
    return ins;
}

/**
 * @brief Basic constructor
 */
CarInfoOdo::CarInfoOdo() : Component(MODULE_CARINFO_ODO) {}

/**
 * @brief init Basic with ComponentManager
 *
 * @param[in] cm ComponentManager
 */
void CarInfoOdo::Init(ComponentManager* cm) {
    cm->registerComponent(MESSAGE_ODO, this);
}

/**
 * @brief process basic message
 *
 * @param[in] categoryId category id
 * @param[in] msg struct pomp_msg message
 */
void CarInfoOdo::process(Message_Group_t message_id, const void* msg) {
    switch (message_id) {
    case MESSAGE_ODO:
        OdoHandler(msg);
        break;

    default:
        break;
    }
}

void CarInfoOdo::OdoHandler(const void* msg) {
    if (!msg) {
        return;
    }

    MsgCarInfoTotalMileage* p = static_cast<MsgCarInfoTotalMileage*>(const_cast<void*>(msg));
    odo_info_ = static_cast<int32_t>(p->nTotalMileage);
    std::string s_odo_info{""};
    if (odo_info_ < ODO_MIN) {
        s_odo_info = ODO_ERR;
    } else if (odo_info_ <= ODO_MAX) {
        s_odo_info = std::to_string(odo_info_);
    } else {
        s_odo_info = std::to_string(ODO_MAX);
    }

    ui::UiNodeManager::singleton().setProperty(KANZI_MODULE_BASICINFO,
                                               KANZI_PROPERTY_BOTTOM_ODO,
                                               s_odo_info);
}
