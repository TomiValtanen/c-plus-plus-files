# c-plus-plus-files

# Blackjack korttipeli.

## Pelinkulku
ruudulla näkyy ohjeet koko aikaisesti ja ne ohjaavat pelaajaa.
Tarkoituksena on saada enemmän pisteitä mitä jakaja kuitenkaan menemättä 21 pisteen yli.
Pisteitä saa seuraavasti Ässä = 1 tai 11 riippuen meneekö pelaajan pisteet yli 21 .Numerokortit antavat osoittamansa luvun ja kuvakortit (jätkä , akka , kunkku) antavat kaikki 10 pistettä.
Pelissä panoksen suuruus on 10 - 100.
Insurance on mahdollista ottaa ja se maksaa puolet alkuperäisestä panoksesta.
Split on myös mahdollinen ja sen voi ottaa, jos alussa jaettavat kaksi korttia ovat samanlaiset. Splitin aikana voi double down.
Double down on myös mahdollista splitin ulkopuolella ja tämä onnistuu, ensimmäisen kahden kortin summa on 9 , 10 tai 11. Sen jälkeen jaetaan 1 kortti ja katsotaan kumpi voittaa.
Sitten on hit / stand mahdollisuus.
Pelin ennätys perustuu siihen paljon oli parhaillaan rahaa pelin aikana.

## Projektissa haastavaa:
Alkuun pääseminen oli hankalaa ja tutustuminen itse peliin. En itse ole suuresti pelannut kyseistä peliä.
Kuitenkin halusin päästä koittamaan siihen lisätä nuo insurancet , double down ja splitin.
Splitin kanssa oli suuria ongelmia ja vielä en ole varma toimiiko siinä blackjack tapahtuma kunnolla vai ei, muut tunnosti toimivan.
Paljon uutta tuli ja Qt creator oli välillä haastava. Jossain välissä debugger lopetti täysin toimintansa ja sitä ei voinut käyttää. Heob koitin saada ,että näkisin
memory leakkejä, mutta ei sekään lähtenyt oikein toimimaan eikä näyttämään mistä niitä tulee. Kaikki tuntui haastavalta , mutta paljon uutta ajatusta tuli.

## Mitä olisin tehnyt toisin:
Tässä ei vielä osaa semmosia ajatella kunnolla. Ei ole niin paljoa tätä koodaamista tullut, mutta varmasti vähän ajan kuluttua kun palaa tähän projektiin uudestaan ja tutkii sitä
niin varmasti huomaa asioita mitkä ois kannattanu tehdä toisin. Kokonaisuutta varmasti ois voinu tarkemmin miettiä etenkin, kun tahdoin laittaa nuo splitit ja muut sinne.Split functiota olisi voinut kovastikkin miettiä eritavalla ja sitä kokonaisuutta miten tekee insurancet ja double downki sinne. Myös olisin voinut paremmin tehdä nuo pelaajan ja jakajan. Vaikka perintää hyödyntäen paremmin ihan ominan classeina varmasti olisivat toimineet hyvin.
Kaiken kaikkiaan oli itsestä mukava tämän projektin kanssa tutkia.
Aikaa tuli kyllä käytettyä omasta mielestä paljonki kyseiseen projektiin vaan tuntui ,että aina löytyi monia erilaisia vaihtoehtoja tehdä asia toisin.
Monet asiat oli omasta mielestä vähän piilossa ja niitä sai etsiä paljonkin internetin ihmeellisestä maailmasta.
