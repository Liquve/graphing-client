#ifndef TASKFORM_H
#define TASKFORM_H

#include <QWidget>
#include <QVector>
#include <QPointF>

namespace Ui { class Taskform; }

class Taskform : public QWidget
{
    Q_OBJECT

public:
    explicit Taskform(QWidget *parent = nullptr);
    ~Taskform();
signals:
    void signalLogout();
    void signalExit();
protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private slots:
    void on_spinBox_a_valueChanged(double arg1);
    void on_spinBox_b_valueChanged(double arg1);
    void on_spinBox_c_valueChanged(double arg1);

private:
    Ui::Taskform *ui;
    QVector<QPointF> graphPoints;
    void requestCalculation();
};

#endif
