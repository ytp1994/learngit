/*
 * Copyright (c) 2019
 * All Rights Reserved by Thundersoft Software Technology Co., Ltd and its affiliates.
 * You may not use, copy, distribute, modify, transimit in any form of this file
 * except in compliance with THUNDERSOFT in writing by applicable law.
 */
#ifndef CARINFO_CARINFO_TIME_H_
#define CARINFO_CARINFO_TIME_H_

#include "component_manager.h"

#define TIME_24_HOURS "0"
#define TIME_12_AM "1"
#define TIME_12_PM "2"

// define last time struct
typedef struct CarInfoTimeStruct {
    uint8_t t_hour;
    uint8_t t_minute;
    uint8_t t_time_format;
};

class CarInfoTime : public Component {
public:
    /**
     * @brief get CarInfoTime singleton
     */
    static CarInfoTime & Singleton();

    /**
     * @brief init CarInfoTime with ComponentManager
     *
     * @param[in] cm ComponentManager
     */
    void Init(ComponentManager * component_manager);

     /**
     * @brief process CarInfoTime message
     *
     * @param[in] categoryId category id
     * @param[in] msg void * message
     */
    void process(Message_Group_t message_id, const void * msg) override;

protected:
    /**
     * @brief CarInfoManager constructor
     */
    CarInfoTime();

    /**
     * @brief CarInfoManager destructor
     */
    ~CarInfoTime() {}

private:
    void TimeHandler(const void* msg);

    std::string m_time_string_ {"00:00"};
    std::string m_time_style_ {TIME_24_HOURS};
};

#endif  // CARINFO_CARINFO_TIME_H_
