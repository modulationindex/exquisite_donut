#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    setupGUI();
    if (_bLogVerbose){
        ofSetLogLevel(OF_LOG_VERBOSE);
    }
    
    ofBackground(0,0,0);
    ofSetFrameRate(_appFrameRate);
    ofSetVerticalSync(_bVerticalSyncOn);
    if (_bEnableSmoothing){
        ofEnableSmoothing();
    }
    
    ofHideCursor();
    _bShowFramerate = false;
    _bShowGui = false;
    
    donutCop.setId(_drawingId);
    
    Tweenzor::init();
    
    for (int i = 0; i < 14; i++){
        if (i >= 8){
            _lineSets.push_back(LineSet(pow(2,i - 8) + i , (i % 2 == 0)? false : true));
        }else{
            _lineSets.push_back(LineSet(2 + i , (i %2 == 0)? false : true));
            
        }
    }
    

    _lastSwitchTimeMillis = ofGetElapsedTimeMillis();
    
    _currentLineSet = 0;
    _lineSets.at(_currentLineSet).show();
    
    _lineTexture.allocate(1080,1920,GL_RGBA,4);
    _particleTexture.allocate(1080,1920,GL_RGBA,4);
    
    if(ofIsGLProgrammableRenderer()){
        //    _shader.load("shaders_gl3/red-mask.vert", "shaders_gl3/red-mask.frag");
    }else{
        _shader.load("shaders/red-mask.vert", "shaders/red-mask.frag");
    }
    

};

//--------------------------------------------------------------
void ofApp::update(){
    
    Tweenzor::update(ofGetElapsedTimeMillis());
  donutCop.update(sprinkles.size());

  // Update the sprinkle system
  for (auto& p : sprinkles) {
    p.update(donutCop.maxVelocity(),donutCop.maxAcceleration());
  }

  // add new sprinkles from messages
  while (donutCop.hasNewSprinkles()) {
    sprinkles.push_back(donutCop.getSprinkle());
  }

  createSprinkles();
  removeSprinkles();
    
    if (ofGetElapsedTimeMillis() - _lastSwitchTimeMillis > 30*1000.){
        advanceSet();
           }
    
    for (int i = 0 ;i < _lineSets.size(); i++){
        _lineSets.at(i).update(sprinkles);
    }
}


void ofApp::advanceSet(){
    _lastSwitchTimeMillis = ofGetElapsedTimeMillis();
    _lineSets.at(_currentLineSet).hide();
    _lineSets.at(_currentLineSet).toggleOpener();
    _currentLineSet = (_currentLineSet + 1) %_lineSets.size();
    _lineSets.at(_currentLineSet).show();
    ofLogVerbose() << "sprinkle count before eat" << sprinkles.size();
    eatSprinkles();
}

void ofApp::eatSprinkles(){
    int deleteFactor = .7;
    if (sprinkles.size() > MINP*deleteFactor){
        for (int i = 0; i < sprinkles.size() - MINP*deleteFactor;i++){
            sprinkles.at(i).kill();
        }
    }
}


//--------------------------------------------------------------
void ofApp::draw(){
    
    if (_currentLineSet < 6){
        for (auto& p : sprinkles) { p.drawMin();}
    }

    ofEnableAlphaBlending();
    _particleTexture.begin();
    ofSetColor(0,0,0,1750);
    ofDrawRectangle(0,0, 1080,1920);
    for (auto& p : sprinkles) { p.draw();}
    _particleTexture.end();
    
    _lineTexture.begin();
    ofClear(0,0,0,0);
    for (auto& p : _lineSets) { p.draw();}
    _lineTexture.end();
    
    _shader.begin();
    _shader.setUniformTexture("srcTex", _lineTexture.getTexture(), 0);
    _shader.setUniformTexture("maskTex",_particleTexture.getTexture(),1);
    drawShaderRect(0,0,1080,1920);
    _shader.end();
    
    
    if (_bShowFramerate){
        ofSetColor(255,0,0);
        stringstream s;
        s << "Frame Rate: " << ofGetFrameRate() << " (FPS)";
        ofDrawBitmapString(s.str(), 10, 10);
    }

    
    
    if (_bShowGui){
        _panel.draw();
    }

}

void ofApp::drawShaderRect(float x, float y, float w, float h){
    
    glBegin(GL_QUADS);
    glTexCoord2f(x, y); glVertex2f(x, y);
    glTexCoord2f(w, y); glVertex2f(w, y);
    glTexCoord2f(w, h); glVertex2f(w, h);
    glTexCoord2f(x, h); glVertex2f(x, h);
    glEnd();
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){}
void ofApp::keyPressed(int key) {
    
    if (key == 32){donutCop.setId(1);}
    if (key == 'a'){advanceSet();}
    if (key == 'd'){eatSprinkles();}
    if (key == 'f'){_bShowFramerate = !_bShowFramerate;}
    if (key == 'g'){_bShowGui = !_bShowGui;
        if (_bShowGui){
            ofShowCursor();
        }else{
            _settings.serialize(_params);
            _settings.save(ofToDataPath(ofToDataPath(kSettingsFileName)));

            ofHideCursor();
        }
    }
}

//--------------------------------------------------------------
void ofApp::createSprinkles() {

  // Check with the donut cop if you're allowed to create a new sprinkle
  if (donutCop.allowedToCreateSprinkle(sprinkles.size())) {

    // Create a new sprinkle
    Sprinkle p(donutCop.maxVelocity(), donutCop.maxAcceleration());

    // Add it to the sprinkles list
    sprinkles.push_back(p);
     

    // Tell the cop that we created one, so it can keep track
    // of how many have been created. 
    donutCop.mentionNewSprinkle();
  }
    
    if (sprinkles.size() > _eatSprinklesOverSize){
        int numToKill = sprinkles.size() - _eatSprinklesOverSize;
        ofLogVerbose() << "Eat " << numToKill << "front. Count exceeded " << _eatSprinklesOverSize << endl;
        
        for (int i= 0; i < numToKill; i++){
            if(!sprinkles.at(i).didEat() && !sprinkles.at(i).isEating()){
                sprinkles.at(i).kill();
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::removeSprinkles() {

  // Loop through and broadcast offscreen sprinkles
  for (auto& p : sprinkles) {
    if (p.isOffScreen() && !p.didEat() && !p.isEating()){
      donutCop.broadcastSprinkle(p);
    }
  }
  
  // Loop through and remove offscreen sprinkles 
  sprinkles.erase(
    remove_if(sprinkles.begin(), sprinkles.end(), [](Sprinkle p) { return p.isOffScreen();}),
  sprinkles.end());
}



void ofApp::drawingIdChanged(int & newId){
    donutCop.setId(newId);
    ofLogVerbose() << "My new id: " << newId;
}


void ofApp::appFrameRateChanged(int & newRate){
    ofSetFrameRate(newRate);
    ofLogVerbose() << "New Frame Rate" << newRate;
}

void ofApp::bVerticalSyncOnChanged(bool & newV){
    ofSetVerticalSync(newV);
    ofLogVerbose() << "VerticalSync " << ((newV) ? "enabled" : "disabled");
}

void ofApp::bEnableSmoothingChanged(bool & newV){
    if (newV){
        ofEnableSmoothing();
    }else{
        ofDisableSmoothing();
    }
    ofLogVerbose() << "Smoothing " << ((newV) ? "enabled" : "disabled");
}

void ofApp::bLogVerboseChanged(bool & newV){
    if (newV){
        ofSetLogLevel(OF_LOG_VERBOSE);
    }else{
        ofSetLogLevel(OF_LOG_SILENT);
    }
    
     ofLogVerbose() << "LogVerbose " << ((newV) ? "enabled" : "disabled");
    
}



void ofApp::setupGUI(){
    _params.add(_drawingId.set("drawingId",1,0,9));
    _params.add(_sceneDurationSec.set("sceneDurationSec",30,15,120));
    _params.add(_eatSprinklesOverSize.set("eatSprinklesOverSize",20,10,50));
    _params.add(_appFrameRate.set("appFrameRate", 60,30,200));
    _params.add(_bVerticalSyncOn.set("bVerticalSyncOn",true));
    _params.add(_bEnableSmoothing.set("bEnableSmoothing",true));
    _params.add(_bLogVerbose.set("bLogVerbose",false));

    _drawingId.addListener(this, &ofApp::drawingIdChanged);
    _appFrameRate.addListener(this, &ofApp::appFrameRateChanged);
    _bVerticalSyncOn.addListener(this, &ofApp::bVerticalSyncOnChanged);
    _bEnableSmoothing.addListener(this, &ofApp::bEnableSmoothingChanged);
    _bLogVerbose.addListener(this, &ofApp::bLogVerboseChanged);

    _panel.setup(_params);
    _panel.setBackgroundColor(ofColor(50,50,50));
    _panel.setPosition(200, 200);
    _panel.loadFromFile(ofToDataPath(kSettingsFileName));
}