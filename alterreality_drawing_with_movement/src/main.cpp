
#include "ofApp.h"

int main()
{
    ofSetupOpenGL(640, 480 * 2, OF_WINDOW);
    ofRunApp(std::make_shared<ofApp>());
}