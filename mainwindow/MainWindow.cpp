//
// Created by imanuel on 08.10.21.
//

#include "MainWindow.h"
#include "../MainApp.h"
#include "../octoprint/OctoApiEventIds.h"
#include "../octoprint/SelectAndPrintFileThread.h"
#include "../octoprint/CancelPrintThread.h"
#include "../octoprint/PausePrintThread.h"
#include "../octoprint/ResumePrintThread.h"
#include "../octoprint/spoolmanager/FetchSpoolsThread.h"
#include "../spoolmanager/AddSpoolDialog.h"
#include "../spoolmanager/EditSpoolDialog.h"
#include <easyhttpcpp/EasyHttp.h>
#include <sstream>
#include <iomanip>
#include <wx/dataview.h>

MainWindow::MainWindow() : MainWindowBase() {
    Bind(wxEVT_SHOW, &MainWindow::handleShow, this);

    Bind(wxEVT_THREAD, &MainWindow::handleFilesFetched, this, OctoApiEventId::OctoFilesFetched);
    Bind(wxEVT_THREAD, &MainWindow::handleFilesFetchError, this, OctoApiEventId::OctoFilesError);

    Bind(wxEVT_THREAD, &MainWindow::handleSpoolsFetched, this, OctoApiEventId::OctoPrintSpoolManagerSpoolsFetched);
    Bind(wxEVT_THREAD, &MainWindow::handleSpoolsFetchError, this,
         OctoApiEventId::OctoPrintSpoolManagerSpoolsFetchError);

    Bind(wxEVT_THREAD, &MainWindow::handleJobFetched, this, OctoApiEventId::OctoJobFetched);
    Bind(wxEVT_THREAD, &MainWindow::handleJobFetchError, this, OctoApiEventId::OctoJobError);

    Bind(wxEVT_THREAD, &MainWindow::handlePrintStartError, this, OctoApiEventId::OctoPrintStartError);

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

    auto fetchSpools = new FetchSpoolsThread(this);
    fetchSpools->Run();

    statusThread = new FetchPrintStatusThread(this);
    statusThread->Run();
}

void MainWindow::handleFilesFetched(wxThreadEvent &event) {
    auto data = event.GetPayload<std::vector<OctoprintFile>>();
    tlcFiles->DeleteAllItems();
    fillFileTree(tlcFiles->GetRootItem(), data);
}

void MainWindow::handleFilesFetchError(wxThreadEvent &event) {
    wxLogStatus(_("Failed to load files"));
}

void MainWindow::fillFileTree(wxTreeListItem parent, const std::vector<OctoprintFile> &files) {
    for (const auto &file: files) {
        auto treeItem = tlcFiles->AppendItem(parent, "");
        tlcFiles->SetItemData(treeItem, new OctoprintFileClientData(file));
        tlcFiles->SetItemText(treeItem, FileListColumns::ColFileName, file.name);
        tlcFiles->SetItemText(treeItem, FileListColumns::ColFileSize, file.getSize());
        tlcFiles->SetItemText(treeItem, FileListColumns::ColFileFullPath, file.path);
        if (file.type == OctoprintFile::File) {
            auto uploaded = new wxDateTime((double) file.uploaded);
            tlcFiles->SetItemText(treeItem, FileListColumns::ColFileUploaded, uploaded->Format());
            tlcFiles->SetItemText(treeItem, FileListColumns::ColFileModelSize, file.getDimensions());
            auto ssFilamentUse = std::stringstream();
            ssFilamentUse << std::fixed << std::setprecision(2) << file.filamentLength / 1000 << "m";
            tlcFiles->SetItemText(treeItem, FileListColumns::ColFileFilamentUse, ssFilamentUse.str());
            tlcFiles->SetItemText(treeItem, FileListColumns::ColFileEstimatedPrintTime, file.getPrintTime());
        } else {
            fillFileTree(treeItem, file.children);
        }
    }
}

void MainWindow::handleJobFetched(wxThreadEvent &event) {
    auto job = event.GetPayload<OctoprintJob>();
    currentJob = job;
    lblFile->SetLabel(job.file);
    lblFinishTime->SetLabel(job.getFinishTime());
    lblTimeElapsed->SetLabel(job.getTimeElapsed());
    lblTimeLeft->SetLabel(job.getTimeLeft());
    prgPrintProgress->SetValue(int(job.printProgress));

    auto selectedFile = tlcFiles->GetSelection();
    auto fileSelected = selectedFile.IsOk() &&
                        dynamic_cast<OctoprintFileClientData *>(tlcFiles->GetItemData(selectedFile))->file.type ==
                        OctoprintFile::File;

    toolbar->EnableTool(MainWindowActions::CancelPrint,
                        job.state == Printing || job.state == Paused || job.state == Pausing);
    toolbar->EnableTool(MainWindowActions::StartPrint,
                        (job.state == Operational || job.state == Error) && job.fileSelected);
    if (fileSelected) {
        toolbar->EnableTool(MainWindowActions::StartPrint,
                            (job.state == Operational || job.state == Error) && fileSelected);
    }
    toolbar->EnableTool(MainWindowActions::PausePrint, job.state == Printing);
    toolbar->EnableTool(MainWindowActions::ResumePrint, job.state == Paused || job.state == Pausing);

    printingMenu->Enable(MainWindowActions::CancelPrint,
                         job.state == Printing || job.state == Paused || job.state == Pausing);
    printingMenu->Enable(MainWindowActions::StartPrint,
                         (job.state == Operational || job.state == Error) && job.fileSelected);
    if (!job.fileSelected) {
        printingMenu->Enable(MainWindowActions::StartPrint,
                             (job.state == Operational || job.state == Error) && fileSelected);
    }
    printingMenu->Enable(MainWindowActions::PausePrint, job.state == Printing);
    printingMenu->Enable(MainWindowActions::ResumePrint, job.state == Paused || job.state == Pausing);
}

void MainWindow::handleJobFetchError(wxThreadEvent &event) {
    wxLogStatus(_("Failed to fetch job files"));
    lblFile->SetLabel(_("No file selected"));
    lblFinishTime->SetLabel(_("No print started"));
    lblTimeElapsed->SetLabel(_("No print started"));
    lblTimeLeft->SetLabel(_("No print started"));
    prgPrintProgress->SetValue(0);

    toolbar->EnableTool(MainWindowActions::CancelPrint, false);
    toolbar->EnableTool(MainWindowActions::StartPrint, false);
    toolbar->EnableTool(MainWindowActions::PausePrint, false);
    toolbar->EnableTool(MainWindowActions::ResumePrint, false);

    printingMenu->Enable(MainWindowActions::CancelPrint, false);
    printingMenu->Enable(MainWindowActions::StartPrint, false);
    printingMenu->Enable(MainWindowActions::PausePrint, false);
    printingMenu->Enable(MainWindowActions::ResumePrint, false);
}

void MainWindow::handleTimer(wxTimerEvent &event) {
    statusThread = new FetchPrintStatusThread(this);
    statusThread->Run();
}

void MainWindow::handleAddSpool(wxCommandEvent &event) {
    auto dialog = new AddSpoolDialog(this);
    dialog->setMaterials(materials);
    dialog->setVendors(vendors);
    dialog->ShowWindowModal();
}

void MainWindow::handleEditSpool(wxCommandEvent &event) {
    auto dialog = new EditSpoolDialog(this);
    dialog->setSpool((OctoprintSpool *) dvlSpools->GetSelection().GetID());
    dialog->setMaterials(materials);
    dialog->setVendors(vendors);
    dialog->ShowWindowModal();
}

void MainWindow::handleDeleteSpool(wxCommandEvent &event) {
}

void MainWindow::handleSelectSpool(wxCommandEvent &event) {
}

void MainWindow::handlePrinterSettings(wxCommandEvent &event) {
    MainApp::getInstance()->ShowPreferencesEditor(this);
}

void MainWindow::handleStartPrint(wxCommandEvent &event) {
    auto treeListItem = tlcFiles->GetSelection();
    if (treeListItem.IsOk() &&
        dynamic_cast<OctoprintFileClientData *>(tlcFiles->GetItemData(treeListItem))->file.type ==
        OctoprintFile::File) {
        auto itemData = tlcFiles->GetItemData(tlcFiles->GetSelection());
        auto selectedFile = dynamic_cast<OctoprintFileClientData *>(itemData)->file;
        if (selectedFile.type == OctoprintFile::Folder) {
            wxMessageBox(_("Please select a file, folders can't be printed"), _("Error"), wxCENTRE | wxICON_ERROR);
        } else {
            auto selectFileThread = new SelectAndPrintFileThread(this, selectedFile.origin, selectedFile.path);
            selectFileThread->Run();
        }
    } else if (currentJob.fileSelected) {
        auto selectFileThread = new SelectAndPrintFileThread(this, currentJob.origin, currentJob.path);
        selectFileThread->Run();
    }
}

void MainWindow::handleResumePrint(wxCommandEvent &event) {
    auto resumePrintThread = new ResumePrintThread(this);
    resumePrintThread->Run();
}

void MainWindow::handlePausePrint(wxCommandEvent &event) {
    auto dialog = new wxMessageDialog(this, _("Are you sure to pause the print?"), _("Pause print?"),
                                      wxCENTER | wxNO_DEFAULT | wxYES_NO | wxICON_QUESTION);
    dialog->SetYesNoLabels("Pause print", "Keep printing");

    dialog->Bind(wxEVT_WINDOW_MODAL_DIALOG_CLOSED, &MainWindow::handlePausePrintDialogClosed, this);
    dialog->ShowWindowModal();
}

void MainWindow::handleCancelPrint(wxCommandEvent &event) {
    auto dialog = new wxMessageDialog(this, _("Are you sure to cancel the print?"), _("Cancel print?"),
                                      wxCENTER | wxNO_DEFAULT | wxYES_NO | wxICON_QUESTION);
    dialog->SetYesNoLabels("Cancel print", "Keep printing");

    dialog->Bind(wxEVT_WINDOW_MODAL_DIALOG_CLOSED, &MainWindow::handleCancelPrintDialogClosed, this);
    dialog->ShowWindowModal();
}

void MainWindow::handlePrintStartError(wxThreadEvent &event) {
    wxLogStatus(_("Failed to start print"));
}

void MainWindow::handleCancelPrintDialogClosed(wxWindowModalDialogEvent &event) {
    if (event.GetReturnCode() == wxID_YES) {
        auto cancelPrintThread = new CancelPrintThread(this);
        cancelPrintThread->Run();
    }
}

void MainWindow::handlePausePrintDialogClosed(wxWindowModalDialogEvent &event) {
    if (event.GetReturnCode() == wxID_YES) {
        auto pausePrintThread = new PausePrintThread(this);
        pausePrintThread->Run();
    }
}

void MainWindow::handleSpoolsFetched(wxThreadEvent &event) {
    data = event.GetPayload<OctoprintSpoolData>();
    spoolListModel->Fill(data.spools, data.selectedSpool);
    dvlSpools->AssociateModel(spoolListModel.get());
    vendors = data.vendors;
    materials = data.materials;

    auto spool = wxDataViewItem(spoolListModel->getSelectedItem());
    auto item = wxDataViewItem(spool);
    if (item.IsOk()) {
        dvlSpools->SetCurrentItem(item);
    }
}

void MainWindow::handleSpoolsFetchError(wxThreadEvent &event) {
    wxLogStatus(_("Failed to fetch spools"));
}
