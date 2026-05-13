#ifndef COVERFORM_H
#define COVERFORM_H

#include <QWidget>

namespace Ui { class CoverForm; }

class CoverForm : public QWidget {
    Q_OBJECT
public:
    explicit CoverForm(QWidget *parent = nullptr);
    ~CoverForm();

signals:
    void signalOpenAuth();

private:
    Ui::CoverForm *ui;
};

#endif
