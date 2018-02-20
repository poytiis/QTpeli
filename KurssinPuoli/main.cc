#include "logiikka.hh"
#include <QApplication>
#include "konffiikkuna.hh"
#include "luopeli.hh"
#include "kaupunkirp.hh"
#include "alykaupunkirp.hh"
#include <QDebug>
#include <vector>
#include <string>
#include <QTime>

int main(int argc, char *argv[])
{
    std::vector<std::string> args(argv, argv+argc);

    QApplication a(argc, argv);
    KurssinPuoli::Logiikka logiikka;

    // Tarkistetaan komentoriviparametrit.
    // Sallitut parametrit:

    QTime alku;
    bool debug = a.arguments().contains("--debug");
    // 1) Aika muodossa hh:mm. Normaali ajo ilman konffi-ikkunaa.
    if (a.arguments().size() != 1){
        alku = QTime::fromString(a.arguments().at(1), "hh:mm");
        if (alku.isValid()){
            logiikka.asetaAika(alku.hour(), alku.minute());
            //TODO: Muut asetukset?
        } else {
            qDebug() << "VIRHEELLINEN AIKAPARAMETRI. Lopetetaan. Anna aika muodossa hh:mm";
            return EXIT_FAILURE;
        }

        logiikka.konffiMuuttunut(alku, debug);
    }

    // 2) Ei parametreja: Näytetään konffi-ikkuna.
    else if (!logiikka.naytaKonffi()){
        qDebug() << "Käyttäjä lopetti pelin.";
        return EXIT_SUCCESS;
    }

    // aloita kutsutaan joko oletuskonffin tai naytakonffin sisältä suoraan
    //logiikka.aloita();

    std::shared_ptr<Rajapinta::KaupunkiRP> UusiTampere = Rajapinta::luoPeli();


    // luodaan pelikartat

    // karttojen mittakaava on sama, 500:1500 pix/kkj3-koord
    // origo on pikkukartan vasemmassa alakulmassa

    QImage pkartta(":/offlinedata/offlinedata/kartta_pieni_500x500.png");
    pkartta.setOffset(QPoint(0,0));
    QImage ikartta(":/offlinedata/offlinedata/kartta_iso_1095x592.png");
    ikartta.setOffset(QPoint(-352,-36));

    UusiTampere->asetaTausta(pkartta, ikartta);

    // onnistuttiinko kaupungin luomisessa
    // TODO: debug-tulostukset pois kaikkialta koodista
    if (logiikka.otaKaupunki(UusiTampere)) {
        qDebug() << "Tampere tallennettu!";
    }


    // tehdään lisätään tiedostoista bussit, pysäkit ja matkustajat
    // Tampereelle ja kutsutaan Tampereen alustusrutiineja, joista
    // viimeinen aloittaa pelin
    logiikka.viimeistelePelinAloitus();

    return a.exec();
}
