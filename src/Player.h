#pragma once

#include "ofMain.h"
#include "Sprite.h"
#include "Shape.h"

class Player : public Shape {
public:
	Player(int pid);
	void init();
	void draw();

	void update();

	virtual bool inside(glm::vec3 p) {
		glm::vec3 s = glm::inverse(getMatrix()) * glm::vec4(p, 1);
		return (s.x > -width / 2 && s.x < width / 2 && s.y > -height / 2 && s.y < height / 2);
	}

	float width, height;

	int energyLevel;

	bool drawable;
	bool haveImage;
	bool showImage;
	ofImage playerImage;

	void setImage(ofImage image) {
		playerImage = image;
		haveImage = true;
	}

	int playerId;

};