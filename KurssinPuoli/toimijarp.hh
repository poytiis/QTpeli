#ifndef TOIMIJARP_HH
#define TOIMIJARP_HH

/**
  * @file
  * @brief Määrittelee yhden toimijan (= pelissä oleva otus) operaatiot kuvaavan rajapintaluokan.
  */

#include "sijainti.hh"

namespace Rajapinta
{

/**
 * @brief ToimijaRP on rajapintaluokka, jonka jokainen pelissä liikkuva toimija toteuttaa.
 *
 * Jos luokan metodi ei ole poikkeustakuultaan nothrow, voi metodista aina vuotaa ulos poikkeus `std::bad_alloc` (muisti loppui).
 */
/**
 * @brief ToimijaID on toimijalle tyypillinen
 */
class ToimijaRP
{
public:

    /**
      * @brief Rajapintaluokan oletusrakentaja (olemassa dokumentaatiota varten).
      * @post Toimija ei alussa ole tuhottu-tilassa.
      */
    ToimijaRP() = default;

    /**
      * @brief Rajapintaluokassa on oletusarvoinen virtuaalipurkaja (olemassa, koska kantaluokalla tulee olla virtuaalipurkaja).
      */
    virtual ~ToimijaRP() = default;

    /**
     * @brief annaSijainti palauttaa toimijan sijainnin
     * @pre -
     * @return Toimijan sijainti.
     * @post Poikkeustakuu: vahva.
     * @exception PeliVirhe Toimijalle ei ole asetettu sijaintia.
     */
    virtual Sijainti annaSijainti() const = 0;

    /**
     * @brief liiku liikuttaa toimijan annettuun sijaintiin.
     * @param sij Toimijan uusi sijainti.
     * @pre -
     * @post Toimijan sijainti on sij. Poikkeustakuu: vahva.
     * @exception PeliVirhe Sijainti on mahdoton.
     */
    virtual void liiku(Sijainti sij) = 0;

    /**
     * @brief onkoTuhottu kertoo, onko toimija tuhottu pelissä.
     * @pre -
     * @return `true`, jos toimija on pelissä tuhottu, muuten `false`.
     * @post Poikkeustakuu: nothrow.
     */
    virtual bool onkoTuhottu() const = 0;

    /**
     * @brief tuhoa merkitsee toimijan tuhotuksi
     * @pre Toimija ei ole vielä tuhottu.
     * @post Toimija on tuhottu, tämän jälkeen onkoTuhottu() palauttaa `true`. Poikkeustakuu: perus.
     */
    virtual void tuhoa() = 0;

};

}

#endif // TOIMIJARP_HH
