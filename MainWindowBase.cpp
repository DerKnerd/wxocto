//
// Created by imanuel on 07.10.21.
//

#include "MainWindowBase.h"

MainWindowBase::MainWindowBase(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &pos,
                               const wxSize &size, long style)
        : wxFrame(parent, id, title, pos, size, style) {
    auto flexGridSizer3 = new wxFlexGridSizer(2, 0, 0, 0);
    flexGridSizer3->SetFlexibleDirection(wxBOTH);
    flexGridSizer3->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);
    flexGridSizer3->AddGrowableRow(1);
    flexGridSizer3->AddGrowableCol(0);
    this->SetSizer(flexGridSizer3);

    auto flexGridSizer98 = new wxFlexGridSizer(0, 2, 0, 0);
    flexGridSizer98->SetFlexibleDirection(wxBOTH);
    flexGridSizer98->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

    flexGridSizer3->Add(flexGridSizer98, 1, wxALL | wxEXPAND, WXC_FROM_DIP(5));

    toolbar = CreateToolBar(wxTB_HORIZONTAL | wxTB_HORZ_LAYOUT | wxTB_NOICONS);
    toolbar->SetToolBitmapSize(wxSize(16, 16));

    toolbar->AddTool(ToolbarItems::StartPrint, _("Start print"), wxNullBitmap, wxNullBitmap, wxITEM_NORMAL, wxT(""),
                     wxT(""),
                     nullptr);

    toolbar->AddTool(ToolbarItems::ResumePrint, _("Resume print"), wxNullBitmap, wxNullBitmap, wxITEM_NORMAL, wxT(""),
                     wxT(""),
                     nullptr);

    toolbar->AddTool(ToolbarItems::PausePrint, _("Pause print"), wxNullBitmap, wxNullBitmap, wxITEM_NORMAL, wxT(""),
                     wxT(""),
                     nullptr);

    toolbar->AddTool(ToolbarItems::CancelPrint, _("Cancel print"), wxNullBitmap, wxNullBitmap, wxITEM_NORMAL, wxT(""),
                     wxT(""),
                     nullptr);

    toolbar->AddSeparator();
    toolbar->AddTool(ToolbarItems::AddSpool, _("Add spool"), wxNullBitmap, wxNullBitmap, wxITEM_NORMAL, wxT(""),
                     wxT(""), nullptr);

    toolbar->AddTool(ToolbarItems::EditSpool, _("Edit spool"), wxNullBitmap, wxNullBitmap, wxITEM_NORMAL, wxT(""),
                     wxT(""),
                     nullptr);

    toolbar->AddTool(ToolbarItems::DeleteSpool, _("Delete spool"), wxNullBitmap, wxNullBitmap, wxITEM_NORMAL, wxT(""),
                     wxT(""),
                     nullptr);
    toolbar->Realize();

    auto flexGridSizer94 = new wxFlexGridSizer(0, 2, 0, 0);
    flexGridSizer94->SetFlexibleDirection(wxBOTH);
    flexGridSizer94->AddGrowableRow(0);
    flexGridSizer94->AddGrowableCol(1);

    flexGridSizer3->Add(flexGridSizer94, 1, wxALL | wxEXPAND, WXC_FROM_DIP(5));

    auto staticBoxSizer5 = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, _("Status")), wxVERTICAL);

    flexGridSizer94->Add(staticBoxSizer5, 1, wxALL | wxEXPAND | wxALIGN_TOP, WXC_FROM_DIP(5));

    auto flexGridSizer7 = new wxFlexGridSizer(0, 2, 0, 0);
    flexGridSizer7->SetFlexibleDirection(wxBOTH);
    flexGridSizer7->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

    staticBoxSizer5->Add(flexGridSizer7, 1, wxALL | wxEXPAND, WXC_FROM_DIP(5));

    m_staticText9 = new wxStaticText(this, wxID_ANY, _("Print progress"), wxDefaultPosition,
                                     wxDLG_UNIT(this, wxSize(-1, -1)), 0);

    flexGridSizer7->Add(m_staticText9, 0, wxALL, WXC_FROM_DIP(5));

    prgPrintProgress = new wxGauge(this, wxID_ANY, 100, wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)),
                                   wxGA_SMOOTH | wxGA_HORIZONTAL);
    prgPrintProgress->SetValue(0);

    flexGridSizer7->Add(prgPrintProgress, 0, wxALL | wxALIGN_CENTER, WXC_FROM_DIP(5));
    prgPrintProgress->SetMinSize(wxSize(200, -1));

    m_staticText17 = new wxStaticText(this, wxID_ANY, _("Time elapsed"), wxDefaultPosition,
                                      wxDLG_UNIT(this, wxSize(-1, -1)), 0);

    flexGridSizer7->Add(m_staticText17, 0, wxALL, WXC_FROM_DIP(5));

    lblTimeElapsed = new wxStaticText(this, wxID_ANY, _("No print started"), wxDefaultPosition,
                                      wxDLG_UNIT(this, wxSize(-1, -1)), 0);

    flexGridSizer7->Add(lblTimeElapsed, 0, wxALL, WXC_FROM_DIP(5));

    m_staticText21 = new wxStaticText(this, wxID_ANY, _("Time left"), wxDefaultPosition,
                                      wxDLG_UNIT(this, wxSize(-1, -1)), 0);

    flexGridSizer7->Add(m_staticText21, 0, wxALL, WXC_FROM_DIP(5));

    lblTimeLeft = new wxStaticText(this, wxID_ANY, _("No print started"), wxDefaultPosition,
                                   wxDLG_UNIT(this, wxSize(-1, -1)), 0);

    flexGridSizer7->Add(lblTimeLeft, 0, wxALL, WXC_FROM_DIP(5));

    m_staticText27 = new wxStaticText(this, wxID_ANY, _("Finish time"), wxDefaultPosition,
                                      wxDLG_UNIT(this, wxSize(-1, -1)), 0);

    flexGridSizer7->Add(m_staticText27, 0, wxALL, WXC_FROM_DIP(5));

    lblFinishTime = new wxStaticText(this, wxID_ANY, _("No print started"), wxDefaultPosition,
                                     wxDLG_UNIT(this, wxSize(-1, -1)), 0);

    flexGridSizer7->Add(lblFinishTime, 0, wxALL, WXC_FROM_DIP(5));

    m_staticText35 = new wxStaticText(this, wxID_ANY, _("File"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)),
                                      0);

    flexGridSizer7->Add(m_staticText35, 0, wxALL, WXC_FROM_DIP(5));

    lblFile = new wxStaticText(this, wxID_ANY, _("No print started"), wxDefaultPosition,
                               wxDLG_UNIT(this, wxSize(-1, -1)), 0);

    flexGridSizer7->Add(lblFile, 0, wxALL, WXC_FROM_DIP(5));

    nbContent = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), wxBK_DEFAULT);
    nbContent->SetName(wxT("nbContent"));

    flexGridSizer94->Add(nbContent, 1, wxALL | wxEXPAND | wxALIGN_TOP, WXC_FROM_DIP(5));

    nbpFiles = new wxPanel(nbContent, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(nbContent, wxSize(-1, -1)),
                           wxTAB_TRAVERSAL);
    nbContent->AddPage(nbpFiles, _("Files"), true);

    auto boxSizer62 = new wxBoxSizer(wxVERTICAL);
    nbpFiles->SetSizer(boxSizer62);

    tlcFiles = new wxTreeListCtrl(nbpFiles, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(nbpFiles, wxSize(-1, -1)),
                                  wxTL_DEFAULT_STYLE);

    boxSizer62->Add(tlcFiles, 1, wxALL | wxEXPAND, WXC_FROM_DIP(5));

    tlcFiles->AppendColumn(_("Name"), -2, wxALIGN_LEFT, wxCOL_RESIZABLE | wxCOL_SORTABLE | wxCOL_REORDERABLE);
    tlcFiles->AppendColumn(_("Size"), -2, wxALIGN_LEFT, wxCOL_RESIZABLE | wxCOL_SORTABLE | wxCOL_REORDERABLE);
    tlcFiles->AppendColumn(_("Uploaded"), -2, wxALIGN_LEFT, wxCOL_RESIZABLE | wxCOL_SORTABLE | wxCOL_REORDERABLE);
    tlcFiles->AppendColumn(_("Model size"), -2, wxALIGN_LEFT, wxCOL_RESIZABLE | wxCOL_SORTABLE | wxCOL_REORDERABLE);
    tlcFiles->AppendColumn(_("Filament use"), -2, wxALIGN_LEFT, wxCOL_RESIZABLE | wxCOL_SORTABLE | wxCOL_REORDERABLE);
    tlcFiles->AppendColumn(_("Estimated print time"), -2, wxALIGN_LEFT,
                           wxCOL_RESIZABLE | wxCOL_SORTABLE | wxCOL_REORDERABLE);
    tlcFiles->SetMinSize(wxSize(-1, -1));

    nbpSpools = new wxPanel(nbContent, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(nbContent, wxSize(-1, -1)),
                            wxTAB_TRAVERSAL);
    nbContent->AddPage(nbpSpools, _("Spools"), false);

    auto boxSizer76 = new wxBoxSizer(wxVERTICAL);
    nbpSpools->SetSizer(boxSizer76);

    dvlSpools = new wxDataViewListCtrl(nbpSpools, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(nbpSpools, wxSize(-1, -1)),
                                       wxDV_ROW_LINES | wxDV_SINGLE);

    boxSizer76->Add(dvlSpools, 1, wxALL | wxEXPAND, WXC_FROM_DIP(5));

    dvlSpools->AppendTextColumn(_("Spoolname"), wxDATAVIEW_CELL_INERT, WXC_FROM_DIP(-2), wxALIGN_LEFT,
                                wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_REORDERABLE);
    dvlSpools->AppendTextColumn(_("Material"), wxDATAVIEW_CELL_INERT, WXC_FROM_DIP(-2), wxALIGN_LEFT,
                                wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_REORDERABLE);
    dvlSpools->AppendTextColumn(_("Last used"), wxDATAVIEW_CELL_INERT, WXC_FROM_DIP(-2), wxALIGN_LEFT,
                                wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_REORDERABLE);
    dvlSpools->AppendTextColumn(_("Weight total"), wxDATAVIEW_CELL_INERT, WXC_FROM_DIP(-2), wxALIGN_LEFT,
                                wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_REORDERABLE);
    dvlSpools->AppendTextColumn(_("Weight used"), wxDATAVIEW_CELL_INERT, WXC_FROM_DIP(-2), wxALIGN_LEFT,
                                wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_REORDERABLE);
    dvlSpools->AppendTextColumn(_("Weight left"), wxDATAVIEW_CELL_INERT, WXC_FROM_DIP(-2), wxALIGN_LEFT,
                                wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_REORDERABLE);
    dvlSpools->AppendTextColumn(_("Length total"), wxDATAVIEW_CELL_INERT, WXC_FROM_DIP(-2), wxALIGN_LEFT,
                                wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_REORDERABLE);
    dvlSpools->AppendTextColumn(_("Length used"), wxDATAVIEW_CELL_INERT, WXC_FROM_DIP(-2), wxALIGN_LEFT,
                                wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_REORDERABLE);
    dvlSpools->AppendTextColumn(_("Length left"), wxDATAVIEW_CELL_INERT, WXC_FROM_DIP(-2), wxALIGN_LEFT,
                                wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_REORDERABLE);
    nbContent->SetMinSize(wxSize(600, -1));


#if wxVERSION_NUMBER >= 2900
    if (!wxPersistenceManager::Get().Find(nbContent)) {
        wxPersistenceManager::Get().RegisterAndRestore(nbContent);
    } else {
        wxPersistenceManager::Get().Restore(nbContent);
    }
#endif

    SetName(wxT("MainWindowBase"));
    SetSize(wxDLG_UNIT(this, wxSize(-1, -1)));
    if (GetSizer()) {
        GetSizer()->Fit(this);
    }
    if (GetParent()) {
        CentreOnParent(wxBOTH);
    } else {
        CentreOnScreen(wxBOTH);
    }
#if wxVERSION_NUMBER >= 2900
    if (!wxPersistenceManager::Get().Find(this)) {
        wxPersistenceManager::Get().RegisterAndRestore(this);
    } else {
        wxPersistenceManager::Get().Restore(this);
    }
#endif
}

MainWindowBase::~MainWindowBase() {
}
