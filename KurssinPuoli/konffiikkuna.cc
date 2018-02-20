#include "konffiikkuna.hh"
#include "ui_konffiikkuna.h"

namespace KurssinPuoli
{

KonffiIkkuna::KonffiIkkuna(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::KonffiIkkuna)
{
    ui->setupUi(this);
    connect(ui->btnAja, SIGNAL(clicked(bool)), this, SLOT(accept()) );
    connect(ui->btnPeruuta, SIGNAL(clicked(bool)), this, SLOT(reject()) );

    ui->timeEdit->setTime(QTime::fromString("10:00", "hh:mm"));
}

KonffiIkkuna::~KonffiIkkuna()
{
    delete ui;
}


int KonffiIkkuna::exec()
{
    int rv = QDialog::exec();
    if (rv == QDialog::Accepted){
        emit konffiMuuttunut(ui->timeEdit->time(), ui->cbDebug->isChecked());
    }
    return rv;
}

} // KurssinPuoli
