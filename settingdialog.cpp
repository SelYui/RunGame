//обработка нажатия кнопок в окне настроек
#include <settingdialog.h>
#include <ui_settingdialog.h>
#include <QMessageBox>

SettingDialog::SettingDialog(QDialog *parent) :
    QDialog(parent),
    ui(new Ui::SettingDialog)
{
    ui->setupUi(this);
}

SettingDialog::~SettingDialog()
{
    delete ui;
}

void SettingDialog::on_startButton_clicked()
{
    this->close();      //закрываем это окно
    emit gameWindow();  //вызываем сигнал открытия окна с игрой
}


void SettingDialog::on_exitButton_clicked()
{
    //выход из программы
    QApplication::quit();
}
