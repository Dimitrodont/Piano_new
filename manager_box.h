#ifndef MANAGER_BOX_H
#define MANAGER_BOX_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>

#include "piano.h"

#pragma once

class piano;

class manager_box : public QWidget
{
    Q_OBJECT
    friend class piano;
    friend class key_box;
public:
    manager_box(piano *parent = nullptr);
    void setTactDur(QString str);

private:
    piano* base = nullptr;

    QVector<QString> octavesNams = {"Субконтроктава", "Контроктава", "Большая октава", "Малая октава", "Первая октава", "Вторая октава", "Третья октава", "Четвертая октава", "Пятая октава"};
    QPushButton *toLeft = nullptr,
                *toRight = nullptr;
    QLabel *octavesInfo = nullptr,
           *postVolInfo = nullptr,
           *postVolName = nullptr,
           *volInfo = nullptr,
           *volName = nullptr,
           *tactDurName = nullptr;
    QVBoxLayout *mainLayout = nullptr;
    QHBoxLayout *upperLay = nullptr,
                *bottomLay = nullptr;
    QSlider *postVolSl = nullptr,
            *volSl = nullptr;
    QLineEdit *tactDur = nullptr;

public slots:
    void shiftChanged(int shiftW);
    void postVolumeChanged(int volume);
    void volumeChanged(int volume);

};

#endif // MANAGER_BOX_H
