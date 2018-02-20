#ifndef KAUPUNKIRP_HH
#define KAUPUNKIRP_HH

/**
  * @file
  * @brief Määrittelee kaupungin operaatiot kuvaavan rajapintaluokan.
  */

#include "toimijarp.hh"
#include "pysakkirp.hh"
#include <memory>
#include <vector>

#include <QImage>

namespace Rajapinta
{

/**
 * @brief KaupunkiRP on rajapintaluokka, jonka pelin kaupungin tulee toteuttaa.
 *
 * Jos luokan metodi ei ole poikkeustakuultaan nothrow, voi metodista aina vuotaa ulos poikkeus `std::bad_alloc` (muisti loppui).
 */
class KaupunkiRP
{
public:
    /**
      * @brief Rajapintaluokan oletusrakentaja (olemassa dokumentaatiota varten).
      * @post Kaupunki on alustustilassa.
      */
    KaupunkiRP() = default;

    /**
      * @brief Rajapintaluokassa on oletusarvoinen virtuaalipurkaja (olemassa, koska kantaluokalla tulee olla virtuaalipurkaja).
      */
    virtual ~KaupunkiRP() = default;

    /**
     * @brief asetaTausta asettaa pelialueen bittikarttakuvan.
     * @param perustaustakuva Pelialueena käytettävä normaalikokoinen kuva. Kuvan vasemman alakulman sijainti pikselikoordinaateissa selviää sen offset()-metodilla.
     * @param isotaustakuva Normaalia suuremman pelialueen taustakuva. Tarvitaan vain, jos tehdään Vieritettävä kartta -lisäosa. Kuvan vasemman alakulman sijainti pikselikoordinaateissa selviää sen offset()-metodilla.
     * @pre Kaupunki on alustustilassa.
     * @post Pelialueen kuva on asetettu. Poikkeustakuu: perus.
     * @exception AlustusVirhe Kuvan asettaminen epäonnistui tai kuva on epäkelpo.
     */
    virtual void asetaTausta(QImage& perustaustakuva, QImage& isotaustakuva) = 0;

    /**
     * @brief asetaKello asettaa pelikellon ajan.
     * @param kello Pelikellon aika kutsun hetkellä.
     * @pre `kello.isValid() == true`.
     * @post Kellonaika on asetettu. Poikkeustakuu: nothrow.
     */
    virtual void asetaKello(QTime kello) = 0;

    /**
     * @brief lisaaPysakki lisää kaupunkiin pysäkin.
     * @param pysakki Osoitin pysäkkiolioon.
     * @pre Kaupunki on alustustilassa.
     * @post Pysäkki on lisätty kaupunkiin. Poikkeustakuu: perus
     * @exception Alustusvirhe Pysäkin sijainti ei ole kelvollinen
     */
    virtual void lisaaPysakki(std::shared_ptr<PysakkiRP> pysakki) = 0;

    /**
     * @brief peliAlkaa siirtää kaupungin alustustilasta pelitilaan.
     * @pre Kaupunki on alustustilassa. asetaTausta() ja asetaKello() on kutsuttu.
     * @post Kaupunki on pelitilassa. Poikkeustakuu: nothrow.
     */
    virtual void peliAlkaa() = 0; // Siirry alustustilasta pelitilaan

    /**
     * @brief lisaaToimija lisää kaupunkiin uuden toimijan.
     * @param uusitoimija Kaupunkiin lisättävä ToimijaRP:n toteuttava olio.
     * @pre -
     * @post Toimija on lisätty kaupunkiin. Poikkeustakuu: perus.
     * @exception PeliVirhe Toimija on jo kaupungissa.
     */
    virtual void lisaaToimija(std::shared_ptr<ToimijaRP> uusitoimija) = 0;

    /**
     * @brief poistaToimija poistaa toimijan kaupungista.
     * @param toimija Poistettava toimija.
     * @pre Kaupunki on pelitilassa.
     * @post Toimija on poistettu kaupungista. Poikkeustakuu: vahva.
     * @exception PeliVirhe Toimijaa ei löydy kaupungista.
     */
    virtual void poistaToimija(std::shared_ptr<ToimijaRP> toimija) = 0;

    /**
     * @brief toimijaTuhottu kertoo kaupungille, että toimija on tuhottu pelissä.
     * @param toimija Toimija, joka merkitään pelissä tuhotuksi.
     * @pre Kaupunki on pelitilassa. Annettu toimija löytyy kaupungista. Toimijalla `toimija.onkoTuhottu() == true`.
     * @post Poikkeustakuu: vahva.
     */
    virtual void toimijaTuhottu(std::shared_ptr<ToimijaRP> toimija) = 0;

    /**
     * @brief loytyykoToimija tarkastaa, onko annettu toimija kaupungissa.
     * @param toimija Toimija, jota haetaan kaupungista.
     * @pre -
     * @return Totuusarvo, joka kertoo löytyykö toimija kaupungista.
     * @post Poikkeustakuu: nothrow.
     */
    virtual bool loytyykoToimija(std::shared_ptr<ToimijaRP> toimija) const = 0;

    /**
     * @brief toimijaLiikkunut on operaatio, jolla kaupungille voi kertoa, että tietty toimija on liikkunut.
     * @param toimija Toimija, joka on liikkunut.
     * @pre Kaupunki on pelitilassa. Annettu toimija löytyy kaupungista.
     * @post Poikkeustakuu: perus.
     */
    virtual void toimijaLiikkunut(std::shared_ptr<ToimijaRP> toimija) = 0;

    /**
     * @brief annaToimijatLahella palauttaa ne toimijat, jotka ovat lähellä annettua sijaintia.
     * @param paikka Sijainti, jota lähellä olevat toimijat listataan.
     * @pre Kaupunki on pelitilassa.
     * @return Vektori paikkaa lähellä olevista toimijoista, joilla kaikille `annaSijainti().onkoLahella(paikka) == true`.
     * @post Poikkeustakuu: vahva.
     */
    virtual std::vector<std::shared_ptr<ToimijaRP>> annaToimijatLahella(Sijainti paikka) const = 0;

    /**
     * @brief peliLoppunut kertoo, onko peli loppunut vai ei.
     * @pre Kaupunki on pelitilassa.
     * @return `true`, jos peli on loppunut, muuten `false`.
     * @post Poikkeustakuu: nothrow.
     */
    virtual bool peliLoppunut() const = 0;
};

}

#endif // KAUPUNKIRP_HH
