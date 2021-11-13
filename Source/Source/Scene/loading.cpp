

#include <thread>
#include "loading.h"
#include "common.h"
#include "Input/Input.h"



void Loading::Update(float elapsedTime)
{

	// �V�[���ύX
	{

	}


	//	��	�@���͏����Ƃ����낢�돑���@	��	�@//

	// TODO: �ǂݍ��ݏ���

	angle += elapsedTime * 200.0f;/* TEST: ��ŏ��� */

	if (scene->IsSceneLoaded())
	{
		ChangeNextScene(scene, false);
		scene = nullptr;
	}
}


void Loading::ModelRender(ID3D11DeviceContext* dc, Shader* shader)
{

}


void Loading::SpriteRender(ID3D11DeviceContext* dc)
{
	Graphics& graphics = Graphics::Instance();

	float screenWidth = CAST_F(graphics.GetScreenWidth());
	float screenHeight = CAST_F(graphics.GetScreenHeight());
	float textureWidth = CAST_F(spr_icon->GetTextureWidth());
	float textureHeight = CAST_F(spr_icon->GetTextureHeight());
	float positionX = screenWidth - textureWidth;
	float positionY = screenHeight - textureHeight;

	spr_back->Render2(dc,
		0, 0,						// �\���ʒu
		1.0f, 1.0f,									// �X�P�[��
		0, 0,										// �摜�؂蔲���ʒu
		1920, 1080,				// �摜�؂蔲���T�C�Y
		0, 0,	// �摜��_
		0,										// �p�x
		1, 1, 1, 1);								// �F���(r,g,b,a)

	spr_icon->Render2(dc,
		positionX, positionY,						// �\���ʒu
		1.0f, 1.0f,									// �X�P�[��
		0, 0,										// �摜�؂蔲���ʒu
		textureWidth, textureHeight,				// �摜�؂蔲���T�C�Y
		textureWidth * 0.5f, textureHeight * 0.5f,	// �摜��_
		angle,										// �p�x
		1, 1, 1, 1);								// �F���(r,g,b,a)
}


void Loading::DeInit()
{
	safe_delete(spr_icon);

	if (scene == nullptr) return;
	scene->DeInit();
	safe_delete(scene);
}


void Loading::Set()
{
	back_color = { 1,1,1,1 };

	std::thread thread(LoadingThread, this, GetSceneManager());

	thread.detach();
}


void Loading::Load()
{
	spr_back = std::make_unique<Sprite>("Data/Sprite/�^�C�g�����S�Ȃ��w�i�摜�i���U���g�j.png");
	spr_icon = new Sprite("Data/Sprite/���[�f�B���O.png");
}


void Loading::ImGui()
{
	ImGui::Text("scene : Loading");
}

void Loading::LoadingThread(Loading* load, SceneManager* scene_manager_)
{
	CoInitialize(nullptr);

	load->scene->Init(scene_manager_);

	CoUninitialize();

	load->scene->SetSceneLoaded();
}