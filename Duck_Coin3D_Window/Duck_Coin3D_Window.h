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

#ifndef Duck_Coin3D_Window_CANVAS_H
#define Duck_Coin3D_Window_CANVAS_H

#include <FL/Fl_Gl_Window.H>

#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/SoSceneManager.h>

#include <string>

// Creiamo una classe che estende Fl_Gl_Window
class Duck_Coin3D_Window : public Fl_Gl_Window {

    bool isGLInitialized;
    SoSceneManager* sceneManager;
    SoSeparator *root;
    SoPerspectiveCamera *myCamera;

    void init_GL();
    SoSeparator* init_scene_graph(const std::string& fileName);

public:
    Duck_Coin3D_Window(int X, int Y, int W, int H, const char* L=nullptr);
    ~Duck_Coin3D_Window() override;
    int handle(int event) override;
    void draw() override ;
};



#endif //Duck_Coin3D_Window_CANVAS_H
