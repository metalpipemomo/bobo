#pragma once

#include "../../../bpch.h"

#include "../Component.h"
#include "../../Renderer/Light.h"

class SpotlightComponent : public Component
{
public:
    Spotlight spotlight;
    SpotlightComponent(Spotlight spotlight) : spotlight(spotlight) {}
};