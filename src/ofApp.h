#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "Player.h"
#include "AgentOne.h"
#include "Shape.h"
#include "ParticleEmitter.h"

class SpriteList {
public:
	void add(AgentOne ao);
	void remove(int);
	void update();
	void draw();
	int listSize();
	void updatePlayerPos(glm::vec3 posCur);
	void updatePlayerAngle(glm::vec3 posCur);
	vector<AgentOne> sprites;
	bool allSpriteImages = true;
};

class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();
	void spawnAgentOne();
	void collisionCheck();
	void wallCollision(glm::vec3 delta);
	void wallRotateCollision(bool isRight);

	void clearAllEnemies(bool isOver);

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	Player* playerOne = NULL;
	int rotationAmount;
	glm::vec3 moved;

	SpriteList* sys;
	float rate;
	float lastSpawned;
	int numSpawned;
	int numAgents;

	int maxEnergyLevel;

	bool bHide;

	ofxIntSlider energyLevelSlider = 3;
	ofxFloatSlider lifeSpanSlider = 15000;
	ofxFloatSlider rateSlider = 1.0;
	ofxIntSlider numAgentsSlider = 15;
	ofxIntSlider agentSpeedSlider = 3;


	ofxPanel gui;

	bool canRun = false;
	bool isOver = false;

	const int OF_KEY_SPACE = 32;

	bool hasBackground;
	ofImage backgroundImage;

	void setImage(ofImage image) {
		backgroundImage = image;
		hasBackground = true;
	}

	bool toggleImageBackground = true;
	bool temp;

	bool movingForward;
	bool movingBackward;
	bool turningLeft;
	bool turningRight;

	//---------------------------------------

	ParticleEmitter *explosionEmitter;
	ParticleEmitter* explosionEmitterTwo;
	ParticleEmitter *rayEmitter;

	float explosionOneTimer = 0;
	float explosionTwoTimer = 0;

	int score;

	ofSoundPlayer thrusterSound;
	ofSoundPlayer raySound;
	ofSoundPlayer explosionSound;

	ofxIntSlider difficultySlider = 0;

};
