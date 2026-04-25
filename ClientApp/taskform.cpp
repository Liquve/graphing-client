#include "taskform.h"
#include "ui_taskform.h"
#include "client_api.h"
#include <QPainter>
#include <QPainterPath>
#include <QHeaderView>
#include <QEvent>
#include <QTableWidgetItem>
#include <QLocale>

Taskform::Taskform(QWidget *parent) : QWidget(parent), ui(new Ui::Taskform) {
    ui->setupUi(this);
    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setHorizontalHeaderLabels({"x", "f(x)"});
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->plotWidget->installEventFilter(this);

    connect(ui->spinBox_a, &QDoubleSpinBox::valueChanged, this, &Taskform::requestCalculation);
    connect(ui->spinBox_b, &QDoubleSpinBox::valueChanged, this, &Taskform::requestCalculation);
    connect(ui->spinBox_c, &QDoubleSpinBox::valueChanged, this, &Taskform::requestCalculation);
    connect(ui->btn_logout, &QPushButton::clicked, this, &Taskform::signalLogout);
    connect(ui->btn_exit, &QPushButton::clicked, this, &Taskform::signalExit);

    connect(&Client_API::getInstance(), &Client_API::signalResponse, this, [this](uint64_t id, bool success, std::vector<std::string> params, QString errorMsg) {
        if (!success) return;

        graphPoints.clear();
        ui->tableWidget->setRowCount(0);
        QLocale cLocale(QLocale::C);

        int nextTargetX = -10; // Начинаем искать с -10

        for (const std::string& val : params) {
            QString pair = QString::fromStdString(val).trimmed();
            QStringList parts = pair.split(":");
            if (parts.size() < 2) continue;

            bool xOk, yOk;
            double x = cLocale.toDouble(parts[0], &xOk);
            QString yStr = parts[1];

            // --- НОВАЯ ЛОГИКА ДЛЯ ТАБЛИЦЫ ---
            // Проверяем, подошли ли мы к очередному целому числу X
            if (xOk && nextTargetX <= 10 && x >= (double)nextTargetX - 0.0001) {
                int row = ui->tableWidget->rowCount();
                ui->tableWidget->insertRow(row);

                // Записываем целое X
                QTableWidgetItem *itemX = new QTableWidgetItem(QString::number(nextTargetX));
                itemX->setTextAlignment(Qt::AlignCenter);
                ui->tableWidget->setItem(row, 0, itemX);

                // Записываем Y
                QTableWidgetItem *itemY;
                if (yStr == "null") {
                    itemY = new QTableWidgetItem("—"); // Ставим прочерк на месте разрыва
                } else {
                    double y = cLocale.toDouble(yStr, &yOk);
                    itemY = new QTableWidgetItem(QString::number(y, 'f', 1)); // Округляем до 1 знака
                }

                itemY->setTextAlignment(Qt::AlignCenter);
                ui->tableWidget->setItem(row, 1, itemY);

                nextTargetX++; // Переходим к поиску следующего целого числа
            }

            // --- ЛОГИКА ДЛЯ ГРАФИКА ---
            if (yStr == "null") {
                // Добавляем пустую точку, чтобы линия графика в этом месте разорвалась
                if (xOk) graphPoints.append(QPointF(x, qQNaN()));
                continue; // Идем дальше
            }

            double y = cLocale.toDouble(yStr, &yOk);
            if (xOk && yOk) {
                graphPoints.append(QPointF(x, y));
            }
        }
        ui->plotWidget->update();
    });

    requestCalculation();
}

Taskform::~Taskform() { delete ui; }

void Taskform::requestCalculation() {
    Client_API::getInstance().sendRequest("calculate", {
                                                           std::to_string((int)ui->spinBox_a->value()),
                                                           std::to_string((int)ui->spinBox_b->value()),
                                                           std::to_string((int)ui->spinBox_c->value())
                                                       });
}

void Taskform::on_spinBox_a_valueChanged(double) {}
void Taskform::on_spinBox_b_valueChanged(double) {}
void Taskform::on_spinBox_c_valueChanged(double) {}

bool Taskform::eventFilter(QObject *watched, QEvent *event) {
    if (watched == ui->plotWidget && event->type() == QEvent::Paint) {
        QPainter painter(ui->plotWidget);
        painter.setRenderHint(QPainter::Antialiasing);
        int w = ui->plotWidget->width(), h = ui->plotWidget->height();
        painter.fillRect(0, 0, w, h, Qt::white);

        if (graphPoints.isEmpty()) return true;

        // Фиксированный масштаб для чистоты картинки
        double xMin = -10.0, xMax = 10.0;
        double yMin = -10.0, yMax = 10.0;
        int m = 45;
        int dW = w - 2*m, dH = h - 2*m;

        // Сетка
        painter.setPen(QPen(QColor("#f0f0f0"), 1));
        for (int i = -10; i <= 10; i += 2) {
            int gx = m + (i - xMin)/(xMax - xMin) * dW;
            int gy = m + dH - (i - yMin)/(yMax - yMin) * dH;
            painter.drawLine(gx, m, gx, h-m);
            painter.drawLine(m, gy, w-m, gy);
        }

        // Оси
        painter.setPen(QPen(Qt::black, 2));
        int zY = m + dH - (0 - yMin)/(yMax - yMin) * dH;
        int zX = m + (0 - xMin)/(xMax - xMin) * dW;
        painter.drawLine(m-10, zY, w-m+10, zY);
        painter.drawLine(zX, m-10, zX, h-m+10);

        // Функция проекции (БЕЗ ЖЕСТКОГО ОГРАНИЧЕНИЯ)
        auto map = [&](QPointF p) {
            double px = m + (p.x() - xMin) / (xMax - xMin) * dW;
            double py = m + dH - (p.y() - yMin) / (yMax - yMin) * dH;
            return QPointF(px, py);
        };

        // РИСОВАНИЕ
        for (int i = 0; i < graphPoints.size() - 1; ++i) {
            QPointF p1 = graphPoints[i];
            QPointF p2 = graphPoints[i+1];

            // 1. ПРОВЕРКА НА РАЗРЫВ (чтобы не было синей стены в x=1)
            // Если одна из точек имеет Y больше 50, а вторая нормальная -
            // значит это уход в асимптоту, линию рисовать НЕ НУЖНО.
            if (qAbs(p1.y()) > 50.0 || qAbs(p2.y()) > 50.0) continue;

            // 2. ПРОВЕРКА НА ЗОНЫ (чтобы ветки не соединялись между собой)
            auto getZone = [](double x) {
                if (x < 0) return 0;
                if (x < 1.0) return 1;
                return 2;
            };
            if (getZone(p1.x()) != getZone(p2.x())) continue;

            // 3. Выбор цвета
            if (p1.x() < 0) painter.setPen(QPen(Qt::red, 3));
            else if (p1.x() < 1.0) painter.setPen(QPen(QColor("#2ecc71"), 3));
            else painter.setPen(QPen(Qt::blue, 3));

            // Рисуем линию
            painter.drawLine(map(p1), map(p2));
        }

        // Подписи
        painter.setPen(Qt::black);
        painter.drawText(m, h - 5, QString("Диапазон X: [-10;10] | Таблица: целые узлы"));

        return true;
    }
    return QWidget::eventFilter(watched, event);
}
