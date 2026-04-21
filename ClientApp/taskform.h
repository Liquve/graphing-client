#ifndef TASKFORM_H
#define TASKFORM_H

#include <QVector>
#include <QPointF>
#include <QWidget>
#include <QPainter>
#include <QPainterPath>

namespace Ui {
class Taskform;
}

class Taskform : public QWidget
{
    Q_OBJECT

public:
    explicit Taskform(QWidget *parent = nullptr);
    ~Taskform();

private slots:
    void on_spinBox_a_valueChanged(double arg1);

    void on_spinBox_b_valueChanged(double arg1);

    void on_spinBox_c_valueChanged(double arg1);

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    Ui::Taskform *ui;
    void calculateAndFillTable();
    QVector<QPointF> graphPoints;
};

#endif // TASKFORM_H
