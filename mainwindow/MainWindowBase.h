//
// Created by imanuel on 07.10.21.
//

#ifndef WXOCTO_MAINWINDOWBASE_H
#define WXOCTO_MAINWINDOWBASE_H

#include <wx/wxprec.h>

#ifndef WX_PRECOMP

#include <wx/wx.h>

#endif

#include <wx/persist.h>
#include <wx/persist/toplevel.h>
#include <wx/persist/bookctrl.h>
#include <wx/persist/treebook.h>
#include <wx/treelist.h>
#include <wx/dataview.h>

enum MainWindowActions {
    StartPrint = 200,
    ResumePrint,
    PausePrint,
    CancelPrint,
    AddSpool,
    EditSpool,
    DeleteSpool,
    PrinterSettings,
};

class MainWindowBase : public wxFrame {
private:
    void handleMenuOrToolbarClicked(wxCommandEvent &event);

protected:
    wxMenu *printingMenu;
    wxToolBarBase *toolbar;
    wxGauge *prgPrintProgress;
    wxStaticText *lblTimeElapsed;
    wxStaticText *lblTimeLeft;
    wxStaticText *lblFinishTime;
    wxStaticText *lblFile;
    wxNotebook *nbContent;
    wxPanel *nbpFiles;
    wxTreeListCtrl *tlcFiles;
    wxPanel *nbpSpools;
    wxDataViewListCtrl *dvlSpools;

protected:
    virtual void setupEvents() {}

public:
    wxToolBarBase *getTbPrinting() { return toolbar; }

    wxGauge *getPrgPrintProgress() { return prgPrintProgress; }

    wxStaticText *getLblTimeElapsed() { return lblTimeElapsed; }

    wxStaticText *getLblTimeLeft() { return lblTimeLeft; }

    wxStaticText *getLblFinishTime() { return lblFinishTime; }

    wxStaticText *getLblFile() { return lblFile; }

    wxTreeListCtrl *getTlcFiles() { return tlcFiles; }

    wxPanel *getNbpFiles() { return nbpFiles; }

    wxDataViewListCtrl *getDvlSpools() { return dvlSpools; }

    wxPanel *getNbpSpools() { return nbpSpools; }

    wxNotebook *getNbContent() { return nbContent; }

    MainWindowBase();

    ~MainWindowBase() override;

    virtual void handleExit();

    virtual void handlePrinterSettings();

    virtual void handleAddSpool();

    virtual void handleEditSpool();

    virtual void handleDeleteSpool();

    virtual void handleStartPrint();

    virtual void handleResumePrint();

    virtual void handlePausePrint();

    virtual void handleCancelPrint();
};


#endif //WXOCTO_MAINWINDOWBASE_H
