#include "authform.h"
#include "ui_authform.h"
#include "client_api.h"
#include <QAction>
#include <QIcon>

AuthForm::AuthForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AuthForm)
{
    ui->setupUi(this);

    // Включаем встроенную кнопку-глазик (показать/скрыть) внутри поля
    ui->lineEdit_password->setEchoMode(QLineEdit::Password); // Убеждаемся, что пароль скрыт
}

AuthForm::~AuthForm()
{
    delete ui;
}

void AuthForm::on_btn_login_clicked()
{
    // 1. Берем текст из твоих красивых полей
    QString login = ui->lineEdit_login->text();
    QString pass = ui->lineEdit_password->text();

    // 2. Склеиваем строку по протоколу препода (auth&login&password)
    QString request = "auth&" + login + "&" + pass;

    // 3. Отдаем Синглтону приказ отправить это на сервер!
    Client_API::getInstance().sendRequest(request);
}


void AuthForm::on_checkBox_showPass_toggled(bool checked)
{
    if (checked) {
        // Если галочка стоит - показываем буквы
        ui->lineEdit_password->setEchoMode(QLineEdit::Normal);
    } else {
        // Если галочки нет - прячем за точки
        ui->lineEdit_password->setEchoMode(QLineEdit::Password);
    }
}


void AuthForm::on_btn_register_clicked()
{
    emit signalOpenRegForm();
}


void AuthForm::on_btn_forget_password_clicked()
{
    emit signalOpenForgotForm();
}

