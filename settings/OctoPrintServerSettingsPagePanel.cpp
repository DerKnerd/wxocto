//
// Created by imanuel on 08.10.21.
//

#include "OctoPrintServerSettingsPagePanel.h"
#include "../MainApp.h"
#include "../helper.h"

OctoPrintServerSettingsPagePanel::OctoPrintServerSettingsPagePanel(wxWindow *parent) : wxPanel(parent) {
    auto serverLabel = new wxStaticText(this, wxID_ANY, _("Octoprint server"));
    server = new wxTextCtrl(this, wxID_ANY);
    server->SetMinSize(wxSize(400, -1));

    auto apiKeyLabel = new wxStaticText(this, wxID_ANY, _("Octoprint api key"));
    apiKey = new wxTextCtrl(this, wxID_ANY);
    apiKey->SetLabel(_("Octoprint api key"));
    apiKey->SetMinSize(wxSize(400, -1));
    apiKey->SetWindowStyleFlag(wxTE_PASSWORD);

    auto sizer = new wxFlexGridSizer(2);
    sizer->SetFlexibleDirection(wxBOTH);
    sizer->SetHGap(WXC_FROM_DIP(5));
    sizer->SetVGap(WXC_FROM_DIP(5));

    sizer->Add(serverLabel, wxSizerFlags().Border());
    sizer->Add(server, wxSizerFlags().Border());

    sizer->Add(apiKeyLabel, wxSizerFlags().Border());
    sizer->Add(apiKey, wxSizerFlags().Border());

    SetSizerAndFit(sizer);

    server->Bind(wxEVT_TEXT, &OctoPrintServerSettingsPagePanel::handleServerChanged, this);
    apiKey->Bind(wxEVT_TEXT, &OctoPrintServerSettingsPagePanel::handleApiKeyChanged, this);
}

bool OctoPrintServerSettingsPagePanel::TransferDataToWindow() {
    currentSettings = MainApp::getInstance()->GetSettings();
    apiKey->SetValue(currentSettings.apiKey);
    server->SetValue(currentSettings.server);
    return true;
}

bool OctoPrintServerSettingsPagePanel::TransferDataFromWindow() {
    MainApp::getInstance()->UpdateSettings(currentSettings);
    return true;
}

void OctoPrintServerSettingsPagePanel::updateSettingsIfNecessary() {
    if (wxPreferencesEditor::ShouldApplyChangesImmediately()) {
        MainApp::getInstance()->UpdateSettings(currentSettings);
    }
}

void OctoPrintServerSettingsPagePanel::handleServerChanged(wxCommandEvent &event) {
    currentSettings.server = event.GetString();
    updateSettingsIfNecessary();
}

void OctoPrintServerSettingsPagePanel::handleApiKeyChanged(wxCommandEvent &event) {
    currentSettings.apiKey = event.GetString();
    updateSettingsIfNecessary();
}
