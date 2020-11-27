/*
 * Copyright (c) 2019
 * All Rights Reserved by Thundersoft Software Technology Co., Ltd and its affiliates.
 * You may not use, copy, distribute, modify, transimit in any form of this file
 * except in compliance with THUNDERSOFT in writing by applicable law.
 */
#ifndef CARINFO_TEMPERATURE_H_
#define CARINFO_TEMPERATURE_H_

#include "component_manager.h"


class CarInfoTemperature : public Component {
public:
    using callback_temp = CallBack_t<const void *>;
    /**
     * @brief get Temperature sigleton
    */
    static CarInfoTemperature & Singleton();

    /**
     * @brief init temperature with ComponentManager
     *
     * @param[in] cm ComponentManager
     */
    void Init(ComponentManager* cm);
    /**
     * @brief process temperature message
     *
     * @param[in] categoryId category id
     *
     * @param[in] msg void * msg
     */
     void process(Message_Group_t message_id, const void * msg) override;

protected:
    /**
     * @brief  temperature constructor
     */
         CarInfoTemperature();
    /**
     * @brief  temperature destructor
     */
         ~CarInfoTemperature() {}

private:
    void TempHandler(const void* msg);

    float temperature_Info_{0.0};
};

#endif  // CARINFO_TEMPERATURE_H_
