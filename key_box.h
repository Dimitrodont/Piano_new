#ifndef KEY_BOX_H
#define KEY_BOX_H

#include <QWidget>
#include <QDir>
#include <QMessageBox>
#include "key.h"
#include "piano.h"

#pragma once

class key;
class piano;

const int WHITE_SIZE = 52;
const int BLACK_SIZE = 36;

class key_box : public QWidget
{
    Q_OBJECT
    friend class key;
    friend class piano;
    friend class manager_box;
public:
    key_box(piano *parent = nullptr);
    void setKeySound(QString folder = "sound");
    void setSlider();
    void keyPress(QKeyEvent *e);
    void keyRelease(QKeyEvent *e);

private:
    piano* base = nullptr;

    key* whiteKeyArr[WHITE_SIZE];
    key* blackKeyArr[BLACK_SIZE];

    int blackHeight = 0,
        blackWidth  = 0,
        whiteHeight = 0,
        whiteWidth  = 0;

    int postVolume = 30,
        volume = 100;

    QPixmap blackKey,
            whiteKey,
            blackKeyPress,
            whiteKeyPress;

    /*валидные клавиши*/
    QString keyStrB = "23567SDGHJ";
    QString keyStrW = "QWERTYUZXCVBNM";

    QLabel* slider = nullptr;

    int shiftW = -5, lW = 5, rW = 14;
    int shiftB = -4, lB = 4, rB = 10;

private slots:
    void shiftToLeft();
    void shiftToRight();
    void postVolumeChanged(int volume);
    void volumeChanged(int volume);

signals:
    void shiftChanged(int shiftW);

};

#endif // KEY_BOX_H
