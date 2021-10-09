//
// Created by imanuel on 08.10.21.
//

#ifndef WXOCTO_MAINWINDOW_H
#define WXOCTO_MAINWINDOW_H

#include "MainWindowBase.h"
#include "../octoprint/FetchFilesThread.h"
#include <wx/app.h>

class MainWindow : public MainWindowBase {
public:
protected:
    void setupEvents() override;

private:
    void handleShow(wxShowEvent &event);

    void updateView();

    void fillFileTree(wxTreeListItem parent, const std::vector<OctoprintFile> &files);

public:
    void handlePrinterSettings() override;

    MainWindow();

    void reloadSettings();

    void handleFilesFetched(wxThreadEvent &event);

    void handleFilesFetchedError(wxThreadEvent &event);
};


#endif //WXOCTO_MAINWINDOW_H
