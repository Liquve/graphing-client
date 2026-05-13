#include "regform.h"
#include "ui_regform.h"
#include "client_api.h"
#include <QMessageBox>
#include <vector>
#include <string>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QAction>
#include <QIcon>

RegForm::RegForm(QWidget *parent) : QWidget(parent), ui(new Ui::RegForm) {
    ui->setupUi(this);
    connect(ui->btn_exit, &QPushButton::clicked, this, [this](){ emit signalExit(); });
    connect(ui->btn_backToAbout, &QPushButton::clicked, this, &RegForm::signalOpenAboutForm);
    QRegularExpression emailRegex("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$");
    ui->lineEdit_email->setValidator(new QRegularExpressionValidator(emailRegex, this));

    QRegularExpression loginRegex("^[^@]*$");
    ui->lineEdit_login->setValidator(new QRegularExpressionValidator(loginRegex, this));

    ui->lineEdit_login->setMaxLength(32);
    ui->lineEdit_email->setMaxLength(64);
    ui->lineEdit_password->setMaxLength(64);
    ui->lineEdit_password_repeat->setMaxLength(64);

    ui->lineEdit_login->setPlaceholderText("Логин");
    ui->lineEdit_email->setPlaceholderText("example@mail.ru");

    auto addEyeToField = [this](QLineEdit *field) {
        QAction *eyeAction = field->addAction(QIcon(":/img/eye_closed.png"), QLineEdit::TrailingPosition);

        connect(eyeAction, &QAction::triggered, this, [field, eyeAction]() {
            if (field->echoMode() == QLineEdit::Password) {
                field->setEchoMode(QLineEdit::Normal);
                eyeAction->setIcon(QIcon(":/img/eye_open.png"));
            } else {
                field->setEchoMode(QLineEdit::Password);
                eyeAction->setIcon(QIcon(":/img/eye_closed.png"));
            }
        });
    };

    addEyeToField(ui->lineEdit_password);
    addEyeToField(ui->lineEdit_password_repeat);
}

RegForm::~RegForm() {
    delete ui;
}

void RegForm::on_btn_back_clicked() {
    emit signalOpenAuthForm();
}

void RegForm::on_btn_register_clicked() {
    QString login = ui->lineEdit_login->text().trimmed();
    QString email = ui->lineEdit_email->text().trimmed();
    QString pass = ui->lineEdit_password->text();
    QString passConfirm = ui->lineEdit_password_repeat->text();

    if (login.isEmpty() || email.isEmpty() || pass.isEmpty()) {
        QMessageBox::warning(this, "Внимание", "Заполните все поля!");
        return;
    }

    if (login.length() > 32 || email.length() > 64 || pass.length() > 64) {
        QMessageBox::warning(this, "Внимание", "Введенные данные слишком длинные!");
        return;
    }

    int pos = 0;
    if (ui->lineEdit_email->validator()->validate(email, pos) != QValidator::Acceptable) {
        QMessageBox::warning(this, "Ошибка", "Введите корректный адрес почты!");
        return;
    }

    if (pass != passConfirm) {
        QMessageBox::warning(this, "Ошибка", "Пароли не совпадают!");
        return;
    }

    Client_API::getInstance().sendRequest("register", {
                                                          login.toStdString(),
                                                          pass.toStdString(),
                                                          login.toStdString(),
                                                          email.toStdString()
                                                      });
}
