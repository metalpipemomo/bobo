#pragma once

#include "bobopch.h"
#include "ModelData.h"

namespace Bobo
{
	class Model
	{
	public:
		Model(ModelData* modelData);
		virtual ~Model();
	private:
		ModelData* p_ModelData = nullptr;
	};
}