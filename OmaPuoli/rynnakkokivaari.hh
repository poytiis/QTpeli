#ifndef RYNNAKKOKIVAARI_H
#define RYNNAKKOKIVAARI_H

#include "ammus.hh"
#include <QGraphicsObject>
#include <QPointF>
/**
 * @file
 * @brief Ammus luokasta periytetty luokka, määrittelee rynnäkkökiväärin ammuksen.
 */

class Rynnakkokivaari: public Ammus
{
public:
    //Rynnakkokivaari();

    /**
     * @brief Rynnakkokivaarin rakentaja
     * @param parent ammuksen ampuja
     * @param suunta ammuksen lähtösuunta
     * @post Rynnäkkökiväärin ammus on alustettu ja toiminnassa. Poikkeustakuu: perus
     */
    Rynnakkokivaari(QGraphicsItem *parent, qreal suunta);

    /**
      * @brief Ammuksen purkaja, tuhoaa new'llä luodun QTimerin.
      * @post ammus on tuhottu oikein.
      */
    ~Rynnakkokivaari();

protected:
    // Ammusluokasta perittyjen funktioiden toteutukset
    virtual void asetaAmmuskuva();

private:
    // Ammusluokassa määriteltyjen arvojen korvaus ryhnnäkkökivääriin paremmin sopiviin.
    int nopeus{6000};
    int kantama{300};
};

#endif // RYNNAKKOKIVAARI_H
