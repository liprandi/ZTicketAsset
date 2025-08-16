#ifndef ZLOGIN_H
#define ZLOGIN_H

#include <QWidget>

namespace Ui {
class ZLogin;
}

class ZLogin : public QWidget
{
    Q_OBJECT

public:
    explicit ZLogin(QWidget *parent = nullptr);
    ~ZLogin();

private:
    Ui::ZLogin *ui;
};

#endif // ZLOGIN_H
