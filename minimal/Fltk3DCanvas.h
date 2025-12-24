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

#ifndef COIN3D_CANVAS_H
#define COIN3D_CANVAS_H

#include <FL/Fl_Gl_Window.H>

#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/SoSceneManager.h>

#include <string>

// Creiamo una classe che estende Fl_Gl_Window
class Fltk3DCanvas : public Fl_Gl_Window {
    void InitGL();
    void InitEmptyScene();
    int initSceneGraph(const std::string& fileName);

    bool isGLInitialized;
    SoSceneManager* sceneManager;
    SoSeparator *root;
    SoPerspectiveCamera *myCamera;

public:
    Fltk3DCanvas(int X, int Y, int W, int H, const char* L=nullptr);

    void draw() override ;
};



#endif //COIN3D_CANVAS_H
