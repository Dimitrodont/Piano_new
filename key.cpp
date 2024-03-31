#include "key.h"

key::key(char color, key_box *parent) : QLabel(parent)
{
    this->base = parent;
    if(color == 'B'){
        this->normalKey = &(parent->blackKey);
        this->pressKey = &(parent->blackKeyPress);
    }else if(color == 'W'){
        this->normalKey = &(parent->whiteKey);
        this->pressKey = &(parent->whiteKeyPress);
    }
    setPixmap(*normalKey);
}

void key::mousePressEvent(QMouseEvent *e)
{
    press();
}

void key::mouseReleaseEvent(QMouseEvent *e)
{
    release();
}

void key::setSound(QString path, QString& log)
{
    if(!QFile::exists(path)){
        log += path + "      ";
        return;
    }
    delete sound;
    sound = new QMediaPlayer(this);
    sound->setMedia((QUrl::fromLocalFile(path)));
}

void key::press()
{
    sound->stop();
    setPixmap(*pressKey);
    sound->setVolume(base->volume);
    sound->play();
}

void key::release()
{
    setPixmap(*normalKey);
    sound->setVolume(static_cast<double>(base->volume) / 100.0 * static_cast<double>(base->postVolume));
}

key::~key()
{
    delete sound;
}
