#include "drooni.hh"
#include "sijainti.hh"
#include "rynnakkokivaari.hh"
#include "miina.hh"
#include "laser.hh"
#include "pelivirhe.hh"
#include <QGraphicsScene>
#include <QPointF>
#include <iostream>
#include <QtGui>
#include <QtMath>
#include <QDebug>
#include <QTimer>

#define PI 3.14159265
#define FPS 45

Drooni::Drooni(QGraphicsItem *parent)
{
    alus_.load(":/images/images/drooniSilmilla.png");
    if (alus_.isNull()) {
        std::cout << "Drone image load failed!" << std::endl;
    }
    else {
        alus_ = alus_.scaledToHeight(2 * puolikas_);
    }
    setFlags(QGraphicsItem::ItemSendsGeometryChanges
             | QGraphicsItem::ItemIsFocusable);
    //suunta = 0;

    animaatioajastin = new QTimer();
    animaatioajastin->start(1000/FPS);
    liikkuuEteen = false;
}

Drooni::~Drooni()
{

}

void Drooni::keyPressEvent(QKeyEvent *event)
{
    // Yritetään liikutella droonia näppiksellä
    event->accept();
    switch (event->key())
    {
    case Qt::Key_Left:
        alaKaantyaVasta();
        liikkuuSulavasti_ = false;
        break;
    case Qt::Key_Right:
        alaKaantyaMyota();
        liikkuuSulavasti_ = false;
        break;
    case Qt::Key_Up:
        alaLiikkuaEteen();
        liikkuuSulavasti_ = false;
        break;
    case Qt::Key_Down:
        alaLiikkuaTaakse();
        liikkuuSulavasti_ = false;
        break;
    case Qt::Key_Space:
        ammu();
        break;
    default:
        event->ignore();

        QGraphicsObject::keyPressEvent(event);
    }
}

void Drooni::keyReleaseEvent(QKeyEvent *event)
{
    event->accept();
    switch (event->key())
    {
    case Qt::Key_Left:
        if (!event->isAutoRepeat()) {
            lopetaKaannosVasta();
        }
        break;
    case Qt::Key_Right:
        if (!event->isAutoRepeat()) {
            lopetaKaannosMyota();
        }
        break;
    case Qt::Key_Up:
        if (!event->isAutoRepeat()) {
            lopetaEteenLiikkuminen();
        }
        break;
    case Qt::Key_Down:
        if (!event->isAutoRepeat()) {
            lopetaTaakseLiikkuminen();
        }
        break;
    default:
        event->ignore();

        QGraphicsObject::keyReleaseEvent(event);
    }
}

short Drooni::ammuksiaJaljella(int ase) const
{
    if (ase == 0) {
        return rk_ammuksia;
    }
    else if (ase == 1) {
        return mm_ammuksia;
    }
    else if (ase == 2) {
        return laser_ammuksia;
    }
    else {
        throw Rajapinta::PeliVirhe("Valittu ase ei kuulu droonin asevalikoimaan!");
    }
}

Rajapinta::Sijainti Drooni::annaSijainti() const
{
    Rajapinta::Sijainti sij;
    sij.asetaXY(pos().x(), 520 - pos().y());
    return sij;
}

void Drooni::liiku(Rajapinta::Sijainti sij)
{
    // Karu toteutus koska ei käyttöä - ainakaan toistaiseksi
    setPos(sij.annaX(), 520 - sij.annaY());
    emit drooniLiikkunut(this);
}

bool Drooni::onkoTuhottu() const
{
    return tuhottu_;
}

void Drooni::tuhoa()
{
    tuhottu_=true;
}

void Drooni::liikuSulavasti(Rajapinta::Sijainti sij)
{
    sulava_kohde_.setX(sij.annaX());
    sulava_kohde_.setY(520 - sij.annaY());
    liikkuuSulavasti_ = true;
    connect(animaatioajastin, &QTimer::timeout, this, &Drooni::tarkistaSulavaLiike);
}

void Drooni::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    // Siirretään koordinaatistoa hetkellisesti jotta kierto tapahtuu droonin keskipisteen
    // ympäri.
    //painter->translate(puolikas_, puolikas_);
    //painter->rotate(suunta);
    //painter->translate(-puolikas_, -puolikas_);

    // Piirretään drooni
    painter->drawImage(-(puolikas_), -(puolikas_), alus_);
}

QRectF Drooni::boundingRect() const
{
    return QRectF(-puolikas_, -puolikas_, 2 * puolikas_, 2 * puolikas_);
}

// Qt kutsuu aina QGraphicsItem:in itemChange metodia kun tapahtuu muutos. Antamalla
// metodille (lisäksi) oma toteutus pystytään rajoittamaan droonin liikkumisaluetta.
QVariant Drooni::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionChange && scene()) {
        QPointF uusiSijainti = value.toPointF();
        QRectF alue = scene()->sceneRect();
        alue.setBottom((alue.bottom() - alus_.height()));
        alue.setRight((alue.right() - alus_.width()));
        alue.translate(puolikas_, puolikas_);

        if (!alue.contains(uusiSijainti)) {
            uusiSijainti.setX(qMin((alue.right()), qMax(uusiSijainti.x(), alue.left())));
            uusiSijainti.setY(qMin((alue.bottom()), qMax(uusiSijainti.y(), alue.top())));
            return uusiSijainti;
        }
    }
    // Kutsutaan 'normaalia' itemChange metodia.
    return QGraphicsItem::itemChange(change, value);
}

void Drooni::alaLiikkuaEteen()
{
    if (!liikkuuEteen) {
        connect(animaatioajastin, &QTimer::timeout, this, &Drooni::liikuYlos);
        liikkuuEteen = true;
    }
}

void Drooni::alaLiikkuaTaakse()
{
    if (!liikkuuTaakse) {
        connect(animaatioajastin, &QTimer::timeout, this, &Drooni::liikuAlas);
        liikkuuTaakse = true;
    }
}

void Drooni::alaKaantyaMyota()
{
    if (!kaantyyOikealle) {
        connect(animaatioajastin, &QTimer::timeout, this, &Drooni::liikuOikea);
        kaantyyOikealle = true;
    }
}

void Drooni::alaKaantyaVasta()
{
    if (!kaantyyVasemmalle) {
        connect(animaatioajastin, &QTimer::timeout, this, &Drooni::liikuVasen);
        kaantyyVasemmalle = true;
    }
}

void Drooni::lopetaEteenLiikkuminen()
{
    if (liikkuuEteen) {
        disconnect(animaatioajastin, &QTimer::timeout, this, &Drooni::liikuYlos);
        hetkellinen_nopeus_ = 0;
        liikkuuEteen = false;
    }
}

void Drooni::lopetaTaakseLiikkuminen()
{
    if (liikkuuTaakse) {
        disconnect(animaatioajastin, &QTimer::timeout, this, &Drooni::liikuAlas);
        hetkellinen_pakitus_nopeus_ = 0;
        liikkuuTaakse = false;
    }
}

void Drooni::lopetaKaannosMyota()
{
    if (kaantyyOikealle) {
        disconnect(animaatioajastin, &QTimer::timeout, this, &Drooni::liikuOikea);
        kaantyyOikealle = false;
    }
}

void Drooni::lopetaKaannosVasta()
{
    if (kaantyyVasemmalle) {
        disconnect(animaatioajastin, &QTimer::timeout, this, &Drooni::liikuVasen);
        kaantyyVasemmalle = false;
    }
}

void Drooni::liikuYlos()
{
    if (hetkellinen_nopeus_ < nopeus_) hetkellinen_nopeus_ += kiihtyvyys_;

    setPos(pos() + (suuntaPiste() * hetkellinen_nopeus_));

    emit drooniLiikkunut(this);
}

void Drooni::liikuAlas()
{
    if (hetkellinen_pakitus_nopeus_ < pakitus_nopeus_) hetkellinen_pakitus_nopeus_ += kiihtyvyys_ / 2;

    setPos(pos() + (suuntaPiste() * (-hetkellinen_pakitus_nopeus_)));

    emit drooniLiikkunut(this);
}

void Drooni::liikuVasen()
{
    //suunta -= kaantymis_nopeus_;
    setRotation(rotation() - kaantymis_nopeus_);
}

void Drooni::liikuOikea()
{
    //suunta += kaantymis_nopeus_;
    setRotation(rotation() + kaantymis_nopeus_);
}

void Drooni::tarkistaSulavaLiike()
{
    // Jos ollaan tarpeeksi lähellä kohdetta, lopetetaan liikkuminen
    qreal etaisyys = qSqrt(qPow((pos().x() - sulava_kohde_.x()),2) + (qPow((pos().y() - sulava_kohde_.y()), 2)));

    if (etaisyys < 20 || !liikkuuSulavasti_) {
        disconnect(animaatioajastin, &QTimer::timeout, this, &Drooni::tarkistaSulavaLiike);
        lopetaEteenLiikkuminen();
        lopetaTaakseLiikkuminen();
        lopetaKaannosMyota();
        lopetaKaannosVasta();
        return;
    }
    alaLiikkuaEteen();

    // Sitten matikkaa
    QPointF suuntap = suuntaPiste();
    QPointF kohdep = sulava_kohde_;

    // Muunnetaan droonin sijainti origoksi
    kohdep.setX(kohdep.x() - pos().x());
    kohdep.setY(kohdep.y() - pos().y());

    qreal dot = suuntap.x()*kohdep.x() + suuntap.y()*kohdep.y();
    qreal det = suuntap.x()*kohdep.y() - suuntap.y()*kohdep.x();
    qreal kulma_rad = qAtan2(dot, det);

    // Jottei drooni väpätä holtittomasti tarkistetaan mennäänkö about oikeeseen suuntaan
    // Mikäli mennään, ei korjata suuntaa :)

    // Tarkistetaan erikseen tilanteet joissa drooni on jo kääntymässä ja suoraan ajo.
    // Kääntymisessä on hieman tarkempi raja oikealle suunnalle.
    if (kaantyySulavasti_) {
        if (qFabs(kulma_rad - 3.1415926 / 2) < 0.03) {
            lopetaKaannosMyota();
            lopetaKaannosVasta();
            kaantyySulavasti_ = false;
            return;
        }
    }
    else if (qFabs(kulma_rad - 3.1415926 / 2) < 0.07) {
        return;
    }

    // Tarkastetaan kääntymissuunta ja käännytään oikeaan suuntaan
    if (kulma_rad < 3.1415926 / 2 && kulma_rad > -3.1415926 / 2) {
        //qDebug() << "+";
        lopetaKaannosVasta();
        alaKaantyaMyota();
        kaantyySulavasti_ = true;
    }
    else {
        lopetaKaannosMyota();
        alaKaantyaVasta();
        kaantyySulavasti_ = true;
        //qDebug() << "-";
    }
}

void Drooni::ammu()
{
    //scene()->addItem(new Rynnakkokivaari(this, rotation()));
    //scene()->addItem(new Miina(this));
    switch (valittu_ase_) {
    case 0:
        if (rk_ammuksia > 0) {
            scene()->addItem(new Rynnakkokivaari(this, rotation()));
            --rk_ammuksia;
            emit drooniAmpunut(0, rk_ammuksia);
        }
        break;
    case 1:
        if (mm_ammuksia > 0) {
            scene()->addItem(new Miina(this));
            --mm_ammuksia;
            emit drooniAmpunut(1, mm_ammuksia);
        }
        break;
    case 2:
        if (laser_ammuksia > 0) {
            scene()->addItem(new Laser(this, rotation()));
            --laser_ammuksia;
            emit drooniAmpunut(2, laser_ammuksia);
        }
        break;
    default:
        qDebug() << "Asetta ei asetettu oikein!";
        break;
    }
    if (rk_ammuksia < 1 && mm_ammuksia < 1 && laser_ammuksia < 1) {
        emit ammuksetLopussa();
    }
}

void Drooni::valitseRynkky()
{
    valittu_ase_ = 0;
    setFocus();
}

void Drooni::valitseMiina()
{
    valittu_ase_ = 1;
    setFocus();
}

void Drooni::valitseLaser()
{
    valittu_ase_ = 2;
    setFocus();
}

QPointF Drooni::suuntaPiste() const
{
    qreal dx, dy;
    dx = qCos((rotation() - 90) * PI / 180);
    dy = qSin((rotation() - 90) * PI / 180);

    return QPointF(dx, dy);
}

