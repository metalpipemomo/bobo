#pragma once

#include "EntityManager.h"
#include "SceneManager.h"
#include "Scene.h"

// Wraps Scene stuff and obfuscates whole entity stuff

class GameObject
{
public:
	GameObject(bool addTransform = true)
	{
		m_SceneName = SceneManager::GetActiveScene()->m_SceneName;
		m_Id = SceneManager::GetScene(m_SceneName)->CreateEntity();

		if (!addTransform) return;
		AddComponent<Transform>();

	}

	GameObject(const GameObject& gameObject, bool addTransform = true)
	{
		m_SceneName = SceneManager::GetActiveScene()->m_SceneName;
		m_Id = SceneManager::GetScene(m_SceneName)->CreateEntity(gameObject.m_Id);

		if (!addTransform) return;
		AddComponent<Transform>();
	}

	~GameObject()
	{
		GetScene()->DestroyEntity(m_Id);
	}

	std::vector<GameObject*> GetChildren()
	{
		auto kids = GetScene()->GetChildren(m_Id);

		if (kids.size() == 0) return {};

		std::vector<GameObject*> children;

		for (auto& kid : kids)
		{
			children.push_back(new GameObject(kid, m_SceneName));
		}

		return children;
	}

	void SetParent(const GameObject& gameObject)
	{
		GetScene()->SetParent(m_Id, gameObject.m_Id);
	}

	template <typename T, typename ...Args>
	bool AddComponent(Args&&... args)
	{
		return GetScene()->AddComponent<T>(m_Id, args...);
	}

	template <typename T>
	T* GetComponent()
	{
		return GetScene()->GetComponent<T>(m_Id);
	}

private:
	// For getting children and not incrementing entity counter
	GameObject(const Entity& childId, const std::string& sceneName)
		: m_Id(childId), m_SceneName(sceneName) {}

	Scene* GetScene()
	{
		return SceneManager::GetScene(m_SceneName);
	}

	Entity m_Id;
	std::string m_SceneName;
};