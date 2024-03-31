#include "manager_box.h"

manager_box::manager_box(piano *parent) : QWidget(parent)
{
    base = parent;

    mainLayout = new QVBoxLayout;
    upperLay = new QHBoxLayout;
    bottomLay = new QHBoxLayout;

    toLeft = new QPushButton("<<");
    toLeft->setEnabled(false);
    octavesInfo = new QLabel(octavesNams[0] + " - " + octavesNams[1]);
    octavesInfo->setFixedWidth(180);
    octavesInfo->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    toRight = new QPushButton(">>");

    postVolSl = new QSlider(Qt::Horizontal);
    postVolSl->setFixedWidth(300);
    postVolSl->setMaximum(100);
    postVolSl->setValue(30);
    postVolInfo = new QLabel;
    postVolInfo->setText(QString::number(postVolSl->value()) + "%");
    postVolInfo->setFixedWidth(30);
    postVolName = new QLabel("Продолжительность");

    volSl = new QSlider(Qt::Horizontal);
    volSl->setFixedWidth(300);
    volSl->setMaximum(100);
    volSl->setValue(100);
    volInfo = new QLabel;
    volInfo->setText(QString::number(volSl->value()) + "%");
    volInfo->setFixedWidth(30);
    volName = new QLabel("Громкость");

    tactDurName = new QLabel("Такт (мс)");
    tactDur = new QLineEdit(this);
    tactDur->setText(QString::number(base->getTactDur()));
    tactDur->setFixedWidth(100);

    upperLay->addWidget(postVolName);
    upperLay->addWidget(postVolSl);
    upperLay->addWidget(postVolInfo);
    upperLay->addWidget(volName);
    upperLay->addWidget(volSl);
    upperLay->addWidget(volInfo);
    upperLay->addStretch();

    bottomLay->addWidget(toLeft);
    bottomLay->addWidget(octavesInfo);
    bottomLay->addWidget(toRight);
    bottomLay->addSpacing(50);
    bottomLay->addWidget(tactDurName);
    bottomLay->addWidget(tactDur);
    bottomLay->addStretch();

    mainLayout->addLayout(upperLay);
    mainLayout->addLayout(bottomLay);
    setLayout(mainLayout);

    connect(tactDur, SIGNAL(textChanged(const QString&)), base, SLOT(setTactDur(const QString&)));
    connect(postVolSl, SIGNAL(valueChanged(int)), this, SLOT(postVolumeChanged(int)));
    connect(volSl, SIGNAL(valueChanged(int)), this, SLOT(volumeChanged(int)));
}

void manager_box::setTactDur(QString str)
{
    tactDur->setText(str);
}

void manager_box::shiftChanged(int shiftW)
{
    if(shiftW != 44){
        toRight->setEnabled(true);
    }else{
        toRight->setEnabled(false);
    }
    if(shiftW != -5){
        toLeft->setEnabled(true);
    }else{
        toLeft->setEnabled(false);
    }

    octavesInfo->setText(octavesNams[(shiftW+6)/7] + " - " + octavesNams[1 + (shiftW+6)/7]);
}

void manager_box::postVolumeChanged(int volume)
{
    postVolInfo->setText(QString::number(volume) + "%");
}

void manager_box::volumeChanged(int volume)
{
    volInfo->setText(QString::number(volume) + "%");
}

