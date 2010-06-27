/*
 * ofxStyleLoader.h
 *
 *  Created on: 23-feb-2009
 *      Author: arturo castro
 */

#pragma once

#include "ofxXmlSettings.h"
#include "ofxWTypes.h"

class ofxWStyleLoader{
public:

	static ofxWStyleLoader & getLoader(){
		static ofxWStyleLoader instance;
		return instance;
	}

	ofxWidgetsStyle getStyle(string controlType, string state, string style="default");
	ofxWFrameStyle getFrameStyle(string state, string style="default");

private:
	ofxXmlSettings xmlStyles;

	ofxWStyleLoader();

};
