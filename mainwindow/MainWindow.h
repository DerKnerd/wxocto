//
// Created by imanuel on 08.10.21.
//

#ifndef WXOCTO_MAINWINDOW_H
#define WXOCTO_MAINWINDOW_H

#include "MainWindowBase.h"
#include "../octoprint/FetchFilesThread.h"
#include "../octoprint/FetchPrintStatusThread.h"
#include "../octoprint/OctoprintJob.h"
#include <wx/app.h>

class MainWindow : public MainWindowBase {
public:
protected:
    void setupEvents() override;

private:
    wxTimer *pollOctoTimer;
    FetchPrintStatusThread *statusThread = nullptr;
    OctoprintJob currentJob;

    void handleShow(wxShowEvent &event);

    void updateView();

    void fillFileTree(wxTreeListItem parent, const std::vector<OctoprintFile> &files);

public:
    void handleStartPrint(wxCommandEvent &event) override;

private:
    void handleFilesFetched(wxThreadEvent &event);

    void handleFilesFetchError(wxThreadEvent &event);

    void handleJobFetched(wxThreadEvent &event);

    void handleJobFetchError(wxThreadEvent &event);

    void handleSpoolsFetched(wxThreadEvent &event);

    void handleSpoolsFetchError(wxThreadEvent &event);

    void handleTimer(wxTimerEvent &event);

    void handlePrintStartError(wxThreadEvent &event);

    void handleCancelPrintDialogClosed(wxWindowModalDialogEvent &event);

    void handlePausePrintDialogClosed(wxWindowModalDialogEvent &event);

public:
    void handleResumePrint(wxCommandEvent &event) override;

    void handlePausePrint(wxCommandEvent &event) override;

    void handleCancelPrint(wxCommandEvent &event) override;

    void handlePrinterSettings(wxCommandEvent &event) override;

    void handleAddSpool(wxCommandEvent &event) override;

    void handleEditSpool(wxCommandEvent &event) override;

    void handleDeleteSpool(wxCommandEvent &event) override;

    MainWindow();

    void reloadSettings();

    void handleSelectSpool(wxCommandEvent &event) override;
};


#endif //WXOCTO_MAINWINDOW_H
