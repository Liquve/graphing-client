#ifndef CLIENT_API_H
#define CLIENT_API_H

#include <QObject>
#include <QTcpSocket>
#include <vector>
#include <string>
#include "GraphingProtocol.h" // Этот файл должен быть в папке клиента

class Client_API : public QObject
{
    Q_OBJECT

private:
    QTcpSocket *socket;
    uint64_t nextRequestId = 1;

    explicit Client_API(QObject *parent = nullptr);
    ~Client_API();

public:
    static Client_API& getInstance() {
        static Client_API instance;
        return instance;
    }

    Client_API(const Client_API&) = delete;
    Client_API& operator=(const Client_API&) = delete;

    void connectToServer(const QString &ip, int port);

    // Универсальная функция для отправки запросов
    uint64_t sendRequest(const std::string& type, const std::vector<std::string>& parameters = {});

signals:
    void signalConnected();
    void signalDisconnected();
    // Этот сигнал будет нести ответ от сервера: (ID запроса, успешно ли, параметры, текст ошибки)
    void signalResponse(uint64_t id, bool success, std::vector<std::string> params, QString errorMsg);

private slots:
    void onReadyRead();
};

#endif // CLIENT_API_H
