#include "manager_of_forms.h"
#include <QApplication>
#include "client_api.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Manager_Of_Forms w; // Создаем твое окно задачи
    Client_API::getInstance().connectToServer("127.0.0.1", 33333);

    return a.exec();
}