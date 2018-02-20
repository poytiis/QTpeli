#include "laser.hh"
#include "pelivirhe.hh"
#include "nyssekuva.hh"
#include <QPainter>
#include <QPointF>
#include <QGraphicsScene>
#include <QDebug>
#include <QGraphicsItem>

Laser::Laser(QGraphicsItem *parent, qreal suunta)
{
    asetaAmmuskuva();
    Ammus::nopeus = nopeus;
    Ammus::kantama = kantama;
    alustaAmmus(parent, suunta - 180);

    QTimer::singleShot(200, this, &Ammus::tuhoaAmmus);
}

Laser::~Laser()
{

}

void Laser::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawImage(-(ammuskuva.width() / 2), 0,ammuskuva);
}

QRectF Laser::boundingRect() const
{
    //qDebug() << "IMAHFIRING MAI LAZER!!!";
    qreal margin = 0.5;
    return QRectF(-(ammuskuva.width() / 2) - margin, 0, ammuskuva.width() + 2 * margin, ammuskuva.height() + 2 * margin);
}

void Laser::asetaAmmuskuva()
{
    Ammus::ammuskuva.load(":/images/images/laser.png");
    if (ammuskuva.isNull()) {
        throw Rajapinta::PeliVirhe("Ammuskuvan avaaminen epäonnistui!");
    }
}

bool Laser::tarkistaOsuma()
{
    bool osumia{false};
    QList<QGraphicsItem*> osumat = collidingItems();
    for (int i = 0; i < osumat.size(); ++i) {
        Nyssekuva* nysseptr = dynamic_cast<Nyssekuva*>(osumat.at(i));
        if (nysseptr != nullptr && scene()->sceneRect().contains(nysseptr->pos().x(), nysseptr->pos().y())
                && !nysseptr->tuhottu()) {
            //qDebug() << "OSUMA!!! ROAR!";
            nysseptr->osuma();
            osuma();
            if (!osumia) osumia = true;
        }
    }
    if (osumia) return true;
    else return false;
}
