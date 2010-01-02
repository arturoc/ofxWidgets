/*
 * ofxYAGuiControl.cpp
 *
 *  Created on: 19-feb-2009
 *      Author: arturo castro
 */

#include "ofxWidget.h"
#include "ofxWControlPanel.h"


#ifdef OFXWIDGETS_USING_TUIO
ofxTuioClient * ofxWidget::tuioClient = NULL;
#endif

ofxWidget::ofxWidget(const string & name) {
	//
	this->name = name;

	if(dynamic_cast<ofxWControlPanel*>(this)==NULL){
		//ofxWControlPanel::getControlPanel()->registerControl(this);
		enable();
	}
	setVisible(true);
	styleLoader = &(ofxWStyleLoader::getLoader());
	state		= OFX_WIDGET_UNFOCUSED;
}

ofxWidget::~ofxWidget() {

}
