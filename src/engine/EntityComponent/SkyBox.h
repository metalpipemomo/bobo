#pragma once

#include "../../bpch.h"
#include "../Model/Model.h"



class SkyBox 
{
public:
    SkyBox(Model* box)
    {
        p_Model = box;
    }

    Model* p_Model;

    
};