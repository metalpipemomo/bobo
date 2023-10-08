#pragma once

#include "EntityManager.h"
#include "SceneManager.h"
#include "Scene.h"

// Wraps Scene stuff and obfuscates whole entity stuff

class GameObject
{
public:
	GameObject()
	{
		auto sm = SceneManager::GetInstance();
		m_SceneName = sm->GetActiveScene()->m_SceneName;
		m_Id = sm->GetScene(m_SceneName)->CreateEntity();
	}

	GameObject(const GameObject& gameObject)
	{
		auto sm = SceneManager::GetInstance();
		m_SceneName = sm->GetActiveScene()->m_SceneName;
		m_Id = sm->GetScene(m_SceneName)->CreateEntity(gameObject.m_Id);
	}

	~GameObject()
	{
		GetScene()->DestroyEntity(m_Id);
	}

	/*std::vector<GameObject*> GetChildren()
	{
		auto kids = GetScene()->GetChildren(m_Id);

		if (kids.size() == 0) return {};

		std::vector<GameObject*> children;

		for (auto& kid : kids)
		{
			children.push_back(new GameObject(kid, m_SceneName));
		}

		return children;
	}*/

	void SetParent(const GameObject& gameObject)
	{
		GetScene()->SetParent(m_Id, gameObject.m_Id);
	}

	template <typename T, typename ...Args>
	bool AddComponent(Args&&... args)
	{
		return GetScene()->AddComponent<T>(args);
	}

	template <typename T>
	T* GetComponent()
	{
		return GetScene()->GetComponent<T>();
	}

private:
	// For getting children and not incrementing entity counter
	GameObject(const Entity& childId, const std::string& sceneName)
		: m_Id(childId), m_SceneName(sceneName) {}

	Scene* GetScene()
	{
		auto sm = SceneManager::GetInstance();
		return sm->GetScene(m_SceneName);
	}

	Entity m_Id;
	std::string m_SceneName;
};