#include "gamewidget.h"
#include "hero.h"
#include "global.h"
#include <QDebug>

Hero::Hero(QObject *parent) :
    QObject(parent), QGraphicsItem()
{
    //начальные настройки
    step = 1;
    countForSteps = 0;
    meandr = 0;
    widthHero = right_hero[step*2+meandr] - left_hero[step*2+meandr];
    heightHero = bot_hero - top_hero;
    pressKey = VK_SPACE;

    down_border = GameWidget::heightDesk - GameWidget::heightDesk/5;
    dino_sprite = new QImage(":/images/offline-sprite-2x.png");      //берем препятствия из этой картинки
    //dino_sprite = QPixmap(":/images/offline-sprite-2x.png");      //берем препятствия из этой картинки
    //mask = QPixmap::fromImage(dino_sprite->createAlphaMask());
    //dino_sprite->fill(QColor(Qt::red));
    //dino_sprite->setColor();
}

QRectF Hero::boundingRect() const
{
    return QRectF(0, -heightHero, widthHero+20, heightHero+20);          // Ограничиваем область, в которой лежит герой
}

// Рисуем героя
void Hero::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    widthHero = right_hero[step*2+meandr] - left_hero[step*2+meandr];
    heightHero = bot_hero - top_hero;
    painter->drawImage(0, -heightHero,*dino_sprite,left_hero[step*2+meandr],top_hero,widthHero,heightHero);
    //painter->drawPixmap(0, -heightHero,mask,left_hero[step*2+meandr],top_hero,widthHero,heightHero);
    //painter->setBrush(Qt::red);

//    //технология для отрисовки полигона столкновения
//    painter->setPen(QPen(Qt::red, 1));
//    QPolygon polygon;
//    if(step != 3)
//        polygon << QPoint(5,-heightHero/3) << QPoint(5, -heightHero) << QPoint(widthHero, -heightHero) << QPoint(widthHero, -heightHero*2/3) << QPoint(widthHero/2, 0);
//    else
//        polygon << QPoint(5,-heightHero/3) << QPoint(5, -heightHero*2/3) << QPoint(widthHero, -heightHero*2/3) << QPoint(widthHero, -heightHero/3) << QPoint(widthHero/2, 0);
//    //painter->setBrush(Qt::red);     /// Устанавливаем кисть, которой будем отрисовывать объект
//    painter->drawPolygon(polygon);
//    if(step == 2){
//        polygon << QPoint(5,-heightHero/3) << QPoint(5, -heightHero) << QPoint(widthHero, -heightHero) << QPoint(widthHero, -heightHero*2/3) << QPoint(widthHero/2, 0);
//    painter->setBrush(Qt::red);     // Устанавливаем кисть, которой будем отрисовывать объект
//    painter->drawPolygon(polygon);}
//    qDebug() << heightHero << " " << widthHero;
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

void Hero::slotPressKey()
{
    // Проверяем, нажата ли была какая-либо из кнопок управления объектом
    if(GetAsyncKeyState(VK_SPACE) || GetAsyncKeyState(VK_UP)){
        pressKey = VK_SPACE;
    }
    else if(GetAsyncKeyState(VK_DOWN)){
        pressKey = VK_DOWN;}
    else
        pressKey = 0;
    //и организуем движение по ним
    MovingHeroScene();
}

void Hero::MovingHeroScene()
{
    // При нажатии пробела, и организуем прыжок
    if(pressKey == VK_SPACE && step == 1){
        step = 0;
        // начальные настройки
        t = 0;
        speed = -GameWidget::heightDesk;
    }
    if(pressKey == VK_DOWN)  // При нажатии на стрелку, нагибаемся и ускоряем падение
        step = 3;
    else if(step == 3)
        step = 0;

    switch(step){
    case 0:{
        speed = speed + g*(t/20);
        t++;
        setPos(mapToParent(0, speed/200));      // обновляем положение героя в сцене
        break;}
    case 3:{// если в воздухе, ускоряем падение
        speed = speed + g*(t/5);
        t++;
        setPos(mapToParent(0, speed/200));      // обновляем положение героя в сцене
        break;}
    default:{
        break;}
    }
    //проверка на выход за границу снизу
    if(this->y() > (down_border)){
        this->setY(down_border);
        // при достижении земли переходим в состояние бега (если не нагнулись)
        if(step != 3)
            step = 1;
    }
    //переключаем meandr для переключения изображений
    countForSteps++;
    if(countForSteps == 24)
        meandr = 1;
    else if(countForSteps == 24*2){
        meandr = 0;
        countForSteps = 0;
    }
    update(QRectF(-20,-heightHero-20, widthHero+20, heightHero+20));         // обновляем рисунок

    //Проверка на столкновение с объектом
    QPolygonF polygon;  //создаем полигон вокруг динозаврика, который будем проверять на столкновение с объектом
    if(step != 3) //для наклона свой, отдельный полигон
        polygon = QPolygonF()
                        << mapToScene(5,-heightHero/3)
                        << mapToScene(5, -heightHero)
                        << mapToScene(widthHero, -heightHero)
                        << mapToScene(widthHero, -heightHero*2/3)
                        << mapToScene(widthHero/2, 0);
    else
        polygon = QPolygonF()
                        << mapToScene(5,-heightHero/3)
                        << mapToScene(5, -heightHero*2/3)
                        << mapToScene(widthHero, -heightHero*2/3)
                        << mapToScene(widthHero, -heightHero/3)
                        << mapToScene(widthHero/2, 0);
    QList<QGraphicsItem *> foundItems = scene()->items(polygon);
    /* Проверяем все элементы.
     * Если это препятствие - высылаем сигнал в ядро игры
     * иначе, ничего не делаем
     * */
    foreach(QGraphicsItem *item, foundItems) {
        if (dynamic_cast<Obstracle*> (item)){
            step = 2; //картинка смерти
            emit signalCheckItem(item);
        }
        else
            continue;
    }
}

Hero::~Hero()
{
    delete dino_sprite;
}
