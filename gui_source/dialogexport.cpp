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
#include "dialogexport.h"
#include "ui_dialogexport.h"

DialogExport::DialogExport(QWidget *parent,PDBProcess::PDBDATA *pData) :
    QDialog(parent),
    ui(new Ui::DialogExport)
{
    ui->setupUi(this);

    this->pData=pData;

    ui->comboBoxExportType->setEnabled(false);

    if(pData->handleOptions.exportType==QWinPDB::ET_CPLUSPLUS)
    {
        ui->groupBoxSortType->setEnabled(true);
        ui->groupBoxOptions->setEnabled(true);
    }
    else if(pData->handleOptions.exportType==QWinPDB::ET_XNTSV)
    {
        ui->groupBoxSortType->setEnabled(false);
        ui->groupBoxOptions->setEnabled(false);
    }

    ui->comboBoxSortType->addItem(tr("No"),QWinPDB::ST_NO);
    ui->comboBoxSortType->addItem(QString("ID"),QWinPDB::ST_ID);
    ui->comboBoxSortType->addItem(tr("Name"),QWinPDB::ST_NAME);

    qint32 nCount=pData->stats.listSymbols.count();

    if(nCount<100000)
    {
        ui->comboBoxSortType->addItem(tr("Dependencies"),QWinPDB::ST_DEP);
    }

    ui->comboBoxExportType->addItem(QString("C++"),QWinPDB::ET_CPLUSPLUS);
    ui->comboBoxExportType->addItem(QString("XNTSV"),QWinPDB::ET_XNTSV);

    ui->comboBoxFixOffsets->addItem(tr("No"),QWinPDB::FO_NO);
    ui->comboBoxFixOffsets->addItem(tr("Struct and unions"),QWinPDB::FO_STRUCTSANDUNIONS);
    ui->comboBoxFixOffsets->addItem(tr("All"),QWinPDB::FO_ALL);

    ui->checkBoxShowComments->setChecked(pData->handleOptions.bShowComments);
    ui->checkBoxFixTypes->setChecked(pData->handleOptions.bFixTypes);
    ui->checkBoxAddAlignment->setChecked(pData->handleOptions.bAddAlignment);

    {
        int nCount=ui->comboBoxFixOffsets->count();

        for(int i=0;i<nCount;i++)
        {
            if(ui->comboBoxFixOffsets->itemData(i).toUInt()==pData->handleOptions.fixOffsets)
            {
                ui->comboBoxFixOffsets->setCurrentIndex(i);

                break;
            }
        }
    }
    {
        int nCount=ui->comboBoxSortType->count();

        for(int i=0;i<nCount;i++)
        {
            if(ui->comboBoxSortType->itemData(i).toUInt()==pData->handleOptions.sortType)
            {
                ui->comboBoxSortType->setCurrentIndex(i);

                break;
            }
        }
    }
    {
        int nCount=ui->comboBoxExportType->count();

        for(int i=0;i<nCount;i++)
        {
            if(ui->comboBoxExportType->itemData(i).toUInt()==pData->handleOptions.exportType)
            {
                ui->comboBoxExportType->setCurrentIndex(i);

                break;
            }
        }
    }
}

DialogExport::~DialogExport()
{
    delete ui;
}

QWinPDB::HANDLE_OPTIONS DialogExport::getHandleOptions()
{
    QWinPDB::HANDLE_OPTIONS result={};

    result.bShowComments=ui->checkBoxShowComments->isChecked();
    result.bFixTypes=ui->checkBoxFixTypes->isChecked();
    result.bAddAlignment=ui->checkBoxAddAlignment->isChecked();
    result.fixOffsets=(QWinPDB::FO)ui->comboBoxFixOffsets->currentData().toUInt();
    result.sortType=(QWinPDB::ST)ui->comboBoxSortType->currentData().toUInt();
    result.exportType=(QWinPDB::ET)ui->comboBoxExportType->currentData().toUInt();

    return result;
}

void DialogExport::on_pushButtonOK_clicked()
{
    pData->handleOptions=getHandleOptions();

    QString sFileName=QFileInfo(pData->sPDBFileName).completeBaseName();

    if(pData->handleOptions.exportType==QWinPDB::ET_CPLUSPLUS)
    {
        sFileName=QFileInfo(pData->sPDBFileName).filePath()+QDir::separator()+QString("%1.h").arg(sFileName);
        sFileName=QFileDialog::getSaveFileName(this,tr("Save file"),sFileName,QString("H %1 (*.h);;%2 (*)").arg(tr("Files"),tr("All files")));
    }
    else if(pData->handleOptions.exportType==QWinPDB::ET_XNTSV)
    {
        sFileName=QFileInfo(pData->sPDBFileName).filePath()+QDir::separator()+QString("%1.json").arg(sFileName);
        sFileName=QFileDialog::getSaveFileName(this,tr("Save file"),sFileName,QString("JSON %1 (*.json);;%2 (*)").arg(tr("Files"),tr("All files")));
    }

    if(!sFileName.isEmpty())
    {
        pData->handleOptions.sResultFileName=sFileName;

        DialogProcess dp(this,pData,PDBProcess::TYPE_EXPORT);
        connect(&dp,SIGNAL(errorMessage(QString)),this,SLOT(errorMessage(QString)));
        connect(&dp,SIGNAL(infoMessage(QString)),this,SLOT(infoMessage(QString)));
        dp.exec();

        this->close();
    }
}

void DialogExport::on_pushButtonCancel_clicked()
{
    this->close();
}

void DialogExport::errorMessage(QString sText)
{
    QMessageBox::critical(this,tr("Error"),sText);
}

void DialogExport::infoMessage(QString sText)
{
    QMessageBox::information(this,tr("Information"),sText);
}
