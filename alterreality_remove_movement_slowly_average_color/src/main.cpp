
#include "ofApp.h"

int main()
{

    int wi = 900;
    int he = 1100;

    ofSetupOpenGL(wi, he, OF_WINDOW);
    ofRunApp(std::make_shared<ofApp>());
}