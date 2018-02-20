#ifndef LUOPELI_HH
#define LUOPELI_HH

/**
  * @file
  * @brief Määrittelee kaupungin luovan funktion (jonka opiskelijat toteuttavat).
  */

#include "kaupunkirp.hh"

#include <memory>

/**
 * @brief
 * Kaikki kurssin määrittelemät rajapinnat löytyvät Rajapinta-nimiavaruudesta.
 */
namespace Rajapinta
{

/**
 * @brief luoPeli luo pelin kaupungin ja palauttaa osoittimen siihen.
 * @pre -
 * @return osoitin luotuun kaupunkiin (joka on vielä alustustilassa).
 * @post Poikkeustakuu: perus.
 */
std::shared_ptr<KaupunkiRP> luoPeli();

}

#endif // LUOPELI_HH

