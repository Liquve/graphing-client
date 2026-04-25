#include "client_api.h"
#include <QDebug>

Client_API::Client_API(QObject *parent) : QObject(parent)
{
    socket = new QTcpSocket(this);

    connect(socket, &QTcpSocket::connected, this, &Client_API::signalConnected);
    connect(socket, &QTcpSocket::disconnected, this, &Client_API::signalDisconnected);
    connect(socket, &QTcpSocket::readyRead, this, &Client_API::onReadyRead);

    // Добавляем обработку ошибок (например, если сервер выключен)
    connect(socket, &QTcpSocket::errorOccurred, this, [this](QAbstractSocket::SocketError socketError) {
        QString errorMsg;
        switch (socketError) {
        case QAbstractSocket::RemoteHostClosedError:
            errorMsg = "Сервер разорвал соединение.";
            break;
        case QAbstractSocket::HostNotFoundError:
            errorMsg = "Сервер не найден. Проверьте IP адрес.";
            break;
        case QAbstractSocket::ConnectionRefusedError:
            errorMsg = "Сервер выключен или порт закрыт.";
            break;
        default:
            errorMsg = "Ошибка сети: " + socket->errorString();
        }
        emit signalResponse(0, false, {}, errorMsg); // Посылаем сигнал ошибки
    });
}

Client_API::~Client_API()
{
    socket->disconnectFromHost();
}

void Client_API::connectToServer(const QString &ip, int port)
{
    socket->connectToHost(ip, port);
}

uint64_t Client_API::sendRequest(const std::string& type, const std::vector<std::string>& parameters)
{
    if (socket->state() != QAbstractSocket::ConnectedState) {
        emit signalResponse(0, false, {}, "Нет соединения с сервером!");
        return 0;
    }

    uint64_t id = nextRequestId++;
    GraphingProtocol::Message msg = GraphingProtocol::Message::request(id, type, parameters);
    std::string serialized = GraphingProtocol::serialize(msg) + "\n";
    socket->write(serialized.c_str());
    return id;
}

void Client_API::onReadyRead()
{
    while (socket->canReadLine()) {
        QByteArray line = socket->readLine().trimmed();
        if (line.isEmpty()) continue;
        try {
            GraphingProtocol::Message msg = GraphingProtocol::parse(line.toStdString());
            if (msg.kind == GraphingProtocol::MessageKind::Response) {
                emit signalResponse(msg.correlationId, msg.success, msg.parameters, QString::fromStdString(msg.errorMessage));
            }
        } catch (...) {}
    }
}
