#ifndef PELIVIRHE_HH
#define PELIVIRHE_HH

/**
  * @file
  * @brief Määrittelee pelitilassa tapahtuvia virheitä ilmaisevan poikkeusluokan.
  */

#include <exception>
#include <QString>

namespace Rajapinta
{

/**
 * @brief Poikkeusluokka, joka ilmaisee pelitilassa tapahtuvia virheitä.
 */
class PeliVirhe : public std::exception
{
public:

    /**
     * @brief Oletusrakentaja.
     * @pre -
     * @post Luo PeliVirheen ilman viestiä.
     */
    PeliVirhe();

    /**
     * @brief Rakentaja.
     * @param viesti Poikkeuksen syytä selventävä viesti.
     * @pre -
     * @post Luo PeliVirheen, joka sisältää poikkeuksen syytä selventävän viestin.
     */
    PeliVirhe(const QString& viesti);

    /**
     * @brief Purkaja.
     */
    virtual ~PeliVirhe();

    /**
     * @brief Toteuttaa std::exception rajapinnan.
     * @pre -
     * @post Poikkeustakuu: nothrow.
     * @return Poikkeusluokan nimi.
     */
    virtual const char* what() const noexcept;

    /**
     * @brief Antaa poikkeuksen syytä selventävän viestin.
     * @pre -
     * @post Poikkeustakuu: nothrow.
     * @return Rakentajassa annettu viesti tai tyhjä merkkijono.
     */
    QString annaViesti() const;


private:
    QString viesti_;
};

}

#endif // PELIVIRHE_HH
