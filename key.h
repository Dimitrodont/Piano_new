#ifndef KEY_H
#define KEY_H

#include <QLabel>
#include <QMediaPlayer>
#include <QMouseEvent>
#include "key_box.h"

#pragma once

class key_box;

class key : public QLabel
{
    Q_OBJECT
    friend class key_box;
private:
    QMediaPlayer* sound = nullptr;  //ИСПОЛЬЗУЙ QMediaPlayer ОН ЛУЧШЕ ЧЕМ QSound и QSoundEffect
    QPixmap* normalKey = nullptr;
    QPixmap* pressKey = nullptr;
    key_box* base = nullptr;

public:
    key(char color, key_box *parent = nullptr);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void setSound(QString path, QString& log);
    void press();
    void release();
    ~key();
};

#endif // KEY_H
