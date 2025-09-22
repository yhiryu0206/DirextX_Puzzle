/*------------------------------
*	ƒQ[ƒ€‚Ìˆ—[game.cpp]
*
* §ìŽÒFŽRŽº”ò—´		“ú•tF2025/9/21
------------------------------*/
#include "game.h"
#include "back_ground.h"
#include "block.h"
#include "stage.h"
#include "carrier.h"
#include "Audio.h"
#include "keylogger.h"

static GameBackGround g_BackGround;
static GameBlock* g_Block = GameBlock::GetBlock();
static Stage* g_Stage = Stage::GetStage();
static Carrier* g_Carrier = Carrier::GetCarrier();


void Game::Initialize()
{
	m_GameSoundId = LoadAudio("resource/Sound/“ü‚è]‚ð–]‚ÞÂ‚«¹ˆæ.wav");
	
	PlayAudio(m_GameSoundId);
	g_BackGround.Initialize();
	g_Block->Initialize();
	g_Stage->Initialize(522, 72);
	g_Carrier->Initialize();

	g_Carrier->Reset(522, 72);
}

void Game::Finalize()
{
	g_Carrier->Finalize();
	g_Stage->Finalize();
	g_BackGround.Finalize();
	g_Block->Finalize();
}

void Game::Update(double elapsedTime)
{
	if (IsKeyDown(VK_UP))
	{
		g_Carrier->SwapUp();
	}
	if (IsKeyDown(KK_DOWN))
	{
		g_Carrier->SwapDown();
	}

	if (IsKeyDown(KK_RIGHT))
	{
		g_Carrier->MoveRight();
	}
	if (IsKeyDown(KK_LEFT))
	{
		g_Carrier->MoveLeft();
	}

	if (IsKeyDown(KK_S))
	{
		g_Carrier->Fall();
	}

	g_Carrier->Update(elapsedTime);
}

void Game::Draw()
{
	g_BackGround.Draw();
	g_Stage->Draw();
	g_Carrier->Draw();
}

Game* Game::GetGame()
{
	static Game instance;   // Meyers Singleton
	return &instance;
}
