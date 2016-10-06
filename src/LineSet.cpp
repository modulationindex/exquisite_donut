//
//  LineSet.cpp
//  donutDemo
//
//  Created by Chris Carlson on 9/18/16.
//
//

#include "LineSet.h"
#include "ofxTweenzor.h"



LineSet::LineSet(int numLines, bool bOpener){
    
    this->_bOpener = bOpener;
    _xSpacing = 1080 / (numLines);
    for (int i = 0; i < numLines ; i++){
        int xV = _xSpacing * (i);
        if (xV <= 0 || xV >= 1080){ continue;}
        _xCoords.push_back(xV);
        _targetThickness.push_back(2.);
        _thickness.push_back(2.);
        _frameTriggered.push_back(false);
        
    }
    _thicknessFactor = 50. / numLines;
    
    if (!_bOpener){
        _thicknessFactor = 50./numLines;
    }
    _width = 100.;
    _alpha = 0.;
    _lastFrameTime = 0.001 * ofGetElapsedTimeMillis();
    _leftThresh = MIN(5./ (float)_xSpacing,0.15);
    _rightThresh = 1. - _leftThresh;
}

void LineSet::draw(){
    
    if (_alpha > 0.0001f){
        ofSetColor(255, 255,255,_alpha * 255.);
        for (int i = 0; i < _xCoords.size(); i++){
            ofSetLineWidth(2);
            float thickI = _thickness[i];
            ofDrawRectangle(_xCoords[i] - 0.5 * thickI,0.f,thickI,1920.);
        }
    }
    
}


bool LineSet::isOpener(){
    return _bOpener;
}

void LineSet::show(){
    _frameCounter = 0;
    if (_bOpener){
        Tweenzor::add(&_alpha,_alpha,1.,0.f,1.f,EASE_OUT_CUBIC);
        
    }else{
        Tweenzor::add(&_alpha,_alpha,1.,0.f,2.f,EASE_OUT_CUBIC);
        
        
    }
    for (int i = 0; i < _targetThickness.size(); i++){
        (_bOpener) ? _targetThickness[i] = _xSpacing : _targetThickness[i] = 1;
    }
}

void LineSet::hide(){
    
    for (int i = 0; i < _targetThickness.size(); i++){
        _targetThickness[i] = 1;
    }
    if (_bOpener){
        Tweenzor::add(&_alpha,_alpha,0.,0.f,1.f,EASE_IN_CUBIC);
        
    }else{
        Tweenzor::add(&_alpha,_alpha,0.,2.f,2.f,EASE_IN_CUBIC);
        
        
    }
    
}

void LineSet::toggleOpener(){
    _bOpener = !_bOpener;
}

void LineSet::update(vector<Sprinkle> & sprinkles){
    _frameCounter += 1;
    int screenW = 1080;
    float invXSpacing = 1./_xSpacing;
    int numCoords = _xCoords.size();
    float timeSec = 0.001 * ofGetElapsedTimeMillis();
    float frameInterval = timeSec - _lastFrameTime;
    float targetFrameRate = ofGetTargetFrameRate();
    
    std::fill(_frameTriggered.begin(), _frameTriggered.end(), false);
    
    
    if (_alpha > 0.){
        //use even spacing of lines to our advantage. calculate nearest line and determine if needs triggering.
        for (auto & sp : sprinkles){
            if (sp.didEat() || sp.isEating()) continue;
            float screenX = sp.getX()*screenW;
            
            if (screenX <=0 || screenX >= screenW){ continue; }
            
            float div = screenX * invXSpacing;
            int leftIndex = (int)div;
            float fraction = (float)leftIndex + 1. - div;
            leftIndex -= 1;
            
            if (fraction < _rightThresh){
                if (fraction < _leftThresh && leftIndex >= 0){
                    if (!_frameTriggered[leftIndex]){
                        _frameTriggered[leftIndex] = true;
                        (_bOpener) ? _targetThickness.at(leftIndex) = 10 : _targetThickness.at(leftIndex) += 10. * _xSpacing * targetFrameRate * frameInterval ;
                    }
                }
            }else{
                int rightIndex = leftIndex + 1;
                if (rightIndex < numCoords){
                    if (!_frameTriggered[rightIndex]){
                        _frameTriggered[rightIndex] = true;
                        (_bOpener) ? _targetThickness.at(rightIndex) = 10 : _targetThickness.at(rightIndex) += 10. * _xSpacing * targetFrameRate * frameInterval;
                        
                    }
                }
            }
        }
        
        
        float multiplier = (_bOpener) ? 1.1  : 0.9;
        
        //smooth animation
        for (int i = 0 ; i < _xCoords.size(); i++){
            _targetThickness.at(i) *= multiplier;
           
            (_targetThickness.at(i) < 2) ? _targetThickness.at(i) = 2 : _targetThickness.at(i);
            (_targetThickness.at(i) > _xSpacing) ? _targetThickness.at(i) = _xSpacing - 2 : _targetThickness.at(i);
            float leakyValue = 0.15;
            _thickness[i] = (1. - leakyValue) * _thickness[i] + leakyValue * _targetThickness[i];
        }
        
    }
    
    _lastFrameTime = timeSec;
    
}