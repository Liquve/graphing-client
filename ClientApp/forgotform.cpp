#include "forgotform.h"
#include "ui_forgotform.h"

ForgotForm::ForgotForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ForgotForm)
{
    ui->setupUi(this);
}

ForgotForm::~ForgotForm()
{
    delete ui;
}

void ForgotForm::on_btn_back_clicked()
{
    emit signalOpenAuthForm();
}

