//This expanded example combines the ofxLeapMotion openFrameworks add-on examples from https://github.com/ofTheo/ofxLeapMotion with additional functionality.
//
//Code and commenting was done in collaboration between Glenna Xie (https://github.com/glennax) & Jinnie Templin (https://github.com/jin-temp)
//
#pragma once

#include "ofMain.h"
#include "ofxLeapMotion.h"

class testApp : public ofBaseApp{

  public:
    void setup();
    void update();
    void draw();
	
    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    void exit();
		
	ofxLeapMotion leap;
	vector <ofxLeapMotionSimpleHand> simpleHands;
    
	vector <int> fingersFound; 
	ofEasyCam cam;
	ofLight l1;
	ofLight l2;
	ofMaterial m1;
    ofPolyline handPolyline;
    
	map <int, ofPolyline> fingerTrails;
    
  };
