#ifndef REGFORM_H
#define REGFORM_H

#include <QWidget>

namespace Ui {
class RegForm;
}

class RegForm : public QWidget
{
    Q_OBJECT

public:
    explicit RegForm(QWidget *parent = nullptr);
    ~RegForm();

private:
    Ui::RegForm *ui;

signals:
    void signalOpenAuthForm();  // Крик: "Откройте регистрацию!"
private slots:
    void on_btn_back_clicked();
};

#endif // REGFORM_H
