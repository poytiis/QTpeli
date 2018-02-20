#include "pelivirhe.hh"

namespace Rajapinta
{

PeliVirhe::PeliVirhe() :
    std::exception(), viesti_()
{
}


PeliVirhe::PeliVirhe(const QString& viesti) :
    std::exception(), viesti_(viesti)
{
}


PeliVirhe::~PeliVirhe()
{
}


const char* PeliVirhe::what() const noexcept
{
    return "Rajapinta::PeliVirhe";
}


QString PeliVirhe::annaViesti() const
{
    return viesti_;
}

} // Namespace Rajapinta
