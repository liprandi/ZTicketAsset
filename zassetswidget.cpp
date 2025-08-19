#include <QStandardItemModel>
#include <QPushButton>
#include "zassetswidget.h"
#include "ui_zassetswidget.h"
#include "zbackend.h"
#include "dlgeditasset.h"

ZAssetsWidget::ZAssetsWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ZAssetsWidget)
{
    ui->setupUi(this);
    connect(ui->assetTable, &ZJsonTableWidget::selectionChange, this, [this](int sel){
        if(g_backend){
            if(g_backend->selectAsset(sel))
                g_backend->readUsers();
        }
    });
    connect(ui->pbAddEdit, &QPushButton::clicked, this, [this](bool checked){
        if(g_backend){
            DlgEditAsset dlg;
            dlg.exec();
        }
    });
}

ZAssetsWidget::~ZAssetsWidget()
{
    delete ui;
}

void ZAssetsWidget::populate(){
    bool ok = false;
    if(g_backend)
    {
        ui->assetTable->populate(g_backend->assets());
        if(g_backend->asset().contains("id")){
            const QString a = g_backend->asset()["id"].toString(".");
            const auto& list = g_backend->assets();
            for(int i = 0; i < list.count(); i++)
            {
                if(list[i].isObject())
                {
                    QJsonObject obj = list[i].toObject();
                    if(obj.contains("id"))
                    {
                        const QString b = obj["id"].toString(".");
                        if(!a.compare(b))
                        {
                            ui->assetTable->select(i);
                            ok = true;
                            break;

                        }
                    }
                }
            }
        }
    }
    if(!ok){
        ui->assetTable->select(-1);
    }
}
