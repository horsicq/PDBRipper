// Copyright (c) 2020 hors<horsicq@gmail.com>
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
#include <QCoreApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include "consoleoutput.h"
#include "../pdbprocess.h"
#include "../global.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName(X_ORGANIZATIONNAME);
    QCoreApplication::setOrganizationDomain(X_ORGANIZATIONDOMAIN);
    QCoreApplication::setApplicationName(X_APPLICATIONNAME);
    QCoreApplication::setApplicationVersion(X_APPLICATIONVERSION);

    QCoreApplication app(argc, argv);

    ConsoleOutput consoleOutput;
    QCommandLineParser parser;
    QString sDescription;
    sDescription.append(QString("%1 v%2\n").arg(X_APPLICATIONNAME).arg(X_APPLICATIONVERSION));
    sDescription.append(QString("%1\n").arg("Copyright(C) 2012-2020 hors<horsicq@gmail.com> Web: http://ntinfo.biz"));
    parser.setApplicationDescription(sDescription);
    parser.addHelpOption();
    parser.addVersionOption();

    parser.addPositionalArgument("file","The file to open.");

    QCommandLineOption clOutputFile                 (QStringList()<<"o"<<"outputfile",          "Output file<path>.",                           "path");
    QCommandLineOption clShowComments               (QStringList()<<"c"<<"showcomments",        "Show comments."                                );
    QCommandLineOption clFixTypes                   (QStringList()<<"f"<<"fixtypes",            "Fix types."                                    );
    QCommandLineOption clAddAlignment               (QStringList()<<"l"<<"addalignment",        "Add alignment."                                );
    QCommandLineOption clFixOffsetsStructs          (QStringList()<<"s"<<"fostructsandunions",  "Fix offsets(structs and unions)."              );
    QCommandLineOption clFixOffsetsAll              (QStringList()<<"a"<<"foall",               "Fix offsets(all)."                             );
    QCommandLineOption clSortById                   (QStringList()<<"i"<<"sortbyid",            "Sort by ID."                                   );
    QCommandLineOption clSortByName                 (QStringList()<<"n"<<"sortbyname",          "Sort by name."                                 );
    QCommandLineOption clSortByDeps                 (QStringList()<<"d"<<"sortbydeps",          "Sort by dependencies."                         );
    QCommandLineOption clExportCpp                  (QStringList()<<"p"<<"exportcpp",           "Export C++."                                   );

    parser.addOption(clOutputFile);
    parser.addOption(clShowComments);
    parser.addOption(clFixTypes);
    parser.addOption(clAddAlignment);
    parser.addOption(clFixOffsetsStructs);
    parser.addOption(clFixOffsetsAll);
    parser.addOption(clSortById);
    parser.addOption(clSortByName);
    parser.addOption(clSortByDeps);
    parser.addOption(clExportCpp);

    parser.process(app);

    bool bProcess=false;

    PDBProcess::PDBDATA pdbData={};

    pdbData.pWinPDB=new QWinPDB;

    QObject::connect(pdbData.pWinPDB,SIGNAL(infoMessage(QString)),&consoleOutput,SLOT(infoMessage(QString)));
    QObject::connect(pdbData.pWinPDB,SIGNAL(errorMessage(QString)),&consoleOutput,SLOT(errorMessage(QString)));

    pdbData.handleOptions.bAddAlignment=parser.isSet(clAddAlignment);
    pdbData.handleOptions.bFixTypes=parser.isSet(clFixTypes);
    pdbData.handleOptions.bShowComments=parser.isSet(clShowComments);

    if(parser.isSet(clFixOffsetsStructs))
    {
        pdbData.handleOptions.fixOffsets=QWinPDB::FO_STRUCTSANDUNIONS;
    }
    else if(parser.isSet(clFixOffsetsAll))
    {
        pdbData.handleOptions.fixOffsets=QWinPDB::FO_ALL;
    }
    else
    {
        pdbData.handleOptions.fixOffsets=QWinPDB::FO_NO; // Default
    }

    if(parser.isSet(clSortById))
    {
        pdbData.handleOptions.sortType=QWinPDB::ST_ID;
    }
    else if(parser.isSet(clSortByName))
    {
        pdbData.handleOptions.sortType=QWinPDB::ST_NAME;
    }
    else if(parser.isSet(clSortByDeps))
    {
        pdbData.handleOptions.sortType=QWinPDB::ST_DEP;
    }
    else
    {
        pdbData.handleOptions.sortType=QWinPDB::ST_ID; // Default
    }

    if(parser.isSet(clExportCpp))
    {
        pdbData.handleOptions.exportType=QWinPDB::ET_CPLUSPLUS;
    }
    else
    {
        pdbData.handleOptions.exportType=QWinPDB::ET_CPLUSPLUS; // Default
    }

    QString sOutputFileName=parser.value(clOutputFile);

    // mb TODO multifiles
    QList<QString> listInputFiles=parser.positionalArguments();

    if(listInputFiles.count())
    {
        pdbData.sResultFileName=sOutputFileName;

        QString sPDBFileName=listInputFiles.at(0);

        if(pdbData.pWinPDB->loadFromFile(sPDBFileName))
        {
            {
                PDBProcess pdbProcess(nullptr,&pdbData,PDBProcess::TYPE_IMPORT);
                pdbProcess.process();
            }
            {
                PDBProcess pdbProcess(nullptr,&pdbData,PDBProcess::TYPE_EXPORT);

                QObject::connect(&pdbProcess,SIGNAL(infoMessage(QString)),&consoleOutput,SLOT(infoMessage(QString)));
                QObject::connect(&pdbProcess,SIGNAL(errorMessage(QString)),&consoleOutput,SLOT(errorMessage(QString)));

                pdbProcess.process();
            }

            if(sOutputFileName=="")
            {
                consoleOutput.infoMessage(pdbData.sString);
            }
        }
        else
        {
            consoleOutput.errorMessage(QString("%1: %2").arg(QString("Cannot open file")).arg(sPDBFileName));
        }

        bProcess=true;
    }

    if(!bProcess)
    {
        parser.showHelp();
        Q_UNREACHABLE();
    }

    return 0;
}
