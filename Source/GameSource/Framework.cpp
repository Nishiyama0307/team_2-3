
#include <memory>
#include <sstream>


#include "Framework.h"
#include "common.h"
#include "Graphics/Graphics.h"
#include "Input/Input.h"
#include "Graphics/ResourceManager.h"
#include "EffectManager.h"
#include "audioManager.h"




// ���������Ԋu�ݒ�
static const int syncInterval = 1;

// �Q�[���I���p
bool game_exit = false;



Framework::Framework(HWND hWnd)
	: hWnd(hWnd)
	, input(hWnd)
	, graphics(hWnd)
{
	// �g�p���f���̐������o�^
	ResourceManager::Instance()->init();
	// �g�p�G�t�F�N�g�̐������o�^
	EffectManager::Instance()->Initialize();
	
	AudioManager::Instance().Init();

	sceneManager = std::make_unique<SceneManager>();
}


Framework::~Framework()
{
	AudioManager::Instance().Deinit();
	EffectManager::Instance()->Finalize();
}


void Framework::Update(float elapsedTime)
{
	// ���͍X�V����
	input.Update();

	sceneManager->Update(elapsedTime);

}


void Framework::Render(float elapsedTime)
{
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();

	// IMGUI�t���[���J�n����
	graphics.GetImGuiRenderer()->NewFrame();


	sceneManager->Render(elapsedTime);


	// IMGUI�`��
	graphics.GetImGuiRenderer()->Render(dc);

	// �o�b�N�o�b�t�@�ɕ`�悵�������ʂɕ\������B
	graphics.GetSwapChain()->Present(syncInterval, 0);
}


void Framework::CalculateFrameStats()
{
	// Code computes the average frames per second, and also the 
	// average time it takes to render one frame.  These stats 
	// are appended to the window caption bar.
	static int frames = 0;
	static float time_tlapsed = 0.0f;

	frames++;

	// Compute averages over one second period.
	if ((timer.TimeStamp() - time_tlapsed) >= 1.0f)
	{
		float fps = static_cast<float>(frames); // fps = frameCnt / 1
		float mspf = 1000.0f / fps;
		std::ostringstream outs;
		outs.precision(6);
#ifdef _DEBUG
		outs << "FPS : " << fps << " / " << "Frame Time : " << mspf << " (ms)";
#else
		outs << "�I�[�v�����[���h���ۂ� �A�N�V����RPG���Q�[��";
#endif
		//outs << "�^�C�g��";
		SetWindowTextA(hWnd, outs.str().c_str());

		// Reset for next average.
		frames = 0;
		time_tlapsed += 1.0f;
	}
}


int Framework::Run()
{
	MSG msg = {};

	while (WM_QUIT != msg.message)
	{
		if (!game_exit)
		{
			if (PeekMessage(&msg, NULL, 0, 0,	PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				timer.Tick();
				CalculateFrameStats();

				float elapsedTime = syncInterval == 0
					? timer.TimeInterval()
					: syncInterval / 60.0f
					;

				Update(elapsedTime);
				Render(elapsedTime);
			}
		}
		else
		{
			break;
		}
	}
	return static_cast<int>(msg.wParam);
}


LRESULT CALLBACK Framework::HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (Graphics::Instance().GetImGuiRenderer()->HandleMessage(hWnd, msg, wParam, lParam))
		return true;

	switch (msg)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc;
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_CREATE:
		break;
	case WM_KEYDOWN:	// TODO : �I�������̏C�� (esc�L�[�P�̂ŏI��点���A�m�F������)
#if _DEBUG
		if (wParam == VK_ESCAPE) PostMessage(hWnd, WM_CLOSE, 0, 0);
#endif
		break;
	case WM_ENTERSIZEMOVE:
		// WM_EXITSIZEMOVE is sent when the user grabs the resize bars.
		timer.Stop();
		break;
	case WM_EXITSIZEMOVE:
		// WM_EXITSIZEMOVE is sent when the user releases the resize bars.
		// Here we reset everything based on the new window dimensions.
		timer.Start();
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}
