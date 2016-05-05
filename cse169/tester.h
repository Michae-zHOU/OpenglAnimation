////////////////////////////////////////
// tester.h
////////////////////////////////////////

#ifndef CSE169_TESTER_H
#define CSE169_TESTER_H

#include "core.h"
#include "camera.h"
#include "cube.h"
#include "token.h"
#include "Skeleton.h"
#include "Skin.h"
#include "Light.h"
#include "Vector4.h"
#include "Rig.h"
#include "AnimationPlayer.h"
#include <GLUI/glui.h>
#include <stdio.h>
#include <string.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
////////////////////////////////////////////////////////////////////////////////

class Tester {
public:
	Tester(int argc,char **argv);
	~Tester();

	void Update();
	void Reset();
	void Draw();
    
    //Windows for Curve
    void CurveDraw();
    void Reshape(int x,int y);
    void ClearBuffer();
    void drawPoint(int x, int y, float r, float g, float b);
    
	void Quit();

	// Event handlers
	void Resize(int x,int y);
	void Keyboard(int key,int x,int y);
	void MouseButton(int btn,int state,int x,int y);
	void MouseMotion(int x,int y);
    
    void Controll(int,int,int,int,int,int,int);

private:
	// Window management
	int WindowHandle;
	int WinX,WinY;
    
    int WinX2,WinY2;
    
    int SecondWindowHandle;


	// Input
	bool LeftDown,MiddleDown,RightDown;
	int MouseX,MouseY;

	// Components
	Camera Cam;
	SpinningCube Cube;
};

////////////////////////////////////////////////////////////////////////////////

/*
The 'Tester' is a simple top level application class. It creates and manages a
window with the GLUT extension to OpenGL and it maintains a simple 3D scene
including a camera and some other components.
*/

#endif
