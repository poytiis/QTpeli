#include "pysakki.hh"

namespace KurssinPuoli
{

Pysakki::Pysakki(const Rajapinta::Sijainti& sijainti, const QString& nimi, unsigned int id) :
    sijainti_(sijainti),
    nimi_(nimi),
    id_(id)
{

}

Pysakki::~Pysakki()
{

}

Rajapinta::Sijainti Pysakki::annaSijainti() const
{
    return sijainti_;
}

QString Pysakki::annaNimi() const
{
    return nimi_;
}

unsigned int Pysakki::annaId() const
{
    return id_;
}

std::vector<std::shared_ptr<Rajapinta::MatkustajaRP> > Pysakki::annaMatkustajat() const
{
    return matkustajat_;
}

void Pysakki::asetaSijainti(const Rajapinta::Sijainti &sijainti)
{
    sijainti_ = sijainti;
}

void Pysakki::asetaNimi(const QString &nimi)
{
    nimi_ = nimi;
}

void Pysakki::asetaId(unsigned int id)
{
    id_ = id;
}

void Pysakki::lisaaMatkustaja(const std::weak_ptr<Rajapinta::MatkustajaRP> matkustaja)
{
    matkustajat_.push_back(matkustaja.lock());

}

void Pysakki::poistaMatkustaja(const std::weak_ptr<Rajapinta::MatkustajaRP> matkustaja)
{
    for (auto it = matkustajat_.begin(); it!= matkustajat_.end(); ++it){
        if (*it == matkustaja.lock()){
            matkustajat_.erase(it);
            break;
        }
    }
}

}
