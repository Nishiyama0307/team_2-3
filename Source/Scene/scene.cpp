

#include "scene.h"
#include "sceneManager.h"



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


    // buttom‚ª‰Ÿ‚³‚ê‚Ä‚¢‚È‚¯‚ê‚Î
    else
    {
        // ŽÀ‚Í ChangeNextSceneŠÖ”‚ªŒÄ‚Î‚ê‚é“x‚É,ˆø”‚Ånew‚É‚æ‚Á‚Ä¶¬‚³‚ê‚Ä‚¢‚é‚½‚ßíœ‚µ‚È‚¯‚ê‚Î‚È‚ç‚È‚¢ (ƒJƒX)
        safe_delete(next_scene_);
    }
}


void Scene::ChangeNextScene(Scene* next_scene_, bool isloading)
{
    isloading ? /* true */scene_manager->SetLoadNextScene(next_scene_) : /* false */scene_manager->SetNextScene(next_scene_);

    next_scene_ = nullptr;
}