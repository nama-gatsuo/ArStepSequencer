#pragma once

#include "ofMain.h"
#include "ofxMidi.h"
#include "ofxARToolkitPlus.h"
#include "ofxOpenCv.h"

#include "soundBox.hpp"
#include "countThread.hpp"

class ofApp : public ofBaseApp{
public:
    
    struct Selector {
        int x, y;
        
        Selector() : x(0), y(0) {}
    };
	
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    void exit();
    
    void triggerBangs();
    
    void drawTile();
    void drawSelector();
    
    unsigned int currentBar;
    unsigned int framePerBeat;
    int row;
    
    ofLight light0;
    ofLight light1;
    
    Selector selector;
    vector<soundBox> boxes;
    
    ofxMidiOut midiOut;
    int note, velocity;
    
    float tileRange = 16.0;
    
    // AR
    int width, height;
    ofVideoGrabber vidGrabber;
    
    ofxARToolkitPlus artk;
    int threshold;
    
    ofxCvColorImage colorImage;
    ofxCvGrayscaleImage grayImage;
    ofxCvGrayscaleImage grayThres;
    
    ofImage displayImage;
    vector<ofPoint> displayImageCorners;
    
    // count BPM
    int bpm;
    countThread thread;
		
};
