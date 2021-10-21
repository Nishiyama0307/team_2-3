

#include "scene.h"
#include "sceneManager.h"


int scene_tag = 0;

void Scene::Init(SceneManager* scene_manager_)
{
    scene_manager = scene_manager_;
    Load();
    Set();
}

void Scene::ChangeNextScene(Scene* next_scene_, GamePadButton buttom, bool isloading)
{
    if (Input::Instance().GetGamePad().GetButtonDown() & buttom)
    {
        isloading ? /* true */scene_manager->SetLoadNextScene(next_scene_) : /* false */scene_manager->SetNextScene(next_scene_);

        next_scene_ = nullptr;
    }


    // buttom��������Ă��Ȃ����
    else
    {
        // ���� ChangeNextScene�֐����Ă΂��x��,������new�ɂ���Đ�������Ă��邽�ߍ폜���Ȃ���΂Ȃ�Ȃ� (�J�X)
        safe_delete(next_scene_);
    }
}


void Scene::ChangeNextScene(Scene* next_scene_, bool isloading)
{
    isloading ? /* true */scene_manager->SetLoadNextScene(next_scene_) : /* false */scene_manager->SetNextScene(next_scene_);

    next_scene_ = nullptr;
}