#ifndef ZPLACEHOLDERLINEEDIT_H
#define ZPLACEHOLDERLINEEDIT_H

#include <QLineEdit>
#include <QPainter>
#include <QPropertyAnimation>
#include <QFocusEvent>

class ZPlaceholderLineEdit : public QLineEdit
{
    Q_OBJECT
    Q_PROPERTY(qreal placeholderProgress READ placeholderProgress WRITE setPlaceholderProgress CONSTANT)

public:
    explicit ZPlaceholderLineEdit(QWidget *parent = nullptr)
        : QLineEdit(parent), m_placeholderProgress(0.0)
    {
        m_animation = new QPropertyAnimation(this, "placeholderProgress", this);
        m_animation->setDuration(400);
        m_animation->setEasingCurve(QEasingCurve::InOutQuad);

        connect(this, &QLineEdit::textChanged, this, &ZPlaceholderLineEdit::updatePlaceholderState);
    }

    qreal placeholderProgress() const {
        return m_placeholderProgress;
    }

    void setPlaceholderProgress(qreal progress) {
        m_placeholderProgress = progress;
        update();
    }

protected:
    void paintEvent(QPaintEvent *event) override {

        if(!placeholderText().isEmpty() && m_animPlaceHolder.isEmpty())
        {
            m_animPlaceHolder = placeholderText();
            setPlaceholderText(QString());
        }

        QLineEdit::paintEvent(event);

        if (m_animPlaceHolder.isEmpty())
            return;

        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing, true);
        QColor color = hasFocus() ? QColor(255, 255, 255) : QColor(150, 150, 150);
        p.setPen(color);

        QFont f = font();
        qreal scale = 1.0 - 0.2 * m_placeholderProgress;
        f.setPointSizeF(f.pointSizeF() * scale);
        p.setFont(f);

        QPointF startPos(14, height() / 2 + fontMetrics().height() / 4);
        QPointF endPos(14, fontMetrics().height() / 2); //(8, 4 + fontMetrics().height()); //
        QPointF pos = startPos + (endPos - startPos) * m_placeholderProgress;

        p.drawText(pos, m_animPlaceHolder);
    }

    void focusInEvent(QFocusEvent *e) override {
        QLineEdit::focusInEvent(e);
        updatePlaceholderState();
    }

    void focusOutEvent(QFocusEvent *e) override {
        QLineEdit::focusOutEvent(e);
        updatePlaceholderState();
    }

private:
    QPropertyAnimation *m_animation;
    qreal m_placeholderProgress;
    QString m_animPlaceHolder;

    void updatePlaceholderState() {
        bool active = hasFocus() || !text().isEmpty();
        m_animation->stop();
        m_animation->setStartValue(m_placeholderProgress);
        m_animation->setEndValue(active ? 1.0 : 0.0);
        m_animation->start();
    }
};

#endif // ZPLACEHOLDERLINEEDIT_H
