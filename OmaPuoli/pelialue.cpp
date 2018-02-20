#include "pelialue.hh"
#include "sijainti.hh"
#include "pelivirhe.hh"
#include <QPointF>

PeliAlue::PeliAlue()
{

}

void PeliAlue::lisaaDrooni(Drooni *drooni)
{
    if (!drooni_maaritelty_) {
        this->addItem(drooni);
        drooni_ = drooni;
        this->setFocusItem(drooni);
        drooni_maaritelty_ = true;
        if (drooni_ != drooni || drooni_maaritelty_ == false) {
            throw Rajapinta::PeliVirhe("Droonin lisäys epäonnistui.");
        }
    }
    else {
        throw Rajapinta::PeliVirhe("Pelialueella on jo drooni!");
    }
}

void PeliAlue::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (drooni_maaritelty_) {
        setFocusItem(drooni_);

        Rajapinta::Sijainti sij;
        QPointF target = mouseEvent->scenePos();
        sij.asetaXY(target.x(), 520 - target.y());

        drooni_->liikuSulavasti(sij);
    }
}
