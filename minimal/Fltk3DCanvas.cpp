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

#include <GL/gl.h>

#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/engines/SoElapsedTime.h>
#include <Inventor/nodes/SoCylinder.h>
#include <Inventor/nodes/SoEventCallback.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/SbViewportRegion.h>
#include <Inventor/SoDB.h>

#include <Inventor/nodes/SoComplexity.h>
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/nodes/SoSphere.h>
#include <Inventor/nodes/SoTexture2.h>
#include <Inventor/nodes/SoTranslation.h>

#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/nodes/SoRotationXYZ.h>

#include <iostream>

// 24 FPS TIMER CALLBACK
//     Called 24x per second to redraw the widget
//
static void Timer_CB(void *userdata) {
    auto pb = static_cast<Fltk3DCanvas *>(userdata);
    pb->angle += 0.01;
    if (pb->globeSpin)
        pb->globeSpin->angle = pb->angle;
    // Very important, Coin need to process internal timer, this need to be performed in the canvas periodically
    SoDB::getSensorManager()->processTimerQueue();
    pb->redraw();
    Fl::repeat_timeout(1.0 / 24.0, Timer_CB, userdata);
}


Fltk3DCanvas::Fltk3DCanvas(int X, int Y, int W, int H, const char *L)
    : Fl_Gl_Window(X, Y, W, H, L) {
    Fl::add_timeout(1.0 / 24.0, Timer_CB, (void *) this); // 24fps timer

    root = nullptr;
    globeSpin = nullptr;
    angle = 0;

    isGLInitialized = false;
    root = nullptr;

    SoDB::init();
}

Fltk3DCanvas::~Fltk3DCanvas() {
    if (root)
        root->unref();
}

void Fltk3DCanvas::initGL() {
    if (!isGLInitialized) {
        glEnable(GL_DEPTH_TEST);
        isGLInitialized = true;
        globeScene();
    }
}

void Fltk3DCanvas::globeScene() {
    root = new SoSeparator;
    root->ref();

    // Add a camera and light
    SoPerspectiveCamera *myCamera = new SoPerspectiveCamera;
    myCamera->position.setValue(0., 0., 2.2);
    myCamera->heightAngle = M_PI / 2.5;
    myCamera->nearDistance = 0.5;
    myCamera->farDistance = 10.0;
    root->addChild(myCamera);
    root->addChild(new SoDirectionalLight);

    SoRotationXYZ *globalRotXYZ = new SoRotationXYZ;
    globalRotXYZ->axis = SoRotationXYZ::X;
    globalRotXYZ->angle = M_PI / 9;
    root->addChild(globalRotXYZ);

    // Set up the globe transformations
    globeSpin = new SoRotationXYZ;
    root->addChild(globeSpin);
    globeSpin->angle = angle;
    globeSpin->axis = SoRotationXYZ::Y; // rotate about Y axis

    // Add the globe, a sphere with a texture map.
    // Put it within a separator.
    SoSeparator *sphereSep = new SoSeparator;
    SoTexture2 *myTexture2 = new SoTexture2;
    SoComplexity *sphereComplexity = new SoComplexity;
    sphereComplexity->value = 0.55;
    root->addChild(sphereSep);
    sphereSep->addChild(myTexture2);
    sphereSep->addChild(sphereComplexity);
    sphereSep->addChild(new SoSphere);
    myTexture2->filename = "globe.rgb";
}


void Fltk3DCanvas::draw() {
    if (!valid()) {
        initGL();
    }

    glClearColor( 0.3f, 0.4f, 0.6f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    SbViewportRegion myViewport( w(), h());
    SoGLRenderAction myRenderAction(myViewport);
    myRenderAction.apply(root);

    glFlush();
    this->swap_buffers();
}


