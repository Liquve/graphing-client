#include "aboutform.h"
#include "ui_aboutform.h"

AboutForm::AboutForm(QWidget *parent) : QWidget(parent), ui(new Ui::AboutForm) {
    ui->setupUi(this);

    connect(ui->btn_back, &QPushButton::clicked, this, &AboutForm::signalBack);
    connect(ui->btn_forward, &QPushButton::clicked, this, &AboutForm::signalForward);

    ui->label_image->setMinimumHeight(150);
}

AboutForm::~AboutForm() { delete ui; }
