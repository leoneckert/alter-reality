
#include "ofApp.h"

int main()
{

    int wi = 960;
    int he = 1080;
    

    ofSetupOpenGL(wi, he, OF_WINDOW);
    ofRunApp(std::make_shared<ofApp>());
}