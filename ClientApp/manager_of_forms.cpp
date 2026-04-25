#include "manager_of_forms.h"
#include "client_api.h"
#include <QVBoxLayout>
#include <QMessageBox>
#include <QApplication>

Manager_Of_Forms::Manager_Of_Forms(QWidget *parent) : QWidget(parent)
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    stack = new QStackedWidget(this);
    mainLayout->addWidget(stack);

    authForm = new AuthForm();
    regForm = new RegForm();
    forgotForm = new ForgotForm();
    taskForm = new Taskform();

    stack->addWidget(authForm);
    stack->addWidget(regForm);
    stack->addWidget(forgotForm);
    stack->addWidget(taskForm);

    // ОБРАБОТКА ОТВЕТОВ ОТ СЕРВЕРА
    connect(&Client_API::getInstance(), &Client_API::signalResponse, this, [=](uint64_t id, bool success, std::vector<std::string> params, QString errorMsg) {

        // 1. ОБРАБОТКА ОШИБОК
        if (!success) {
            // Если мы НЕ в окне забытого пароля - показываем ошибку здесь (для входа и регистрации)
            // Окно ForgotForm само покажет свою ошибку через свой внутренний коннект
            if (stack->currentWidget() != forgotForm) {
                QMessageBox::warning(this, "Внимание", errorMsg);
            }
            return;
        }

        // 2. ОБРАБОТКА УСПЕХА (ПЕРЕКЛЮЧЕНИЕ ЭКРАНОВ)
        QWidget* current = stack->currentWidget();

        // Успешный вход -> График
        if (current == authForm) {
            stack->setCurrentWidget(taskForm);
        }
        // Успешная регистрация -> Вход
        else if (current == regForm) {
            QMessageBox::information(this, "Успех", "Регистрация завершена! Теперь войдите.");
            stack->setCurrentWidget(authForm);
        }
        // Успех в сбросе пароля -> Вход
        else if (current == forgotForm) {
            // Если это не выдача токена (параметр короткий или отсутствует), значит пароль изменен
            if (params.empty() || params[0].length() < 20) {
                stack->setCurrentWidget(authForm);
            }
        }
    });

    // НАВИГАЦИЯ МЕЖДУ ОКНАМИ
    // Переходы, которые уже были
    connect(authForm, &AuthForm::signalOpenRegForm, [=](){ stack->setCurrentWidget(regForm); });
    connect(authForm, &AuthForm::signalOpenForgotForm, [=](QString login){
        forgotForm->setLogin(login);
        stack->setCurrentWidget(forgotForm);
    });
    connect(regForm, &RegForm::signalOpenAuthForm, [=](){ stack->setCurrentWidget(authForm); });
    connect(forgotForm, &ForgotForm::signalOpenAuthForm, [=](){ stack->setCurrentWidget(authForm); });

    // НОВАЯ ЛОГИКА: ВЫХОД ИЗ АККАУНТА
    connect(taskForm, &Taskform::signalLogout, this, [=](){
        // Можно спросить подтверждение
        auto res = QMessageBox::question(this, "Выход", "Вы уверены, что хотите выйти из аккаунта?");
        if (res == QMessageBox::Yes) {
            stack->setCurrentWidget(authForm);
            // Очищаем поля пароля для безопасности
            // authForm->clearFields(); // если создашь такой метод
        }
    });

    // НОВАЯ ЛОГИКА: ЗАВЕРШЕНИЕ РАБОТЫ (EXIT)
    // Подключаем сигнал выхода от всех форм к закрытию приложения
    auto exitApp = [=](){
        auto res = QMessageBox::question(this, "Выход", "Закрыть программу?");
        if (res == QMessageBox::Yes) {
            qApp->quit();
        }
    };

    connect(authForm, &AuthForm::signalExit, this, exitApp);
    connect(regForm, &RegForm::signalExit, this, exitApp);
    connect(forgotForm, &ForgotForm::signalExit, this, exitApp);
    connect(taskForm, &Taskform::signalExit, this, exitApp);
}

Manager_Of_Forms::~Manager_Of_Forms() {}
