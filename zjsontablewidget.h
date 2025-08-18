#ifndef ZJSONTABLEWIDGET_H
#define ZJSONTABLEWIDGET_H

#include <QTableWidget>
#include <QJsonArray>

class ZJsonTableWidget : public QTableWidget
{
    Q_OBJECT
public:
    explicit ZJsonTableWidget(QWidget *parent = nullptr);

    void populate(const QJsonArray& records);

signals:
};

#endif // ZJSONTABLEWIDGET_H
