#ifndef PELIIKKUNA_HH
#define PELIIKKUNA_HH

#include "pysakkirp.hh"
#include "nyssekuva.hh"
#include "pelialue.hh"
#include "drooni.hh"
#include <QWidget>
#include <QImage>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGraphicsPixmapItem>
#include <QPointF>
#include <QString>
#include <QTimer>
#include <vector>


namespace Ui {
class PeliIkkuna;
}

/**
 * @file
 * @brief PeliIkkuna on pelin "pääikkunan" tarjoava luokka.
 */
class PeliIkkuna : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief PeliIkkuna luo uuden peli-ikkunan.
     * @param parent peli-ikkunan vanhempi. Tässä ohjelmassa aina määrittämätön.
     * @post Ohjelmassa ei ole vielä peli-ikkuna oliota.
     */
    explicit PeliIkkuna(QWidget *parent = 0);

    /**
     * @brief PeliIkkunan purkaja.
     * @post PeliIkkuna on poistettu hallitusti.
     */
    ~PeliIkkuna();

    /**
     * @brief asetaKartta Asettaa pelialueen pohjakartan.
     * @param kartta Tampereen keskustan kartta
     * @pre Karttaa ei ole vielä asetettu. Kartta on pelilogiikan tarjoama isokartta.
     * @post Pohjakartta on asetettu. Poikkeustakuu: perus
     */
    void asetaKartta(QImage& kartta);

    /**
     * @brief lisaa_pysakki Lisää pysäkin kartalle
     * @param pysakkiptr Osoitin PysakkiRP:n toteuttavaan pysäkkiolioon
     * @return Palauttaa osoittimen pysäkkikuva olioon.
     * @pre Pysäkkiä ei ole vielä piirretty
     * @post Pysäkki on lisätty onnistuneesti. Poikkeustakuu: perus
     */
    QGraphicsPixmapItem* lisaa_pysakki(std::shared_ptr<Rajapinta::PysakkiRP> pysakkiptr);

    /**
     * @brief lisaa_nysse Lisää nyssen kartalle
     * @param toimijaptr Osoitin nysseolioon joka toteuttaa ToimijaRP:n.
     * @return Osoitin Nyssekuva olioon.
     * @pre toimijaptr on nysse ja löytyy kaupungista.
     * @post Nysse on lisätty onnistuneesti. Poikkeustakuu: perus
     */
    Nyssekuva *lisaa_nysse(std::shared_ptr<Rajapinta::ToimijaRP> toimijaptr);

    /**
     * @brief lisaaDrooni Lisää droonin kartalle.
     * @param drooni Pelissä käytettävä drooniolio
     * @pre Droonia ei ole vielä lisätty
     * @post Drooni on lisätty peliin. Poikkeustakuu: perus
     */
    void lisaaDrooni(Drooni* drooni);

    /**
     * @brief PaivitaTulokset Päivittää peliruudulla näytettävät tilastotiedot.
     * @param tulokset Vektori jossa päivitettävät tilastotiedot.
     * @pre -
     * @post Tulokset on päivitetty. Poikkeustakuu: perus
     */
    void PaivitaTulokset(std::vector<int> tulokset);

    /**
     * @brief naytaToplista Näyttää peliruudulla tämänhetkisen top10 tilanteen.
     * @param huipputulokset Vektori top10 tilastosta.
     * @pre Top10 listaa ei ole vielä esitetty
     * @post Top10 lista näkyy peliruudulla. Poikkeustakuu: perus
     */
    void naytaToplista(std::vector<QString> huipputulokset);

    /**
     * @brief asetaStatus Asettaa peliruudun statustekstin.
     * @param status Haluttu statusteksti
     * @pre -
     * @post Statusteksti on asetettu. Poikkeustakuu: perus
     */
    void asetaStatus(QString status);

public slots:
    // Kutsutaan aina kun drooni liikkuu, kohde = drooni
    // Pitää kohteen näkyvissä. Voitaisiin käyttää myös esim. Nyssen tai
    // matkustajan seuraamiseen.
    void liikutaViewport(QGraphicsItem* kohde);

    // Käytettiin sulavan ruudunpäivityksen yhteydessä. Ei toiminut tarpeeksi
    // smoothisti. :(
    void paivitaViewport();

    // Kutsutaan aina kun droonin ammusmäärä muutuu.
    void paivitaAmmukset(int ase, int ammuksia);

private:
    // Tasaisen ruudunpäivityksen kanssa käytetty QTimer. Ei käytössä atm.
    QTimer* ruudunpaivittaja_;
    Ui::PeliIkkuna *ui;
    PeliAlue* scene_;
    QImage karttap;
    Drooni *drooni_;
};

#endif // PELIIKKUNA_HH
