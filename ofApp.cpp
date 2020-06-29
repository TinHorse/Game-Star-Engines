#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(255);
	//ofSetBackgroundAuto(false);
	ofSetFrameRate(100);
	ofSetFullscreen(true);
	ofSetRectMode(OF_RECTMODE_CENTER);
	ofSetLineWidth(0.5);

	player_power = 10;
	player_population = 0;
	playerPowerFont.load("calibri", 15);
	playerPopulationFont.load("calibri", 15);
	totalStarsFont.load("calibri", 15);
	totalPowerFont.load("calibri", 15);

	defeat.load("calibri", 200);
	victory.load("calibri", 200);
	task.load("calibri", 15);
	population_death.load("calibri", 20);

	num_clicks = 0;

	defeat_string = "DEFEAT";
	victory_string = "VICTORY";
	task_string = "Reach 10 colonies";
}

//--------------------------------------------------------------
void ofApp::update(){
	//cout << player_power << endl;
}

//--------------------------------------------------------------
void ofApp::draw(){
	p = to_string(int(player_power));
	p2 = to_string(int(player_population));
	p3 = to_string(int(total_stars));
	p4 = to_string(int(total_power));

	p.append("   power");
	p2.append("  colonies");
	p3.append("  stars");
	p4.append("  power");



	system.run();

	/*
	if (ofGetFrameNum() % 100 == 0) {
		cout << "spawn" << endl;
		//ofVec3f centre = { float(ofGetScreenWidth() / 2), float(ofGetScreenHeight() / 2), 0 };
		//ofVec3f spawn = { ofRandom(ofGetFrameNum() / 10),ofRandom(ofGetFrameNum() / 10),0 };
		ofVec3f rand = { ofRandom(-800,800),ofRandom(-450,450), 0 };
		ofVec3f pos = { float(ofGetScreenWidth() / 2), float(ofGetScreenHeight() / 2),0 };
		system.colonize();
		//system.spawnNode(true, ofVec3f(rand+pos));
	}
	*/
	
	ofSetColor(0);
	playerPowerFont.drawString(p, ofGetScreenWidth() - 130, 100);
	playerPowerFont.drawString(p2, ofGetScreenWidth() - 130, 60);
	totalStarsFont.drawString(p3, 50, 60);
	totalPowerFont.drawString(p4, 50, 100);

	ofSetColor(200, 100, 0);
	totalPowerFont.drawString(task_string, ofGetScreenWidth() - 160, 20);

	if (system.placedFirstColony()) {
		if (player_population <= 0) {
			ofSetColor(200, 0, 0);
			totalPowerFont.drawString(defeat_string, ofGetScreenWidth() / 2, ofGetScreenHeight() / 2);
		}
		else if (player_population >= 10) {
			ofSetColor(100, 200, 0);
			totalPowerFont.drawString(victory_string, ofGetScreenWidth() / 2, ofGetScreenHeight() / 2);
		}
	}
	

	//cout << ofGetFrameRate() << endl;
	
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == ' ') {
		if (player_power >= 5) {
			if (system.colonize()) {
				player_power -= 5;
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	//system.spawnNode(ofVec3f(x - ofGetScreenWidth() / 2, y - ofGetScreenHeight() / 2, 1));
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	num_clicks++;
	//system.spawnNode(ofVec3f(x- ofGetScreenWidth() / 2, y - ofGetScreenHeight()/2, 1));
	if (player_power >= 1) {
		system.spawnNode(true, ofVec3f(x, y, 1));
		player_power--;
		if (num_clicks == 1) {
			system.colonize();
		}
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
