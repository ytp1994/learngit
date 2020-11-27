/*
 * Copyright (c) 2019
 * All Rights Reserved by Thundersoft Software Technology Co., Ltd and its affiliates.
 * You may not use, copy, distribute, modify, transimit in any form of this file
 * except in compliance with THUNDERSOFT in writing by applicable law.
 */

#ifndef CARINFO_CARINFO_SOC_H_
#define CARINFO_CARINFO_SOC_H_

#include <mutex>

#include "component_manager.h"
#include "timer.h"
#include "damping_alga.h"

#define MAX_SOC 100
#define MIN_SOC 0

class CarInfoSoc : public Component {
public:
    /**
     * @brief get CarInfoSoc singleton
     */
    static CarInfoSoc& Singleton();

    /**
     * @brief init CarInfoSoc with ComponentManager
     *
     * @param[in] cm ComponentManager
     */
    void Init(ComponentManager* cm);

    /**
     * @brief process CarInfoSoc message
     *
     * @param[in] categoryId category id
     * @param[in] msg void * message
     */
    void process(Message_Group_t message_id, const void* msg) override;

protected:
    /**
     * @brief CarInfoSoc constructor
     */
    CarInfoSoc();

    /**
     * @brief CarInfoSoc destructor
     */
    ~CarInfoSoc();

private:
    void SocHandler(const void* msg);
    //
    void TimerCallbackSoc(void);
    //计算得到的阻尼数据点list
    std::list<uint16_t> m_soc_list_;
    //
    uint16_t m_car_soc_target_msg_ {0};
    //
    uint16_t m_car_soc_current_msg_ {0};

    //互斥锁，timer线程和运行SOC模块的线程，可能一次数据还没处理完就传输来了下一次数据
    std::mutex m_mutex_soc_;
    //Timer对象
    utils::Timer m_timer_damping_soc_;
    //计算阻尼点的类
    PT1Damping<uint16_t> * m_damping_soc_;
};

#endif  // CARINFO_CARINFO_SOC_H_
