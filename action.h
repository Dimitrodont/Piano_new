#ifndef ACTION_H
#define ACTION_H

#include "editor.h"
#include "piano.h"
#include "interactive_note.h"
#include <QVector>

#pragma once

struct smoll_note;


class action
{
public:
    action(editor* base);
    virtual void undo(){}

protected:
    int fieldPos = 0;
    editor* base = nullptr;
    QVector<smoll_note> vec;
};

class adding : public action
{
public:
    adding(editor* base) : action(base){}
    void undo() override;
};

class removal : public action
{
public:
    removal(editor* base) : action(base){}
    void undo() override;
};

class changeDies : public action
{
public:
    changeDies(editor* base) : action(base){}
    void undo() override;
};

class moving : public action
{
public:
    moving(editor* base);
    void undo() override;
private:
    QVector<smoll_note> prevVec;
    int prevFieldPos;
};

#endif // ACTION_H
