#ifndef MANAGER_OF_FORMS_H
#define MANAGER_OF_FORMS_H

#include <QWidget>
#include "authform.h"
#include "regform.h"
#include "forgotform.h"

class Manager_Of_Forms : public QWidget
{
    Q_OBJECT

public:
    explicit Manager_Of_Forms(QWidget *parent = nullptr);
    ~Manager_Of_Forms(); // Добавили деструктор для порядка

private:
    // Вот наши три "куклы", которыми будет рулить менеджер
    AuthForm *authForm;
    RegForm *regForm;
    ForgotForm *forgotForm;
};

#endif // MANAGER_OF_FORMS_H