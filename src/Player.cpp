#pragma once
#include "ofApp.h"

Player::Player(int pid) {
	playerId = pid;
	init();
}

void Player::init() {
	width = 60;
	height = 30 * sqrt(3);
	pos = ofVec3f(500, 350, 0);
	rot = 0;
	energyLevel = 3;
	drawable = false;
	haveImage = false;
	showImage = true;
}

void Player::draw() {
	ofSetColor(226, 181, 196);

	ofPushMatrix();
	ofMultMatrix(getMatrix());

	//cout << playerImage.load("images/PlayerSprite.png") << "\n";
	if (playerImage.load("images/PlayerSprite.png") && showImage) {
		setImage(playerImage);
		playerImage.draw(-30, -30);
	}
	else {
		ofDrawTriangle(glm::vec3(-30, 30, 0), glm::vec3(30, 30, 0), glm::vec3(0, -30, 0));
	}

	ofPopMatrix();

}

void Player::update() {

}