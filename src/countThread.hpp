//
//  countThread.hpp
//  arStepSequencer
//
//  Created by 永松 歩 on 2015/09/24.
//
//

#pragma once
#include "ofMain.h"

class countThread : public ofThread {
public:
    countThread();
    void threadedFunction();
    void setBPM(int _bpm);
    bool isNewBeat();

protected:
    bool b_new;
    int bpm;
    
};