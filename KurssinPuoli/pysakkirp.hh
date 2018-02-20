#ifndef PYSAKKIRP_HH
#define PYSAKKIRP_HH

#include "matkustajarp.hh"
#include <memory>
/**
  * @file
  * @brief Määrittelee pysäkin operaatiot kuvaavan rajapintaluokan.
  */

#include <QString>

namespace Rajapinta
{

/**
 * @brief PysakkiRP on rajapintaluokka, jonka pysäkit toteuttavat.
 *
 * Jos luokan metodi ei ole poikkeustakuultaan nothrow, voi metodista aina vuotaa ulos poikkeus `std::bad_alloc` (muisti loppui).
 */
class PysakkiRP
{
public:
    /**
      * @brief Rajapintaluokan oletusrakentaja (olemassa dokumentaatiota varten).
      * @post Alussa pysäkillä ei ole matkustajia.
      */
    PysakkiRP() = default;

    /**
      * @brief Rajapintaluokassa on oletusarvoinen virtuaalipurkaja (olemassa, koska kantaluokalla tulee olla virtuaalipurkaja).
      */
    virtual ~PysakkiRP() = default;

    /**
     * @brief sijainti palauttaa pysäkin sijainnin.
     * @pre -
     * @return Pysäkin sijainti
     * @post Poikkeustakuu: nothrow
     */
    virtual Sijainti annaSijainti() const = 0;

    /**
     * @brief nimi palauttaa pysäkin nimen.
     * @pre -
     * @return pysäkin nimi.
     * @post Poikkeustakuu: nothrow
     */
    virtual QString annaNimi() const = 0;

    /**
     * @brief id palauttaa pysäkin id:n eli pysäkkinumeron
     * @pre -
     * @return pysäkkinumero
     * @post Poikkeustakuu: nothrow
     */
    virtual unsigned int annaId() const = 0;

    /**
     * @brief matkustajat palauttaa kaikki pysäkillä olevat matkustajat
     * @pre -
     * @return Vektori, joka sisältää pysäkillä olevat matkustajat.
     * @post Poikkeustakuu: vahva
     */
    virtual std::vector<std::shared_ptr<Rajapinta::MatkustajaRP>> annaMatkustajat() const = 0;
};

}

#endif // PYSAKKIRP_HH
