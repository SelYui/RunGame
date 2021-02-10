#include "distancescore.h"
#include "gamewidget.h"

DistanceScore::DistanceScore(QObject *parent) :
    QObject(parent), QGraphicsItem()
{
    //начальные значения
    score = 0;
    for(int i=0; i<N; i++){
        number[i] = 0;
    }
    score_sprite = new QPixmap(":/images/offline-sprite-2x.png");      //берем препятствия из этой картинки
    widthScore = right_score[score] - left_score[score];
    heightScore = bot_score - top_score;

    setPos((GameWidget::widthDesk*8/10), GameWidget::heightDesk/10);
}

QRectF DistanceScore::boundingRect() const
{
    return QRectF(0, -heightScore, W*N, bot_score-top_score);          // Ограничиваем область, в которой лежит герой
}

// Рисуем героя
void DistanceScore::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    for(int i=0; i<N; i++){
        widthScore = right_score[number[N-1-i]] - left_score[number[N-1-i]];
        heightScore = bot_score - top_score;
        painter->drawPixmap(W*i, -heightScore,*score_sprite,left_score[number[N-1-i]],top_score,widthScore,heightScore);
    }
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

void DistanceScore::ShowScoreTimer(int score)
{
    if(score == 0){
        for(int i=0; i<N; i++){
            number[i] = 0;
        }
    }
    else{
        int i = 0;
        while(score)
        {
            number[i++] = score%10;
            score /= 10;
        }
    }
    update(QRectF(-20,-20, W*N+40, heightScore+20));         // обновляем рисунок
}

DistanceScore::~DistanceScore()
{
    delete score_sprite;
}
