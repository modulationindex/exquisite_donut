#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "sprinkle.h"
#include "donut_cop.h"

#include "LineSet.h"
#include "ofxTweenzor.h"
#include "ofxGui.h"
#include "ofxXmlSettings.h"

const std::string kSettingsFileName = "carlson_settings.xml";

class ofApp : public ofBaseApp{

  public:

    // Generic OF functions
    void setup();
    void update();
    void draw();
    
    // OF Events
    void windowResized(int w, int h);
    void keyPressed(int key);
    
    DonutCop donutCop;
    
    // Sprinkle functions
    std::vector<Sprinkle> sprinkles;  // The vector of sprinkles
    void createSprinkles();
    void removeSprinkles();
    void drawShaderRect(float x, float y, float w, float h);
    void advanceSet();
    void eatSprinkles();
    void setupGUI();
    
    void drawingIdChanged(int & newId);
    void appFrameRateChanged(int & newRate);
    void bVerticalSyncOnChanged(bool & newV);
    void bEnableSmoothingChanged(bool & newV);
    void bLogVerboseChanged(bool & newV);

    
protected:
    vector<LineSet> _lineSets;
    int _currentLineSet;
    float _lastSwitchTimeMillis;
    
    ofFbo _lineTexture;
    ofFbo _particleTexture;
    ofShader _shader;
   
    
    ofParameter<int> _drawingId;
    ofParameter<float> _sceneDurationSec;
    ofParameter<int> _eatSprinklesOverSize;
    ofParameter<bool> _bVerticalSyncOn;
    ofParameter<bool> _bEnableSmoothing;
    ofParameter<bool> _bLogVerbose;

    ofParameter<int> _appFrameRate;
    ofParameterGroup _params;
    ofxPanel _panel;
    
    ofxXmlSettings _settings;
    
    bool _bShowFramerate;
    bool _bShowGui;
};
