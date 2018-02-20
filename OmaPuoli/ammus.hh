// Kantaluokka kaikille pelissä käytettäville ammuksille (aseille)

#ifndef AMMUS_H
#define AMMUS_H

#include <QGraphicsObject>
#include <QTimer>
#include <QPointF>
/**
 * @file
 * @brief Kantaluokka kaikille pelissä käytettäville ammuksille. Tarjoaa perustoiminnoille toteutukset.
 * Muistin loppuessa heittää std::bad_alloc ellei poikkeustakuu ole nothrow.
 */


class Ammus: public QGraphicsObject
{
public:
    /**
     * @brief Tyhmä rakentaja ammukselle, ei tule käyttää.
     */
    Ammus();

    /**
     * @brief Ammuksen "perus" rakentaja; ei ole hyvä käyttää sinänsä, mutta toteutettu malliksi aliluokkien rakentajaksi.
     * @param parent ammuksen vanhempi
     * @param suunta Ammuksen lähtösuunta
     * @pre -
     * @post Ammus on luotu, alustettu ja liikkuu
     */
    Ammus(QGraphicsItem *parent, qreal suunta);

    /**
     *@brief Ammus kantaluokan purkaja, hoitaa kantaluokan new'llä varaamien muuttujien tuhoamisen.
     *
     */
    ~Ammus();

    /**
     * @brief etene liikuttaa ammusta ja tarkistaa mahdolliset osumat. Kantaman ylittyessä tuhoaa ammuksen.
     * @pre Ammus on alustettu
     * @post  Ammus on liikkunut ja tarkastanut osumat. Poikkeustakuu: perus
     */
    virtual void etene();

    /**
     * @brief ulkoOsuma tuhoaa ammuksen. Käytetään kun ammus halutaan tuhota ulkopuolelta.
     * @pre Ammus on alustettu
     * @post Ammus on tuhottu normaalisti tai ajoitettu tuhoutumaan normaalisti. Poikkeustakuu: perus.
     */
    virtual void ulkoOsuma();

public slots:
    /**
     * @brief tuhoaAmmus tuhoaa ammuksen hallitusti, käytetään singleshot timerin kanssa viivästettyyn tuhoamiseen.
     * Hyödyllinen esimerkiksi animoidun räjähdyksen kanssa.
     * @pre Ammus on alustettu
     * @post Ammus on tuhottu oikein. Poikkeustakuu: vahva
     */
    void tuhoaAmmus();

protected:

    // QGraphicsObjectista perittyjen funktioiden toteutukset. Aliluokkien ei tarvitse murehtia näistä.
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual QRectF boundingRect() const;

    /**
     * @brief alustaAmmus Alustaa ammuksen toimintakuntoon. Käynnistää ammuksen ajastimen. Kutsutaan yleensä
     * ammuksen rakentajassa.
     * @param parent Ammuksen ampuja, ammuksen lähtöpaikaksi asetetaan ampujan paikka
     * @param suunta Ammuksen lähtösuunta
     * @pre Ammus on alustamaton
     * @post Ammus on alustettu ja toimintakuntoinen. Poikkeustakuu: perus
     */
    virtual void alustaAmmus(QGraphicsItem *parent, qreal suunta);

    /**
     * @brief asetaAmmuskuva asettaa ammuksen ulkoasun. Toteutettava periytetyssä luokassa, asetettava
     * ammuskuva on QImage Ammus::ammuskuva.
     * @pre -
     * @post Ammus::ammuskuva on asetettu. Poikkeustakuu: perus
     * @exception PeliVirhe Ammuskuvan avaaminen epäonnistui.
     */
    virtual void asetaAmmuskuva()=0;    // Huom! Tämän tulee asettaa Ammus::ammuskuva!

    /**
     * @brief tarkistaOsuma tarkistaa osuuko ammus nysseen ja osuttaessa ilmoittaa sekä ammukselle että
     * nysselle osumasta.
     * @pre Ammus on alustettu
     * @return true mikäli ammus osui, muuten false
     * @post Poikkeustakuu: perus
     */
    virtual bool tarkistaOsuma();

    /**
     * @brief osuma määrittelee ammuksen (ylimääräisen) käyttäytymisen osuessa kohteeseen. Oletustoteutus ei tee mitään.
     * @pre -
     * @post Poikkeustakuu: perus
     */
    virtual void osuma();

    /**
     * @brief suuntaPiste selvittää ammuksen etenemissuunnan.
     * @pre Ammus on alustettu
     * @return etäisyydellä 1 ammuksesta oleva QPointF ammuksen etenemissuunnassa
     * @post Poikkeustakuu: perus
     */
    QPointF suuntaPiste() const;

    // Ammuksen kuva
    QImage ammuskuva;

    // Animoitu osuma?
    bool animoituOsuma_{false};

    // Mahdollista konffata uusiksi periytetyn luokan rakentajassa
    int nopeus{1500};
    int kantama{300};

    // Suunta asteina
    qreal suunta_{0};
    // Ammuksen kulkema matka, käytetään esim. kantaman seurantaan
    qreal etenema;

    // Ammusta liikuttava QTimer. Päivitys 100ms välein.
    QTimer* animaatioajastin;

    // Animoitua tuhoutumista varten tulee tietää onko animaatio jo käynnissä
    bool tuhoutumassa{false};

};

#endif // AMMUS_H
