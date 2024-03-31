#ifndef PIANO_H
#define PIANO_H

#include <QWidget>
#include <QVector>
#include <QList>
#include <QDataStream>
#include <QFileDialog>
#include <QTimer>

#include "editor.h"
#include "key_box.h"
#include "manager_box.h"

#pragma once

class editor;
class key_box;
class manager_box;

struct smoll_note
{
    bool isBlack = false;
    int line, duration, X;
    friend QDataStream &operator<< (QDataStream &out, const smoll_note &n)
    {
        out << n.isBlack << n.line << n.duration << n.X;
        return out;
    }

    friend QDataStream &operator>> (QDataStream &in, smoll_note &n)
    {
        in >> n.isBlack >> n.line >> n.duration >> n.X;
        return in;
    }
};

bool smollNoteCompar(smoll_note a, smoll_note b);

class piano : public QWidget
{
    Q_OBJECT
    friend class editor;
    friend class key_box;
    friend class manager_box;
private:
    QTimer* timer = nullptr;
    bool isPlaing = false;
    editor* ed = nullptr;
    key_box* key = nullptr;
    manager_box* manager = nullptr;
    int tactDur = 1000;
    QString currentFileName = "unnamed.note";

    QList<QVector<smoll_note>> tempData;
    QVector<smoll_note> pressedNote;
    static const QVector<int> skipVec;

public:
    piano(QWidget *parent = nullptr);
    void keyPressEvent(QKeyEvent *e);
    void keyReleaseEvent(QKeyEvent *e);
    void setTempData();
    int getTactDur();
    void closeEvent(QCloseEvent *e);
    static int recalcBlackLine(int line);

public slots:
    bool saveFile();
    void setTactDur(const QString& str);
    void openFile();
    void playPause();
    void stop();
    void timerTick();

};

#endif // PIANO_H
