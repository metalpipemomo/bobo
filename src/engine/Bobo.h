#pragma once

/* ------- GAME INCLUDES ------- */

// Base
#include "Window.h"
#include "Log.h"
#include "Macro.h"
#include "Input.h"
#include "Time.h"

// Entity-Component
#include "EntityComponent/Component.h"
#include "EntityComponent/EntityManager.h"
#include "EntityComponent/Scene.h"
#include "EntityComponent/SceneManager.h"
#include "EntityComponent/GameObject.h"
#include "EntityComponent/BaseComponents/Transform.h"
#include "EntityComponent/BaseComponents/Parent.h"

// Audio
#include "Audio/Audio.h"
#include "Audio/SoundInfo.h"

// Coroutines
#include "Coroutine/Coroutine.h"
#include "Coroutine/CoroutineScheduler.h"
#include "Coroutine/Waits/WaitForSeconds.h"
#include "Coroutine/Waits/WaitForCoroutine.h"
#include "Coroutine/Waits/WaitUntil.h"

// Renderer
#include "Renderer/TextureLoader.h"
#include "Model/ModelLoader.h"

// Physics
#include "Physics/Physics.h"