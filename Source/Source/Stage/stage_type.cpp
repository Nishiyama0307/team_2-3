

#include "stage_type.h"


Stage1_Volcano::Stage1_Volcano()
{
    mdl_stage = std::make_unique<Model>("Data/Model/stage/stage1.mdl");
    //mdl_stage = std::make_unique<Model>("Data/Model/stage/‰¼/LongStage.mdl");

    if (mdl_stage.get() == nullptr) return;

    stage_type = STAGETYPE::STAGE1_VOLCANO;
}

Stage1_Volcano::~Stage1_Volcano()
{

}

Stage2_Desert::Stage2_Desert()
{
    mdl_stage = std::make_unique<Model>("Data/Model/stage/stage2.mdl");

    if (mdl_stage.get() == nullptr) return;

    stage_type = STAGETYPE::STAGE2_DESERT;
}

Stage2_Desert::~Stage2_Desert()
{

}

Stage3_Volcano::Stage3_Volcano()
{
    mdl_stage = std::make_unique<Model>("Data/Model/stage/stage3.mdl");

    if (mdl_stage.get() == nullptr) return;

    stage_type = STAGETYPE::STAGE3_;
}

Stage3_Volcano::~Stage3_Volcano()
{

}

Stage4_Volcano::Stage4_Volcano()
{
    mdl_stage = std::make_unique<Model>("Data/Model/stage/stage4.mdl");

    if (mdl_stage.get() == nullptr) return;

    stage_type = STAGETYPE::STAGE4_;
}

Stage4_Volcano::~Stage4_Volcano()
{

}

All_Stage::All_Stage()
{
    mdl_stage = std::make_unique<Model>("Data/Model/stage/allstage.mdl");

    if (mdl_stage.get() == nullptr) return;

    stage_type = STAGETYPE::ALL_STAGE;
}

All_Stage::~All_Stage()
{

}

castle::castle()
{
    mdl_stage = std::make_unique<Model>("Data/Model/stage/castle.mdl");

    if (mdl_stage.get() == nullptr) return;

    stage_type = STAGETYPE::CASTLE;
}

castle::~castle()
{

}


Sky::Sky()
{
    mdl_stage = std::make_unique<Model>("Data/Model/stage/sky.mdl");

    if (mdl_stage.get() == nullptr) return;

    stage_type = STAGETYPE::SKY;
}

Sky::~Sky()
{

}
