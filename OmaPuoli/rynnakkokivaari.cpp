#include "rynnakkokivaari.hh"
#include "nyssekuva.hh"
#include "pelivirhe.hh"
#include <QDebug>
#include <QGraphicsItem>
#include <QImage>
#include <QList>


Rynnakkokivaari::Rynnakkokivaari(QGraphicsItem* parent, qreal suunta)
{
    asetaAmmuskuva();
    Ammus::nopeus = nopeus;
    Ammus::kantama = kantama;
    alustaAmmus(parent, suunta);
}

Rynnakkokivaari::~Rynnakkokivaari()
{

}


void Rynnakkokivaari::asetaAmmuskuva() {
    Ammus::ammuskuva.load(":/images/images/drooni.png");
    if (ammuskuva.isNull()) {
        throw Rajapinta::PeliVirhe("Ammuskuvan avaaminen epäonnistui!");
    }
    Ammus::ammuskuva = Ammus::ammuskuva.scaledToHeight(10);
}
