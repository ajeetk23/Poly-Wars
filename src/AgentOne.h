#pragma once

#include "ofMain.h"
#include "Sprite.h"
#include "Shape.h"

class AgentOne : public Shape {
public:
	AgentOne();
	void init();
	void draw();

	void update();

	virtual bool inside(glm::vec3 p) {
		glm::vec3 s = glm::inverse(getMatrix()) * glm::vec4(p, 1);
		return (s.x > -width / 2 && s.x < width / 2 && s.y > -height / 2 && s.y < height / 2);
	}

	void setVelocity(const glm::vec3 v);

	void setLifespan(float l);

	void setRotation(float r);

	float age();

	float width, height;

	glm::vec3 velocity;
	float lifespan;
	float birthtime;

	bool drawable;
	bool haveImage;
	bool showImage;
	ofImage agentImage;

	void setImage(ofImage image) {
		agentImage = image;
		haveImage = true;
	}

	int agentType = rand() % 3;

	glm::vec3 acceleration = glm::vec3(0, 0, 0);
	glm::vec3 force = glm::vec3(0, 0, 0);
	float mass = 1.0;
	float damping = .99;
	float angularForce = 0;
	float angularVelocity = 0.0;
	float angularAcceleration = 0.0;

	ofColor *oclr;

};