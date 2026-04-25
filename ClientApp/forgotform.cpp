#include "forgotform.h"
#include "ui_forgotform.h"
#include "client_api.h"
#include <QMessageBox>
#include <QRegularExpressionValidator>

ForgotForm::ForgotForm(QWidget *parent) : QWidget(parent), ui(new Ui::ForgotForm) {
    ui->setupUi(this);
    ui->step2_widget->hide();
    ui->lineEdit_code->setValidator(new QRegularExpressionValidator(QRegularExpression("^\\d{6}$"), this));
    connect(ui->btn_exit, &QPushButton::clicked, this, [this](){ emit signalExit(); });

    connect(&Client_API::getInstance(), &Client_API::signalResponse, this, [this](uint64_t id, bool success, std::vector<std::string> params, QString errorMsg) {
        if (!this->isVisible()) return;

        if (!success) {
            // Показываем только ту ошибку, которая пришла на наш конкретный запрос
            if (id == m_lastRequestId || id == m_verifyRequestId || id == m_resetRequestId) {
                QMessageBox::warning(this, "Ошибка", errorMsg);
            }
            return;
        }

        // 1. Получили токен (ответ на запрос кода)
        if (id == m_lastRequestId && !params.empty()) {
            m_token = QString::fromStdString(params[0]);
            ui->step1_widget->hide();
            ui->step2_widget->show();
            QMessageBox::information(this, "Готово", "Код отправлен на вашу почту.");
        }

        // 2. Код подтвержден (ответ на проверку кода)
        else if (id == m_verifyRequestId) {
            // Код верный, теперь автоматически отправляем сам сброс пароля
            m_resetRequestId = Client_API::getInstance().sendRequest("reset_password", {
                                                                                           m_token.toStdString(),
                                                                                           ui->lineEdit_newPassword->text().toStdString()
                                                                                       });
        }

        // 3. Пароль изменен (ответ на сброс пароля)
        else if (id == m_resetRequestId) {
            QMessageBox::information(this, "Успех", "Пароль успешно изменен!");
            on_btn_back_clicked(); // Сбрасываем форму и возвращаемся
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
    m_token.clear();
    m_lastRequestId = Client_API::getInstance().sendRequest("request_password_reset", { login.toStdString() });
}

void ForgotForm::on_btn_resetPassword_clicked() {
    if (ui->lineEdit_code->text().length() != 6 || ui->lineEdit_newPassword->text().isEmpty()) {
        QMessageBox::warning(this, "Внимание", "Введите код и новый пароль!");
        return;
    }
    // Шлем только проверку кода. Смена пароля пойдет следом сама при успехе.
    m_verifyRequestId = Client_API::getInstance().sendRequest("verify_password_reset", {
                                                                                           m_token.toStdString(),
                                                                                           ui->lineEdit_code->text().toStdString()
                                                                                       });
}

void ForgotForm::on_btn_back_clicked() {
    m_token.clear();
    m_lastRequestId = 0; m_verifyRequestId = 0; m_resetRequestId = 0;
    ui->step2_widget->hide();
    ui->step1_widget->show();
    emit signalOpenAuthForm();
}
