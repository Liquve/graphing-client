#ifndef MANAGER_OF_FORMS_H
#define MANAGER_OF_FORMS_H

#include <QWidget>
#include <QStackedWidget>
#include "authform.h"
#include "regform.h"
#include "forgotform.h"
#include "taskform.h"

class Manager_Of_Forms : public QWidget
{
    Q_OBJECT

public:
    explicit Manager_Of_Forms(QWidget *parent = nullptr);
    ~Manager_Of_Forms();

private:
    QStackedWidget *stack; // Наш "переключатель" страниц
    AuthForm *authForm;
    RegForm *regForm;
    ForgotForm *forgotForm;
    Taskform *taskForm;
};

#endif
