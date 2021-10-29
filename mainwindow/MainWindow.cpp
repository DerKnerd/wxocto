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
#include "../octoprint/spoolmanager/DeleteSpoolThread.h"
#include "../octoprint/spoolmanager/SelectSpoolThread.h"
#include "../octoprint/DeleteFileThread.h"
#include <easyhttpcpp/EasyHttp.h>
#include <sstream>
#include <iomanip>
#include <wx/dataview.h>

MainWindow::MainWindow() : MainWindowBase() {
    Bind(wxEVT_SHOW, &MainWindow::handleShow, this);

    Bind(wxEVT_THREAD, &MainWindow::handleFilesFetched, this, OctoApiEventId::OctoFilesFetched);
    Bind(wxEVT_THREAD, &MainWindow::handleFilesFetchError, this, OctoApiEventId::OctoFilesError);

    Bind(wxEVT_THREAD, &MainWindow::handleFileDeleted, this, OctoApiEventId::OctoFileDeleted);
    Bind(wxEVT_THREAD, &MainWindow::handleFileDeleteError, this, OctoApiEventId::OctoFileDeleteError);

    Bind(wxEVT_THREAD, &MainWindow::handleSpoolsFetched, this, OctoApiEventId::OctoPrintSpoolManagerSpoolsFetched);
    Bind(wxEVT_THREAD, &MainWindow::handleSpoolsFetchError, this,
         OctoApiEventId::OctoPrintSpoolManagerSpoolsFetchError);

    Bind(wxEVT_THREAD, &MainWindow::handleSpoolsDeleted, this, OctoApiEventId::OctoPrintSpoolManagerSpoolDeleted);
    Bind(wxEVT_THREAD, &MainWindow::handleSpoolsDeleteError, this,
         OctoApiEventId::OctoPrintSpoolManagerSpoolDeleteError);

    Bind(wxEVT_THREAD, &MainWindow::handleJobFetched, this, OctoApiEventId::OctoJobFetched);
    Bind(wxEVT_THREAD, &MainWindow::handleJobFetchError, this, OctoApiEventId::OctoJobError);

    Bind(wxEVT_THREAD, &MainWindow::handlePrintStartError, this, OctoApiEventId::OctoPrintStartError);

    Bind(wxEVT_THREAD, &MainWindow::handleSpoolSelected, this, OctoApiEventId::OctoPrintSpoolManagerSpoolSelected);
    Bind(wxEVT_THREAD, &MainWindow::handleSpoolSelectError, this,
         OctoApiEventId::OctoPrintSpoolManagerSpoolSelectError);

    Bind(wxEVT_WINDOW_MODAL_DIALOG_CLOSED, &MainWindow::handleSpoolSaved, this);

    Bind(wxEVT_TIMER, &MainWindow::handleTimer, this);

    pollOctoTimer = new wxTimer(this);
    pollOctoTimer->Start(500);
    spoolListModel = new OctoprintSpoolDataViewListModel();
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
            auto uploaded = new wxDateTime(file.uploaded);
            if (uploaded->IsValid()) {
                tlcFiles->SetItemText(treeItem, FileListColumns::ColFileUploaded, uploaded->Format());
            }
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

    selectedSpoolChoice->Enable(!(job.state == Printing || job.state == Paused || job.state == Pausing));

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

    octoprintMenu->Enable(MainWindowActions::CancelPrint,
                          job.state == Printing || job.state == Paused || job.state == Pausing);
    octoprintMenu->Enable(MainWindowActions::StartPrint,
                          (job.state == Operational || job.state == Error) && job.fileSelected);
    if (!job.fileSelected) {
        octoprintMenu->Enable(MainWindowActions::StartPrint,
                              (job.state == Operational || job.state == Error) && fileSelected);
    }
    octoprintMenu->Enable(MainWindowActions::PausePrint, job.state == Printing);
    octoprintMenu->Enable(MainWindowActions::ResumePrint, job.state == Paused || job.state == Pausing);
    checkIfFileIsDeletable();
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

    octoprintMenu->Enable(MainWindowActions::CancelPrint, false);
    octoprintMenu->Enable(MainWindowActions::StartPrint, false);
    octoprintMenu->Enable(MainWindowActions::PausePrint, false);
    octoprintMenu->Enable(MainWindowActions::ResumePrint, false);
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
    auto dialog = new wxMessageDialog(this, _("Are you sure to delete the selected spool?"), _("Delete spool?"),
                                      wxCENTER | wxNO_DEFAULT | wxYES_NO | wxICON_WARNING);
    dialog->SetYesNoLabels(_("Delete spool"), _("Keep spool"));

    dialog->Bind(wxEVT_WINDOW_MODAL_DIALOG_CLOSED, &MainWindow::handleDeleteSpoolDialog, this);
    dialog->ShowWindowModal();
}

void MainWindow::handlePrinterSettings(wxCommandEvent &event) {
    MainApp::getInstance()->ShowPreferencesEditor(this);
}

void MainWindow::handleStartPrint(wxCommandEvent &event) {
    auto treeListItem = tlcFiles->GetSelection();
    auto filamentLengthToPrint = 0.0;
    auto canPrint = false;
    if (treeListItem.IsOk() &&
        dynamic_cast<OctoprintFileClientData *>(tlcFiles->GetItemData(treeListItem))->file.type ==
        OctoprintFile::File) {
        auto itemData = tlcFiles->GetItemData(tlcFiles->GetSelection());
        auto selectedFile = dynamic_cast<OctoprintFileClientData *>(itemData)->file;
        if (selectedFile.type == OctoprintFile::Folder) {
            wxMessageBox(_("Please select a file, folders can't be printed"), _("Error"), wxCENTRE | wxICON_ERROR);
        } else {
            canPrint = true;
            filamentLengthToPrint = selectedFile.filamentLength;
        }
    } else if (currentJob.fileSelected) {
        canPrint = true;
        filamentLengthToPrint = currentJob.filamentLength;
    }

    if (canPrint) {
        auto selection = selectedSpoolChoice->GetSelection();
        if (selection == 0) {
            auto selectSpoolThread = new SelectSpoolThread(this, -1);
            selectSpoolThread->Run();
        } else {
            auto selectedSpool = (OctoprintSpool *) selectedSpoolChoice->GetClientData(selection);
            if (selectedSpool->leftLength < filamentLengthToPrint) {
                wxMessageBox(
                        _("You don't have enough filament on the selected spool, please choose a different spool"));
            } else {
                auto selectSpoolThread = new SelectSpoolThread(this, selectedSpool->databaseId);
                selectSpoolThread->Run();
            }
        }
    }
}

void MainWindow::handleResumePrint(wxCommandEvent &event) {
    auto resumePrintThread = new ResumePrintThread(this);
    resumePrintThread->Run();
}

void MainWindow::handlePausePrint(wxCommandEvent &event) {
    auto dialog = new wxMessageDialog(this, _("Are you sure to pause the print?"), _("Pause print?"),
                                      wxCENTER | wxNO_DEFAULT | wxYES_NO | wxICON_QUESTION);
    dialog->SetYesNoLabels(_("Pause print"), _("Keep printing"));

    dialog->Bind(wxEVT_WINDOW_MODAL_DIALOG_CLOSED, &MainWindow::handlePausePrintDialogClosed, this);
    dialog->ShowWindowModal();
}

void MainWindow::handleCancelPrint(wxCommandEvent &event) {
    auto dialog = new wxMessageDialog(this, _("Are you sure to cancel the print?"), _("Cancel print?"),
                                      wxCENTER | wxNO_DEFAULT | wxYES_NO | wxICON_QUESTION);
    dialog->SetYesNoLabels(_("Cancel print"), _("Keep printing"));

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
    octoprintSpoolData = event.GetPayload<OctoprintSpoolData>();
    spoolListModel->Fill(octoprintSpoolData.spools);
    dvlSpools->AssociateModel(spoolListModel);
    vendors = octoprintSpoolData.vendors;
    materials = octoprintSpoolData.materials;
    selectedSpoolChoice->Clear();
    auto foundSpool = false;
    selectedSpoolChoice->Append(_("No spool"));
    for (const auto &item: octoprintSpoolData.spools) {
        selectedSpoolChoice->Append(item->displayName, item);
        if (item->databaseId == octoprintSpoolData.selectedSpool) {
            selectedSpool = *item;
            foundSpool = true;
        }
    }

    if (foundSpool) {
        selectedSpoolChoice->SetStringSelection(selectedSpool.displayName);
    } else {
        selectedSpoolChoice->SetSelection(0);
    }
}

void MainWindow::handleSpoolsFetchError(wxThreadEvent &event) {
    wxLogStatus(_("Failed to fetch spools"));
}

void MainWindow::handleSpoolSaved(wxWindowModalDialogEvent &event) {
    auto fetchSpools = new FetchSpoolsThread(this);
    fetchSpools->Run();
}

void MainWindow::handleDeleteSpoolDialog(wxWindowModalDialogEvent &event) {
    auto selectedSpoolItem = dvlSpools->GetSelection();
    auto selectedSpool = (OctoprintSpool *) (selectedSpoolItem.m_pItem);

    auto deleteSpool = new DeleteSpoolThread(this, selectedSpool->databaseId);
    deleteSpool->Run();
}

void MainWindow::handleDvlSpoolsSelectionChanged(wxDataViewEvent &event) {
    auto item = event.GetItem();
    toolbar->EnableTool(MainWindowActions::DeleteSpool, item.IsOk());
    toolbar->EnableTool(MainWindowActions::EditSpool, item.IsOk());

    spoolsMenu->Enable(MainWindowActions::DeleteSpool, item.IsOk());
    spoolsMenu->Enable(MainWindowActions::EditSpool, item.IsOk());
}

void MainWindow::handleSpoolsDeleted(wxThreadEvent &event) {
    auto fetchSpools = new FetchSpoolsThread(this);
    fetchSpools->Run();
}

void MainWindow::handleSpoolsDeleteError(wxThreadEvent &event) {
    wxMessageBox(_("Failed to delete the spool"));
}

void MainWindow::handleSpoolSelected(wxThreadEvent &event) {
    selectFileAndPrint();
}

void MainWindow::selectFileAndPrint() {
    auto treeListItem = tlcFiles->GetSelection();
    if (treeListItem.IsOk() &&
        dynamic_cast<OctoprintFileClientData *>(tlcFiles->GetItemData(treeListItem))->file.type ==
        OctoprintFile::File) {
        auto itemData = tlcFiles->GetItemData(tlcFiles->GetSelection());
        auto selectedFile = dynamic_cast<OctoprintFileClientData *>(itemData)->file;
        auto selectFileThread = new SelectAndPrintFileThread(this, selectedFile.origin, selectedFile.path);
        selectFileThread->Run();
    } else if (currentJob.fileSelected) {
        auto selectFileThread = new SelectAndPrintFileThread(this, currentJob.origin, currentJob.path);
        selectFileThread->Run();
    }
}

void MainWindow::handleSpoolSelectError(wxThreadEvent &event) {
    wxMessageBox(_("Failed to select spool"));
}

void MainWindow::handleTlcFilesSelectionChanged(wxDataViewEvent &event) {
    checkIfFileIsDeletable();
}

void MainWindow::checkIfFileIsDeletable() {
    auto treeListItem = tlcFiles->GetSelection();
    if (treeListItem.IsOk()) {
        auto item = dynamic_cast<OctoprintFileClientData *>(tlcFiles->GetItemData(treeListItem));
        auto canDelete = item->file.path != currentJob.path && item->file.type == OctoprintFile::File;

        toolbar->EnableTool(DeleteFile, canDelete);
        octoprintMenu->Enable(DeleteFile, canDelete);
    } else {
        toolbar->EnableTool(DeleteFile, false);
        octoprintMenu->Enable(DeleteFile, false);
    }
}

void MainWindow::handleUploadFile(wxCommandEvent &event) {

}

void MainWindow::handleDeleteFile(wxCommandEvent &event) {
    auto dialog = new wxMessageDialog(this, _("Are you sure to delete the file?"), wxMessageBoxCaptionStr,
                                      wxCENTER | wxNO_DEFAULT | wxYES_NO | wxICON_QUESTION);
    dialog->SetYesNoLabels(_("Delete file"), _("Keep file"));

    dialog->Bind(wxEVT_WINDOW_MODAL_DIALOG_CLOSED, &MainWindow::handleDeleteFileDialog, this);
    dialog->ShowWindowModal();
}

void MainWindow::handleDeleteFileDialog(wxWindowModalDialogEvent &event) {
    auto treeListItem = tlcFiles->GetSelection();
    auto item = dynamic_cast<OctoprintFileClientData *>(tlcFiles->GetItemData(treeListItem));
    auto deleteFile = new DeleteFileThread(this, item->file);
    deleteFile->Run();
}

void MainWindow::handleFileDeleted(wxThreadEvent &event) {
    updateView();
}

void MainWindow::handleFileDeleteError(wxThreadEvent &event) {
    updateView();
}
