#pragma once
class ISceneHandler {
public:
	virtual void Awake() = 0;
	
	virtual void Update(float deltaTime) = 0;

	virtual void Render() = 0;

	virtual void Destory() = 0;
};