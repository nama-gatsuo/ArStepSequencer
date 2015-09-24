#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    //ofSetVerticalSync(true);
    
    ofSetSmoothLighting(true);
    
    light0.setPosition(50, 300.0, 200.0);
    light0.setAmbientColor(ofFloatColor(0.2, 0.2, 0.2, 1.0));
    light0.setSpecularColor(ofFloatColor(0.9, 0.9, 0.9, 1.0));
    light0.setDiffuseColor(ofFloatColor(1.0, 1.0, 1.0, 1.0));
    light0.setDirectional();
    light0.lookAt(ofVec3f(0.0,0.0,0.0));
    
    light1.setPosition(-50, 300.0, 200.0);
    light1.setAmbientColor(ofFloatColor(0.2, 0.2, 0.2, 1.0));
    light1.setSpecularColor(ofFloatColor(0.9, 0.9, 0.9, 1.0));
    light1.setDiffuseColor(ofFloatColor(1.0, 1.0, 1.0, 1.0));
    light1.setPointLight();
    light1.lookAt(ofVec3f(0.0,0.0,0.0));
    
    row = 8;
    currentBar = 1;
    framePerBeat = 7;
    
    ofBackground(40);
    
    midiOut.openPort(0);
    
    // AR
    width = 960;
    height = 540;
    
    vidGrabber.setVerbose(true);
    vidGrabber.setDeviceID(1);
    vidGrabber.initGrabber(width, height);
    
    colorImage.allocate(width, height);
    grayImage.allocate(width, height);
    
    artk.setup(width, height);
    
    threshold = 160;
    artk.setThreshold(threshold);
    
    bpm = 120;
    thread.startThread();
    
}

//--------------------------------------------------------------
void ofApp::update(){
    vidGrabber.update();
    if (vidGrabber.isFrameNew()) {
        colorImage.setFromPixels(vidGrabber.getPixels(), width, height);
        grayImage = colorImage;
        artk.update(grayImage.getPixels());
    }
    
    if (thread.isNewBeat()) {
        currentBar++;
        
        if (currentBar == 17) {
            currentBar = 1;
        }
        triggerBangs();
    }
    
    if (boxes.size() > 0) {
        vector<soundBox>::iterator it;
        
        for (it = boxes.begin(); it < boxes.end(); it++) {
            it->update();
        }
    }

}

void ofApp::triggerBangs() {
    if (boxes.size() > 0) {
        vector<soundBox>::iterator it;
        
        for (it = boxes.begin(); it < boxes.end(); it++) {
            if (it->x == currentBar - 1) {
                it->applyForce(ofVec3f(0.0, 0.0, 2.0));
                switch (it->y) {
                    case 0:
                        midiOut.sendNoteOn(1, 36, 120);
                        break;
                    case 1:
                        midiOut.sendNoteOn(1, 38, 120);
                        break;
                    case 2:
                        midiOut.sendNoteOn(1, 40, 120);
                        break;
                    case 3:
                        midiOut.sendNoteOn(1, 42, 120);
                        break;
                    case 4:
                        midiOut.sendNoteOn(1, 44, 120);
                        break;
                    case 5:
                        midiOut.sendNoteOn(1, 48, 120);
                        break;
                    case 6:
                        midiOut.sendNoteOn(1, 50, 120);
                        break;
                    case 7:
                        midiOut.sendNoteOn(1, 51, 120);
                        break;
                    default:
                        break;
                }
            }
        }
    }
}


//--------------------------------------------------------------
void ofApp::draw(){
    ofSetHexColor(0xfffffff);
    colorImage.draw(0, 0);
    
    //artk.draw();
    
    ofSetColor(255);
    string msg = "fps: " + ofToString(ofGetFrameRate(), 2) +"\nBPM: " + ofToString(bpm, 2);
    ofDrawBitmapString(msg, 10, 20);
    
    artk.applyProjectionMatrix();
    
    int numDetected = artk.getNumDetectedMarkers();
    
    glPushMatrix();
    for (int i = 0; i < numDetected; i++) {
        artk.applyModelMatrix(i);
        ofEnableDepthTest();
        ofEnableLighting();
        light0.enable();
        light1.enable();
        
        ofSetColor(40);
        ofRect(tileRange * (-9), tileRange * (-row/2 - 1), tileRange * 18, tileRange * (row +2));
        drawTile();
        drawSelector();
        
        if (boxes.size() > 0) {
            vector<soundBox>::iterator it;
            for (it = boxes.begin(); it < boxes.end(); it++) {
                it->display();
            }
        }
        
        //light0.draw();
        //light1.draw();
        
        light0.disable();
        light1.disable();
        ofDisableLighting();
        ofDisableDepthTest();
    }
    glPopMatrix();
}

void ofApp::drawTile() {
    
    ofSetColor(255);
    for (int y = 0; y < row+1; y++) {
        for (int x = 0; x < 17; x++) {
            
            ofVec2f center = ofVec2f(tileRange*(x-8), tileRange*(y-row/2));
            
            glBegin(GL_LINES);
            
            glVertex3f(center.x, center.y - tileRange/10, 0.2);
            glVertex3f(center.x, center.y + tileRange/10, 0.2);
            
            glVertex3f(center.x - tileRange/10, center.y, 0.2);
            glVertex3f(center.x + tileRange/10, center.y, 0.2);
            
            glEnd();
        }
    }

    ofDrawBitmapString("Kick", tileRange * (selector.x - 9), tileRange * (- 3.5), tileRange);
    ofDrawBitmapString("Snare", tileRange * (selector.x - 9), tileRange * (- 2.5), tileRange);
    ofDrawBitmapString("Clave", tileRange * (selector.x - 9), tileRange * (- 1.5), tileRange);
    ofDrawBitmapString("HiHat", tileRange * (selector.x - 9), tileRange * (- 0.5), tileRange);
    ofDrawBitmapString("Maracas", tileRange * (selector.x - 9), tileRange * 0.5, tileRange);
    ofDrawBitmapString("CongaMid", tileRange * (selector.x - 9), tileRange * 1.5, tileRange);
    ofDrawBitmapString("CongaHi", tileRange * (selector.x - 9), tileRange * 2.5, tileRange);
    ofDrawBitmapString("Cowbell", tileRange * (selector.x - 9), tileRange * 3.5, tileRange);
    
    glPushMatrix();
    ofSetColor(100);
    ofTranslate(tileRange * ((float)currentBar - 9), tileRange * (-row/2), 0.5);
    
    glBegin(GL_QUADS);
    glVertex3f(0, 0, 0);
    glVertex3f(0, tileRange * row, 0);
    glVertex3f(tileRange, tileRange * row, 0);
    glVertex3f(tileRange, 0, 0);
    glEnd();
    
    glPopMatrix();
}

void ofApp::drawSelector() {
    glPushMatrix();
    ofTranslate(tileRange * (selector.x - 8), tileRange * (selector.y - row/2));
    
    ofSetColor(70);
    
    glBegin(GL_QUADS);
    glVertex3f(0, 0, 1.0);
    glVertex3f(tileRange, 0, 1.0);
    glVertex3f(tileRange, tileRange, 1.0);
    glVertex3f(0, tileRange, 1.0);

    glEnd();
    
    ofSetColor(255);
    
    string msg = "(" + ofToString(selector.x) + "," + ofToString(selector.y) + ")";
    ofDrawBitmapString(msg, 0.5, 0.1);
    
    glPopMatrix();
}

void ofApp::exit() {
    midiOut.closePort();
    thread.stopThread();
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key) {
        case OF_KEY_UP:
            selector.y++;
            break;
        case OF_KEY_DOWN:
            selector.y--;
            break;
        case OF_KEY_LEFT:
            selector.x--;
            break;
        case OF_KEY_RIGHT:
            selector.x++;
            break;
            
        case ' ':
            boxes.push_back(soundBox(selector.x, selector.y,
                                     ofVec3f(tileRange * (selector.x - 8 + 0.5),
                                             tileRange * (selector.y - row/2 + 0.5),
                                             tileRange/2)));
            break;
        case '+':
            bpm++;
            thread.setBPM(bpm);
            break;
        case '-':
            bpm--;
            thread.setBPM(bpm);
            break;
        case 'x':
            vector<soundBox>::iterator it;
            for (it = boxes.begin(); it < boxes.end(); it++) {
                if (it->x == selector.x && it->y == selector.y) {
                    boxes.erase(it);
                }
            }
            break;

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

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

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
