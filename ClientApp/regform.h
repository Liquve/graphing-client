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

signals:
    void signalOpenAuthForm();
    void signalOpenAboutForm();
    void signalExit();

private slots:
    void on_btn_back_clicked();
    void on_btn_register_clicked();

private:
    Ui::RegForm *ui;
};

#endif
