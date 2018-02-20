#ifndef NYSSEKUVA_H
#define NYSSEKUVA_H

#include "kaupunkirp.hh"
#include <toimijarp.hh>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QGraphicsItem>
#include <memory>

/**
 * @file
 * @brief Nyssekuva tarjoaa graafisen ilmeen pelin nysseille. Periytetty QGraphicsItemista
 */
class Nyssekuva: public QGraphicsItem
{
public:
    /**
     * @brief Nyssekuva Luokan rakentaja
     * @param nysse osoitin kuvastettavaan nysseen
     */
    Nyssekuva(std::shared_ptr<Rajapinta::ToimijaRP> nysse);

    /**
      * @brief Luokan purkaja
      * @post Nyssekuva on tuhottu oikein
      */
    ~Nyssekuva();

    /**
     * @brief osuma Kutsutaan kun nysse on ottanut osuman ammuksesta.
     * Ilmoittaa pelilogiikan nysselle tuhoutumisesta (Tuhoaa toimijan ja kertoo kaupungille)
     * @pre Nyssekuva on ottanut osuman
     * @post Nysse on tuhottu. Poikkeustakuu: perus
     */
    void osuma();

    /**
     * @brief kerroKaupunki kertoo Nyssekuvalle missä kaupungissa se on.
     * @param kaupunki KaupunkiRP:n toteuttava kaupunkiolio
     * @post kaupunki_ asetettu. Poikkeustakuu: nothrow
     */
    void kerroKaupunki(Rajapinta::KaupunkiRP* kaupunki);

    /**
     * @brief tuhoudu Vaihtaa nyssekuvan ulkoasun tuhoutuneeseen nysseen.
     * @pre -
     * @post Ulkoasu on tuhoutunut nysse. Poikkeustakuu: nothrow
     */
    void tuhoudu();

    /**
     * @brief tuhottu Kertoo onko Nyssekuvan ulkoasu tuhoutunut nysse.
     * @return Boolean arvo
     * @post Poikkeustakuu: nothrow
     */
    bool tuhottu();
protected:
    // QGraphicsItemin puhtaiden virtuaalifunktioiden toteutukset
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual QRectF boundingRect() const;

private:
    bool tuhottu_{false};
    QGraphicsPixmapItem* realNysse_;
    std::shared_ptr<Rajapinta::ToimijaRP> nysse_;
    Rajapinta::KaupunkiRP* kaupunki_;
};

#endif // NYSSEKUVA_H
