#ifndef LOGIIKKA_HH
#define LOGIIKKA_HH

#include "matkustaja.hh"
#include <list>
#include "nysse.hh"
#include "offlinelukija.hh"
#include <QTime>
#include <QTimer>
#include "kaupunkirp.hh"

namespace KurssinPuoli
{

class Logiikka : public QObject
{
    Q_OBJECT
public:
    Logiikka(QObject *parent = 0);

    bool lueOfflineData(const QString& bussit, const QString& pysakit);

    void viimeistelePelinAloitus();

    bool naytaKonffi();

    void tiedostoKonffi();

    void asetaAika(unsigned short hr, unsigned short min);

    bool otaKaupunki(std::shared_ptr<Rajapinta::KaupunkiRP> kaupunki);

    void annaReititAlykaupungille() const;

    void annaVuorotAlyKaupungille() const;

public slots:
    void etene();
    void konffiMuuttunut(QTime aika, bool debug);
    void kasvataAikaa();
    void lisaaUusiaBusseja();
    void lisaaUusiaMatkustajia();

private:
    // peliajan ja todellisen ajan kulun suhde
    static const int AJAN_NOPEUS;
    // aika päivitysten välillä millisekunneissa
    static const int PAIVITYSVALI_MS;

    std::shared_ptr<Rajapinta::KaupunkiRP> kaupunkirp_;
    std::list< std::shared_ptr<Matkustaja> > matkustajat_;
    std::list< std::shared_ptr<Nysse> > bussit_;
    std::vector< std::shared_ptr<Pysakki> > pysakit_;
    std::shared_ptr<OfflineData> offlinedata_;
    QString bussitiedosto_;
    QString pysakkitiedosto_;
    bool debugtila_;
    bool pelialkanut_;

    // Nykyinen kellonaika
    QTime aika_;

    // Ajastin, joka kasvattaa aikaa aika_ sekunnin välein
    QTimer ajastin_;

    // Ajastin, joka liikuttaa busseja tasaisin väliajoin
    QTimer animaatioajastin_;

    // Ajastin, joka tarkistaa lähtevät bussit minuutin välein
    QTimer lahtoajastin_;

    // Palauttaa true, jos uusi sijainti saatiin laskettua
    // False, jos bussi saapui päätepysäkille tai sen ei pitäisi olla liikenteessä
    bool laskeBussinUusiSijainti(std::shared_ptr<Nysse> bussi);

    void lisaaBussit();
    void lisaaPysakitJaMatkustajat();

    // Laskee pysäkkiajat lähtöajan ja pysäkille kuluvan ajan perusteella
    std::map<QTime, std::weak_ptr<Pysakki>> laskePysakkiAjat(
                                                std::map<QTime, std::shared_ptr<Pysakki>>& pysakit,
            QTime& lahtoaika);
    void luoBussi(std::shared_ptr<BussiData> bussi, QTime lahtoaika);

    int bussiSID_;
};

}

#endif // LOGIIKKA_HH
