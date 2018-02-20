#ifndef ALUSTUSVIRHE_HH
#define ALUSTUSVIRHE_HH

/**
  * @file
  * @brief Määrittelee alustusvirheitä ilmaisevan poikkeusluokan.
  */

#include <exception>
#include <QString>

namespace Rajapinta {

/**
 * @brief Poikkeusluokka, joka ilmaisee pelin alustamisessa tapahtunutta virhettä.
 */
class AlustusVirhe : public std::exception
{
public:

    /**
     * @brief Oletusrakentaja.
     * @post Luo AlustusVirheen ilman viestiä.
     */
    AlustusVirhe();

    /**
     * @brief Rakentaja.
     * @param viesti Poikkeuksen syytä selventävä viesti.
     * @pre -
     * @post Luo AlustusVirheen, joka sisältää poikkeuksen syytä selventävän viestin.
     */
    AlustusVirhe(const QString& viesti);

    /**
     * @brief Purkaja.
     */
    virtual ~AlustusVirhe();

    /**
     * @brief Toteuttaa std::exception rajapinnan.
     * @pre -
     * @post Poikkeustakuu: nothrow.
     * @return Poikkeusluokan nimi.
     */
    virtual const char* what() const noexcept;

    /**
     * @brief Antaa poikkeusksen syytä selventävä viestin.
     * @pre -
     * @post Poikkeustakuu: nothrow.
     * @return Rakentajassa annettu viesti tai tyhjä merkkijono.
     *
     */
    QString annaViesti() const;


private:
    QString viesti_;
};

} // Namespace Rajapinta

#endif // ALUSTUSVIRHE_HH
