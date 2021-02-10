#ifndef HERO_H
#define HERO_H

#include <QObject>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QPainter>
#include <QDesktopWidget>
#include <QImage>

/* Подключаем библиотеку, отвечающую за использование WinAPI
 * Данная библиотека необходима для асинхронной проверки состояния клавиш
 * */
#include <windows.h>

class Hero : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    explicit Hero(QObject *parent = nullptr);
    ~Hero();
    void MovingHero(int pressKey);           // Обработку перемещения героя

signals:
    /* Сигнал, который передаётся в ядро игры с элементом QGraphicsItem,
     * объект на который наткнулся герой, и требуется принять решение о том,
     * что с этим элементом делать.
     * */
    void signalCheckItem(QGraphicsItem *item);

public slots:
    void slotPressKey();            // Слот, который обрабатывает нажатие клавишь и вызывает функцию управление героем

protected:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    void MovingHeroScene();

    // Для прыжка
    int down_border;        // Нижняя граница - земля, по которой бежит герой
    double speed, V0, t;    // Скорость и время, для вычисления равнозамедленного движения
    double g = 9.81;        // Ускорение свободного падения
    int step;               // Признак положения героя: 0- бежит, 1- летит вверх/вниз, 2- смерть, 3- наклонен вниз

    // Высота и ширина героя
    int heightHero;
    int widthHero;

    //Для отрисовки героя
    QImage *dino_sprite;
    //QPixmap dino_sprite;
    QPixmap mask;
    bool meandr;  //для изображения перемещения переключаем ее то на 0, то на 1
    int countForSteps;  // Счётчик для отсчета тиков таймера, при которых мы переключаем изображение
    int pressKey;       // Признак нажатия клавиши в игре

    //таблица координат героя
    const int top_hero = 2;
    const int bot_hero = 95;
    const int left_hero[8] = {1338,1426,1514,1602,1690,1778,1866,1984};
    const int right_hero[8] = {1425,1513,1601,1689,1777,1865,1983,2101};
};

#endif // HERO_H
