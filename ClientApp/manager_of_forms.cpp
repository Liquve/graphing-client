#include "manager_of_forms.h"

Manager_Of_Forms::Manager_Of_Forms(QWidget *parent) : QWidget(parent)
{
    // 1. Создаем все окна (они пока скрыты)
    authForm = new AuthForm();
    regForm = new RegForm();
    forgotForm = new ForgotForm();

    // 2. СЛУШАЕМ КРИКИ (СИГНАЛЫ) ОТ ОКОН И ПЕРЕКЛЮЧАЕМ ИХ

    // Если Авторизация кричит "Открыть Регистрацию!"
    connect(authForm, &AuthForm::signalOpenRegForm, this, [=](){
        authForm->hide(); // Прячем вход
        regForm->show();  // Показываем регистрацию
    });

    // Если Авторизация кричит "Забыл пароль!"
    connect(authForm, &AuthForm::signalOpenForgotForm, this, [=](){
        authForm->hide();
        forgotForm->show();
    });

    // Если Регистрация кричит "Назад!"
    connect(regForm, &RegForm::signalOpenAuthForm, this, [=](){
        regForm->hide();
        authForm->show();
    });

    // Если Сброс пароля кричит "Назад!"
    connect(forgotForm, &ForgotForm::signalOpenAuthForm, this, [=](){
        forgotForm->hide();
        authForm->show();
    });

    // 3. САМОЕ ГЛАВНОЕ: При запуске программы показываем только окно Входа
    authForm->show();
}

Manager_Of_Forms::~Manager_Of_Forms()
{
    // Убираем за собой мусор при закрытии программы
    delete authForm;
    delete regForm;
    delete forgotForm;
}