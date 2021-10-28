//
// Created by imanuel on 07.10.21.
//

#include "MainWindowBase.h"

#include <iomanip>
#include "../MainApp.h"
#include "../helper.h"

MainWindowBase::MainWindowBase() : wxFrame(nullptr, wxID_ANY, _("wxOcto"), wxDefaultPosition, wxSize(1600, 900),
                                           wxDEFAULT_FRAME_STYLE | wxCLIP_CHILDREN),
                                   spoolListModel(new OctoprintSpoolDataViewListModel()) {
    SetIcon(wxICON(wxocto));
    SetMinClientSize(wxSize(1280, 600));
    auto panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(-1, -1));
    statusBar = CreateStatusBar();

    toolbar = CreateToolBar(wxTB_HORIZONTAL | wxTB_HORZ_LAYOUT | wxTB_NOICONS | wxTB_TEXT);
    toolbar->SetToolBitmapSize(wxSize(16, 16));

    toolbar->AddTool(MainWindowActions::PrinterSettings, _("Printer settings"), wxNullBitmap, wxNullBitmap,
                     wxITEM_NORMAL, "", "", nullptr);
    toolbar->AddSeparator();

    toolbar->AddTool(MainWindowActions::StartPrint, _("Start print"), wxNullBitmap, wxNullBitmap, wxITEM_NORMAL, "", "",
                     nullptr);
    toolbar->AddTool(MainWindowActions::ResumePrint, _("Resume print"), wxNullBitmap, wxNullBitmap, wxITEM_NORMAL, "",
                     "", nullptr);
    toolbar->AddTool(MainWindowActions::PausePrint, _("Pause print"), wxNullBitmap, wxNullBitmap, wxITEM_NORMAL, "", "",
                     nullptr);
    toolbar->AddTool(MainWindowActions::CancelPrint, _("Cancel print"), wxNullBitmap, wxNullBitmap, wxITEM_NORMAL, "",
                     "", nullptr);

    toolbar->AddSeparator();
    toolbar->AddTool(MainWindowActions::AddSpool, _("Add spool"), wxNullBitmap, wxNullBitmap, wxITEM_NORMAL, "", "",
                     nullptr);
    toolbar->AddTool(MainWindowActions::EditSpool, _("Edit spool"), wxNullBitmap, wxNullBitmap, wxITEM_NORMAL, "", "",
                     nullptr);
    toolbar->AddTool(MainWindowActions::DeleteSpool, _("Delete spool"), wxNullBitmap, wxNullBitmap, wxITEM_NORMAL, "",
                     "", nullptr);

    toolbar->Realize();

    auto menuBar = new wxMenuBar();
    this->SetMenuBar(menuBar);

    auto fileMenu = new wxMenu();
    fileMenu->Append(MainWindowActions::PrinterSettings, _("Printer settings"));
    fileMenu->AppendSeparator();
    fileMenu->Append(wxID_EXIT, _("Exit"));

    printingMenu = new wxMenu();
    printingMenu->Append(MainWindowActions::StartPrint, _("Start print"));
    printingMenu->Append(MainWindowActions::ResumePrint, _("Resume print"));
    printingMenu->Append(MainWindowActions::PausePrint, _("Pause print"));
    printingMenu->Append(MainWindowActions::CancelPrint, _("Cancel print"));

    auto spoolsMenu = new wxMenu();
    spoolsMenu->Append(MainWindowActions::AddSpool, _("Add spool"));
    spoolsMenu->Append(MainWindowActions::EditSpool, _("Edit spool"));
    spoolsMenu->Append(MainWindowActions::DeleteSpool, _("Delete spool"));

    menuBar->Append(fileMenu, "File");
    menuBar->Append(printingMenu, "Printing");
    menuBar->Append(spoolsMenu, "Spools");

    auto contentSizer = new wxFlexGridSizer(0, 2, 0, 0);
    contentSizer->SetFlexibleDirection(wxBOTH);
    contentSizer->AddGrowableRow(0);
    contentSizer->AddGrowableCol(1);

    panel->SetSizer(contentSizer);
    toolbar->EnableTool(MainWindowActions::DeleteSpool, false);
    toolbar->EnableTool(MainWindowActions::EditSpool, false);

    auto printStatusSizer = new wxStaticBoxSizer(new wxStaticBox(panel, wxID_ANY, _("Status")), wxVERTICAL);

    contentSizer->Add(printStatusSizer, 0, wxALL, WXC_FROM_DIP(5));

    auto printStatusContentSizer = new wxFlexGridSizer(0, 2, 0, 0);
    printStatusContentSizer->SetFlexibleDirection(wxBOTH);
    printStatusContentSizer->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_ALL);

    printStatusSizer->Add(printStatusContentSizer, 1, wxALL | wxEXPAND, WXC_FROM_DIP(5));

    auto printProgressLabel = new wxStaticText(panel, wxID_ANY, _("Print progress"), wxDefaultPosition,
                                               wxDLG_UNIT(this, wxSize(-1, -1)), 0);

    printStatusContentSizer->Add(printProgressLabel, 0, wxALL, WXC_FROM_DIP(5));

    prgPrintProgress = new wxGauge(panel, wxID_ANY, 100, wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)),
                                   wxGA_SMOOTH | wxGA_HORIZONTAL);
    prgPrintProgress->SetValue(0);

    printStatusContentSizer->Add(prgPrintProgress, 0, wxALL | wxALIGN_CENTER, WXC_FROM_DIP(5));
    prgPrintProgress->SetMinSize(wxSize(300, -1));

    auto timeElapsedLabel = new wxStaticText(panel, wxID_ANY, _("Time elapsed"), wxDefaultPosition,
                                             wxDLG_UNIT(this, wxSize(-1, -1)), 0);

    printStatusContentSizer->Add(timeElapsedLabel, 0, wxALL, WXC_FROM_DIP(5));

    lblTimeElapsed = new wxStaticText(panel, wxID_ANY, _("No print started"), wxDefaultPosition,
                                      wxDLG_UNIT(this, wxSize(-1, -1)), 0);

    printStatusContentSizer->Add(lblTimeElapsed, 0, wxALL, WXC_FROM_DIP(5));

    auto timeLeftLabel = new wxStaticText(panel, wxID_ANY, _("Time left"), wxDefaultPosition,
                                          wxDLG_UNIT(this, wxSize(-1, -1)), 0);

    printStatusContentSizer->Add(timeLeftLabel, 0, wxALL, WXC_FROM_DIP(5));

    lblTimeLeft = new wxStaticText(panel, wxID_ANY, _("No print started"), wxDefaultPosition,
                                   wxDLG_UNIT(this, wxSize(-1, -1)), 0);

    printStatusContentSizer->Add(lblTimeLeft, 0, wxALL, WXC_FROM_DIP(5));

    auto finishTimeLabel = new wxStaticText(panel, wxID_ANY, _("Finish time"), wxDefaultPosition,
                                            wxDLG_UNIT(this, wxSize(-1, -1)), 0);

    printStatusContentSizer->Add(finishTimeLabel, 0, wxALL, WXC_FROM_DIP(5));

    lblFinishTime = new wxStaticText(panel, wxID_ANY, _("No print started"), wxDefaultPosition,
                                     wxDLG_UNIT(this, wxSize(-1, -1)), 0);

    printStatusContentSizer->Add(lblFinishTime, 0, wxALL, WXC_FROM_DIP(5));

    auto fileLabel = new wxStaticText(panel, wxID_ANY, _("File"), wxDefaultPosition,
                                      wxDLG_UNIT(this, wxSize(-1, -1)),
                                      0);

    printStatusContentSizer->Add(fileLabel, 0, wxALL, WXC_FROM_DIP(5));

    lblFile = new wxStaticText(panel, wxID_ANY, _("No print started"), wxDefaultPosition,
                               wxDLG_UNIT(this, wxSize(100, -1)), wxST_ELLIPSIZE_END | wxST_NO_AUTORESIZE);

    printStatusContentSizer->Add(lblFile, 0, wxALL, WXC_FROM_DIP(5));

    auto selectedSpoolLabel = new wxStaticText(panel, wxID_ANY, _("Selected spool"), wxDefaultPosition,
                                               wxDLG_UNIT(this, wxSize(-1, -1)),
                                               0);

    printStatusContentSizer->Add(selectedSpoolLabel, 0, wxALL, WXC_FROM_DIP(5));

    selectedSpoolChoice = new wxChoice(panel, wxID_ANY, wxDefaultPosition, wxSize(-1, -1));

    printStatusContentSizer->Add(selectedSpoolChoice, 1, wxALL | wxEXPAND, WXC_FROM_DIP(5));

    nbContent = new wxNotebook(panel, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), wxBK_DEFAULT);
    nbContent->SetName(wxT("nbContent"));

    contentSizer->Add(nbContent, 1, wxALL | wxEXPAND | wxALIGN_TOP, WXC_FROM_DIP(5));

    nbpFiles = new wxPanel(nbContent, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(nbContent, wxSize(-1, -1)),
                           wxTAB_TRAVERSAL);
    nbContent->AddPage(nbpFiles, _("Files"), true);

    auto tlcFilesSizer = new wxBoxSizer(wxVERTICAL);
    nbpFiles->SetSizer(tlcFilesSizer);

    tlcFiles = new wxTreeListCtrl(nbpFiles, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(nbpFiles, wxSize(-1, -1)),
                                  wxTL_DEFAULT_STYLE);

    tlcFilesSizer->Add(tlcFiles, 1, wxALL | wxEXPAND, WXC_FROM_DIP(5));

    tlcFiles->AppendColumn(_("Name"), -2, wxALIGN_LEFT, wxCOL_RESIZABLE | wxCOL_SORTABLE | wxCOL_REORDERABLE);
    tlcFiles->AppendColumn(_("Size"), -2, wxALIGN_LEFT, wxCOL_RESIZABLE | wxCOL_SORTABLE | wxCOL_REORDERABLE);
    tlcFiles->AppendColumn(_("Uploaded"), -2, wxALIGN_LEFT, wxCOL_RESIZABLE | wxCOL_SORTABLE | wxCOL_REORDERABLE);
    tlcFiles->AppendColumn(_("Model size"), -2, wxALIGN_LEFT, wxCOL_RESIZABLE | wxCOL_REORDERABLE);
    tlcFiles->AppendColumn(_("Filament use"), -2, wxALIGN_LEFT, wxCOL_RESIZABLE | wxCOL_SORTABLE | wxCOL_REORDERABLE);
    tlcFiles->AppendColumn(_("Estimated print time"), -2, wxALIGN_LEFT,
                           wxCOL_RESIZABLE | wxCOL_SORTABLE | wxCOL_REORDERABLE);
    tlcFiles->AppendColumn(_("Full path"), -2, wxALIGN_LEFT, wxCOL_RESIZABLE | wxCOL_REORDERABLE);
    tlcFiles->SetMinSize(wxSize(-1, -1));
    tlcFiles->SetItemComparator(new OctoprintFileTreeListItemComparator());

    nbpSpools = new wxPanel(nbContent, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(nbContent, wxSize(-1, -1)),
                            wxTAB_TRAVERSAL);
    nbContent->AddPage(nbpSpools, _("Spools"), false);

    auto dvlSpoolsSizer = new wxBoxSizer(wxVERTICAL);
    nbpSpools->SetSizer(dvlSpoolsSizer);

    dvlSpools = new wxDataViewListCtrl(nbpSpools, Spools, wxDefaultPosition, wxDLG_UNIT(nbpSpools, wxSize(-1, -1)),
                                       wxDV_ROW_LINES | wxDV_SINGLE);

    dvlSpoolsSizer->Add(dvlSpools, 1, wxALL | wxEXPAND, WXC_FROM_DIP(5));

    dvlSpools->AppendTextColumn(_("Name"), wxDATAVIEW_CELL_INERT, WXC_FROM_DIP(-2), wxALIGN_LEFT,
                                wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);
    dvlSpools->AppendTextColumn(_("Material"), wxDATAVIEW_CELL_INERT, WXC_FROM_DIP(-2), wxALIGN_LEFT,
                                wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);
    dvlSpools->AppendTextColumn(_("Last used"), wxDATAVIEW_CELL_INERT, WXC_FROM_DIP(-2), wxALIGN_LEFT,
                                wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);
    dvlSpools->AppendTextColumn(_("Weight total (g)"), wxDATAVIEW_CELL_INERT, WXC_FROM_DIP(-2), wxALIGN_LEFT,
                                wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);
    dvlSpools->AppendTextColumn(_("Weight used (g)"), wxDATAVIEW_CELL_INERT, WXC_FROM_DIP(-2), wxALIGN_LEFT,
                                wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);
    dvlSpools->AppendTextColumn(_("Weight left (g)"), wxDATAVIEW_CELL_INERT, WXC_FROM_DIP(-2), wxALIGN_LEFT,
                                wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);
    dvlSpools->AppendTextColumn(_("Length total (mm)"), wxDATAVIEW_CELL_INERT, WXC_FROM_DIP(-2), wxALIGN_LEFT,
                                wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);
    dvlSpools->AppendTextColumn(_("Length used (mm)"), wxDATAVIEW_CELL_INERT, WXC_FROM_DIP(-2), wxALIGN_LEFT,
                                wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);
    dvlSpools->AppendTextColumn(_("Length left (mm)"), wxDATAVIEW_CELL_INERT, WXC_FROM_DIP(-2), wxALIGN_LEFT,
                                wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);
    nbContent->SetMinSize(wxSize(600, -1));

    if (!wxPersistenceManager::Get().Find(nbContent)) {
        wxPersistenceManager::Get().RegisterAndRestore(nbContent);
    } else {
        wxPersistenceManager::Get().Restore(nbContent);
    }

    SetName("MainWindowBase");
    SetSize(wxDLG_UNIT(this, wxSize(-1, -1)));
    if (GetSizer()) {
        GetSizer()->Fit(panel);
    }
    if (GetParent()) {
        CentreOnParent(wxBOTH);
    } else {
        CentreOnScreen(wxBOTH);
    }

    if (!wxPersistenceManager::Get().Find(this)) {
        wxPersistenceManager::Get().RegisterAndRestore(this);
    } else {
        wxPersistenceManager::Get().Restore(this);
    }

    setupEvents();

    // Default event handler
    Bind(wxEVT_MENU, &MainWindowBase::handlePrinterSettings, this, PrinterSettings);
    Bind(wxEVT_MENU, &MainWindowBase::handleStartPrint, this, StartPrint);
    Bind(wxEVT_MENU, &MainWindowBase::handleResumePrint, this, ResumePrint);
    Bind(wxEVT_MENU, &MainWindowBase::handlePausePrint, this, PausePrint);
    Bind(wxEVT_MENU, &MainWindowBase::handleCancelPrint, this, CancelPrint);
    Bind(wxEVT_MENU, &MainWindowBase::handleAddSpool, this, AddSpool);
    Bind(wxEVT_MENU, &MainWindowBase::handleEditSpool, this, EditSpool);
    Bind(wxEVT_MENU, &MainWindowBase::handleDeleteSpool, this, DeleteSpool);
    Bind(wxEVT_MENU, &MainWindowBase::handleExit, this, wxID_EXIT);
    Bind(wxEVT_DATAVIEW_SELECTION_CHANGED, &MainWindowBase::handleDvlSpoolsSelectionChanged, this, Spools);
}

MainWindowBase::~MainWindowBase() = default;

void MainWindowBase::handleExit(wxCommandEvent &event) {
    MainApp::getInstance()->DismissPreferencesEditor();
    Close(true);
}

int OctoprintFileTreeListItemComparator::Compare(wxTreeListCtrl *treelist, unsigned int column, wxTreeListItem first,
                                                 wxTreeListItem second) {
    auto firstItem = dynamic_cast<OctoprintFileClientData *>(treelist->GetItemData(first));
    auto secondItem = dynamic_cast<OctoprintFileClientData *>(treelist->GetItemData(second));

    switch (column) {
        case FileListColumns::ColFileName:
            return firstItem->file.name.compare(secondItem->file.name);
        case FileListColumns::ColFileSize:
            return (int) (firstItem->file.size - secondItem->file.size);
        case FileListColumns::ColFileUploaded:
            return (int) (firstItem->file.uploaded - secondItem->file.uploaded);
        case FileListColumns::ColFileFilamentUse:
            return (int) (firstItem->file.filamentLength - secondItem->file.filamentLength);
        case FileListColumns::ColFileEstimatedPrintTime:
            return (int) (firstItem->file.estimatedPrintTime - secondItem->file.estimatedPrintTime);
        default:
            return 0;
    }
}

unsigned int OctoprintSpoolDataViewListModel::GetColumnCount() const {
    return SpoolColumnCount;
}

wxString OctoprintSpoolDataViewListModel::GetColumnType(unsigned int col) const {
    return "string";
}

void OctoprintSpoolDataViewListModel::Fill(const std::vector<OctoprintSpool *> &data) {
    items.clear();

    auto parent = wxDataViewItem(nullptr);
    for (auto item: data) {
        items.push_back(item);
        ItemAdded(parent, wxDataViewItem(item));
    }
}

OctoprintSpoolDataViewListModel::OctoprintSpoolDataViewListModel() : wxDataViewModel(),
                                                                     items(std::vector<OctoprintSpool *>()) {}

unsigned int
OctoprintSpoolDataViewListModel::GetChildren(const wxDataViewItem &item, wxDataViewItemArray &children) const {
    for (auto elem: items) {
        children.Add(wxDataViewItem(elem));
    }

    return items.size();
}

void OctoprintSpoolDataViewListModel::GetValue(wxVariant &variant, const wxDataViewItem &item, unsigned int col) const {
    auto spool = (OctoprintSpool *) item.GetID();
    switch (col) {
        case ColSpoolName:
            variant = spool->displayName;
            break;
        case ColSpoolMaterial:
            variant = spool->material;
            break;
        case ColSpoolLastUsed:
            variant = spool->getLastUsed();
            break;
        case ColSpoolWeightTotal:
            variant = spool->totalWeight;
            break;
        case ColSpoolWeightUsed:
            variant = spool->usedWeight;
            break;
        case ColSpoolWeightLeft:
            variant = spool->leftWeight;
            break;
        case ColSpoolLengthTotal:
            variant = spool->getTotalLength();
            break;
        case ColSpoolLengthUsed:
            variant = spool->getUsedLength();
            break;
        case ColSpoolLengthLeft:
            variant = spool->getLeftLength();
            break;
        default:
            wxFAIL;
            break;
    }
}

bool OctoprintSpoolDataViewListModel::SetValue(const wxVariant &variant, const wxDataViewItem &item, unsigned int col) {
    auto spool = (OctoprintSpool *) item.GetID();
    switch (col) {
        case ColSpoolName:
            spool->displayName = variant.GetString();
            break;
        case ColSpoolMaterial:
            spool->material = variant.GetString();
            break;
        case ColSpoolLastUsed:
            spool->lastUsed.ParseDate(variant);
            break;
        case ColSpoolWeightTotal:
            spool->totalWeight = variant.GetString();
            break;
        case ColSpoolWeightUsed:
            spool->usedWeight = variant.GetString();
            break;
        case ColSpoolWeightLeft:
            spool->leftWeight = variant.GetString();
            break;
        case ColSpoolLengthTotal:
            spool->totalLength = variant;
            break;
        case ColSpoolLengthUsed:
            spool->usedLength = variant;
            break;
        case ColSpoolLengthLeft:
            spool->leftLength = variant;
            break;
        default:
            wxFAIL;
            break;
    }

    return true;
}

wxDataViewItem OctoprintSpoolDataViewListModel::GetParent(const wxDataViewItem &item) const {
    return wxDataViewItem(nullptr);
}

bool OctoprintSpoolDataViewListModel::IsContainer(const wxDataViewItem &item) const {
    if (!item.IsOk())
        return true;

    return false;
}
