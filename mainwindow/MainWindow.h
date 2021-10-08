//
// Created by imanuel on 08.10.21.
//

#ifndef WXOCTO_MAINWINDOW_H
#define WXOCTO_MAINWINDOW_H

#include "MainWindowBase.h"
#include <wx/app.h>

class MainWindow : public MainWindowBase {
public:
protected:
    void setupEvents() override;

public:
    void handlePrinterSettings() override;

public:
    MainWindow();
    void reloadSettings();
};


#endif //WXOCTO_MAINWINDOW_H
