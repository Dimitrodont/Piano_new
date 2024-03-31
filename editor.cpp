#include "editor.h"

int editor::step(int target, int stepRange)
{
    qDebug() << "target" << target;
    int rem, result;
    bool isNegative = false;

    if(target < 0){
        isNegative = true;
        target = -1*target;
    }
    rem = target % stepRange;

    if(rem < static_cast<float>(stepRange) / 2.0){
        result = target - rem;
    }else{
        result = stepRange - rem + target;
    }
    qDebug() << "result" << result;
    if(isNegative)
        return -1*result;
    return result;
}

void editor::updateTempVec()
{
    tempVecPress.clear();
    smoll_note temp;
    for(interactive_note* i : groupIN){
        temp.X = i->pos().x();
        temp.line = i->pos().y();
        temp.duration = i->getDuration();
        temp.isBlack = i->getIsBlack();
        tempVecPress.push_back(temp);
    }
    qDebug() << "add " << tempVecPress.size();
}

editor::editor(piano *parent) : QWidget(parent)
{
    base = parent;

    QString path = QDir::currentPath();
    path += "/debug";


    int dur = 16;
    while(dur){
        noteImg.push_back(QImage(path + "/img/" + QString::number(dur) + "note.png"));
        dur /= 2;
    }

    noteSize = noteImg[0].size();

    field.adjust(100, marg*3, 100 + noteSize.width() * 19, 28 * lineGap);
    setFixedSize(base->key->width(), 28 * lineGap + 2 * marg);

    dur = 16;
    for(int i = 0; i < noteImg.size(); ++i){
        note_deck* newDeck = new note_deck(&noteImg[i], dur, this);
        newDeck->move(field.right() + marg, field.top() + i * (newDeck->height() + marg/2));
        noteDeck.push_back(newDeck);
        dur /= 2;
    }

    trebleClef = new QImage(path + "/img/treble_clef.png");
    bassClef = new QImage(path + "/img/bass_clef.png");
    diese = new QImage(path + "/img/diese.png");

    QSize btnSize(50, 50);


    for (int i = 0; i < colorArrL; ++i) {
        dur = 16;
        for (int j = 0; j < colorArrC; ++j) {
            colorArrImg[i][j] = new QImage(path + "/img/" + QString::number(dur) + "note_" + colorsCh[i] + ".png");
            dur /= 2;
        }
    }

    saveButt = new QPushButton(this);
    saveButt->setIcon(QIcon(path + "/img/save_btn.png"));
    saveButt->setIconSize(btnSize);
    saveButt->setFixedSize(btnSize);
    saveButt->move(noteDeck.back()->pos() + QPoint(0, noteDeck.back()->size().height() + 20));
    connect(saveButt, SIGNAL(clicked()), base, SLOT(saveFile()));

    openButt = new QPushButton(this);
    openButt->setIcon(QIcon(path + "/img/open_btn.png"));
    openButt->setIconSize(btnSize);
    openButt->setFixedSize(btnSize);
    openButt->move(saveButt->pos() + QPoint(0, saveButt->size().height()));
    connect(openButt, SIGNAL(clicked()), base, SLOT(openFile()));

    playPauseButt = new QPushButton(this);
    playPauseButt->setIcon(QIcon(path + "/img/pause_play_btn.png"));
    playPauseButt->setIconSize(btnSize);
    playPauseButt->setFixedSize(btnSize);
    playPauseButt->move(openButt->pos() + QPoint(0, openButt->size().height()));
    connect(playPauseButt, SIGNAL(clicked()), base, SLOT(playPause()));

    stopButt = new QPushButton(this);
    stopButt->setIcon(QIcon(path + "/img/stop_btn.png"));
    stopButt->setIconSize(btnSize);
    stopButt->setFixedSize(btnSize);
    stopButt->move(playPauseButt->pos() + QPoint(0, playPauseButt->size().height()));
    connect(stopButt, SIGNAL(clicked()), base, SLOT(stop()));

    setColorButt = new QPushButton(this);
    setColorButt->setIcon(QIcon(path + "/img/color_btn.png"));
    setColorButt->setIconSize(btnSize);
    setColorButt->setFixedSize(btnSize);
    setColorButt->move(stopButt->pos() + QPoint(0, stopButt->size().height()));
    setColorButt->setCheckable(true);
    connect(setColorButt, SIGNAL(clicked(bool)), this, SLOT(setColor(bool)));
}

editor::~editor()
{
    delete trebleClef;
    delete bassClef;
    delete diese;
    for(action* a : journal)
        delete a;
    journal.clear();
}

void editor::paintEvent(QPaintEvent* pe)
{
    QPainter painter(this);
    painter.setPen(QPen(Qt::black, 3, Qt::SolidLine));

    painter.drawRect(this->rect());

    painter.setBrush(QBrush(Qt::white));
    painter.setPen(QPen(Qt::transparent, 3, Qt::SolidLine));
    painter.drawRect(field);

    painter.setPen(QPen(Qt::black, 3, Qt::SolidLine));
    painter.drawLine(field.left(), field.top(), field.left(), field.bottom());
    painter.drawLine(field.right(), field.top(), field.right(), field.bottom());

    int counter = k;
    painter.setPen(QPen(Qt::gray, 1, Qt::SolidLine));
    for(int x = noteSize.width() / 2; x < field.right() - field.left(); x += noteSize.width()){
        painter.drawLine(field.left() + x, field.top(), field.left() + x, field.bottom());
        if(counter % 16 == 0){
            painter.setPen(QPen(Qt::black, 3, Qt::SolidLine));
            painter.drawLine(field.left() + x, field.top(), field.left() + x, field.bottom());
            painter.drawText(field.left() + x, field.bottom() + 15, QString::number(counter / 16));
            painter.setPen(QPen(Qt::gray, 1, Qt::SolidLine));
        }
        ++counter;
    }
    counter = 1;

    for(int y = field.top() + lineGap/2; y < field.bottom(); y += lineGap){
        if(counter > 9 && counter < 21 && counter != 15){
            painter.setPen(QPen(Qt::black, 2, Qt::SolidLine));
        }else{
            painter.setPen(QPen(Qt::gray, 1, Qt::SolidLine));
        }
        painter.drawLine(field.left(), y, field.right(), y);
        ++counter;
    }
    painter.drawImage(0, 180, *trebleClef);
    painter.drawImage(0, 350, *bassClef);

    if (isPlaing) {
        painter.setBrush(QBrush(Qt::red));
        painter.setPen(QPen(Qt::red, 3, Qt::SolidLine));
        painter.drawLine(field.left(), field.top(), field.left(), field.bottom());
        painter.drawLine(field.left() + noteSize.width(), field.top(), field.left() + noteSize.width(), field.bottom());
        painter.drawLine(field.left(), field.top(), field.left() + noteSize.width(), field.top());
        painter.drawLine(field.left(), field.bottom(), field.left() + noteSize.width(), field.bottom());
    }
}

void editor::mouseMoveEvent(QMouseEvent *e)
{
    if(groupIN.empty()){ return; }

    moved = true;
    QPoint dt = QPoint(posPrev - e->pos());

    for(interactive_note *j : groupIN){
        j->move(j->pos() - dt);
    }
    posPrev = e->pos();
}

void editor::keyPress(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Control){
        ctrOn = true;
    }else if(e->key() == Qt::Key_Delete){
        dellGroup();
    }
    if(e->key() == Qt::Key_A && ctrOn){
        groupIN.clear();
        for(interactive_note* i : allIN){
            i->setFlag(true);
            groupIN.insert(i);
        }
        updateTempVec();
        tempFieldPos = k;
        qDebug() << "CTRL+A k " << k << " tempK " << tempFieldPos;
    }else if(e->key() == Qt::Key_Z && ctrOn && !journal.empty()){
        journal.back()->undo();
        delete  journal.back();
        journal.pop_back();
        qDebug() << "ctrl + z";
        updateTempVec();
        tempFieldPos = k;
        saved = false;
        qDebug() << "CTRL+Z k " << k << " tempK " << tempFieldPos;
    }
}

void editor::keyRelease(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Control){
        ctrOn = false;
    }
}

void editor::mousePressEvent(QMouseEvent *e)
{
    clearGroup();
    mouseOn = true;
    updateTempVec();
    tempFieldPos = k;
    qDebug() << "MOUSE PRESS ON FIELD k " << k << " tempK " << tempFieldPos;
}

void editor::mouseReleaseEvent(QMouseEvent *e)
{
    mouseOn = false;
    if(!groupIN.empty()){
        positioning();
        hideAndSeek();
        action* a = new adding(this);
        journal.push_back(a);
        saved = false;
        updateTempVec();
        tempFieldPos = k;
        qDebug() << "MOUSE RELEASE ON FIELD k " << k << " tempK " << tempFieldPos;
    }
}

void editor::moveField(int value)
{
    k += value;
    if(mouseOn){
        for(interactive_note* i : allIN){
            if(!i->getFlag()){
                i->move(i->pos().x() - (noteSize.width() * value), i->pos().y());
            }
        }
    }else{
        for(interactive_note* i : allIN){
            i->move(i->pos().x() - (noteSize.width() * value), i->pos().y());
        }
    }

    update();
    hideAndSeek();
}

void editor::wheelEvent(QWheelEvent *event)
{
    if (event->angleDelta().y() > 0){
        moveField(1);
    }
    else{
        moveField(-1);
    }
}

void editor::clearGroup()
{
    for(interactive_note *j : groupIN){
        j->setFlag(false);
    }
    groupIN.clear();
}

void editor::positioning()
{
    int X, Y;
    for(interactive_note *j : groupIN){
        X = step(j->pos().x(), noteSize.width());
        Y = step(j->pos().y(), lineGap/2);
        if(Y < field.top() - 3*noteSize.height()/5){
            Y = field.top() - 3*noteSize.height()/5;
        }else if(Y > field.bottom() - noteSize.height()){
            Y = field.bottom() - noteSize.height() + 1;
        }
        j->move(X, Y);
        j->setDiese(j->getIsBlack());
    }
}

void editor::dellGroup()
{
    if(!groupIN.empty()){
        action* a = new removal(this);
        journal.push_back(a);
        saved = false;
    }
    QVector<interactive_note*> temp;
    for(interactive_note* i : allIN){
        if(i->getFlag()){
            delete i;
        }else{
            temp.push_back(i);
        }
    }
    allIN = temp;
    groupIN.clear();
    updateTempVec();
    tempFieldPos = k;
    qDebug() << "DELL GROUP k " << k << " tempK " << tempFieldPos;
}

void editor::hideAndSeek()
{
    for(interactive_note* i : allIN){
        if(i->pos().x() < field.left() || i->pos().x() > field.right()){
            i->hide();
        }else{
            i->show();
        }
    }
}

QVector<smoll_note> editor::getSmollNoteVec()
{
    smoll_note temp;
    temp.duration = 16;
    temp.line = 0;
    temp.X = 0;
    temp.isBlack = false;
    QVector<smoll_note> smollNoteVec(allIN.size(), temp);
    for(int i = 0; i < allIN.size(); ++i){
        temp.duration = allIN[i]->getDuration();
        temp.line = allIN[i]->pos().y() / (lineGap / 2);
        temp.X = allIN[i]->pos().x() / noteSize.width();
        temp.isBlack = allIN[i]->getIsBlack();
        smollNoteVec[i] = temp;
    }
    return smollNoteVec;
}

QVector<smoll_note> editor::getCurCol()
{
    smoll_note temp;
    QVector<smoll_note> smollNoteVec;

    for(int i = 0; i < allIN.size(); ++i){
        // if in first column
        if (allIN[i]->pos().x() / noteSize.width() == 2) {
            temp.duration = allIN[i]->getDuration();
            temp.line = allIN[i]->pos().y() / (lineGap / 2);
            temp.X = allIN[i]->pos().x() / noteSize.width();
            temp.isBlack = allIN[i]->getIsBlack();
            smollNoteVec.push_back(temp);
        }
    }

    return smollNoteVec;
}

bool editor::hasRightNotes()
{

    for(int i = 0; i < allIN.size(); ++i){
        // if note right
        if (allIN[i]->pos().x() / noteSize.width() > 2) {
            return true;
        }
    }

    return false;
}

int editor::getFirstPos()
{
    int first = 9999999, x;
    if (allIN.isEmpty()) {
        return 0;
    }
    for(int i = 0; i < allIN.size(); ++i){
        x = allIN[i]->pos().x() / noteSize.width();
        if (first > x) {
            first = x;
        }
    }
    return first;
}

void editor::rewriteField(const QList<QVector<smoll_note> > &tempData)
{
    for(action* a : journal)
        delete a;
    journal.clear();
    saved = false;

    groupIN.clear();
    updateTempVec();
    tempFieldPos = k;
    qDebug() << "REWRITE FIELD k " << k << " tempK " << tempFieldPos;

    for(int i = 0; i < allIN.size(); ++i){
        delete allIN[i];
    }
    allIN.clear();
    interactive_note* newNote;
    int dur, count;
    for(int i = 0; i < tempData.size(); ++i){
        for(int j = 0; j < tempData[i].size(); ++j){
            dur = tempData[i][j].duration;
            count = 0;
            while(dur != 16){
                dur *= 2;
                ++count;
            }
            newNote = new interactive_note(&noteImg[count], tempData[i][j].duration, this);
            newNote->move(tempData[i][j].X * noteSize.width(), tempData[i][j].line * (lineGap / 2));
            newNote->setDiese(tempData[i][j].isBlack);
            newNote->show();
            allIN.push_back(newNote);
        }
    }
    hideAndSeek();
    update();
}

int editor::getK()
{
    return k;
}

void editor::setK(int k)
{
    this->k = k;
}

bool editor::isSaved()
{
    return saved;
}

bool editor::isColorized()
{
    return colorized;
}

void editor::setSavedFlag(bool value)
{
    saved = value;
}

const QImage *editor::getDieseImg()
{
    return diese;
}

const QImage *editor::getColorizedImg(int y, int dur)
{
    int line = y / (lineGap / 2) % colorArrL;
    int imgIndex = 0;
    while (dur != 16) {
        dur *= 2;
        ++imgIndex;
    }


    if (line < 0 || line > colorArrL - 1 || imgIndex < 0 || imgIndex > colorArrC - 1) {
        return diese;
    }

    return colorArrImg[line][imgIndex];
}

void editor::addNote(note_deck *d)
{
    mouseOn = true;
    clearGroup();
    posPrev = d->pos();
    moved = false;
    interactive_note* newNote = new interactive_note(d->getImg(), d->getDur(), this);
    newNote->move(posPrev);
    newNote->show();
    allIN.push_back(newNote);
    groupIN.insert(newNote);
    newNote->setFlag(true);

}

void editor::mousePress(interactive_note *i, QMouseEvent *e)
{
    posPrev = i->pos() + e->pos();
    moved = false;
    mouseOn = true;

    updateTempVec();
    tempFieldPos = k;
    qDebug() << "MOUSE PRESS ON NOTE k " << k << " tempK " << tempFieldPos;

    if(i->getFlag() || ctrOn){ return; }

    i->setFlag(true);
    if(!groupIN.empty()){ clearGroup(); }
    groupIN.insert(i);
    updateTempVec();
    tempFieldPos = k;
    qDebug() << "MOUSE PRESS ON NOTE POST k " << k << " tempK " << tempFieldPos;
}

void editor::mouseRelease(interactive_note *i, QMouseEvent *e)
{
    int newX = i->pos().x() + e->pos().x();
    mouseOn = false;
    if(ctrOn && !moved){
        i->setFlag(true);
        groupIN.insert(i);
        updateTempVec();
    }

    positioning();
    hideAndSeek();

    qDebug() << "k " << k << " tempK " << tempFieldPos << " groupSize " << groupIN.size() << " tempVecSize " << tempVecPress.size();

    if(groupIN.size() == tempVecPress.size()){
        bool check = false;
        int count = 0;
        for(interactive_note* i : groupIN){
            if(i->pos().x() != tempVecPress[count].X || i->pos().y() != tempVecPress[count].line || k != tempFieldPos){
                check = true;
                break;
            }
            ++count;
        }
        if(check){
            action* a = new moving(this);
            journal.push_back(a);
            saved = false;
        }
    }
}

void editor::dieseChange()
{
    action* a = new changeDies(this);
    journal.push_back(a);
    saved = false;
}

void editor::setColor(bool flag)
{
    colorized = flag;
    for (int i = 0; i < allIN.size(); ++i) {
        allIN[i]->update();
    }
}
