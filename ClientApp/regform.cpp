#include "regform.h"
#include "ui_regform.h"

RegForm::RegForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RegForm)
{
    ui->setupUi(this);
}

RegForm::~RegForm()
{
    delete ui;
}

void RegForm::on_btn_back_clicked()
{
    emit signalOpenAuthForm();
}

