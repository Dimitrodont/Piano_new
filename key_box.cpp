#include "key_box.h"

void key_box::setSlider()
{
    if(shiftW < 0){
        slider->move(0, whiteHeight);
        slider->resize(9 * whiteWidth, 5);
    }else{
        slider->move(shiftW * whiteWidth, whiteHeight);
        slider->resize(14*whiteWidth, 5);
    }
}

void key_box::shiftToLeft()
{
    shiftW -= 7;
    shiftB -= 5;
    emit shiftChanged(shiftW);
    lW = 0;
    lB = 0;
    rW = 14;
    rB = 10;

    if(shiftW == -5){
        lW = 5;
        lB = 4;
    }
    setSlider();
}

void key_box::shiftToRight()
{
    shiftW += 7;
    shiftB += 5;
    emit shiftChanged(shiftW);
    lW = 0;
    lB = 0;
    rW = 14;
    rB = 10;

    if(shiftW == 44){
        rW = 8;
        rB = 5;
    }
    setSlider();
}

void key_box::postVolumeChanged(int volume)
{
    postVolume = volume;
}

void key_box::volumeChanged(int volume)
{
    this->volume = volume;
}

key_box::key_box(piano *parent) : QWidget(parent)
{
    base = parent;
    QString log = "";   //все ошибки сюда

    QString pixArr[4] = {"/img/black_key.bmp",
                         "/img/white_key.bmp",
                         "/img/black_key_press.bmp",
                         "/img/white_key_press.bmp"};

    QString path = QDir::currentPath();
    path += "/debug";

    for(int i = 0; i < 4; ++i){
        if(!QFile::exists(path + pixArr[i]))
            log += path + pixArr[i] + "      ";
    }

    blackKey = QPixmap(path + pixArr[0]);
    whiteKey = QPixmap(path + pixArr[1]);
    blackKeyPress = QPixmap(path + pixArr[2]);
    whiteKeyPress = QPixmap(path + pixArr[3]);

    blackHeight = blackKey.height();
    blackWidth = blackKey.width();
    whiteHeight = whiteKey.height();
    whiteWidth = whiteKey.width();

    for(int i = 0; i < WHITE_SIZE; i++){
        whiteKeyArr[i] = new key('W', this);
        whiteKeyArr[i]->move(whiteWidth*i, 0);
    }

    int counter = 0;
    const int center = whiteWidth-blackWidth/2;
    const int bord = 2*whiteWidth;
    const int octaveWidth = 7*whiteWidth;
    blackKeyArr[counter] = new key('B', this);
    blackKeyArr[counter]->move(center, 0);
    for(int i = 0; i < 7; i++){
        for(int j = 0; j < 6; j++){
            if(j==2){ continue; }
            blackKeyArr[++counter] = new key('B', this);
            blackKeyArr[counter]->move(bord + i*octaveWidth + j*whiteWidth + center, 0);
        }
    }
    setKeySound();

    slider = new QLabel(this);
    slider->setStyleSheet("QLabel { background-color: black; }");
    setSlider();

    setFixedSize(whiteWidth * WHITE_SIZE, whiteHeight + slider->height());
}

void key_box::setKeySound(QString folder)
{
    QString log = "";   //все ошибки сюда
    QString path = QDir::currentPath();
    path += "/debug";
    path += '/' + folder;
    QString noteName = "CDEFGAB";
    QString noteForm = ".wav";
    int counter = 0;
    /*ЗВУКИ БЕЛЫХ*/
    whiteKeyArr[counter]->setSound(path + "/0" + noteName[5] + noteForm, log);
    whiteKeyArr[++counter]->setSound(path + "/0" + noteName[6] + noteForm, log);
    for(int i = 1; i < 8; i++){ /*октавы*/
        for(int j = 0; j < 7; j++){
            whiteKeyArr[++counter]->setSound(path + '/' + QString::number(i) + noteName[j] + noteForm, log);
        }
    }
    whiteKeyArr[++counter]->setSound(path + "/8" + noteName[0] + noteForm, log);
    /*ЗВУКИ ЧЕРНЫХ*/
    counter = 0;
    blackKeyArr[counter]->setSound(path + "/0" + noteName[6] + 'b' + noteForm, log);
    for(int i = 1; i < 8; i++){ /*октавы*/
        for(int j = 0; j < 6; j++){
            if(j==2){ continue; }
            blackKeyArr[++counter]->setSound(path + '/' + QString::number(i) + noteName[j+1] + 'b' + noteForm, log);
        }
    }

    if(!log.isEmpty()){
        QMessageBox msgBox;
        msgBox.critical(nullptr, "Can't find files:", log);
    }
}

void key_box::keyPress(QKeyEvent *e)
{
    if(e->isAutoRepeat())
        return;
/*поиск белоый нажатой клавиши*/
    for(int i = lW; i < rW; ++i){
        if(keyStrW[i] == e->key()){
            whiteKeyArr[i + shiftW]->press();
            return;
        }
    }
/*поиск черной нажатой клавиши*/
    for(int i = lB; i < rB; ++i){
        if(keyStrB[i] == e->key()){
            blackKeyArr[i + shiftB]->press();
            return;
        }
    }
}

void key_box::keyRelease(QKeyEvent *e)
{
    if(e->isAutoRepeat())
        return;
/*поиск белоый отпущеной клавиши*/
    for(int i = lW; i < rW; ++i){
        if(keyStrW[i] == e->key()){
            whiteKeyArr[i + shiftW]->release();
            return;
        }
    }
/*поиск черной отпущеной клавиши*/
    for(int i = lB; i < rB; ++i){
        if(keyStrB[i] == e->key()){
            blackKeyArr[i + shiftB]->release();
            return;
        }
    }
}
