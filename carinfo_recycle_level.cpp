/*
 * Copyright (c) 2019
 * All Rights Reserved by Thundersoft Software Technology Co., Ltd and its
 * affiliates. You may not use, copy, distribute, modify, transimit in any form
 * of this file except in compliance with THUNDERSOFT in writing by applicable
 * law.
 */

#include "carinfo_recycle_level.h"
#include "cluster_config.h"
#include "cluster_kanzi_app_def.h"
#include "ui_node_manager.h"

/**
 * @brief get CarInfoRecycleLevel singleton
 */
CarInfoRecycleLevel& CarInfoRecycleLevel::Singleton() {
    static CarInfoRecycleLevel instance;
    return instance;
}

/**
 * @brief CarInfoRecycleLevel constructor
 */
CarInfoRecycleLevel::CarInfoRecycleLevel() :
    Component(MODULE_CARINFO_RECYCLELEVEL) {}

/**
 * @brief init CarInfoRecycleLevel with Componentmanager
 *
 * @param[in] cm ComponentManager
 */
void CarInfoRecycleLevel::Init(ComponentManager* cm) {
    cm->registerComponent(MESSAGE_RECYCLELEVEL, this);
}

/**
 * @breif process CarInfoRecycleLevel message
 *
 * @param[in] categoryId category id
 * @param[in] msg struct pomp_msg message
 */
void CarInfoRecycleLevel::process(Message_Group_t message_id, const void* msg) {
    switch (message_id) {
        case MESSAGE_RECYCLELEVEL:
            RecycleLevelHandler(msg);
            break;

        default:
            break;
    }
}

void CarInfoRecycleLevel::RecycleLevelHandler(const void* msg) {
    if (!msg) {
        return;
    }

    MsgCarInfoPowRecLev* p = static_cast<MsgCarInfoPowRecLev*>(const_cast<void*>(msg));
    switch (p->nRecycleLevel) {
        case RecycleLevel_e::RecycleLevel_Low:
            recycle_level_info_ = KANZI_LEVEL_LOW;
            break;
        case RecycleLevel_e::RecycleLevel_Middle:
            recycle_level_info_ = KANZI_LEVEL_MIDDLE;
            break;
        case RecycleLevel_e::RecycleLevel_High:
            recycle_level_info_ = KANZI_LEVEL_HIGH;
            break;
        default:
            recycle_level_info_ = KANZI_LEVEL_DEFAULT_OR_ERR;
            break;
    }
    ui::UiNodeManager::singleton().setProperty(KANZI_MODULE_BASICINFO,
                                               KANZI_PROPERTY_BOTTOM_RECYCLE,
                                               static_cast<int32_t>(recycle_level_info_));
}
