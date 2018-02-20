// Tilaa kommentille ^

#include "luopeli.hh"
#include "kaupunki.hh"
#include "kaupunkirp.hh"
#include <memory>


std::shared_ptr<Rajapinta::KaupunkiRP> Rajapinta::luoPeli(){
    auto tre = std::make_shared<Kaupunki> ();
    return tre;
}

