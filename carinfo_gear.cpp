/*
 * Copyright (c) 2019
 * All Rights Reserved by Thundersoft Software Technology Co., Ltd and its affiliates.
 * You may not use, copy, distribute, modify, transimit in any form of this file
 * except in compliance with THUNDERSOFT in writing by applicable law.
 */

#include <string.h>
#include "carinfo_gear.h"
#include "cluster_kanzi_app_def.h"
#include "ui_node_manager.h"
#include "cluster_config.h"

/**
 * @brief get Basic singleton
 */
CarInfoGear &CarInfoGear::Singleton() {
    static CarInfoGear ins;
    return ins;
}

/**
 * @brief Basic constructor
 */
CarInfoGear::CarInfoGear() : Component(MODULE_CARINFO_GEAR) {
}

/**
 * @brief init Basic with ComponentManager
 *
 * @param[in] cm ComponentManager
 */
void CarInfoGear::Init(ComponentManager *cm) {
    cm->registerComponent(MESSAGE_GEAR, this);
}

/**
 * @brief process basic message
 *
 * @param[in] categoryId category id
 * @param[in] msg struct pomp_msg message
 */
void CarInfoGear::process(Message_Group_t message_id, const void *msg) {
    switch (message_id) {
        case MESSAGE_GEAR:
            GearHandler(msg);
            break;

        default:
            break;
    }
}

void CarInfoGear::GearHandler(const void* msg) {
    if (!msg) {
        return;
    }

    MsgCarInfoGear* p = static_cast<MsgCarInfoGear*> (const_cast<void*>(msg));

    switch (p->nGearPosition) {
        case Gear_e::GEAR_BLANK:
            gear_info_ = KANZI_GEAR_BLANK;
            break;
        case Gear_e::GEAR_P:
            gear_info_ = KANZI_GEAR_P;
            break;
        case Gear_e::GEAR_R:
            gear_info_ = KANZI_GEAR_R;
            break;
        case Gear_e::GEAR_N:
            gear_info_ = KANZI_GEAR_N;
            break;
        case Gear_e::GEAR_D:
            gear_info_ = KANZI_GEAR_D;
            break;
        case Gear_e::GEAR_EP:
            gear_info_ = KANZI_GEAR_EP;
            break;
        default:
            gear_info_ = KANZI_GEAR_BLANK;
            break;
    }
    ui::UiNodeManager::singleton().setProperty(KANZI_MODULE_BASICINFO,
                                               KANZI_PROPERTY_BOTTOM_GEAR,
                                               static_cast<int32_t>(gear_info_));
}
