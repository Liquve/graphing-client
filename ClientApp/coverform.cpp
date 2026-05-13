#include "coverform.h"
#include "ui_coverform.h"

CoverForm::CoverForm(QWidget *parent) : QWidget(parent), ui(new Ui::CoverForm) {
    ui->setupUi(this);
    connect(ui->btn_start, &QPushButton::clicked, this, &CoverForm::signalOpenAuth);
}

CoverForm::~CoverForm() { delete ui; }
