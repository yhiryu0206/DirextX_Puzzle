/*------------------------------
*	シーン処理[scene.cpp]
*
* 制作者：山室飛龍		日付：2025/9/21
------------------------------*/
#include "scene.h"
#include "Audio.h"

void Scene::Initialize()
{
	InitAudio();
	switch (m_Scene)
	{
	case Scene::SCENE_TITLE:
		m_Title.Initialize();
		break;
	case Scene::SCENE_GAME:
		m_Game.Initialize();
		break;
	case Scene::SCENE_RESELT:
		m_Reselt.Initialize();
		break;
	default:
		break;
	}
}

void Scene::Finalize()
{
	switch (m_Scene)
	{
	case Scene::SCENE_TITLE:
		m_Title.Finalize();
		break;
	case Scene::SCENE_GAME:
		m_Game.Finalize();
		break;
	case Scene::SCENE_RESELT:
		m_Reselt.Finalize();
		break;

	default:
		break;
	}
	UninitAudio();

}

void Scene::Update(double elapsedTime)
{
	switch (m_Scene)
	{
	case Scene::SCENE_TITLE:
		m_Title.Update(elapsedTime);
		break;
	case Scene::SCENE_GAME:
		m_Game.Update(elapsedTime);
		break;
	case Scene::SCENE_RESELT:
		m_Reselt.Update(elapsedTime);
		break;
	default:
		break;
	}
}

void Scene::Draw()
{
	switch (m_Scene)
	{
	case Scene::SCENE_TITLE:
		m_Title.Draw();
		break;
	case Scene::SCENE_GAME:
		m_Game.Draw();
		break;
	case Scene::SCENE_RESELT:
		m_Reselt.Draw();
		break;
	default:
		break;
	}
}

void Scene::SetNextScene(SceneChange next)
{
	m_SceneNext = next;
}

void Scene::ChangeScene()
{
	if (m_Scene != m_SceneNext)
	{
		/* 終了処理 */
		Finalize();
		m_Scene = m_SceneNext;

		/* 初期化処理 */
		Initialize();
	}
}

Scene* Scene::GetScene()
{
	static Scene instance;   // Meyers Singleton
	return &instance;
}
