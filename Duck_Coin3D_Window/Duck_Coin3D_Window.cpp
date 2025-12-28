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

#include "Duck_Coin3D_Window.h"

#include <ostream>
#include <GL/gl.h>
#include <GL/glut.h>

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

    SoDB::getSensorManager()->processTimerQueue();
    SoDB::getSensorManager()->processDelayQueue(TRUE);

    auto pb = static_cast<Duck_Coin3D_Window *>(userdata);
    pb->redraw();
    Fl::repeat_timeout(1.0/24.0, Timer_CB, userdata);
}

Duck_Coin3D_Window::Duck_Coin3D_Window(int X, int Y, int W, int H, const char *L )
    : Fl_Gl_Window(X, Y, W, H, L) {

    Fl::add_timeout(1.0/24.0, Timer_CB, (void*)this);       // 24fps timer

    isGLInitialized = false;
    sceneManager = nullptr;
    root = nullptr;
    myCamera = nullptr;

    SoDB::init();

    // initialize sceneManager
    sceneManager = new SoSceneManager;

    auto root = new SoSeparator;
    root->ref();

    // add a camera to the graph
    auto perspCamera = new SoPerspectiveCamera;
    root->addChild(perspCamera);

    //add a light
    root->addChild(new SoDirectionalLight);
    // finally add our scene by calling createScene()
    root->addChild(init_scene_graph("duck.iv"));

    // Pass the scene graph to sceneManager
    sceneManager->setSceneGraph(root);

    SbViewportRegion viewportRegion;
    viewportRegion.setWindowSize(W, H);
    sceneManager->setViewportRegion(viewportRegion);

    // view the whole scene
    perspCamera->viewAll(root, sceneManager->getViewportRegion());
}

Duck_Coin3D_Window::~Duck_Coin3D_Window() {
    sceneManager->getSceneGraph()->unref();
    delete sceneManager;
}

void Duck_Coin3D_Window::init_GL() {

}

void Duck_Coin3D_Window::draw() {
    if (!valid()) {

    }
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    sceneManager->render();
}
#if 1
// Il metodo handle intercetta tutti gli eventi inviati al widget
int Duck_Coin3D_Window::handle(int event)  {
    switch (event) {
        case FL_PUSH: // Click del mouse
            std::cout << "Tasto premuto: " << Fl::event_button()
                      << " a coordinate: (" << Fl::event_x() << "," << Fl::event_y() << ")" << std::endl;
            return 1; // Ritorna 1 per dire che l'evento è stato gestito

        case FL_DRAG: // Trascinamento
        case FL_MOVE: // Movimento semplice
            char coords[50];
            sprintf(coords, "X: %d, Y: %d", Fl::event_x(), Fl::event_y());
            this->copy_label(coords); // Aggiorna il testo sul widget
            return 1;

        case FL_ENTER: // Il mouse entra nel widget
            color(FL_LIGHT1);
            redraw();
            return 1;

        case FL_LEAVE: // Il mouse esce dal widget
            color(FL_WHITE);
            label("Muovi il mouse qui dentro");
            redraw();
            return 1;

        default:
            return Fl_Gl_Window::handle(event); // Passa gli altri eventi alla classe base
    }
    return 0;
}
#endif


SoSeparator*
Duck_Coin3D_Window::init_scene_graph(const std::string& fileName)
{
    root = new SoSeparator;
    root->ref();

    // Aggiungiamo un materiale (colore rosso)
    SoMaterial *myMaterial = new SoMaterial;
    myMaterial->diffuseColor.setValue(1.0, 0.0, 0.0);
    root->addChild(myMaterial);

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

    // Bigger duck group
    SoSeparator *bigDuck = new SoSeparator;
    root->addChild(bigDuck);
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

    root->unrefNoDelete();
    return root;
}
