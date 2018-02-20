#include "peliikkuna.hh"
#include "ui_peliikkuna.h"
#include "drooni.hh"
#include "nyssekuva.hh"
#include "pelialue.hh"
#include <QDebug>
#include <iostream>
#include<QString>
#include<string>
#include <QGraphicsPixmapItem>
#include <QRectF>
#include <QPointF>
#include <QScrollBar>
#include <QTimer>
#include <QDebug>

PeliIkkuna::PeliIkkuna(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PeliIkkuna)
{
    ui->setupUi(this);

    scene_ = new PeliAlue();

    ui->pelinaytto->setScene(scene_);
    //ui->pelinaytto->setViewport(new QGLWidget());
    ui->pelinaytto->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    ui->pelinaytto->setFocus();



}

PeliIkkuna::~PeliIkkuna()
{
    delete ui;
}

void PeliIkkuna::asetaKartta(QImage &kartta)
{
    QPointF ylanurkka = kartta.offset();
    scene_->setSceneRect(ylanurkka.x(), ylanurkka.y(), kartta.width(), kartta.height());
    QGraphicsPixmapItem* karttatemp = scene_->addPixmap(QPixmap::fromImage(kartta));
    karttatemp->setPos(kartta.offset());

    // Tasasta ruudunpäivitystä varten toteutus. Käytännössä ei käyttökelpoinen koska
    // vakavia performance ongelmia :(
//    ruudunpaivittaja_ = new QTimer(this);
//    connect(ruudunpaivittaja_, &QTimer::timeout, this, &PeliIkkuna::paivitaViewport);
//    ruudunpaivittaja_->start(17);

}

QGraphicsPixmapItem *PeliIkkuna::lisaa_pysakki(std::shared_ptr<Rajapinta::PysakkiRP> pysakkiptr){

    QImage pysakkikuva;
    pysakkikuva.load(":/images/images/pallo.png");

    if (pysakkikuva.isNull()) {
        std::cout << "Pysäkki kuvan lataus epäonnistui!" << std::endl;
        return nullptr;
    }
    pysakkikuva = pysakkikuva.scaledToWidth(15, Qt::SmoothTransformation);

    Rajapinta::Sijainti pysakin_sijainti = pysakkiptr->annaSijainti();

    QGraphicsPixmapItem* pysakki = new QGraphicsPixmapItem(QPixmap::fromImage(pysakkikuva));

    // Asetetaan pysäkki paikallensa ja keskitetään kuva
    pysakki->setPos(pysakin_sijainti.annaX(), (520-pysakin_sijainti.annaY()));
    pysakki->setOffset(-(pysakkikuva.width() / 2), -(pysakkikuva.height() / 2));

    scene_->addItem(pysakki);

    return pysakki;
}

Nyssekuva* PeliIkkuna::lisaa_nysse(std::shared_ptr<Rajapinta::ToimijaRP> toimijaptr)
{
    Nyssekuva* nysse = new Nyssekuva(toimijaptr);

    nysse->setPos(toimijaptr->annaSijainti().annaX(),
                    (520 - toimijaptr->annaSijainti().annaY()));
    nysse->setZValue(2);

    scene_->addItem(nysse);

    return nysse;
}

void PeliIkkuna::lisaaDrooni(Drooni* drooni)
{
    drooni_ = drooni;
    scene_->lisaaDrooni(drooni);

    connect(ui->upButton, &QPushButton::pressed, drooni_, &Drooni::alaLiikkuaEteen);
    connect(ui->upButton, &QPushButton::released, drooni_, &Drooni::lopetaEteenLiikkuminen);
    connect(ui->downButton, &QPushButton::pressed, drooni_, &Drooni::alaLiikkuaTaakse);
    connect(ui->downButton, &QPushButton::released, drooni_, &Drooni::lopetaTaakseLiikkuminen);
    connect(ui->leftButton, &QPushButton::pressed, drooni_, &Drooni::alaKaantyaVasta);
    connect(ui->leftButton, &QPushButton::released, drooni_, &Drooni::lopetaKaannosVasta);
    connect(ui->rightButton, &QPushButton::pressed, drooni_, &Drooni::alaKaantyaMyota);
    connect(ui->rightButton, &QPushButton::released, drooni_, &Drooni::lopetaKaannosMyota);
    connect(ui->ammuButton, &QPushButton::pressed, drooni_, &Drooni::ammu);

    connect(ui->rynkky, &QPushButton::pressed, drooni_, &Drooni::valitseRynkky);
    connect(ui->miina, &QPushButton::pressed, drooni_, &Drooni::valitseMiina);
    connect(ui->laser, &QPushButton::pressed, drooni_, &Drooni::valitseLaser);
    connect(drooni_, &Drooni::drooniLiikkunut, this, &PeliIkkuna::liikutaViewport);
    connect(drooni_, &Drooni::drooniAmpunut, this, &PeliIkkuna::paivitaAmmukset);

    ui->rkAmmukset->display(drooni_->ammuksiaJaljella(0));
    ui->mmAmmukset->display(drooni_->ammuksiaJaljella(1));
    ui->laserAmmukset->display(drooni_->ammuksiaJaljella(2));
}

void PeliIkkuna::PaivitaTulokset(std::vector<int> tulokset)

{
 ui->lcdNumber->display(tulokset.at(0));
 ui->lcdNumber_2->display(tulokset.at(1));
 ui->lcdNumber_3->display(tulokset.at(2));
 ui->lcdNumber_4->display(tulokset.at(3));
 ui->lcdNumber_5->display(tulokset.at(4));
}

void PeliIkkuna::naytaToplista(std::vector<QString> huipputulokset)
{
    QString riviTulos{""};
    int sijoitus{1};

    for(auto tulos:huipputulokset){
        riviTulos=QString::number(sijoitus)+"."+" "+tulos;

        ui->toplista_browser->append(riviTulos);
        sijoitus++;
    }
}

void PeliIkkuna::asetaStatus(QString status)
{
    ui->statusLabel->setText(status);
}


void PeliIkkuna::liikutaViewport(QGraphicsItem* kohde)
{
    ui->pelinaytto->ensureVisible(kohde, 100, 100);
}

// Tarjoaa tasaisen ruudunpäivityksen. Harmi vain että on turhan raskas.
void PeliIkkuna::paivitaViewport()
{
    scene_->update();
}

void PeliIkkuna::paivitaAmmukset(int ase, int ammuksia)
{
    switch (ase) {
    case 0:
        ui->rkAmmukset->display(ammuksia);
        break;
    case 1:
        ui->mmAmmukset->display(ammuksia);
        break;
    case 2:
        ui->laserAmmukset->display(ammuksia);
        break;
    default:
        qDebug() << "Tuntematonta asetta käytetty!";
        break;
    }

}


