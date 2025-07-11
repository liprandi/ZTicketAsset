#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <cassert>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->stackedPane->setCurrentIndex(0);

    m_menu.push_back({tr("😐"), ":/icons/icon0"});
    m_menu.push_back({tr("Login"), ":/icons/icon1"});
    m_menu.push_back({tr("Ticket"), ":/icons/icon2"});
    m_menu.push_back({tr("Asset"), ":/icons/icon3"});
    m_menu.push_back({tr("Step"), ":/icons/icon4"});

    QList<QAction*> act_menu = {0, ui->actionLogin, ui->actionAsset, ui->actionTicket, ui->actionAsset};


    for(int i = 0; i < m_menu.size() && i < act_menu.size(); i++) {

        const auto& item = m_menu[i];
        auto action = ui->widget->addAction(item[0],
                          QIcon(item[1]));
        action->setEnabled(i < 2);
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
                ui->widget->selectAction(i);
            });
        }
    }
}

MainWindow::~MainWindow() { delete ui; }
