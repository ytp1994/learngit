/*
 * Copyright (c) 2019
 * All Rights Reserved by Thundersoft Software Technology Co., Ltd and its affiliates.
 * You may not use, copy, distribute, modify, transimit in any form of this file
 * except in compliance with THUNDERSOFT in writing by applicable law.
 */
#include "carinfo_soc.h"

#include <string>

#include "cluster_config.h"
#include "cluster_kanzi_app_def.h"
#include "ui_node_manager.h"

/**
 * @brief get CarInfoSoc singleton
 */
CarInfoSoc & CarInfoSoc::Singleton() {
    static CarInfoSoc kSocSingleton;
    return kSocSingleton;
}

/**
 * @brief CarInfoSoc constructor
 */
CarInfoSoc::CarInfoSoc():
    Component(MODULE_CARINFO_ENERGY) {}

CarInfoSoc::~CarInfoSoc() {
    delete m_damping_soc_;
}

/**
 * @brief init CarInfoSoc with ComponentManager
 *
 * @param[in] cm ComponentManager
 */
void CarInfoSoc::Init(ComponentManager * component_manager) {
    component_manager->registerComponent(MESSAGE_ENERGY, this);
    
    //初始化时将回调函数给到Timer类中
    //成员函数指针pf，回调函数
    void (CarInfoSoc::*pf)() = &CarInfoSoc::TimerCallbackSoc;  

    m_timer_damping_soc_.configure(utils::Timer::Mill_t(100),  //0.1秒，也就是每隔这段时间给KANZI发送一次数据
                                   std::bind(pf, this),    //成员函数指针需要有具体的类的对象或者this指针才能进行调用，调用bind得到的可调用对象就相当于调用pf(this)，其实就相当于this->pf
                                   utils::Timer::ActionAlways);
    
    //new damping指针，m_Deviation为2，其实创建这个指针的唯一作用就是后面可以调用CalcNewPoint()这一个函数
    m_damping_soc_ = new PT1Damping<uint16_t>(2);
}

/**
 * @brief process carinfo drivesoc message
 *
 * @param[in] categoryId category id
 * @param[in] msg struct pomp_msg message
 */
void CarInfoSoc::process(Message_Group_t message_id, const void * msg) {
    switch (message_id) {
        case MESSAGE_ENERGY:
            SocHandler(msg);
            break;

        default:
            break;
    }
}

/**
 * @brief SocHandler
 *
 * @param[in] msg soc msg from DBus
 */
void CarInfoSoc::SocHandler(const void* msg) {
    if (!msg) {
        return;
    }

    MsgCarInfoBatteryPower* soc_msg = static_cast<MsgCarInfoBatteryPower*> (const_cast<void*>(msg));

    m_mutex_soc_.lock();
    
    //如果当前传输到kanzi端的数据和最新收到的数据不一样
    //m_car_soc_current_msg_是下面TimerCallbackSoc()函数最后一次运行
    //如果上次生成的阻尼数据list全都传输给kanzi端后，才来了新的数据，m_car_soc_current_msg_也就是取的m_soc_list_最后一个数据，也即上次从底层收到的数据
    //如果是上次生成的阻尼数据点还没有全都给到kanzi端，就来了新的数据，那么这个current_msg就是之前最后一次发送的数据
    if (m_car_soc_current_msg_ != soc_msg->nBatteryPower) {
        //清空list（list中保存的是上一次的阻尼数据）
        m_soc_list_.clear();
        //目标数据是本次收到的消息数据
        m_car_soc_target_msg_ = soc_msg->nBatteryPower;
        //限制在最大最小之间
        if (m_car_soc_target_msg_ > MAX_SOC) {
            m_car_soc_target_msg_ = MAX_SOC;
        }
        if (m_car_soc_target_msg_ < MIN_SOC) {
            m_car_soc_target_msg_ = MIN_SOC;
        }
        //如果m_damping_soc_指针是空的？Init()函数中new了指针，所以只要init()了就不会是空的指针
        if (!m_damping_soc_) {
            m_mutex_soc_.unlock();
            return;
        }
        //计算得到用于阻尼的数据点list，注意这里计算得到的list是通过参数引用来得到结果，也就是调用CalcNewPoint()计算得到的结果是在m_soc_list_中
        m_damping_soc_->CalcNewPoint(m_car_soc_current_msg_,
                                       m_car_soc_target_msg_,
                                       m_soc_list_,
                                       2);
        
        //如果timer类线程还没有开启，那就调用start()函数创建线程（start()函数中会把stop置0，并且有判断stop，如果stop是0也就是已经正在运行了会直接return）
        //也就开始了while(1)死循环运行run()函数，其中再运行回调函数，也就是TimerCallbackSoc()函数
        //也就是这个地方创建一个新线程，然后线程函数中通过chrono可以控制两次run()函数运行的时间差，也就是控制两次调用回调函数的时间差，通过这种方式就实现了控制
        if (m_timer_damping_soc_.isStop()) {
            m_timer_damping_soc_.start();
        }
        m_mutex_soc_.unlock();
    } else {
        m_mutex_soc_.unlock();
    }
}

//TimerCallbackSoc()函数作为回调函数给到Timer类中
//互斥锁问题：TimerCallbackSoc()运行在Timer线程中，而SocHandler()运行在其他线程中
//运行的线程不一致，TimerCallbackSoc()是要修改m_car_soc_current_msg_和m_soc_list_数据，SocHandler()要修改m_soc_list_，但是实际上两个函数修改的时机不是同时的，那为什么要加锁？
//正常来说：SocHandler()中已经修改完了，然后才会创建Timer线程，然后再结束Timer线程
//但是可能一个消息来了，然后创建Timer线程调用回调函数的时候，又来了一个消息调用了SocHandler()函数，这样就会出现冲突，所以需要 互斥锁
void CarInfoSoc::TimerCallbackSoc() {
    //try_lock()尝试解锁，和lock()的区别是如果lock失败，不会阻塞在这里不停的尝试，而是直接返回
    if (m_mutex_soc_.try_lock()) {
        if (!m_soc_list_.empty()) {
            //取list中第一个数据作为当前的数据，将这个数据传输到KANZI端并pop掉
            m_car_soc_current_msg_ = m_soc_list_.front();
            m_soc_list_.pop_front();
            ui::UiNodeManager::singleton().setProperty(KANZI_MODULE_BASICINFO,
                                               KANZI_PROPERTY_BOTTOM_SOC,
                                               std::to_string(m_car_soc_current_msg_));
        }
        //如果list是空的，也就是计算得到的数据点全都传给了KANZI了，就结束Timer类中创建的线程的运行
        if (m_soc_list_.empty()) {
            m_timer_damping_soc_.stopActived();
        }
        m_mutex_soc_.unlock();
    }
}
