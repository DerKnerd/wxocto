//
// Created by imanuel on 16.10.21.
//

#include "SpoolDialogBase.h"

SpoolDialogBase::SpoolDialogBase(wxWindow *parent, const wxString &title)
        : wxDialog(parent, wxID_ANY, title, wxDefaultPosition, wxSize(-1, -1), wxDEFAULT_DIALOG_STYLE,
                   wxASCII_STR(wxDialogNameStr)) {
    auto dialogSizer = new wxBoxSizer(wxVERTICAL);
    auto staticBoxesSizer = new wxBoxSizer(wxHORIZONTAL);
    auto filamentInformationSizer = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, _("Filament information")),
                                                         wxVERTICAL);
    auto filamentInformationContentSizer = new wxFlexGridSizer(2);
    auto displayNameLabel = new wxStaticText(this, wxID_ANY, _("Display name"));
    auto colorNameLabel = new wxStaticText(this, wxID_ANY, _("Color name"));
    auto colorPickerLabel = new wxStaticText(this, wxID_ANY, _("Color"));
    auto vendorLabel = new wxStaticText(this, wxID_ANY, _("Vendor"));
    auto materialLabel = new wxStaticText(this, wxID_ANY, _("Material"));
    auto densityLabel = new wxStaticText(this, wxID_ANY, _(L"Density (g/m³)"));
    auto diameterLabel = new wxStaticText(this, wxID_ANY, _("Diameter (mm)"));
    auto toleranceLabel = new wxStaticText(this, wxID_ANY, _("Tolerance (mm)"));

    displayName = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxSize(200, -1));
    colorName = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxSize(200, -1));
    colorPicker = new wxColourPickerCtrl(this, wxID_ANY, "", wxDefaultPosition, wxSize(200, -1));
    vendor = new wxComboBox(this, wxID_ANY, "", wxDefaultPosition, wxSize(200, -1));
    material = new wxComboBox(this, wxID_ANY, "", wxDefaultPosition, wxSize(200, -1));
    density = new wxSpinCtrlDouble(this, wxID_ANY, "", wxDefaultPosition, wxSize(200, -1));
    diameter = new wxSpinCtrlDouble(this, wxID_ANY, "", wxDefaultPosition, wxSize(200, -1));
    tolerance = new wxSpinCtrlDouble(this, wxID_ANY, "", wxDefaultPosition, wxSize(200, -1));

    density->SetDigits(2);
    diameter->SetDigits(2);
    tolerance->SetDigits(2);
    density->SetRange(0, 500000000);
    diameter->SetRange(0, 500000000);
    tolerance->SetRange(0, 500000000);

    filamentInformationContentSizer->Add(displayNameLabel, 0, wxALL, FromDIP(5));
    filamentInformationContentSizer->Add(displayName, 0, wxALL, FromDIP(5));

    filamentInformationContentSizer->Add(colorNameLabel, 0, wxALL, FromDIP(5));
    filamentInformationContentSizer->Add(colorName, 0, wxALL, FromDIP(5));

    filamentInformationContentSizer->Add(colorPickerLabel, 0, wxALL, FromDIP(5));
    filamentInformationContentSizer->Add(colorPicker, 0, wxALL, FromDIP(5));

    filamentInformationContentSizer->Add(vendorLabel, 0, wxALL, FromDIP(5));
    filamentInformationContentSizer->Add(vendor, 0, wxALL, FromDIP(5));

    filamentInformationContentSizer->Add(materialLabel, 0, wxALL, FromDIP(5));
    filamentInformationContentSizer->Add(material, 0, wxALL, FromDIP(5));

    filamentInformationContentSizer->Add(densityLabel, 0, wxALL, FromDIP(5));
    filamentInformationContentSizer->Add(density, 0, wxALL, FromDIP(5));

    filamentInformationContentSizer->Add(diameterLabel, 0, wxALL, FromDIP(5));
    filamentInformationContentSizer->Add(diameter, 0, wxALL, FromDIP(5));

    filamentInformationContentSizer->Add(toleranceLabel, 0, wxALL, FromDIP(5));
    filamentInformationContentSizer->Add(tolerance, 0, wxALL, FromDIP(5));

    filamentInformationSizer->Add(filamentInformationContentSizer, 0, wxALL, FromDIP(5));

    auto weightAndLengthInformationSizer = new wxStaticBoxSizer(
            new wxStaticBox(this, wxID_ANY, _("Weight and length information")), wxVERTICAL);
    auto weightAndLengthInformationContentSizer = new wxFlexGridSizer(2);
    auto temperatureToolLabel = new wxStaticText(this, wxID_ANY, _(L"Tool temperature (°C)"));
    auto temperatureBedLabel = new wxStaticText(this, wxID_ANY, _(L"Bed temperature (°C)"));
    auto totalWeightLabel = new wxStaticText(this, wxID_ANY, _("Total weight (g)"));
    auto usedWeightLabel = new wxStaticText(this, wxID_ANY, _("Used weight (g)"));
    auto spoolWeightLabel = new wxStaticText(this, wxID_ANY, _("Spool weight (g)"));
    auto totalLengthLabel = new wxStaticText(this, wxID_ANY, _("Total length (mm)"));
    auto usedLengthLabel = new wxStaticText(this, wxID_ANY, _("Used length (mm)"));

    temperatureTool = new wxSpinCtrlDouble(this, wxID_ANY, "", wxDefaultPosition, wxSize(200, -1));
    temperatureBed = new wxSpinCtrlDouble(this, wxID_ANY, "", wxDefaultPosition, wxSize(200, -1));
    totalWeight = new wxSpinCtrlDouble(this, wxID_ANY, "", wxDefaultPosition, wxSize(200, -1));
    usedWeight = new wxSpinCtrlDouble(this, wxID_ANY, "", wxDefaultPosition, wxSize(200, -1));
    spoolWeight = new wxSpinCtrlDouble(this, wxID_ANY, "", wxDefaultPosition, wxSize(200, -1));
    totalLength = new wxSpinCtrlDouble(this, wxID_ANY, "", wxDefaultPosition, wxSize(200, -1));
    usedLength = new wxSpinCtrlDouble(this, wxID_ANY, "", wxDefaultPosition, wxSize(200, -1));

    temperatureTool->SetDigits(2);
    temperatureBed->SetDigits(2);
    totalWeight->SetDigits(2);
    usedWeight->SetDigits(2);
    spoolWeight->SetDigits(2);
    totalLength->SetDigits(2);
    usedLength->SetDigits(2);

    temperatureTool->SetRange(0, 500000000);
    temperatureBed->SetRange(0, 500000000);
    totalWeight->SetRange(0, 500000000);
    usedWeight->SetRange(0, 500000000);
    spoolWeight->SetRange(0, 500000000);
    totalLength->SetRange(0, 500000000);
    usedLength->SetRange(0, 500000000);

    weightAndLengthInformationContentSizer->Add(temperatureToolLabel, 0, wxALL, FromDIP(5));
    weightAndLengthInformationContentSizer->Add(temperatureTool, 0, wxALL, FromDIP(5));

    weightAndLengthInformationContentSizer->Add(temperatureBedLabel, 0, wxALL, FromDIP(5));
    weightAndLengthInformationContentSizer->Add(temperatureBed, 0, wxALL, FromDIP(5));

    weightAndLengthInformationContentSizer->Add(totalWeightLabel, 0, wxALL, FromDIP(5));
    weightAndLengthInformationContentSizer->Add(totalWeight, 0, wxALL, FromDIP(5));

    weightAndLengthInformationContentSizer->Add(usedWeightLabel, 0, wxALL, FromDIP(5));
    weightAndLengthInformationContentSizer->Add(usedWeight, 0, wxALL, FromDIP(5));

    weightAndLengthInformationContentSizer->Add(spoolWeightLabel, 0, wxALL, FromDIP(5));
    weightAndLengthInformationContentSizer->Add(spoolWeight, 0, wxALL, FromDIP(5));

    weightAndLengthInformationContentSizer->Add(totalLengthLabel, 0, wxALL, FromDIP(5));
    weightAndLengthInformationContentSizer->Add(totalLength, 0, wxALL, FromDIP(5));

    weightAndLengthInformationContentSizer->Add(usedLengthLabel, 0, wxALL, FromDIP(5));
    weightAndLengthInformationContentSizer->Add(usedLength, 0, wxALL, FromDIP(5));

    weightAndLengthInformationSizer->Add(weightAndLengthInformationContentSizer, 0, wxALL, FromDIP(5));

    auto purchaseInformationSizer = new wxStaticBoxSizer(
            new wxStaticBox(this, wxID_ANY, _("Purchase information")), wxVERTICAL);
    auto purchaseInformationContentSizer = new wxFlexGridSizer(2);
    auto purchasedFromLabel = new wxStaticText(this, wxID_ANY, _("Purchased from"));
    auto costLabel = new wxStaticText(this, wxID_ANY, _("Cost"));
    auto purchasedOnLabel = new wxStaticText(this, wxID_ANY, _("Purchased on"));

    purchasedFrom = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxSize(200, -1));
    cost = new wxSpinCtrlDouble(this, wxID_ANY, "", wxDefaultPosition, wxSize(200, -1));
    purchasedOn = new wxDatePickerCtrl(this, wxID_ANY, wxDateTime::Now(), wxDefaultPosition, wxSize(200, -1));

    cost->SetDigits(2);
    cost->SetRange(0, 500000000);

    purchaseInformationContentSizer->Add(purchasedFromLabel, 0, wxALL, FromDIP(5));
    purchaseInformationContentSizer->Add(purchasedFrom, 0, wxALL, FromDIP(5));

    purchaseInformationContentSizer->Add(costLabel, 0, wxALL, FromDIP(5));
    purchaseInformationContentSizer->Add(cost, 0, wxALL, FromDIP(5));

    purchaseInformationContentSizer->Add(purchasedOnLabel, 0, wxALL, FromDIP(5));
    purchaseInformationContentSizer->Add(purchasedOn, 0, wxALL, FromDIP(5));

    purchaseInformationSizer->Add(purchaseInformationContentSizer, 0, wxALL, FromDIP(5));

    staticBoxesSizer->Add(filamentInformationSizer, 0, wxALL, FromDIP(5));
    staticBoxesSizer->Add(weightAndLengthInformationSizer, 0, wxALL, FromDIP(5));
    staticBoxesSizer->Add(purchaseInformationSizer, 0, wxALL, FromDIP(5));

    dialogSizer->Add(staticBoxesSizer);

    auto dialogButtonSizer = CreateStdDialogButtonSizer(wxAPPLY | wxCLOSE);
    this->SetSizerAndFit(dialogSizer);
    GetSizer()->Add(dialogButtonSizer, 0, wxGROW | wxALL, 5);
    auto applyButton = dialogButtonSizer->GetApplyButton();
    applyButton->SetLabel(_("Save spool"));

    Bind(wxEVT_BUTTON, &SpoolDialogBase::handleSave, this, wxID_APPLY);
}

void SpoolDialogBase::setSpool(OctoprintSpool *spool) {
    displayName->SetValue(spool->displayName);
    colorName->SetValue(spool->colorName);
    colorPicker->SetColour(spool->color);
    vendor->SetValue(spool->vendor);
    material->SetValue(spool->material);
    density->SetValue(spool->density);
    diameter->SetValue(spool->diameter);
    tolerance->SetValue(spool->tolerance);

    temperatureTool->SetValue(spool->temperatureTool);
    temperatureBed->SetValue(spool->temperatureBed);
    totalWeight->SetValue(spool->totalWeight);
    usedWeight->SetValue(spool->usedWeight);
    spoolWeight->SetValue(spool->spoolWeight);
    totalLength->SetValue(spool->totalLength);
    usedLength->SetValue(spool->usedLength);

    purchasedFrom->SetValue(spool->purchasedFrom);
    cost->SetValue(spool->cost);
    purchasedOn->SetValue(spool->purchasedOn);

    databaseId = spool->databaseId;
}

OctoprintSpool SpoolDialogBase::getSpool() const {
    auto spool = OctoprintSpool();
    spool.displayName = displayName->GetValue();
    spool.colorName = colorName->GetValue();
    spool.color = colorPicker->GetColour().GetAsString();
    spool.vendor = vendor->GetValue();
    spool.material = material->GetValue();
    spool.density = density->GetValue();
    spool.diameter = diameter->GetValue();
    spool.tolerance = tolerance->GetValue();
    spool.temperatureTool = temperatureTool->GetValue();
    spool.temperatureBed = temperatureBed->GetValue();
    spool.totalWeight = std::to_string(totalWeight->GetValue());
    spool.usedWeight = std::to_string(usedWeight->GetValue());
    spool.spoolWeight = std::to_string(spoolWeight->GetValue());
    spool.totalLength = totalLength->GetValue();
    spool.usedLength = usedLength->GetValue();
    spool.purchasedFrom = purchasedFrom->GetValue();
    spool.cost = cost->GetValue();
    spool.purchasedOn = purchasedOn->GetValue();
    spool.databaseId = databaseId;

    return spool;
}