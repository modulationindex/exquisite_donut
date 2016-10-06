#include "sprinkle.h"

//duplicate defines from donut_cop. couldn't think of a faster way to do this
#define MAXV 0.01 //0.65//switching to pixels / sec orig val: 0.01
#define MAXA 0.001

#define PIXEL_VEL_MAX .8
#define PIXEL_ACCEL_MAX 2.
#define TARGET_RAD 40
//--------------------------------------------------------------
// Generate the sprinkle with random parameters
Sprinkle::Sprinkle(float maxVel, float maxAcc) {
    
    maxVel = ofMap(maxVel, 0,MAXV, 0, PIXEL_VEL_MAX );
    maxAcc = ofMap(maxAcc, 0, MAXA, 0, PIXEL_ACCEL_MAX);
    
    
    maxY = 1920./1080.;
    
    x = ofRandomuf();
    y = ofMap(ofRandomuf(),0.0,1.0,0.0,maxY);
    xVel = ofRandomf() * maxVel;
    yVel = ofRandomf() * maxVel;
    xAcc = ofRandomf() * maxAcc;
    yAcc = ofRandomf() * maxAcc;
    free1 = ofRandomuf();
    free2 = ofRandomuf();
    rad = 0.1;
    bDidEat = false;
    bEating = false;
    lastFrameTime = ofGetElapsedTimeMillis();
}

//--------------------------------------------------------------
// Generate the sprinkle from an OSC message
Sprinkle::Sprinkle(const ofxOscMessage &m) {

    bDidEat = false;
    rad = TARGET_RAD;
    bEating = false;
  maxY = 1920./1080.;

  x = 0;
  y = m.getArgAsFloat(0);
  xVel = ofMap(m.getArgAsFloat(1),-MAXV,MAXV,-PIXEL_VEL_MAX,PIXEL_VEL_MAX);
  yVel = ofMap(m.getArgAsFloat(2),-MAXV,MAXV,-PIXEL_VEL_MAX,PIXEL_VEL_MAX);
  xAcc = ofMap(m.getArgAsFloat(3),-MAXA,MAXA,-PIXEL_ACCEL_MAX,PIXEL_ACCEL_MAX);
  yAcc = ofMap(m.getArgAsFloat(4),-MAXA,MAXA,-PIXEL_ACCEL_MAX,PIXEL_ACCEL_MAX);
  free1 = m.getArgAsFloat(5);
  free2 = m.getArgAsFloat(6);

  // Handle starting on the right
  if (xVel < 0 ) { x = 1.0;}
    targetThickness = 50;
    thickness = 50;
    lastFrameTime = ofGetElapsedTimeMillis();

}

//--------------------------------------------------------------
void Sprinkle::update(float maxVel, float maxAcc) {
    maxVel = ofMap(maxVel, 0,MAXV, 0, PIXEL_VEL_MAX );
    maxAcc = ofMap(maxAcc, 0, MAXA, 0, PIXEL_ACCEL_MAX);
    

    float elapsedTimeSec = 0.001 * (ofGetElapsedTimeMillis() - lastFrameTime);
    
    if (!bEating && !bDidEat && !(fabsf(rad - TARGET_RAD) < 0.001)){
        float leakyV = 0.08 * ofGetTargetFrameRate() * elapsedTimeSec;
        rad = rad * (1.- leakyV) + leakyV*TARGET_RAD ;
        rad = ofClamp(rad,0.1,TARGET_RAD);
    }
    
    // bounce off top and bottom
  if (y >= maxY || y <= 0.0) {
    yVel *= -1.0;
    yAcc *= -1.0;
      y += (yVel * elapsedTimeSec);
  }

  x += (xVel * elapsedTimeSec);
  y += (yVel * elapsedTimeSec);
  xVel += (xAcc * elapsedTimeSec);
  yVel += (yAcc * elapsedTimeSec);
  xVel = ofClamp(xVel,-maxVel, maxVel);
  yVel = ofClamp(yVel, -maxVel, maxVel);
  xAcc = maxAcc*ofRandomf();
  yAcc = maxAcc*ofRandomf();
    
    
    lastFrameTime = ofGetElapsedTimeMillis();
    
    if (bDidEat == true){
        bEating = false;
        x = 50000;
    }
    
    //thickness = 0.9 * thickness + 0.1 * targetThickness;
}

bool Sprinkle::isEating(){
    return bEating;
}

bool Sprinkle::didEat(){
    return (bDidEat);
}

void Sprinkle::kill(){
    if (bEating || bDidEat) return;
    Tweenzor::add(&rad,rad,0.01f,.2f,1.5f,EASE_OUT_CUBIC);
    bEating = true;
    bDidEat = false;
    Tweenzor::addCompleteListener(Tweenzor::getTween(&rad), this, &Sprinkle::killComplete);
}

void Sprinkle::killComplete(float *args){
    //hack - move way offscreen for deletion
    bDidEat = true;
}

float Sprinkle::getScreenX(){
    return ofMap(x,0.0, 1.0, 0.0, 1080.);
}

float Sprinkle::getScreenY(){
    return ofMap(y,0.,maxY,0.,1920.);
}


//--------------------------------------------------------------
void Sprinkle::draw() {
  ofFill();
  float xPos = getScreenX();
    float yPos = getScreenY();
    ofSetColor(255,0,0);
    ofDrawCircle(xPos,yPos,rad +  free1 * sin(TWO_PI * (0.1 + 3. * free2) * ofGetElapsedTimeMillis()*0.001)*10.);
    // ofDrawRectangle(xPos-0.5*rad, yPos-0.5*rad, rad,rad);
}

void Sprinkle::drawMin() {
    ofFill();
    float xPos = getScreenX();
    float yPos = getScreenY();
    ofSetColor(255,255,255);
    ofDrawCircle(xPos,yPos,2);
}

//--------------------------------------------------------------
bool Sprinkle::isOffScreen() {
  return ((x > 1 || x < 0));
}

//--------------------------------------------------------------
ofxOscMessage Sprinkle::createOSCMessage() const {

  ofxOscMessage m;
  m.addFloatArg(y);
    m.addFloatArg(ofMap(xVel,-PIXEL_VEL_MAX,PIXEL_VEL_MAX,-MAXV, MAXV));
  m.addFloatArg(ofMap(yVel,-PIXEL_VEL_MAX,PIXEL_VEL_MAX,-MAXV, MAXV));
  m.addFloatArg(ofMap(xAcc,-PIXEL_ACCEL_MAX,PIXEL_ACCEL_MAX,-MAXA, MAXA));
  m.addFloatArg(ofMap(yAcc,-PIXEL_ACCEL_MAX,PIXEL_ACCEL_MAX,-MAXA, MAXA));
  m.addFloatArg(free1);
  m.addFloatArg(free2);
  return m;
}