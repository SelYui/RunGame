//Основное окно с игрой
#ifndef WIDGET_H
#define WIDGET_H

#include <QApplication>
#include <QWidget>
#include <QDesktopWidget>

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>

#include <QShortcut>
#include <QTimer>
#include <QPushButton>
#include <QKeyEvent>

#include "hero.h"
#include "obstracles.h"
#include "distancescore.h"
#include "dinothread.h"
#include "dinoml.h"


QT_BEGIN_NAMESPACE
namespace Ui { class GameWidget; }
QT_END_NAMESPACE

class GameWidget : public QWidget
{
    Q_OBJECT

public:
    GameWidget(QWidget *parent = nullptr);
    ~GameWidget();
/*
    double getDistance() const;        // Функция для получения расстояния между героем до всех объектов на сцене
    double getSpeedGame() const;       // Функция для получения скорости игры
    double getJumpHero() const;        // Функция для получения координаты по оси Y героя
*/
    //Для определения разрешения экрана
    static int heightDesk;
    static int widthDesk;
    //публичные параметры для нейросети:высота препятствий, ширина препятствий, расстояние героя до препятствий
    static std::vector<double> hieghtObstr[10], widthObstr[10], Distance[10];
    QPixmap *dino_sprite;            // Картинки игры

private slots:
    void slotGameOver();                    // Слот конца игры (наткнулись на препятствие)
    void Start_Game_btn();                      // Слот принятия сигнала от кнопки Start
    void Exit_Game_btn();                       // Слот принятия сигнала от кнопки Exit
    void Training_Dino_btn();                   // Запуск по кнопке тренировки динозавриков
    void onGenerateObsracle();              // Гененрация препятствий
    void slotGameCount();                   // Увеличние счета игры

private:
    void showSettingButton();   // Показываем кнопки в графической сцене

    Ui::GameWidget  *ui;
    QGraphicsScene  *scene;         // Объявляем графическую сцену
    Hero            *hero;          // героя
    QTimer          *timer;         // таймер игры для обновления объектов
    QTimer          *timerHero;     /* Объявляем игровой таймер, благодаря которому
                                     * будет производиться изменения положения объекта на сцене
                                     * при воздействии на него клавишами
                                     * */
    QTimer          *timerCreateBarrier;    // Таймер для создания препятствий

    Obstracle       *obstracle;     // Объявлем препятствия
    QList<QGraphicsItem *> list_obstracles;          // Список со всеми препятствиями, присутствующими в игре
    QList<QGraphicsItem *> list_heroes;          // Список со всеми героями, присутствующими в игре

    double count;                   // Переменная, которая хранит счёт игре

    DinoML *dino_ML;                 // Обявление функции для постройки модели обученной играть в Дино
    DinoThread dino_thread;         // Объявляем потоки для создания в них героев

    DistanceScore   *nums_core;  //Счет в игре

    QPushButton     *btn_StartGame;     // Старт игры
    QPushButton     *btn_ExitGame;      // Выход из игры
    QPushButton     *btn_StartLearn;      // Выход из игры



};
#endif // WIDGET_H
