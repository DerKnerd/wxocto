//
// Created by imanuel on 08.10.21.
//

#ifndef WXOCTO_OCTOPRINTSERVERSETTINGSPAGEPANEL_H
#define WXOCTO_OCTOPRINTSERVERSETTINGSPAGEPANEL_H

#include <wx/wxprec.h>

#ifndef WX_PRECOMP

#include <wx/wx.h>
#include <wx/preferences.h>
#include "wxOctoSettings.h"

#endif

class OctoPrintServerSettingsPagePanel : public wxPanel {
public:
    explicit OctoPrintServerSettingsPagePanel(wxWindow *parent);

    bool TransferDataToWindow() override;

    bool TransferDataFromWindow() override;

private:
    void updateSettingsIfNecessary();

    void handleServerChanged(wxCommandEvent &event);

    void handleApiKeyChanged(wxCommandEvent &event);

    wxTextCtrl *server;
    wxTextCtrl *apiKey;
    wxOctoSettings currentSettings;
};


#endif //WXOCTO_OCTOPRINTSERVERSETTINGSPAGEPANEL_H
