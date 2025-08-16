#include "zbackend.h"
#include "zlogin.h"
#include "ui_zlogin.h"

ZLogin::ZLogin(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ZLogin)
{
    ui->setupUi(this);

    connect(ui->pbLogin, &QPushButton::clicked, this, [=, this]{
        if(g_backend)
        {
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
