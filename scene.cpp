/*------------------------------
*	シーン処理[scene.cpp]
*
* 制作者：山室飛龍		日付：2025/9/21
------------------------------*/
#include "scene.h"

#include "game.h"

Game* g_pGame;

void Scene::Initialize()
{
	switch (m_Scene)
	{
	case Scene::SCENE_TITLE:

		break;
	case Scene::SCENE_GAME:
		g_pGame->Initialize();
		break;
	case Scene::SCENE_RESELT:

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
		break;
	case Scene::SCENE_GAME:
		g_pGame->Finalize();
		break;
	case Scene::SCENE_RESELT:
		break;
	case Scene::SCENE_MAX:
		break;
	default:
		break;
	}
}

void Scene::Updata(double elapsedTime)
{
	switch (m_Scene)
	{
	case Scene::SCENE_TITLE:
		break;
	case Scene::SCENE_GAME:
		g_pGame->Updata(elapsedTime);
		break;
	case Scene::SCENE_RESELT:
		break;
	case Scene::SCENE_MAX:
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
		break;
	case Scene::SCENE_GAME:
		g_pGame->Draw();
		break;
	case Scene::SCENE_RESELT:
		break;
	case Scene::SCENE_MAX:
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
