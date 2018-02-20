#include "nysse.hh"
#include <cassert>
#include <map>
#include <QDebug>

namespace KurssinPuoli
{

Nysse::Nysse(unsigned int linja) :
    linja_(linja),
    nimi_(QString::number(linja).toStdString()),
    pysakki_(),
    tuhottu_(false)

{
}

int Nysse::annaSID() const {
    return SID_;
}

void Nysse::asetaSID(int sid) {
    SID_ = sid;
}

std::string Nysse::annaNimi() const
{
    return nimi_;
}

std::vector<std::shared_ptr<Rajapinta::MatkustajaRP> > Nysse::annaMatkustajat() const
{
    return matkustajat_;
}

Rajapinta::Sijainti Nysse::annaSijainti() const
{
    return sijainti_;
}

void Nysse::liiku(Rajapinta::Sijainti sij)
{
    sijainti_ = sij;
}

void Nysse::tuhoa()
{
    for (std::shared_ptr<Rajapinta::MatkustajaRP> matkustaja: matkustajat_) {
        matkustaja->tuhoa();
    }
    tuhottu_ = true;
}

bool Nysse::onkoTuhottu() const
{
    return tuhottu_;
}

std::map<QTime, std::pair<Rajapinta::Sijainti, std::weak_ptr<Pysakki> > > Nysse::aikareitti() const
{
    return aikareitti2_;
}


void Nysse::lisaaMatkustaja(std::shared_ptr<Rajapinta::MatkustajaRP> matkustaja)
{
    matkustajat_.push_back(matkustaja);
}

void Nysse::poistaMatkustaja(std::shared_ptr<Rajapinta::MatkustajaRP> matkustaja)
{
    for (auto it = matkustajat_.begin(); it != matkustajat_.end(); it++) {
        if (it->get() == matkustaja.get()) {
            matkustajat_.erase(it);
            break;
        }
    }
}

std::weak_ptr<Pysakki> Nysse::annaPaatepysakki() const
{
    return aikareitti2_.rbegin()->second.second;
}


unsigned int Nysse::annaLinja()
{
    return linja_;
}

std::weak_ptr<Pysakki> Nysse::annaPysakki()
{
    return pysakki_;
}

Rajapinta::Sijainti Nysse::liikutaSeuraavaanSijaintiin(QTime aika)
{
    // tässä vain liikutetaan bussia seuraavaan sijaintiin
    // tätä rutiinia ei ikinä kutsuta, jos seuraavaa sijaintia ei ole
    // tai bussi on tuhottu tai on tapahtunut aikakiepsahdus

    // liikutetaan iteraattoria pykälä eteenpäin bussin kokonaisreitillä
    //++ar2Iterator_;
    // asetetaan sijainti uuteen pykälään
    // TODO: POISTA: ei täällä?    sijainti_ = ar2Iterator_->second->first;

    std::map< QTime, std::pair< Rajapinta::Sijainti, std::weak_ptr<Pysakki> > >::iterator seuraava = ar2Iterator_;
    ++seuraava;
    if (seuraava != aikareitti2_.end())
    {
        if (seuraava->first == aika)
        {
            // saavutettu seuraava reittipiste
            ar2Iterator_ = seuraava;

            // asetetaan pysäkiksi (on joko nullptr tai weakptr pysäkkiin)
            pysakki_ = ar2Iterator_->second.second;

            // palautetaan tämä uusi sijainti
            return ar2Iterator_->second.first;
        }
        else
        {
            // osuus matkasta kohti suraavaa reittipistettä
            double etaisyys = ar2Iterator_->first.secsTo(aika) / (double)ar2Iterator_->first.secsTo(seuraava->first);
            return Rajapinta::Sijainti::eteneSuoraa(ar2Iterator_->second.first, seuraava->second.first, etaisyys);
        }
    }
    else
    {
        // TODO: puolenyön ylitys
        // tässä vaiheessa kaadetaan ohjelma, jos tänne päädytään
        assert(false);
    }
}

void Nysse::laskeAloitusSijainti(QTime aika)
{
    qDebug() << aika;

    for (ar2Iterator_ = aikareitti2_.begin(); ar2Iterator_ != aikareitti2_.end(); ar2Iterator_++) {
        if (aika == ar2Iterator_->first) {
            // löydetty se piste, jonka ajanhetki on sama kuin pelin sisäinen aika
            // juuri nyt

            // tässä pisteessä ei välttämättä ole bussipysäkkiä
            sijainti_ = ar2Iterator_->second.first;

            // tarkastellaan bussipysäkin olemassaolo tälle ajanhetkelle
            // voi olla, että nyt ollaan esim. pisteessä 10.06 ja pysäkit ovat
            // 10.05 ja 10.07

            pysakki_ = ar2Iterator_->second.second;

            return;

        }
    }

    // TODO: assertit
    qDebug() << "TÄHÄN EI PITÄISI IKINÄ JOUTUA!!!!!!!";
}

void Nysse::asetaReitti(const std::map<QTime, std::pair<Rajapinta::Sijainti, std::shared_ptr<Pysakki> > > &aikareitti,
                        QTime &lahtoaika)
{

    // lasketaan bussin jokaiselle reittipisteelle aika alkuperäisestä
    // suhteellisesta ajasta
    // datassa esim. pysäkki nro 3 ajanhetkellä 6 min
    // --> selvitetään, mihin kellonaikaan pysäkillä ollaan pelin sisäisessä
    // ajassa --> esim. pysäkki nro 3 saavutetaan ajanhetkellä 10:06
    // sisältää myös reittipisteet, ei pelkkiä pysäkkejä
    std::map<QTime, std::pair<Rajapinta::Sijainti, std::weak_ptr<Pysakki> > > aikareittiajat;
    QTime aika = lahtoaika;

    for (std::pair<QTime, std::pair< Rajapinta::Sijainti, std::shared_ptr<Pysakki> > > aikareittipiste : aikareitti) {
        QTime ua;
        ua = aika.addSecs( ( aikareittipiste.first.hour() *60*60) + ( aikareittipiste.first.minute() * 60) + aikareittipiste.first.second() );
        aikareittiajat.insert(std::make_pair(ua, std::make_pair(aikareittipiste.second.first, aikareittipiste.second.second) ));
    }

    // aikareitti sisältää reitin kaikki pisteet 15 s välein
    aikareitti2_ = aikareittiajat;
    // alustukset
    ar2Iterator_ = aikareitti2_.begin();
    sijainti_ = ar2Iterator_->second.first;
    Q_ASSERT(annaPaatepysakki().lock() != nullptr);
}

void Nysse::asetaKaupunki(std::shared_ptr<Rajapinta::KaupunkiRP> omistajaKaupunki)
{
   // qDebug() << "Nysselle kaupunki";
    kaupunkip_ = omistajaKaupunki;
   // qDebug() << "ok";
}

}
