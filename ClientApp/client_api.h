#ifndef CLIENT_API_H
#define CLIENT_API_H

#include <QObject>
#include <QTcpSocket> // Класс для работы с сетью по протоколу TCP

class Client_API : public QObject
{
    Q_OBJECT // Макрос для работы сигналов

private:
    QTcpSocket *socket; // Твой "кабель" до сервера

    // ПРИВАТНЫЙ конструктор. Никто не сможет сделать "new Client_API()"
    explicit Client_API(QObject *parent = nullptr);
    ~Client_API();

public:
    // Та самая функция Синглтона Майерса. Она выдает единственный экземпляр.
    static Client_API& getInstance() {
        static Client_API instance;
        return instance;
    }

    // Запрещаем копировать нашего "официанта"
    Client_API(const Client_API&) = delete;
    Client_API& operator=(const Client_API&) = delete;

    // --- ФУНКЦИИ КОТОРЫЕ МОЖНО ВЫЗЫВАТЬ ---
    void connectToServer(const QString &ip, int port); // Подключиться
    void sendRequest(const QString &request);          // Отправить текст

signals:
    // --- СИГНАЛЫ (КРИКИ), КОТОРЫЕ СЛУШАЕТ МЕНЕДЖЕР ФОРМ ---
    void signalAuthSuccess(); // "Ура, пароль верный!"
    void signalAuthFailed();  // "Пароль неверный!"
    void signalConnected();   // "Мы подключились к серверу!"

private slots:
    // Сюда будут прилетать ответы от сервера
    void onReadyRead();
};

#endif // CLIENT_API_H