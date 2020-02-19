// copyright (c) 2020 hors<horsicq@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
#ifndef GUIMAINWINDOW_H
#define GUIMAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include "dialogprocess.h"
#include "dialogabout.h"
#include "dialogoptions.h"
#include "../global.h"

namespace Ui
{
class GuiMainWindow;
}

class GuiMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit GuiMainWindow(QWidget *parent = 0);
    ~GuiMainWindow();

private slots:
    void on_actionOpen_triggered();
    void on_listWidgetSymTags_currentRowChanged(int currentRow);
    void on_actionC_C_triggered();

    void on_comboBoxType_currentIndexChanged(int index);
    void handle();
//    void on_checkBoxOffsets_toggled(bool checked);
//    void on_checkBoxSizes_toggled(bool checked);
    void on_actionOptions_triggered();
    void on_actionAbout_triggered();
    void adjustWindow();

private:
    enum CBT
    {
        CBT_CLASSES=0,
        CBT_STRUCTS,
        CBT_UNIONS,
        CBT_INTERFACES,
        CBT_ENUMS
    };

    Ui::GuiMainWindow *ui;
    QWinPDB *pWinPDB;
    QWinPDB::STATS stats;
    PDBRIPPER::OPTIONS options;
};

#endif // GUIMAINWINDOW_H
