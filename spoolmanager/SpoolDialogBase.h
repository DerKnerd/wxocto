//
// Created by imanuel on 16.10.21.
//

#ifndef WXOCTO_SPOOLDIALOGBASE_H
#define WXOCTO_SPOOLDIALOGBASE_H

#include <wx/wxprec.h>

#ifndef WX_PRECOMP

#include <wx/wx.h>

#endif

#include <wx/clrpicker.h>
#include <wx/spinctrl.h>
#include <wx/datectrl.h>
#include "../octoprint/spoolmanager/OctoprintSpool.h"

class SpoolDialogBase : public wxDialog {
public:
    explicit SpoolDialogBase(wxWindow *parent, const wxString &title, int winid);

    enum SpoolDialogId {
        AddSpoolDialog = 1,
        EditSpoolDialog,
    };

protected:
    // Filament information
    wxTextCtrl *displayName;
    wxTextCtrl *colorName;
    wxColourPickerCtrl *colorPicker;
    wxComboBox *vendor;
    wxComboBox *material;
    wxSpinCtrlDouble *density;
    wxSpinCtrlDouble *diameter;
    wxSpinCtrlDouble *tolerance;

    // Temperature and measurements
    wxSpinCtrlDouble *temperatureTool;
    wxSpinCtrlDouble *temperatureBed;
    wxSpinCtrlDouble *totalWeight;
    wxSpinCtrlDouble *usedWeight;
    wxSpinCtrlDouble *spoolWeight;
    wxSpinCtrlDouble *totalLength;
    wxSpinCtrlDouble *usedLength;

    // Purchase details
    wxTextCtrl *purchasedFrom;
    wxSpinCtrlDouble *cost;
    wxDatePickerCtrl *purchasedOn;

    virtual void handleSave(wxCommandEvent &event) = 0;

private:
    long databaseId = -1;
    long version = 1;
    wxDateTime lastUse;
    wxDateTime firstUse;

public:
    void setSpool(OctoprintSpool *spool);

    void setMaterials(const std::vector<wxString> &materials);

    void setVendors(const std::vector<wxString> &vendors);

    [[nodiscard]] OctoprintSpool getSpool() const;
};


#endif //WXOCTO_SPOOLDIALOGBASE_H
