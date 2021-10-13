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

#include <utility>
#include "../octoprint/OctoprintFile.h"
#include "../octoprint/spoolmanager/OctoprintSpool.h"

enum MainWindowActions {
    StartPrint = 200,
    ResumePrint,
    PausePrint,
    CancelPrint,
    AddSpool,
    EditSpool,
    DeleteSpool,
    SelectSpool,
    PrinterSettings,
};

enum FileListColumns {
    ColFileName = 0,
    ColFileSize,
    ColFileUploaded,
    ColFileModelSize,
    ColFileFilamentUse,
    ColFileEstimatedPrintTime,
    ColFileFullPath
};

enum SpoolListColumns {
    ColSpoolName = 0,
    ColSpoolMaterial,
    ColSpoolLastUsed,
    ColSpoolWeightTotal,
    ColSpoolWeightUsed,
    ColSpoolWeightLeft,
    ColSpoolLengthTotal,
    ColSpoolLengthUsed,
    ColSpoolLengthLeft,
    SpoolColumnCount
};

class OctoprintFileClientData : public wxClientData {
public:
    explicit OctoprintFileClientData(OctoprintFile file) : file(std::move(file)) {}

    OctoprintFile file;
};

class OctoprintFileTreeListItemComparator : public wxTreeListItemComparator {
public:
    int Compare(wxTreeListCtrl *treelist, unsigned int column, wxTreeListItem first, wxTreeListItem second) override;
};

class OctoprintSpoolDataViewListModel : public wxDataViewVirtualListModel {
public:
    [[nodiscard]] unsigned int GetColumnCount() const override;

    OctoprintSpoolDataViewListModel();

    [[nodiscard]] wxString GetColumnType(unsigned int col) const override;

    void GetValueByRow(wxVariant &variant, unsigned int row, unsigned int col) const override;

    bool SetValueByRow(const wxVariant &variant, unsigned int row, unsigned int col) override;

    void Fill(std::vector<OctoprintSpool> data);

private:
    std::vector<OctoprintSpool> items;
};

class MainWindowBase : public wxFrame {
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
    wxStatusBar *statusBar;
    OctoprintSpoolDataViewListModel *spoolListModel;

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

    virtual void handleExit(wxCommandEvent &event);

    virtual void handlePrinterSettings(wxCommandEvent &event) = 0;

    virtual void handleAddSpool(wxCommandEvent &event) = 0;

    virtual void handleEditSpool(wxCommandEvent &event) = 0;

    virtual void handleDeleteSpool(wxCommandEvent &event) = 0;

    virtual void handleSelectSpool(wxCommandEvent &event) = 0;

    virtual void handleStartPrint(wxCommandEvent &event) = 0;

    virtual void handleResumePrint(wxCommandEvent &event) = 0;

    virtual void handlePausePrint(wxCommandEvent &event) = 0;

    virtual void handleCancelPrint(wxCommandEvent &event) = 0;
};


#endif //WXOCTO_MAINWINDOWBASE_H
