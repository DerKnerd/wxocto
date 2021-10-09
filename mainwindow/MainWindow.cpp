//
// Created by imanuel on 08.10.21.
//

#include "MainWindow.h"
#include "../MainApp.h"
#include <easyhttpcpp/EasyHttp.h>
#include <sstream>
#include <iomanip>

MainWindow::MainWindow() : MainWindowBase() {
    Bind(wxEVT_SHOW, &MainWindow::handleShow, this);
    Bind(wxEVT_THREAD, &MainWindow::handleFilesFetched, this, FetchFilesThread::OctoFilesFetched);
    Bind(wxEVT_THREAD, &MainWindow::handleFilesFetchedError, this, FetchFilesThread::OctoFilesError);
}

void MainWindow::setupEvents() {
}

void MainWindow::reloadSettings() {
    auto settings = MainApp::getInstance()->GetSettings();
    if (settings.apiKey != "") {
        updateView();
    }
}

void MainWindow::handlePrinterSettings() {
    MainWindowBase::handlePrinterSettings();
    MainApp::getInstance()->ShowPreferencesEditor(this);
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
}

void MainWindow::handleFilesFetched(wxThreadEvent &event) {
    auto data = event.GetPayload<std::vector<OctoprintFile>>();
    fillFileTree(tlcFiles->GetRootItem(), data);
}

void MainWindow::handleFilesFetchedError(wxThreadEvent &event) {
    wxMessageBox(_("Failed to load files"));
}

void MainWindow::fillFileTree(wxTreeListItem parent, const std::vector<OctoprintFile> &files) {
    for (const auto& file: files) {
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

