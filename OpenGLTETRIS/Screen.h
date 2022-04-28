#pragma once

struct Screen {
	virtual void init() = 0;
	virtual void reset() = 0;
	virtual void keyboard(unsigned char _key) = 0;
	virtual void tick() = 0;
	virtual void update() = 0;
	virtual void draw() = 0;
};