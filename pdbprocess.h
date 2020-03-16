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
#ifndef PDBPROCESS_H
#define PDBPROCESS_H

#include <QObject>
#include "qwinpdb.h"

class PDBProcess : public QObject
{
    Q_OBJECT

public:
    enum TYPE
    {
        TYPE_IMPORT=0,
        TYPE_EXPORT
    };

    explicit PDBProcess(QObject *parent, QWinPDB *pWinPDB, QWinPDB::STATS *pStats, QString *psString, TYPE type);
    void stop();

signals:
    void completed();
    void setProgressMaximum(int);
    void setProgressValue(int);

public slots:
    void process();

private:
    QWinPDB *pWinPDB;
    QWinPDB::STATS *pStats;
    QString *psString;
    TYPE type;
};

#endif // PDBPROCESS_H
