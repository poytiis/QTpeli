#include "alustusvirhe.hh"

namespace Rajapinta
{

AlustusVirhe::AlustusVirhe() :
    std::exception(), viesti_()
{
}


AlustusVirhe::AlustusVirhe(const QString& viesti):
    std::exception(), viesti_(viesti)
{
}


AlustusVirhe::~AlustusVirhe()
{
}

const char* AlustusVirhe::what() const noexcept
{
    return "Rajapinta::AlustusVirhe";
}


QString AlustusVirhe::annaViesti() const
{
    return viesti_;
}

} // Namespace Rajapinta
