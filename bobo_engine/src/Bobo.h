#pragma once

/* ------- GAME INCLUDES ------- */

#include "Bobo/Application.h"
#include "Bobo/Log.h"
#include "Bobo/Model/ModelLoader.h"
#include "Bobo/Audio/AudioEngine.h"
#include "Bobo/ECS/Scenes/SceneManager.h"
#include "Bobo/ECS/Scenes/Scene.h"
#include "Bobo/ECS/BaseComponents/Transform.h"
#include "Bobo/ECS/BaseComponents/RigidBody.h"
#include "Bobo/ECS/BaseSystems/PhysicsSystem.h"
#include "Bobo/Coroutine/Coroutine.h"
#include "Bobo/Coroutine/Waits/WaitForSeconds.h"
#include "Bobo/Coroutine/Waits/WaitUntil.h"
#include "Bobo/Coroutine/CoroutineScheduler.h"

/* ------- Entry Point ------- */

#include "Bobo/EntryPoint.h"