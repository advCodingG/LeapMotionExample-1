//This expanded example combines the ofxLeapMotion openFrameworks add-on examples from https://github.com/ofTheo/ofxLeapMotion with additional functionality.
//
//Code and commenting was done in collaboration between Glenna Xie (https://github.com/glennax) & Jinnie Templin (https://github.com/jin-temp)
//
#include "testApp.h"

/* Note on OS X, you must have this in the Run Script Build Phase of your project. 
where the first path ../../../addons/ofxLeapMotion/ is the path to the ofxLeapMotion addon. 

cp -f ../../../addons/ofxLeapMotion/libs/lib/osx/libLeap.dylib "$TARGET_BUILD_DIR/$PRODUCT_NAME.app/Contents/MacOS/libLeap.dylib"; install_name_tool -change ./libLeap.dylib @executable_path/libLeap.dylib "$TARGET_BUILD_DIR/$PRODUCT_NAME.app/Contents/MacOS/$PRODUCT_NAME";

   If you don't have this you'll see an error in the console: dyld: Library not loaded: @loader_path/libLeap.dylib
*/

//--------------------------------------------------------------
void testApp::setup(){

    ofSetFrameRate(60);
    ofSetVerticalSync(true);
	ofSetLogLevel(OF_LOG_VERBOSE);

	leap.open();
    leap.setupGestures();
    
	l1.setPosition(200, 300, 50);
    l2.setPosition(0, 0 , 50);
	//l2.setPosition(-200, -200, 50);

	cam.setOrientation(ofPoint(-20, 0, 0));

	glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
}


//--------------------------------------------------------------
void testApp::update(){
    //clear the fingers from last frame and get the new hands
	fingersFound.clear();
    simpleHands = leap.getSimpleHands();
    
    if( leap.isFrameNew() && simpleHands.size() ){
    
        //map x, y and z coordinates from the leap onto the screen
        leap.setMappingX(-230, 230, -ofGetWidth()/2, ofGetWidth()/2);
		leap.setMappingY(90, 490, -ofGetHeight()/2, ofGetHeight()/2);
        leap.setMappingZ(-150, 150, -200, 200);
    
        //iterate through each hand
        for(int i = 0; i < simpleHands.size(); i++){
            //give each finger on the hand an identifier
            for(int j = 0; j < simpleHands[i].fingers.size(); j++){
                int id = simpleHands[i].fingers[j].id;
                //create polylines to represent each finger's movement
                ofPolyline & polyline = fingerTrails[id]; 
                ofPoint pt = simpleHands[i].fingers[j].pos;
                
                
                //if the distance between the last point and the current point is too big - lets clear the line 
                //this stops us connecting to an old drawing
                if( polyline.size() && (pt-polyline[polyline.size()-1] ).length() > 50 ){
                    polyline.clear(); 
                }
                
                //add our point to our trail
                polyline.addVertex(pt); 
                
                //store fingers seen this frame for drawing
                fingersFound.push_back(id);
            }
        }
        
        //give each hand a palm
        for(int i = 0; i < simpleHands.size(); i++){
        
            ofPoint palm = simpleHands[i].handPos;
                    
            // create a polyline for the palm movement, clear it after a certain length
            if( handPolyline.size() && (palm-handPolyline[handPolyline.size()-1] ).length() > 100 ){
                handPolyline.clear();
            }
                    
            handPolyline.addVertex(palm);

        }

    }

    //call the leap and see if any gestures have been detected
    leap.updateGestures();

	//IMPORTANT! - tell ofxLeapMotion that the frame is no longer new.
	leap.markFrameAsOld();	
}

//--------------------------------------------------------------
void testApp::draw(){
	ofDisableLighting();
    ofBackgroundGradient(ofColor(90, 90, 90), ofColor(30, 30, 30),  OF_GRADIENT_BAR);
	
	ofSetColor(255);
	//tell the user whether or not there is a leap connected
    ofDrawBitmapString("ofxLeapMotion - Example App\nLeap Connected? " + ofToString(leap.isConnected()), 20, 20);
    //tell the user what the different color trails indicate
    ofDrawBitmapString("Yellow polylines indicate finger gestures", 900, 20);
    ofDrawBitmapString("Turquoise polylines indicate hand gestures", 900, 40);
    
    //create a new string variable to determine what to print to the screen based on which gesture is detected by the leap
    string msg;

    
      switch (leap.iGestures) {
        case 1:
            msg = "Screen Tap";
            msg += "\nPosition: "+ofToString(leap.screenTapPosition);
            break;
            
        case 2:
            msg = "Key Tap";
            msg += "\nPosition: " + ofToString(leap.keyTapPosition);
            break;
            
        case 3:
            msg = "Swipe Right";
            msg += "\nSwipe Speed mm/s: " + ofToString(leap.swipeSpeed) + "\nDuration sec's: "+ofToString(leap.swipeDurationSeconds);
            break;
            
        case 4:
            msg = "Swipe Left";
            msg += "\nSwipe Speed mm/s: " + ofToString(leap.swipeSpeed) + "\nDuration sec's: "+ofToString(leap.swipeDurationSeconds);
            break;
            
        case 5:
            msg = "Swipe Down";
            msg += "\nSwipe Speed mm/s: " + ofToString(leap.swipeSpeed) + "\nDuration sec's: "+ofToString(leap.swipeDurationSeconds);
            break;
            
        case 6:
            msg = "Swipe Up";
            msg += "\nSwipe Speed mm/s: " + ofToString(leap.swipeSpeed) + "\nDuration sec's: "+ofToString(leap.swipeDurationSeconds);
            break;
            
        case 7:
            msg = "Swipe Forward";
            msg += "\nSwipe Speed mm/s: " + ofToString(leap.swipeSpeed) + "\nDuration sec's: "+ofToString(leap.swipeDurationSeconds);
            break;
            
        case 8:
            msg = "Swipe Backwards";
            msg += "\nSwipe Speed mm/s: " + ofToString(leap.swipeSpeed) + "\nDuration sec's: "+ofToString(leap.swipeDurationSeconds);
            break;
            
        case 9:
            msg = "Circle Left";
            msg += "\nCircle Radius: " + ofToString(leap.circleRadius) + "\nCircle Center: "+ofToString(leap.circleCenter);
            break;
            
        case 10:
            msg = "Circle Right";
            msg += "\nCircle Radius: " + ofToString(leap.circleRadius) + "\nCircle Center: "+ofToString(leap.circleCenter);
            break;
            
        default:
            msg = "Waiting for hand movement...";
            break;
    }
    
    ofSetColor(255);
    ofDrawBitmapString(msg, 20, 80);

	cam.begin();	

	// draw a 3d grid so we can see our hand in 3d space
    ofPushMatrix();
		ofRotate(90, 0,1, 0);
		ofSetColor(20);
		ofDrawGrid(600, 20, false);
	ofPopMatrix();
	
	//enable the lights so we can see the 3d space
    ofEnableLighting();
	l1.enable();
	l2.enable();
	
//	m1.begin();
//	m1.setShininess(1);
    
    
    //draw the hands detected by the leap
    for(int j = 0; j < simpleHands.size(); j++){
        
        simpleHands[j].debugDraw();
        
    }

    //if the leap detects hand gestures, draw the polyline of the gesture movement based on (palm position) in turquoise
    if(leap.iGestures == 3 || leap.iGestures == 4 || leap.iGestures == 5 || leap.iGestures == 6 || leap.iGestures == 7 || leap.iGestures == 8 ){
        
        for(int i = 0; i < simpleHands.size(); i++){
            ofSetColor(ofColor::turquoise);
            ofSetLineWidth(100);
            handPolyline.getSmoothed(20);
            handPolyline.simplify();
            handPolyline.draw();
        
        
        }
    //if the leap detects finger gestures, draw the polyline of the gesture movement in yellow
    }else if(leap.iGestures == 1 || leap.iGestures == 2 || leap.iGestures == 9 || leap.iGestures == 10){
    
        
        for(int i = 0; i < fingersFound.size(); i++){
            handPolyline.clear();
           
            int id = fingersFound[i];
            
            ofPolyline & polyline = fingerTrails[id];
            polyline.getSmoothed(10);
            
            ofSetColor(ofColor::yellow);
            ofSetLineWidth(20);
            polyline.draw();

        }


    
    }

	
    l2.disable();

//	m1.end();
	cam.end();
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
  
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){
    
}

//--------------------------------------------------------------
void testApp::exit(){
    // let's close down Leap and kill the controller
    leap.close();
}
