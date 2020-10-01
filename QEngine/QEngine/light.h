#pragma once
enum LightType {
	DIRECTION,
	POINT,
	SPOT,
};

class Light {
protected:
	LightType type;
public:
	LightType GetType() { return type; }

	
};