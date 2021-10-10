//
// Created by imanuel on 08.10.21.
//

#include "MainWindow.h"
#include "../MainApp.h"
#include "../octoprint/OctoApiEventIds.h"
#include "../octoprint/OctoprintJob.h"
#include <easyhttpcpp/EasyHttp.h>
#include <sstream>
#include <iomanip>

MainWindow::MainWindow() : MainWindowBase() {
    Bind(wxEVT_SHOW, &MainWindow::handleShow, this);
    Bind(wxEVT_THREAD, &MainWindow::handleFilesFetched, this, OctoApiEventId::OctoFilesFetched);
    Bind(wxEVT_THREAD, &MainWindow::handleFilesFetchedError, this, OctoApiEventId::OctoFilesError);
    Bind(wxEVT_THREAD, &MainWindow::handleJobFetched, this, OctoApiEventId::OctoJobFetched);
    Bind(wxEVT_THREAD, &MainWindow::handleJobFetchedError, this, OctoApiEventId::OctoJobError);
    Bind(wxEVT_TIMER, &MainWindow::handleTimer, this);
    pollOctoTimer = new wxTimer(this);
    pollOctoTimer->Start(500);
}

void MainWindow::setupEvents() {
}

void MainWindow::reloadSettings() {
    auto settings = MainApp::getInstance()->GetSettings();
    if (settings.apiKey != "") {
        updateView();
    }
}

void MainWindow::handleShow(wxShowEvent &event) {
    auto settings = MainApp::getInstance()->GetSettings();
    if (settings.apiKey != "") {
        updateView();
    }
}

void MainWindow::updateView() {
    auto fetchFiles = new FetchFilesThread(this);
    fetchFiles->Run();

    statusThread = new FetchPrintStatusThread(this);
    statusThread->Run();
}

void MainWindow::handleFilesFetched(wxThreadEvent &event) {
    auto data = event.GetPayload<std::vector<OctoprintFile>>();
    tlcFiles->DeleteAllItems();
    fillFileTree(tlcFiles->GetRootItem(), data);
}

void MainWindow::handleFilesFetchedError(wxThreadEvent &event) {
    wxMessageBox(_("Failed to load files"));
}

void MainWindow::fillFileTree(wxTreeListItem parent, const std::vector<OctoprintFile> &files) {
    for (const auto &file: files) {
        auto treeItem = tlcFiles->AppendItem(parent, "");
        tlcFiles->SetItemData(treeItem, new OctoprintFileClientData(file));
        tlcFiles->SetItemText(treeItem, FileListColumns::ColName, file.name);
        tlcFiles->SetItemText(treeItem, FileListColumns::ColSize, file.getSize());
        if (file.type == OctoprintFile::File) {
            auto uploaded = new wxDateTime(file.uploaded);
            tlcFiles->SetItemText(treeItem, FileListColumns::ColUploaded, uploaded->Format());
            tlcFiles->SetItemText(treeItem, FileListColumns::ColModelSize, file.getDimensions());
            auto ssFilamentUse = std::stringstream();
            ssFilamentUse << std::fixed << std::setprecision(2) << file.filamentLength / 1000 << "m";
            tlcFiles->SetItemText(treeItem, FileListColumns::ColFilamentUse, ssFilamentUse.str());
            tlcFiles->SetItemText(treeItem, FileListColumns::ColEstimatedPrintTime, file.getPrintTime());
        } else {
            fillFileTree(treeItem, file.children);
        }
    }
}

void MainWindow::handleJobFetched(wxThreadEvent &event) {
    auto job = event.GetPayload<OctoprintJob>();
    lblFile->SetLabel(job.file);
    lblFinishTime->SetLabel(job.getFinishTime());
    lblTimeElapsed->SetLabel(job.getTimeElapsed());
    lblTimeLeft->SetLabel(job.getTimeLeft());
    prgPrintProgress->SetValue(int(job.printProgress));
}

void MainWindow::handleJobFetchedError(wxThreadEvent &event) {
    wxMessageBox(_("Failed to load job"));
}

void MainWindow::handleTimer(wxTimerEvent &event) {
    statusThread = new FetchPrintStatusThread(this);
    statusThread->Run();
}

void MainWindow::handleAddSpool(wxCommandEvent &event) {

}

void MainWindow::handleEditSpool(wxCommandEvent &event) {

}

void MainWindow::handleDeleteSpool(wxCommandEvent &event) {

}

void MainWindow::handlePrinterSettings(wxCommandEvent &event) {
    MainApp::getInstance()->ShowPreferencesEditor(this);
}

