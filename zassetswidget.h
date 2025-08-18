#ifndef ZASSETSWIDGET_H
#define ZASSETSWIDGET_H

#include <QWidget>
#include <QTreeView>

namespace Ui {
class ZAssetsWidget;
}

class ZAssetsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ZAssetsWidget(QWidget *parent = nullptr);
    ~ZAssetsWidget();

public slots:
    void populate();

private:
    Ui::ZAssetsWidget *ui;
};

#endif // ZASSETSWIDGET_H
