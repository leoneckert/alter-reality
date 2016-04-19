
#include "ofApp.h"

int main()
{
//    int wi = 800;
//    int he = 600;
    int wi = 620;
    int he = 480;
    ofSetupOpenGL(wi * 2, he * 2, OF_WINDOW);
    ofRunApp(std::make_shared<ofApp>());
}