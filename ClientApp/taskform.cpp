#include "taskform.h"
#include "ui_taskform.h"
#include <cmath>


Taskform::Taskform(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Taskform)
{
    ui->setupUi(this);

    ui->splitter->widget(0)->setMaximumWidth(250);

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    QPixmap pic = ui->label_formula->pixmap();
    ui->label_formula->setPixmap(pic.scaledToWidth(230, Qt::SmoothTransformation));

     ui->plotWidget->installEventFilter(this);

    calculateAndFillTable();
}

Taskform::~Taskform()
{
    delete ui;
}

// главная функция вычислений
void Taskform::calculateAndFillTable()
{
    // 1. Берем числа из интерфейса
    double a = ui->spinBox_a->value();
    double b = ui->spinBox_b->value();
    double c = ui->spinBox_c->value();

    // 2. Готовим таблицу
    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setRowCount(201);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "x" << "f(x)");

    graphPoints.clear();

    int row = 0;

    // 3. Считаем формулу для x от -10 до 10
    for (double x = -10; x <= 10.01; x += 0.1)
    {
        double fx = 0.0;

        if (x < 0) {
            fx = std::cosh(a * x);
        }
        else if (x >= 0 && x < 1) {
            double log_inner = b * x + 1;
            if (log_inner > 0) fx = std::log(log_inner);
            else fx = 0; // Защита
        }
        else if (x >= 1) {
            if (x - 1 != 0) fx = c / (x - 1);
            else fx = 0; // Защита от деления на 0
        }

        // 4. Записываем в таблицу
        QTableWidgetItem *itemX = new QTableWidgetItem(QString::number(x, 'f', 2));
        QTableWidgetItem *itemFx = new QTableWidgetItem(QString::number(fx, 'f', 4));

        ui->tableWidget->setItem(row, 0, itemX);
        ui->tableWidget->setItem(row, 1, itemFx);

        graphPoints.append(QPointF(x, fx));

        row++;
    }

    ui->plotWidget->update();
}

void Taskform::on_spinBox_a_valueChanged(double arg1)
{
    calculateAndFillTable();
}


void Taskform::on_spinBox_b_valueChanged(double arg1)
{
    calculateAndFillTable();
}


void Taskform::on_spinBox_c_valueChanged(double arg1)
{
    calculateAndFillTable();
}

// художник графика
// --- НАШ ПЕРЕХВАТЧИК СОБЫТИЙ И ХУДОЖНИК ---
bool Taskform::eventFilter(QObject *watched, QEvent *event)
{
    // Если сигнал пришел от нашего белого холста и это сигнал "НАРИСУЙ МЕНЯ!"
    if (watched == ui->plotWidget && event->type() == QEvent::Paint)
    {
        // Вызываем художника, и даем ему холст plotWidget
        QPainter painter(ui->plotWidget);
        painter.setRenderHint(QPainter::Antialiasing);

        int w = ui->plotWidget->width();
        int h = ui->plotWidget->height();

        // 1. Заливаем холст белым цветом
        painter.fillRect(0, 0, w, h, Qt::white);

        // Если точек нет - просто оставляем белый квадрат
        if (graphPoints.isEmpty()) return true;

        // Фиксируем масштаб экрана, чтобы видеть детали, а не гигантские цифры
        double xMin = -10.0, xMax = 10.0;
        double yMin = -10.0, yMax = 10.0;

        int margin = 30;
        int drawW = w - 2 * margin;
        int drawH = h - 2 * margin;

        // 3. РИСУЕМ ОСИ (Серые пунктирные)
        painter.setPen(QPen(Qt::lightGray, 1, Qt::DashLine));
        // Ось Y = 0 (Горизонтальная по центру)
        int zeroY = margin + drawH - (0 - yMin) / (yMax - yMin) * drawH;
        painter.drawLine(0, zeroY, w, zeroY);

        // Ось X = 0 (Вертикальная по центру)
        int zeroX = margin + (0 - xMin) / (xMax - xMin) * drawW;
        painter.drawLine(zeroX, 0, zeroX, h);

        // 4. РИСУЕМ ГРАФИК (Цветной и идеально гладкий!)
        QPen penA(QColor("#e74c3c"), 3); // Красная (x < 0)
        QPen penB(QColor("#27ae60"), 3); // Зеленая (0 <= x < 1)
        QPen penC(QColor("#2980b9"), 3); // Синяя (x >= 1)

        // Создаем три отдельных умных пути
        QPainterPath pathA, pathB, pathC;
        bool firstA = true, firstB = true, firstC = true;

        for (int i = 0; i < graphPoints.size(); ++i) {
            double x = graphPoints[i].x();
            double y = graphPoints[i].y();

            // Защита: если Y улетел в космос (асимптота), просто не добавляем эту точку в путь
            if (y > 30.0 || y < -30.0) continue;

            // Переводим координаты в пиксели
            double px = margin + (x - xMin) / (xMax - xMin) * drawW;
            double py = margin + drawH - (y - yMin) / (yMax - yMin) * drawH;

            // Распределяем точки по трем путям
            if (x < 0) {
                if (firstA) { pathA.moveTo(px, py); firstA = false; }
                else { pathA.lineTo(px, py); }
            }
            else if (x >= 0 && x < 1) {
                if (firstB) { pathB.moveTo(px, py); firstB = false; }
                else { pathB.lineTo(px, py); }
            }
            else {
                if (firstC) { pathC.moveTo(px, py); firstC = false; }
                else { pathC.lineTo(px, py); }
            }
        }

        // Рисуем все три пути с идеальным сглаживанием!
        painter.setPen(penA);
        painter.drawPath(pathA);

        painter.setPen(penB);
        painter.drawPath(pathB);

        painter.setPen(penC);
        painter.drawPath(pathC);

        return true; // Говорим Qt: "Мы сами всё нарисовали, больше ничего не делай!"
    }

    // Все остальные события пропускаем как обычно
    return QWidget::eventFilter(watched, event);
}