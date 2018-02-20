#include "miina.hh"
#include "nyssekuva.hh"
#include "pelivirhe.hh"
#include <QImage>
#include <iostream>
#include <QDebug>
#include <QTimer>
#include <QImage>
#include <QGraphicsScene>
#include <QPropertyAnimation>
#include <string>
#include <vector>

#include <chrono>
#include <thread>

Miina::Miina(QGraphicsItem *parent)
{
    asetaAmmuskuva();
    Ammus::nopeus = nopeus;
    Ammus::kantama = kantama;
    Ammus::animoituOsuma_ = true;
    alustaAmmus(parent, 0);

    // Räjähdysanimaation kuvien lataus ja tallennus mappiin.
    std::vector<std::string> luvut;
    for (short int i = 0; i<9; ++i) {
        for (short int j = 0; j<10; ++j) {
            std::string luku;
            if (i == 0) {
                luku += '0';

            }
            else {
                luku += std::to_string(i);
            }
            luku += std::to_string(j);
            luvut.push_back(luku);
        }
    }
    luvut.push_back("90");
    std::vector<std::string>::iterator it;
    const std::string rKuva{":/images/images/explosion1_00"};
    const std::string rPaate{".png"};
    QString tiedostonimi;
    short int i{1};
    for (it = luvut.begin(); it != luvut.end(); ++it) {
        tiedostonimi = QString::fromUtf8((rKuva + *it + rPaate).c_str());
        rajahdyskuvat_[i] = QImage(tiedostonimi);
        ++i;
    }

    rajahtanyt_ = false;
}

Miina::~Miina()
{
    delete rajahdysAnimaatio_;
}

void Miina::asetaAmmuskuva()
{
    Ammus::ammuskuva.load(":/images/images/drooni.png");
    if (ammuskuva.isNull()) {
        throw Rajapinta::PeliVirhe("Ammuskuvan avaaminen epäonnistui!");
    }
    Ammus::ammuskuva = Ammus::ammuskuva.scaledToHeight(10);
}

void Miina::alustaAmmus(QGraphicsItem *parent, qreal suunta)
{
    etenema = 0;
    setRotation(suunta);
    setPos(parent->pos());
    setZValue(5);

    animaatioajastin = new QTimer(this);
    connect(animaatioajastin, &QTimer::timeout, this, &Miina::etene);
    animaatioajastin->start(herkkyys_);
}

void Miina::osuma()
{
    if (!rajahtanyt_) {
        rajahdysAnimaatio_ = new QPropertyAnimation(this, "rVaihe");
        rajahdysAnimaatio_->setDuration(2000);
        rajahdysAnimaatio_->setStartValue(1);
        rajahdysAnimaatio_->setEndValue(90);

        rajahdysAnimaatio_->start();
        rajahtanyt_ = true;
    }
    else {
        tarkistaOsuma();
        return;
    }
}

bool Miina::tarkistaOsuma()
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

        // Miina voi räjähtäessään räjäyttää toisia, vielä räjähtämättömiä miinoja
        Miina* miinaptr = dynamic_cast<Miina*>(osumat.at(i));
        if (miinaptr != nullptr && rajahtanyt_ && !miinaptr->rajahtanyt_) {
            //qDebug() << "Miina osuu kaveriinsa!";
            miinaptr->osuma();
            miinaptr->ulkoOsuma();
            return true;
        }
    }
    return false;
}

int Miina::getRajahdysVaihe() const
{
    return rajahdysVaihe_;
}

void Miina::setRajahdysVaihe(int rajahdysVaihe)
{
    rajahdysVaihe_ = rajahdysVaihe;
    prepareGeometryChange();
    Ammus::ammuskuva = rajahdyskuvat_[rajahdysVaihe_];

    if (rajahdysVaihe_ == 11) tarkistaOsuma();
}
