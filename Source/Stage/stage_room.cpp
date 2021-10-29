

#include "stage_room.h"



StageRoom::StageRoom()
{
    mdl_stage = std::make_unique<Model>("Data/Model/stage/stage1.mdl");
}

StageRoom::~StageRoom()
{

}