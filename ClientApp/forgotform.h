#ifndef FORGOTFORM_H
#define FORGOTFORM_H

#include <QWidget>

namespace Ui {
class ForgotForm;
}

class ForgotForm : public QWidget
{
    Q_OBJECT

public:
    explicit ForgotForm(QWidget *parent = nullptr);
    ~ForgotForm();

private:
    Ui::ForgotForm *ui;

signals:
    void signalOpenAuthForm();  // Крик: "Откройте регистрацию!"
private slots:
    void on_btn_back_clicked();
};

#endif // FORGOTFORM_H
