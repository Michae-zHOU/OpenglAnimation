////////////////////////////////////////
// tester.cpp
////////////////////////////////////////

#include "tester.h"

#define WINDOWTITLE	"Animation"


////////////////////////////////////////////////////////////////////////////////

static Tester *TESTER;
Skeleton* _skel;
Skin* _skin;
string filename;
string filename1;
string str = "/Users/ziyaozhou/Desktop/cse169/";
static Light light;
static Light light1;
int skelVisible = -1;
Joint* curr;
int currJoint = 0;
float DOF1;
GLUI_StaticText* text,*channelNum,*do0,*do1,*do2,*do3,*do4,*do5;

Rig* _rig;
AnimationPlayer* animator;
int mode = 0;

static float* pixels;

std::clock_t start;
float duration;

int chanNum = 0;

void Tester::ClearBuffer()
{
    for (int i=0; i<WinX2*WinY2; ++i)
    {
        pixels[i*3]   = 0;
        pixels[i*3+1] = 0;
        pixels[i*3+2] = 0;
    }  
}

void Tester::drawPoint(int x, int y, float r, float g, float b)
{
    int offset = y*WinX2*3 + x*3;
    pixels[offset]   = r;
    pixels[offset+1] = g;
    pixels[offset+2] = b;
}

void Tester::CurveDraw() {
    ClearBuffer();
    
    glViewport(0, 0, WinX2, WinY2);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    float start = animator->getStart();
    float end = animator->getEnd();
    float span = end - start;
    float step = span/WinX2;
    
    drawPoint(0, WinY2/2, 1, 0, 0);
    
    if(chanNum<0)
        chanNum = 0;
    if(chanNum > animator->anim->numofchannel-1)
        chanNum = animator->anim->numofchannel-1;

    float offset = animator->anim->Evaluate(start, chanNum);
    
    for(int i = 0; i <= WinX2; i++){
        float x = animator->anim->Evaluate(start+step*i, chanNum);
        
        int yOffset = WinY2/2+(int)(100*(x-offset));
            
        if(yOffset >= 0 && yOffset <= WinY2){
            drawPoint(i, yOffset, 1, 0, 0);
        }
    }
    
    for(int i = 0; i<animator->anim->clist[chanNum].keysize; i++ ){
        
        float in = animator->anim->clist[chanNum].KeyFramelist[i].TangentIn;
        float out = animator->anim->clist[chanNum].KeyFramelist[i].TangentOut;
        
        float s = animator->anim->clist[chanNum].KeyFramelist[i].Time-start;
        int place = s/step;
        
        float x = animator->anim->Evaluate(start+step*place, chanNum);
        
        int yOffset = WinY2/2+(int)(100*(x-offset));
        
        if(yOffset >= 0 && yOffset <= WinY2 && place>=0 && place <= WinX2){
            drawPoint(place, yOffset, 0, 1, 0);
            
            float p2 = place;
            for(int k=0; k<40; k++){
                place++;
                p2--;
                
                float x1=x+in*step*k;
                float x2=x+out*step*k;
                
                int y1 = WinY2/2+(int)(100*(x1-offset));
                int y2 = WinY2/2+(int)(100*(x2-offset));
                
                if(y2 >= 0 && y2 <= WinY2 && place>=0 && place <= WinX2 && i != animator->anim->clist[chanNum].keysize-1)
                    drawPoint(place, y2, 0, 0, 1);
                
                if(y1 >= 0 && y1 <= WinY2 && p2>=0 && p2 <= WinX2 && i!=0)
                    drawPoint(p2, y1, 0, 0, 1);
            }
        }
    }
    
    
    glDrawPixels(WinX, WinY, GL_RGB, GL_FLOAT, pixels);
    
    glFinish();
    glutSwapBuffers();
}

void Tester::Reshape(int new_width, int new_height)
{
    WinX2 = new_width;
    WinY2 = new_height;
    delete[] pixels;
    pixels = new float[WinX2 * WinY2 * 3];
}

void control_cb( int ID ){
    if (_skel == NULL) {
        return;
    }
    if (ID == 1) {
        skelVisible *= -1;
    }
    switch (ID) {
        case 2:
            curr->dofs[0].Value++;
            break;
        case 3:
            curr->dofs[0].Value--;
            break;
        case 4:
            curr->dofs[1].Value++;
            break;
        case 5:
            curr->dofs[1].Value--;
            break;
        case 6:
            curr->dofs[2].Value++;
            break;
        case 7:
            curr->dofs[2].Value--;
            break;
        case 8:
            curr->dofs[3].Value++;
            break;
        case 9:
            curr->dofs[3].Value--;
            break;
        case 12:
            curr->dofs[4].Value++;
            break;
        case 13:
            curr->dofs[4].Value--;
            break;
        case 14:
            curr->dofs[5].Value++;
            break;
        case 15:
            curr->dofs[5].Value--;
            break;
        case 16:
            chanNum++;
            break;
        case 17:
            chanNum--;
            break;
    }
    if (ID == 10) {
        currJoint-=1;
        
        if (currJoint < 0)
            currJoint = 0;
        
        if (currJoint > (_skel->jointVec.size() -1))
            currJoint = _skel->jointVec.size() -1;
        
        curr->chosen = false;
        curr = _skel->jointVec.at(currJoint);
        curr->chosen = true;
        
        text->set_text(&(curr->name)[0]);
    }
    if (ID == 11) {
        currJoint+=1;
        
        if (currJoint < 0)
            currJoint = 0;
        
        if (currJoint > (_skel->jointVec.size() -1))
            currJoint = _skel->jointVec.size() -1;
   
        curr->chosen = false;
        curr = _skel->jointVec.at(currJoint);
        curr->chosen = true;
        
        text->set_text(&(curr->name)[0]);
    }
    channelNum->set_text(&("Channel: "+chanNum)[0]);
    do0->set_text(&("1: "+to_string(curr->dofs[0].Value))[0]);
    do1->set_text( &("2: "+to_string(curr->dofs[1].Value))[0]);
    do2->set_text( &("3: "+to_string(curr->dofs[2].Value))[0]);
    do3->set_text( &("4: "+to_string(curr->dofs[3].Value))[0]);
    do4->set_text( &("5: "+to_string(curr->dofs[4].Value))[0]);
    do5->set_text( &("6: "+to_string(curr->dofs[5].Value))[0]);
}

int main(int argc, char **argv) {
    string str1 = str;
    
    string projectName;
    cout<< "Select Project: ";
    getline(cin, projectName);

    if(projectName.compare("project3") == 0){
        mode = 1;
        
        string comm = str;
        cout<<"Enter File Name: ";
        getline(cin, filename);
        
        if(filename.length() <= 0)
            filename = "wasp";
        
        comm.append("/"+filename+"/");
        
        str = comm;
        str.append(filename+".skel");
        
        str1 = comm;
        str1.append(filename+".skin");
   
        cout<<"Enter Animation File Name: ";
        getline(cin, filename);
        
        if(filename.length() <=0)
            filename = "wasp_walk";
        
        comm.append(filename+".anim");
        
        cout << "Loading..."<< endl;
        
        _skel = new Skeleton(&str[0]);
        _skin = new Skin(&str1[0]);
        _skin->setSkeleton(_skel);
        
        _rig = new Rig(_skel,_skin);
        
        animator = new AnimationPlayer();
        
        animator->LoadSkel(_skel);
        animator->LoadAnim(&comm[0]);
        
        if (_skel->exceptionHandler||_skin->exceptionHandler||animator->exceptionHandler) {
            exit(1);
        }
        
        cout<<"Successfully Load Animation."<<endl;
        
        if(_skel != NULL){
            curr = _skel->jointVec.at(currJoint);
            curr->chosen = true;
        }
    }
    else if(projectName.compare("project2") == 0){
        
        cout << "Enter The Skel File: ";
        getline(cin,filename);
        str.append(filename);
    
        cout << "Enter The Skin File: ";
        getline(cin,filename1);
        str1.append(filename1);
    
        cout << "Loading..."<< endl;
    
        if (filename.length() != 0) {
            _skel = new Skeleton(&str[0]);
        
            if (_skel->exceptionHandler) {
                exitHandler = true;
                exit(1);
            }
        
            if (filename1.length() != 0) {
                _skin = new Skin(&str1[0]);
                _skin->setSkeleton(_skel);
            
                if (_skin->exceptionHandler) {
                    exitHandler = true;
                    exit(1);
                }
            }
        }
        else{
            if (filename1.length() != 0) {
                _skin = new Skin(&str1[0]);
                if (_skin->exceptionHandler) {
                    exitHandler = true;
                    exit(1);
                }
            }
            else{
                str.append("wasp.skel");
                _skel = new Skeleton(&str[0]);
                str1.append("wasp.skin");
                _skin = new Skin(&str1[0]);
                _skin->setSkeleton(_skel);
            
                if (_skel->exceptionHandler||_skin->exceptionHandler) {
                    exitHandler = true;
                    exit(1);
                }
            }
        }
        
        if(_skel != NULL){
            curr = _skel->jointVec.at(currJoint);
            curr->chosen = true;
        }
    }
    
    glutInit(&argc, argv);
    
	TESTER = new Tester(argc,argv);
    
	glutMainLoop();
	return 0;
}

////////////////////////////////////////////////////////////////////////////////

// These are really HACKS to make glut call member functions instead of static functions
static void display()									{TESTER->Draw();}
static void idle()										{TESTER->Update();}
static void resize(int x,int y)							{TESTER->Resize(x,y);}
static void keyboard(unsigned char key,int x,int y)		{TESTER->Keyboard(key,x,y);}
static void mousebutton(int btn,int state,int x,int y)	{TESTER->MouseButton(btn,state,x,y);}
static void mousemotion(int x, int y)					{TESTER->MouseMotion(x,y);}

static void curveDisplay()                              {TESTER->CurveDraw();}
static void reshape(int x, int y)                         {TESTER->Reshape(x,y);}
////////////////////////////////////////////////////////////////////////////////

Tester::Tester(int argc,char **argv) {
	WinX=640;
	WinY=480;
	LeftDown=MiddleDown=RightDown=false;
	MouseX=MouseY=0;

	// Create the window
	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
	glutInitWindowSize( WinX, WinY );
	glutInitWindowPosition( 0, 0 );
	WindowHandle = glutCreateWindow( WINDOWTITLE );
	glutSetWindowTitle( WINDOWTITLE );
	glutSetWindow( WindowHandle );
    
	// Background color
	glClearColor( 0., 0., 0., 1. );

    glEnable(GL_DEPTH_TEST);                                    //Enable depth buffering
    glClear(GL_DEPTH_BUFFER_BIT);                               //Clear depth buffer
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);                //Set polygon drawing mode to fill front and back of each polygon
    //glDisable(GL_CULL_FACE);                                    //Disable backface culling to render both sides of polygons
    glShadeModel(GL_SMOOTH);                                    //Set shading to smooth
    glEnable(GL_CULL_FACE);
    
    glEnable(GL_COLOR_MATERIAL);                                //Enable color materials
    glEnable(GL_LIGHTING);                                      //Enable lighting
    glEnable(GL_NORMALIZE);                                     //Auto normalize surface normals
    glEnable(GL_TEXTURE_2D);                                    //Enable 2D textures
    glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);        //Enable Local Viewer Light Model
    
    // Callbacks
	glutDisplayFunc( display );
	glutIdleFunc( idle );
	glutKeyboardFunc( keyboard );
	glutMouseFunc( mousebutton );
	glutMotionFunc( mousemotion );
	glutPassiveMotionFunc( mousemotion );
	glutReshapeFunc( resize );

	// Initialize components
	Cam.SetAspect(float(WinX)/float(WinY));
    
    Vector4 lightPos(0.0, 5.0, 10.0, 1.0);
    Vector4 lightPos1(-10.0, -5.0, 45.0, 0.0);
    light.position = lightPos;
    light1.position = lightPos1;
    
    light.ambientColor = Color::randomDarkPastel();
    light1.diffuseColor = Color::yellow();
    
    light.quadraticAttenuation = 0.02;
    light1.quadraticAttenuation = 0.02;
    
    if(mode == 0||mode == 1){
        if(exitHandler){
            glFinish();
            glutDestroyWindow(WindowHandle);
            exit(1);
        }
    
        if (_skel == NULL) {
            return;
        }
        GLUI *glui = GLUI_Master.create_glui( "GLUI" );
        GLUI_Panel *ep = new GLUI_Panel(glui,"",true);
        
        new GLUI_Button(ep, "Last Joint", 10,control_cb);
        new GLUI_Button(ep, "Next Joint", 11,control_cb);
    
        text = new GLUI_StaticText(ep, &(curr->name)[0]);
        
        new GLUI_Button(ep, "Last Channel", 17, control_cb);
        new GLUI_Button(ep, "Next Channel", 16, control_cb);
        channelNum = new GLUI_StaticText(ep, &("Channel: "+to_string(chanNum))[0]);
    
        do0 = new GLUI_StaticText(ep, &("1: "+to_string(curr->dofs[0].Value))[0]);
        new GLUI_Button(ep, "Increase", 2,control_cb);
        new GLUI_Button(ep, "Decrease", 3,control_cb);
    
        do1 = new GLUI_StaticText(ep, &("2: "+to_string(curr->dofs[1].Value))[0]);
        new GLUI_Button(ep, "Increase", 4,control_cb);
        new GLUI_Button(ep, "Decrease", 5,control_cb);
    
        do2 = new GLUI_StaticText(ep, &("3: "+to_string(curr->dofs[2].Value))[0]);
        new GLUI_Button(ep, "Increase", 6,control_cb);
        new GLUI_Button(ep, "Decrease", 7,control_cb);
    
        do3 = new GLUI_StaticText(ep, &("4: "+to_string(curr->dofs[3].Value))[0]);
        new GLUI_Button(ep, "Increase", 8,control_cb);
        new GLUI_Button(ep, "Decrease", 9,control_cb);
    
        do4 = new GLUI_StaticText(ep, &("5: "+to_string(curr->dofs[4].Value))[0]);
        new GLUI_Button(ep, "Increase", 12,control_cb);
        new GLUI_Button(ep, "Decrease", 13,control_cb);
    
        do5 = new GLUI_StaticText(ep, &("6: "+to_string(curr->dofs[5].Value))[0]);
        new GLUI_Button(ep, "Increase", 14,control_cb);
        new GLUI_Button(ep, "Decrease", 15,control_cb);
    
    
        GLUI_Checkbox* CB = new GLUI_Checkbox(ep, "Skeleton", &skelVisible, 1, control_cb);
    
        glui->set_main_gfx_window( WindowHandle );
    
        GLUI_Master.set_glutIdleFunc( idle );
        
        if(mode == 1){
            WinX2=640;
            WinY2=480;

            glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
            glutInitWindowSize( WinX2, WinY2 );
            glutInitWindowPosition( WinX+1, 0 );
            SecondWindowHandle = glutCreateWindow( "Curve" );
            glutSetWindowTitle( "Curve" );
            glutSetWindow( SecondWindowHandle );
            
            glClearColor( 0., 0., 0., 1. );
            
            glEnable(GL_DEPTH_TEST);                                    //Enable depth buffering
            glClear(GL_DEPTH_BUFFER_BIT);                               //Clear depth buffer
            
            // Callbacks
            glutDisplayFunc( curveDisplay );
            glutIdleFunc( idle );
            glutReshapeFunc( reshape );
            glutKeyboardFunc( keyboard );
            
            pixels = new float[WinX2 * WinY2 * 3];
        }
    }
    start = std::clock();
}

////////////////////////////////////////////////////////////////////////////////

Tester::~Tester() {
	glFinish();
	glutDestroyWindow(WindowHandle);
    
    if(mode == 1){
        glutDestroyWindow(SecondWindowHandle);
    }
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Update() {
	// Update the components in the world
	Cam.Update();
	//Cube.Update();
    
    if(mode == 0){
        if (_skel != NULL) {
            _skel->Update();
        }
    
        if (_skin != NULL) {
            _skin->Update();
        }
    }
    else if(mode == 1){
        duration = ( (float)std::clock() - (float)start ) / (float) CLOCKS_PER_SEC;
   
        animator->Play(duration*1);
        
        _rig->Update();
        
        channelNum->set_text(&("Channel: "+to_string(chanNum))[0]);
        do0->set_text(&("1: "+to_string(curr->dofs[0].Value))[0]);
        do1->set_text( &("2: "+to_string(curr->dofs[1].Value))[0]);
        do2->set_text( &("3: "+to_string(curr->dofs[2].Value))[0]);
        do3->set_text( &("4: "+to_string(curr->dofs[3].Value))[0]);
        do4->set_text( &("5: "+to_string(curr->dofs[4].Value))[0]);
        do5->set_text( &("6: "+to_string(curr->dofs[5].Value))[0]);
    }
    
    
	// Tell glut to re-display the scene
	glutSetWindow(WindowHandle);
    glutPostRedisplay();
    
    if(mode == 1){
        glutSetWindow(SecondWindowHandle);
        glutPostRedisplay();
    }
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Reset() {
	Cam.Reset();
	Cam.SetAspect(float(WinX)/float(WinY));
    _skel->Reset();
    
    if(mode == 1)
        start = std::clock();
	//Cube.Reset();
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Draw() {
    
	// Begin drawing scene
	glViewport(0, 0, WinX, WinY);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    if(mode == 0){
        light.bind(0);
        light1.bind(1);

        // Draw components
        Cam.Draw();		// Sets up projection & viewing matrices
        //Cube.Draw();
        if (_skel != NULL && skelVisible == -1)
            _skel->Draw();
    
        if (_skin != NULL)
            _skin->Draw();
    }
    else{
        light1.diffuseColor = Color::blue();
        
        light.bind(0);
        light1.bind(1);
        
        Cam.Draw();		// Sets up projection & viewing matrices
        _rig->setSkelVisible(skelVisible);
        
        _rig->Draw();
    }
	// Finish drawing scene
	glFinish();
	glutSwapBuffers();
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Quit() {
	glFinish();
	glutDestroyWindow(WindowHandle);
    if(mode == 1)
       glutDestroyWindow(SecondWindowHandle);
	exit(0);
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Resize(int x,int y) {
	WinX = x;
	WinY = y;
	Cam.SetAspect(float(WinX)/float(WinY));
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Keyboard(int key,int x,int y) {
	switch(key) {
		case 0x1b:		// Escape
			Quit();
			break;
		case 'r':
			Reset();
			break;
        case 'l':
            skelVisible *= -1;
            break;
        case '+':
            Controll(1,0,0,0,0,0,0);
            break;
        case '-':
            Controll(-1,0,0,0,0,0,0);
            break;
        case 'w':
            Controll(0,0,0,1,0,0,0);
            break;
        case 's':
            Controll(0,0,0,-1,0,0,0);
            break;
        case 'a':
            Controll(0,-1,0,0,0,0,0);
            break;
        case 'd':
            Controll(0,1,0,0,0,0,0);
            break;
        case 'q':
            Controll(0,0,1,0,0,0,0);
            break;
        case 'e':
            Controll(0,0,-1,0,0,0,0);
            break;
        case 'z':
            Controll(0,0,0,0,1,0,0);
            break;
        case 'Z':
            Controll(0,0,0,0,-1,0,0);
            break;
        case 'x':
            Controll(0,0,0,0,0,1,0);
            break;
        case 'X':
            Controll(0,0,0,0,0,-1,0);
            break;
        case 'c':
            Controll(0,0,0,0,0,0,1);
            break;
        case 'C':
            Controll(0,0,0,0,0,0,-1);
            break;
        case ',':
            chanNum++;
            break;
        case '.':
            chanNum--;
            break;
    }
}

void Tester::Controll(int changeJoint, int x, int y, int z, int xrot, int yrot, int zrot){
    if(mode == 0||mode == 1){
        if (_skel == NULL) {
            return;
        }
    
        currJoint += changeJoint;
        if (currJoint < 0)
            currJoint = 0;
    
        if (currJoint > (_skel->jointVec.size() -1))
            currJoint = _skel->jointVec.size() -1;
        
        curr->chosen = false;
        curr = _skel->jointVec.at(currJoint);
        curr->chosen = true;
        
        text->set_text(&(curr->name)[0]);
    
        DOF d0 = curr->dofs[0];
        d0.SetValue(d0.getValue()+x);
        curr->dofs[0] = d0;
    
        DOF d1 = curr->dofs[1];
        d1.SetValue(d1.getValue()+y);
        curr->dofs[1] = d1;
    
        DOF d2 = curr->dofs[2];
        d2.SetValue(d2.getValue()+z);
        curr->dofs[2] = d2;
    
        DOF d3 = curr->dofs[3];
        d3.SetValue(d3.getValue()+xrot);
        curr->dofs[3] = d3;
    
        DOF d4 = curr->dofs[4];
        d4.SetValue(d4.getValue()+yrot);
        curr->dofs[4] = d4;
    
        DOF d5 = curr->dofs[5];
        d5.SetValue(d5.getValue()+zrot);
        curr->dofs[5] = d5;
        
        do0->set_text(&("1: "+to_string(curr->dofs[0].Value))[0]);
        do1->set_text( &("2: "+to_string(curr->dofs[1].Value))[0]);
        do2->set_text( &("3: "+to_string(curr->dofs[2].Value))[0]);
        do3->set_text( &("4: "+to_string(curr->dofs[3].Value))[0]);
        do4->set_text( &("5: "+to_string(curr->dofs[4].Value))[0]);
        do5->set_text( &("6: "+to_string(curr->dofs[5].Value))[0]);
        channelNum->set_text(&("Channel: "+to_string(chanNum))[0]);
    
        cout<<curr->name<<": "
            <<curr->dofs[0].getValue()<<" "<<curr->dofs[1].getValue()
            <<" "<<curr->dofs[2].getValue()<<" "<<curr->dofs[3].getValue()
            <<" "<<curr->dofs[4].getValue()<<" "<<curr->dofs[5].getValue()
            <<endl;
    }
    else{
    
    }
}
////////////////////////////////////////////////////////////////////////////////

void Tester::MouseButton(int btn,int state,int x,int y) {
	if(btn==GLUT_LEFT_BUTTON) {
		LeftDown = (state==GLUT_DOWN);
	}
	else if(btn==GLUT_MIDDLE_BUTTON) {
		MiddleDown = (state==GLUT_DOWN);
	}
	else if(btn==GLUT_RIGHT_BUTTON) {
		RightDown = (state==GLUT_DOWN);
	}
}

////////////////////////////////////////////////////////////////////////////////

void Tester::MouseMotion(int nx,int ny) {
	int dx = nx - MouseX;
	int dy = -(ny - MouseY);

	MouseX = nx;
	MouseY = ny;

	// Move camera
	// NOTE: this should really be part of Camera::Update()
	if(LeftDown) {
		const float rate=1.0f;
		Cam.SetAzimuth(Cam.GetAzimuth()+dx*rate);
		Cam.SetIncline(Cam.GetIncline()-dy*rate);
	}
	if(RightDown) {
		const float rate=0.01f;
		Cam.SetDistance(Cam.GetDistance()*(1.0f-dx*rate));
	}
}

////////////////////////////////////////////////////////////////////////////////
