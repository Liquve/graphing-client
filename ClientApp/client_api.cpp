#include "client_api.h"
#include <QDebug>
#include <QStringList>

// Конструктор
Client_API::Client_API(QObject *parent) : QObject(parent)
{
    socket = new QTcpSocket(this);

    // Связываем событие "сервер прислал буквы" с нашей функцией onReadyRead
    connect(socket, &QTcpSocket::readyRead, this, &Client_API::onReadyRead);

    // Просто для отладки: сообщаем, если подключились успешно
    connect(socket, &QTcpSocket::connected, this, [this](){
        qDebug() << "УСПЕХ: Соединение с сервером установлено!";
        emit signalConnected();
    });
}

Client_API::~Client_API()
{
    socket->disconnectFromHost();
}

// Метод для подключения к серверу
void Client_API::connectToServer(const QString &ip, int port)
{
    qDebug() << "Пытаемся подключиться к" << ip << port << "...";
    socket->connectToHost(ip, port);
}

// Метод для отправки сообщений (запросов)
void Client_API::sendRequest(const QString &request)
{
    if (socket->state() == QTcpSocket::ConnectedState) {
        // Отправляем строку и добавляем \n как признак конца сообщения
        socket->write((request + "\n").toUtf8());
        qDebug() << "КЛИЕНТ ОТПРАВИЛ:" << request;
    } else {
        qDebug() << "ОШИБКА: Нет подключения к серверу!";
    }
}

// Сюда приходят ответы от сервера
void Client_API::onReadyRead()
{
    // Читаем всё, что пришло
    QByteArray data = socket->readAll();
    QString response = QString::fromUtf8(data).trimmed(); // Убираем лишние пробелы/энтеры

    qDebug() << "СЕРВЕР ОТВЕТИЛ:" << response;

    // --- РАЗБИРАЕМ ОТВЕТ ПО ПРОТОКОЛУ ПРЕПОДА ---
    // Препод писал, что ответ: "auth+&login" или "auth-"

    QStringList parts = response.split("&"); // Режем строку по амперсанду
    QString command = parts[0];              // Берем первое слово (auth+, auth-, reg+ и тд)

    if (command == "auth+") {
        emit signalAuthSuccess(); // Кричим на всю программу: ПУСКАЙТЕ ЕГО!
    }
    else if (command == "auth-") {
        emit signalAuthFailed();  // Кричим: НЕ ПУСКАТЬ!
    }
    // Здесь потом допишем if для check+ и stat+
}