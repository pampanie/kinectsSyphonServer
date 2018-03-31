#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetLogLevel(OF_LOG_VERBOSE);
	ofSetFrameRate(30);
	ofSetVerticalSync(false);
	
	
	// init kinect kinect2 .............................................
	kinect1.setRegistration(true);
	//	kinect1.init();
	//kinect.init(true); // shows infrared instead of RGB video image
	kinect1.init(false, false); // disable video image (faster fps)
	
	kinect1.open();		// opens first available kinect
	//kinect.open(1);	// open a kinect by id, starting with 0 (sorted by serial # lexicographically))
	//kinect.open("A00362A08602047A");	// open a kinect using it's unique serial #
	
	// print the intrinsic IR sensor values
	if(kinect1.isConnected()) {
		ofLogNotice() << "sensor-emitter dist: " << kinect1.getSensorEmitterDistance() << "cm";
		ofLogNotice() << "sensor-camera dist:  " << kinect1.getSensorCameraDistance() << "cm";
		ofLogNotice() << "zero plane pixel size: " << kinect1.getZeroPlanePixelSize() << "mm";
		ofLogNotice() << "zero plane dist: " << kinect1.getZeroPlaneDistance() << "mm";
	}
	
	
	//	colorImg.allocate(kinect1.width, kinect1.height);
	k1GrayImage.allocate(kinect1.width, kinect1.height);
	k1GrayImageThreshNear.allocate(kinect1.width, kinect1.height);
	k1GrayImageThreshFar.allocate(kinect1.width, kinect1.height);
	
	
	
	//see how many devices we have.
	ofxKinectV2 tmp;
	vector <ofxKinectV2::KinectDeviceInfo> deviceList = tmp.getDeviceList();
	//	cout << deviceList.size() << endl;
	
	kinect2.open(deviceList[0].serial);
	k2GrayImage.allocate(kinect2.getDepthPixels().getWidth(),kinect2.getDepthPixels().getHeight());
	k2GrayImageThreshNear.allocate(kinect2.getDepthPixels().getWidth(), kinect2.getDepthPixels().getHeight());
	k2GrayImageThreshFar.allocate(kinect2.getDepthPixels().getWidth(), kinect2.getDepthPixels().getHeight());
	
	
	
	
	gui.setup("settings");
	gui.setDefaultBackgroundColor(ofColor(0, 0, 0, 127));
	gui.setDefaultFillColor(ofColor(160, 160, 160, 160));
	
	
	
	// ...............  kinect
	gui.add(bThreshWithOpenCV.set("thresh with opencv",false));
	gui.add(k1GrayThreshNear.set("kinect1 near",0,1,255));
	gui.add(k1GrayThreshFar.set("kinect1 far",0,1,255));
	gui.add(k1Angle.set("kinect1 angle",0,2,90));
	
	gui.add(k2Near.set("kinect2 near",0,100,10000));
	gui.add(k2Far.set("kinect2 far",0,100,6000));
	//	gui.add(k2Angle.set("kinect2 angle",0,2,90));
	gui.add(k2GrayThreshNear.set("kinect2 gray near",0,1,255));
	gui.add(k2GrayThreshFar.set("kinect2 gray far",0,1,255));
	
	
	// seva setting with give name
	if (!ofFile("settings.xml"))
		gui.saveToFile("settings.xml");
	
	gui.loadFromFile("settings.xml");
	
}

//--------------------------------------------------------------
void ofApp::update(){
	kinect1.setCameraTiltAngle(k1Angle.get());
	kinect1.update();
	
	// there is a new frame and we are connected
	if(kinect1.isFrameNew()) {
		
		// load grayscale depth image from the kinect1 source
		k1GrayImage.setFromPixels(kinect1.getDepthPixels());
		
		// we do two thresholds - one for the far plane and one for the near plane
		// we then do a cvAnd to get the pixels which are a union of the two thresholds
		if(bThreshWithOpenCV) {
			k1GrayImageThreshNear = k1GrayImage;
			k1GrayImageThreshFar = k1GrayImage;
			k1GrayImageThreshNear.threshold(k1GrayThreshNear.get(), true);
			k1GrayImageThreshFar.threshold(k1GrayThreshFar.get());
			cvAnd(k1GrayImageThreshNear.getCvImage(), k1GrayImageThreshFar.getCvImage(), k1GrayImage.getCvImage(), NULL);
			
		} else {
			
			// or we do it ourselves - show people how they can work with the pixels
			ofPixels & pix = k1GrayImage.getPixels();
			int numPixels = pix.size();
			for(int i = 0; i < numPixels; i++) {
				if(pix[i] < k1GrayThreshNear.get() && pix[i] > k1GrayThreshFar.get()) {
					pix[i] = 255;
				} else {
					pix[i] = 0;
				}
			}
		}
		
		// update the cv images
		k1GrayImage.flagImageChanged();
		
	}
	
	
	
	// kinect2
	kinect2.update();
	if(kinect2.isFrameNew()){
		kinect2.minDistance = k2Near.get();
		kinect2.maxDistance = k2Far.get();
		
		
		// load grayscale depth image from the kinect1 source
		k2GrayImage.setFromPixels(kinect2.getDepthPixels());
		
		// we do two thresholds - one for the far plane and one for the near plane
		// we then do a cvAnd to get the pixels which are a union of the two thresholds
		if(bThreshWithOpenCV) {
			k2GrayImageThreshNear = k2GrayImage;
			k2GrayImageThreshFar = k2GrayImage;
			k2GrayImageThreshNear.threshold(k2GrayThreshNear.get(), true);
			k2GrayImageThreshFar.threshold(k2GrayThreshFar.get());
			cvAnd(k2GrayImageThreshNear.getCvImage(), k2GrayImageThreshFar.getCvImage(), k2GrayImage.getCvImage(), NULL);
			
		} else {
			
			// or we do it ourselves - show people how they can work with the pixels
			ofPixels & pix = k2GrayImage.getPixels();
			int numPixels = pix.size();
			for(int i = 0; i < numPixels; i++) {
				if(pix[i] < k2GrayThreshNear.get() && pix[i] > k2GrayThreshFar.get()) {
					pix[i] = 255;
				} else {
					pix[i] = 0;
				}
			}
		}
		
		// update the cv images
		k2GrayImage.flagImageChanged();
	}
	
}

//--------------------------------------------------------------
void ofApp::draw(){
	k1GrayImage.draw(0,0);
	k2GrayImage.draw(650,0);
	gui.draw();
}

//--------------------------------------------------------------
void ofApp::exit(){
	kinect1.setCameraTiltAngle(0); // zero the tilt on exit
	kinect1.close();
	
	kinect2.close();
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
