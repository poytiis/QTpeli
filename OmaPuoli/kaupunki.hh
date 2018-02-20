#ifndef KAUPUNKI_H
#define KAUPUNKI_H

#include "kaupunkirp.hh"
#include "pysakkirp.hh"
#include "toimijarp.hh"
#include "peliikkuna.hh"
#include "drooni.hh"
#include "tilasto.hh"
#include "nyssekuva.hh"
#include <memory>
#include <vector>
#include <unordered_map>
#include <QImage>
#include <QTime>
#include <QGraphicsObject>
#include <QObject>

/**
 * @file
 * @brief Kaupunki luokka on KaupunkiRP:stä periytetty. Kaupunki hallinnoi pelin
 * tapahtumia.
 */

class Kaupunki : public QObject, public Rajapinta::KaupunkiRP
{
    Q_OBJECT
public:
    Kaupunki();
    virtual ~Kaupunki();

    // KaupunkiRP:n mukaiset metodit
    virtual void asetaTausta(QImage& perustaustakuva, QImage& isotaustakuva);
    virtual void asetaKello(QTime kello);
    virtual void lisaaPysakki(std::shared_ptr<Rajapinta::PysakkiRP> pysakki);
    virtual void peliAlkaa(); // Siirry alustustilasta pelitilaan
    virtual void lisaaToimija(std::shared_ptr<Rajapinta::ToimijaRP> uusitoimija);
    virtual void poistaToimija(std::shared_ptr<Rajapinta::ToimijaRP> toimija);
    virtual void toimijaTuhottu(std::shared_ptr<Rajapinta::ToimijaRP> toimija);
    virtual bool loytyykoToimija(std::shared_ptr<Rajapinta::ToimijaRP> toimija) const;
    virtual void toimijaLiikkunut(std::shared_ptr<Rajapinta::ToimijaRP> toimija);
    virtual std::vector<std::shared_ptr<Rajapinta::ToimijaRP>> annaToimijatLahella(Rajapinta::Sijainti paikka) const;
    virtual bool peliLoppunut() const;

    // Omaan puoleen liittyvät funktiot
public slots:
    /**
     * @brief aloitaLopetus Käynnistää ajastimen joka X ajan kuluttua päättää pelin.
     * Kutsutaan kun droonin ammukset loppuvat.
     * @pre Peli on käynnissä
     * @post Peli on ajastettu loppumaan
     */
    void ajastaLopetus();

private:
    // Ajastetun lopetuksen QTimer kutsuu tätä kerran. Tarkistaa onko lopettamisen
    // edellytykset yhä voimassa ja tarvittaessa lopettaa pelin.
    void lopetaPeli();
    // Pelin loppuessa kutsutaan avaaLopetusDialogi(), joka esittää lopetus ruudun
    // ja sen jälkeen sulkee ohjelman.
    void avaaLopetusDialogi();

    std::unordered_map<std::shared_ptr<Rajapinta::PysakkiRP>, QGraphicsPixmapItem*> pysakit_;
    std::unordered_map<std::shared_ptr<Rajapinta::ToimijaRP>, Nyssekuva*> nysset_;
    std::vector<std::shared_ptr<Rajapinta::MatkustajaRP> > matkustajat_;

    // Peli-ikkuna
    std::shared_ptr<PeliIkkuna> peliruutu;
    std::shared_ptr<Tilasto> tilastoikkuna_;

    unsigned int ajastin_aika_{5000};
    bool ajastettu_lopetus_{false};
    bool peli_loppunut_{false};

    Drooni* drooni;
    QTime peliaika_{QTime(0,0,0,0)};
    QTime aika_;

};

#endif // KAUPUNKI_H
