#include <QStandardItemModel>
#include <QPushButton>
#include "zTicketswidget.h"
#include "ui_zticketswidget.h"
#include "zbackend.h"

ZTicketsWidget::ZTicketsWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ZTicketsWidget)
{
    ui->setupUi(this);
}

ZTicketsWidget::~ZTicketsWidget()
{
    delete ui;
}

void ZTicketsWidget::populate(){
    if(g_backend)
    {
        ui->TicketTable->populate(g_backend->tickets());
    }
}
