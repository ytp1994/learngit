/*
 * Copyright (c) 2019
 * All Rights Reserved by Thundersoft Software Technology Co., Ltd and its
 * affiliates. You may not use, copy, distribute, modify, transimit in any form
 * of this file except in compliance with THUNDERSOFT in writing by applicable
 * law.
 */

#ifndef CARINFO_CARINFO_DTE_H_
#define CARINFO_CARINFO_DTE_H_

#include "component_manager.h"

#define DTE_MIN 0
#define DTE_MAX 999
#define DTE_ERR "---"
#change
class CarInfoDte : public Component {
public:
    /**
     * @brief get CarInfoDte singleton
     */
    static CarInfoDte& Singleton();
    /**
     * @brief init CarInfoDte with ComponentManager
     *
     * @param[in] cm ComponentManager
     */
    void Init(ComponentManager* cm);
    /**
     * @brief process CarInfoDte message
     *
     * @param[in] category_id category id
     * @param[in] msg void* msg
     */
    void process(Message_Group_t message_id, const void* msg) override;

protected:
    /**
     * @brief CarInfoDte constructor
     */
    CarInfoDte();
    /**
     * @brief CarInfoDte destructor
     */
    ~CarInfoDte() {}

private:
    void DteHandler(const void*);

    uint16_t dte_info_{0};
};

#endif  // CARINFO_CARINFO_DTE_H_
