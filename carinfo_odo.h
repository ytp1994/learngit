/*
 * Copyright (c) 2019
 * All Rights Reserved by Thundersoft Software Technology Co., Ltd and its affiliates.
 * You may not use, copy, distribute, modify, transimit in any form of this file
 * except in compliance with THUNDERSOFT in writing by applicable law.
 */
#ifndef CARINFO_ODO_H_
#define CARINFO_ODO_H_

#include "component_manager.h"

#define ODO_MAX 999999
#define ODO_MIN 0
#define ODO_ERR "------"
#change
class CarInfoOdo : public Component {
public:
    /**
     * @brief get Basic singleton
     */
    static CarInfoOdo& Singleton();

    /**
     * @brief init Basic with ComponentManager
     *
     * @param[in] cm ComponentManager
     */
    void Init(ComponentManager* cm);

    /**
     * @brief process basic message
     *
     * @param[in] categoryId category id
     * @param[in] msg void * message
     */
    void process(Message_Group_t message_id, const void* msg) override;

protected:
    /**
     * @brief Basic constructor
     */
    CarInfoOdo();

    /**
     * @brief Basic destructor
     */
    ~CarInfoOdo() {}

private:
    void OdoHandler(const void* msg);

    int32_t odo_info_;
};

#endif  // CARINFO_ODO_H_
