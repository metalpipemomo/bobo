#pragma once

class Entity;
	class Component
	{
	public:
		virtual ~Component() {}
		virtual void Awake() {}
		virtual void Update() {}
		virtual void FixedUpdate() {}
		bool SetEnabled(bool enabled) { m_Enabled = enabled; }
		bool Enabled() { return m_Enabled; }
		// Override this if you wish to allow multiple of the Component to be active on an Entity at a time
		virtual bool AllowDuplicatesOfType() { return false; }
	protected:
		Entity* p_Parent;
		bool m_Enabled;
	};