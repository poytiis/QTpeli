#include "tilasto.hh"
#include<fstream>
#include<sstream>
#include<QDebug>
#include<string>
#include<QFile>
#include<QTextStream>
#include<QString>
#include<map>
#include<sstream>


Tilasto::Tilasto()

{

}

Tilasto::~Tilasto()
{

}

int Tilasto::annaPisteet() const
{
    return pisteet_;
}

void Tilasto::matkustajiaKuoli(int lkm)
{
    matkustajat_lkm_-=lkm;
    tuhotut_matkustajat_+=lkm;
    pisteet_+=lkm;

}

void Tilasto::lisaaMatkustajia(int lkm)
{
    matkustajat_lkm_+=lkm;

}
void Tilasto::nysseTuhoutui()
{
    tuhotut_nysset_++;
    pisteet_+=50;
}

void Tilasto::uusiNysse()
{
    nysset_lkm_++;
}

void Tilasto::nyssePoistui()
{
    nysset_lkm_--;
}


std::vector<int> Tilasto::paivita_tilasto() const
{
    std::vector<int> pisteet;
    pisteet.push_back(pisteet_);
    pisteet.push_back(tuhotut_nysset_);
    pisteet.push_back(nysset_lkm_);
    pisteet.push_back(tuhotut_matkustajat_);
    pisteet.push_back(matkustajat_lkm_);

    return pisteet;

}





std::vector<QString> Tilasto::lue_tiedosto(QString tiedosto)
{
    std::vector<QString> tulokset;
    QString tulos;

    QFile Qtiedosto(tiedosto);
    Qtiedosto.open(QFile::ReadOnly);
    QTextStream teksti(&Qtiedosto);
    tulos=teksti.readLine();
    //QString k=teksti.readLine();
    while(tulos!=""){



        huipputulokset_.push_back(tulos);
        tulokset.push_back(tulos);

        tulos=teksti.readLine();

    }
        Qtiedosto.close();

        //palautetaan tulokset kaupungille, joka päivittää peli-ikkunaa.
        return tulokset;

}

void Tilasto::kirjoitaTiedostoon(int tulos, QString nimi ) const

{

    QString polku="tekstitiedostot";
    QFile Qtiedosto(polku);

    Qtiedosto.open(QFile::ReadWrite);



     QTextStream tiedostovirta(&Qtiedosto);

    short int tulosten_lkm;
    int rivi_tulos;
    std::string rivi_nimi;
    std::string stdnimi;
    bool tulosLisatty{false};

    //tiedosto tyhjä lisätään ensimmäiseksi
    if(huipputulokset_.size()==0){

        tiedostovirta<<nimi<<" "<<tulos;
        Qtiedosto.close();
        return;



    }else{

            tulosten_lkm=huipputulokset_.size();
            qDebug()<<tulosten_lkm;
    }




    int kierroslaskuri{0};

   //käydään kaikki tulokset läpi ja lisätään uusitulos oikeaan kohtaan
    for(auto tulokset: huipputulokset_){
        kierroslaskuri++;

        //puretaan QString intiksi ja stringiksi
        stdnimi=tulokset.toStdString();
        std::istringstream rivivirta (stdnimi);
        rivivirta>>rivi_nimi>>rivi_tulos;

        if(kierroslaskuri==10 and tulosLisatty){
            break;
        }



        //tulos on pienempi kuin kyseisellä sijalla oleva
        else if(rivi_tulos>tulos){
            qDebug()<<"tulos ei pärjännyt"<<tulokset;
            tiedostovirta<<tulokset<<"\n";

            //lisätään tulos listalle viimeiseksi jos listalla ei ole 10 tulosta
            if(kierroslaskuri==tulosten_lkm and !tulosLisatty and kierroslaskuri!=10) {
                        qDebug()<<"lisätään vikaksi";
                        tiedostovirta<<nimi<<" "<<tulos;
            }

            continue;
        //tulos yltää listalle
        }else if(rivi_tulos<=tulos and !tulosLisatty){
            qDebug()<<"lisätään uusi tulos";

           tiedostovirta<<nimi<<" "<<tulos<<"\n";
           tiedostovirta<<tulokset<<"\n";
           tulosLisatty=true;




        }
    }



    Qtiedosto.close();




}

int Tilasto::selvitaListasijoitus(int uusiTulos) const
{
    short int sijoitus{1};
    int rivi_tulos;
    std::string stdnimi;
    std::string rivi_nimi;
    for(auto tulos:huipputulokset_){

        stdnimi=tulos.toStdString();
        std::istringstream rivivirta (stdnimi);
        rivivirta>>rivi_nimi>>rivi_tulos;
        if(uusiTulos<rivi_tulos){
            sijoitus++;
        }else{
            qDebug()<<sijoitus<<"sijoitus";
            return sijoitus;
        }

    }
    qDebug()<<sijoitus<<"sijoitus";
    return sijoitus;
}


