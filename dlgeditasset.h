#ifndef DLGEDITASSET_H
#define DLGEDITASSET_H

#include <QJsonObject>
#include <QDialog>

namespace Ui {
class DlgEditAsset;
}

class DlgEditAsset : public QDialog
{
    Q_OBJECT

public:
    explicit DlgEditAsset(QWidget *parent = nullptr);
    ~DlgEditAsset();

private:
    Ui::DlgEditAsset *ui;
    QJsonObject m_asset;
};

#endif // DLGEDITASSET_H
