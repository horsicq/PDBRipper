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
#include "pdbprocess.h"

PDBProcess::PDBProcess(QObject *parent, PDBDATA *pData,TYPE type) : QObject(parent)
{
    this->g_pData=pData;
    this->g_type=type;

    connect(pData->pWinPDB,SIGNAL(completed()),this,SIGNAL(completed()));
    connect(pData->pWinPDB,SIGNAL(setProgressMinimum(int)),this,SIGNAL(setProgressMinimum(int)));
    connect(pData->pWinPDB,SIGNAL(setProgressMaximum(int)),this,SIGNAL(setProgressMaximum(int)));
    connect(pData->pWinPDB,SIGNAL(setProgressValue(int)),this,SIGNAL(setProgressValue(int)));
}

void PDBProcess::process()
{
    if(g_type==TYPE_IMPORT)
    {
        g_pData->stats=g_pData->pWinPDB->getStats();
    }
    else if(g_type==TYPE_EXPORT)
    {
        g_pData->sString=g_pData->pWinPDB->exportString(&(g_pData->stats),&(g_pData->handleOptions));

        if(g_pData->handleOptions.sResultFileName!="")
        {
            QFile file;
            file.setFileName(g_pData->handleOptions.sResultFileName);

            if(file.open(QIODevice::ReadWrite))
            {
                file.resize(0);
                file.write(g_pData->sString.toLatin1().data(),g_pData->sString.length());
                file.close();

                emit infoMessage(QString("%1: %2").arg(tr("File saved")).arg(g_pData->handleOptions.sResultFileName));
            }
            else
            {
                emit errorMessage(QString("%1: %2").arg(tr("Cannot save file")).arg(g_pData->handleOptions.sResultFileName));
            }
        }
    }
}

void PDBProcess::stop()
{
    g_pData->pWinPDB->stop();
}
