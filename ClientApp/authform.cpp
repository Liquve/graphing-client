#include "authform.h"
#include "ui_authform.h"
#include "client_api.h"
#include <QMessageBox>
#include <QRegularExpressionValidator>
#include <QAction> // Нужно для работы с иконкой внутри поля
#include <QIcon>   // Нужно для работы с иконками

AuthForm::AuthForm(QWidget *parent) : QWidget(parent), ui(new Ui::AuthForm) {
    ui->setupUi(this);
    connect(ui->btn_exit, &QPushButton::clicked, this, [this](){ emit signalExit(); });

    QRegularExpression loginRegex("^[^@]*$");
    ui->lineEdit_login->setValidator(new QRegularExpressionValidator(loginRegex, this));
    ui->lineEdit_login->setMaxLength(32);
    ui->lineEdit_password->setMaxLength(64);
    ui->lineEdit_login->setPlaceholderText("Логин");

    QAction *eyeAction = ui->lineEdit_password->addAction(
        QIcon(":/img/eye_closed.png"),
        QLineEdit::TrailingPosition // Позиция справа внутри поля
        );

    connect(ui->btn_backToAbout, &QPushButton::clicked, this, &AuthForm::signalOpenAboutForm);
    connect(eyeAction, &QAction::triggered, this, [=]() {
        if (ui->lineEdit_password->echoMode() == QLineEdit::Password) {
            ui->lineEdit_password->setEchoMode(QLineEdit::Normal);
            eyeAction->setIcon(QIcon(":/img/eye_open.png")); // Меняем на открытый глаз
        } else {
            ui->lineEdit_password->setEchoMode(QLineEdit::Password);
            eyeAction->setIcon(QIcon(":/img/eye_closed.png")); // Меняем на закрытый глаз
        }
    });
}

AuthForm::~AuthForm() { delete ui; }

void AuthForm::on_btn_login_clicked() {
    QString login = ui->lineEdit_login->text().trimmed();
    QString pass = ui->lineEdit_password->text();

    if (login.isEmpty() || pass.isEmpty()) {
        QMessageBox::warning(this, "Внимание", "Введите логин и пароль!");
        return;
    }

    if (login.length() > 32 || pass.length() > 64) {
        QMessageBox::warning(this, "Внимание", "Введенные данные слишком длинные!");
        return;
    }

    Client_API::getInstance().sendRequest("login", {
                                                       login.toStdString(),
                                                       pass.toStdString()
                                                   });
}

void AuthForm::on_btn_register_clicked() {
    emit signalOpenRegForm();
}

void AuthForm::on_btn_forget_password_clicked() {
    QString currentLogin = ui->lineEdit_login->text().trimmed();
    if (currentLogin.isEmpty()) {
        QMessageBox::information(this, "Подсказка", "Введите свой логин в поле выше, а затем нажмите 'Забыли пароль?'");
        return;
    }
    emit signalOpenForgotForm(currentLogin);
}
