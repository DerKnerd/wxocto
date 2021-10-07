#include <wx/app.h>
#include <wx/image.h>
#include "MainWindow.h"

// Define the MainApp
class MainApp : public wxApp {
public:
    MainApp() = default;

    ~MainApp() override = default;

    bool OnInit() override {
        // Add the common image handlers
        wxImage::AddHandler(new wxPNGHandler);
        wxImage::AddHandler(new wxJPEGHandler);

        auto mainFrame = new MainWindow();
        SetTopWindow(mainFrame);
        return GetTopWindow()->Show();
    }
};

wxDECLARE_APP(MainApp);

wxIMPLEMENT_APP(MainApp);