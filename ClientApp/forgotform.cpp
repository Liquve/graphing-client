#include "forgotform.h"
#include "ui_forgotform.h"
#include "client_api.h"
#include <QMessageBox>
#include <QRegularExpressionValidator>
#include <QAction>
#include <QIcon>

ForgotForm::ForgotForm(QWidget *parent) : QWidget(parent), ui(new Ui::ForgotForm) {
    ui->setupUi(this);
    ui->step2_widget->hide();

    ui->lineEdit_code->setValidator(new QRegularExpressionValidator(QRegularExpression("^\\d{6}$"), this));
    ui->lineEdit_loginOrEmail->setMaxLength(32);
    ui->lineEdit_code->setMaxLength(6);
    ui->lineEdit_newPassword->setMaxLength(64);

    connect(ui->btn_exit, &QPushButton::clicked, this, [this](){ emit signalExit(); });

    QAction *eyeAction = ui->lineEdit_newPassword->addAction(
        QIcon(":/img/eye_closed.png"),
        QLineEdit::TrailingPosition
        );

    connect(eyeAction, &QAction::triggered, this, [=]() {
        if (ui->lineEdit_newPassword->echoMode() == QLineEdit::Password) {
            ui->lineEdit_newPassword->setEchoMode(QLineEdit::Normal);
            eyeAction->setIcon(QIcon(":/img/eye_open.png"));
        } else {
            ui->lineEdit_newPassword->setEchoMode(QLineEdit::Password);
            eyeAction->setIcon(QIcon(":/img/eye_closed.png"));
        }
    });

    connect(&Client_API::getInstance(), &Client_API::signalResponse, this, [this](uint64_t id, bool success, std::vector<std::string> params, QString errorMsg) {
        if (!this->isVisible()) return;

        if (!success) {
            if (id == m_lastRequestId || id == m_verifyRequestId || id == m_resetRequestId) {
                QMessageBox::warning(this, "Ошибка", errorMsg);
            }
            return;
        }

        if (id == m_lastRequestId && !params.empty()) {
            m_token = QString::fromStdString(params[0]);
            ui->step1_widget->hide();
            ui->step2_widget->show();
            QMessageBox::information(this, "Готово", "Код отправлен на вашу почту.");
        }
        else if (id == m_verifyRequestId) {
            m_resetRequestId = Client_API::getInstance().sendRequest("reset_password", {
                                                                                           m_token.toStdString(),
                                                                                           ui->lineEdit_newPassword->text().toStdString()
                                                                                       });
        }
        else if (id == m_resetRequestId) {
            QMessageBox::information(this, "Успех", "Пароль успешно изменен!");
            on_btn_back_clicked();
        }
    });
}

ForgotForm::~ForgotForm() { delete ui; }

void ForgotForm::setLogin(const QString &login) {
    ui->lineEdit_loginOrEmail->setText(login);
}

void ForgotForm::on_btn_sendRequest_clicked() {
    QString login = ui->lineEdit_loginOrEmail->text().trimmed();
    if (login.isEmpty()) return;

    if (login.length() > 32) {
        QMessageBox::warning(this, "Внимание", "Логин слишком длинный!");
        return;
    }

    m_token.clear();
    m_lastRequestId = Client_API::getInstance().sendRequest("request_password_reset", { login.toStdString() });
}

void ForgotForm::on_btn_resetPassword_clicked() {
    QString code = ui->lineEdit_code->text();
    QString newPass = ui->lineEdit_newPassword->text();

    if (code.length() != 6 || newPass.isEmpty()) {
        QMessageBox::warning(this, "Внимание", "Введите код (6 цифр) и новый пароль!");
        return;
    }

    if (newPass.length() > 64) {
        QMessageBox::warning(this, "Внимание", "Пароль слишком длинный (макс. 64 симв.)!");
        return;
    }

    m_verifyRequestId = Client_API::getInstance().sendRequest("verify_password_reset", {
                                                                                           m_token.toStdString(),
                                                                                           ui->lineEdit_code->text().toStdString()
                                                                                       });
}

void ForgotForm::on_btn_back_clicked() {
    m_token.clear();
    m_lastRequestId = 0; m_verifyRequestId = 0; m_resetRequestId = 0;
    ui->lineEdit_code->clear();
    ui->lineEdit_newPassword->clear();
    ui->step2_widget->hide();
    ui->step1_widget->show();
    emit signalOpenAuthForm();
}
