

#include "stage_room.h"



StageRoom::StageRoom()
{
    mdl_stage = std::make_unique<Model>("Data/Model/LongStage.mdl");
}

StageRoom::~StageRoom()
{

}