#pragma once

#include "ofMain.h"
#include "Header.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
		ofVec3f pos = { ofGetScreenWidth() / 2, ofGetScreenHeight() / 2,0 };
		double player_power;
		double player_population;
		double total_stars;
		double total_power;
		NodeSystem system{pos, &player_power, &player_population, &total_stars, &total_power };

		ofTrueTypeFont playerPowerFont;
		ofTrueTypeFont playerPopulationFont;
		ofTrueTypeFont totalStarsFont;
		ofTrueTypeFont totalPowerFont;

		ofTrueTypeFont defeat;
		ofTrueTypeFont victory;
		ofTrueTypeFont task;
		ofTrueTypeFont population_death;

		int num_clicks;
		ofCamera cam;

		bool placed_first_colony;



		string p;
		string p2;
		string p3;
		string p4;

		string defeat_string;
		string victory_string;
		string task_string;


};
