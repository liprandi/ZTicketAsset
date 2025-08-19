#include <QStandardItemModel>
#include <QPushButton>
#include "zTicketswidget.h"
#include "ui_zticketswidget.h"
#include "zjsontablewidget.h"
#include "zbackend.h"

ZTicketsWidget::ZTicketsWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ZTicketsWidget)
{
    ui->setupUi(this);
    connect(ui->TicketTable, &ZJsonTableWidget::selectionChange, this, [this](int sel){
        if(g_backend){
            if(g_backend->selectTicket(sel))
                g_backend->readUsers();
        }
    });
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
