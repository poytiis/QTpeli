#ifndef OFFLINELUKIJA_HH
#define OFFLINELUKIJA_HH

#include <QString>
#include "nysse.hh"
#include <list>
#include "pysakki.hh"
#include "sijainti.hh"
#include <map>
#include <QJsonObject>

namespace KurssinPuoli
{

struct paikka {
    bool pysakki;
    std::shared_ptr<Pysakki> pysakkiPtr;
    std::shared_ptr<Rajapinta::Sijainti> sijaintiPtr;
};

struct BussiData {
    unsigned int linja; // tarkoittaa bussin numeroa, esim. linja 25
    unsigned int reittiId; // yksilöivä tunniste, juokseva numero
    std::string reittiNimi; // tarkoittaa linjan nimeä muodossa "Pyynikintori - Hervanta"

    std::map< QTime, std::shared_ptr<Pysakki> > pysakit;
    std::list<QTime> aikataulu;
    std::vector< Rajapinta::Sijainti > reitti;
    std::map<QTime, Rajapinta::Sijainti > aikareitti;
    std::map< QTime, std::pair<Rajapinta::Sijainti, std::shared_ptr<Pysakki>> > aikareitti2;
};

struct OfflineData {
    std::vector< std::shared_ptr<Pysakki> > pysakit;
    std::list< std::shared_ptr<BussiData> > bussit;
};

class OfflineLukija
{
public:
    OfflineLukija();

    std::shared_ptr<OfflineData> lueTiedostot(const QString& bussitiedosto,
                                              const QString& pysakkitiedosto);

private:
    std::shared_ptr<OfflineData> offlinedata_;

    void lueBussiTiedosto(const QString& bussitiedosto);
    void luePysakkiTiedosto(const QString& pysakkitiedosto);
    void lueLahtoajat(const QJsonArray& timearray, BussiData* bussi);
    void lueReitti(std::shared_ptr<BussiData> bussi, QJsonObject& o);
    std::shared_ptr<Pysakki> etsiPysakki(int id);
    QTime laskeQTime(int aika);
};

}

#endif // OFFLINELUKIJA_HH
