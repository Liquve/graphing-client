#ifndef CLIENT_API_H
#define CLIENT_API_H

#include <QObject>
#include <QTcpSocket>
#include <vector>
#include <string>
#include "GraphingProtocol.h"

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
    void disconnectFromServer();

    uint64_t sendRequest(const std::string& type, const std::vector<std::string>& parameters = {});

signals:
    void signalConnected();
    void signalDisconnected();
    void signalResponse(uint64_t id, bool success, std::vector<std::string> params, QString errorMsg);

private slots:
    void onReadyRead();
};

#endif
