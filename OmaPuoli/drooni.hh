// Pelissä käytettävän droonin esittely

#ifndef DROONI_H
#define DROONI_H

#include "toimijarp.hh"
#include "sijainti.hh"
#include "rynnakkokivaari.hh"
#include <QGraphicsObject>
#include <QImage>
#include <QPointF>
#include <QTimer>

/**
 * @file
 * @brief Drooni luokka toteuttaa pelissä käytettävän droonin. Periytetty
 * QGraphicsObject ja ToimijaRP luokista.
 */

class Drooni : public QGraphicsObject, public Rajapinta::ToimijaRP
{
    Q_OBJECT

public:
    /**
     * @brief Droonin rakentaja, luo uuden drooni olion.
     * @param parent droonin vanhempi, mahdollista jättää määrittelemättä.
     * @pre -
     * @post Drooni on luotu ja valmis käytettäväksi. Poikkeustakuu: ??
     */
    Drooni(QGraphicsItem* parent = NULL);

    /**
     * @brief ~Drooni tuhoaa droonin.
     * @post drooni on tuhottu.
     */
    virtual ~Drooni();

    /**
     * @brief keyPressEvent vastaanottaa olion saaman näppäimistösyötteen.
     * Muuntaa nuolinäppäimet ja välilyönnin droonin liikkeiksi. Ei yleensä tarvitse
     * kutsua itse.
     * @param event QKeyEvent* tyyppinen tapahtuma.
     * @pre -
     * @post Drooni on reagoinnut painikkeeseen halutulla tavalla. Poikkeustakuu: ??
     */
    void keyPressEvent(QKeyEvent* event);

    /**
     * @brief keyReleaseEvent ks. keyPressEvent.
     * @param event ks. keyPressEvent.
     */
    void keyReleaseEvent(QKeyEvent* event);

    /**
     * @brief ammuksiaJaljella Kertoo paljonko kysytyssä aseessa on ammuksia jäljellä
     * @param ase Aseen, jonka ammustiedot halutaan, numero. 1 = RK, 2 = Maamiina, 3 = Laser
     * @return Jäljellä olevien ammusten lukumäärä
     * @pre ase on välillä [0,2]
     * @post Poikkeustakuu: vahva
     * @exception PeliVirhe mikäli aseen tietoja ei löydy
     */
    short int ammuksiaJaljella(int ase) const;

    // ToimijaRP:n toteutus - MUISTA TEHDÄ!!!
    virtual Rajapinta::Sijainti annaSijainti() const;
    virtual void liiku(Rajapinta::Sijainti sij);
    virtual bool onkoTuhottu() const;
    virtual void tuhoa();

    /**
     * @brief liikuSulavasti Laittaa droonin liikkumaan sulavasti kohti annettua sijaintia.
     * @param sij Määränpää sijainti
     * @pre -
     * @post Drooni liikkuu kohti määränpäätä. Poikkeustakuu: perus
     */
    void liikuSulavasti(Rajapinta::Sijainti sij);


    // Droonin sulavan liikkumisen mahdollistamiseen käytetty ajastin
    // ja boolean arvot
    QTimer* animaatioajastin;
    bool liikkuuEteen;
    bool liikkuuTaakse;
    bool kaantyyVasemmalle;
    bool kaantyyOikealle;



protected:
    // QGraphicsObjectin vaatimien metodien toteutus
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);
    virtual QRectF boundingRect() const;
    // Toteutettu droonin pitämiseksi pelialueella. Kutsutaan aina kun droonin sijainti
    // muuttuu.
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);

public slots:
    /**
     * @brief alaLiikkuaEteen Laittaa droonin liikkumaan eteenpäin kunnes liike lopetetaan.
     * @pre -
     * @post liikkuuEteen = true
     */
    void alaLiikkuaEteen();

    /**
     * @brief alaLiikkuaTaakse Laittaa droonin liikkumaan taaksepäin kunnes liike lopetetaan.
     * @pre -
     * @post liikkuuTaakse = true
     */
    void alaLiikkuaTaakse();

    /**
     * @brief alaKaantyaMyota Laittaa droonin kääntymään myötäpäivään kunnes kääntyminen lopetetaan.
     * @pre -
     * @post kaantyyOikealle = true
     */
    void alaKaantyaMyota();

    /**
     * @brief alaKaantyaVasta Laittaa droonin kääntymään vastapäivään kunnes kääntyminen lopetetaan.
     * @pre -
     * @post kaantyyVasemmalle = true;
     */
    void alaKaantyaVasta();

    /**
     * @brief lopetaEteenLiikkuminen Lopettaa droonin eteenpäin liikkumisen.
     * @pre -
     * @post liikkuuEteen = false
     */
    void lopetaEteenLiikkuminen();

    /**
     * @brief lopetaTaakseLiikkuminen Lopettaa droonin taaksepäin liikkumisen.
     * @pre -
     * @post liikkuuTaakse = false
     */
    void lopetaTaakseLiikkuminen();

    /**
     * @brief lopetaKaannosMyota Lopettaa droonin kääntymisen myötäpäivään.
     * @pre -
     * @post kaantyyOikealle = false
     */
    void lopetaKaannosMyota();

    /**
     * @brief lopetaKaannosVasta Lopettaa droonin kääntymisen vastapäivään.
     * @pre -
     * @post kaantyyVasemmalle = false
     */
    void lopetaKaannosVasta();


    /**
     * @brief ammu luo uuden valitun aseen ammuksen ja lisää sen samaan QGraphicsScenee droonin
     * kanssa.
     * @pre Drooni kuuluu QGraphicsSceneen ja ase on valituna.
     * @post Uusi ammus on luotu ja lisätty QGraphicsSceneen. Poikkeustakuu: vahva
     */
    void ammu();
    /**
     * @brief valitseRynkky valitsee rynnäkkökiväärin aktiiviseksi aseeksi.
     * @pre -
     * @post Rynnäkkökivääri on valittuna. Poikkeustakuu: vahva
     */
    void valitseRynkky();
    /**
     * @brief valitseMiina valitsee maamiinan aktiiviseksi aseeksi.
     * @pre -
     * @post Maamiina on valittuna. Poikkeustakuu: vahva
     */
    void valitseMiina();
    /**
     * @brief valitseLaser valitsee laserin aktiiviseksi aseeksi.
     * @pre -
     * @post Laser on valittuna. Poikkeustakuu: vahva
     */
    void valitseLaser();

signals:
    /**
     * @brief drooniLiikkunut signaali lähetetään aina kun drooni on liikkunut.
     * @param ottaa droonin parametrinansa.
     */
    void drooniLiikkunut(QGraphicsItem* drooni);

    /**
     * @brief drooniAmpunut signaali lähetetään aina kun drooni on ampunut.
     * @param ase Ampumiseen käytetty ase.
     * @param ammuksia_jaljella Aseen jäljellä oleva ammusmäärä.
     */
    void drooniAmpunut(short int ase, short int ammuksia_jaljella);

    /**
     * @brief ammuksetLopussa signaali lähetetään kun droonin ammukset ovat lopussa.
     */
    void ammuksetLopussa();

private:
    // Droonin liikuttamiseen "konepellin alla" käytetyt funktiot.
    void liikuYlos();
    void liikuAlas();
    void liikuVasen();
    void liikuOikea();
    void tarkistaSulavaLiike();
    QPointF sulava_kohde_{QPointF(0,0)};
    bool kaantyySulavasti_{false};
    bool liikkuuSulavasti_{false};

    // Palauttaa droonin keulan edessä 1 etäisyydellä olevan pisteen
    QPointF suuntaPiste() const;
    // Droonin kuva
    QImage alus_;
    // Keulan suunta asteina
    //qreal suunta;

    // Droonin huippunopeus, tämänhetkinen nopeus ja kiihtyvyys
    short int nopeus_{4};
    qreal hetkellinen_nopeus_{0};
    qreal kiihtyvyys_{0.1};
    qreal pakitus_nopeus_{(double)nopeus_/2};
    qreal hetkellinen_pakitus_nopeus_{0};
    short int kaantymis_nopeus_{4};

    short int puolikas_{20};

    // Aseen valinta
    short int valittu_ase_{0};

    // Droonin ammukset. Julkisia, koska esim. kauppa voi lisätä ammuksia suoraan.
    short int rk_ammuksia{20};
    short int mm_ammuksia{10};
    short int laser_ammuksia{3};

    bool tuhottu_{false};

};

#endif // DROONI_H
