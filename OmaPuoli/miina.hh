#ifndef MIINA_H
#define MIINA_H

#include "ammus.hh"
#include <QGraphicsObject>
#include <QPointF>
#include <unordered_map>
#include <QImage>
#include <QPropertyAnimation>

/**
 * @file
 * @brief Ammus luokasta periytetty luokka, määrittelee miinan toiminnan. Räjähdyksessä käytetyt kuvat http://wrathgames.com/blog WrathGames Studio.
 * Muistin loppuessa voidaan heittää aina std::bad_alloc.
 */

class Miina: public Ammus
{
    Q_OBJECT
    Q_PROPERTY(int rVaihe READ getRajahdysVaihe WRITE setRajahdysVaihe)
public:

    /**
     * @brief Miinan rakentaja
     * @param parent miinan jättäjä, yleensä drooni
     * @post Miina asetettu ja alustettu. Poikkeustakuu: perus
     */
    Miina(QGraphicsItem *parent);

    /**
      * @brief Miinan purkaja
      * @post miina on tuhottu oikein
      */
    ~Miina();

    // Setter ja Getter Q_PROPERTY makroa varten animoinnin toteuttamiseen.
    int getRajahdysVaihe() const;
    void setRajahdysVaihe(int rajahdysVaihe);

protected:
    // Ammusluokasta periytettyjen funktioiden toteutukset
    virtual void asetaAmmuskuva();
    //virtual bool tarkistaOsuma();

    // Tehdään alustukselle oma toteutus alustuksesta jossa hitaampi QTimer
    virtual void alustaAmmus(QGraphicsItem *parent, qreal suunta);

    // Räjähdystä varten määritellään osuma() uudelleen
    virtual void osuma();

    // Räjähdysten ketjuttamista varten määritellään tarkistaOsuma uudelleen niin,
    // että se huomioi myös toiset, vielä räjähtämättömät, miinat mikäli räjähdys on
    // käynnissä.
    virtual bool tarkistaOsuma();

private:
    // Ammusluokassa määriteltyjen arvojen korvaus miinalle tyypillisillä arvoilla
    int nopeus{0};
    int kantama{30};
    // Herkkyys kertoo kuinka usein osuma tarkastetaan (ms)
    short int herkkyys_{2000};

    // Räjähdysanimaation ajamisessa käytettyjä arvoja.

    // rajahdysVaihe kertoo animaation hetken väliltä 1-90.
    int rajahdysVaihe_{0};
    // Animaation käyttämät .png kuvat nätisti mapissa.
    std::unordered_map<short int, QImage> rajahdyskuvat_;
    // Kertoo onko ammus räjähtänyt, eli onko animaatio käynnissä.
    bool rajahtanyt_;
    // Itse animaatio.
    QPropertyAnimation* rajahdysAnimaatio_;
};

#endif // MIINA_H
