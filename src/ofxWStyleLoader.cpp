/*
 * ofxWStyleLoader.cpp
 *
 *  Created on: 27/06/2010
 *      Author: arturo
 */

#include "ofxWStyleLoader.h"


ofxWidgetsStyle ofxWStyleLoader::getStyle(string controlType, string state, string style){
	ofxWidgetsStyle controlStyle;
	controlStyle.background.color.r=xmlStyles.getValue(style+":"+controlType+":"+state+":background:color:r",255);
	controlStyle.background.color.g=xmlStyles.getValue(style+":"+controlType+":"+state+":background:color:g",255);
	controlStyle.background.color.b=xmlStyles.getValue(style+":"+controlType+":"+state+":background:color:b",255);
	controlStyle.background.color.a=xmlStyles.getValue(style+":"+controlType+":"+state+":background:color:a",255);
	controlStyle.background.width=xmlStyles.getValue(style+":"+controlType+":"+state+":background:width",200);
	controlStyle.background.height=xmlStyles.getValue(style+":"+controlType+":"+state+":background:height",20);
	controlStyle.background.lineWidth=xmlStyles.getValue(style+":"+controlType+":"+state+":background:lineWidth",0);


	controlStyle.foreground.color.r=xmlStyles.getValue(style+":"+controlType+":"+state+":foreground:color:r",255);
	controlStyle.foreground.color.g=xmlStyles.getValue(style+":"+controlType+":"+state+":foreground:color:g",255);
	controlStyle.foreground.color.b=xmlStyles.getValue(style+":"+controlType+":"+state+":foreground:color:b",255);
	controlStyle.foreground.width=xmlStyles.getValue(style+":"+controlType+":"+state+":foreground:width",198);
	controlStyle.foreground.height=xmlStyles.getValue(style+":"+controlType+":"+state+":foreground:height",18);
	controlStyle.foreground.lineWidth=xmlStyles.getValue(style+":"+controlType+":"+state+":foreground:lineWidth",0);

	controlStyle.border.color.r=xmlStyles.getValue(style+":"+controlType+":"+state+":border:color:r",0);
	controlStyle.border.color.g=xmlStyles.getValue(style+":"+controlType+":"+state+":border:color:g",0);
	controlStyle.border.color.b=xmlStyles.getValue(style+":"+controlType+":"+state+":border:color:b",0);
	controlStyle.border.width=xmlStyles.getValue(style+":"+controlType+":"+state+":border:width",200);
	controlStyle.border.height=xmlStyles.getValue(style+":"+controlType+":"+state+":border:height",20);
	controlStyle.border.lineWidth=xmlStyles.getValue(style+":"+controlType+":"+state+":border:lineWidth",2);

	controlStyle.position.x=xmlStyles.getValue(style+":"+controlType+":"+state+":position:x",30);
	controlStyle.position.y=xmlStyles.getValue(style+":"+controlType+":"+state+":position:y",30);

	controlStyle.text.ttf=true;//xmlStyles.getValue(style+":"+controlType+":"+state+":text:ttf",true);
	if(controlStyle.text.ttf){
		controlStyle.text.font = new ofTrueTypeFont;
		controlStyle.text.font->loadFont(xmlStyles.getValue(style+":"+controlType+":"+state+":text:font","helvetica.ttf"),8);
	}
	controlStyle.text.color.r=xmlStyles.getValue(style+":"+controlType+":"+state+":text:color:r",0);
	controlStyle.text.color.g=xmlStyles.getValue(style+":"+controlType+":"+state+":text:color:g",0);
	controlStyle.text.color.b=xmlStyles.getValue(style+":"+controlType+":"+state+":text:color:b",0);
	controlStyle.text.position.x=xmlStyles.getValue(style+":"+controlType+":"+state+":text:position:x",24);
	controlStyle.text.position.y=xmlStyles.getValue(style+":"+controlType+":"+state+":text:position:y",14);

	return controlStyle;
}

ofxWFrameStyle ofxWStyleLoader::getFrameStyle(string state, string style){
	ofxWFrameStyle frameStyle = (ofxWFrameStyle)getStyle("frame",state,style);
	frameStyle.width		= xmlStyles.getValue(style+":frame:"+state+":width",200);
	frameStyle.height		= xmlStyles.getValue(style+":frame:"+state+":height",200);
	frameStyle.hSpacing		= xmlStyles.getValue(style+":frame:"+state+":hSpacing",20);
	frameStyle.vSpacing		= xmlStyles.getValue(style+":frame:"+state+":vSpacing",10);
	frameStyle.growOnHeight = xmlStyles.getValue(style+":frame:"+state+":grow","height")=="height"?true:false;
	frameStyle.decoration_h = xmlStyles.getValue(style+":frame:"+state+":decoration_height",20);
	frameStyle.interFrameVSpace = xmlStyles.getValue(style+":frame:"+state+":interFrameVSpace",10);
	frameStyle.interFrameHSpace = xmlStyles.getValue(style+":frame:"+state+":interFrameHSpace",10);
	return frameStyle;
}

ofxWStyleLoader::ofxWStyleLoader(){
	xmlStyles.loadFile("default.xml");
	xmlStyles.setVerbose(true);
}
