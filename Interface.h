#pragma once
#include "Object.h"

class IDrawable {
protected:
	Triangle2 sprite;
public:
	virtual void draw(BuffTy buffer) const = 0;
	virtual ~IDrawable() = default;
};
class IActable {
protected:
	Triangle2 sprite;
public:
	IActable() = default;
	IActable(Triangle2 sprite) : sprite{ sprite } {}
	virtual void act(float dt) = 0;
	virtual ~IActable() = default;
};

class IObject {
protected:
	Triangle2 sprite;
public:
	IObject() = default;
	IObject(Triangle2 obj) : sprite{ obj } {}
	const Triangle2& getTriangle() const { return sprite; }
	virtual ~IObject() = default;
};