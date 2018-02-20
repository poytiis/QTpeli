#include <QString>
#include <string>
#include <QtTest>
#include <vector>
#include <QDebug>
#include<QFile>
#include<sstream>

#include "tilasto.hh"


class YksikkoTesti : public QObject
{
    Q_OBJECT

public:
    YksikkoTesti();
    //laskee tiedoston rivit
    int laskeTiedostonRivit();

    //luo testitiedoston ajohakemistoon
    void luoTestitiedosto();

private Q_SLOTS:
    void testitapaus1();

    //testaa päivittääkö tilasto matkustajista saatuja pisteitä oikein
    void TilastonPaivitysTesti();
    void TilastonPaivitysTesti_data();

    //testaa onko tilastoluokan tuhottujen, poistuneiden ja hengissä olevien nyssejen lkm oikein
    void nyssejenLkmOikein();
    void nyssejenLkmOikein_data();

    //testaa onko tiedosto huipputuloksista luettu ja tallennettu oikein
    void huippepelaajatOikein();


    //testaa että listan ensimmäiseksi lisääminen onnistuu
    void listanKarkeenLisays();

    //testaa tilannetta, jossa liian huonoa tulosta yritetään lisätä listalle
    void lisataanLiianHuonoTulos();

    //testaa listan viimeiseksi lisäämisen sekä että täyteen listaan lisättäessä
    //tulosten lukumäärä pysyy kymmenessä.
    void lisataanListanViimeiseksi();

    //testaa listan keskelle lisäämisen
    void lisataanKeskelle();

};

YksikkoTesti::YksikkoTesti()
{
    luoTestitiedosto();
}

int YksikkoTesti::laskeTiedostonRivit()
{
    QFile Qtiedosto("testitiedosto.txt");
    Qtiedosto.open(QFile::ReadOnly);
    QTextStream rivivirta(&Qtiedosto);
    int tiedostonRivit{0};
    QString rivi;
    rivi=rivivirta.readLine();
    while(rivi!=""){
        tiedostonRivit++;
        rivi=rivivirta.readLine();

    }
    Qtiedosto.close();
    return tiedostonRivit;
}

void YksikkoTesti::luoTestitiedosto()
{
    QFile Qtiedosto("tekstitiedostot");
    Qtiedosto.open(QFile::ReadWrite);
    Qtiedosto.resize(0);
    std::vector<QString> testitulokset{"eka 100","toka 99", "kolmas 88", "neljas 77","viides 55", "kuudes 44"
                                       ,"seiska 33", "kasi 22", "ysi 11","kymppi 2"};
    QTextStream riviVirta (&Qtiedosto);

    for(auto rivit:testitulokset){
        riviVirta<<rivit<<"\n";


    }

    Qtiedosto.close();
}

void YksikkoTesti::testitapaus1()
{
    QVERIFY2(true, "Failure");
}





void YksikkoTesti::TilastonPaivitysTesti()

{
    QFETCH( unsigned int, tuhotutMatkustajat);

    Tilasto tilasto;
    tilasto.matkustajiaKuoli(tuhotutMatkustajat);

    int pisteet=tilasto.annaPisteet();


    QVERIFY2(pisteet==tuhotutMatkustajat, "virheellinen pistemäärä");


}

void YksikkoTesti::TilastonPaivitysTesti_data()
{
   QTest::addColumn< unsigned int>("tuhotutMatkustajat");


   QTest::newRow("ei tuhottuja matkustajia")<<0u;
   QTest::newRow("1 matkustaja")<<1u;
   QTest::newRow("99 matkustajaa")<<99u;
}

void YksikkoTesti::nyssejenLkmOikein()
{


    Tilasto tilasto;

    QFETCH(unsigned int , nyssetLkm);
    QFETCH(unsigned int , poistuneetNysset);
    QFETCH(unsigned int , tuhotutNysset);

    for(unsigned int i{0};i<nyssetLkm;++i){
        tilasto.uusiNysse();
    }
    for(unsigned int i{0}; i<poistuneetNysset;++i){
        tilasto.nyssePoistui();
    }
    for(unsigned int i{0};i<tuhotutNysset;++i){
        tilasto.nysseTuhoutui();
    }
    std::vector<int> tulokset;
    tulokset=tilasto.paivita_tilasto();

    QVERIFY2(tulokset.at(1)==tuhotutNysset,"tuhotut nysset virheellinen");
    QVERIFY2(tulokset.at(2)==nyssetLkm-poistuneetNysset," nyssejen lkm  virheellinen");




}

void YksikkoTesti::nyssejenLkmOikein_data()
{
   QTest::addColumn< unsigned int> ("nyssetLkm" );
   QTest::addColumn<unsigned int> ("poistuneetNysset");
   QTest::addColumn<unsigned int>("tuhotutNysset");



   QTest::newRow("nyssejä ei poistunut pelista")<<10u<<0u<<0u;
   QTest::newRow("kaikki kaupunkiin lisätyt nysset tuhottu")<<12u<<12u<<12u;
   QTest::newRow("kaikki nysset päässeet päätepysäkeille")<<10u<<10u<<0u;
   QTest::newRow("sekalainen määrä tuhottuja ja poistuneita nyssejä")<<40u<<23u<<12u;
}

void YksikkoTesti::huippepelaajatOikein()
{
    Tilasto tilasto;
    std::vector<QString> rivit;
    rivit=tilasto.lue_tiedosto("tekstitiedostot");
    int tulos{-1};
    int rivienMaara=rivit.size();
    std::string nimi{""};
    for(auto rivi:rivit){
        std::string stdRivi=rivi.toStdString();
        std::istringstream riviVirta{stdRivi};
        riviVirta>>nimi>>tulos;
        if(nimi=="" or tulos==-1){
            QFAIL("tiedosto luettu virheellisesti");
        }

    }
    QVERIFY2(rivienMaara==laskeTiedostonRivit(),"Virhe tiedoston rivien määrässä");

}



void YksikkoTesti::listanKarkeenLisays()
{
    Tilasto tilasto;
    std::vector<QString> rivit;
    rivit=tilasto.lue_tiedosto("tekstitiedostot");
    //lisätään tulos kärkeen
    int testitulos=100000;
    QString testaaja{"testaaja"};
    tilasto.kirjoitaTiedostoon(testitulos,testaaja);
    std::vector<QString> karkitulos;
    karkitulos=tilasto.lue_tiedosto("tekstitiedostot");

    //testataan että sijoitukset ovat pudonneet yhdellä
    for(int i{1}; i<9;++i){
        QVERIFY2(karkitulos.at(i)==rivit.at(i-1),"tulokset eivät ole pudonneet sijalla");
    }
    //tarkastetaan että tulos kärjessä
    QString karki=testaaja+" "+QString::number(testitulos);
    QVERIFY2(karki==karkitulos.at(0),"tulos ei siirtynyt karkeen");

    //palautetaan tiedosto
    luoTestitiedosto();


}

void YksikkoTesti::lisataanLiianHuonoTulos()
{
    Tilasto tilasto;
    std::vector<QString> rivit;
    rivit=tilasto.lue_tiedosto("tekstitiedostot");
    //lisätään tulos joka ei mahdu listalle
    int testitulos=1;
    QString testaaja{"testaaja"};
    tilasto.kirjoitaTiedostoon(testitulos,testaaja);
    std::vector<QString> samalista;
    samalista=tilasto.lue_tiedosto("tekstitiedostot");

    QVERIFY2(samalista==rivit,"lisattiin listaan kuulumaton tulos");

    luoTestitiedosto();
}

void YksikkoTesti::lisataanListanViimeiseksi()
{
    Tilasto tilasto;
    std::vector<QString> rivit;
    rivit=tilasto.lue_tiedosto("tekstitiedostot");
    //lisätään tulos joka sjoittuu viimeiseksi listalla.
    int testitulos=3;
    QString testaaja{"testaaja"};
    tilasto.kirjoitaTiedostoon(testitulos,testaaja);

    std::vector<QString> vikasijamuuttunut;
    vikasijamuuttunut=tilasto.lue_tiedosto("tekstitiedostot");

    QVERIFY2(vikasijamuuttunut.back()!=rivit.back(),"vika sija ei ole muuttunut");

    if(vikasijamuuttunut.size()!=10){
        QFAIL("listalla virheellinen määrä tuloksia");
    }

    luoTestitiedosto();
}

void YksikkoTesti::lisataanKeskelle()
{

    Tilasto tilasto;
    std::vector<QString> rivit;
    rivit=tilasto.lue_tiedosto("tekstitiedostot");
    //lisätään tulos joka sjoittuu keskelle listaa.
    int testitulos=12;
    QString testaaja{"testaaja"};
    tilasto.kirjoitaTiedostoon(testitulos,testaaja);
    std::vector<QString> ysisijamuuttunut;
   ysisijamuuttunut=tilasto.lue_tiedosto("tekstitiedostot");

   QVERIFY2(rivit.at(8)==ysisijamuuttunut.at(9),"keskelle lisäys epäonnistunut");
   QVERIFY2(rivit.at(2)==ysisijamuuttunut.at(2),"keskelle lisääminen epäonnistunut");
   if(ysisijamuuttunut.size()!=10){
       QFAIL("keskelle lisääminen epäonnistunut");
   }


}




QTEST_APPLESS_MAIN(YksikkoTesti)

#include "tst_yksikkotesti.moc"
