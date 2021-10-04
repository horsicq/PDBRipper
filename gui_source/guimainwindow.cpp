// copyright (c) 2020-2021 hors<horsicq@gmail.com>
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

    setWindowTitle(QString("%1 v%2").arg(X_APPLICATIONDISPLAYNAME,X_APPLICATIONVERSION));

    g_xOptions.setName(X_OPTIONSFILE);

    QList<XOptions::ID> listIDs;

    listIDs.append(XOptions::ID_STYLE);
    listIDs.append(XOptions::ID_SAVELASTDIRECTORY);
    listIDs.append(XOptions::ID_STAYONTOP);

    g_xOptions.setValueIDs(listIDs);
    g_xOptions.load();

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
        _openFile(QCoreApplication::arguments().at(1));
    }
}

GuiMainWindow::~GuiMainWindow()
{
    g_xOptions.save();

    if(pdbData.pWinPDB)
    {
        delete pdbData.pWinPDB;
    }

    delete ui;
}

void GuiMainWindow::on_actionOpen_triggered()
{
    QString sDirectory=g_xOptions.getLastDirectory();

    QString sFileName=QFileDialog::getOpenFileName(this,tr("Open file"),sDirectory, QString("PDB %1 (*.pdb);;%2 (*)").arg(tr("Files"),tr("All files")));

    _openFile(sFileName);
}

void GuiMainWindow::on_actionOptions_triggered()
{
    DialogOptions dialogOptions(this,&g_xOptions);
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
    g_xOptions.adjustStayOnTop(this);
}

void GuiMainWindow::_openFile(QString sFileName)
{
    if((sFileName!="")&&(QFileInfo(sFileName).isFile()))
    {
        g_xOptions.setLastDirectory(QFileInfo(sFileName).absolutePath());

        if(pdbData.pWinPDB)
        {
            delete pdbData.pWinPDB;
        }

        pdbData.pWinPDB=new QWinPDB;

        connect(pdbData.pWinPDB,SIGNAL(errorMessage(QString)),this,SLOT(errorMessage(QString)));

        QAbstractItemModel *pOldModel=pFilter->sourceModel();

        pFilter->setSourceModel(0);

        delete pOldModel;

        ui->lineEditSearch->clear();
        ui->textBrowserResult->clear();

        if(pdbData.pWinPDB->loadFromFile(sFileName))
        {
            pdbData.sPDBFileName=sFileName;

            DialogProcess dp(this,&pdbData,PDBProcess::TYPE_IMPORT);
            dp.exec();

            int nCount=pdbData.stats.listSymbols.count();

            QStandardItemModel *pModel=new QStandardItemModel(nCount,2,this);

            pModel->setHeaderData(0,Qt::Horizontal,tr("ID"));
            pModel->setHeaderData(1,Qt::Horizontal,tr("Symbol"));

            for(int i = 0; i<nCount; i++)
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
        }
        else
        {
            QMessageBox::critical(this,tr("Error"),QString("%1: %2").arg(tr("Cannot open file")).arg(sFileName));
        }
    }
}

void GuiMainWindow::on_lineEditSearch_textChanged(const QString &arg1)
{
    pFilter->setFilterRegExp(arg1);
    pFilter->setFilterCaseSensitivity(Qt::CaseInsensitive);
    pFilter->setFilterKeyColumn(1);
}

void GuiMainWindow::onCurrentChanged(const QModelIndex &current, const QModelIndex &previous)
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

void GuiMainWindow::on_actionCPP_triggered()
{
    if(pdbData.pWinPDB)
    {
        pdbData.handleOptions.bAddAlignment=false;
        pdbData.handleOptions.bFixTypes=false;
        pdbData.handleOptions.bShowComments=false;
        pdbData.handleOptions.fixOffsets=QWinPDB::FO_NO;
        pdbData.handleOptions.sortType=QWinPDB::ST_ID;
        pdbData.handleOptions.exportType=QWinPDB::ET_CPLUSPLUS;

        DialogExport dialogExport(this,&pdbData);

        dialogExport.exec();
    }
}

void GuiMainWindow::on_actionXNTSV_triggered()
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

void GuiMainWindow::on_checkBoxFixTypes_toggled(bool checked)
{
    Q_UNUSED(checked)

    handle();
}

void GuiMainWindow::on_actionQuit_triggered()
{
    this->close();
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

            _openFile(sFileName);
        }
    }
}
