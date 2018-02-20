// Pelin loppuessa esitettävä dialogi.
// Kertoo saadut pisteet ja sijoituksen mikäli pelaaja pääsi top10
// listalle.

#ifndef LOPETUSDIALOG_HH
#define LOPETUSDIALOG_HH

#include "tilasto.hh"
#include <QDialog>

namespace Ui {
class LopetusDialog;
}

class LopetusDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LopetusDialog(QWidget *parent = 0);
    ~LopetusDialog();

    // Tilasto on annettava ennen dialogin näyttämistä. Explicit rakentaja ei
    // tykännyt kun siihen yritti implementoida tämänkin toiminnallisuuden :D
    void annaTilasto(std::shared_ptr<Tilasto> tilastoptr);

    // Tallettaa top10 tuloksen ja lopettaa pelin
    void tallennaJaLopeta();

private:
    Ui::LopetusDialog *ui;
    std::shared_ptr<Tilasto> tilasto_olio_;
};

#endif // LOPETUSDIALOG_HH
