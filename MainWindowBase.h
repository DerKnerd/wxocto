//
// Created by imanuel on 07.10.21.
//

#ifndef WXOCTO_MAINWINDOWBASE_H
#define WXOCTO_MAINWINDOWBASE_H

#include <wx/wx.h>

#if wxVERSION_NUMBER >= 2900

#include <wx/persist.h>
#include <wx/persist/toplevel.h>
#include <wx/persist/bookctrl.h>
#include <wx/persist/treebook.h>
#include <wx/treelist.h>
#include <wx/dataview.h>

#endif

#ifdef WXC_FROM_DIP
#undef WXC_FROM_DIP
#endif
#if wxVERSION_NUMBER >= 3100
#define WXC_FROM_DIP(x) wxWindow::FromDIP(x, NULL)
#else
#define WXC_FROM_DIP(x) x
#endif

enum ToolbarItems {
    StartPrint = 200,
    ResumePrint,
    PausePrint,
    CancelPrint,
    AddSpool,
    EditSpool,
    DeleteSpool,
};

class MainWindowBase : public wxFrame {
protected:
    wxToolBarBase *toolbar;
    wxStaticText *m_staticText9;
    wxGauge *prgPrintProgress;
    wxStaticText *m_staticText17;
    wxStaticText *lblTimeElapsed;
    wxStaticText *m_staticText21;
    wxStaticText *lblTimeLeft;
    wxStaticText *m_staticText27;
    wxStaticText *lblFinishTime;
    wxStaticText *m_staticText35;
    wxStaticText *lblFile;
    wxNotebook *nbContent;
    wxPanel *nbpFiles;
    wxTreeListCtrl *tlcFiles;
    wxPanel *nbpSpools;
    wxDataViewListCtrl *dvlSpools;

protected:
    virtual void OnStartPrintClick(wxCommandEvent &event) { event.Skip(); }

public:
    wxToolBarBase *GetTbPrinting() { return toolbar; }

    wxStaticText *GetStaticText9() { return m_staticText9; }

    wxGauge *GetPrgPrintProgress() { return prgPrintProgress; }

    wxStaticText *GetStaticText17() { return m_staticText17; }

    wxStaticText *GetLblTimeElapsed() { return lblTimeElapsed; }

    wxStaticText *GetStaticText21() { return m_staticText21; }

    wxStaticText *GetLblTimeLeft() { return lblTimeLeft; }

    wxStaticText *GetStaticText27() { return m_staticText27; }

    wxStaticText *GetLblFinishTime() { return lblFinishTime; }

    wxStaticText *GetStaticText35() { return m_staticText35; }

    wxStaticText *GetLblFile() { return lblFile; }

    wxTreeListCtrl *GetTlcFiles() { return tlcFiles; }

    wxPanel *GetNbpFiles() { return nbpFiles; }

    wxDataViewListCtrl *GetDvlSpools() { return dvlSpools; }

    wxPanel *GetNbpSpools() { return nbpSpools; }

    wxNotebook *GetNbContent() { return nbContent; }

    explicit MainWindowBase(wxWindow *parent, wxWindowID id = wxID_ANY, const wxString &title = _("wxOcto"),
                            const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxSize(1280, 600),
                            long style = wxDEFAULT_FRAME_STYLE);

    ~MainWindowBase() override;
};


#endif //WXOCTO_MAINWINDOWBASE_H
