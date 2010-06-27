/*
 * ofxWCurve.h
 *
 *  Created on: 08/01/2010
 *      Author: arturo
 */

#ifndef OFXWCURVE_H_
#define OFXWCURVE_H_

#include "ofxWidget.h"
#include "ofxWCircleButton.h"

class ofxWCurve: public ofxWButton {
public:
	ofxWCurve()
		:ofxWButton("curve")
	{
		activeButton = 0;
		mouseOverButton = 0;
	}
	virtual ~ofxWCurve(){

	}

	void resetZoom(bool & pressed){
		if(!pressed){
			setZoom(min_x,max_x);
		}
	}

	void moveZoom(float & init_zoom){
		float _zoom_max = zoom_max - zoom_min + init_zoom;
		setZoom(init_zoom,_zoom_max);
	}

	void showMarkers(bool & pressed){
		for(unsigned i=0; i<buttons.size(); i++){
			buttons[i]->setVisible(pressed);
		}
	}

	void init(ofRectangle origin, float _min_x=0, float _max_x=1, float _min_y=-1, float _max_y=1){
		zoom_min = min_x = _min_x;
		zoom_max = max_x = _max_x;
		min_y = _min_y;
		max_y = _max_y;

		ofxWButton::init("");
		this->area = origin;

		points.push_back(ofPoint(0,ofMap(0,min_y,max_y,1,0))); //area.x,area.y+ofMap(0,min_y,max_y,area.height,0))
		buttons.push_back( new ofxWCircleButton("button_"+ofToString((int)buttons.size()-1)));
		buttons[0]->init("");
		buttons[0]->setPosition(ofPoint(area.x,area.y+ofMap(0,min_y,max_y,area.height,0)));
		ofAddListener(buttons[0]->focusedEvent,this,&ofxWCurve::mouseOver);
		ofAddListener(buttons[0]->boolEvent,this,&ofxWCurve::buttonPressed);

		points.push_back(ofPoint(1,ofMap(0,min_y,max_y,1,0)));
		buttons.push_back(new ofxWCircleButton("button_"+ofToString((int)buttons.size()-1)));
		buttons[1]->init("");
		buttons[1]->setPosition(ofPoint(area.x+area.width,area.y+ofMap(0,min_y,max_y,area.height,0)));
		ofAddListener(buttons[1]->focusedEvent,this,&ofxWCurve::mouseOver);
		ofAddListener(buttons[1]->boolEvent,this,&ofxWCurve::buttonPressed);
		//area.x+area.width,area.y+ofMap(0,min_y,max_y,area.height,0)));

		controls.init(area.x,area.y+area.height,200,55,"","",false);

		add_points = true;
		zoom = false;
		select_area = false;
		show_markers = true;
		controls.addGroupedToggle("add points",&add_points,"curve_controls");
		controls.addGroupedToggle("zoom",&zoom,"curve_controls");
		controls.addGroupedToggle("select to area",&select_area,"curve_controls");
		ofAddListener(controls.addButton("reset zoom").boolEvent,this,&ofxWCurve::resetZoom);
		ofAddListener(controls.addSpinSlider("move x",min_x,min_x,max_x).floatEvent,this,&ofxWCurve::moveZoom);
		ofAddListener(controls.addToggle("show markers",&show_markers).boolEvent,this,&ofxWCurve::showMarkers);
	}

	virtual void render(ofxWidgetsStyle & style){
		ofPushStyle();
		ofNoFill();
		ofSetColor(style.border.color.r,style.border.color.g,style.border.color.b);

		ofRect(area.x,area.y,area.width,area.height);
		ofBeginShape();
		bool lerp_next=false;
		for(unsigned i=0;i<points.size(); i++){
			if(ofMap(points[i].x,0,1,min_x,max_x)>=zoom_min && ofMap(points[i].x,0,1,min_x,max_x)<=zoom_max){
				if(lerp_next){
					ofVertex(area.x,
							 ofLerp(points[i-1].y,points[i].y,
									 ofMap(min_x,zoom_min,zoom_max,0,1)
									 - ofMap(ofMap(points[i-1].x,0,1,min_x,max_x),zoom_min,zoom_max,0,1)
									)
							* area.height + area.y);
					lerp_next=false;
				}
				ofVertex(area.x + ofMap(ofMap(points[i].x,0,1,min_x,max_x),zoom_min,zoom_max,0,1)*area.width,
						 area.y + points[i].y*area.height);
			}else{
				lerp_next=true;
			}
		}
		ofEndShape(false);

		for(float i=zoom_min; i<=zoom_max; i+=(zoom_max-zoom_min)/10.0){
			float x=ofMap(i,zoom_min,zoom_max,area.x,area.width+area.x);
			float y=area.y+area.height;
			float textSize = style.text.font->getStringBoundingBox(ofToString(i,2),0,0).width;
			style.text.font->drawString(ofToString(i,2),x-textSize*0.5,y+style.text.font->getLineHeight()*1.5);
			ofLine(x,y,x,y+style.text.font->getLineHeight());
		}

		ofLine(area.x,area.y+ofMap(0,min_y,max_y,area.height,0),area.x+area.width,area.y+ofMap(0,min_y,max_y,area.height,0));

		ofSetColor(style.border.color.r,style.border.color.g,style.border.color.b);
		if(mouseOverButton){
			if(style.text.ttf){
				style.text.font->drawString(ofToString(ofMap((mouseOverButton->getControlPosition().x-area.x)/area.width,0,1,zoom_min,zoom_max))+
										","+ofToString(ofMap(((mouseOverButton->getControlPosition().y-area.y)/area.height*2-1)*-1,-1,1,min_y,max_y)),
												mouseOverButton->getControlPosition().x + style.text.position.x,
												mouseOverButton->getControlPosition().y +style.text.position.y);
			}else{
				ofDrawBitmapString(ofToString((mouseOverButton->getControlPosition().x-area.x)/area.width)+
						","+ofToString(((mouseOverButton->getControlPosition().y-area.y)/area.height*2-1)*-1)
								,mouseOverButton->getControlPosition().x + style.text.position.x,
								mouseOverButton->getControlPosition().y + style.text.position.y);
			}
		}
		if(style.text.ttf){
			//style.text.font->drawString(ofToString(zoom_min),area.x,area.y+area.height/2-style.text.font->getLineHeight()/2);
			//style.text.font->drawString(ofToString(zoom_max),area.x+area.width+style.text.font->getLineHeight()/2,area.y+area.height/2-style.text.font->getLineHeight()/2);
			style.text.font->drawString(ofToString(max_y),area.x+area.width+style.text.font->getLineHeight()/2,area.y);
			style.text.font->drawString(ofToString(min_y),area.x+area.width+style.text.font->getLineHeight()/2,area.y+area.height);
		}else{
			ofDrawBitmapString(ofToString(min_x),area.x,area.y+area.height/2);
		}

		ofPopStyle();
	}



	vector<ofPoint> getNormalizedPoints(){
		vector<ofPoint> normPoints;
		for(unsigned i=0;i<points.size();i++){
			ofPoint normPoint(ofMap(points[i].x,0,1,min_x,max_x),ofMap(points[i].y,1,0,min_y,max_y));
			normPoints.push_back(normPoint);
		}
		return normPoints;
	}

	void setZoom(float _zoom_min, float _zoom_max){
		zoom_min=_zoom_min;
		zoom_max=_zoom_max;

		for(unsigned i=0;i<buttons.size();i++){
			buttons[i]->setPosition(ofPoint(area.x + ofMap(ofMap(points[i].x,0,1,min_x,max_x),zoom_min,zoom_max,0,1)*area.width,
					area.y + points[i].y*area.height));
		}
	}

	vector<ofPoint> points;
	ofEvent<bool> 	pointsChanged;
protected:

	virtual ofRectangle getActiveArea(ofxWidgetsStyle & style){
		return area;
	}

	virtual ofRectangle getTotalArea(ofxWidgetsStyle & style){
		return area;
	}

	void update(){
		ofxWButton::update();

		if(activeButton){
			for(unsigned i=0; i<buttons.size(); i++){
				if(buttons[i]==activeButton){
					ofPoint pos = mousePos;
					if(i==0 || i==buttons.size()-1){
						pos.x = points[i].x * area.width + area.x;
					}else{
						if(buttons[i-1]->getControlPosition().x>pos.x)
							pos.x=buttons[i-1]->getControlPosition().x;
						else if(buttons[i+1]->getControlPosition().x<pos.x)
							pos.x=buttons[i+1]->getControlPosition().x;
					}
					if(pos.y>area.y+area.height)pos.y=area.y+area.height;
					if(pos.y<area.y)pos.y=area.y;
					buttons[i]->setPosition(pos);
					points[i] = ofPoint(ofMap(ofMap(pos.x,area.x,area.x+area.width,zoom_min,zoom_max),min_x,max_x,0,1),
							(pos.y-area.y)/area.height);

					bool changed = true;
					ofNotifyEvent(pointsChanged,changed);
					break;
				}
			}
		}

	}

	void buttonPressed(const void * sender, bool & pressed){
		if(pressed){
			activeButton = (ofxWCircleButton*)sender;
			if(!show_markers) ((ofxWCircleButton*)sender)->setVisible(true);
		}else{
			activeButton = 0;
			if(!show_markers) ((ofxWCircleButton*)sender)->setVisible(false);
		}
	}

	void mouseOver(const void * sender, bool & focused){
		if(focused){
			mouseOverButton = (ofxWCircleButton*)sender;
		}else{
			mouseOverButton = 0;
		}
	}


	virtual ofxWidgetsState manageEvent(ofxWidgetsEvent event, ofxWidgetEventArgs & args, ofxWidgetsState currentState){

		prevMousePos = mousePos;
		mousePos.x = mouse.x;
		mousePos.y = mouse.y;

		if(event==OFX_W_E_POINTER_PRESSED){
			bool insideButton = false;
			for(unsigned i=0; i<buttons.size(); i++){
				ofRectangle buttonArea = buttons[i]->getControlActiveArea();
				buttonArea.x += buttons[i]->getControlPosition().x;
				buttonArea.y += buttons[i]->getControlPosition().y;
				if(mousePos.x>buttonArea.x && mousePos.x<buttonArea.x+buttonArea.width &&
					mousePos.y>buttonArea.y && mousePos.y<buttonArea.y+buttonArea.height){
					insideButton = true; break;
				}
			}
			if(!insideButton){
				if(add_points){
					activeButton = addPoint(mousePos);
				}else if(zoom){
					float zoom_center = ofMap((mousePos.x-area.x)/area.width,0,1,zoom_min,zoom_max);
					float _zoom_min = zoom_center - (zoom_max-zoom_min)/4.0f;
					float _zoom_max = zoom_center + (zoom_max-zoom_min)/4.0f;
					setZoom(_zoom_min,_zoom_max);
				}
			}
		}else if(event==OFX_W_E_POINTER_RELEASED){
			if(activeButton && !show_markers) activeButton->setVisible(false);
			activeButton=0;
			bool changed = true;
			ofNotifyEvent(pointsChanged,changed);
		}

		return ofxWButton::manageEvent(event,args,currentState);
	}



private:

	ofxWCircleButton* addPoint(ofPoint pos){
		unsigned i;
		for(i=0; i<buttons.size(); i++){
			if(buttons[i]->getControlPosition().x>pos.x) break;
		}
		buttons.insert(buttons.begin()+i, new ofxWCircleButton("button_"+ofToString((int)buttons.size()-1)));
		buttons[i]->init("");
		buttons[i]->setPosition(pos);
		ofAddListener(buttons[i]->boolEvent,this,&ofxWCurve::buttonPressed);
		ofAddListener(buttons[i]->focusedEvent,this,&ofxWCurve::mouseOver);
		points.insert(points.begin()+i,ofPoint(ofMap(ofMap(pos.x,area.x,area.x+area.width,zoom_min,zoom_max),min_x,max_x,0,1),
				(pos.y-area.y)/area.height));
		return buttons[i];
	}

	vector<ofxWCircleButton *> buttons;
	ofxWCircleButton * activeButton;
	ofxWCircleButton * mouseOverButton;
	ofPoint mousePos;
	ofPoint prevMousePos;
	ofRectangle area;
	ofxWFrame controls;

	float min_x, max_x, min_y, max_y;
	float zoom_min, zoom_max;

	bool add_points, zoom, select_area, show_markers;
};

#endif /* OFXWQUADWARP_H_ */
