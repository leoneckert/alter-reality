
#include "ofApp.h"

int main()
{
//    int wi = 800;
//    int he = 600;
    int wi = 620;
    int he = 480;

//    int wi = 640;
//    int he = 360;
    
//    int wi = 1280;
//    int he = 960;
    ofSetupOpenGL(wi * 2, he * 2, OF_WINDOW);
    ofRunApp(std::make_shared<ofApp>());
}