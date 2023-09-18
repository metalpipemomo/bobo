#pragma once

namespace Bobo2
{
	class System
	{
	public:
		virtual ~System() = default;

		virtual void Update() {}

		virtual void FixedUpdate() {}
	};
}