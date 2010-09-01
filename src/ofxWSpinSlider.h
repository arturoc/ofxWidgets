/*
 * ofxWFineSlider.h
 *
 *  Created on: 23/12/2009
 *      Author: arturo
 */

#ifndef OFXWSPINSLIDER_H_
#define OFXWSPINSLIDER_H_

#include "ofxWSlider.h"
#include "ofxWArrowButton.h"

class ofxWSpinSlider: public ofxWSlider {
public:
	ofxWSpinSlider(const string & name);
	virtual ~ofxWSpinSlider();

	void init(const string & _title, float * value, float _min, float _max, float _step, const string & style);
	void init(const string & _title, int * value, int _min, int _max, int _step, const string & style);
	void init(const string & _title, float _value, float _min, float _max, float _step, const string & style);
	void init(const string & _title, int _value, int _min, int _max, int _step, const string & style);

	virtual void enable();
	virtual void disable();
	virtual void setVisible(bool visible);

protected:
	void render(ofxWidgetsStyle & style);

	void upPressed(bool & value);
	void downPressed(bool & value);

	virtual ofxWidgetsState manageEvent(ofxWidgetsEvent event, ofxWidgetEventArgs & args, ofxWidgetsState currentState);

	ofRectangle getActiveArea(ofxWidgetsStyle & style);

private:
	ofxWArrowButton down,up;
	float step;
};

#endif /* OFXWSPINSLIDER_H_ */
