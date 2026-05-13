#include <QApplication>
#include "manager_of_forms.h"
#include "client_api.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Manager_Of_Forms w;
    w.show();

    Client_API::getInstance().connectToServer("127.0.0.1", 13579);

    return a.exec();
}
