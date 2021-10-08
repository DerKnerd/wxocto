//
// Created by imanuel on 08.10.21.
//

#include "MainWindow.h"
#include "../MainApp.h"


MainWindow::MainWindow() : MainWindowBase() {
}

void MainWindow::setupEvents() {

}

void MainWindow::reloadSettings() {

}

void MainWindow::handlePrinterSettings() {
    MainWindowBase::handlePrinterSettings();
    MainApp::getInstance()->ShowPreferencesEditor(this);
}
