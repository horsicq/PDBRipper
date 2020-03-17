// copyright (c) 2017-2018 hors<horsicq@gmail.com>
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
#include "dialogprocess.h"
#include "ui_dialogprocess.h"

DialogProcess::DialogProcess(QWidget *parent, PDBProcess::PDBDATA *pData, PDBProcess::TYPE type) :
    QDialog(parent),
    ui(new Ui::DialogProcess)
{
    ui->setupUi(this);

    thread=new QThread;

    pPDBProcess=new PDBProcess(0,pData,type);
    pPDBProcess->moveToThread(thread);

    connect(pPDBProcess, SIGNAL(completed()), this, SLOT(onCompleted()));
    connect(pPDBProcess, SIGNAL(setProgressMaximum(int)), this, SLOT(onSetProgressMaximum(int)));
    connect(pPDBProcess, SIGNAL(setProgressValue(int)), this, SLOT(onSetProgressValue(int)));

    bIsRun=true;
    connect(thread, SIGNAL(started()), pPDBProcess, SLOT(process()));
    thread->start();
}

DialogProcess::~DialogProcess()
{
    if(bIsRun)
    {
        pPDBProcess->stop();
    }

    thread->quit();
    thread->wait();

    delete ui;
    delete thread;
}

void DialogProcess::on_pushButtonCancel_clicked()
{
    if(bIsRun)
    {
        pPDBProcess->stop();
        bIsRun=false;
    }
}

void DialogProcess::onCompleted()
{
    this->close();
}

void DialogProcess::onSetProgressMaximum(int nMax)
{
    ui->progressBarTotal->setMaximum(nMax);
}

void DialogProcess::onSetProgressValue(int nValue)
{
    ui->progressBarTotal->setValue(nValue);
}
