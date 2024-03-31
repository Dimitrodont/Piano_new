#ifndef NOTE_DECK_H
#define NOTE_DECK_H

#include <QLabel>
#include <QPainter>

#include "editor.h"

#pragma once

class editor;

class note_deck : public QLabel
{
    Q_OBJECT
public:
    note_deck(QImage *img, int duration, editor *parent = nullptr);
    void mousePressEvent(QMouseEvent *e);
    void paintEvent(QPaintEvent* pe);
    QImage* getImg(){ return img; }
    int getDur(){ return duration; }

private:
    editor* base = nullptr;
    QImage *img = nullptr;
    int duration = 0;

signals:
    void mousePress(note_deck* d);
};

#endif // NOTE_DECK_H
