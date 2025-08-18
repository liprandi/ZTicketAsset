#include <QStandardItemModel>
#include <QPushButton>
#include "zStepswidget.h"
#include "ui_zstepswidget.h"
#include "zbackend.h"

ZStepsWidget::ZStepsWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ZStepsWidget)
{
    ui->setupUi(this);
}

ZStepsWidget::~ZStepsWidget()
{
    delete ui;
}

void ZStepsWidget::populate(){
    if(g_backend)
    {
        ui->StepTable->populate(g_backend->steps());
    }
}
