#include <QStandardItemModel>
#include <QPushButton>
#include "zassetswidget.h"
#include "ui_zassetswidget.h"
#include "zbackend.h"

ZAssetsWidget::ZAssetsWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ZAssetsWidget)
{
    ui->setupUi(this);
}

ZAssetsWidget::~ZAssetsWidget()
{
    delete ui;
}

void ZAssetsWidget::populate(){
    if(g_backend)
    {
        ui->assetTable->populate(g_backend->assets());
    }
}
