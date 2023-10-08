#pragma once

class Component
{
public:
	Component() {}
	virtual ~Component() = default;
	virtual void Awake() {};
	virtual void Update() {};
	virtual void FixedUpdate() {};
};