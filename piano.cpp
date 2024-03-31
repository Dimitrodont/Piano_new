#include "piano.h"

const QVector<int> piano::skipVec({1, 4, 8, 11, 15, 18, 22, 25, 29, 32, 36, 39, 43, 46, 50});

piano::piano(QWidget *parent) : QWidget(parent)
{
    int gap = 10;

    key = new key_box(this);
    ed = new editor(this);
    manager = new manager_box(this);
    timer = new QTimer(this);

    ed->move(0, 0);
    manager->move(0, ed->height());
    key->move(0, ed->height() + manager->height() * 2 + gap);

    setMaximumSize(key->width(), ed->height() + manager->height() * 2 + key->height() + gap);

    setWindowTitle("Piano: " + currentFileName);

    connect(manager->toLeft, SIGNAL(clicked()), key, SLOT(shiftToLeft()));
    connect(manager->toRight, SIGNAL(clicked()), key, SLOT(shiftToRight()));
    connect(key, SIGNAL(shiftChanged(int)), manager, SLOT(shiftChanged(int)));
    connect(manager->postVolSl, SIGNAL(valueChanged(int)), key, SLOT(postVolumeChanged(int)));
    connect(manager->volSl, SIGNAL(valueChanged(int)), key, SLOT(volumeChanged(int)));
    connect(timer, SIGNAL(timeout()), this, SLOT(timerTick()));
}

void piano::keyPressEvent(QKeyEvent *e)
{
    ed->keyPress(e);
    key->keyPress(e);
}

void piano::keyReleaseEvent(QKeyEvent *e)
{
    ed->keyRelease(e);
    key->keyRelease(e);
}

bool piano::saveFile()
{
    QString filename = QFileDialog::getSaveFileName(this, "Save file", currentFileName, "Notes (*.note)");

    if(filename == ""){
        return false;
    }

    setTempData();

    QFile file(filename);
    QFileInfo fileInfo(file.fileName());
    if(file.open(QIODevice::WriteOnly)){
        QDataStream stream(&file);
        stream.setVersion(QDataStream::Qt_4_2);
        stream << tactDur;
        stream << ed->getK();
        stream << tempData;
        if(stream.status() != QDataStream::Ok){
            qDebug() << "Ошибка записи";
        }
    }
    file.close();
    ed->setSavedFlag(true);
    currentFileName = fileInfo.fileName();
    setWindowTitle("Piano: " + currentFileName);
    return true;
}

void piano::setTempData()
{
    QVector<smoll_note> smollNoteVec(ed->getSmollNoteVec());

    tempData.clear();

    if(smollNoteVec.isEmpty()){
        return;
    }
    qSort(smollNoteVec.begin(), smollNoteVec.end(), smollNoteCompar);

    int currX = smollNoteVec.first().X;
    QVector<smoll_note> tempVec;
    for(int i = 0; i < smollNoteVec.size(); ++i){
        if(currX != smollNoteVec[i].X){
            tempData.push_back(tempVec);
            tempVec.clear();
            ++currX;
            while(currX != smollNoteVec[i].X){
                ++currX;
                tempData.push_back(tempVec);
            }
        }
        tempVec.push_back(smollNoteVec[i]);
    }
    tempData.push_back(tempVec);
}

int piano::getTactDur()
{
    return tactDur;
}

void piano::closeEvent(QCloseEvent *e)
{
    if(ed->isSaved()){
        e->accept();
        return;
    }

    e->ignore();
    QMessageBox msgBox(this);
    msgBox.setText("Документ был изменен.");
    msgBox.setInformativeText("Вы хотите сохранить изменения?");
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);
    int ret = msgBox.exec();
    if(ret == QMessageBox::Save){
        if(saveFile()){
            e->accept();
        }
    }else if(ret == QMessageBox::Discard){
        e->accept();
    }
}

int piano::recalcBlackLine(int line)
{
    int d = 0;
    for (int i = 0; i < skipVec.size(); ++i) {
        if (line > skipVec[i]) {
            ++d;
        } else {
            break;
        }
    }
    return line - d;
}

void piano::setTactDur(const QString& str)
{
    tactDur = str.toInt();
    qDebug() << tactDur;
}

void piano::openFile()
{
    QString filename = QFileDialog::getOpenFileName(this, "Open file", "", "Notes (*.note)");

    if(filename == ""){
        return;
    }
    int k = 0;
    QFile file(filename);
    QFileInfo fileInfo(file.fileName());
    if(file.open(QIODevice::ReadOnly)){
        QDataStream stream(&file);
        stream. setVersion(QDataStream::Qt_4_2);
        stream >> tactDur;
        stream >> k;
        tempData.clear();
        stream >> tempData;
        if(stream.status() != QDataStream::Ok){
            qDebug() << "Ошибка чтения";
        }
    }
    file.close();
    ed->setK(k);
    manager->setTactDur(QString::number(tactDur));
    ed->rewriteField(tempData);
    currentFileName = fileInfo.fileName();
    setWindowTitle("Piano: " + currentFileName);
}

void piano::playPause()
{
    if (isPlaing) {
        // then PAUSE
        timer->stop();
        // release all pressed keys
        for (int i = 0; i < pressedNote.size(); ++i) {
            if (pressedNote[i].isBlack) {
                key->blackKeyArr[BLACK_SIZE - 1 - recalcBlackLine(pressedNote[i].line)]->release();
            } else {
                key->whiteKeyArr[WHITE_SIZE - 1 - pressedNote[i].line]->release();
            }
        }
    } else {
        // then PLAY
        ed->isPlaing = true;
        timer->setInterval(tactDur / 16);
        timer->start();
    }
    isPlaing = !isPlaing;
}

void piano::stop()
{
    timer->stop();
    for (int i = 0; i < pressedNote.size(); ++i) {
        if (pressedNote[i].isBlack) {
            key->blackKeyArr[BLACK_SIZE - 1 - recalcBlackLine(pressedNote[i].line)]->release();
        } else {
            key->whiteKeyArr[WHITE_SIZE - 1 - pressedNote[i].line]->release();
        }
    }
    pressedNote.clear();

    isPlaing = false;
    ed->isPlaing = isPlaing;

    // move to begin
    int begin;
    if (ed->getFirstPos() == 0) {
        begin = ed->getK() * -1;
    } else {
        begin = ed->getFirstPos() - 2;
    }
    ed->moveField(begin - 1);
}

void piano::timerTick()
{
    // duration -= 1
    for (int i = 0; i < pressedNote.size(); ++i) {
        pressedNote[i].duration--;
    }

    ed->moveField(1);
    QVector<smoll_note> curCol(ed->getCurCol());

    bool inPressed;
    for (int i = 0; i < curCol.size(); ++i) {
        if (curCol[i].isBlack) {
            key->blackKeyArr[BLACK_SIZE - 1 - recalcBlackLine(curCol[i].line)]->press();
        } else {
            key->whiteKeyArr[WHITE_SIZE - 1 - curCol[i].line]->press();
        }
        inPressed = false;
        //find in pressed note
        for (int j = 0; j < pressedNote.size(); ++j) {
            if (pressedNote[j].line == curCol[i].line && pressedNote[j].isBlack == curCol[i].isBlack) {
                inPressed = true;
                pressedNote[j].duration = 1.0 / static_cast<double>(curCol[i].duration) * 16.0;
                break;
            }
        }
        if (!inPressed) {
            pressedNote.push_back(curCol[i]);
            // change duration to normal, now duration = tick count to play
            pressedNote.back().duration = 1.0 / static_cast<double>(pressedNote.back().duration) * 16.0;
        }
    }

    // chek and release key
    QVector<int> targets;
    for (int i = 0; i < pressedNote.size(); ++i) {
        if (pressedNote[i].duration <= 0) {
            if (pressedNote[i].isBlack) {
                key->blackKeyArr[BLACK_SIZE - 1 - recalcBlackLine(pressedNote[i].line)]->release();
            } else {
                key->whiteKeyArr[WHITE_SIZE - 1 - pressedNote[i].line]->release();
            }
            targets.push_back(i);
        }
    }
    /*remuve 0 duration note*/
    for (int i = 0; i < targets.size(); ++i) {
        pressedNote.remove(targets[i] - i);
    }

    if (curCol.isEmpty() && pressedNote.isEmpty() && !ed->hasRightNotes()) {
        playPause();    // pause
    }
}

bool smollNoteCompar(smoll_note a, smoll_note b)
{
    return a.X < b.X;
}
