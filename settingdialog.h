#ifndef SETTINGWINDOW_H
#define SETTINGWINDOW_H

#include <QDialog>

namespace Ui { class SettingDialog; }

class SettingDialog : public QDialog
{
    Q_OBJECT

public:
    SettingDialog(QDialog *parent = nullptr);
    ~SettingDialog();

signals:
    void gameWindow();  //сигнал на открытие первого окна с игрой

private slots:
    // Слоты обработки нажатия кнопки
    void on_startButton_clicked();

    void on_exitButton_clicked();

private:
    Ui::SettingDialog *ui;

};

#endif // SETTINGWINDOW_H
