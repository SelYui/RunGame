#ifndef OBSTRACLES_H
#define OBSTRACLES_H

#include <QGraphicsPixmapItem>
#include <QPixmap>

class Obstracle : public QGraphicsPixmapItem{
public:
    Obstracle();
    ~Obstracle();
    void advance(int phase);
    int getHeightObstr() const;     // Функция для получения высоты препятствия
    int getWidthObstr() const;      // Функция для получения ширины препятствия

private:
    int heightObstr;
    int widthObstr;
    double distance;         // Сюда записываем вычисленное растояние

    int numb_obstr;  //Номер используемой картинки
    QPixmap *im_obstr;

    //таблица координат препятствий
    const int top_obstr = 2;
    const int bot_obstr[2] = {71,101};
    const int left_obstr[7] = {446,480,548,652,702,802,850};
    const int right_obstr[7] = {479,547,649,701,801,849,951};
};

#endif // OBSTRACLES_H
