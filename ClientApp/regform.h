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
    void signalOpenAuthForm();  // Сигнал для возврата к окну входа
    void signalExit();

private slots:
    // Эти функции (слоты) должны быть здесь, чтобы кнопки из .ui работали
    void on_btn_back_clicked();
    void on_btn_register_clicked();
    void on_checkBox_showPass_toggled(bool checked); // ВОТ ЭТОЙ СТРОЧКИ НЕ ХВАТАЛО

private:
    Ui::RegForm *ui;
};

#endif // REGFORM_H
