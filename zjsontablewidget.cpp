#include "zjsontablewidget.h"
#include <QJsonObject>
#include <QPushButton>
#include <QHBoxLayout>

ZJsonTableWidget::ZJsonTableWidget(QWidget *parent)
    : QTableWidget{parent}
    , m_rowSelected(-1)
{
    setItemDelegate(new BorderDelegate(this));
}

void ZJsonTableWidget::populate(const QJsonArray& records)
{
    clear();
    if (records.isEmpty() || !records.at(0).isObject()) {
        qDebug() << "No records or invalid format";
        return;
    }

    // Get headers from the first record
    QJsonObject firstObj = records[0].toObject();
    QStringList headers = firstObj.keys();
    headers << "Actions";  // add actions column

    clear();
    setColumnCount(headers.size());
    setRowCount(records.size());
    setHorizontalHeaderLabels(headers);
    setAlternatingRowColors(true);

    // Fill rows
    for (int row = 0; row < records.size(); ++row) {
        QJsonObject obj = records.at(row).toObject();
        int col = 0;
        for (QString &key : firstObj.keys()) {
            QString valStr;
            QJsonValue v = obj.value(key);

            if(v.isString())
                valStr = v.toString();
            else if (v.isDouble())
                valStr = QString::number(v.toDouble());
            else if (v.isBool())
                valStr = v.toBool() ? "true" : "false";
            else if (v.isArray())
                valStr = "[Array]";
            else if (v.isObject())
                valStr = "{Object}";
            else if (v.isNull())
                valStr = "null";

            auto* item = new QTableWidgetItem(valStr);
            item->setFlags(item->flags() | Qt::ItemIsEditable);
            setItem(row, col, item);
            col++;
        }

        // --- Add Edit/Delete buttons in last column ---
        QWidget *btnWidget = new QWidget(this);
        QPushButton *editBtn = new QPushButton("Edit", btnWidget);
        QPushButton *delBtn  = new QPushButton("Delete", btnWidget);
        editBtn->setMinimumHeight(32);
        editBtn->setMinimumWidth(100);
        delBtn->setMinimumHeight(32);
        delBtn->setMinimumWidth(100);

        QHBoxLayout *layout = new QHBoxLayout(btnWidget);
        layout->addWidget(editBtn);
        layout->addWidget(delBtn);
        layout->setContentsMargins(0,0,0,0);
        layout->setSpacing(4);
        btnWidget->setLayout(layout);

        setCellWidget(row, headers.size()-1, btnWidget);
        setRowHeight(row, 64);

            // Connect buttons
        QObject::connect(editBtn, &QPushButton::clicked, this, [this, row]() {
            qDebug() << "Edit row" << row;
            // TODO: open editor dialog or inline edit
        });

        QObject::connect(delBtn, &QPushButton::clicked, this, [this, row]() {
            qDebug() << "Delete row" << row;
            removeRow(row);
        });

        connect(this, &QTableWidget::itemClicked,
                this, [this](QTableWidgetItem *item){
                    select(item->row());
                    repaint();
                    emit selectionChange(selected());
                });

    }

    resizeColumnsToContents();
    setColumnWidth(columnCount() -  1, 230);

}
