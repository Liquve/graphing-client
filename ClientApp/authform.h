#ifndef AUTHFORM_H
#define AUTHFORM_H

#include <QWidget>

namespace Ui { class AuthForm; }

class AuthForm : public QWidget
{
    Q_OBJECT
public:
    explicit AuthForm(QWidget *parent = nullptr);
    ~AuthForm();

signals:
    void signalOpenRegForm();    // Переход к регистрации
    void signalOpenForgotForm(QString login);
    void signalExit();

private slots:
    void on_btn_login_clicked();
    void on_btn_register_clicked();
    void on_btn_forget_password_clicked();
    void on_checkBox_showPass_toggled(bool checked);

private:
    Ui::AuthForm *ui;
};

#endif
