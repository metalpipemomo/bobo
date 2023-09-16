#include "bobopch.h"
#include "Model.h"

namespace Bobo
{
	/** Model constructor that takes in model data object for use
	* once loaded into model loader.
	*/
	Model::Model(ModelData* modelData)
	{
		modelData = modelData;
	}

	/** Model destructor. */
	Model::~Model()
	{

	}
}