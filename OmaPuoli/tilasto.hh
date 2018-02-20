#ifndef TILASTO_H
#define TILASTO_H
#include "tilastorp.hh"

#include<memory>
#include<QString>
#include<vector>
#include<map>
#include<string>


class Tilasto: public Rajapinta::TilastoRP
{
public:
    Tilasto();
    ~Tilasto();
    virtual int annaPisteet() const;
    virtual void matkustajiaKuoli(int lkm);
    virtual void lisaaMatkustajia(int lkm);
    virtual void nysseTuhoutui();
    virtual void uusiNysse() ;
    virtual void nyssePoistui();

    //TilastoRP ulkopuoliset metodit
    /**
     * @brief paivita_tilasto palauttaa kaupungille pistetaulua varten tarvittavat luvut.
     * @return vektori, jossa on 5 tilaston private muuttujaa
     * @pre olio rakennettu
     * @post poikkeustakuu: vahva
     */
    std::vector<int> paivita_tilasto() const;

    /**
     * @brief lue_tiedosto lukee tekstitiedoston, johon on tallennettu parhaiden pelaajien tiedot
     * @return vektorin, jossa alkioina on tiedoston rivi tallenettuna QStringiin
     * @pre tiedoston rivit on muotoa " nimi tulos "
     * @post poikkeustakuu: vahva
     * @param tiedoston hakupolku
     */

    std::vector<QString> lue_tiedosto(QString);

    /**
     * @brief kirjoitaTiedostoon lisää pelaajan tuloksen top-listalle, jos on aihetta
     * @param pelaajan tulos ja nimi
     * @pre tiedosto johon kirjoitetaan  on olemassa
     * @post poikkeustakuu: vahva
     */
    void kirjoitaTiedostoon(int, QString) const;

    /**
     * @brief selvitaListasijoitus Kertoo sijoituksen top 10 listalla annetuilla pisteillä.
     * @param uusiTulos Pisteet joidenka sijoitus halutaan.
     * @return Sijoitus top10 listalla, 11 jos ei top10 listalla
     * @pre -
     * @post Poikkeustakuu: vahva
     */
    int selvitaListasijoitus(int uusiTulos) const;



private:
    int pisteet_{0};
    int nysset_lkm_{0};
    int matkustajat_lkm_{0};
    int tuhotut_matkustajat_{0};
    int tuhotut_nysset_{0};

    //tiedostosta luetut tulokset
    std::vector<QString> huipputulokset_;

};


#endif // TILASTO_H
