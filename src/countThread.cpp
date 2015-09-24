//
//  countThread.cpp
//  arStepSequencer
//
//  Created by 永松 歩 on 2015/09/24.
//
//

#include "countThread.hpp"

countThread::countThread(){
    b_new = false;
    bpm = 120;
}

void countThread::threadedFunction(){
    // 150beat/min -> 2.5beat/sec -> 400msec/beat -> 25msec * 16times
    
    while (isThreadRunning()) {
        if (lock()) {
            float msecPerBeat = 60 * 1000 / (bpm * 4);
            b_new = true;
            unlock();
            
            sleep((int)msecPerBeat);
        } else {
            // If we reach this else statement, it means that we could not
            // lock our mutex, and so we do not need to call unlock().
            // Calling unlock without locking will lead to problems.
            ofLogWarning("threadedFunction()") << "Unable to lock mutex.";
        }
    }
}

void countThread::setBPM(int _bpm){
    if (lock()) {
        bpm = _bpm;
        unlock();
    } else {
        // If we reach this else statement, it means that we could not
        // lock our mutex, and so we do not need to call unlock().
        // Calling unlock without locking will lead to problems.
        ofLogWarning("threadedFunction()") << "Unable to lock mutex.";
    }
}

bool countThread::isNewBeat(){
    ofScopedLock lock(mutex);
    if (b_new) {
        b_new = false;
        return true;
    } else {
        return b_new;
    }
}