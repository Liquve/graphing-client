#ifndef FORGOTFORM_H
#define FORGOTFORM_H

#include <QWidget>

namespace Ui { class ForgotForm; }

class ForgotForm : public QWidget
{
    Q_OBJECT
public:
    explicit ForgotForm(QWidget *parent = nullptr);
    ~ForgotForm();
    void setLogin(const QString &login); // Метод для подстановки логина

signals:
    void signalOpenAuthForm();
    void signalExit();

private slots:
    void on_btn_sendRequest_clicked();
    void on_btn_resetPassword_clicked();
    void on_btn_back_clicked();

private:
    Ui::ForgotForm *ui;
    QString m_token;
    uint64_t m_lastRequestId = 0;
    uint64_t m_verifyRequestId = 0;
    uint64_t m_resetRequestId = 0;
};

#endif
