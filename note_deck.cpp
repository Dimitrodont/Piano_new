#include "note_deck.h"

note_deck::note_deck(QImage *img, int duration, editor *parent) : QLabel(parent)
{
    this->base = parent;
    this->duration = duration;
    this->img = img;
    setFixedSize(img->size());

    connect(this, SIGNAL(mousePress(note_deck*)), base, SLOT(addNote(note_deck*)));
}

void note_deck::mousePressEvent(QMouseEvent *e)
{
    emit mousePress(this);
}

void note_deck::paintEvent(QPaintEvent *pe)
{
    QPainter painter(this);
    painter.drawImage(0, 0, *img);
    painter.setPen(QPen(Qt::black, 2, Qt::SolidLine));
    painter.drawRect(rect());
}
