//=============================================================================
// Creator: Stan Palisoc
//
// Stan Palisoc's Mediocrely Attrocious Game Engine
//				 Maybe Awesome Game Engine
//
//============================================================================ 
#pragma once

#include "Common.h"

//App Headers
#include "App.h"
#include "AppState.h"


// Component Headers
#include "ColliderComponent.h"
#include "TransformComponent.h"

// Service headers
#include "CameraService.h"

//Terrain Headers
#include "Terrain/Terrain.h"

//Skydome Headers
#include "Skydome.h"

//World Headers
#include "Component.h"
#include "GameObject.h"
#include "GameObjectFactory.h"
#include "GameWorld.h"
#include "Service.h"
namespace MAGE 
{
	App& MainApp();
}