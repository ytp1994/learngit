/*
 * Copyright (c) 2019
 * All Rights Reserved by Thundersoft Software Technology Co., Ltd and its affiliates.
 * You may not use, copy, distribute, modify, transimit in any form of this file
 * except in compliance with THUNDERSOFT in writing by applicable law.
 */
#include "carinfo_drivemode.h"

#include <string>

#include "cluster_config.h"
#include "cluster_kanzi_app_def.h"
#include "ui_node_manager.h"

/**
 * @brief get CarInfoDriveMode singleton
 */
CarInfoDriveMode & CarInfoDriveMode::Singleton() {
    static CarInfoDriveMode kDriveModeSingleton;
    return kDriveModeSingleton;
}

/**
 * @brief CarInfoDriveMode constructor
 */
CarInfoDriveMode::CarInfoDriveMode():
    Component(MODULE_CARINFO_DRIVEMODE) {}

/**
 * @brief init CarInfoDriveMode with ComponentManager
 *
 * @param[in] cm ComponentManager
 */
void CarInfoDriveMode::Init(ComponentManager * component_manager) {
    component_manager->registerComponent(MESSAGE_DRIVEMODE, this);
}

/**
 * @brief process carinfo drivemode message
 *
 * @param[in] categoryId category id
 * @param[in] msg struct pomp_msg message
 */
void CarInfoDriveMode::process(Message_Group_t message_id, const void * msg) {
    switch (message_id) {
        case MESSAGE_DRIVEMODE:
            DriveModeHandler(msg);
            break;

        default:
            break;
    }
}

/**
 * @brief DriveModeHandler
 *
 * @param[in] drivemode msg from DBus
 */
void CarInfoDriveMode::DriveModeHandler(const void* msg) {
    if (!msg) {
        return;
    }

    MsgCarInfoDrivingMode* drive_mode_msg = static_cast<MsgCarInfoDrivingMode*> (const_cast<void*>(msg));
    switch (drive_mode_msg->nDrivingMode) {
        case DrivingMode_e::DrivingMode_Comfort:
            m_drive_mode_ = KANZI_MODE_COMFORT;
            break;
        case DrivingMode_e::DrivingMode_Sport:
            m_drive_mode_ = KANZI_MODE_SPORT;
            break;
        case DrivingMode_e::DrivingMode_Race:
            m_drive_mode_ = KANZI_MODE_RACE;
            break;
        case DrivingMode_e::DrivingMode_Race_Plus:
            m_drive_mode_ = KANZI_MODE_RACE_PLUS;
            break;
        default:
            break;
    }
    ui::UiNodeManager::singleton().setProperty(KANZI_MODULE_BASICINFO,
                                               KANZI_PROPERTY_BOTTOM_THEME,
                                               m_drive_mode_);
}
