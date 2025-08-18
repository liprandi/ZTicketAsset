#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <cassert>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_backend(this) {

    ui->setupUi(this);
    ui->stackedPane->setCurrentIndex(0);

    m_menu.push_back({tr("About"), ":/icons/icon0"});
    m_menu.push_back({tr("Login"), ":/icons/icon1"});
    m_menu.push_back({tr("Asset"), ":/icons/icon2"});
    m_menu.push_back({tr("Ticket"), ":/icons/icon3"});
    m_menu.push_back({tr("Step"), ":/icons/icon4"});

    QList<QAction*> act_menu = {0, ui->actionLogin, ui->actionAsset, ui->actionTicket, ui->actionStep};


    for(int i = 0; i < m_menu.size() && i < act_menu.size(); i++) {

        const auto& item = m_menu[i];
        auto action = ui->sidemenu->addAction(item[0],
                          QIcon(item[1]));
        action->setEnabled(i < 3);
        connect(action, &QAction::triggered, this, [=, this](bool){
            assert(i < m_menu.size());
            ui->stackedPane->setCurrentIndex(i);
            for(int j = 1; j < act_menu.size(); j++)
            {
                act_menu[j]->setChecked(j == i);
            }
        });
        if(i > 0) {
            connect(act_menu[i], &QAction::triggered, this, [=, this](bool){
                ui->sidemenu->selectAction(i);
            });
        }
    }

    connect(&m_backend, &ZBackEnd::usersUpdated, this, [this](){
        ui->updateBar->setValue(m_backend.updatePercent());
    });
    connect(&m_backend, &ZBackEnd::assetsUpdated, this, [this](){
        ui->updateBar->setValue(m_backend.updatePercent());
    });
    connect(&m_backend, &ZBackEnd::ticketsUpdated, this, [this](){
        ui->updateBar->setValue(m_backend.updatePercent());
    });
    connect(&m_backend, &ZBackEnd::stepsUpdated, this, [this](){
        ui->updateBar->setValue(m_backend.updatePercent());
    });
    connect(ui->pbRefresh, &QPushButton::clicked, this, [this](bool){
        m_backend.readUsers();
    });
    connect(ui->stackedPane, &QStackedWidget::currentChanged, this, [this](int index){
        if(index == 2)
            ui->assets->populate();
    });
    m_backend.login({"agostino.beltrando", "C4mb14m1"});
}

MainWindow::~MainWindow() { delete ui; }
