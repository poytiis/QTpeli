#include "lopetusdialog.hh"
#include "ui_lopetusdialog.h"
#include "tilasto.hh"
#include <QString>
#include <QApplication>
#include <string>
#include <QDebug>

LopetusDialog::LopetusDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LopetusDialog)
{
    ui->setupUi(this);

}

LopetusDialog::~LopetusDialog()
{
    delete ui;
}

void LopetusDialog::annaTilasto(std::shared_ptr<Tilasto> tilastoptr)
{
    tilasto_olio_ = tilastoptr;

    // Kun ollaan saatu tilasto osoitin, voidaan lopetusdialogi kustomoida
    // sijoituksen mukaan.
    int pisteet = tilasto_olio_->annaPisteet();
    ui->pisteet->display(pisteet);
    int sijoitus = tilasto_olio_->selvitaListasijoitus(pisteet);

    // Mikäli ei päästy top10 listalle, ei turhaan kysytä nimimerkkiä
    if (sijoitus > 10) {
        ui->top10Label->setText("Tuloksesi ei valitettavasti yltänyt Top10 listalle.");
        ui->nimimerkkiLabel->hide();
        ui->nimimerkkiKentta->hide();
        ui->lopetaButton->setText("Lopeta");
        connect(ui->lopetaButton, &QPushButton::clicked, this,
                &LopetusDialog::accept);
    }

    // Päästiin top10
    // Onnitellaan, kysytään nimimerkki ja valmistaudutaan tallentamaan tulos
    else {
        QString onnittelu_txt = QStringLiteral("Ylsit tuloksellasi Top10 listan sijalle %1, onneksi olkoon!").arg(tilasto_olio_->selvitaListasijoitus(tilasto_olio_->annaPisteet()));

        qDebug() << onnittelu_txt;
        ui->top10Label->setText(onnittelu_txt);
        ui->lopetaButton->setText("Tallenna ja lopeta");
        connect(ui->lopetaButton, &QPushButton::clicked, this,
                &LopetusDialog::tallennaJaLopeta);
    }
}

void LopetusDialog::tallennaJaLopeta()
{
    QString nimimerkki{ui->nimimerkkiKentta->text()};

    // Mikäli nimimerkkiä ei ole syötetty, ei tallenneta ollenkaan tulosta listoille
    if (nimimerkki.length() > 0) {
        tilasto_olio_->kirjoitaTiedostoon(tilasto_olio_->annaPisteet(),
                                          nimimerkki);
    }
    accept();
}
