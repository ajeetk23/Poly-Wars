#pragma once
#include "ofApp.h"

AgentOne::AgentOne() {
	init();
}

void AgentOne::init() {
	lifespan = 15000;    // default milliseconds

	width = 20;
	height = 10 * sqrt(3);
	pos = ofVec3f(rand() % (ofGetScreenWidth() - (12 * (int)width / 2)) + (3 * (int)width / 2), rand() % (ofGetScreenHeight() - (12 * (int)height / 2)) + (3 * (int)height / 2), 0);
	rot = 0;
	velocity = ofVec3f(100, 100, 0);
	verts.push_back(glm::vec3(-10, 10, 0));
	verts.push_back(glm::vec3(10, 10, 0));
	verts.push_back(glm::vec3(0, -10, 0));
	drawable = false;
	haveImage = false;
	showImage = true;

}

void AgentOne::draw() {


	ofPushMatrix();
	ofMultMatrix(getMatrix());

	if (agentImage.load("images/AgentOneSprite.png") && showImage) {
		setImage(agentImage);
		agentImage.draw(-30, -30);
	}
	else {
		if (agentType == 0) {
			ofSetColor(0, 255, 255);
			oclr = new ofColor(0, 255, 255);
		}
		else if (agentType == 1) {
			ofSetColor(153, 102, 255);
			oclr = new ofColor(153, 102, 255);
		}
		else if (agentType == 2) {
			ofSetColor(255, 0, 102);
			oclr = new ofColor(255, 0, 102);
		}
		ofDrawTriangle(glm::vec3(-10, 10, 0), glm::vec3(10, 10, 0), glm::vec3(0, -10, 0));
	}


	ofPopMatrix();

}

void AgentOne::update() {
	//pos += (velocity / ofGetFrameRate());
}

void AgentOne::setVelocity(const glm::vec3 v) {
	if (agentType == 0) {
		velocity = v / 8;
	}
	else if (agentType == 1) {
		velocity = v / 15;
	}
	else if (agentType == 2) {
		velocity = v / 30;
	}
}

void AgentOne::setLifespan(float l) {
	lifespan = l;
}

void AgentOne::setRotation(float r) {
	rot = r;
}

float AgentOne::age() {
	return (ofGetElapsedTimeMillis() - birthtime);
}