/*
 * Copyright (c) 2019
 * All Rights Reserved by Thundersoft Software Technology Co., Ltd and its affiliates.
 * You may not use, copy, distribute, modify, transimit in any form of this file
 * except in compliance with THUNDERSOFT in writing by applicable law.
 */
#include "carinfo_time.h"

#include <iostream>
#include <string>

#include "cluster_config.h"
#include "cluster_kanzi_app_def.h"
#include "ui_node_manager.h"

// static instantation CarInfoTimeStruct
static CarInfoTimeStruct kCarInfoTime = {0, 0, 0};

/**
 * @brief get CarInfoTime Singleton
 */
CarInfoTime & CarInfoTime::Singleton() {
    static CarInfoTime kTimeSingleton;
    return kTimeSingleton;
}

/**
 * @brief CarInfoTime constructor
 */
CarInfoTime::CarInfoTime():
    Component(MODULE_CARINFO_TIME) {}

/**
 * @brief init CarInfoTime with ComponentManager
 *
 * @param[in] cm ComponentManager
 */
void CarInfoTime::Init(ComponentManager * component_manager) {
    component_manager->registerComponent(MESSAGE_TIME, this);
}

/**
 * @brief process carinfo time message
 *
 * @param[in] categoryId category id
 * @param[in] msg struct pomp_msg message
 */
void CarInfoTime::process(Message_Group_t message_id, const void * msg) {
    switch (message_id) {
        case MESSAGE_TIME:
            TimeHandler(msg);
            break;

        default:
            break;
    }
}

/**
 * @brief TimeHandler
 *
 * @param[in] msg time msg from DBus
 */
void CarInfoTime::TimeHandler(const void* msg) {
    if (!msg) {
        return;
    }

    MsgCarInfoTime* time_msg = static_cast<MsgCarInfoTime*> (const_cast<void*>(msg));
    uint8_t u_hour{time_msg->nHour};
    uint8_t u_minute{time_msg->nMinute};
    uint8_t u_format{time_msg->nSecond};
    std::string s_hour{""};
    std::string s_minute{""};

    // judge the time is legal or not
    if (u_hour >=0 && u_hour <24 && u_minute >= 0 && u_minute < 60) {
        // judge the time is changed or not
        if (u_hour == kCarInfoTime.t_hour
            && u_minute == kCarInfoTime.t_minute
            && u_format == kCarInfoTime.t_time_format) {
                return;
        } else {
            // judge the minute <10 or >10
            if (u_minute < 10) {
                s_minute = "0" + std::to_string(u_minute);
            } else {
                s_minute = std::to_string(u_minute);
            }

            // judge time format, 12 : 12h, 24 : 24h
            if (u_format == 12) {
                if (u_hour > 12) {
                    u_hour -= 12;
                    m_time_style_ = TIME_12_PM;
                } else if (u_hour == 12) {
                    m_time_style_ = TIME_12_PM;
                } else if (u_hour == 0) {
                    u_hour += 12;
                    m_time_style_ = TIME_12_AM;
                } else {
                    m_time_style_ = TIME_12_AM;
                }
                m_time_string_ = std::to_string(u_hour) + ":" + s_minute;
                ui::UiNodeManager::singleton().setProperty(KANZI_MODULE_BASICINFO,
                                                    KANZI_PROPERTY_TOP_TIMESTYLE,
                                                    m_time_style_);

            } else if (u_format == 24) {
                m_time_style_ = TIME_24_HOURS;
                ui::UiNodeManager::singleton().setProperty(KANZI_MODULE_BASICINFO,
                                                KANZI_PROPERTY_TOP_TIMESTYLE,
                                                m_time_style_);
                 if (u_hour < 10) {
                    m_time_string_ = "0" + std::to_string(u_hour) + ":" + s_minute;
                } else {
                    m_time_string_ = std::to_string(u_hour) + ":" + s_minute;
                }
            } else {
                // error
                return;
            }

            // storage time
            kCarInfoTime.t_hour = time_msg->nHour;
            kCarInfoTime.t_minute = time_msg->nMinute;
            kCarInfoTime.t_time_format = u_format;
        }
    } else {
        // error
            m_time_string_ = "--:--";
    }

    ui::UiNodeManager::singleton().setProperty(KANZI_MODULE_BASICINFO,
                                               KANZI_PROPERTY_TOP_TIME,
                                               m_time_string_);
}
