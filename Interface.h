#pragma once
#include "Object.h"

class IDrawable {
protected:
	Triangle sprite;
public:
	virtual void draw(BuffTy buffer) {
		sprite.draw(buffer);
	}
	virtual ~IDrawable() = default;
};
class IActable {
protected:
	Triangle start;
public:
	IActable() = default;
	IActable(Triangle start) : start{ start } {}
	virtual void act(float dt, vec2<float> vec) = 0;
	virtual ~IActable() = default;
};

class IObject {
protected:
	Triangle sprite;
public:
	IObject() = default;
	IObject(Triangle obj) : sprite{ obj } {}
	const Triangle& getTriangle() const { return sprite; }
	virtual ~IObject() = default;
};