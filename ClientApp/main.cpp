#include <QApplication>          // ОБЯЗАТЕЛЬНО: для работы графического интерфейса
#include "manager_of_forms.h"    // Твой менеджер окон
#include "client_api.h"          // Твой сетевой клиент

int main(int argc, char *argv[])
{
    // 1. Создаем объект приложения
    QApplication a(argc, argv);

    // 2. Создаем главное окно (менеджер форм) и показываем его
    Manager_Of_Forms w;
    w.show();

    // 3. Подключаемся к серверу (убедись, что сервер запущен на этом порту)
    Client_API::getInstance().connectToServer("127.0.0.1", 13579);

    // 4. Запускаем цикл обработки событий
    return a.exec();
}
