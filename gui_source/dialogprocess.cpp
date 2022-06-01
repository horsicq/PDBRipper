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
#include "dialogprocess.h"
#include "ui_dialogprocess.h"

DialogProcess::DialogProcess(QWidget *parent, PDBProcess::PDBDATA *pData, PDBProcess::TYPE type) :
    QDialog(parent),
    ui(new Ui::DialogProcess)
{
    ui->setupUi(this);

    g_pData=pData;

    if(type==PDBProcess::TYPE_EXPORT)
    {
        setWindowTitle(tr("Export"));
    }
    else if(type==PDBProcess::TYPE_IMPORT)
    {
        setWindowTitle(tr("Import"));
    }

    pThread=new QThread;

    pPDBProcess=new PDBProcess(0,pData,type);
    pPDBProcess->moveToThread(pThread);

    connect(pPDBProcess,SIGNAL(completed()),this,SLOT(onCompleted()));

    connect(pPDBProcess,SIGNAL(errorMessage(QString)),this,SIGNAL(errorMessage(QString)));
    connect(pPDBProcess,SIGNAL(infoMessage(QString)),this,SIGNAL(infoMessage(QString)));

    bIsRun=true;
    connect(pThread, SIGNAL(started()), pPDBProcess, SLOT(process()));
    pThread->start();

    g_pTimer=new QTimer(this);
    connect(g_pTimer,SIGNAL(timeout()),this,SLOT(timerSlot()));
    g_pTimer->start(1000);

    nReturnCode=QDialog::Accepted;

    ui->progressBarTotal->setMaximum(100);
    ui->progressBarTotal->setValue(0);
}

DialogProcess::~DialogProcess()
{
    if(bIsRun)
    {
        pPDBProcess->stop();
    }

    pThread->quit();
    pThread->wait();

    delete ui;

    delete pThread;
    delete pPDBProcess;
}

void DialogProcess::on_pushButtonCancel_clicked()
{
    nReturnCode=QDialog::Rejected;

    if(bIsRun)
    {
        pPDBProcess->stop();
        bIsRun=false;
    }
}

void DialogProcess::timerSlot()
{
    if(g_pData)
    {
        if(g_pData->stats.nTotal)
        {
            ui->progressBarTotal->setMaximum(g_pData->stats.nTotal);
            ui->progressBarTotal->setValue(g_pData->stats.nCurrent);
            ui->labelStatus->setText(g_pData->stats.sStatus);
        }
    }
}

void DialogProcess::onCompleted()
{
    done(nReturnCode);
}
