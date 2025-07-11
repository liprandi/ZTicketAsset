#include "sidebar.h"

#include <QDebug>
#include <QEvent>
#include <QPaintEvent>
#include <QPainter>

#define action_height 90

SideBar::SideBar(QWidget *parent)
    : QWidget(parent), mCheckedAction(NULL), mOverAction(NULL) {
  setMouseTracking(true);
}

void SideBar::paintEvent(QPaintEvent */*event*/) {
    QPainter p(this);

    QFont fontText(p.font());
    fontText.setFamily("Helvetica Neue");
    p.setFont(fontText);

    int action_y = 0;
    p.fillRect(rect(), QColor(100, 100, 100));
    for (auto& action : mActions) {

        QRgb col = 0;
        QRect actionRect(0, action_y, rect().width(), action_height);

        if (action->isChecked()) {
            col += 0x2020cf;
        }

        if (action == mOverAction) {
            col += 0x008f8f;
        }

        if(col) {
            p.fillRect(actionRect, QColor(col));
        }

        p.setPen(QColor(255, 255, 255));
        QSize size = p.fontMetrics().size(Qt::TextSingleLine, action->text());
        QRect actionTextRect(QPoint(actionRect.width() / 2 - size.width() / 2,
                                    actionRect.bottom() - size.height() - 5),
                             size);
        p.drawText(actionTextRect, Qt::AlignCenter, action->text());

        QRect actionIconRect(0, action_y + 10, actionRect.width(),
                             actionRect.height() - 2 * actionTextRect.height() -
                                 10);
        QIcon actionIcon(action->icon());
        actionIcon.paint(&p, actionIconRect, Qt::AlignCenter, action->isEnabled() ? action->isChecked() ? QIcon::Selected: QIcon::Normal: QIcon::Disabled);

        action_y += actionRect.height();
    }
}

QSize SideBar::minimumSizeHint() const {
    return action_height * QSize(1, mActions.size());
}

void SideBar::addAction(QAction *action) {
    mActions.push_back(action);
    action->setCheckable(true);
    update();
}

QAction *SideBar::addAction(const QString &text, const QIcon &icon) {
    QAction *action = new QAction(icon, text, this);
    action->setCheckable(true);
    mActions.push_back(action);
    update();
    return action;
}

void SideBar::mousePressEvent(QMouseEvent *event) {
    QAction *tempAction = actionAt(event->pos(), true);
    if (tempAction == NULL)
        return;
    mCheckedAction = tempAction;
//    if(mCheckedAction->isChecked())
        mCheckedAction->trigger();
    update();
    QWidget::mousePressEvent(event);
}

void SideBar::mouseMoveEvent(QMouseEvent *event) {
    QAction *tempAction = actionAt(event->pos());
    if (tempAction == NULL) {
        mOverAction = NULL;
        update();
        return;
    }
    if(mOverAction == tempAction)
        return;
    mOverAction = tempAction;
    update();
    QWidget::mouseMoveEvent(event);
}

void SideBar::leaveEvent(QEvent *event) {
    mOverAction = NULL;
    update();
    QWidget::leaveEvent(event);
}

QAction *SideBar::actionAt(const QPoint &at, bool toggle) {
    int action_y = 0;
    QAction* ret = NULL;
    for(size_t i = 0; i < mActions.size(); i++) {
        auto action = mActions[i];
        QRect actionRect(0, action_y, rect().width(), action_height);
        if (actionRect.contains(at)) {
            ret = action;
        }
        else {
            if(toggle && action->isCheckable())
                action->setChecked(false);
        }
        action_y += actionRect.height();
    }
    return ret;
}

void SideBar::selectAction(int index)
{
    if(index < mActions.size())
    {
        if(mActions[index]->isEnabled() && mActions[index]->isCheckable())
        {
            mCheckedAction = mActions[index];
            for(auto& act : mActions)
                if(act != mCheckedAction)
                    act->setChecked(false);
            mCheckedAction->trigger();
        }
        update();
    }
}

#undef action_height
