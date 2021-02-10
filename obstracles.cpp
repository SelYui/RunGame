#include "obstracles.h"
#include "global.h"
#include "gamewidget.h"
#include "math.h"

Obstracle :: Obstracle(): QGraphicsPixmapItem()   //QGraphicsRectItem()
{
    numb_obstr = rand()%7 + 1;  // рандомно выбираем одну из 7 фигур для препятствий
    im_obstr = new QPixmap(":/images/offline-sprite-2x.png");      //берем препятствия из этой картинки

    if(numb_obstr < 4)
        heightObstr = bot_obstr[0]-top_obstr;
    else
        heightObstr = bot_obstr[1]-top_obstr;

    widthObstr = right_obstr[numb_obstr-1]-left_obstr[numb_obstr-1];
    setPixmap(im_obstr->copy(left_obstr[numb_obstr-1],top_obstr,widthObstr,heightObstr));

    // выставляем ее на сцену
    setPos(GameWidget::widthDesk, GameWidget::heightDesk - GameWidget::heightDesk/5 - heightObstr);
}

//покадровое изменение нашего препятствия. Вызывается дважды
void Obstracle::advance(int phase)
{
    if(phase)
    {
        //указываем смещение нашего объекта по нужной оси
        moveBy(-(speedGame), 0);

        // вычисляем расстояние до героя
        //QLineF lineToTarget(QPointF(0, 0), mapFromItem(target, 0, 0));
        //distance = lineToTarget.length();
        //std::cout<< "distanc = \t" << lineToTarget.length() << '\t' << distanc << std::endl;
    }
}

int Obstracle::getHeightObstr() const
{
    // возвращаем высоту препятствия
    return heightObstr;
}

int Obstracle::getWidthObstr() const
{
    // возвращаем высоту препятствия
    return widthObstr;
}

Obstracle :: ~Obstracle()
{
    delete im_obstr;
}
