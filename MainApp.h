//
// Created by imanuel on 08.10.21.
//

#ifndef WXOCTO_MAINAPP_H
#define WXOCTO_MAINAPP_H

#include <wx/wxprec.h>

#ifndef WX_PRECOMP

#include <wx/wx.h>

#endif

#include <wx/preferences.h>

#include "mainwindow/MainWindow.h"
#include "settings/wxOctoSettings.h"


class MainApp : public wxApp {
private:
    wxScopedPtr<wxPreferencesEditor> preferenceEditor;
    wxOctoSettings appSettings;
    MainWindow *mainWindow;
    wxConfigBase *configuration;

public:
    MainApp();

    ~MainApp() override = default;

    bool OnInit() override;
    int OnExit() override;

    void ShowPreferencesEditor(wxWindow *parent);

    void DismissPreferencesEditor();

    const wxOctoSettings &GetSettings() const { return appSettings; }

    void UpdateSettings(const wxOctoSettings &settings);

    static MainApp *getInstance();
};


#endif //WXOCTO_MAINAPP_H
