#include "ammus.hh"
#include "nyssekuva.hh"
#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QDebug>
#include <qmath.h>

const float PI = 3.14159265;

Ammus::Ammus()
{

}

Ammus::Ammus(QGraphicsItem *parent, qreal suunta)
{
    alustaAmmus(parent, suunta);
}

Ammus::~Ammus()
{
    delete animaatioajastin;
}

QPointF Ammus::suuntaPiste() const
{
    qreal dx, dy;
    dx = qCos((rotation() - 90) * PI / 180);
    dy = qSin((rotation() - 90) * PI / 180);

    return QPointF(dx, dy);
}

void Ammus::etene()
{
    // Liikutetaan ammusta
    setPos(pos() + (suuntaPiste() * (nopeus / 1000)));
    etenema += nopeus / 1000;

    // Tarkasta onko kantamalla
    if (etenema > kantama) {
        scene()->removeItem(this);
        delete this;
        return;
    }

    // Tarkasta osuiko
    if (tarkistaOsuma()) {
        if (animoituOsuma_) {
            if (!tuhoutumassa) {
                QTimer::singleShot(2200, this, &Ammus::tuhoaAmmus);
                tuhoutumassa = true;
            }
            return;
        }
        else{
            tuhoaAmmus();
            return;
        }
    }

}

void Ammus::ulkoOsuma()
{
    if (animoituOsuma_) {
        if (!tuhoutumassa) {
            QTimer::singleShot(2200, this, &Ammus::tuhoaAmmus);
            tuhoutumassa = true;
        }
        return;
    }
    else{
        tuhoaAmmus();
        return;
    }
}

void Ammus::tuhoaAmmus()
{
    scene()->removeItem(this);
    deleteLater();
}

void Ammus::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawImage(-(ammuskuva.width() / 2), -(ammuskuva.height() / 2),ammuskuva);
}

QRectF Ammus::boundingRect() const
{
    //qDebug() << "Ollaan vanhemmassa!";
    qreal margin = 2.5;
    return QRectF(-(ammuskuva.width() / 2) - margin, -(ammuskuva.height() / 2) - margin, ammuskuva.width() + 2 * margin, ammuskuva.height() + 2 * margin);
}

void Ammus::alustaAmmus(QGraphicsItem *parent, qreal suunta)
{
    etenema = 0;
    setRotation(suunta);
    setPos(parent->pos());
    setZValue(5);

    animaatioajastin = new QTimer(this);
    connect(animaatioajastin, &QTimer::timeout, this, &Ammus::etene);
    animaatioajastin->start(100);
}

bool Ammus::tarkistaOsuma()
{
    QList<QGraphicsItem*> osumat = collidingItems();
    for (int i = 0; i < osumat.size(); ++i) {
        Nyssekuva* nysseptr = dynamic_cast<Nyssekuva*>(osumat.at(i));
        if (nysseptr != nullptr && scene()->sceneRect().contains(nysseptr->pos().x(), nysseptr->pos().y())
                && !nysseptr->tuhottu()) {
            //qDebug() << "OSUMA!!! ROAR!";
            nysseptr->osuma();
            osuma();
            return true;
        }
    }
    return false;
}

void Ammus::osuma()
{
    return;
}

void Ammus::asetaAmmuskuva()
{
    qDebug() << "Virtuaalinen ammuskuva kutsuttu, miksi?";
}
