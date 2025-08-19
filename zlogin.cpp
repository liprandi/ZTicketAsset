#include "zbackend.h"
#include "zlogin.h"
#include "ui_zlogin.h"

ZLogin::ZLogin(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ZLogin)
{
    ui->setupUi(this);

    if(g_backend && g_backend->credentials().contains("nickname")){
        const auto cred = g_backend->credentials();
        ui->leUser->setText(cred["nickname"].toString("."));
        ui->lePassword->setText("C4mb14m1");
    }
    else {
        ui->leUser->setText("");
        ui->lePassword->setText("");
    }

    connect(ui->pbLogin, &QPushButton::clicked, this, [=, this]{
        if(g_backend) {
            QStringList param;

            param.push_back(ui->leUser->text());
            param.push_back(ui->lePassword->text());
            g_backend->login(param);
        }
    });

}

ZLogin::~ZLogin()
{
    delete ui;
}
