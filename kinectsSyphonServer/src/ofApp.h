#pragma once

#include "ofMain.h"
#include "ofxGui.h"

#include "ofxOpenCv.h"
#include "ofxCv.h"

#include "ofxKinect.h"
#include "ofxKinectV2.h"

#include "ofxSyphon.h"

using namespace cv;
using namespace ofxCv;

class ofApp : public ofBaseApp{
	
public:
	void setup();
	void update();
	void draw();
	void exit();

	ofxKinect 				kinect1;
	ofxKinectV2				kinect2;
	
	ofParameter<bool> 		bThreshWithOpenCV;
	//	bool 				bDrawPointCloud;
	
	ofParameter<int>  		k1Angle;
	ofParameter<int>  		k2Angle;
	
	ofxCvColorImage 		colorImg;
	ofxCvGrayscaleImage 	k1GrayImage; // grayscale depth image
	ofxCvGrayscaleImage 	k1GrayImageThreshNear; // the near thresholded image
	ofxCvGrayscaleImage 	k1GrayImageThreshFar; // the far thresholded image
	
	
	ofxCvGrayscaleImage 	k2GrayImage; // grayscale depth image
	ofxCvGrayscaleImage 	k2GrayImageThreshNear; // the near thresholded image
	ofxCvGrayscaleImage 	k2GrayImageThreshFar; // the far thresholded image
	
//	ofxCvContourFinder 		k2ContourFinder;
	
	
	ofxPanel			gui;

	ofParameter<int>	k1GrayThreshNear;
	ofParameter<int>	k1GrayThreshFar;
	
	ofParameter<int>	k2Near;
	ofParameter<int>	k2Far;
	ofParameter<int>	k2GrayThreshNear;
	ofParameter<int>	k2GrayThreshFar;
	

	


	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	
};
