#pragma once

class Component
{
public:
	virtual ~Component() = default;
	virtual void Awake() {};
	virtual void Update() {};
	virtual void FixedUpdate() {};
};