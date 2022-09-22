/* Copyright (c) 2020-2022 hors<horsicq@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef GUIMAINWINDOW_H
#define GUIMAINWINDOW_H

#include <QDragEnterEvent>
#include <QFileDialog>
#include <QMainWindow>
#include <QMessageBox>
#include <QMimeData>
#include <QSortFilterProxyModel>
#include <QStandardItemModel>
#include "../global.h"
#include "dialogabout.h"
#include "dialogexport.h"
#include "dialogoptions.h"
#include "dialogprocess.h"
#include "xoptions.h"

namespace Ui
{
class GuiMainWindow;
}

class GuiMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit GuiMainWindow(QWidget *pParent=0);
    ~GuiMainWindow();

private slots:
    void createMenus();
    void actionOpenSlot();
    void actionCloseSlot();
    void actionExitSlot();
    void actionCPPSlot();
    void actionXNTSVSlot();
    void actionOptionsSlot();
    void actionAboutSlot();

    void adjustWindow();

    void processFile(QString sFileName);
    void cleanUp();

    void on_lineEditSearch_textChanged(const QString &arg1);
    void onCurrentChanged(const QModelIndex &current,const QModelIndex &previous);

    void handle();
    void on_tableViewSymbols_clicked(const QModelIndex &index);
    void on_checkBoxShowComments_toggled(bool checked);

    void setHandleOptions(QWinPDB::HANDLE_OPTIONS *pHandleOptions);
    QWinPDB::HANDLE_OPTIONS getHandleOptions();

    void on_comboBoxFixOffsets_currentIndexChanged(int index);
    void on_checkBoxAddAlignment_toggled(bool checked);
    void on_checkBoxFixTypes_toggled(bool checked);

    void errorMessage(QString sText);

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;

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
    PDBProcess::PDBDATA g_pdbData;
    XOptions g_xOptions;
    QSortFilterProxyModel *g_pFilter;
};

#endif // GUIMAINWINDOW_H
