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
#include "guimainwindow.h"
#include "ui_guimainwindow.h"

GuiMainWindow::GuiMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GuiMainWindow)
{
    ui->setupUi(this);

    setWindowTitle(QString("%1 v%2").arg(X_APPLICATIONNAME).arg(X_APPLICATIONVERSION));

    pWinPDB=0;

    DialogOptions::loadOptions(&options);
    adjustWindow();
}

GuiMainWindow::~GuiMainWindow()
{
    DialogOptions::saveOptions(&options);
    delete ui;
}

void GuiMainWindow::on_actionOpen_triggered()
{
    QString sFileName=QFileDialog::getOpenFileName(this, tr("Open File..."),QString(), tr("PDB-Files (*.pdb);;All Files (*)"));

    if(!sFileName.isEmpty())
    {
        if(pWinPDB)
        {
            delete pWinPDB;
        }

        pWinPDB =new QWinPDB;

        if(pWinPDB->loadFromFile(sFileName))
        {
            DialogProcess dp(this,pWinPDB);
            dp.getStats(&stats);
            dp.exec();

            // TODO clear

            ui->comboBoxType->clear();

            if(stats.listClasses.count())
            {
                ui->comboBoxType->addItem(tr("Classes"),CBT_CLASSES);
            }
            if(stats.listStructs.count())
            {
                ui->comboBoxType->addItem(tr("Structs"),CBT_STRUCTS);
            }
            if(stats.listUnions.count())
            {
                ui->comboBoxType->addItem(tr("Unions"),CBT_UNIONS);
            }
            if(stats.listInterfaces.count())
            {
                ui->comboBoxType->addItem(tr("Interfaces"),CBT_INTERFACES);
            }
            if(stats.listEnums.count())
            {
                ui->comboBoxType->addItem(tr("Enums"),CBT_ENUMS);
            }
        }
        else
        {
            // TODO
        }
    }
}

void GuiMainWindow::on_listWidgetSymTags_currentRowChanged(int currentRow)
{
    handle();
}

void GuiMainWindow::on_actionC_C_triggered()
{
    QString sFileName=QFileDialog::getSaveFileName(this, tr("Save File..."),"export.h", tr("h-Files (*.h);;All Files (*)"));

    if(!sFileName.isEmpty())
    {
        QString sString; // TODO
        QFile file;
        file.setFileName(sFileName);

        if(file.open(QIODevice::ReadWrite))
        {
            file.resize(0);
            file.write(sString.toLatin1().data(),sString.length());
            file.close();
            QMessageBox::information(0, "Informational", QString("File saved: %1").arg(sFileName));
        }
        else
        {
            QMessageBox::critical(0, "Critical",QString("Cannot save file: %1").arg(sFileName));
        }
    }
}

void GuiMainWindow::on_comboBoxType_currentIndexChanged(int index)
{
    if(index!=-1)
    {
        int nType=ui->comboBoxType->currentData().toInt();
        QList<QWinPDB::PDB_RECORD> listRecords;

        if(nType==CBT_CLASSES)              listRecords=stats.listClasses;
        else if(nType==CBT_STRUCTS)         listRecords=stats.listStructs;
        else if(nType==CBT_UNIONS)          listRecords=stats.listUnions;
        else if(nType==CBT_INTERFACES)      listRecords=stats.listInterfaces;
        else if(nType==CBT_ENUMS)           listRecords=stats.listEnums;

        ui->listWidgetSymTags->clear();

        int nCount=listRecords.count();
        for(int i=0;i<nCount;i++)
        {
            QListWidgetItem *item=new QListWidgetItem(ui->listWidgetSymTags);
            item->setText(listRecords.at(i).sName);
            item->setData(Qt::UserRole,(int)listRecords.at(i).dwID);

            ui->listWidgetSymTags->addItem(item);
        }
    }
}

void GuiMainWindow::handle()
{
    int currentRow=ui->listWidgetSymTags->currentRow();

    if(currentRow!=-1)
    {
        int nID=ui->listWidgetSymTags->currentItem()->data(Qt::UserRole).toInt();
        QWinPDB::HANDLE_OPTIONS handleOptions={0};

//        handleOptions.bOffsets=ui->checkBoxOffsets->isChecked();
//        handleOptions.bSizes=ui->checkBoxSizes->isChecked();

//        QString sResult=pWinPDB->handle(nID,&handleOptions);

//        ui->textBrowserResult->setText(sResult);
        QWinPDB::ELEM elem=pWinPDB->getElem(nID);

        QString sText=QWinPDB::elemToString(&elem,&handleOptions,0,false);

        ui->textBrowserResult->setText(sText);
    }
}

//void MainWindow::on_checkBoxOffsets_toggled(bool checked)
//{
//    handle();
//}

//void MainWindow::on_checkBoxSizes_toggled(bool checked)
//{
//    handle();
//}

void GuiMainWindow::on_actionOptions_triggered()
{
    DialogOptions dialogOptions(this,&options);
    dialogOptions.exec();

    adjustWindow();
}

void GuiMainWindow::on_actionAbout_triggered()
{
    DialogAbout dialogAbout(this);

    dialogAbout.exec();
}

void GuiMainWindow::adjustWindow()
{
    Qt::WindowFlags wf=windowFlags();
    if(options.bStayOnTop)
    {
        wf|=Qt::WindowStaysOnTopHint;
    }
    else
    {
        wf&=~(Qt::WindowStaysOnTopHint);
    }
    setWindowFlags(wf);

    show();
}
