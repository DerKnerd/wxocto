//
// Created by imanuel on 08.10.21.
//

#ifndef WXOCTO_MAINWINDOW_H
#define WXOCTO_MAINWINDOW_H

#include "MainWindowBase.h"
#include "../octoprint/FetchFilesThread.h"
#include "../octoprint/FetchPrintStatusThread.h"
#include <wx/app.h>

class MainWindow : public MainWindowBase {
public:
protected:
    void setupEvents() override;

private:
    wxTimer *pollOctoTimer;
    FetchPrintStatusThread *statusThread = nullptr;

    void handleShow(wxShowEvent &event);

    void updateView();

    void fillFileTree(wxTreeListItem parent, const std::vector<OctoprintFile> &files);


    void handleFilesFetched(wxThreadEvent &event);

    void handleFilesFetchedError(wxThreadEvent &event);

    void handleJobFetched(wxThreadEvent &event);

    void handleJobFetchedError(wxThreadEvent &event);

    void handleTimer(wxTimerEvent &event);

public:
    void handlePrinterSettings(wxCommandEvent &event) override;

    void handleAddSpool(wxCommandEvent &event) override;

    void handleEditSpool(wxCommandEvent &event) override;

    void handleDeleteSpool(wxCommandEvent &event) override;

    MainWindow();

    void reloadSettings();
};


#endif //WXOCTO_MAINWINDOW_H
