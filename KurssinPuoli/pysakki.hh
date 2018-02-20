#ifndef PYSAKKI_HH
#define PYSAKKI_HH

#include "sijainti.hh"
#include "matkustajarp.hh"
#include "matkustaja.hh"
#include "pysakkirp.hh"

#include <QString>
#include <set>

namespace KurssinPuoli
{

class Pysakki : public Rajapinta::PysakkiRP
{
public:
    Pysakki(const Rajapinta::Sijainti& annaSijainti, const QString& nimi, unsigned int id);
    ~Pysakki();

    //PysakkiRP
    Rajapinta::Sijainti annaSijainti() const;
    QString annaNimi() const;
    unsigned int annaId() const;
    std::vector<std::shared_ptr<Rajapinta::MatkustajaRP>> annaMatkustajat() const;

    void asetaSijainti(const Rajapinta::Sijainti &sijainti);
    void asetaNimi(const QString &nimi);
    void asetaId(unsigned int id);
    void lisaaMatkustaja(const std::weak_ptr<Rajapinta::MatkustajaRP> matkustaja);
    void poistaMatkustaja(const std::weak_ptr<Rajapinta::MatkustajaRP> matkustaja);

private:
    Rajapinta::Sijainti sijainti_;
    QString nimi_;
    unsigned int id_;
    std::vector<std::shared_ptr<Rajapinta::MatkustajaRP>> matkustajat_;
};

}

#endif // PYSAKKI_HH
