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
    connect(ui->StepTable, &ZJsonTableWidget::selectionChange, this, [this](int sel){
        if(g_backend){
            if(g_backend->selectStep(sel))
                g_backend->readUsers();
        }
    });
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
