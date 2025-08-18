#ifndef ZTICKETSWIDGET_H
#define ZTICKETSWIDGET_H

#include <QWidget>
#include <QTreeView>

namespace Ui {
class ZTicketsWidget;
}

class ZTicketsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ZTicketsWidget(QWidget *parent = nullptr);
    ~ZTicketsWidget();

public slots:
    void populate();

private:
    Ui::ZTicketsWidget *ui;
};

#endif // ZTICKETSWIDGET_H
