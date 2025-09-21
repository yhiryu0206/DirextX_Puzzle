/*------------------------------
*	ブロックを積む処理[stage.h]
*
* 制作者：山室飛龍		日付：2025/9/21
------------------------------*/
#include "stage.h"
#include "block.h"
#include "texture.h"
#include "sprite.h"

static Stage g_Stage;

static constexpr int BLOCK_H_COUNT = 8;
static constexpr int BLOCK_V_COUNT = 10;

static int g_StageBlock[BLOCK_V_COUNT][BLOCK_H_COUNT];
static int g_DestroyMap[BLOCK_V_COUNT][BLOCK_H_COUNT];

bool searchHConnected();
bool searchVConnected();

void Stage::Initialize(float x, float y)
{
	m_OffsetX = x;
	m_OffsetY = y;

	for (int i = 0; i < BLOCK_V_COUNT; i++)
	{
		for (int j = 0; j < BLOCK_H_COUNT; j++)
		{
			g_StageBlock[i][j] = -1;
		}
	}
}

void Stage::Finalize()
{
}

void Stage::Update()
{
}

void Stage::Draw()
{
	GameBlock* Block = GameBlock::GetBlock();
	for (int i = 0; i < BLOCK_V_COUNT; i++)
	{
		for (int j = 0; j < BLOCK_H_COUNT; j++)
		{
			Block->Draw(g_StageBlock[i][j], 
				m_OffsetX + (j * Block->GetWidth()),
				m_OffsetY + (i * Block->GetHeight()));
		}
	}
}

int Stage::GetBlockHCount()
{
	return BLOCK_H_COUNT;
}

int Stage::GetBlockVCount()
{
	return BLOCK_V_COUNT;
}

int Stage::GetBlock(int x, int y)
{
	if (x < 0 || x >= BLOCK_H_COUNT)return STAGE_WALL;
	if (y < 0 || y >= BLOCK_V_COUNT)return STAGE_WALL;

	return g_StageBlock[y][x];
}

void Stage::SetBlock(int kindIndex, int x, int y)
{
	GameBlock Block{};
	g_StageBlock[y][x] = kindIndex;
	Block.Draw(kindIndex, m_OffsetX + Block.GetWidth() * x,
		m_OffsetY + Block.GetHeight() * y);
}

int Stage::PitisionToStageX(float x)
{
	GameBlock Block{};
	return static_cast<int> ((x - m_OffsetX) / Block.GetWidth());
}

int Stage::PitisionToStageY(float y)
{
	GameBlock Block{};
	return static_cast<int> ((y - m_OffsetY) / Block.GetHeight());
}

bool Stage::SearchConnected()
{
	for (int i = 0; i < BLOCK_V_COUNT; i++)
	{
		for (int j = 0; j < BLOCK_H_COUNT; j++)
		{
			g_DestroyMap[i][j] = false;
		}
	}
	bool ret = false;
	if (searchHConnected())
	{
		ret = true;
	}

	if (searchVConnected())
	{
		ret = true;
	}
	return ret;
}

void Stage::DestroyBlocks()
{
	int destroyCount = 0;
	for (int i = 0; i < BLOCK_V_COUNT; i++)
	{
		for (int j = 0; j < BLOCK_H_COUNT; j++)
		{
			if (g_DestroyMap[i][j])
			{
				//点数計算はここで
				g_StageBlock[i][j] = -1;
				destroyCount++;
			}
		}
	}
}

bool Stage::Adjust()
{
	bool ret = false;
	for (int v = BLOCK_V_COUNT - 1; v >= 1; v--)
	{
		for (int h = 0; h < BLOCK_H_COUNT; h++)
		{
			if (g_StageBlock[v][h] < 0 && g_StageBlock[v - 1][h] >= 0)
			{
				g_StageBlock[v][h] = g_StageBlock[v - 1][h];
				g_StageBlock[v - 1][h] = -1;
				ret = true;
			}
		}
	}

	if (ret)
	{
		Adjust();
	}
	return ret;
}

Stage* Stage::GetStage()
{
	static Stage instance;   // Meyers Singleton
	return &instance;
}

bool searchHConnected()
{
	bool ret = false;

	for (int y = 0; y < BLOCK_V_COUNT; y++)
	{
		int keepKind = -1;
		int keepCount = 0;
		int keepX = 0;
		for (int x = 0; x < BLOCK_H_COUNT; x++)
		{
			int kind = g_StageBlock[y][x];

			if (kind < 0)
			{
				keepKind = kind;
				if (keepCount >= 3)
				{
					for (int i = 0; i < keepCount; i++)
					{
						g_DestroyMap[y][keepX + i] = true;
					}
					ret = true;
				}
				keepCount = 0;

			}
			else if (keepKind == kind)
			{
				keepCount++;
			}
			else
			{
				if (keepCount >= 3)
				{
					for (int i = 0; i < keepCount; i++)
					{
						g_DestroyMap[y][keepX + i] = true;
					}
					ret = true;

				}

				keepX = x;
				keepCount = 1;
				keepKind = kind;
			}
		}
		if (keepCount >= 3)
		{
			for (int i = 0; i < keepCount; i++)
			{
				g_DestroyMap[y][keepX + i] = true;
			}
			ret = true;
		}
	}
	return ret;
}

bool searchVConnected()
{
	bool ret = false;
	for (int x = 0; x < BLOCK_H_COUNT; x++)
	{
		int keepKind = -1;
		int keepCount = 0;
		int keepY = 0;
		for (int y = 0; y < BLOCK_V_COUNT; y++)
		{
			int kind = g_StageBlock[y][x];

			if (kind < 0)
			{
				keepKind = kind;
				if (keepCount >= 3)
				{
					for (int i = 0; i < keepCount; i++)
					{
						g_DestroyMap[keepY + i][x] = true;
					}
					ret = true;

				}
				keepCount = 0;


			}
			else if (keepKind == kind)
			{
				keepCount++;
			}
			else
			{
				if (keepCount >= 3)
				{
					for (int i = 0; i < keepCount; i++)
					{
						g_DestroyMap[keepY + i][x] = true;
					}
					ret = true;
				}

				keepY = y;
				keepCount = 1;
				keepKind = kind;
			}

		}

		if (keepCount >= 3)
		{
			for (int i = 0; i < keepCount; i++)
			{
				g_DestroyMap[keepY + i][x] = true;
			}
			ret = true;

		}
	}
	return ret;
}
