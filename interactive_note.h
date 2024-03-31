#ifndef INTERACTIVE_NOTE_H
#define INTERACTIVE_NOTE_H

#include <QLabel>
#include <QPainter>
#include <QMouseEvent>

#include "editor.h"

#pragma once

class editor;

class interactive_note : public QLabel
{
    Q_OBJECT
public:
    interactive_note(QImage *img, int duration, editor *parent = nullptr);
    void paintEvent(QPaintEvent* pe);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void mouseDoubleClickEvent(QMouseEvent *e);
    void setFlag(bool flag);
    bool getFlag();
    bool getIsBlack();
    int getDuration();
    bool setDiese(bool value);

private:
    editor *base = nullptr;
    QImage *img = nullptr;
    bool flag = false;
    bool isBlack = false;
    int duration = 0;

signals:
    void mousePress(interactive_note *i, QMouseEvent *e);
    void mouseRelease(interactive_note *i, QMouseEvent *e);
    void dieseChange();
};

#endif // INTERACTIVE_NOTE_H
