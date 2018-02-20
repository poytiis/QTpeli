#ifndef ALYKAUPUNKIRP_HH
#define ALYKAUPUNKIRP_HH


/**
  * @file
  * @brief Määrittelee älykaupungin operaatiot kuvaavan rajapintaluokan.
  */


#include "kaupunkirp.hh"
#include "matkustajarp.hh"

namespace Rajapinta
{

/**
 * @brief AlyKaupunkiRP on rajapintaluokka, jonka älykaupungit toteuttavat.
 *
 * AlyKaupunkiRP on periytetty KaupunkiRP-rajapinnasta, eli sisältää myös senkin.
 * Jos luokan metodi ei ole poikkeustakuultaan nothrow, voi metodista aina vuotaa ulos poikkeus `std::bad_alloc` (muisti loppui).
 */

class AlyKaupunkiRP : public virtual KaupunkiRP
{
public:

    /**
      * @brief Rajapintaluokan oletusrakentaja (olemassa dokumentaatiota varten).
      * @post Kaupunki on alustustilassa.
      */
    AlyKaupunkiRP() = default;

    /**
      * @brief Rajapintaluokassa on oletusarvoinen virtuaalipurkaja (olemassa, koska kantaluokalla tulee olla virtuaalipurkaja).
      */
    virtual ~AlyKaupunkiRP() = default;


    /**
     * @brief lisaaReitti antaa älykaupungille yhtä bussilinjaa (reittiä) kuvaavan reitin tiedot.
     * @param reittiId Reitin yksilöivä tunniste.
     * @param reittiNimi Reitin nimi.
     * @param reitti Vektori, joka sisältää reitin. Reitti on pareja muodossa <kulunut aika ensimmäisestä pysäkistä, pysäkkiId>.
     * @pre Kaupunki on alustustilassa.
     * @post Kaupunki tuntee annetun reitin.
     * @exception AlustusVirhe Annettu reitti on kelvoton.
     */

    virtual void lisaaReitti( int reittiId, std::string reittiNimi,
                             std::vector< std::pair<int, int> > reitti ) = 0;

    /**
     * @brief lisaaVuoro antaa älykaupungille yhden bussivuoron.
     * @param reittiId Reitin, jolla bussivuoro kulkee, yksilöivä tunniste.
     * @param lahtoaika Validi aika, jolloin bussi lähtee ensimmäiseltä pysäkiltä kulkemaan reitille.
     * @pre Kaupunki on alustustilassa.
     * @post Kaupunki tietää vuoron.
     */

    virtual void lisaaVuoro(int reittiId, QTime lahtoaika) = 0;


    /**
     * @brief luoAlyMatkustaja pyytää älykaupungilta älymatkustajan. Älykaupunki luo älymatkustajan, joka on periytetty KurssinPuoli-nimiavaruuden matkustajasta.
     * @param maaranpaa Osoitin aikaisemmin kaupungille annettuun pysäkkiin, joka on luotavan älymatkustajan määränpää.
     * @return Osoitin uuteen älymatkustajaan.
     */
    virtual std::shared_ptr<MatkustajaRP> luoAlyMatkustaja(std::weak_ptr<PysakkiRP> maaranpaa) const = 0;

};

}
#endif // ALYKAUPUNKIRP_HH
