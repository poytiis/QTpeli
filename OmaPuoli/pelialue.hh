#ifndef PELIALUE_H
#define PELIALUE_H

#include "drooni.hh"
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

/**
 * @file
 * @brief PeliAlue on periytetty QGraphicsScenestä pelin tarkoituksiin sopivaksi.
 */

class PeliAlue: public QGraphicsScene
{
public:
    PeliAlue();

    /**
     * @brief lisaaDrooni lisää droonin pelialueelle.
     * @param drooni Osoitin lisättävään drooniin.
     * @pre Pelialueelle ei ole vielä lisätty droonia.
     * @post Drooni on lisätty pelialueelle. Poikkeustakuu: perus
     * @exception PeliVirhe mikäli pelialueella on jo drooni tai droonin lisäys
     * epäonnistui.
     */
    void lisaaDrooni(Drooni* drooni);
protected:
    // Korvataan QGraphicsScenen oletustoteutus droonin hallinnan parantamiseksi.
    // WIP
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);

private:
    bool drooni_maaritelty_{false};
    Drooni* drooni_;
};

#endif // PELIALUE_H
