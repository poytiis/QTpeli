#ifndef SIJAINTI_HH
#define SIJAINTI_HH

/**
  * @file
  * @brief Määrittelee sijaintiin (koordinaatteihin) liittyviä operaatioita
  * toteuttavan luokan.
  *
  */

namespace Rajapinta
{

/**
 * @brief Sijainti on luokka, joka sijaintiin liittyviä palveluita.
 *
 * Luokka tarjoaa muutokset karttakoordinaateista pikselikoordinaatteihin ja takaisin, etäisyyden laskemisen sekä mahdollisuuden
 * tuottaa kahden pisteen välissä olevia sijainteja.
 *
 */
class Sijainti
{
public:
    /**
     * @brief Oletusrakentaja
     * @post Sijainniksi on asetettu pohjois=6700000, ita=3500000.
     */
    Sijainti();

    /**
     * @brief Rakentaja, joka alustaa sijainnin tiettyyn karttakoordinaatistopisteeseen.
     * @param pohjoiskoord sijainnin pohjoiskoordinaatti
     * @param itakoord sijainnin itäkoordinaatti
     * @pre -
     * @post Sijainti on asetettu annetuksi.
     */
    Sijainti(int pohjoiskoord, int itakoord);

    /**
     * @brief annaX palauttaa sijainnin x-koordinaatin pelin käyttöliittymän pikselikoordinaatistossa.
     * @pre -
     * @return X-pikselikoordinaatti.
     * @post Poikkeustakuu: nothrow.
     */
    int annaX() const;

    /**
     * @brief annaYpalauttaa sijainnin y-koordinaatin pelin käyttöliittymän pikselikoordinaatistossa.
     * @pre -
     * @return Y-pikselikoordinaatti.
     * @post Poikkeustakuu: nothrow.
     */
    int annaY() const;

    /**
     * @brief siirraXY siirtää sijainnin uuteen paikkaan pikselikoordinaattien perusteella.
     * @param x uuden sijainnin x-pikselikoordinaatti
     * @param y uuden sijainnin y-pikselikoordinaatti
     * @pre -
     * @post Sijainti on päivitetty. Poikkeustakuu: vahva.
     */
    void asetaXY(int x, int y);

    /**
     * @brief annaPohjoisKoord palauttaa sijainnin pohjoiskoordinaatin karttakoordinaatistossa.
     * @pre -
     * @return Pohjoiskoordinaatti
     * @post Poikkeustakuu: nothrow.
     */
    double annaPohjoisKoord() const;

    /**
     * @brief annaItaKoord palauttaa sijainnin itäkoordinaatin karttakoordinaatistossa.
     * @pre -
     * @return Itäkoordinaatti
     * @post Poikkeustakuu: nothrow.
     */
    double annaItaKoord() const;

    /**
     * @brief asetaPohjIta siirtää sijainnin uuteen paikkaan karttakoordinaattien perusteella.
     * @param pohjkoord uuden sijainnin pohjoiskoordinaatti
     * @param itakoord uuden sijainnin itäkoordinaatti
     * @pre -
     * @post Sijainti on päivitetty. Poikkeustakuu: vahva.
     */
    void asetaPohjIta(int pohjkoord, int itakoord);

    /**
     * @brief onkoLahella kertoo, onko annettu sijainti lähellä tätä sijaintia.
     *
     * Palauttaa toden, jos kutsussa `s1.onkoLahella(s2)` sijainnit `s1` ja `s2` ovat pelissä niin lähellä toisiaan, että
     * niillä voi olla vaikutusta toisiinsa (esim. droonin osumakohta on niin lähellä nysseä, että se tuhoutuu tai
     * matkustaja on niin lähellä kulkuneuvoa, että voi nousta kyytiin).
     * @param sij sijainti, jonka läheisyyttä tutkitaan.
     * @pre -
     * @return Totuusarvo, joka kertoo onko sij lähellä tätä sijaintia.
     * @post Poikkeustakuu: nothrow.
     */
    bool onkoLahella(Sijainti const& sij) const;

    /**
     * @brief laskeEtaisyys laskee kahden sijainnin etäisyyden karttakoordinaatistossa.
     * @param a ensimmäinen sijainti
     * @param b toinen sijainti
     * @pre -
     * @return kahden sijainnin etäisyys
     * @post Poikkeustakuu: nothrow.
     */
    static double laskeEtaisyys(Sijainti a, Sijainti b);

    /**
     * @brief eteneSuoraa laskee sijainnin halutussa paikassa kahden sijainnin välissä
     * @param a ensimmäinen sijainti
     * @param b toinen sijainti
     * @param etaisyys suhdeluku, mihin kohtaa a:n ja b:n välissä halutaan. 0.0=a:ssa, 1.0=b:ssä, 0.5=puolessa välissä.
     * @pre -
     * @return Sijainti, joka on halutussa kohdassa a:n ja b:n välissä.
     * @post Poikkeustakuu: nothrow
     */
    static Sijainti eteneSuoraa(Sijainti a, Sijainti b, double etaisyys);

    /**
     * @brief tulostaMolemmat tulostaa (debuggaustarkoituksessa) sijainnista sekä kartta- että pikselikoordinaatit
     * @pre -
     * @post Tulostaa koordinaatit. Poikkeustakuu: vahva.
     */
    void tulostaMolemmat();
    bool operator==(const Sijainti& sijainti);

private:

    static int xIdasta(int itakoord);
    static int yPohjoisesta(int pohjkoord);
    static int ItaXsta(int x);
    static int PohjYsta(int y);

    int pohjkoord_;
    int itakoord_;
    int x_;
    int y_;
};

}

#endif // SIJAINTI_HH
