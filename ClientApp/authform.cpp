#include "authform.h"
#include "ui_authform.h"
#include "client_api.h"
#include <QMessageBox>
#include <QRegularExpressionValidator>

AuthForm::AuthForm(QWidget *parent) : QWidget(parent), ui(new Ui::AuthForm) {
    ui->setupUi(this);
    connect(ui->btn_exit, &QPushButton::clicked, this, [this](){ emit signalExit(); });
    // Ограничение для логина: не даем вводить @ (раз мы договорились, что логин - это не почта)
    QRegularExpression loginRegex("^[^@]*$");
    ui->lineEdit_login->setValidator(new QRegularExpressionValidator(loginRegex, this));
}

AuthForm::~AuthForm() { delete ui; }

// Кнопка ВОЙТИ
void AuthForm::on_btn_login_clicked() {
    QString login = ui->lineEdit_login->text().trimmed();
    QString pass = ui->lineEdit_password->text();

    if (login.isEmpty() || pass.isEmpty()) {
        QMessageBox::warning(this, "Внимание", "Введите логин и пароль!");
        return;
    }

    // Отправляем запрос серверу.
    // ВАЖНО: сервер ждет команду "login" и 2 параметра: login, password
    Client_API::getInstance().sendRequest("login", {
                                                       login.toStdString(),
                                                       pass.toStdString()
                                                   });
}

// Кнопка РЕГИСТРАЦИЯ
void AuthForm::on_btn_register_clicked() {
    emit signalOpenRegForm();
}

// Кнопка ЗАБЫЛ ПАРОЛЬ
void AuthForm::on_btn_forget_password_clicked() {
    QString currentLogin = ui->lineEdit_login->text().trimmed();
    emit signalOpenForgotForm(currentLogin); // Отправляем логин менеджеру
}

// Показать/скрыть пароль
void AuthForm::on_checkBox_showPass_toggled(bool checked) {
    ui->lineEdit_password->setEchoMode(checked ? QLineEdit::Normal : QLineEdit::Password);
}
