#include "sijainti.hh"
#include <math.h>
#include <QDebug>

// @todo : Pitäisikö oletusrakentajan alustaa sijainti jotenkin "kelvottomaksi"?
// @todo : laskeEtaisyys, pitäisikö todella laskea karttakoordinaatteina? Tarvitaanko? Hämääkö, kun on static?

namespace Rajapinta
{

Sijainti::Sijainti() : pohjkoord_(6700000), itakoord_(3500000)
{

}

Sijainti::Sijainti(int pohjoiskoord, int itakoord) : Sijainti()
{

    // LATITUDE = LEVEYSPIIRI = Y-AXIS
    // LONGITUDE = PITUUSPIIRI = X-AXIS
    asetaPohjIta(pohjoiskoord, itakoord);
}

int Sijainti::annaX() const
{
    return x_;
}

int Sijainti::annaY() const
{
    return y_;
}

void Sijainti::asetaXY(int x, int y)
{
    x_ = x;
    y_ = y;
    itakoord_ = ItaXsta(x);
    pohjkoord_ = PohjYsta(y);
}

double Sijainti::annaPohjoisKoord() const
{
    return pohjkoord_;
}

double Sijainti::annaItaKoord() const
{
    return itakoord_;
}

void Sijainti::asetaPohjIta(int pohjkoord, int itakoord)
{
    pohjkoord_ = pohjkoord;
    itakoord_ = itakoord;
    x_ = xIdasta(itakoord);
    y_ = yPohjoisesta(pohjkoord);

    // TODO: siivoa debugit
    //    qDebug() << x_;
    //    qDebug() << itakoord_;
    //    qDebug() << y_;
    //    qDebug() << pohjkoord_;

}

bool Sijainti::onkoLahella(const Sijainti &sij) const
{
    int raja = 10;
    int dx = sij.annaX() - annaX();
    int dy = sij.annaY() - annaY();

    return dx*dx + dy*dy <= raja*raja;
}

int Sijainti::xIdasta(int itakoord)
{
    // 3327034 on kkj3-yhteiskoordinaatiston piste
    // pienen kartan vasemmassa alakulmassa
    // skaalaus on 500:1500
    return static_cast<int>((itakoord - 3327034) * 0.333333);
}

int Sijainti::yPohjoisesta(int pohjkoord)
{
    // 6824642 on kkj3-yhteiskoordinaatiston piste
    // pienen kartan vasemmassa alakulmassa
    // skaalaus on 500:1500
    return static_cast<int>((pohjkoord - 6824642) * 0.333333);
}


int Sijainti::ItaXsta(int x)
{
    return static_cast<int>(x/0.333333 + 3327034);
}

int Sijainti::PohjYsta(int y)
{
    return static_cast<int>(y/0.333333 + 6824642);
}

double Sijainti::laskeEtaisyys(Sijainti a, Sijainti b)
{


    // Approksimaatio, koska etäisyydet hyvin lyhyitä
    double etaisyys = sqrt(pow(a.annaItaKoord() - b.annaItaKoord(), 2) + pow(a.annaPohjoisKoord() - b.annaPohjoisKoord(), 2));
    return etaisyys;
}

Sijainti Sijainti::eteneSuoraa(Sijainti a, Sijainti b, double etaisyys)
{
    // Muodostetaan vektori
    double levdiff = b.annaPohjoisKoord() - a.annaPohjoisKoord();
    double pitdiff = b.annaItaKoord() - a.annaItaKoord();

    // Uusi sijainti löydetään kulkemalla pisteestä a vektoria pitkin haluttu osuus sen pituudesta
    return Sijainti(a.annaPohjoisKoord() + levdiff*etaisyys, a.annaItaKoord() + pitdiff*etaisyys);
}

void Sijainti::tulostaMolemmat() {
    qDebug() << pohjkoord_ << " " << itakoord_;
    qDebug() << x_ << " " << y_;
}

bool Sijainti::operator==(const Sijainti &sijainti)
{
    // vertailuoper. ok
    // tarvitaan, jotta voidaan sanoa, onko bussipysäkki ja sijainti yhtä suuret
    return (this->x_==sijainti.annaX() && this->y_ == sijainti.annaY());
}

}
