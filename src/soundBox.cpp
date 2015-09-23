//
//  soundBox.cpp
//  arStepSequencer
//
//  Created by 永松 歩 on 2015/09/20.
//
//

#include "soundBox.hpp"

soundBox::soundBox(int _x, int _y, ofVec3f pos){
    x = _x;
    y = _y;
    location = pos;
    acceleration = ofVec3f(0.0, 0.0, 0.0);
    velocity = ofVec3f(0.0, 0.0, 0.0);
}

soundBox::~soundBox(){
}

void soundBox::update(){
    applyForce(ofVec3f(0.0, 0.0, -0.3)); // garavity
    velocity += acceleration;
    location += velocity;
    
    if (location.z < 8.0) {
        location.z = 8.0;
        velocity = ofVec3f(0.0, 0.0, 0.0);
        acceleration = ofVec3f(0.0, 0.0, 0.0);
    }
}

void soundBox::display(){
    ofColor hsb;
    hsb.setHsb(120 + 10*y, 100, 100);
    ofSetColor(hsb);
    ofDrawBox(location, size);
}

void soundBox::applyForce(ofVec3f force){
    force /= mass;
    acceleration += force;
}

