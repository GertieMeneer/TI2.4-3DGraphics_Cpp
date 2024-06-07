#pragma once

class Component {
public:
	Component();
	~Component();

	virtual void update(float deltaTime) {};
};
