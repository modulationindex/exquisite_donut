//
//  LineSet.h
//  donutDemo
//
//  Created by Chris Carlson on 9/18/16.
//
//

#ifndef __donutDemo__LineSet__
#define __donutDemo__LineSet__

#include <stdio.h>
#include "sprinkle.h"
#include "ofMain.h"



class LineSet{
public:
    LineSet(int numLines = 20, bool bOpener = false);
    void draw();
    void update(vector<Sprinkle> & sprinkles);
    void show();
    void hide();
    void toggleOpener();
    bool isOpener();
    
protected:
    vector<float> _xCoords;
    vector<float> _targetThickness;
    vector<float> _thickness;
    vector<bool> _frameTriggered;
    
    int _xSpacing;
    int _frameCounter;
    bool _bIsHiding;
    bool _bOpener;
    float _lastFrameTime;
    float _leftThresh;
    float _rightThresh;
    float _alpha;
    float _width;
    float _thicknessFactor;

};

#endif /* defined(__donutDemo__LineSet__) */
