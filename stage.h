/*------------------------------
*	ブロックを積む処理[stage.h]
*
* 制作者：山室飛龍		日付：2025/9/21
------------------------------*/

#ifndef STAGE_H
#define STAGE_H

class Stage
{
public:

	float m_OffsetX = 0.0f;
	float m_OffsetY = 0.0f;

	int STAGE_WALL = 99;
	
	void Initialize(float x,float y);
	void Finalize();
	//double elapsedTime
	void Update();
	void Draw();

	int GetBlockHCount();	/* ステージのブロックの横個数 */
	int GetBlockVCount();	/* ステージのブロックの縦個数 */

	int GetBlock(int x, int y);
	void SetBlock(int kindIndex, int x, int y);
	
	int PitisionToStageX(float x);
	int PitisionToStageY(float y);

	bool SearchConnected();
	void DestroyBlocks();
	bool Adjust();

	static Stage* GetStage();

};

#endif // !
