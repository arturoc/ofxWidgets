#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){
	ofSetVerticalSync(true);
	length_float = 5;
	length_int = 5;
	toggle = false;
	button = false;

	//ofSetLogLevel(OF_LOG_VERBOSE);
	//optional position and size
	//if not set it gets from xml and resize when adding controls
	gui.init(50,200,50,200);

	//controls not linked with any var
	gui.addSlider("float", (float)5.0, 0.0,10.0,"FLOAT_SLIDER");
	gui.addSlider("int", (int)5, 0,10,"INT_SLIDER");
	gui.addButton("button","BUTTON");
	gui.addToggle("toggle","TOGGLE");

	//controls linked with a var
	gui.addSlider("length float", &length_float, 0.0,10.0,"FLOAT_LENGTH");
	gui.addSlider("length int", &length_int, 0,10,"INT_LENGTH");
	gui.addSpinSlider("length int", &length_float, 0.0,10.0,0.1,"INT_FINE_LENGTH");
	gui.addSpinSlider("length int", &length_int, 0,10,1,"INT_FINE_LENGTH");
	gui.addButton("button",&button,"BUTTON_VAR");
	gui.addToggle("toggle",&toggle,"TOGGLE_VAR");

	//grouped toggles
	option1=true; option2=0; option3=0;
	gui.addGroupedToggle("option 1",&option1,"group1");
	gui.addGroupedToggle("option 2",&option2,"group1");
	gui.addGroupedToggle("option 3","group1");

	gui.addTextBox("text","hola","TEXT_BOX");

	//controls with listener
	ofxWSlider & slider = gui.addSlider("float event", (float)5.0, 0.0,10.0,"FLOAT_EVENT");
	ofAddListener(slider.floatEvent,this,&testApp::onSliderChange);

	//gui.addSaveButton("values.xml","test_values");
	//gui.addLoadButton("values.xml","test_values");
}

void testApp::onSliderChange(float & value){
	event_value = value;
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
	//querying the gui for values
	if(gui.getValueB("BUTTON")) ofSetColor(0,0,0);
	else ofSetColor(255,255,255);
	ofRect(10,60,gui.getValueF("FLOAT_SLIDER",5)*100.0,10);
	if(gui.getValueB("TOGGLE")) ofSetColor(0,0,0);
	else ofSetColor(255,255,255);
	ofRect(10,80,gui.getValueI("INT_SLIDER",5)*100,10);


	//directly modify variables
	if(button) ofSetColor(0,0,0);
	else ofSetColor(255,255,255);
	ofRect(10,110,length_float*100.0,10);
	if(toggle) ofSetColor(0,0,0);
	else ofSetColor(255,255,255);
	ofRect(10,130,length_int*100,10);

	ofSetColor(0,0,0);
	ofDrawBitmapString("event value: " + ofToString(event_value), 10,160);
	ofDrawBitmapString("text box: " + gui.getValueS("TEXT_BOX"), 10,180);
}


//--------------------------------------------------------------
void testApp::keyPressed  (int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

