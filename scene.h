/*------------------------------
*	シーン処理[scene.h]
*
* 制作者：山室飛龍		日付：2025/9/21
------------------------------*/
#ifndef SCENE_H
#define SCENE_H

#include "title.h"
#include "game.h"
#include "reselt.h"

class Scene
{
public:
	void Initialize();
	void Finalize();
	void Update(double elapsedTime);
	void Draw();

	enum SceneChange
	{
		SCENE_TITLE,
		SCENE_GAME,
		SCENE_RESELT,
		SCENE_MAX,
	};
	void SetNextScene(SceneChange next);
	void ChangeScene();

	static Scene* GetScene();

private:
	Title m_Title;
	Game m_Game;
	Reselt m_Reselt;

	SceneChange m_Scene = SCENE_TITLE;
	SceneChange m_SceneNext = m_Scene;

};
#endif // !SCENE_H
