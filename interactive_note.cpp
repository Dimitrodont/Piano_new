#include "interactive_note.h"

interactive_note::interactive_note(QImage *img, int duration, editor *parent) : QLabel(parent)
{
    base = parent;
    this->img = img;
    this->duration = duration;
    flag = false;
    setFixedSize(img->size());

    connect(this, SIGNAL(mousePress(interactive_note*, QMouseEvent*)), parent, SLOT(mousePress(interactive_note*, QMouseEvent*)));
    connect(this, SIGNAL(mouseRelease(interactive_note*, QMouseEvent*)), parent, SLOT(mouseRelease(interactive_note*, QMouseEvent*)));
    connect(this, SIGNAL(dieseChange()), parent, SLOT(dieseChange()));
}

void interactive_note::paintEvent(QPaintEvent *pe)
{
    QPainter painter(this);
    if (base->isColorized()) {
        painter.drawImage(0, 0, *base->getColorizedImg(y(), duration));
    } else {
        painter.drawImage(0, 0, *img);
    }

    if(flag){
        painter.setPen(QPen(Qt::black, 3, Qt::SolidLine));
        painter.drawRect(rect());
    }
    if(isBlack){
        painter.drawImage(0, 0, *base->getDieseImg());
    }
    painter.setPen(QPen(Qt::black, 3, Qt::SolidLine));
}

void interactive_note::mousePressEvent(QMouseEvent *e)
{
    emit mousePress(this, e);
}

void interactive_note::mouseReleaseEvent(QMouseEvent *e)
{
    emit mouseRelease(this, e);
}

void interactive_note::mouseDoubleClickEvent(QMouseEvent *e)
{
    bool check = setDiese(!isBlack);
    if(check){
        emit dieseChange();
    }
}

void interactive_note::setFlag(bool flag)
{
    this->flag = flag;
    update();
}

bool interactive_note::getFlag()
{
    return flag;
}

bool interactive_note::getIsBlack()
{
    return isBlack;
}

int interactive_note::getDuration()
{
    return duration;
}

bool interactive_note::setDiese(bool value)
{
    if(!value){
        isBlack = value;
        update();
        return true;
    }
    int line = pos().y() / (base->lineGap / 2);
    if(line == 0 || line % 7 == 1 || line % 7 == 5 || line == 51){
        isBlack = !value;
        update();
        return false;
    }
    isBlack = value;
    update();

    return true;
}


