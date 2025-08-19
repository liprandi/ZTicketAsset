#include "zbackend.h"
#include "dlgeditasset.h"
#include "ui_dlgeditasset.h"

DlgEditAsset::DlgEditAsset(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DlgEditAsset)
{
    ui->setupUi(this);

    if(g_backend && g_backend->users().count() > 0){
        ui->users->clear();
        ui->users->addItem(tr("shared asset"));
        for(const auto u : g_backend->users())
        {
            if(u.isObject())
            {
                QJsonObject obj = u.toObject();
                ui->users->addItem(obj["nickname"].toString("."));
            }
        }
    }
    if(g_backend && g_backend->asset().contains("id")){
        const auto asset = g_backend->asset();
        ui->id->setText(asset["id"].toString("."));
        ui->model->setText(asset["model"].toString("."));
        ui->type->setCurrentText(asset["type"].toString("."));
        ui->description->setText(asset["description"].toString("."));
        ui->users->setCurrentText(asset["user"].toString("."));
    }
    else {
        ui->id->setText("");
        ui->model->setText("");
        ui->type->setCurrentIndex(-1);
        ui->description->setText("");
        ui->users->setCurrentIndex(-1);
    }
}

DlgEditAsset::~DlgEditAsset()
{
    delete ui;
}
