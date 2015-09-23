//
//  soundBox.hpp
//  arStepSequencer
//
//  Created by 永松 歩 on 2015/09/20.
//
//

#ifndef soundBox_hpp
#define soundBox_hpp

#include <stdio.h>
#include "ofMain.h"

#endif /* soundBox_hpp */

class soundBox {
public:
    soundBox(int _x, int _y, ofVec3f pos);
    ~soundBox();
    void update();
    void display();
    void applyForce(ofVec3f force);
    
    ofVec3f location;
    ofVec3f velocity;
    ofVec3f acceleration;
    
    int x, y;

private:
    float size = 15.0;
    float mass = 5.0;

};

