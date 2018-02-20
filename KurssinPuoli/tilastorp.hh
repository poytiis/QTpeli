#ifndef TILASTORP_HH
#define TILASTORP_HH

/**
  * @file
  * @brief Määrittelee pistelaskutilastoja kuvaavan rajapintaluokan.
  */

namespace Rajapinta
{

/**
 * @brief TilastoRP on rajapinta, joka määrää pelitilastoa pitävän olion operaatiot.
 *
 * Jos luokan metodi ei ole poikkeustakuultaan nothrow, voi metodista aina vuotaa ulos poikkeus `std::bad_alloc` (muisti loppui).
 */
class TilastoRP
{
public:
    /**
      * @brief Rajapintaluokan oletusrakentaja (olemassa dokumentaatiota varten).
      * @post Alussa pistetilasto on nollattu.
      */
    TilastoRP() = default;

    /**
      * @brief Rajapintaluokassa on oletusarvoinen virtuaalipurkaja (olemassa, koska kantaluokalla tulee olla virtuaalipurkaja).
      */
    virtual ~TilastoRP() = default;

    /**
     * @brief annaPisteet palauttaa pelin senhetkisen pistemäärän.
     * @pre -
     * @return pistemäärä
     * @post Poikkeustakuu: nothrow
     */
    virtual int annaPisteet() const = 0;

    /**
     * @brief matkustajiaKuoli ilmoittaa, että matkustajia on eliminoitu.
     * @param lkm montako matkustajaa menehtyi.
     * @pre lkm > 0
     * @post Poikkeustakuu: vahva
     */
    virtual void matkustajiaKuoli(int lkm) = 0;

    /**
     * @brief lisaaMatkustajia ilmoittaa, että uusia matkustajia on lisätty peliin.
     * @param lkm montako uutta matkustajaa on lisätty.
     * @pre lkm > 0
     * @post Poikkeustakuu: vahva
     */
    virtual void lisaaMatkustajia(int lkm) = 0;

    /**
     * @brief nysseTuhoutui ilmoittaa, että nysse on tuhottu pelissä.
     * @pre -
     * @post Poikkeustakuu: vahva
     */
    virtual void nysseTuhoutui() = 0;

    /**
     * @brief uusiNysse ilmoittaa, että peliin on lisätty uusi nysse.
     * @pre -
     * @post Poikkeustakuu: vahva
     */
    virtual void uusiNysse() = 0;

    /**
     * @brief nyssePoistui ilmoittaa, että pelistä on poistunut nysse.
     * @pre -
     * @post Poikkeustakuu: vahva
     */
    virtual void nyssePoistui() = 0;
};

}

#endif // TILASTORP_HH
