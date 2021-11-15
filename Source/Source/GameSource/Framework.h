#pragma once

#include <windows.h>
#include "HighResolutionTimer.h"
#include "Audio/Audio.h"
#include "Graphics/Graphics.h"
#include "Input/input.h"
#include "Scene/SceneManager.h"

class Framework
{
public:
	Framework(HWND hWnd);
	~Framework();

private:
	void Update(float elapsedTime/*Elapsed seconds from last frame*/);
	void Render(float elapsedTime/*Elapsed seconds from last frame*/);

	// �t���[�����[�g�v�Z
	void CalculateFrameStats();

public:
	// �A�v���P�[�V�������[�v
	int Run();

	// ���b�Z�[�W�n���h��
	LRESULT CALLBACK HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
	const HWND				hWnd;
	HighResolutionTimer		timer;
	Audio					audio;
	Graphics				graphics;
	Input					input;

	//�V�[���}�l�[�W���[
	std::unique_ptr<SceneManager> sceneManager = nullptr;
};

