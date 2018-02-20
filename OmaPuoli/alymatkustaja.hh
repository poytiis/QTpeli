#ifndef ALYMATKUSTAJA_H
#define ALYMATKUSTAJA_H

#include "matkustaja.hh"
#include "sijainti.hh"
#include <memory>

class AlyMatkustaja: public KurssinPuoli::Matkustaja
{
public:
    AlyMatkustaja();
    ~AlyMatkustaja();

//    // ToimijaRP interface..
//    Rajapinta::Sijainti annaSijainti();
//    void liiku(Rajapinta::Sijainti sij);
//    void tuhoa();
//    bool onkoTuhottu();

    virtual bool nousetkoNysseen(std::weak_ptr<Nysse> nysse) const;
    virtual bool menetkoPysakille(std::weak_ptr<Rajapinta::PysakkiRP>) const;
};

#endif // ALYMATKUSTAJA_H
