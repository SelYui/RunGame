#ifndef DISTANCESCORE_H
#define DISTANCESCORE_H

#include <QObject>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QPainter>
#include <QDesktopWidget>
#include <QPixmap>
#include <QDebug>

const int N = 6;    //число цифр в счете
const int W = 21;    //максимальная ширина одной цифры

class DistanceScore : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    explicit DistanceScore(QObject *parent = nullptr);
    ~DistanceScore();
    void ShowScoreTimer(int score);           // Слот, который отвечает за обработку изменения счета

protected:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    // Высота и ширина цифры
    int heightScore;
    int widthScore;

    // Счет в игре
    int score;
    int number[N];

    //Для отрисовки счета
    QPixmap *score_sprite;

    //таблица координат цифр
    const int top_score = 2;
    const int bot_score = 23;
    const int left_score[10] = {954,976,994,1014,1034,1054,1074,1094,1114,1134};
    const int right_score[10] = {972,992,1012,1032,1052,1072,1092,1112,1132,1152};
};

#endif // DISTANCESCORE_H
