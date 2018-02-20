#ifndef MATKUSTAJARP_HH
#define MATKUSTAJARP_HH

/**
  * @file
  * @brief Määrittelee matkustajan operaatiot kuvaavan rajapintaluokan.
  */

#include "toimijarp.hh"
#include "kulkuneuvorp.hh"

namespace Rajapinta
{

class PysakkiRP;

/**
 * @brief MatkustajaRP on rajapintaluokka, jonka kaikki pelin matkustajat toteuttavat.
 *
 * MatkustajaRP on periytetty ToimijaRP-rajapinnasta, eli sisältää myös senkin.
 * Jos luokan metodi ei ole poikkeustakuultaan nothrow, voi metodista aina vuotaa ulos poikkeus `std::bad_alloc` (muisti loppui).
 */
class MatkustajaRP : public virtual ToimijaRP
{
public:
    /**
      * @brief Rajapintaluokan oletusrakentaja (olemassa dokumentaatiota varten).
      * @post Matkustaja ei alussa ole missään kulkuneuvossa. Matkustajan määränpääpysäkki on asetettu.
      */

    // TODO: alustus
    MatkustajaRP() = default;

    /**
      * @brief Rajapintaluokassa on oletusarvoinen virtuaalipurkaja (olemassa, koska kantaluokalla tulee olla virtuaalipurkaja).
      */
    virtual ~MatkustajaRP() = default;

    /**
     * @brief onkoKulkuneuvossa kertoo, onko matkustaja sillä hetkellä missään kulkuneuvossa.
     * @pre -
     * @return Totuusarvo, joka kertoo onko matkustaja kulkuneuvossa.
     * @post Poikkeustakuu: nothrow
     */
    virtual bool onkoKulkuneuvossa() const = 0;

    /**
     * @brief annaKulkuneuvo palauttaa kulkuneuvon, jossa matkustaja on.
     * @pre -
     * @return Kulkuneuvo, jossa matkustaja on. Tyhjä osoitin, jos matkustaja ei ole kulkuneuvossa.
     * @post Poikkeustakuu: nothrow.
     */
    virtual std::shared_ptr<KulkuneuvoRP> annaKulkuneuvo() const = 0;

};

}

#endif // MATKUSTAJARP_HH
