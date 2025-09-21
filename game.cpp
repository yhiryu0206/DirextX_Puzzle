/*------------------------------
*	ゲームの処理[game.cpp]
*
* 制作者：山室飛龍		日付：2025/9/21
------------------------------*/
#include "game.h"
#include "back_ground.h"
#include "block.h"
#include "stage.h"
#include "carrier.h"

#include "keylogger.h"

static GameBackGround g_BackGround;
static GameBlock* g_Block = GameBlock::GetBlock();
static Stage* g_Stage = Stage::GetStage();
static Carrier* g_Carrier = Carrier::GetCarrier();


void Game::Initialize()
{
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

void Game::Updata(double elapsedTime)
{
	if (KeyLoggerIsTrigger(KK_UP))
	{
		g_Carrier->SwapUp();
	}
	if (KeyLoggerIsTrigger(KK_DOWN))
	{
		g_Carrier->SwapDown();
	}

	if (KeyLoggerIsTrigger(KK_RIGHT))
	{
		g_Carrier->MoveRight();
	}
	if (KeyLoggerIsTrigger(KK_LEFT))
	{
		g_Carrier->MoveLeft();
	}

	if (KeyLoggerIsTrigger(KK_S))
	{
		g_Carrier->Fall();
	}

	g_Carrier->Updata(elapsedTime);
}

void Game::Draw()
{
	g_BackGround.Draw();
	g_Block->Draw(1,100,100);
	g_Stage->Draw();
	g_Carrier->Draw();
}

Game* Game::GetGame()
{
	static Game instance;   // Meyers Singleton
	return &instance;
}
