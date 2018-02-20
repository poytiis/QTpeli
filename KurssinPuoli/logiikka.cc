#include "logiikka.hh"
#include <QDebug>
#include "offlinelukija.hh"
#include "konffiikkuna.hh"
#include "alykaupunkirp.hh"
#include <QTimer>
#include <memory>

#include <iostream>

namespace KurssinPuoli
{

Logiikka::Logiikka(QObject *parent)
    : QObject(parent),
      debugtila_(false),
      pelialkanut_(false),
      aika_(QTime::currentTime().hour(), QTime::currentTime().minute(), QTime::currentTime().second()),
      bussiSID_(0)
{
}

bool Logiikka::lueOfflineData(const QString &bussit, const QString &pysakit)
{
    OfflineLukija offlinelukija;
    if((offlinedata_ = offlinelukija.lueTiedostot(bussit, pysakit)) == NULL) {
        return false;
    }
    return true;

}

void Logiikka::viimeistelePelinAloitus()
{

    // lisätään kaikki nysseihin liittyvät tiedot älykaupungille,
    // jos kyseessä on älykaupunki

    // funktiot eivät tee mitään, jos kyseessä ei ole älykaupunki
    annaReititAlykaupungille();
    annaVuorotAlyKaupungille();

    // Lisää tällä hetkellä liikkeellä olevat bussit
    lisaaBussit();

    lisaaPysakitJaMatkustajat();

    kaupunkirp_->asetaKello(aika_);
    kaupunkirp_->peliAlkaa();
    pelialkanut_ = true;

    connect(&ajastin_, SIGNAL(timeout()), this, SLOT(kasvataAikaa()));
    ajastin_.start(PAIVITYSVALI_MS);

}

bool Logiikka::naytaKonffi()
{
    KonffiIkkuna k;
    connect(&k, SIGNAL(konffiMuuttunut(QTime,bool)),
            this, SLOT(konffiMuuttunut(QTime,bool)) );
    return k.exec() == QDialog::Accepted;
}

void Logiikka::asetaAika(unsigned short hr, unsigned short min)
{
    aika_.setHMS(hr, min, 0);
}

void Logiikka::etene()
{
    // kerrotaan tasaminuutein uusi kellonaika kaupungille
    if (aika_.second() == 0) {
        kaupunkirp_->asetaKello(aika_);
    }

    // Käy läpi nykyiset matkustajat ja poistaa tietorakenteista kaikki ne,
    // jotka on tuhottu pelin puolelta
    // matkustaja on tuhottu vain, jos se on ammuttu
    for (std::list<std::shared_ptr<Matkustaja>>::iterator it = matkustajat_.begin();
         it != matkustajat_.end(); it++) {

        // Tarkista onko ammuttu
        if (it->get()->onkoTuhottu()) {

            // Poista matkustaja bussin tai pysäkin kirjanpidosta
            if (it->get()->onkoKulkuneuvossa()) {
                it->get()->annaKulkuneuvo()->poistaMatkustaja(*it);
            } else {
                std::shared_ptr<Pysakki> pysakki = std::dynamic_pointer_cast<Pysakki>(it->get()->annaPysakki());
                pysakki->poistaMatkustaja(*it);
            }

            // Poista matkustaja kirjapidosta
            if (kaupunkirp_->loytyykoToimija(*it)) {
                kaupunkirp_->poistaToimija(*it);
            }

            it = matkustajat_.erase(it);
            continue;
        }

    }



    // Käy läpi nykyiset bussit ja tuhoaa ne, jotka on tuhottu
    for (std::list<std::shared_ptr<Nysse>>::iterator it = bussit_.begin();
         it != bussit_.end();) {

        std::shared_ptr<Nysse> bussi = *it;

        // Tarkista onko ammuttu
        if (bussi->onkoTuhottu()) {





            // Jos bussi on merkitty tuhotuksi, käydään kaikki sen matkustajat läpi ja varmistetaan,
            // että nekin on merkitty tuhotuiksi
            // jos ei ole merkitty --> merkitään ja poistetaan sekä bussi että matkustajat tietorakenteista

            for (std::shared_ptr<Rajapinta::MatkustajaRP> matkustajarp : bussi->annaMatkustajat()) {
                std::shared_ptr<Matkustaja> matkustaja = std::dynamic_pointer_cast<Matkustaja> (matkustajarp);

                if (!matkustaja->onkoTuhottu()) {
                    matkustaja->tuhoa();
                }

                // jos kaupunki pitää edelleen kirjaa tuhotun bussin matkustajista
                // poistetaan matkustajat kaupungilta
                if (kaupunkirp_->loytyykoToimija(matkustaja)) {
                    kaupunkirp_->poistaToimija(matkustaja);
                }

                // poistetaan matkustaja kurssipuolen tietorakenteesta
                matkustajat_.remove(matkustaja);
            }



            qDebug() << "Poistetaan nysse: " << QString::fromStdString(bussi->annaNimi());

            std::shared_ptr<Rajapinta::ToimijaRP> toimijaBussi = std::dynamic_pointer_cast<Rajapinta::ToimijaRP> (*it);

            if (kaupunkirp_->loytyykoToimija(toimijaBussi)) {
                kaupunkirp_->poistaToimija(toimijaBussi);
            }

            it = bussit_.erase(it);
            continue;
        }

        // Liikuta bussia
        bool ret = laskeBussinUusiSijainti(bussi);

        if (!ret) { // Poista bussi, jos päätepysäkki tai väärä aika


            // pakotetaan matkustajat bussin päättärille
            std::vector<std::shared_ptr<Rajapinta::MatkustajaRP>> matkustajat = bussi->annaMatkustajat();

            std::weak_ptr<Pysakki> paatepysakki = bussi->annaPaatepysakki();


            // jokainen bussin matkustaja helberiin bussista ennen bussin poistoa
            for (auto matkustajaIt = matkustajat.begin(); matkustajaIt != matkustajat.end(); matkustajaIt++) {
                // oletetaan että nämäkin matkustajat ovat tietyn tyypin olioita, kuten muuallakin
                std::shared_ptr<Matkustaja> matkustaja = std::dynamic_pointer_cast<Matkustaja>(*matkustajaIt);
                Q_ASSERT(matkustaja != nullptr);

                // päivittää matkustajan pakollisena bussin päättärille
                // matkustajan sijainti on muutettu jo edellisellä ajanhetkellä,
                // kun bussi saapui tälle päätepysäkille
                matkustaja->menePysakille(paatepysakki);
                bussi->poistaMatkustaja(matkustaja);
                paatepysakki.lock()->lisaaMatkustaja(matkustaja);
                kaupunkirp_->toimijaLiikkunut(matkustaja);
            }

     //       qDebug() << "Poistetaan nysseV: " << QString::fromStdString(bussi->annaNimi()) << " SID: " << bussi->annaSID();


            // tarkistetaan kaupungin näkökulma siitä, onko bussi jo poistettu
            if (kaupunkirp_->loytyykoToimija(*it)) {
                kaupunkirp_->poistaToimija(*it);
            }

            it = bussit_.erase(it);

        } else {

            ++it;
        }
    }

    // ############################################################################################################

    // nyt on:
    // 1. poistettu tuhotut matkustajat ja bussit
    // 2. poistettu hämärään tilaan joutuneet bussit
    // 3. liikutettu bussit niiden uusille sijainneille
    // 4. pakotettu matkustajat päättäreille päätyneistä busseista pysäkeille


    // lisätään uudet bussit
    if (aika_.second() == 0) {
        lisaaUusiaBusseja();
    }


    // käydään läpi ne pysäkit, joilla on busseja
    // --> eli käydään läpi bussit ja pyydetään niiden pysäkit, koska on enemmän pysäkkejä kuin busseja

    for (std::shared_ptr <Nysse> bussi : bussit_) {

        std::shared_ptr <Pysakki> pysakki = bussi->annaPysakki().lock();

        if (pysakki != nullptr) {

            // 1. matkustajat pysäkille, jos haluavat
            for (std::shared_ptr <Rajapinta::MatkustajaRP> matkustajarp : bussi->annaMatkustajat()) {

                std::shared_ptr <Matkustaja> matkustaja = std::dynamic_pointer_cast<Matkustaja> (matkustajarp);
                Q_ASSERT(matkustaja != nullptr);

                if (matkustaja->menetkoPysakille(pysakki)) {
                    matkustaja->menePysakille(pysakki);
                    bussi->poistaMatkustaja(matkustajarp);
                    pysakki->lisaaMatkustaja(matkustaja);
                    kaupunkirp_->toimijaLiikkunut(matkustaja);
                }

            }
        }
    }


    // 2. kerrotaan jokaiselle tällä pysäkillä olevalle matkustajalle
    //    kaikista niistä busseista, jotka ovat tällä hetkellä tällä pysäkillä

    for (std::shared_ptr <Nysse> pysakinbussi : bussit_) {

        std::shared_ptr <Pysakki> pysakki = pysakinbussi->annaPysakki().lock();

        // pysakinbussi on vain sellainen bussi, joka on tällä samalla pysäkillä
        if (pysakki != nullptr) {

            for (std::shared_ptr <Rajapinta::MatkustajaRP> pysakinmatkustajarp : pysakki->annaMatkustajat()) {

                std::shared_ptr <Matkustaja> pysakinmatkustaja = std::dynamic_pointer_cast<Matkustaja> (pysakinmatkustajarp);
                Q_ASSERT(pysakinmatkustaja != nullptr);

                if (pysakinmatkustaja->nousetkoNysseen(pysakinbussi)) {
                    pysakinmatkustaja->nouseNysseen(pysakinbussi);
                    pysakki->poistaMatkustaja(pysakinmatkustaja);
                    pysakinbussi->lisaaMatkustaja(pysakinmatkustaja);
                    kaupunkirp_->toimijaLiikkunut(pysakinmatkustaja);
                }

            }
        }

    }

}



void Logiikka::tiedostoKonffi() {
    qDebug () << "tiedostoKonffi";

    // TODO: oikeat tiedostot tänne jahka valmiita

    QString oletusPysakkiTiedosto = ":/offlinedata/offlinedata/full_stations_kkj3.json";
    QString liteBussiTiedosto = ":/offlinedata/offlinedata/final_bus_liteN.json";

    bool ret = lueOfflineData(liteBussiTiedosto, oletusPysakkiTiedosto);

    if (ret) {
        bussitiedosto_ = liteBussiTiedosto;
        pysakkitiedosto_ = oletusPysakkiTiedosto;
    } else {
        qDebug () << "Tiedostojen lukemisessa ilmeni jonkinlainen virhe.";
        // TODO: virheenkäsittely
    }
}

void Logiikka::konffiMuuttunut(QTime aika, bool debug)
{
    // käsitellään komentoriviparametrit
    this->debugtila_ = debug;
    this->aika_ = aika;

    // tiedostojen lukeminen jne.
    this->tiedostoKonffi();
}

void Logiikka::kasvataAikaa()
{
    if ( kaupunkirp_->peliLoppunut() )
    {
        ajastin_.stop();
        return;
    }

    aika_ = aika_.addMSecs(AJAN_NOPEUS * PAIVITYSVALI_MS);
    if (aika_.second() == 0) {
        qDebug() << "";
        qDebug() << "aika on: " << aika_.toString();
    }

    // liikuttaa kaikki vanhat bussit
    etene();


}

bool Logiikka::laskeBussinUusiSijainti(std::shared_ptr<Nysse> bussi)
{

    // Tarkistetaan onko bussi liikenteessä
    // Huolehtii myös päätepysäkeillä olevien bussien poistot
    // --> jos bussi pääsi päätepysäkille edellisen etene-rutiinin aikana,
    //     on pelin sisäinen aika nyt suurempi kuin bussin viimeisen reittipisteen
    //     aika --> return false ja bussi poistetaan
    if (aika_ < bussi->aikareitti().begin()->first || aika_ > bussi->aikareitti().rbegin()->first) {
        // Ei liikenteessä
        //        qDebug() << "Bussi numero " << bussi->annaLinja() << " ei liikenteessä";
        return false;
    }

    // Haetaan uusi sijainti
    Rajapinta::Sijainti uusi = bussi->liikutaSeuraavaanSijaintiin(aika_);

    //qDebug() << uusi.annaX() << "," << uusi.annaY();

    bussi->liiku(uusi);
    kaupunkirp_->toimijaLiikkunut(bussi);

    // ainoa paikka, jossa matkustajia liikutetaan
    // TODO ANTTI:  eikö tämä riitä? kun aika liikkuu eteenpäin, seuraavalla kierroksella bussin
    //              ja matkustajien sijainti on jo päättäri --> kukaan ei liiku, mutta bussi poistetaan
    std::vector<std::shared_ptr<Rajapinta::MatkustajaRP>> matkustajat = bussi->annaMatkustajat();
    for (auto it = matkustajat.begin(); it != matkustajat.end(); it++) {
        it->get()->liiku(uusi);
        kaupunkirp_->toimijaLiikkunut(*it);
    }

    return true;
}

void Logiikka::lisaaBussit()
{
    qDebug() << "Nykyinen aika: " << aika_.toString();
    // Etsi offlinedatasta sellaiset bussit, jotka ovat liikenteessä
    for (std::shared_ptr<BussiData> bussi: offlinedata_->bussit) {
        // Lähtöaika < nykyinen aika
        // Lähtöaika + viimeinen pysäkki > nykyinen aika

        for (QTime lahtoaika : bussi->aikataulu) {

            // viimeisen reittipisteen (joka on myös pysäkki samaan aikaan) suhteellinen aika ensimmäisestä reittipisteestä
            // lisättynä pelin aikaan juuri tällä hetkellä
            // --> jos lähtö pienempi ja tämä saatu aika suurempi kuin aika nyt, bussi on ajamassa
            QTime paatepysakkiaika = lahtoaika.addSecs( ((--bussi->aikareitti2.end())->first.hour() *60*60) + (--bussi->aikareitti2.end())->first.minute() * 60 );
            if (lahtoaika < aika_ &&  paatepysakkiaika > aika_ ) {

                luoBussi(bussi, lahtoaika);

                // jos debugtila päällä, niin lisätään vain yksi ainoa
                if (debugtila_) {
                    qDebug() << "Debuggi päällä --> vain yksi onnikka";
                    return;
                }

            }
        }

    }

    qDebug() << "Liikenteessä olevien nyssejen määrä: " << bussit_.size();
}

void Logiikka::lisaaUusiaBusseja()
{
    // qDebug() << aika_;
    if (debugtila_ && bussit_.size() != 0) {
        return;
    }


    for (std::shared_ptr<BussiData> bussi: offlinedata_->bussit) {
        for (QTime lahtoaika : bussi->aikataulu) {
            //  qDebug() << "Lahtoaika: " << lahtoaika.toString();
            //  qDebug() << bussi->pysakit.rbegin()->first;
            //  qDebug() << "Päätösaika: " << lahtoaika.addSecs(bussi->pysakit.rbegin()->first * 60).toString();
            //  qDebug() << lahtoaika;

            if (lahtoaika == aika_) {

                luoBussi(bussi, lahtoaika);

                // jos debugtila päällä, niin lisätään vain yksi ainoa
                if (debugtila_) {
                    return;
                }

                break;
            }

        }
    }



    qDebug() << "Liikenteessä olevien bussien määrä: " << bussit_.size();
}


void Logiikka::luoBussi(std::shared_ptr<BussiData> bussi, QTime lahtoaika)
{

    QTime aika = (--(bussi->aikareitti2.end()))->first;
    int minuutit = aika.minute() + aika.hour() * 60;

    if (aika_ == lahtoaika.addSecs( minuutit * 60 )) {
        // palataan, jos bussi on lisäyksen aikana päätepysäkillä
        return;
    }


    // Luodaan uusi bussi ja lisätään se kaupunkiin

    std::shared_ptr<Nysse> uusibussi = std::make_shared<Nysse>(bussi->linja);

    // TODO: mieti, onko tarvetta
    // lasketaan bussin pysakeille ajat ja asetetaan ne bussille
    // uusibussi->asetaPysakit(laskePysakkiAjat(bussi->pysakit, lahtoaika));

    // lasketaan bussin reitille ajat ja asetetaan ne bussille
    uusibussi->asetaReitti(bussi->aikareitti2, lahtoaika);

    uusibussi->laskeAloitusSijainti(aika_);
    bussit_.push_back(uusibussi);
    uusibussi->asetaKaupunki(kaupunkirp_);


    uusibussi->asetaSID(bussiSID_);

    bussiSID_ += 1;
    kaupunkirp_->lisaaToimija(uusibussi);

    // Bussi liikenteessä
    qDebug() << "Bussi lisätty! Linjanro: " << bussi->linja; // <<  " SID: " << uusibussi->annaSID();
}

std::map<QTime, std::weak_ptr<Pysakki> > Logiikka::laskePysakkiAjat(std::map<QTime, std::shared_ptr<Pysakki> > &pysakit, QTime &lahtoaika)
{
    // käydään kaikki pysäkit läpi ja lasketaan niille minuuttiaikataulut
    // palautetaan bussille weak_pointterit pysäkkeihin aikamäpissä
    std::map<QTime, std::weak_ptr<Pysakki>> pysakkiajat;
    QTime aika = lahtoaika;
    for (std::pair<QTime, std::shared_ptr<Pysakki>> pysakki : pysakit) {
        pysakkiajat.insert(std::make_pair(aika.addSecs(pysakki.first.minute() * 60), pysakki.second));
    }

    return pysakkiajat;
}

void Logiikka::lisaaPysakitJaMatkustajat()
{
    // annetaan kaupungille ensin kaikki pysäkit ennen kuin pysäkeille
    // (mahdollisesti) luodaan matkustajia
    for ( std::shared_ptr<Pysakki> pysakki: offlinedata_->pysakit) {
        kaupunkirp_->lisaaPysakki(pysakki);
    }

    for ( std::shared_ptr<Pysakki> pysakki: offlinedata_->pysakit) {


        // lisätään matkustajia sopivasti ripotellen
        // luodaan siis näin monta uutta matkustajaa
        // ja sijoitetaan ne tämän for-silmukan pysäkille
        int randi = rand() % 10 + 1;

        for (int i = 0; i < randi; i++) {

            std::weak_ptr<Rajapinta::PysakkiRP> maaranpaaPysakki = offlinedata_->pysakit.at( rand() % offlinedata_->pysakit.size() );

            std::shared_ptr<Matkustaja> uusiMatkustaja = nullptr;


            std::shared_ptr <Rajapinta::AlyKaupunkiRP> alykp = std::dynamic_pointer_cast<Rajapinta::AlyKaupunkiRP> (kaupunkirp_);

            if (alykp) {
                std::shared_ptr<Rajapinta::MatkustajaRP> uusiMatkustajaRP = alykp->luoAlyMatkustaja( maaranpaaPysakki );
                uusiMatkustaja = std::dynamic_pointer_cast<Matkustaja> (uusiMatkustajaRP);
            } else {
                uusiMatkustaja = std::make_shared<Matkustaja>( maaranpaaPysakki );
            }

            // lisätään matkustaja tälle pysäkille
            uusiMatkustaja->menePysakille( pysakki );
            matkustajat_.push_back(uusiMatkustaja);
            pysakki->lisaaMatkustaja(uusiMatkustaja);

            kaupunkirp_->lisaaToimija(uusiMatkustaja);
        }


    }

}

void Logiikka::lisaaUusiaMatkustajia()
{

    // TODO: tämä pitää miettiä; lisätäänkö ylipäätään

    //    std::shared_ptr<Pysakki> kohdePysakki = pysakit_.at(2);

    //    for (int i = 0; i < rand() % 7 + 2; i++) {
    //        std::shared_ptr<Matkustaja> uusiMatkustaja = std::make_shared<Matkustaja>();
    //        matkustajat_.push_back(uusiMatkustaja);
    //        kohdePysakki->lisaaMatkustaja(uusiMatkustaja);
    //    }

    //    qDebug() << " ";
    //    qDebug() << "Lisätty matkustajia pysäkille: " << kohdePysakki->annaNimi();
    //    qDebug() << "Matkustajia liikenteessä nyt: " << matkustajat_.size();

}

bool Logiikka::otaKaupunki(std::shared_ptr<Rajapinta::KaupunkiRP> kaupunki) {

    kaupunkirp_ = kaupunki;
    return true;
}

void Logiikka::annaVuorotAlyKaupungille() const
{

    // tarkistetaan, onko pelissä käytössä älykaupunki
    std::shared_ptr <Rajapinta::AlyKaupunkiRP> alykp = std::dynamic_pointer_cast<Rajapinta::AlyKaupunkiRP> (kaupunkirp_);

    if (!alykp) {
        return;
    }

    // käydään kaikki bussit läpi ja annetaan kaikki niiden
    // lähtöajat älykaupungille vuoroina

    // vuoro on siis esim. bussilinjan 25 "Pyynikintori - Jokupaikka" klo 07.20 lähtevä ajo
    for (std::shared_ptr<BussiData> bussi: offlinedata_->bussit) {

        for (QTime lahtoaika : bussi->aikataulu) {
            alykp->lisaaVuoro( bussi->reittiId, lahtoaika );
        }
    }

}

void Logiikka::annaReititAlykaupungille() const
{

    // tarkistetaan, onko pelissä käytössä älykaupunki
    std::shared_ptr <Rajapinta::AlyKaupunkiRP> alykp = std::dynamic_pointer_cast<Rajapinta::AlyKaupunkiRP> (kaupunkirp_);

    if (!alykp) {
        return;
    }

    for (std::shared_ptr<BussiData> bussi: offlinedata_->bussit) {

        // luodaan vectori, joka on muotoa:
        // 0, 1. pysäkki
        // 2, 2. pysäkki
        // 4, 3. pysäkki
        // jne.


        std::vector< std::pair< int, int > > reittiAlyKaupungille;

        for ( std::pair< QTime, std::pair< Rajapinta::Sijainti, std::shared_ptr<Pysakki> > > aikareittipiste : bussi->aikareitti2 ) {

            if (aikareittipiste.second.second == nullptr) {
                continue;
            }

            reittiAlyKaupungille.push_back( std::make_pair(aikareittipiste.first.hour() * 60 + aikareittipiste.first.minute(), aikareittipiste.second.second->annaId()) );
        }

        alykp->lisaaReitti(bussi->reittiId, bussi->reittiNimi, reittiAlyKaupungille);

    }
}



// peliajan ja todellisen ajan kulun suhde
const int Logiikka::AJAN_NOPEUS = 10;
// aika päivitysten välillä millisekunneissa
const int Logiikka::PAIVITYSVALI_MS = 100;

}
