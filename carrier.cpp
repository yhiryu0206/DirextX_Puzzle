/*------------------------------
*	ゲームの製作[carrier.cpp]
*
* 制作者：山室飛龍		日付：2025/9/21
------------------------------*/
#include "carrier.h"
#include "block.h"
#include "stage.h"
#include "Audio.h"

#include "scene.h"

static std::random_device rd;
static std::mt19937 mt(rd());

void Carrier::Initialize()
{
	GameBlock* Block = GameBlock::GetBlock();
	FALL_HEIGHT = Block->GetHeight();
	MOVE_WIDTH = Block->GetWidth();

	m_BlockList[0] = -1;
	m_BlockList[1] = -1;
	m_BlockList[2] = -1;

	m_CarrierState = FALL_WAIT;
	m_CarrierX = 522;
	m_CarrierY = 72;

	m_AccumulatedTime = 0.0;
	m_FallWaitStartTime = 0.0f;
	m_DestroyWaitStartTime = 0.0f;

	m_LandingSoundId = LoadAudio("resource/Sound/パッ.wav");

}

void Carrier::Finalize()
{

}

void Carrier::Update(double elapsedTime)
{
	Stage* Stage = Stage::GetStage();
	GameBlock* Block = GameBlock::GetBlock();
	Scene* g_Scene = Scene::GetScene();

	m_AccumulatedTime += elapsedTime;
	double ratio = std::min((m_AccumulatedTime - m_MoveStartTime) / MOVE_TIME, 1.0);
	float ease = static_cast<float>(ratio) * static_cast<float>(MOVE_TIME);

	switch (m_CarrierState)
	{
	case Carrier::FALL_WAIT:
	
		double time;
		time = m_AccumulatedTime - m_FallWaitStartTime;
		if (time >= m_FallWaitTime)
		{
			int mx = Stage->PitisionToStageX(m_CarrierX);
			int my = Stage->PitisionToStageY(m_CarrierY +
				Block->GetHeight() * 2);

			if (Stage->GetBlock(mx, my + 1 )< 0)
			{
				m_CarrierState = FALL;
			}
			else
			{
				m_CarrierState = LANDING;
			}
		}
		break;
	
	case Carrier::FALL:
	{
		m_CarrierY += FALL_HEIGHT;
		m_CarrierState = FALL_WAIT;
		m_FallWaitStartTime = m_AccumulatedTime;
		break;
	}
	case Carrier::FALL_FORCE:
	{
		do
		{
			m_CarrierY += FALL_HEIGHT;
			int mx = Stage->PitisionToStageX(m_CarrierX);
			int my = Stage->PitisionToStageY(m_CarrierY +
				Block->GetHeight() * 2);
			if (Stage->GetBlock(mx, my + 1) < 0)
			{
				continue;
			}

			// 行きすぎているので1段戻す
			m_CarrierY -= FALL_HEIGHT;
			m_CarrierState = LANDING;

		} while (m_CarrierState != LANDING);
		break;
	}
	case Carrier::MOVE_LEFT:
	{
		m_CarrierX = m_MoveStartX - ease;
		if (ratio >= 1.0)
		{
			m_CarrierX = m_MoveStartX - MOVE_WIDTH; // 最終位置に補正
			m_CarrierState = FALL_WAIT;
		}
		break;
	}
	case Carrier::MOVE_RIGHT:
	{
		m_CarrierX = m_MoveStartX + ease;
		if (ratio >= 1.0)
		{
			m_CarrierX = m_MoveStartX + MOVE_WIDTH; // 最終位置に補正
			m_CarrierState = FALL_WAIT;
		}
		break;
	}
	case Carrier::LANDING:
	{
		int mx = Stage->PitisionToStageX(m_CarrierX);
		int my = Stage->PitisionToStageY(m_CarrierY);
		for (int i = 0; i < 3; i++)
		{
			Stage->SetBlock(m_BlockList[i], mx, my + i);
		}
		PlayAudio(m_LandingSoundId);
		m_CarrierState = DESTROY;

		break;
	}
	case Carrier::DESTROY:
		Stage->SearchConnected();
		Stage->DestroyBlocks();
		m_CarrierState = FALL_WAIT;
		m_CarrierState = ADJUST;
		m_DestroyWaitStartTime = m_AccumulatedTime;
		break;


	case Carrier::ADJUST:
		if (!Stage->Adjust())
		{
			Reset(522, 72);
			if (Stage->GetBlock(0, 0) >= 0)
			{
				m_CarrierState = GAMEOVER;
			}
			else
			{
				m_CarrierState = FALL_WAIT;
			}
		}

		break;
	case Carrier::ADJUST_WAIT:
		if (m_AccumulatedTime - m_DestroyWaitStartTime >= 1.0)
		{
			m_CarrierState = DESTROY;
		}
		break;
	case Carrier::GAMEOVER:

		g_Scene->SetNextScene(g_Scene->SCENE_RESELT);
		g_Scene->ChangeScene();

		break;

	default:
		break;
	}
}

void Carrier::Draw()
{
	GameBlock* Block = GameBlock::GetBlock();
	if (m_CarrierState > LANDING && m_CarrierState != GAMEOVER)return;
	for (int i = 0; i < 3; i++)
	{
		Block->Draw(m_BlockList[i], m_CarrierX,
			m_CarrierY + Block->GetHeight() * i);
	}
}

void Carrier::Reset(float x, float y)
{
	std::uniform_int_distribution<int> dist(0, 5);
	m_CarrierX = x;
	m_CarrierY = y;

	m_BlockList[0] = dist(mt);
	m_BlockList[1] = dist(mt);
	m_BlockList[2] = dist(mt);

	m_CarrierState = FALL_WAIT;

}

void Carrier::MoveLeft()
{
	Stage* Stage = Stage::GetStage();
	if (m_CarrierState != FALL_WAIT) return;

	int mx = Stage->PitisionToStageX(m_CarrierX);
	int my = Stage->PitisionToStageY(m_CarrierY);
	for (int i = 0; i < 3; i++)
	{
		if (Stage->GetBlock(mx - 1, my + i) >= 0)
		{
			m_CarrierState = FALL_WAIT;
			return;
		}
	}
	m_CarrierState = MOVE_LEFT;
	m_MoveStartX = m_CarrierX;
	m_MoveStartTime = m_AccumulatedTime;
}

void Carrier::MoveRight()
{
	Stage* Stage = Stage::GetStage();
	if (m_CarrierState != FALL_WAIT) return;

	int mx = Stage->PitisionToStageX(m_CarrierX);
	int my = Stage->PitisionToStageY(m_CarrierY);
	for (int i = 0; i < 3; i++)
	{
		if (Stage->GetBlock(mx + 1, my + i) >= 0)
		{
			m_CarrierState = FALL_WAIT;
			return;
		}
	}
	m_CarrierState = MOVE_RIGHT;
	m_MoveStartX = m_CarrierX;
	m_MoveStartTime = m_AccumulatedTime;
}

void Carrier::SwapUp()
{
	int temp = m_BlockList[0];
	m_BlockList[0] = m_BlockList[1];
	m_BlockList[1] = m_BlockList[2];
	m_BlockList[2] = temp;
}

void Carrier::SwapDown()
{
	int temp = m_BlockList[2];
	m_BlockList[2] = m_BlockList[1];
	m_BlockList[1] = m_BlockList[0];
	m_BlockList[0] = temp;
}

void Carrier::Fall()
{
	if (m_CarrierState != FALL_WAIT)return;
	m_CarrierState = FALL_FORCE;
}

const int* Carrier::GetBlock()
{
	return m_BlockList;
}

Carrier* Carrier::GetCarrier()
{
	static Carrier instance;   // Meyers Singleton
	return &instance;
}
