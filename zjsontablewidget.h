#ifndef ZJSONTABLEWIDGET_H
#define ZJSONTABLEWIDGET_H

#include <QTableWidget>
#include <QJsonArray>
#include <QStyledItemDelegate>
#include <QPainter>


class ZJsonTableWidget : public QTableWidget
{
    Q_OBJECT
public:
    explicit ZJsonTableWidget(QWidget *parent = nullptr);

    void populate(const QJsonArray& records);
    inline int selected(){return m_rowSelected;}
    inline void select(int sel){m_rowSelected = (sel >=0  && sel < rowCount()) ? sel: -1;}
signals:
    void selectionChange(int sel);

private:
    int m_rowSelected;
};

class BorderDelegate : public QStyledItemDelegate {
public:
    int targetRow;
    BorderDelegate(ZJsonTableWidget *parent)
        : QStyledItemDelegate(parent), m_super(parent){}

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override {
        QStyledItemDelegate::paint(painter, option, index);

        if (index.row() == m_super->selected()) {
            painter->save();
            QPen pen(Qt::white, 2);
            painter->setPen(pen);
            painter->drawRect(option.rect.adjusted(0, 0, -1, -1));
            painter->restore();
        }
    }
private:
    ZJsonTableWidget* m_super;
};

#endif // ZJSONTABLEWIDGET_H
