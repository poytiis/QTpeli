#include "kaupunki.hh"
#include "pysakkirp.hh"
#include "drooni.hh"
#include "nyssekuva.hh"
#include"tilasto.hh"
#include "pelivirhe.hh"
#include "lopetusdialog.hh"
#include<QTime>
#include<iostream>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <memory>
#include<QDebug>
#include<QString>
#include<QDateTime>
#include<QImage>
#include<QtGui>
#include <algorithm>

Kaupunki::Kaupunki() :
    peliruutu(new PeliIkkuna())
{
    Q_INIT_RESOURCE(images);
    tilastoikkuna_ = std::unique_ptr<Tilasto> (new Tilasto());
    drooni = new Drooni;

    connect(drooni, &Drooni::ammuksetLopussa, this, &Kaupunki::ajastaLopetus);

    drooni->setZValue(10);   // Piirretään drooni päällimmäisenä
    // Siirretään drooni about pelialueelle
    drooni->setPos(30, 30);
    //drooni->setFocus();
    peliruutu->lisaaDrooni(drooni);

}

Kaupunki::~Kaupunki()
{
    delete drooni;
}

void Kaupunki::asetaTausta(QImage &perustaustakuva, QImage &isotaustakuva)
{
    peliruutu->asetaKartta(isotaustakuva);
}

void Kaupunki::asetaKello(QTime kello)

{

    qDebug() <<peliaika_.toString() << " Ennen";
    //aika_=kello;
    peliaika_.setHMS(kello.hour(), kello.minute(), kello.second());
    qDebug() <<peliaika_.toString()<<"asasasasas";

    // Jos halutaan seurata peliin käytettyä aikaa peliajassa, niin tässä jossain kello käyntiin
    //aika_.start();
}

void Kaupunki::lisaaPysakki(std::shared_ptr<Rajapinta::PysakkiRP> pysakki)
{
    QGraphicsPixmapItem* pysakkikuvaptr{peliruutu->lisaa_pysakki(pysakki)};

    if (pysakkikuvaptr != nullptr) {
        pysakit_[pysakki] = pysakkikuvaptr;
        tilastoikkuna_->lisaaMatkustajia(pysakki->annaMatkustajat().size());
    }
    else {
        qDebug() << "Kuvassa, on ongelma, jokin virhe ohjelmoinnissa";
    }

}

void Kaupunki::peliAlkaa()
{
    peliruutu->show();
    std::vector<QString> tulokset;
    tulokset=tilastoikkuna_->lue_tiedosto("tekstitiedostot");
    peliruutu->naytaToplista(tulokset);
    aika_.start();
}

void Kaupunki::lisaaToimija(std::shared_ptr<Rajapinta::ToimijaRP> uusitoimija)
{
    // Kokeillaan dynamic_pointer_castin avulla setviä onko kyseessä nysse
    // vai matkustaja.
    std::shared_ptr<Rajapinta::KulkuneuvoRP> nysseptr =
            std::dynamic_pointer_cast<Rajapinta::KulkuneuvoRP>(uusitoimija);


    if (nysseptr) {
        if (nysset_.find(nysseptr) != nysset_.end()) {
            throw Rajapinta::PeliVirhe("Toimija on jo kaupungissa.");
        }
        Nyssekuva* nyssekuvaptr{peliruutu->lisaa_nysse(uusitoimija)};
        tilastoikkuna_->lisaaMatkustajia(nysseptr->annaMatkustajat().size());

        // Kannattaisiko lisaa_nysse:n vaih heittää mikäli homma ei skulaa?
        if (nyssekuvaptr != nullptr) {
            nyssekuvaptr->kerroKaupunki(this);
            nysset_[uusitoimija] = nyssekuvaptr;
            tilastoikkuna_->uusiNysse();
        }
        else {
            qDebug() << "Kuvassa, on ongelma, jokin virhe ohjelmoinnissa";
        }
    }
    else {
        // Kyseessä pakko olla matkustaja
        std::shared_ptr<Rajapinta::MatkustajaRP> matkustajaptr =
                std::dynamic_pointer_cast<Rajapinta::MatkustajaRP>(uusitoimija);

        if (std::find(matkustajat_.begin(), matkustajat_.end(), matkustajaptr) != matkustajat_.end()) {
            throw Rajapinta::PeliVirhe("Toimija on jo kaupungissa.");
        }

        matkustajat_.push_back(matkustajaptr);
        tilastoikkuna_->lisaaMatkustajia(1);

    }
}

void Kaupunki::poistaToimija(std::shared_ptr<Rajapinta::ToimijaRP> toimija)
{
    std::shared_ptr<Rajapinta::KulkuneuvoRP> nysseptr =
            std::dynamic_pointer_cast<Rajapinta::KulkuneuvoRP>(toimija);

    if(nysseptr){
        qDebug() << "Poistetaan nysseä kaupungista!";
        if (nysset_.find(nysseptr) == nysset_.end()) {
            throw Rajapinta::PeliVirhe("Toimijaa ei löydy kaupungista.");
        }

        // Huom! Tässä poistetaan toimijat mapista ToimijaRp pointteri, Nyssekuva pointteri
        // pari. Nyssekuvaa ei kuitenkaan tuhota, vaan se jää QGraphicsSceneen. QGraphicsScene
        // tuhoaa Nyssekuvan oikein omassa purkajassaan. :)

        // BUGFIX'd: Nyssekuva tuhotaan, mikäli bussi poistuu eikä nyssekuva ole tuhoutuneen nyssen
        // kuva
        if (!nysset_[toimija]->tuhottu()) {
            delete nysset_[toimija];
        }

        nysset_.erase(nysseptr);
        tilastoikkuna_->nyssePoistui();
    }
    else{
        //qDebug() << "Poistetaan matkustajaa kaupungista!";
        std::shared_ptr<Rajapinta::MatkustajaRP> matkustajaptr =
                std::dynamic_pointer_cast<Rajapinta::MatkustajaRP>(toimija);

        if (std::find(matkustajat_.begin(), matkustajat_.end(), matkustajaptr) == matkustajat_.end()) {
            throw Rajapinta::PeliVirhe("Toimijaa ei löydy kaupungista.");
        }

        matkustajat_.erase(std::find(matkustajat_.begin(), matkustajat_.end(), matkustajaptr));
        tilastoikkuna_->matkustajiaKuoli(1);
    }
}

void Kaupunki::toimijaTuhottu(std::shared_ptr<Rajapinta::ToimijaRP> toimija)
{
    // Tätä kutsutaan aina kun nysse tuhoutuu; Pitänee myös järkkäillä kutsu aina
    // kun matkustaja tuhoutuu.


    // matkustaja tuhoutuu meidän versiossa vain ollessaan tuhoutuvassa bussissa.
    // Helpoin tapa hoitaa varmaan rekursion avulla matkustajien poisto.Tuhotaan eka bussin
    // matkustajat, sen jälkeen itse nysse.
    std::shared_ptr<Rajapinta::KulkuneuvoRP> nysseptr =
         std::dynamic_pointer_cast<Rajapinta::KulkuneuvoRP>(toimija);

    // Nysse tuhoutui
    if(nysseptr){
         // tilasto paivitettys
         tilastoikkuna_->nysseTuhoutui();

         // tuhotaan nyssen matkustajat
         // Eipäs tuhotakkaan, jätetään se KurssinPuolen logiikalle. Aiheutti ikävän
         // satunnaisen kräshin linux ympäristössä :(

         // tuhotaan nysse(kuva)
         // Yleensä nyssekuva on tuhottu jo osuman yhteydessä, mutta varmistetaan
         // asia tuhoamalla se uudelleen. Voihan olla, että esim. logiikka ottaa vallan(?)
         // ja tuhoaa kaiken.
         std::unordered_map<std::shared_ptr<Rajapinta::ToimijaRP>, Nyssekuva*>::iterator nysseIterator=nysset_.find(toimija);
         if (nysseIterator != nysset_.end()) {
            nysseIterator->second->tuhoudu();
         }
    }
    else {
        // Matkustaja tuhoutui
        // Asiasta kerrotaan tilastolle, kun logiikka poistaa matkustajan kaupungista.
        toimija->tuhoa();
    }
}

bool Kaupunki::loytyykoToimija(std::shared_ptr<Rajapinta::ToimijaRP> toimija) const
{
    std::shared_ptr<Rajapinta::KulkuneuvoRP> nysseptr =
            std::dynamic_pointer_cast<Rajapinta::KulkuneuvoRP>(toimija);

    if(nysseptr){
        if(nysset_.find(toimija)!=nysset_.end()){
            return true;
        }
        else{
            return false;
        }
    }
    else{
        std::shared_ptr<Rajapinta::MatkustajaRP> matkustajaptr =
                std::dynamic_pointer_cast<Rajapinta::MatkustajaRP>(toimija);

        if(std::find(matkustajat_.begin(),matkustajat_.end(),matkustajaptr)
                !=matkustajat_.end()){
            return true;
        }
        else{
            return false;
        }
    }
}

void Kaupunki::toimijaLiikkunut(std::shared_ptr<Rajapinta::ToimijaRP> toimija)
{
    std::vector<std::shared_ptr<Rajapinta::MatkustajaRP> >::iterator matkustaja{std::find(matkustajat_.begin(), matkustajat_.end(), toimija)};
    std::unordered_map<std::shared_ptr<Rajapinta::ToimijaRP>, Nyssekuva*>::iterator nysse{nysset_.find(toimija)};

    if (nysse == nysset_.end() && matkustaja == matkustajat_.end()) {
        throw Rajapinta::PeliVirhe("Toimijaa ei löydy kaupungista.");
    }
    else if (nysse == nysset_.end()) {
        // Matkustaja liikkunut
    }
    else {
        nysse->second->setPos(toimija->annaSijainti().annaX(),
                                      (520 - toimija->annaSijainti().annaY()));
    }
    peliruutu->PaivitaTulokset(tilastoikkuna_->paivita_tilasto());
}

std::vector<std::shared_ptr<Rajapinta::ToimijaRP> > Kaupunki::annaToimijatLahella(Rajapinta::Sijainti paikka) const
{
    // Me ei käytetä tässä missään, mut toteutetaan ny ko rajapinta vaatii ;)
    std::vector<std::shared_ptr<Rajapinta::ToimijaRP> > tulos;
    for (auto it = nysset_.begin(); it != nysset_.end(); ++it) {
        if (it->first->annaSijainti().onkoLahella(paikka)) {
            tulos.push_back(it->first);
        }
    }
    return tulos;
}

bool Kaupunki::peliLoppunut() const
{
    if (peli_loppunut_) {
        qDebug() << "Peli logiikka päätetty ajassa: " << aika_.elapsed();
        peliruutu->asetaStatus("Peli päättyi.");
        QTimer::singleShot(200, this, &Kaupunki::avaaLopetusDialogi);
    }
    return peli_loppunut_;
}

void Kaupunki::ajastaLopetus()
{
    if(!ajastettu_lopetus_) {
        QTimer::singleShot(ajastin_aika_, this, &Kaupunki::lopetaPeli);
        ajastettu_lopetus_ = true;
        peliruutu->asetaStatus("Ammukset lopussa. Peli päättyy 5s kuluttua!");
    }
}

void Kaupunki::lopetaPeli()
{
    if (ajastettu_lopetus_ && !peli_loppunut_) {
        if (drooni->ammuksiaJaljella(0) > 0 ||
                drooni->ammuksiaJaljella(1) > 0 ||
                drooni->ammuksiaJaljella(2) > 0) {
            ajastettu_lopetus_ = false;
            peliruutu->asetaStatus("Ammuksia ladattu, peli jatkuu!");
            return;
        }

        peli_loppunut_ = true;
        qDebug() << "Peli asetettu päättyneeksi ajassa: " << aika_.elapsed();
    }
}

void Kaupunki::avaaLopetusDialogi()
{
    LopetusDialog* dialog = new LopetusDialog();
    dialog->annaTilasto(tilastoikkuna_);
    dialog->exec();
    peliruutu->close();
}

