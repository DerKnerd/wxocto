//
// Created by imanuel on 08.10.21.
//

#include "OctoPrintServerSettingsPage.h"
#include "OctoPrintServerSettingsPagePanel.h"

wxWindow *OctoPrintServerSettingsPage::CreateWindow(wxWindow *parent) {
    return new OctoPrintServerSettingsPagePanel(parent);
}
