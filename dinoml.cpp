#include "gamewidget.h"
#include "dinoml.h"
#include <QDebug>

DinoML::DinoML(QObject *parent) : QObject(parent)
{
    qDebug() << this->thread()->currentThread();
}
