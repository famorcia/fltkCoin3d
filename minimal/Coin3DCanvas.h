/*
 * wxCoin3D Porting on wxWidgets of Coin3D (a.k.a Open Inventor) examples
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

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Gl_Window.H>
#include <FL/gl.h>

//#include <Inventor/nodes/SoPerspectiveCamera.h>


// Creiamo una classe che estende Fl_Gl_Window
class MyGLWindow : public Fl_Gl_Window {
public:
    MyGLWindow(int X, int Y, int W, int H, const char* L=0);

    void draw() override ;
};

#if 0
class Coin3DCanvas : public wxGLCanvas
{
public:
    Coin3DCanvas(wxWindow *parent,
                 wxGLAttributes&,
                 wxWindowID id = wxID_ANY,
                 const wxPoint& pos = wxDefaultPosition,
                 const wxSize& size = wxDefaultSize,
                 long style = 0,
                 const wxString& name = "Coin3DCanvas"
    );

    virtual ~Coin3DCanvas();

protected:
    void OnPaint(wxPaintEvent& event);
    void OnSize(wxSizeEvent& event);
    void OnEraseBackground(wxEraseEvent& event);
    void OnTimer(wxTimerEvent& event);

private:
    enum {
        TIMER_ID = 3000
    };
    void InitGL();
    int InitDuckGraph();

    bool isGLInitialized;
    wxTimer timer;
    wxGLContext* glRealContext;
    SoSeparator *root;
    SoPerspectiveCamera *myCamera;
    int W;
    int H;
    wxDECLARE_NO_COPY_CLASS(Coin3DCanvas);
    wxDECLARE_EVENT_TABLE();
};
#endif


#endif //COIN3D_CANVAS_H
