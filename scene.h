/*------------------------------
*	シーン処理[scene.h]
*
* 制作者：山室飛龍		日付：2025/9/21
------------------------------*/
#ifndef SCENE_H
#define SCENE_H

class Scene
{
public:
	void Initialize();
	void Finalize();
	void Updata(double elapsedTime);
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

	SceneChange m_Scene = SCENE_GAME;
	SceneChange m_SceneNext = m_Scene;

};
#endif // !SCENE_H
