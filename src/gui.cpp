// main.cpp
#include <wx/wx.h>

class MyApp : public wxApp {
public:
    virtual bool OnInit();
};

class MyFrame : public wxFrame {
public:
    MyFrame(const wxString& title);
};

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit() {
    MyFrame* frame = new MyFrame("Hello wxWidgets");
    frame->Show(true);
    return true;
}

MyFrame::MyFrame(const wxString& title)
    : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(400, 300)) {
    // ウィジェットなどをここに配置
    wxPanel* panel = new wxPanel(this, wxID_ANY);
    new wxStaticText(panel, wxID_ANY, "こんにちは、wxWidgets！", wxPoint(20, 20));
}
