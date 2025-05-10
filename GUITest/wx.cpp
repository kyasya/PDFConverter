// File: wx_button.cpp
#include <wx/wx.h>

class MyApp : public wxApp
{
public:
    virtual bool OnInit();
};

class MyFrame : public wxFrame
{
public:
    MyFrame() : wxFrame(NULL, wxID_ANY, "Hello from wxWidgets")
    {
        new wxButton(this, wxID_ANY, "Click Me", wxPoint(30, 30));
    }
};

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
    MyFrame *frame = new MyFrame();
    frame->Show(true);
    return true;
}
