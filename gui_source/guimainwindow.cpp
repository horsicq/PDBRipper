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
#include "guimainwindow.h"
#include "ui_guimainwindow.h"

GuiMainWindow::GuiMainWindow(QWidget *pParent) :
    QMainWindow(pParent),
    ui(new Ui::GuiMainWindow)
{
    ui->setupUi(this);

    setWindowTitle(QString("%1 v%2").arg(X_APPLICATIONDISPLAYNAME,X_APPLICATIONVERSION));

    g_xOptions.setName(X_OPTIONSFILE);

    g_xOptions.addID(XOptions::ID_VIEW_STYLE,"Fusion");
//    g_xOptions.addID(XOptions::ID_VIEW_QSS,"");
    g_xOptions.addID(XOptions::ID_VIEW_LANG,"System");
    g_xOptions.addID(XOptions::ID_VIEW_STAYONTOP,false);
    g_xOptions.addID(XOptions::ID_FILE_SAVELASTDIRECTORY,true);
    g_xOptions.addID(XOptions::ID_FILE_SAVERECENTFILES,true);
    g_xOptions.addID(XOptions::ID_VIEW_FONT,"");
//    g_xOptions.addID(XOptions::ID_FILE_CONTEXT,".pdb");

    g_xOptions.load();

    connect(&g_xOptions,SIGNAL(openFile(QString)),this,SLOT(processFile(QString)));

    createMenus();

    adjustWindow();

    setAcceptDrops(true);

    pdbData.pWinPDB=0;

    pFilter=new QSortFilterProxyModel(this);
    ui->tableViewSymbols->setModel(pFilter);

    QSignalBlocker blocker(ui->comboBoxFixOffsets);

    ui->comboBoxFixOffsets->addItem(tr("No"),QWinPDB::FO_NO);
    ui->comboBoxFixOffsets->addItem(tr("Struct and unions"),QWinPDB::FO_STRUCTSANDUNIONS);
    ui->comboBoxFixOffsets->addItem(tr("All"),QWinPDB::FO_ALL);

    pdbData.handleOptions=QWinPDB::getDefaultHandleOptions();

    setHandleOptions(&(pdbData.handleOptions));

    if(QCoreApplication::arguments().count()>1)
    {
        processFile(QCoreApplication::arguments().at(1));
    }
}

GuiMainWindow::~GuiMainWindow()
{
    g_xOptions.save();

    if(pdbData.pWinPDB)
    {
        delete pdbData.pWinPDB;
        pdbData.pWinPDB=nullptr;
    }

    delete ui;
}

void GuiMainWindow::createMenus()
{
    QMenu *pMenuFile=new QMenu(tr("File"),ui->menubar);
    QMenu *pMenuAction=new QMenu(tr("Action"),ui->menubar);
    QMenu *pMenuTools=new QMenu(tr("Tools"),ui->menubar);
    QMenu *pMenuHelp=new QMenu(tr("Help"),ui->menubar);
    QMenu *pMenuExport=new QMenu(tr("Export"),ui->menubar);

    ui->menubar->addAction(pMenuFile->menuAction());
    ui->menubar->addAction(pMenuAction->menuAction());
    ui->menubar->addAction(pMenuTools->menuAction());
    ui->menubar->addAction(pMenuHelp->menuAction());

    QAction *pActionOpen=new QAction(tr("Open"),this);
    QAction *pActionClose=new QAction(tr("Close"),this);
    QAction *pActionExit=new QAction(tr("Exit"),this);
    QAction *pActionExportCPP=new QAction(QString("C++"),this);
    QAction *pActionExportXNTSV=new QAction(QString("XNTSV"),this);
    QAction *pActionOptions=new QAction(tr("Options"),this);
    QAction *pActionAbout=new QAction(tr("About"),this);

    pMenuFile->addAction(pActionOpen);
    pMenuFile->addMenu(g_xOptions.createRecentFilesMenu(this));
    pMenuFile->addAction(pActionClose);
    pMenuFile->addAction(pActionExit);
    pMenuExport->addAction(pActionExportCPP);
    pMenuExport->addAction(pActionExportXNTSV);
    pMenuAction->addMenu(pMenuExport);
    pMenuTools->addAction(pActionOptions);
    pMenuHelp->addAction(pActionAbout);

    connect(pActionOpen,SIGNAL(triggered()),this,SLOT(actionOpenSlot()));
    connect(pActionClose,SIGNAL(triggered()),this,SLOT(actionCloseSlot()));
    connect(pActionExit,SIGNAL(triggered()),this,SLOT(actionExitSlot()));
    connect(pActionExportCPP,SIGNAL(triggered()),this,SLOT(actionCPPSlot()));
    connect(pActionExportXNTSV,SIGNAL(triggered()),this,SLOT(actionXNTSVSlot()));
    connect(pActionOptions,SIGNAL(triggered()),this,SLOT(actionOptionsSlot()));
    connect(pActionAbout,SIGNAL(triggered()),this,SLOT(actionAboutSlot()));
}

void GuiMainWindow::actionOpenSlot()
{
    QString sDirectory=g_xOptions.getLastDirectory();

    QString sFileName=QFileDialog::getOpenFileName(this,tr("Open file"),sDirectory,QString("PDB %1 (*.pdb);;%2 (*)").arg(tr("Files"),tr("All files")));

    processFile(sFileName);
}

void GuiMainWindow::actionCloseSlot()
{
    cleanUp();
}

void GuiMainWindow::actionExitSlot()
{
    this->close();
}

void GuiMainWindow::setHandleOptions(QWinPDB::HANDLE_OPTIONS *pHandleOptions)
{
    ui->checkBoxShowComments->setChecked(pHandleOptions->bShowComments);
    ui->checkBoxFixTypes->setChecked(pHandleOptions->bFixTypes);
    ui->checkBoxAddAlignment->setChecked(pHandleOptions->bAddAlignment);

    int nCount=ui->comboBoxFixOffsets->count();

    for(int i=0;i<nCount;i++)
    {
        if(ui->comboBoxFixOffsets->itemData(i).toUInt()==pHandleOptions->fixOffsets)
        {
            ui->comboBoxFixOffsets->setCurrentIndex(i);

            break;
        }
    }
}

QWinPDB::HANDLE_OPTIONS GuiMainWindow::getHandleOptions()
{
    QWinPDB::HANDLE_OPTIONS result={};

    result.bShowComments=ui->checkBoxShowComments->isChecked();
    result.bFixTypes=ui->checkBoxFixTypes->isChecked();
    result.bAddAlignment=ui->checkBoxAddAlignment->isChecked();
    result.fixOffsets=(QWinPDB::FO)ui->comboBoxFixOffsets->currentData().toUInt();

    return result;
}

void GuiMainWindow::on_comboBoxFixOffsets_currentIndexChanged(int index)
{
    Q_UNUSED(index);

    handle();
}

void GuiMainWindow::on_checkBoxAddAlignment_toggled(bool checked)
{
    Q_UNUSED(checked)

    handle();
}

void GuiMainWindow::actionCPPSlot()
{
    if(pdbData.pWinPDB)
    {
        pdbData.handleOptions.bAddAlignment=false;
        pdbData.handleOptions.bFixTypes=false;
        pdbData.handleOptions.bShowComments=false;
        pdbData.handleOptions.fixOffsets=QWinPDB::FO_NO;
        pdbData.handleOptions.sortType=QWinPDB::ST_NO;
        pdbData.handleOptions.exportType=QWinPDB::ET_CPLUSPLUS;

        DialogExport dialogExport(this,&pdbData);

        dialogExport.exec();
    }
}

void GuiMainWindow::actionXNTSVSlot()
{
    if(pdbData.pWinPDB)
    {
        pdbData.handleOptions.bAddAlignment=false;
        pdbData.handleOptions.bFixTypes=true;
        pdbData.handleOptions.bShowComments=false;
        pdbData.handleOptions.fixOffsets=QWinPDB::FO_NO;
        pdbData.handleOptions.sortType=QWinPDB::ST_NAME;
        pdbData.handleOptions.exportType=QWinPDB::ET_XNTSV;

        DialogExport dialogExport(this,&pdbData);

        dialogExport.exec();
    }
}

void GuiMainWindow::actionOptionsSlot()
{
    DialogOptions dialogOptions(this,&g_xOptions);
    dialogOptions.exec();

    adjustWindow();
}

void GuiMainWindow::actionAboutSlot()
{
    DialogAbout dialogAbout(this);

    dialogAbout.exec();
}

void GuiMainWindow::adjustWindow()
{
    g_xOptions.adjustWindow(this);
}

void GuiMainWindow::processFile(QString sFileName)
{
    if((sFileName!="")&&(QFileInfo(sFileName).isFile()))
    {
        g_xOptions.setLastFileName(sFileName);

        cleanUp();

        pdbData.pWinPDB=new QWinPDB;

        connect(pdbData.pWinPDB,SIGNAL(errorMessage(QString)),this,SLOT(errorMessage(QString)));

        if(pdbData.pWinPDB->loadFromFile(sFileName))
        {
            pdbData.sPDBFileName=sFileName;

            DialogProcess dp(this,&pdbData,PDBProcess::TYPE_IMPORT);
            dp.exec();

            int nCount=pdbData.stats.listSymbols.count();

            QStandardItemModel *pModel=new QStandardItemModel(nCount,2,this);

            pModel->setHeaderData(0,Qt::Horizontal,QString("ID"));
            pModel->setHeaderData(1,Qt::Horizontal,tr("Symbol"));

            for(int i = 0;i<nCount;i++)
            {
                QStandardItem *itemID = new QStandardItem;
                itemID->setData((quint32)(pdbData.stats.listSymbols.at(i).dwID),Qt::DisplayRole);
                itemID->setData((quint32)(pdbData.stats.listSymbols.at(i).type),Qt::UserRole+1);
                itemID->setTextAlignment(Qt::AlignRight);
                pModel->setItem(i,0,itemID);

                QStandardItem *itemSymbol = new QStandardItem;
                itemSymbol->setText(pdbData.stats.listSymbols.at(i).sName);
                pModel->setItem(i,1,itemSymbol);
            }

//            ui->tableViewSymbols->setModel(model);

            pFilter->setSourceModel(pModel);

            ui->tableViewSymbols->horizontalHeader()->setSectionResizeMode(0,QHeaderView::ResizeToContents);
            ui->tableViewSymbols->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);

            connect(ui->tableViewSymbols->selectionModel(),SIGNAL(currentChanged(QModelIndex const&,QModelIndex const&)),this,SLOT(onCurrentChanged(QModelIndex const&,QModelIndex const&)));

            setWindowTitle(sFileName);
        }
        else
        {
            QMessageBox::critical(this,tr("Error"),QString("%1: %2").arg(tr("Cannot open file")).arg(sFileName));
        }
    }
}

void GuiMainWindow::cleanUp()
{
    if(pdbData.pWinPDB)
    {
        delete pdbData.pWinPDB;
        pdbData.pWinPDB=nullptr;
    }

    QAbstractItemModel *pOldModel=pFilter->sourceModel();

    pFilter->setSourceModel(0);

    delete pOldModel;

    ui->lineEditSearch->clear();
    ui->textBrowserResult->clear();
}

void GuiMainWindow::on_lineEditSearch_textChanged(const QString &arg1)
{
    pFilter->setFilterRegExp(arg1);
    pFilter->setFilterCaseSensitivity(Qt::CaseInsensitive);
    pFilter->setFilterKeyColumn(1);
}

void GuiMainWindow::onCurrentChanged(const QModelIndex &current,const QModelIndex &previous)
{
    Q_UNUSED(current)
    Q_UNUSED(previous)

    handle();
}

void GuiMainWindow::handle()
{
    QItemSelectionModel *pSelectionModel=ui->tableViewSymbols->selectionModel();

    if(pSelectionModel)
    {
        QModelIndexList list=pSelectionModel->selection().indexes();

        if(list.count())
        {
            pdbData.handleOptions=getHandleOptions();
            quint32 nID=list.at(0).data(Qt::DisplayRole).toUInt();

            QWinPDB::ELEM_INFO elemInfo=pdbData.pWinPDB->handleElement(nID,&(pdbData.handleOptions));

            QString sText=elemInfo.sText;

            ui->textBrowserResult->setText(sText);
        }
    }
}

void GuiMainWindow::on_tableViewSymbols_clicked(const QModelIndex &index)
{
    Q_UNUSED(index)

    handle();
}

void GuiMainWindow::on_checkBoxShowComments_toggled(bool checked)
{
    Q_UNUSED(checked)

    handle();
}

void GuiMainWindow::on_checkBoxFixTypes_toggled(bool checked)
{
    Q_UNUSED(checked)

    handle();
}

void GuiMainWindow::errorMessage(QString sText)
{
    QMessageBox::critical(this,tr("Error"),sText);
}

void GuiMainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}

void GuiMainWindow::dragMoveEvent(QDragMoveEvent *event)
{
    event->acceptProposedAction();
}

void GuiMainWindow::dropEvent(QDropEvent *event)
{
    const QMimeData* mimeData=event->mimeData();

    if(mimeData->hasUrls())
    {
        QList<QUrl> urlList=mimeData->urls();

        if(urlList.count())
        {
            QString sFileName=urlList.at(0).toLocalFile();

            QFileInfo fiLink(sFileName);

            if(fiLink.isSymLink())
            {
                sFileName=fiLink.symLinkTarget();
            }

            processFile(sFileName);
        }
    }
}
