#include "ofApp.h"





//  Add a Sprite to the Sprite System
//
void SpriteList::add(AgentOne ao) {
	sprites.push_back(ao);
}

// Remove a sprite from the sprite system. Note that this function is not currently
// used. The typical case is that sprites automatically get removed when the reach
// their lifespan.
//
void SpriteList::remove(int i) {
	sprites.erase(sprites.begin() + i);
}


//  Update the SpriteSystem by checking which sprites have exceeded their
//  lifespan (and deleting).  Also the sprite is moved to it's next
//  location based on velocity and direction.
//
void SpriteList::update() {

	if (sprites.size() == 0) return;
	vector<AgentOne>::iterator s = sprites.begin();
	vector<AgentOne>::iterator tmp;

	// check which sprites have exceed their lifespan and delete
	// from list.  When deleting multiple objects from a vector while
	// traversing at the same time, use an iterator.
	//
	while (s != sprites.end()) {
		if (s->lifespan != -1 && s->age() > s->lifespan) {
			//			cout << "deleting sprite: " << s->name << endl;
			tmp = sprites.erase(s);
			s = tmp;

		}
		else s++;
	}


	int moveDir;
	float typetwotime = ofGetElapsedTimeMillis();
	//  Move sprite
	//
	for (int i = 0; i < sprites.size(); i++) {
		//ofVec3f pV = ofVec3f(500, 350, 0);
		//sprites[i].pos += sprites[i].velocity / ofGetFrameRate();
		moveDir = rand() % 4;


		if (sprites[i].agentType == 0 || sprites[i].agentType == 1) {
			sprites[i].pos += (sprites[i].velocity) / ofGetFrameRate();
		}
		else if (sprites[i].agentType == 2) {
			if (ofGetElapsedTimeMillis() - typetwotime >= 5000) {
				moveDir = rand() % 4;
				typetwotime = ofGetElapsedTimeMillis();
			}
			if (moveDir == 0) {
				sprites[i].pos += ofVec3f(sprites[i].velocity.x / 2, 0, 0) / ofGetFrameRate();
			}
			else if (moveDir == 1) {
				sprites[i].pos += ofVec3f(0, sprites[i].velocity.y / 2, 0) / ofGetFrameRate();
			}
			else if (moveDir == 2) {
				sprites[i].pos += ofVec3f(-sprites[i].velocity.x / 2, 0, 0) / ofGetFrameRate();
			}
			else if (moveDir == 3) {
				sprites[i].pos += ofVec3f(0, -sprites[i].velocity.y / 2, 0) / ofGetFrameRate();
			}
		}
		
	}
}

//  Render all the sprites
//
void SpriteList::draw() {
	for (int i = 0; i < sprites.size(); i++) {
		if (allSpriteImages) {
			sprites[i].showImage = true;
		}
		else {
			sprites[i].showImage = false;
		}
		sprites[i].draw();

	}
}

int SpriteList::listSize() {
	return sprites.size();
}

void SpriteList::updatePlayerPos(glm::vec3 posCur) {
	float dt = 1.0 / ofGetFrameRate();

	for (int i = 0; i < sprites.size(); i++) {
		if (sprites[i].agentType == 0 || sprites[i].agentType == 1) {
			sprites[i].pos += (sprites[i].velocity * dt);
			glm::vec3 accel = sprites[i].acceleration;
			accel += (sprites[i].force * 1.0 / sprites[i].mass);
			sprites[i].velocity += accel * dt;
			sprites[i].velocity *= sprites[i].damping;


			sprites[i].setVelocity(posCur - sprites[i].pos);
		}
	}
}

void SpriteList::updatePlayerAngle(glm::vec3 posCur) {
	float dt = 1.0 / ofGetFrameRate();

	for (int i = 0; i < sprites.size(); i++) {
		if (sprites[i].agentType == 0) {

			sprites[i].pos += (sprites[i].velocity * dt);
			glm::vec3 accel = sprites[i].acceleration;
			accel += (sprites[i].force * 1.0 / sprites[i].mass);
			sprites[i].velocity += accel * dt;
			sprites[i].velocity *= sprites[i].damping;

			sprites[i].setRotation(glm::degrees(-glm::orientedAngle(glm::normalize(posCur - sprites[i].pos), glm::vec3(0, -1, 0), glm::vec3(0, 0, 1))));
		}
		else if (sprites[i].agentType == 1) {
			sprites[i].rot += 5;
			sprites[i].setRotation(sprites[i].rot);
		}
	}
}

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetVerticalSync(true);

	ofSetBackgroundColor(ofColor::black);


	thrusterSound.load("sounds/thrusters_loopwav-14699.mp3");
	thrusterSound.setLoop(true);

	raySound.load("sounds/beam-8-43831.mp3");
	raySound.setLoop(true);

	explosionSound.load("sounds/transition-explosion-121425.mp3");

	playerOne = new Player(1);
	sys = new SpriteList();

	lastSpawned = 0;
	numSpawned = 0;
	rate = 1;
	//rate = 3;

	gui.setup();
	gui.add(energyLevelSlider.setup("Energy Level", 3, 1, 10));
	gui.add(lifeSpanSlider.setup("Lifespan", 15000, 1000, 30000));
	gui.add(rateSlider.setup("Agent Rate Spawn", 1, 0.5, 5));
	gui.add(numAgentsSlider.setup("Number of Agents", 10, 5, 30));
	gui.add(difficultySlider.setup("Difficulty", 0, 0, 3));
	bHide = false;

	movingForward = false;
	movingBackward = false;
	turningLeft = false;
	turningRight = false;



	explosionEmitter = new ParticleEmitter();
	explosionEmitter->type = RadialEmitter;
	explosionEmitter->setGroupSize(100);
	explosionEmitter->setParticleRadius(10);

	explosionEmitterTwo = new ParticleEmitter();
	explosionEmitterTwo->type = RadialEmitter;
	explosionEmitterTwo->setGroupSize(100);
	explosionEmitterTwo->setParticleRadius(10);

	rayEmitter = new ParticleEmitter();
	rayEmitter->type = DirectionalEmitter;
	rayEmitter->setVelocity(ofVec3f(100, 100, 0));
	rayEmitter->setGroupSize(1);
	rayEmitter->setParticleRadius(5);

	score = 0;
}

//--------------------------------------------------------------
void ofApp::update() {
	ofSoundUpdate();

	explosionEmitter->update();
	explosionEmitterTwo->update();

	playerOne->update();

	rayEmitter->setPos(playerOne->pos);
	rayEmitter->setVelocity(ofVec3f(500 * glm::sin(glm::radians(playerOne->rot)), -500 * glm::cos(glm::radians(playerOne->rot)), 0));
	rayEmitter->setGroupSize(1);
	rayEmitter->update();




	if (movingForward) {
		moved = ofVec3f(-15 * glm::sin(glm::radians(playerOne->rot)), 15 * glm::cos(glm::radians(playerOne->rot)), 0);

		wallCollision(moved);
	}
	else if (movingBackward) {
		moved = ofVec3f(-15 * glm::sin(glm::radians(playerOne->rot)), 15 * glm::cos(glm::radians(playerOne->rot)), 0);

		wallCollision(-moved);
	}

	if (turningLeft) {
		//playerOne->rot -= glm::radians((float)180);
		wallRotateCollision(false);
	}
	else if (turningRight) {
		//playerOne->rot += glm::radians((float)180);
		wallRotateCollision(true);
	}


	
	for (int iii = 0; iii < sys->listSize(); iii++) {
		
	}




	//player->rot += rotationAmount;
	//player->update();
	maxEnergyLevel = energyLevelSlider;
	rate = rateSlider;
	numAgents = numAgentsSlider;

	int difficultyLevel = difficultySlider;

	if (difficultyLevel == 1) {
		maxEnergyLevel = 3;
		rate = 1;
		numAgents = 6;
	}
	else if (difficultyLevel == 2) {
		maxEnergyLevel = 6;
		rate = 2;
		numAgents = 10;
	}
	else if (difficultyLevel == 3) {
		maxEnergyLevel = 10;
		rate = 3;
		numAgents = 15;
	}



	float time = ofGetElapsedTimeMillis();

	if ((time - lastSpawned) > (1000.0)) {
		// call virtual to spawn a new sprite
		//
		for (int ii = 0; ii < rate; ii++) {
			if (numSpawned < numAgents)
				spawnAgentOne();
		}


		lastSpawned = time;
	}

	sys->updatePlayerPos(playerOne->pos);
	//sys->updatePlayerAngle(player->pos);
	sys->update();
	sys->updatePlayerAngle(playerOne->pos);

	collisionCheck();

	if (ofGetElapsedTimeMillis() - explosionOneTimer >= 3000) {
		explosionEmitter->stop();
	}
	if (ofGetElapsedTimeMillis() - explosionTwoTimer >= 3000) {
		explosionEmitterTwo->stop();
	}

	if (sys->listSize() < numSpawned) {
		//numSpawned-=(int)rate;
		int temp = numSpawned - sys->listSize();
		numSpawned -= temp;
	}
}

//--------------------------------------------------------------
void ofApp::draw() {
	if (canRun && !isOver) {

		if (backgroundImage.load("images/BackgroundImageSmallerTwo.png") && toggleImageBackground) {
			backgroundImage.draw(0, 0);
		}
		else {
			ofSetBackgroundColor(ofColor::black);
		}




		

		playerOne->draw();
		sys->draw();



		//ofPushMatrix();
		//ofTranslate(ofGetWindowWidth() / 2, ofGetWindowHeight() / 2);

		explosionEmitter->draw();
		explosionEmitterTwo->draw();
		rayEmitter->draw();

		//ofPopMatrix();




		if (!bHide) {
			gui.draw();
		}

		ofSetColor(255, 255, 255);
		string eText = "Total Energy Level: ";
		ofDrawBitmapString(eText, 100, 450);

		string eVal = std::to_string(playerOne->energyLevel) + "/ " + std::to_string(maxEnergyLevel);
		ofDrawBitmapString(eVal, 250, 450);

		string eTime = "Elapsed Time: " + std::to_string(ofGetElapsedTimeMillis());
		ofDrawBitmapString(eTime, 100, 500);

		string eFrameRate = "Frame Rate: " + std::to_string(ofGetFrameRate());
		ofDrawBitmapString(eFrameRate, 100, 550);

		string eScore = "Player Score: " + std::to_string(score);
		ofDrawBitmapString(eScore, 100, 600);

	}
	else {
		ofSetColor(255, 255, 255);
		string eText = "Welcome to PolySpace Wars! ";
		ofDrawBitmapString(eText, 300, 200);
		eText = "Press Space to Start and Restart the Game ";
		ofDrawBitmapString(eText, 300, 250);
		eText = "Press C to toggle between sprites and images ";
		ofDrawBitmapString(eText, 300, 300);
		eText = "Press S to fire the particle ray at enemies ";
		ofDrawBitmapString(eText, 300, 350);
		eText = "Score as many points possible by shooting enemies ";
		ofDrawBitmapString(eText, 300, 400);
	}
	
	
}

void ofApp::spawnAgentOne() {
	if (numSpawned <= numAgents) {
		AgentOne sprite;
		sprite.setLifespan(lifeSpanSlider);
		sprite.birthtime = ofGetElapsedTimeMillis();
		sys->add(sprite);
		numSpawned++;
	}
	
}

void ofApp::collisionCheck() {
	if (sys->sprites.size() == 0) return;


	for (int ii = 0; ii < sys->listSize(); ii++) {
		glm::mat4 Mat = sys->sprites[ii].getMatrix();

		for (int jj = 0; jj < sys->sprites[ii].verts.size(); jj++) {

			if (playerOne->inside(Mat * glm::vec4(sys->sprites[ii].verts[jj], 1))) {
				if (ii + 1 != sys->listSize()) {
					explosionEmitter->setPos(sys->sprites[ii].pos);

					sys->remove(ii);
					explosionSound.play();

					explosionEmitter->start();
					explosionOneTimer = ofGetElapsedTimeMillis();
				}
				playerOne->energyLevel--;
				cout << "Energy Levels " << playerOne->energyLevel << "\n";
				if (playerOne->energyLevel <= 0) {
					cout << "Game Over\n";
					cout << "Restart Game? Press Spacebar\n";
					canRun = false;
					isOver = true;
					score = 0;
				}
				//cout << "Energy Levels " << player->energyLevel << "\n";
			}
		}
	}

	

	// check which sprites have exceed their lifespan and delete
	// from list.  When deleting multiple objects from a vector while
	// traversing at the same time, use an iterator.
	//
	for (Particle p : rayEmitter->sys->particles) {
		vector<AgentOne>::iterator s = sys->sprites.begin();
		vector<AgentOne>::iterator tmp;

		while (s != sys->sprites.end()) {
			ofVec3f p1 = ofVec3f(p.position.x + p.radius, p.position.y, p.position.z);
			ofVec3f p2 = ofVec3f(p.position.x, p.position.y + p.radius, p.position.z);
			ofVec3f p3 = ofVec3f(p.position.x - p.radius, p.position.y, p.position.z);
			ofVec3f p4 = ofVec3f(p.position.x, p.position.y - p.radius, p.position.z);

			bool insideEnemy = s->inside(p.position) || s->inside(p1)
				|| s->inside(p2) || s->inside(p3) ||
				s->inside(p4);

			if (insideEnemy) {
				explosionEmitterTwo->setPos(s->pos);

				tmp = sys->sprites.erase(s);
				s = tmp;
				explosionSound.play();

				explosionEmitterTwo->start();
				explosionTwoTimer = ofGetElapsedTimeMillis();

				score++;
			}
			else s++;
		}
	}
	

	clearAllEnemies(isOver);
}

void ofApp::wallCollision(glm::vec3 delta) {
	glm::mat4 Mat = playerOne->getMatrix();
	glm::vec3 p1 = Mat * glm::vec4(glm::vec3(-30, 30, 0), 1);
	glm::vec3 p2 = Mat * glm::vec4(glm::vec3(30, 30, 0), 1);
	glm::vec3 p3 = Mat * glm::vec4(glm::vec3(0, -30, 0), 1);

	glm::vec3 p1Delta = p1 - delta;
	glm::vec3 p2Delta = p2 - delta;
	glm::vec3 p3Delta = p3 - delta;


	if ((p1Delta.x >= 0 && p1Delta.x <= ofGetWindowWidth()) &&
		(p2Delta.x >= 0 && p2Delta.x <= ofGetWindowWidth()) &&
		(p3Delta.x >= 0 && p3Delta.x <= ofGetWindowWidth())) {
		playerOne->pos.x -= delta.x;
	}
	if ((p1Delta.y >= 0 && p1Delta.y <= ofGetWindowHeight()) &&
		(p2Delta.y >= 0 && p2Delta.y <= ofGetWindowHeight()) &&
		(p3Delta.y >= 0 && p3Delta.y <= ofGetWindowHeight())) {
		playerOne->pos.y -= delta.y;
	}
}

void ofApp::wallRotateCollision(bool isRight) {
	glm::mat4 Mat = playerOne->getMatrix();
	glm::vec3 p1 = Mat * glm::vec4(glm::vec3(-30, 30, 0), 1);
	glm::vec3 p2 = Mat * glm::vec4(glm::vec3(30, 30, 0), 1);
	glm::vec3 p3 = Mat * glm::vec4(glm::vec3(0, -30, 0), 1);

	//float curRot = playerOne->rot;
	if (isRight) {
		if ((p1.x >= 0 && p1.x <= ofGetWindowWidth()) &&
			(p2.x >= 0 && p2.x <= ofGetWindowWidth()) &&
			(p3.x >= 0 && p3.x <= ofGetWindowWidth())) {
			playerOne->rot += glm::radians((float)180);
		}
		if ((p1.y >= 0 && p1.y <= ofGetWindowHeight()) &&
			(p2.y >= 0 && p2.y <= ofGetWindowHeight()) &&
			(p3.y >= 0 && p3.y <= ofGetWindowHeight())) {
			playerOne->rot += glm::radians((float)180);
		}
		//curRot += glm::radians((float)180);

	}
	else {
		if ((p1.x >= 0 && p1.x <= ofGetWindowWidth()) &&
			(p2.x >= 0 && p2.x <= ofGetWindowWidth()) &&
			(p3.x >= 0 && p3.x <= ofGetWindowWidth())) {
			playerOne->rot -= glm::radians((float)180);
		}
		if ((p1.y >= 0 && p1.y <= ofGetWindowHeight()) &&
			(p2.y >= 0 && p2.y <= ofGetWindowHeight()) &&
			(p3.y >= 0 && p3.y <= ofGetWindowHeight())) {
			playerOne->rot -= glm::radians((float)180);
		}
		//curRot -= glm::radians((float)180);
	}
}

void ofApp::clearAllEnemies(bool isOver) {
	if (isOver) {
		for (int ii = 0; ii < sys->listSize(); ii++) {
			sys->remove(ii);
		}
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	switch (key) {
	case 'C':
	case 'c':
		temp = toggleImageBackground;
		toggleImageBackground = !temp;
		temp = playerOne->showImage;
		playerOne->showImage = !temp;
		temp = sys->allSpriteImages;
		sys->allSpriteImages = !temp;


		break;
	case 'F':
	case 'f':
		ofToggleFullscreen();
		break;
	case 'H':
	case 'h':
		bHide = !bHide;
		break;
	case 'r':
		break;
	case 's':
		rayEmitter->start();
		raySound.play();
		break;
	case 'u':
		break;
	case 32:
		canRun = true;
		isOver = false;
		playerOne->energyLevel = maxEnergyLevel;
		playerOne->pos = ofVec3f(500, 350, 0);
		playerOne->rot = 0;
		break;
	case OF_KEY_ALT:
		break;
	case OF_KEY_CONTROL:
		break;
	case OF_KEY_SHIFT:
		break;
	case OF_KEY_DEL:
		break;
	case OF_KEY_LEFT:
		turningLeft = true;
		break;
	case OF_KEY_RIGHT:
		turningRight = true;
		break;
	case OF_KEY_UP:
		movingForward = true;
		thrusterSound.play();
		break;
	case OF_KEY_DOWN:
		movingBackward = true;
		thrusterSound.play();
		break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
	switch (key) {
	case OF_KEY_LEFT:
		turningLeft = false;
		break;
	case OF_KEY_RIGHT:
		turningRight = false;
		break;
	case OF_KEY_UP:
		movingForward = false;
		thrusterSound.stop();
		break;
	case OF_KEY_DOWN:
		movingBackward = false;
		thrusterSound.stop();
		break;
	case OF_KEY_ALT:
		break;
	case OF_KEY_CONTROL:
		break;
	case OF_KEY_SHIFT:
		break;
	case 's':
		rayEmitter->stop();
		raySound.stop();
		break;
	}
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
	//rayEmitter->start();
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
	//rayEmitter->stop();
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
