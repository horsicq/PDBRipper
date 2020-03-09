#include "dialogexport.h"
#include "ui_dialogexport.h"

DialogExport::DialogExport(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogExport)
{
    ui->setupUi(this);
}

DialogExport::~DialogExport()
{
    delete ui;
}
