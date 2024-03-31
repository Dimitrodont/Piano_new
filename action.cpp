#include "action.h"

action::action(editor* base)
{
    this->base = base;
    fieldPos = base->k;
    smoll_note temp;
    for(interactive_note* i : base->groupIN){
        temp.X = i->pos().x();
        temp.line = i->pos().y();
        temp.duration = i->getDuration();
        temp.isBlack = i->getIsBlack();
        vec.push_back(temp);
    }
}

void adding::undo()
{
    base->moveField(fieldPos - base->k);
    base->clearGroup();
    for(smoll_note s : vec){
        int count = 0;
        for(interactive_note* i : base->allIN){
            if(s.X == i->pos().x() && s.line == i->pos().y()){
                delete i;
                base->allIN.erase(base->allIN.begin() + count);
                break;  // подразумевается, что ноты добавляются по одной!
            }
            ++count;
        }
    }
}

void removal::undo()
{
    base->moveField(fieldPos - base->k);
    base->clearGroup();
    int dur, count;
    interactive_note* newNote = nullptr;
    for(smoll_note s : vec){
        dur = s.duration;
        count = 0;
        while(dur != 16){
            dur *= 2;
            ++count;
        }
        newNote = new interactive_note(&base->noteImg[count], s.duration, base);
        newNote->move(s.X, s.line);
        newNote->setDiese(s.isBlack);
        newNote->setFlag(true);
        newNote->show();
        base->allIN.push_back(newNote);
        base->groupIN.insert(newNote);
    }
}

void changeDies::undo()
{
    base->moveField(fieldPos - base->k);
    base->clearGroup();
    for(smoll_note s : vec){
        for(interactive_note* i : base->allIN){
            if(s.X == i->pos().x() && s.line == i->pos().y()){
                i->setDiese(!s.isBlack);
                i->setFlag(true);
                base->groupIN.insert(i);
                i->update();
                break;  // подразумевается, что ноты добавляются по одной!
            }
        }
    }
}

moving::moving(editor *base) : action(base)
{
    prevVec = base->tempVecPress;
    prevFieldPos = base->tempFieldPos;
    qDebug() << "action " << vec.size();
}

void moving::undo()
{
    int count = 0;
    base->moveField(fieldPos - base->k);
    base->clearGroup();
    for(smoll_note s : vec){
        for(interactive_note* i : base->allIN){
            if(s.X == i->pos().x() && s.line == i->pos().y()){
                i->move(prevVec[count].X + base->noteSize.width() * (prevFieldPos - base->k), prevVec[count].line);
                i->setDiese(prevVec[count].isBlack);
                i->setFlag(true);
                base->groupIN.insert(i);
                ++count;
            }
        }
    }
    base->moveField(prevFieldPos - base->k);
}
