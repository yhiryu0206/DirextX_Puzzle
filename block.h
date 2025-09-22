/*------------------------------
*	ブロック処理[block.h]
*
* 制作者：山室飛龍		日付：2025/9/21
------------------------------*/

#ifndef BLOCK_H
#define BLOCK_H

class GameBlock
{
public:
	void Initialize();
	void Finalize();
	void Update();
	void Draw(int kindIndex,float x,float y);

	float GetWidth();
	float GetHeight();

	static GameBlock* GetBlock();


private:

	int m_BlockTexId;

	float m_BlockWidth=64;
	float m_BlockHeight=64;
};
#endif // !BLOCK_H
