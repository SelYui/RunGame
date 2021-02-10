//инициализация всех окон
#include "gamewidget.h"
#include "ui_gamewidget.h"
#include "global.h"
#include "math.h"
#include "iostream"

int GameWidget::heightDesk, GameWidget::widthDesk;
std::vector<double> GameWidget::hieghtObstr[10], GameWidget::widthObstr[10], GameWidget::Distance[10];

GameWidget::GameWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GameWidget)
{
    ui->setupUi(this);

    // графическая сцена
    scene = new QGraphicsScene();       // Инициализируем графическую сцену

    ui->graphicsView->setScene(scene);  // Устанавливаем графическую сцену в graphicsView
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);                // Устанавливаем сглаживание
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);   // Отключаем скроллбар по вертикали
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // Отключаем скроллбар по горизонтали

    //вычисляем параметры экрана
    QDesktopWidget widget;
    QRect mainScreenSize;
    mainScreenSize = widget.availableGeometry(widget.primaryScreen());
    heightDesk = mainScreenSize.height();       // Высота экрана
    widthDesk = mainScreenSize.width();         // Ширина экрана
    scene->setSceneRect(0, 0, widthDesk, heightDesk);                                 // Устанавливаем область графической сцены (настроить в соответствии с экраном)

// Для удобства не забудь убрать
    scene->addLine(0,(heightDesk)/2,widthDesk,(heightDesk)/2,QPen(Qt::black));   // Добавляем горизонтальную линию через центр
    scene->addLine(widthDesk/2,0,widthDesk/2,heightDesk,QPen(Qt::black));   // Добавляем вертикальную линию через центр
    scene->addLine(0,(heightDesk-heightDesk/5),widthDesk,(heightDesk-heightDesk/5),QPen(Qt::black)); // Добавим линию дороги
    //scene->addLine(0,(heightDesk-100),widthDesk,(heightDesk-100),QPen(Qt::black)); // Добавим линию высоты прыжка

    scene->addRect(scene->sceneRect());    //добавим прямоугольник размером со сцену

    // Выставляем счет игры
    nums_core = new DistanceScore();
    scene->addItem(nums_core);     // Добавляем цифры на сцену

    // Создание кнопок на сцене
    btn_StartGame = new QPushButton("Start Game", ui->graphicsView);
    btn_ExitGame = new QPushButton("Exit Game", ui->graphicsView);
    btn_StartLearn = new QPushButton("Training Dino", ui->graphicsView);
    showSettingButton();                                                // Выставляю кнопки
    btn_StartGame->show();     //показываем кнопки
    btn_ExitGame->show();
    btn_StartLearn->show();
    btn_StartGame->setFocus();

    //hero = new Hero();      // Инициализируем героя игры

    // Инициализация таймера для обновления положения элементов игры и счета игры
    timer = new QTimer();

    // Инициализация таймера создания объектов
    timerCreateBarrier = new QTimer();
}

void GameWidget::showSettingButton()
{
    int width, height;      //Ширина и высота выджетов

    // Расположение кнопок на сцене
    width = widthDesk/8;
    height = heightDesk/20;
    btn_StartGame->setGeometry({widthDesk/2 - width - 5,
                               (heightDesk)/2,
                               width,
                               height});
    btn_ExitGame->setGeometry({widthDesk/2 - width + btn_StartGame->geometry().width() + 2 + 5,
                               (heightDesk)/2,
                               width,
                               height});
    btn_StartLearn->setGeometry({widthDesk/2 - width - 5,
                                 (heightDesk)/2 + btn_StartGame->geometry().height() + 2 + 5,
                                 width,
                                 height});

    // задаем стиль виджетов на сцене
    ui->graphicsView->setStyleSheet(
                      "QPushButton {"
                      "font-size: 20pt;"        //размер шрифта
                      "font-family: MS Serif;"  //шрифт
                      "background-color: red;"  //цвет
                      "border-style: outset;"
                      "border-width: 2px;"      //толщина рамки
                      "border-color: darkRed;"     //цвет рамки
                      "border-radius: 20px;"    //радиу закругления кнопки
                      "}"
                      "QPushButton:pressed {"
                      "background-color: darkRed;"
                      "border-style: insert;"
                      "}"
                      "QPushButton:default {"
                      "border-color: navy;"
                      "}");

    // Вызываем слот обработки сигнала нажатия кнопки.
    connect(btn_StartGame, &QPushButton::clicked, this, &GameWidget::Start_Game_btn);
    connect(btn_ExitGame, &QPushButton::clicked, this, &GameWidget::Exit_Game_btn);
    connect(btn_StartLearn, &QPushButton::clicked, this, &GameWidget::Training_Dino_btn);

    btn_StartGame->setFocus();
}

void GameWidget::Start_Game_btn()
{
    btn_StartGame->hide();     //скрываю кнопки
    btn_ExitGame->hide();
    btn_StartLearn->hide();

    //удаляем все препятствия со сцены
    foreach(QGraphicsItem *obstracle, list_obstracles) {
        scene->removeItem(obstracle);           // Удаляем со сцены
        list_obstracles.removeOne(obstracle);   // Удаляем из списка
        delete obstracle;                       // Вообще удаляем
    }

    //удаляем всех героев со сцены
    foreach(QGraphicsItem *hero, list_heroes) {
        scene->removeItem(hero);           // Удаляем со сцены
        list_heroes.removeOne(hero);   // Удаляем из списка
        delete hero;                       // Вообще удаляем
    }

    hero = new Hero();  //Инициализируем нового героя

    scene->addItem(hero);     // Добавляем героя на сцену
    hero->setPos(widthDesk/10, heightDesk - heightDesk/5);        // Устанавливаем героя в начало сцены
    list_heroes.append(hero);  // добовляем героя в список препятствий

    // Начальные настройки
    speedGame = 3;          //Скорость игры
    count = 0;              //Счет игры

    //Обработка нажатия клавиши
    connect(timer, &QTimer::timeout, hero, &Hero::slotPressKey);

    //подключаем создание наших объектов раз в 3 секунды +- рандом
    connect(timerCreateBarrier, SIGNAL(timeout()), this, SLOT(onGenerateObsracle()));
    timerCreateBarrier->start(3000);

    //подключаем счетчик счета
    connect(timer, SIGNAL(timeout()), this, SLOT(slotGameCount()));
    //подключаем обновление экрана с объектом
    connect(timer, SIGNAL(timeout()), scene, SLOT(advance()));
    timer->start(1000/120);     //обработка - 120 кадров в секунду

    // Подключаем сигнал от Героя, в котором передаются Объекты, на которые он наткнулся
    connect(hero, &Hero::signalCheckItem, this, &GameWidget::slotGameOver);
}

void GameWidget::Training_Dino_btn()
{
//    btn_StartGame->hide();     //скрываю кнопки
//    btn_ExitGame->hide();
//    btn_StartLearn->hide();

    //удаляем все препятствия со сцены
    foreach(QGraphicsItem *obstracle, list_obstracles) {
        scene->removeItem(obstracle);           // Удаляем со сцены
        list_obstracles.removeOne(obstracle);   // Удаляем из списка
        delete obstracle;                       // Вообще удаляем
    }

    //удаляем всех героев со сцены
    foreach(QGraphicsItem *hero, list_heroes) {
        scene->removeItem(hero);           // Удаляем со сцены
        list_heroes.removeOne(hero);   // Удаляем из списка
        delete hero;                       // Вообще удаляем
    }

    dino_ML = new DinoML();

    // Создаем поток и передаем туда нашу модель
    dino_ML->moveToThread(&dino_thread);
    dino_thread.start();
}

void GameWidget::onGenerateObsracle()
{
    int time;
    time = 6000/(rand()%(static_cast<int>(speedGame)) + 2);

    //отключаем таймер создания объекта, создаем объект, включаем таймер заного
    //disconnect(timerCreateBarrier, SIGNAL(timeout()), this, SLOT(onGenerateObsracle()));
    timerCreateBarrier->stop();
    //строим препятствия с разброссом в ширину области сцены
    obstracle = new Obstracle();        // инициализируем объект
    scene->addItem(obstracle);          // добавляем объект на сцену
    obstracle->setZValue(-1);           // помещаем препятствие ниже Героя
    list_obstracles.append(obstracle);  // добовляем препятствие в список препятствий
    //Включаем таймер
    timerCreateBarrier->start(time);    // Пусть будет такой диапазон создания препятствий
}

void GameWidget::Exit_Game_btn()
{
    //выход из программы
    QApplication::quit();
}


void GameWidget::slotGameCount()
{
    // TEH проверка нажатия Esc
    if(GetAsyncKeyState(VK_ESCAPE)){
        GameWidget::slotGameOver();
    }

    int i=0;    //для заполнения элементов вектора

    count += speedGame;
    nums_core->ShowScoreTimer(static_cast<int>(count)/100);

    speedGame += 0.0001;
    qDebug() << speedGame;

    foreach(QGraphicsItem *obstracle, list_obstracles) {
        // получаем расстояние каждого объекта
        Obstracle *obstr = static_cast<Obstracle *>(obstracle);
        //std::cout<< "distance_widg = \t" << obstr->getDistance() << '\t' << obstr->getWidthObstr() << '\t' << obstr->getHeightObstr() << std::endl;
        //Distance[i] = obstracle->pos().x() - hero->pos().x();
        //std::cout<< "distance_widg = \t" << obstracle->pos().x() << '\t' << hero->pos().x() << '\t' << obstr->getWidthObstr() << '\t' << obstr->getHeightObstr() << std::endl;
        // удаляем далекоудаленные объекты
        if(obstracle->pos().x() < -widthDesk/2)
        {
            scene->removeItem(obstracle);           // Удаляем со сцены
            list_obstracles.removeOne(obstracle);   // Удаляем из списка
            delete obstracle;                       // Вообще удаляем
        }
    }

    //std::cout<< count << "\t" << speedGame << "\t" << static_cast<int>(count) << "\t" << static_cast<int>(speedGame) << std::endl;
}

void GameWidget::slotGameOver()
{
    timer->stop();
    timerCreateBarrier->stop();
    //это будет при проигреше
    btn_StartGame->show();     //показываем кнопки
    btn_ExitGame->show();     //показываем кнопки
    btn_StartLearn->show();
    btn_StartGame->setFocus();

    //Отключаем все сигналы от слотов
    disconnect(timer, SIGNAL(timeout()), scene, SLOT(advance()));
    disconnect(timer, SIGNAL(timeout()), this, SLOT(slotGameCount()));
    disconnect(timerCreateBarrier, SIGNAL(timeout()), this, SLOT(onGenerateObsracle()));
    disconnect(timer, &QTimer::timeout, hero, &Hero::slotPressKey);

//    // Удаляем все препятствия
//    foreach(QGraphicsItem *obstracle, list_obstracles) {
//        scene->removeItem(obstracle);           // Удаляем со сцены
//        list_obstracles.removeOne(obstracle);   // Удаляем из списка
//        delete obstracle;                       // Вообще удаляем
//    }
}

/*
double GameWidget::getDistance() const
{}
double GameWidget::getSpeedGame() const
{

}
double GameWidget::getJumpHero() const
{
    return hero->pos().y();
}
*/
GameWidget::~GameWidget()
{
    delete ui;
    delete scene;
    delete btn_StartGame;
    delete btn_ExitGame;
    delete btn_StartLearn;
    delete timer;
    delete timerCreateBarrier;
}
