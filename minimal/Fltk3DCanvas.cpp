/*
 * fltkCoin3D Porting on wxWidgets of Coin3D (a.k.a Open Inventor) examples
 * Copyright (C) 2022  Fabrizio Morciano

 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.

 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
 * USA
 */

#include "Fltk3DCanvas.h"

#include <ostream>
#include <GL/gl.h>
#include <GL/glut.h>

#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/engines/SoElapsedTime.h>
#include <Inventor/events/SoMouseButtonEvent.h>
#include <Inventor/nodes/SoCylinder.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/nodes/SoEventCallback.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoRotationXYZ.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/SbViewportRegion.h>
#include <Inventor/SoDB.h>
#include <Inventor/SoInput.h>
#include <Inventor/nodekits/SoNodeKit.h>
#include <Inventor/SoInteraction.h>
#include <Inventor/engines/SoBoolOperation.h>
#include <Inventor/engines/SoCompose.h>
#include <Inventor/engines/SoGate.h>
#include <Inventor/nodes/SoSphere.h>
#include <Inventor/events/SoMotion3Event.h>

#include <iostream>

// 24 FPS TIMER CALLBACK
//     Called 24x per second to redraw the widget
//
static void Timer_CB(void *userdata) {
    auto pb = static_cast<Fltk3DCanvas *>(userdata);
    pb->redraw();
    Fl::repeat_timeout(1.0/24.0, Timer_CB, userdata);
}


#define COIN 1
Fltk3DCanvas::Fltk3DCanvas(int X, int Y, int W, int H, const char *L )
    : Fl_Gl_Window(X, Y, W, H, L) {

    Fl::add_timeout(1.0/24.0, Timer_CB, (void*)this);       // 24fps timer

    isGLInitialized = false;
    sceneManager = nullptr;
    root = nullptr;
    myCamera = nullptr;

#ifdef COIN
    SoDB::init();
    SoNodeKit::init();
    SoInteraction::init();

    // Add a camera and light
    myCamera = new SoPerspectiveCamera;
    myCamera->ref();
    myCamera->position.setValue(0., -4., 8.0);
    myCamera->heightAngle = M_PI/2.5;
    myCamera->nearDistance = 1.0;
    myCamera->farDistance = 15.0;

    sceneManager = new SoSceneManager;
    sceneManager->setCamera(myCamera);

    if(initSceneGraph("duck.iv")) {
        fprintf(stderr, "couldn't read IV file\n");
        exit(1);
    }

    SbViewportRegion viewportRegion;
    viewportRegion.setWindowSize(W, H);
    sceneManager->setViewportRegion(viewportRegion);

    isGLInitialized = false;
#endif

}

void  Fltk3DCanvas::InitEmptyScene() {

}

void Fltk3DCanvas::InitGL() {
    if(!isGLInitialized) {
        glEnable(GL_DEPTH_TEST);
        isGLInitialized = true;
    }

    SbColor backColor;
    backColor.setValue(0.3f, 0.4f, 0.6f);
    sceneManager->setBackgroundColor( backColor);
}

void Fltk3DCanvas::draw() {
    if (!valid()) {
#ifdef COIN
        InitGL();
        // Imposta il viewport quando la finestra è creata o ridimensionata
#else

        glViewport(0, 0, w(), h());
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-1, 1, -1, 1, -1, 1);
        glMatrixMode(GL_MODELVIEW);
#endif

    }

#ifdef COIN
    SbViewportRegion myViewport(w(), h());

    SoGLRenderAction myRenderAction(myViewport);
    myRenderAction.apply(root);

    glFlush();
#else

    // Cancella lo schermo con colore nero
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    // Disegna un triangolo colorato
    glLoadIdentity();
    glBegin(GL_TRIANGLES);
    glColor3f(1.0, 0.0, 0.0);
    glVertex2f(-0.6f, -0.4f);
    glColor3f(0.0, 1.0, 0.0);
    glVertex2f(0.6f, -0.4f);
    glColor3f(0.0, 0.0, 1.0);
    glVertex2f(0.0f, 0.6f);
    glEnd();
#endif
    std::cout<<"Prova"<<std::endl;
}


int
Fltk3DCanvas::initSceneGraph(const std::string& fileName)
{
    if(root)
        root->unref();

    root = new SoSeparator;
    root->ref();

    // Add a camera and light
    //SoPerspectiveCamera *
    myCamera = new SoPerspectiveCamera;
    myCamera->position.setValue(0., -4., 8.0);
    myCamera->heightAngle = M_PI/2.5;
    myCamera->nearDistance = 1.0;
    myCamera->farDistance = 15.0;
    root->addChild(myCamera);
    root->addChild(new SoDirectionalLight);

    // Rotate scene slightly to get better view
    auto globalRotXYZ = new SoRotationXYZ;
    globalRotXYZ->axis = SoRotationXYZ::X;
    globalRotXYZ->angle = M_PI/9;
    root->addChild(globalRotXYZ);

    // Pond group
    auto pond = new SoSeparator;
    root->addChild(pond);
    auto pondTranslation = new SoTranslation;
    pondTranslation->translation.setValue(0., -6.725, 0.);
    pond->addChild(pondTranslation);
    // water
    auto waterMaterial = new SoMaterial;
    waterMaterial->diffuseColor.setValue(0., 0.3, 0.8);
    pond->addChild(waterMaterial);
    SoCylinder *waterCylinder = new SoCylinder;
    waterCylinder->radius.setValue(4.0);
    waterCylinder->height.setValue(0.5);
    pond->addChild(waterCylinder);
    // rock
    SoMaterial *rockMaterial = new SoMaterial;
    rockMaterial->diffuseColor.setValue(0.8, 0.23, 0.03);
    pond->addChild(rockMaterial);
    SoSphere *rockSphere = new SoSphere;
    rockSphere->radius.setValue(0.9);
    pond->addChild(rockSphere);

    // Read the duck object from a file and add to the group
    SoInput myInput;
    SoInput::addDirectoryFirst("./data");

    if (!myInput.openFile(fileName.c_str()))
        exit (1);
    SoSeparator *duckObject = SoDB::readAll(&myInput);
    if (duckObject == NULL)
        exit (1);

/////////////////////////////////////////////////////////////
// CODE FOR The Inventor Mentor STARTS HERE

    // Bigger duck group
    SoSeparator *bigDuck = new SoSeparator;
    root->addChild(bigDuck);
#if 0
    SoRotationXYZ * bigDuckRotXYZ = new SoRotationXYZ;
    bigDuck->addChild(bigDuckRotXYZ);
    SoTransform *bigInitialTransform = new SoTransform;
    bigInitialTransform->translation.setValue(0., 0., 3.5);
    bigInitialTransform->scaleFactor.setValue(6., 6., 6.);
    bigDuck->addChild(bigInitialTransform);
    bigDuck->addChild(duckObject);

    // Smaller duck group
    SoSeparator *smallDuck = new SoSeparator;
    root->addChild(smallDuck);
    SoRotationXYZ *smallDuckRotXYZ = new SoRotationXYZ;
    smallDuck->addChild(smallDuckRotXYZ);
    SoTransform *smallInitialTransform = new SoTransform;
    smallInitialTransform->translation.setValue(0., -2.24, 1.5);
    smallInitialTransform->scaleFactor.setValue(4., 4., 4.);
    smallDuck->addChild(smallInitialTransform);
    smallDuck->addChild(duckObject);

    // Use a gate engine to start/stop the rotation of
    // the bigger duck.
    SoGate *bigDuckGate = new SoGate(SoMFFloat::getClassTypeId());
    bigDuckGate->ref();
    SoElapsedTime * bigDuckTime = new SoElapsedTime;
    bigDuckTime->ref();
    bigDuckGate->input->connectFrom(&bigDuckTime->timeOut);
    bigDuckRotXYZ->axis = SoRotationXYZ::Y;  // Y axis
    bigDuckRotXYZ->angle.connectFrom(bigDuckGate->output);

    // Each mouse button press will enable/disable the gate
    // controlling the bigger duck.
    SoEventCallback *myEventCB = new SoEventCallback;
    myEventCB->addEventCallback(
            SoMouseButtonEvent::getClassTypeId(),
            nullptr, bigDuckGate);
    root->addChild(myEventCB);

    // Use a Boolean engine to make the rotation of the smaller
    // duck depend on the bigger duck.  The smaller duck moves
    // only when the bigger duck is still.
    SoBoolOperation *myBoolean = new SoBoolOperation;
    myBoolean->ref();
    myBoolean->a.connectFrom(&bigDuckGate->enable);
    myBoolean->operation = SoBoolOperation::NOT_A;

    SoGate * smallDuckGate = new SoGate(SoMFFloat::getClassTypeId());
    smallDuckGate->ref();
    SoElapsedTime * smallDuckTime = new SoElapsedTime;
    smallDuckTime->ref();
    smallDuckGate->input->connectFrom(&smallDuckTime->timeOut);
    smallDuckGate->enable.connectFrom(&myBoolean->output);
    smallDuckRotXYZ->axis = SoRotationXYZ::Y;  // Y axis
    smallDuckRotXYZ->angle.connectFrom(smallDuckGate->output);

// CODE FOR The Inventor Mentor ENDS HERE
/////////////////////////////////////////////////////////////
#endif
    SbViewportRegion myViewport(w(), h());
    myCamera->viewAll(root, myViewport);

    return 0;
}
