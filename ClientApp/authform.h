#ifndef AUTHFORM_H
#define AUTHFORM_H

#include <QWidget>

namespace Ui {
class AuthForm;
}

class AuthForm : public QWidget
{
    Q_OBJECT

public:
    explicit AuthForm(QWidget *parent = nullptr);
    ~AuthForm();

private slots:
    void on_btn_login_clicked();

    void on_checkBox_showPass_toggled(bool checked);

    void on_btn_register_clicked();

    void on_btn_forget_password_clicked();

private:
    Ui::AuthForm *ui;

signals:
    void signalOpenRegForm();    // Крик: "Откройте регистрацию!"
    void signalOpenForgotForm(); // Крик: "Откройте сброс пароля!"
};

#endif // AUTHFORM_H

