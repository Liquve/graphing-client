#include "regform.h"
#include "ui_regform.h"
#include "client_api.h"
#include <QMessageBox>
#include <vector>
#include <string>
#include <QRegularExpression>          // Библиотека для правил (регулярок)
#include <QRegularExpressionValidator> // Библиотека для ограничений ввода

RegForm::RegForm(QWidget *parent) : QWidget(parent), ui(new Ui::RegForm) {
    ui->setupUi(this);
    connect(ui->btn_exit, &QPushButton::clicked, this, [this](){ emit signalExit(); });
    // 1. Правило для Email: разрешаем буквы, цифры, точки, тире и ОБЯЗАТЕЛЬНО одну собачку
    QRegularExpression emailRegex("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$");
    QRegularExpressionValidator *emailValidator = new QRegularExpressionValidator(emailRegex, this);
    ui->lineEdit_email->setValidator(emailValidator);

    // 2. Правило для Логина: разрешаем всё, КРОМЕ символа @
    // Регулярка [^@]* означает "любые символы в любом количестве, кроме @"
    QRegularExpression loginRegex("^[^@]*$");
    QRegularExpressionValidator *loginValidator = new QRegularExpressionValidator(loginRegex, this);
    ui->lineEdit_login->setValidator(loginValidator);
}

RegForm::~RegForm()
{
    delete ui;
}

void RegForm::on_btn_back_clicked()
{
    emit signalOpenAuthForm();
}

void RegForm::on_checkBox_showPass_toggled(bool checked)
{
    QLineEdit::EchoMode mode = checked ? QLineEdit::Normal : QLineEdit::Password;
    ui->lineEdit_password->setEchoMode(mode);
    ui->lineEdit_password_repeat->setEchoMode(mode);
}

void RegForm::on_btn_register_clicked() {
    QString login = ui->lineEdit_login->text().trimmed();
    QString email = ui->lineEdit_email->text().trimmed();
    QString pass = ui->lineEdit_password->text();
    QString passConfirm = ui->lineEdit_password_repeat->text();

    // 1. Простая проверка на пустоту
    if (login.isEmpty() || email.isEmpty() || pass.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Заполните все поля!");
        return;
    }

    // 2. Проверка: является ли введенный email корректным (есть ли точка, домен и т.д.)
    // (Повторно проверяем валидатором, так как он может быть в состоянии "частично введен")
    int pos = 0; // Создаем переменную для позиции (обязательно для функции validate)
    if (ui->lineEdit_email->validator()->validate(email, pos) != QValidator::Acceptable) {
        QMessageBox::warning(this, "Ошибка", "Введите корректный адрес почты (например, user@mail.ru)");
        return;
    }

    // Дополнительная защита: проверяем, не ввел ли пользователь почту в поле логина
    if (login.contains("@")) {
        QMessageBox::warning(this, "Ошибка", "Логин не может содержать символ @. Используйте просто имя.");
        return;
    }

    // 3. Проверка паролей
    if (pass != passConfirm) {
        QMessageBox::warning(this, "Ошибка", "Пароли не совпадают!");
        return;
    }

    // Отправка на сервер
    Client_API::getInstance().sendRequest("register", {
                                                          login.toStdString(),
                                                          pass.toStdString(),
                                                          login.toStdString(),
                                                          email.toStdString()
                                                      });
}
