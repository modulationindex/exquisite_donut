#pragma once
#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxTweenzor.h"

class Sprinkle{
  public:
 
    // Constructors
    Sprinkle(const ofxOscMessage &m);
    Sprinkle(float maxVel, float maxAcc);
    
    // Standard OF functions
    void draw();
    void update(float maxVel, float maxAcc);
    
    // Custom functions
    ofxOscMessage createOSCMessage() const;
    bool isOffScreen();
    void kill();
    void drawMin();
    float getX() const {return x;};
    float getY() const {return y;};
    float targetThickness;
    bool isEating();
    bool didEat();
    void killComplete(float * args);
    float thickness;
    float getScreenX();
    float getScreenY();
  protected:
    float maxY;
    float x;
    float y;
    float xVel;
    float yVel;
    float xAcc;
    float yAcc;
    float free1;
    float free2;
    float rad = 40;
    bool bDidEat;
    bool bEating;
    float lastFrameTime;

};