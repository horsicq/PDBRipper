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
#ifndef DIALOGPROCESS_H
#define DIALOGPROCESS_H

#include <QDialog>
#include <QThread>
#include <QWidget>
#include "../pdbprocess.h"

namespace Ui
{
class DialogProcess;
}

class DialogProcess : public QDialog
{
    Q_OBJECT

public:
    explicit DialogProcess(QWidget *parent,PDBProcess::PDBDATA *pData,PDBProcess::TYPE type);
    ~DialogProcess();

private slots:
    void on_pushButtonCancel_clicked();

public slots:
    void onCompleted();
    void onSetProgressMinimum(int nMin);
    void onSetProgressMaximum(int nMax);
    void onSetProgressValue(int nValue);

signals:
    void errorMessage(QString sText);
    void infoMessage(QString sText);

private:
    Ui::DialogProcess *ui;
    QThread *thread;
    PDBProcess *pPDBProcess;
    bool bIsRun;
    int nReturnCode;
};

#endif // DIALOGPROCESS_H
