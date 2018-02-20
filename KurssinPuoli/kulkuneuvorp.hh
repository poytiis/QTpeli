#ifndef KULKUNEUVORP_HH
#define KULKUNEUVORP_HH

/**
  * @file
  * @brief Määrittelee kulkuneuvon operaatiot kuvaavan rajapintaluokan.
  */

#include "toimijarp.hh"
//#include "matkustajarp.hh"
#include <vector>
#include <memory>

namespace Rajapinta
{

class MatkustajaRP;

/**
 * @brief KulkuneuvoRP on rajapinta, joka kuvaa pelissä olevat matkustajia ottavat kulkuneuvot (siis käytännössä nysset).
 *
 * KulkuneuvoRP on periytetty ToimijaRP-rajapinnasta, eli sisältää myös senkin.
 * Jos luokan metodi ei ole poikkeustakuultaan nothrow, voi metodista aina vuotaa ulos poikkeus `std::bad_alloc` (muisti loppui).
 */
class KulkuneuvoRP : public virtual ToimijaRP
{
public:
    /**
      * @brief Rajapintaluokan oletusrakentaja (olemassa dokumentaatiota varten).
      * @post Alussa kulkuneuvossa ei ole matkustajia.
      */
    KulkuneuvoRP() = default;

    /**
      * @brief Rajapintaluokassa on oletusarvoinen virtuaalipurkaja (olemassa, koska kantaluokalla tulee olla virtuaalipurkaja).
      */
    virtual ~KulkuneuvoRP() = default;

    /**
     * @brief annaNimi palauttaa kulkuneuvon nimen (joka ei välttämättä ole uniikki)
     * @pre -
     * @return kulkuneuvon nimi
     * @post Poikkeustakuu: vahva
     */
    virtual std::string annaNimi() const = 0;

    /**
     * @brief annaMatkustajat palauttaa kaikki kulkuneuvon matkustajat.
     * @pre -
     * @return Vektori, jossa on kulkuneuvossa sillä hetkellä olevat matkustajat.
     * @post Poikkeustakuu: vahva.
     */
    virtual std::vector<std::shared_ptr<MatkustajaRP> > annaMatkustajat() const = 0;

    /**
     * @brief lisaaMatkustaja lisää kulkuneuvoon uuden matkustajan
     * @param matkustaja Kulkuneuvoon lisättävä MatkustajaRP:n toteuttava olio.
     * @pre `matkustaja.onkoKulkuneuvossa() == false`.
     * @post Matkustaja on lisätty kulkuneuvoon. Poikkeustakuu: perus.
     */
    virtual void lisaaMatkustaja(std::shared_ptr<MatkustajaRP> matkustaja) = 0;

    /**
     * @brief poistaMatkustaja poistaa matkustajan kulkuneuvosta
     * @param matkustaja Kulkuneuvosta poistettava matkustaja.
     * @pre -
     * @post Matkustaja on poistettu kulkuneuvosta. Poikkeustakuu: perus.
     * @exception PeliVirhe Matkustajaa ei ole ko. kulkuneuvossa
     */
    virtual void poistaMatkustaja(std::shared_ptr<MatkustajaRP> matkustaja) = 0;

};

}

#endif // KULKUNEUVORP_HH
