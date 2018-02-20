#include "offlinelukija.hh"


#include <string>
#include <iostream>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include <QJsonArray>
#include "pysakki.hh"
#include <QString>

namespace KurssinPuoli
{

OfflineLukija::OfflineLukija()
{
}

std::shared_ptr<OfflineData> OfflineLukija::lueTiedostot(const QString& bussitiedosto,
                                                         const QString& pysakkitiedosto)
{
    offlinedata_ = std::make_shared<OfflineData>();
    luePysakkiTiedosto(pysakkitiedosto);
    lueBussiTiedosto(bussitiedosto);


    return offlinedata_;
}

void OfflineLukija::lueBussiTiedosto(const QString &bussitiedosto)
{
    QString filecontent;
    QFile file;
    file.setFileName(bussitiedosto);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    filecontent = file.readAll();
    file.close();

    QJsonParseError parse_error;
    QJsonDocument document = QJsonDocument::fromJson( filecontent.toUtf8()  , &parse_error);
    if (parse_error.error != QJsonParseError::NoError) {
        qDebug() << "Error parsing bus JSON: " << parse_error.errorString() << " at: " << filecontent.mid(parse_error.offset-3, parse_error.offset+3);
        return;
    }
    QJsonObject jsonObject = document.object();
    QJsonArray jsonArray = document.array();

    qDebug() << bussitiedosto;
    //qDebug() << jsonObject;
    //qDebug() << jsonArray;

    for (int i = 0; i < jsonArray.size(); i++) {

        QJsonObject o = jsonArray.at(i).toObject();
        //qDebug() << o;
        std::shared_ptr<BussiData> bussi = std::make_shared<BussiData>();
        bussi->reittiId = o.value("busId").toString().toInt();
        bussi->reittiNimi = o.value("busLineName").toString().toStdString();
        bussi->linja = o.value("busNro").toString().toInt();

        // Lue lähtöajat päätepysäkiltä
        QJsonArray lahtoajat = o.value("startTimes").toArray();
        for (int j = 0; j < lahtoajat.size(); j++) {
            QTime lahtoaika = laskeQTime(lahtoajat.at(j).toString().toInt());
            bussi->aikataulu.push_back(lahtoaika);
        }

        // Lue pysäkit ja aika päätepysäkiltä
        QJsonArray fullSS = o.value("fullSS").toArray();
        for (int j = 0; j < fullSS.size(); j++) {

            // station / stop = SS obj
            QJsonObject SSobj = fullSS.at(j).toObject();
            std::string tikaikastring = SSobj.value("mns").toString().toStdString();

            int aikamm = SSobj.value("mm").toInt();
            int aikass = SSobj.value("ss").toInt();
            QTime aika = QTime(aikamm/60, aikamm % 60, aikass);

            std::shared_ptr<Pysakki> pysakki = nullptr;

            if (SSobj.value("stop") == true) {

                // Etsitään pysäkkiin pointteri
                int id = SSobj.value("stationId").toInt();
                //qDebug() << id;
                 pysakki = etsiPysakki(id);
                // Aika, joka kuluu päätepysäkiltä tälle pysäkille
                QJsonValue timevalue = SSobj.value("mm");
                //qDebug() << timevalue.toString();

            }

            int lev = SSobj.value("y").toDouble();
            int pit = SSobj.value("x").toDouble();

            std::pair<Rajapinta::Sijainti, std::shared_ptr<Pysakki> > parivaljakko = { Rajapinta::Sijainti(lev, pit), pysakki };

            bussi->aikareitti2.insert(std::pair<QTime, std::pair<Rajapinta::Sijainti, std::shared_ptr<Pysakki> > >( aika, parivaljakko ));

        }

        offlinedata_->bussit.push_back(bussi);

    }







        //qDebug() << bussi->id << " " << bussi->linja;
        //   lueReitti(bussi, o);
        // TODO: huolehdi pysäkeistä
        //        Pysakki *pysakki = new Pysakki(Sijainti(o.value("Y").toString().toDouble(), o.value("X").toString().toDouble()),
        //                                       o.value("Name").toString(),
        //                                       (unsigned int)o.value("StationId").toString().toInt());

        //        qDebug() << pysakki->id();
        //        qDebug() << pysakki->nimi();
        //        qDebug() << pysakki->sijainti().annaLeveyspiiri();
        //        qDebug() << pysakki->sijainti().annaPituuspiiri();


    }

void OfflineLukija::luePysakkiTiedosto(const QString &pysakkitiedosto)
{
    QString filecontent;
    QFile file;
    file.setFileName(pysakkitiedosto);
    //qDebug() << pysakkitiedosto;
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    filecontent = file.readAll();
    file.close();

    QJsonDocument document = QJsonDocument::fromJson(filecontent.toUtf8());
    QJsonObject jsonObject = document.object();
    QJsonArray jsonArray = document.array();

    //qDebug() << jsonObject;
    //qDebug() << jsonArray;

    for (int i = 0; i < jsonArray.size(); i++) {
        QJsonObject o = jsonArray.at(i).toObject();
        //qDebug() << o;
        std::shared_ptr<Pysakki> pysakki = std::make_shared<Pysakki>(Rajapinta::Sijainti(o.value("y").toString().toDouble(), o.value("x").toString().toDouble()),
                                                                     o.value("name").toString(),
                                                                     (unsigned int)o.value("stationId").toString().toInt());

        //        qDebug() << pysakki->id();
        //        qDebug() << pysakki->nimi();
        //        qDebug() << pysakki->sijainti().annaLeveyspiiri();
        //        qDebug() << pysakki->sijainti().annaPituuspiiri();

        offlinedata_->pysakit.push_back(pysakki);
        //        qDebug() << pysakki->id();
        //        qDebug() << pysakki->nimi();
        //        qDebug() << pysakki->sijainti().annaLeveyspiiri();
        //        qDebug() << pysakki->sijainti().annaPituuspiiri();

    }
}

void OfflineLukija::lueReitti(std::shared_ptr<BussiData> bussi, QJsonObject& o)
{
    QJsonArray reitti = o.value("shape").toArray();
    for (int i = 0; i < reitti.size(); i++) {

        QJsonObject piste = reitti.at(i).toObject();

        double lev = piste.value("y").toString().toDouble();
        double pit = piste.value("x").toString().toDouble();
        bussi->reitti.push_back(Rajapinta::Sijainti(lev, pit));
    }
}

std::shared_ptr<Pysakki> OfflineLukija::etsiPysakki(int id)
{
    std::shared_ptr<Pysakki> pysakki = NULL;
    for (unsigned int i = 0; i < offlinedata_->pysakit.size(); i++) {
        pysakki = offlinedata_->pysakit.at(i);
        int current = pysakki->annaId();
        if (current == id) break;
    }
    return pysakki;
}

QTime OfflineLukija::laskeQTime(int aika)
{
    return QTime(aika/100, aika%100);
}

}
