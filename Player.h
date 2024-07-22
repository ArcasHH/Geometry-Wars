#pragma once
#include "Object.h"
#include "Constants.h"
class IDrawable {
public:
	virtual void draw(BuffTy) = 0;
	virtual ~IDrawable() = default;
};
class IActable {
protected:
	Triangle start;
public:
	IActable(Triangle start) : start{ start } {}
	virtual void act(float dt) = 0;
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

struct Player: public IDrawable, IObject{
	Triangle sprite;
	vec2<float> position;
	vec2<float> speed;
	vec2<float> dir;
	bool is_control{ true };

	void act(float dt);
	void draw(BuffTy buffer);

    Player() = default;
	Player(Triangle tr) : sprite{ tr }, position{ tr.getCenter() }, speed{}, dir{ 0,1} {
		moveTo(vec2<float>(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2)); //move player to screen center
	}
	//bool collide(Triangle& t);

private:
    void control(float dt);
    void moveBy(vec2<float> vec);
	void moveTo(vec2<float> vec);
	void rotate();
	bool out_of_bounds(int width);
};

