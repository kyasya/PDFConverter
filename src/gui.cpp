// main.cpp
#include <wx/wx.h>
#include <wx/notebook.h>

class MainFrame : public wxFrame 
{
	public:
		MainFrame();

	private:
		void OnExit (wxCommandEvent&);
		void OnAbout(wxCommandEvent&);

		wxPanel* CreatePdfToPngTab(wxWindow* parent);
		wxPanel* CreatePngToPdfTab(wxWindow* parent);
		wxPanel* CreateMergeTab	  (wxWindow* parent);
		wxPanel* CreateExtractTab (wxWindow* parent);

		wxBoxSizer* CreateFileInput(wxWindow* parent, const wxString& label, int textCtrlId);
		wxBoxSizer* CreateDirInput (wxWindow* parent, const wxString& label, int textCtrlId);
};


class MyApp : public wxApp 
{
	public:
		bool OnInit() override
		{
			auto frame = new MainFrame();
			frame->Show(true);
			return true;
		}
};

MainFrame::MainFrame(): wxFrame(nullptr, wxID_ANY, "PDF Controller", wxDefaultPosition, wxSize(600, 400))
{
    // メニュー
    auto menuBar   = new wxMenuBar();
    auto fileMenu  = new wxMenu();
    auto aboutMenu = new wxMenu();
    
	fileMenu ->Append( wxID_EXIT,  "Exit");
    aboutMenu->Append(wxID_ABOUT, "About");
    
	menuBar->Append( fileMenu,  "File");
    menuBar->Append(aboutMenu, "About");
    
	SetMenuBar(menuBar);

    // ノートブック
    auto notebook = new wxNotebook(this, wxID_ANY);
    notebook->AddPage(CreatePdfToPngTab(notebook), "PDF to PNG");
    notebook->AddPage(CreatePngToPdfTab(notebook), "PNG to PDF");
    notebook->AddPage(CreateMergeTab   (notebook), 		"Merge");
    notebook->AddPage(CreateExtractTab (notebook), "Extraction");

    // イベント
    Bind(wxEVT_MENU,  &MainFrame::OnExit, this,  wxID_EXIT);
    Bind(wxEVT_MENU, &MainFrame::OnAbout, this, wxID_ABOUT);
}

void MainFrame::OnExit(wxCommandEvent&) {Close(true);}

void MainFrame::OnAbout(wxCommandEvent&) 
{
    wxMessageBox("PDF Controller\nver 2.0\nProgrammed by K*", "About", wxOK | wxICON_INFORMATION);
}

wxPanel* MainFrame::CreatePdfToPngTab(wxWindow* parent) 
{
    auto panel = new wxPanel(parent);
    auto sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(new wxStaticText(panel,  wxID_ANY, "PDF to PNG Converter"), 0, wxALL, 10);
	sizer->Add( CreateFileInput(panel,	 "PDF名", wxID_HIGHEST+1), 0, wxEXPAND | wxALL, 5);
    sizer->Add(new 	   wxButton(panel, wxID_HIGHEST+2, "Convert"), 0, wxALL, 5);
    panel->SetSizer(sizer);
    return panel;
}

wxPanel* MainFrame::CreatePngToPdfTab(wxWindow* parent) 
{
    auto panel = new wxPanel(parent);
    auto sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(new wxStaticText(panel, wxID_ANY, "PNG to PDF Converter"), 0, wxALL, 10);
    sizer->Add(	CreateFileInput(panel, "PNG名", wxID_HIGHEST + 3), 0, wxEXPAND | wxALL, 5);
    sizer->Add(new	   wxButton(panel, wxID_HIGHEST + 4, "Convert"), 0, wxALL, 5);
    panel->SetSizer(sizer);
    return panel;
}

wxPanel* MainFrame::CreateMergeTab(wxWindow* parent) 
{
    auto panel = new wxPanel(parent);
    auto sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(new wxStaticText(panel, wxID_ANY, "Merge"), 0, wxALL, 10);
    sizer->Add(CreateFileInput(panel, "PDF/PNG名", wxID_HIGHEST + 5), 0, wxEXPAND | wxALL, 5);
    sizer->Add(CreateDirInput(panel, "出力先", wxID_HIGHEST + 6), 0, wxEXPAND | wxALL, 5);
    sizer->Add(new wxButton(panel, wxID_HIGHEST + 7, "Merge"), 0, wxALL, 5);
    panel->SetSizer(sizer);
    return panel;
}

wxPanel* MainFrame::CreateExtractTab(wxWindow* parent) 
{
    auto panel = new wxPanel(parent);
    auto sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(new wxStaticText(panel, wxID_ANY, "Extraction"), 0, wxALL, 10);
    sizer->Add(CreateFileInput(panel, "PDF名", wxID_HIGHEST + 8), 0, wxEXPAND | wxALL, 5);
    sizer->Add(new wxButton(panel, wxID_HIGHEST + 9, "Extraction"), 0, wxALL, 5);
    panel->SetSizer(sizer);
    return panel;
}

wxBoxSizer* MainFrame::CreateFileInput(wxWindow* parent, const wxString& label, int textCtrlId) 
{
    auto* sizer = new wxBoxSizer(wxHORIZONTAL);
    sizer->Add(new wxStaticText(parent, wxID_ANY, label, wxDefaultPosition, wxSize(80, -1)), 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);
    sizer->Add(new wxTextCtrl(parent, textCtrlId, "", wxDefaultPosition, wxSize(300, -1)), 1);
    sizer->Add(new wxButton(parent, wxID_ANY, "Browse"), 0, wxLEFT, 5);
    return sizer;
}

wxBoxSizer* MainFrame::CreateDirInput(wxWindow* parent, const wxString& label, int textCtrlId) 
{
    auto* sizer = new wxBoxSizer(wxHORIZONTAL);
    sizer->Add(new wxStaticText(parent, wxID_ANY, label, wxDefaultPosition, wxSize(80, -1)), 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);
    sizer->Add(new wxTextCtrl(parent, textCtrlId, "", wxDefaultPosition, wxSize(300, -1)), 1);
    sizer->Add(new wxButton(parent, wxID_ANY, "Select Folder"), 0, wxLEFT, 5);
    return sizer;
}


wxIMPLEMENT_APP(MyApp);
