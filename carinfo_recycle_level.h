/*
 * Copyright (c) 2019
 * All Rights Reserved by Thundersoft Software Technology Co., Ltd and its
 * affiliates. You may not use, copy, distribute, modify, transimit in any form
 * of this file except in compliance with THUNDERSOFT in writing by applicable
 * law.
 */

#ifndef CARINFO_CARINFO_RECYCLE_LEVEL_H_
#define CARINFO_CARINFO_RECYCLE_LEVEL_H_

#include "component_manager.h"

typedef enum {
    KANZI_LEVEL_DEFAULT_OR_ERR = 0,
    KANZI_LEVEL_LOW = 1,
    KANZI_LEVEL_MIDDLE = 2,
    KANZI_LEVEL_HIGH = 3,
} KanziRecycleLevelEnum;

class CarInfoRecycleLevel : public Component {
public:
    /**
     * @brief get CarInfoRecycleLevel singleton
     */
    static CarInfoRecycleLevel& Singleton();
    /**
     * @brief init CarInfoRecycleLevel with ComponentManager
     *
     * @param[in] cm ComponentManager
     */
    void Init(ComponentManager* cm);
    /**
     * @brief process CarInfoRecycleLevel message
     *
     * @param[in] category_id category id
     * @param[in] msg void* msg
     */
    void process(Message_Group_t message_id, const void* msg) override;

protected:
    /**
     * @brief CarInfoRecycleLevel constructor
     */
    CarInfoRecycleLevel();
    /**
     * @brief CarInfoRecycleLevel destructor
     */
    ~CarInfoRecycleLevel() {}

private:
    void RecycleLevelHandler(const void* msg);

    KanziRecycleLevelEnum recycle_level_info_{KANZI_LEVEL_LOW};
};

#endif  // CARINFO_CARINFO_RECYCLE_LEVEL_H_
