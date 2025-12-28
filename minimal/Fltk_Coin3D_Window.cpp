/**
* fltkCoin3D Porting on fltk of Coin3D (a.k.a Open Inventor) examples
 *
 * MIT License
 *
 * Copyright (c) 2025 Fabrizio Morciano
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#include "Fltk_Coin3D_Window.h"

#include <GL/gl.h>

#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/engines/SoElapsedTime.h>
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

// 24 FPS TIMER CALLBACK
//     Called 24x per second to redraw the widget
//
static void Timer_CB(void *userdata) {
    auto pb = static_cast<Fltk_Coin3D_Window *>(userdata);
    if (!pb)
        return;
    pb->update_angle();
    // Very important, Coin need to process internal timer, this need to be performed in the canvas periodically
    SoDB::getSensorManager()->processTimerQueue();
    pb->redraw();
    Fl::repeat_timeout(1.0 / 24.0, Timer_CB, userdata);
}


Fltk_Coin3D_Window::Fltk_Coin3D_Window(int X, int Y, int W, int H, const char *L)
    : Fl_Gl_Window(X, Y, W, H, L) {
    Fl::add_timeout(1.0 / 24.0, Timer_CB, (void *) this); // 24fps timer

    root = nullptr;
    globeSpin = nullptr;
    angle = 0;

    isGLInitialized = false;
    root = nullptr;

    SoDB::init();
}

Fltk_Coin3D_Window::~Fltk_Coin3D_Window() {
    if (root)
        root->unref();
}

void Fltk_Coin3D_Window::initGL() {
    if (!isGLInitialized) {
        glEnable(GL_DEPTH_TEST);
        isGLInitialized = true;
        globeScene();
    }
}

void Fltk_Coin3D_Window::globeScene() {
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

void Fltk_Coin3D_Window::update_angle() {
    // could be called by timer before initialization is done
    if (globeSpin) {
        angle += 0.01;
        globeSpin->angle = angle;
    }
}

void Fltk_Coin3D_Window::draw() {
    if (!valid()) {
        initGL();
    }

    glClearColor(0.3f, 0.4f, 0.6f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    SbViewportRegion myViewport(w(), h());
    SoGLRenderAction myRenderAction(myViewport);
    myRenderAction.apply(root);

    glFlush();
    this->swap_buffers();
}
