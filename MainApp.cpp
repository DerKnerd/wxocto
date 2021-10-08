//
// Created by imanuel on 08.10.21.
//

#include "MainApp.h"
#include "settings/OctoPrintServerSettingsPage.h"

MainApp::MainApp() {
    mainWindow = nullptr;
    configuration = wxConfigBase::Get();
    configuration->SetAppName("wxOcto");
    configuration->SetVendorName("DerKnerd");
    appSettings = wxOctoSettings();

    appSettings.server = configuration->Read("/printer/server", "octopi.local");
    appSettings.apiKey = configuration->Read("/printer/apikey");
}

bool MainApp::OnInit() {
    if (!wxApp::OnInit())
        return false;

    // Add the common image handlers
    wxInitAllImageHandlers();
    SetAppDisplayName(_("wxOcto"));
    mainWindow = new MainWindow();
    SetTopWindow(mainWindow);

    return GetTopWindow()->Show();
}

void MainApp::ShowPreferencesEditor(wxWindow *parent) {
    if (!preferenceEditor) {
        preferenceEditor.reset(new wxPreferencesEditor());
    }

    preferenceEditor->AddPage(new OctoPrintServerSettingsPage());
    preferenceEditor->Show(parent);
}

void MainApp::DismissPreferencesEditor() {
    if (preferenceEditor) {
        preferenceEditor->Dismiss();
    }
}

void MainApp::UpdateSettings(const wxOctoSettings &settings) {
    appSettings = settings;
    configuration->Write("/printer/server", wxString(appSettings.server));
    configuration->Write("/printer/apikey", wxString(appSettings.apiKey));
    mainWindow->reloadSettings();
}

MainApp *MainApp::getInstance() {
    auto app = wxApp::GetInstance();

    return dynamic_cast<MainApp *>(app);
}

int MainApp::OnExit() {
    delete wxConfigBase::Set((wxConfigBase *) nullptr);
    return wxAppBase::OnExit();
}

