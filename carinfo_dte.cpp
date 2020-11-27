/*
 * Copyright (c) 2019
 * All Rights Reserved by Thundersoft Software Technology Co., Ltd and its
 * affiliates. You may not use, copy, distribute, modify, transimit in any form
 * of this file except in compliance with THUNDERSOFT in writing by applicable
 * law.
 */

#include "carinfo_dte.h"
#include "cluster_config.h"
#include "cluster_kanzi_app_def.h"
#include "ui_node_manager.h"

/**
 * @brief get CarInfoDte singleton
 */
CarInfoDte& CarInfoDte::Singleton() {
    static CarInfoDte instance;
    return instance;
}

/**
 * @brief CarInfoDte constructor
 */
CarInfoDte::CarInfoDte() : Component(MODULE_CARINFO_DTE) {}

/**
 * @brief init CarInfoDte with Componentmanager
 *
 * @param[in] cm ComponentManager
 */
void CarInfoDte::Init(ComponentManager* cm) {
    cm->registerComponent(MESSAGE_DTE, this);
}

/**
 * @breif process CarInfoDte message
 *
 * @param[in] categoryId category id
 * @param[in] msg struct pomp_msg message
 */
void CarInfoDte::process(Message_Group_t message_id, const void* msg) {
    switch (message_id) {
    case MESSAGE_DTE:
        DteHandler(msg);
        break;

    default:
        break;
    }
}

void CarInfoDte::DteHandler(const void* msg) {
    if (!msg) {
        return;
    }

    MsgCarInfoContinuousMileage* p = static_cast<MsgCarInfoContinuousMileage*>(const_cast<void*>(msg));
    dte_info_ = p->nContinuousMileage;
    std::string s_dte_info{""};
    if (dte_info_ < DTE_MIN) {
        s_dte_info = DTE_ERR;
    } else if (dte_info_ <= DTE_MAX) {
        s_dte_info = std::to_string(dte_info_);
    } else {
        s_dte_info = std::to_string(DTE_MAX);
    }

    ui::UiNodeManager::singleton().setProperty(KANZI_MODULE_BASICINFO,
                                               KANZI_PROPERTY_BOTTOM_RANGE,
                                               s_dte_info);
}
