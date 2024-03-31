#ifndef EDITOR_H
#define EDITOR_H

#include <QPainter>
#include <QtAlgorithms>
#include <QPushButton>
#include <QDir>
#include <QMouseEvent>
#include <QLabel>

#include "interactive_note.h"
#include "note_deck.h"
#include "piano.h"
#include "action.h"

#pragma once

class piano;
class interactive_note;
class note_deck;
class action;
struct smoll_note;

class editor : public QWidget
{
    Q_OBJECT
    friend class action;
    friend class adding;
    friend class removal;
    friend class changeDies;
    friend class moving;
    friend class piano;

private:
    piano* base = nullptr;

    int marg = 10, minDur = 16, k = 0;
    QRect field;
    QVector<interactive_note*> allIN;
    QSet<interactive_note*> groupIN;
    QVector<QImage> noteImg;
    bool ctrOn = false;
    bool mouseOn = false;
    bool moved = false;
    bool saved = true;
    bool isPlaing = false;
    bool colorized = false;
    QPoint posPrev;
    QSize noteSize;
    QVector<note_deck*> noteDeck;
    QImage *trebleClef = nullptr;
    QImage *bassClef = nullptr;
    QImage *diese = nullptr;
    QPushButton *saveButt = nullptr;
    QPushButton *openButt = nullptr;
    QPushButton *playPauseButt = nullptr;
    QPushButton *stopButt = nullptr;
    QPushButton *setColorButt = nullptr;
    QList<action*> journal;
    QVector<smoll_note> tempVecPress;
    int tempFieldPos;
    static const int colorArrL = 7; // соответствует числу линий и цветов
    const char colorsCh[colorArrL] = {'r', 'p', 'b', 'l', 'g', 'y', 'o'};
    static const int colorArrC = 5; // соответствует числу нот разной продолжительности
    QImage *colorArrImg[colorArrL][colorArrC];

    int step(int target, int stepRange);
    void updateTempVec();

public:
    editor(piano *parent = nullptr);
    ~editor();
    void paintEvent(QPaintEvent* pe);
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void moveField(int value);
    void wheelEvent(QWheelEvent* event);
    void keyPress(QKeyEvent *e);
    void keyRelease(QKeyEvent *e);
    void clearGroup();
    void positioning();
    void dellGroup();
    void hideAndSeek();
    QVector<smoll_note> getSmollNoteVec();
    QVector<smoll_note> getCurCol();
    bool hasRightNotes();
    int getFirstPos();
    void rewriteField(const QList<QVector<smoll_note>> &tempData);
    int getK();
    void setK(int k);
    bool isSaved();
    bool isColorized();
    void setSavedFlag(bool value);
    const QImage* getDieseImg();
    const QImage* getColorizedImg(int y, int dur);

    const int lineGap = 20;

public slots:
    void addNote(note_deck* d);
    void mousePress(interactive_note *i, QMouseEvent *e);
    void mouseRelease(interactive_note *i, QMouseEvent *e);
    void dieseChange();
    void setColor(bool flag);
};

#endif // EDITOR_H
