#ifndef KONFFIIKKUNA_H
#define KONFFIIKKUNA_H

#include <QDialog>
#include <QTime>

namespace Ui {
class KonffiIkkuna;
}

namespace KurssinPuoli
{

/**
 * @brief Konfiguraatio-dialogi.
 */
class KonffiIkkuna : public QDialog
{
    Q_OBJECT

public:

    /**
     * @brief Rakentaja
     * @param parent QDialogin parent.
     * @pre -
     */
    explicit KonffiIkkuna(QWidget *parent = 0);

    /**
     * @brief Purkaja.
     */
    virtual ~KonffiIkkuna();

    /**
     * @brief Erikoistettu QDialog::exec. Suorittaa dialogin.
     * @return QDialog::Accepted, jos käyttäjä hyväksyi konfiguraation.
     *  Muuten palautetaan QDialog::Rejected.
     * @pre -
     * @post Emittoi konffiMuuttunut-signaalin, jos käyttäjä hyväksyi konfiguraation.
     */
    virtual int exec();


signals:

    /**
     * @brief Signaali, jolla kerrotaan hyväksytystä konfiguraatiosta
     * @param aika Pelin aloitusaika.
     * @param debug Tosi, jos käytetään yhden bussin debug-konfiguraatiota.
     */
    void konffiMuuttunut(QTime aika, bool debug);

private:
    Ui::KonffiIkkuna *ui;
};

} // KurssinPuoli

#endif // KONFFIIKKUNA_H
