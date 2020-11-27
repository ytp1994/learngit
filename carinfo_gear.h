/*
 * Copyright (c) 2019
 * All Rights Reserved by Thundersoft Software Technology Co., Ltd and its affiliates.
 * You may not use, copy, distribute, modify, transimit in any form of this file
 * except in compliance with THUNDERSOFT in writing by applicable law.
 */
#ifndef CARINFO_GEAR_H_
#define CARINFO_GEAR_H_

#include "component_manager.h"

typedef enum {
    KANZI_GEAR_BLANK,
    KANZI_GEAR_P,
    KANZI_GEAR_R,
    KANZI_GEAR_N,
    KANZI_GEAR_D,
    KANZI_GEAR_EP
}KanziGearPosition_t;

class CarInfoGear : public Component {
public:
    /**
     * @brief get Basic singleton
     */
    static CarInfoGear & Singleton();

    /**
     * @brief init Basic with ComponentManager
     *
     * @param[in] cm ComponentManager
     */
    void Init(ComponentManager * cm);

    /**
     * @brief process basic message
     *
     * @param[in] categoryId category id
     * @param[in] msg void * message
     */
    void process(Message_Group_t message_id, const void * msg) override;

protected:
    /**
     * @brief Basic constructor
     */
    CarInfoGear();

    /**
     * @brief Basic destructor
     */
    ~CarInfoGear() {}

private:
    void GearHandler(const void* msg);

    KanziGearPosition_t gear_info_;
};

#endif  // CARINFO_GEAR_H_
