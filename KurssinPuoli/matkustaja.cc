#include "matkustaja.hh"
#include "nysse.hh"
#include "pelivirhe.hh"

namespace KurssinPuoli
{

Matkustaja::Matkustaja(std::weak_ptr< Rajapinta::PysakkiRP > maaranpaa) :
    tuhottu_(false),
    maaranpaa_(maaranpaa)
{
}


Rajapinta::Sijainti Matkustaja::annaSijainti() const
{
    // palauttaa nyssensä sijainnin tai oman sijaintinsa
    if (onkoKulkuneuvossa() ) {

        return nyssep_.lock()->annaSijainti();
       } else {

        return pysakkip_.lock()->annaSijainti();
    }
}

void Matkustaja::liiku(Rajapinta::Sijainti sij)
{
    if (!(sij == annaSijainti())) {
        throw Rajapinta::PeliVirhe("Matkustajaa ei voi liikuttaa pois pysäkiltä tai nyssestä!");
    }
}

void Matkustaja::tuhoa()
{
    tuhottu_ = true;
}

bool Matkustaja::onkoTuhottu() const
{
    return tuhottu_;
}

bool Matkustaja::onkoKulkuneuvossa() const
{
    if (nyssep_.expired()) {
        return false;
    }
    return true;
}

std::shared_ptr<Rajapinta::KulkuneuvoRP> Matkustaja::annaKulkuneuvo() const
{
    return nyssep_.lock();
}

std::shared_ptr<Rajapinta::PysakkiRP> Matkustaja::annaPysakki() const
{
    return pysakkip_.lock();
}



bool Matkustaja::nousetkoNysseen(std::weak_ptr< Nysse > /*bussi*/) const
{
    // nousee bussiin 50 % todennäköisyydellä
    if (rand() % 2 == 0) {
        return false;
    }

    return true;
}

void Matkustaja::nouseNysseen(std::weak_ptr< Nysse > bussi)
{
    // asetetaan nysspointteri osoittamaan annettuun nysseen
    // ja unohdetaan pysakkipointteri kokonaan
    nyssep_ = bussi;
    pysakkip_.reset();
}

bool Matkustaja::menetkoPysakille(std::weak_ptr<Rajapinta::PysakkiRP> /*pysakki*/) const
{

    // jää pysäkille
    if (rand() % 2 == 0) {
        return false;
    }

    return true;
}

void Matkustaja::menePysakille(std::weak_ptr< Rajapinta::PysakkiRP > pysakki)
{
    // menee pysäkille bussista
    nyssep_.reset();
    pysakkip_ = pysakki;
}

}
