/*
 * Copyright (c) 2019
 * All Rights Reserved by Thundersoft Software Technology Co., Ltd and its affiliates.
 * You may not use, copy, distribute, modify, transimit in any form of this file
 * except in compliance with THUNDERSOFT in writing by applicable law.
 */
#ifndef CARINFO_CARINFO_DRIVEMODE_H_
#define CARINFO_CARINFO_DRIVEMODE_H_

#include "component_manager.h"
#change2amend
typedef enum {
    KANZI_MODE_COMFORT = 0,
    KANZI_MODE_SPORT = 1,
    KANZI_MODE_RACE = 2,
    KANZI_MODE_RACE_PLUS = 3
}KanziDriveModeEnum;


class CarInfoDriveMode : public Component {
public:
    /**
     * @brief get CarInfoDriveMode singleton
     */
    static CarInfoDriveMode & Singleton();

    /**
     * @brief init CarInfoDriveMode with ComponentManager
     *
     * @param[in] cm ComponentManager
     */
    void Init(ComponentManager * component_manager);

     /**
     * @brief process CarInfoDriveMode message
     *
     * @param[in] categoryId category id
     * @param[in] msg void * message
     */
    void process(Message_Group_t message_id, const void * msg) override;

protected:
    /**
     * @brief CarInfoManager constructor
     */
    CarInfoDriveMode();

    /**
     * @brief CarInfoManager destructor
     */
    ~CarInfoDriveMode() {}

private:
    void DriveModeHandler(const void * msg);

    KanziDriveModeEnum m_drive_mode_ { KANZI_MODE_COMFORT };
};

#endif  // CARINFO_CARINFO_DRIVEMODE_H_
