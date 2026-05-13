#include "manager_of_forms.h"
#include "client_api.h"
#include "coverform.h"
#include "aboutform.h"
#include <QVBoxLayout>
#include <QMessageBox>
#include <QApplication>

Manager_Of_Forms::Manager_Of_Forms(QWidget *parent) : QWidget(parent)
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    stack = new QStackedWidget(this);
    mainLayout->addWidget(stack);

    coverForm  = new CoverForm();
    aboutForm  = new AboutForm();
    authForm   = new AuthForm();
    regForm    = new RegForm();
    forgotForm = new ForgotForm();
    taskForm   = new Taskform();

    stack->addWidget(coverForm);
    stack->addWidget(aboutForm);
    stack->addWidget(authForm);
    stack->addWidget(regForm);
    stack->addWidget(forgotForm);
    stack->addWidget(taskForm);

    connect(&Client_API::getInstance(), &Client_API::signalResponse, this, [=](uint64_t id, bool success, std::vector<std::string> params, QString errorMsg) {

        if (!success) {
            if (stack->currentWidget() != forgotForm) {
                QMessageBox::warning(this, "Внимание", errorMsg);
            }
            return;
        }

        QWidget* current = stack->currentWidget();

        if (current == authForm || current == regForm) {
            stack->setCurrentWidget(taskForm);
            this->showMaximized();
        }
        else if (current == forgotForm) {
            if (params.empty() || params[0].length() < 20) {
                stack->setCurrentWidget(authForm);
            }
        }
    });

    connect(coverForm, &CoverForm::signalOpenAuth, [=](){
        stack->setCurrentWidget(aboutForm);
    });

    connect(aboutForm, &AboutForm::signalBack, [=](){
        stack->setCurrentWidget(coverForm); // Назад к титульнику
    });
    connect(aboutForm, &AboutForm::signalForward, [=](){
        stack->setCurrentWidget(authForm);  // Вперед ко входу
    });

    connect(authForm, &AuthForm::signalOpenRegForm, [=](){ stack->setCurrentWidget(regForm); });
    connect(authForm, &AuthForm::signalOpenForgotForm, [=](QString login){
        forgotForm->setLogin(login);
        stack->setCurrentWidget(forgotForm);
    });

    connect(authForm, &AuthForm::signalOpenAboutForm, [=](){
        stack->setCurrentWidget(aboutForm);
    });

    connect(regForm, &RegForm::signalOpenAboutForm, [=](){
        stack->setCurrentWidget(aboutForm);
    });

    connect(regForm, &RegForm::signalOpenAuthForm, [=](){ stack->setCurrentWidget(authForm); });
    connect(forgotForm, &ForgotForm::signalOpenAuthForm, [=](){ stack->setCurrentWidget(authForm); });

    connect(taskForm, &Taskform::signalLogout, this, [=]() {
        auto res = QMessageBox::question(this, "Выход", "Вы уверены, что хотите выйти из аккаунта?");
        if (res == QMessageBox::Yes) {
            Client_API::getInstance().disconnectFromServer();
            Client_API::getInstance().connectToServer("127.0.0.1", 13579);

            this->showNormal();
            stack->setCurrentWidget(authForm);
        }
    });

    auto exitApp = [=](){
        auto res = QMessageBox::question(this, "Выход", "Закрыть программу?");
        if (res == QMessageBox::Yes) {
            QApplication::quit();
        }
    };

    connect(authForm, &AuthForm::signalExit, this, exitApp);
    connect(regForm, &RegForm::signalExit, this, exitApp);
    connect(forgotForm, &ForgotForm::signalExit, this, exitApp);
    connect(taskForm, &Taskform::signalExit, this, exitApp);

    this->resize(1200, 800);
    this->setMinimumSize(950, 650);
    this->setWindowTitle("Система визуализации графиков");

    stack->setCurrentWidget(coverForm);
}

Manager_Of_Forms::~Manager_Of_Forms() {}
