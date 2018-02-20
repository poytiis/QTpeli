#ifndef MATKUSTAJA_HH
#define MATKUSTAJA_HH

#include "matkustajarp.hh"
#include "kaupunkirp.hh"
#include "sijainti.hh"

#include <memory>


namespace KurssinPuoli
{

class Nysse;

class Matkustaja : public Rajapinta::MatkustajaRP
{
public:
    Matkustaja(std::weak_ptr< Rajapinta::PysakkiRP> maaranpaa);

    Rajapinta::Sijainti annaSijainti() const;
    void liiku(Rajapinta::Sijainti sij);
    void tuhoa();
    bool onkoTuhottu() const;

    // MatkustajaRP interface
    bool onkoKulkuneuvossa() const;
    std::shared_ptr<Rajapinta::KulkuneuvoRP> annaKulkuneuvo() const;
    std::shared_ptr<Rajapinta::PysakkiRP> annaPysakki() const;

    // Kertoo matkustajalle bussin saapumisesta pysäkille ja kysyy,
    // haluaako matkustaja nousta pysäkiltä bussiin
    virtual bool nousetkoNysseen(std::weak_ptr< Nysse > nysse ) const;
    // Siirtää matkustajan pysäkiltä bussiin
    void nouseNysseen(std::weak_ptr< Nysse > nysse );

    // Kertoo matkustajalle bussin saapumisesta pysäkille ja kysyy,
    // haluaako matkustaja nousta bussista ulos pysäkille
    virtual bool menetkoPysakille(std::weak_ptr<Rajapinta::PysakkiRP>) const;
    // siirtää matkustajan bussista pysäkille
    void menePysakille(std::weak_ptr<Rajapinta::PysakkiRP> pysakki);

protected:
    bool tuhottu_;
    std::weak_ptr< Rajapinta::PysakkiRP > maaranpaa_;


private:

    // missä nyssessä, jos nyssessä
    std::weak_ptr< Nysse > nyssep_;
    // millä pysäkillä, jos pysäkillä
    std::weak_ptr< Rajapinta::PysakkiRP > pysakkip_;
    // minne pyrkii


    };

}

#endif // MATKUSTAJA_HH
