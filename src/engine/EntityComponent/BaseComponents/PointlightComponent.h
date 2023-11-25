#pragma once

#include "../../../bpch.h"

#include "../Component.h"
#include "../../Renderer/Light.h"

class PointlightComponent : public Component
{
public:
    Pointlight pointlight;
    PointlightComponent(Pointlight pointlight) : pointlight(pointlight) {}
};