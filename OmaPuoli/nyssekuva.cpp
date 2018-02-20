#include "nyssekuva.hh"
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QImage>
#include <QPixmap>
#include <QDebug>

Nyssekuva::Nyssekuva(std::shared_ptr<Rajapinta::ToimijaRP> nysse)
{
    QImage kuva;
    kuva.load(":/images/images/Nysse.png");

    if (kuva.isNull()) {
        qDebug() << "Nyssekuvan kuvatiedoston lataus epäonnistui.";
    }
    kuva = kuva.scaledToWidth(40, Qt::SmoothTransformation);
    realNysse_ = new QGraphicsPixmapItem(QPixmap::fromImage(kuva));
    realNysse_->setParentItem(this);

    realNysse_->setOffset(-(kuva.width() / 2), -(kuva.height() / 2));

    nysse_ = nysse;
}

Nyssekuva::~Nyssekuva()
{
    delete realNysse_;
}

void Nyssekuva::osuma()
{
    nysse_->tuhoa();
    kaupunki_->toimijaTuhottu(nysse_);
    //tuhoudu();
}

void Nyssekuva::kerroKaupunki(Rajapinta::KaupunkiRP *kaupunki)
{
    kaupunki_ = kaupunki;
}

void Nyssekuva::tuhoudu()
{
    QPixmap tuhottu(":/images/images/NysseTuhottuRajattu.png");
    tuhottu = tuhottu.scaledToWidth(40, Qt::SmoothTransformation);
    realNysse_->setPixmap(tuhottu);
    setZValue(1);
    tuhottu_ = true;
}

bool Nyssekuva::tuhottu()
{
    return tuhottu_;
}

void Nyssekuva::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    realNysse_->paint(painter, option, widget);
}

QRectF Nyssekuva::boundingRect() const
{
    return realNysse_->boundingRect();
}
