/*
 * fltkCoin3D Porting on fltk of Coin3D (a.k.a Open Inventor) examples
 * Copyright (C) 2025  Fabrizio Morciano

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
