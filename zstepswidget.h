#ifndef ZSTEPSWIDGET_H
#define ZSTEPSWIDGET_H

#include <QWidget>

namespace Ui {
class ZStepsWidget;
}

class ZStepsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ZStepsWidget(QWidget *parent = nullptr);
    ~ZStepsWidget();

public slots:
    void populate();

private:
    Ui::ZStepsWidget *ui;
};

#endif // ZSTEPSWIDGET_H
