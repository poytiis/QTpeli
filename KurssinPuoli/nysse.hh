#ifndef NYSSE_HH
#define NYSSE_HH

#include "kulkuneuvorp.hh"
#include "kaupunkirp.hh"
#include "matkustajarp.hh"
#include "sijainti.hh"
#include "pysakki.hh"

#include <QTime>
#include <memory>
#include <set>
#include <map>
#include <vector>

namespace KurssinPuoli
{

class Nysse : public Rajapinta::KulkuneuvoRP
{
public:
    Nysse(unsigned int linja);

    // ToimijaRP interface
    std::string annaNimi() const;

    Rajapinta::Sijainti annaSijainti() const;
    void liiku(Rajapinta::Sijainti sij);
    void tuhoa();
    bool onkoTuhottu() const;

    //void tulostaSijainti(int reittipiste);

    // KulkuneuvoRP
    std::vector<std::shared_ptr<Rajapinta::MatkustajaRP>> annaMatkustajat() const;
    void lisaaMatkustaja(std::shared_ptr<Rajapinta::MatkustajaRP> matkustaja);
    void poistaMatkustaja(std::shared_ptr<Rajapinta::MatkustajaRP> matkustaja);

    std::weak_ptr<Pysakki> annaPaatepysakki() const;


    std::map<QTime, std::pair<Rajapinta::Sijainti, std::weak_ptr<Pysakki> > > aikareitti() const;

    void asetaReitti(const std::map<QTime, std::pair<Rajapinta::Sijainti, std::shared_ptr<Pysakki> > > &aikareitti,
                     QTime &lahtoaika);

    unsigned int annaLinja();
    std::weak_ptr<Pysakki> annaPysakki();

    Rajapinta::Sijainti liikutaSeuraavaanSijaintiin(QTime aika);

    // Kutsutaan ennen pelin käynnistystä, laskee seuraavan pysäkin
    void laskeAloitusSijainti(QTime aika);

    void asetaKaupunki(std::shared_ptr<Rajapinta::KaupunkiRP> omistajaKaupunki);



    int annaSID() const;
    void asetaSID(int sid);
private:
    unsigned int linja_;
    std::string nimi_;
    std::shared_ptr<Rajapinta::KaupunkiRP> kaupunkip_;
    Rajapinta::Sijainti sijainti_;
    std::vector<std::shared_ptr<Rajapinta::MatkustajaRP>> matkustajat_;
    std::map< QTime, std::pair< Rajapinta::Sijainti, std::weak_ptr<Pysakki> > > aikareitti2_;
    std::map< QTime, std::pair< Rajapinta::Sijainti, std::weak_ptr<Pysakki> > >::iterator ar2Iterator_;


    std::weak_ptr<Pysakki> pysakki_;
    bool tuhottu_;
    int SID_;
};

}

#endif // NYSSE_HH
