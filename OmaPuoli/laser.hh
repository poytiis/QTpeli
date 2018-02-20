#ifndef LASER_H
#define LASER_H

#include "ammus.hh"
#include <QGraphicsObject>
#include <QRectF>

/**
 * @file
 * @brief Ammus luokasta periytetty luokka, määrittelee laserin toiminnan.
 */

class Laser: public Ammus
{
public:

    /**
     * @brief Laserin rakentaja
     * @param parent laserin ampunut toimija, yleensä drooni
     * @param suunta suunta minne laser lähtee
     * @post Laser luotu onnistuneesti ja ajoitettu tuhoutumaan 200ms kuluttua. Poikkeustakuu: perus
     */
    Laser(QGraphicsItem *parent, qreal suunta);

    /**
      * @brief Laserin purkaja
      * @post Laser on tuhottu oikein
      */
    ~Laser();
protected:
    // Jotta collisionDetection toimii oikein ja laser lähtee vain eteenpäin
    // pitää paint ja boundingRect toteuttaa hieman kantaluokasta eriävällä tavalla
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual QRectF boundingRect() const override;

    // Esitellään asetaAmmuskuva() koska ammusluokka vaatii sen. Todellisuudessa
    // piirretään viiva paint funktiossa.
    virtual void asetaAmmuskuva();
    virtual bool tarkistaOsuma();

private:
    // Määritellään laserille oma nopeus ja kantama
    int nopeus{0}; // Laser ei etene, se on instant!
    int kantama{2000}; // Koko pelialue käytännössä
};

#endif // LASER_H
