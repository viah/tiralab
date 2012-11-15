#!/bin/sh

testfile1=$(mktemp)
testfile2=$(mktemp)
outfile=$(mktemp)
expected_output=$(mktemp)


# Create some test material

cat << EOF > $testfile1
Tensori on matematiikassa tietyn tyyppinen geometrinen kokonaisuus, tai 
vaihtoehtoisesti yleinen suure. Tensorin käsitteessä yhdistyvät skalaarilla 
kertominen, vektoriavaruudet ja lineaarioperaatiot. Tensorit voidaan kirjoittaa 
koordinaatistojen avulla tai taulukkoesityksen muodossa, mutta ne on määritelty 
esitystavasta riippumatta. Ne ovat niin sanottuja multilinearikuvauksia 
vektoriavaruudelta kerroinkunnalle.

Tensorit on määritelty siten että niiden ominaisuudet säilyvät 
koordinaatistojen tavallisissa muunnoksissa. Tästä seuraa että tensorit ovat 
tärkeitä fysiikassa ja teknisillä aloilla. Erityisesti niihin törmää yleisessä 
suhteellisuusteoriassa ja hydrodynamiikassa. Tensorilaskennan tutkiminen 
muodostaa osan niin sanotusta multilineaarisesta algebrasta.
EOF

cat << EOF > $testfile2
TIG-hitsaus (engl. Tungsten Inert Gas Arc Welding) on kaasukaarihitsausprosessi,
jossa valokaari palaa sulamattoman volframielektrodin ja työkappaleen välissä 
suojakaasun ympäröimänä. Suojakaasu on useimmiten argonia, joka ei reagoi sulan 
kanssa. TIG-hitsausta voidaan tehdä lisäainetta syöttäen tai ilman lisäainetta.
Lisäaine on yleensä 1,5–3,5 mm paksu noin metrin mittainen paljas lanka, jonka 
koostumus vastaa hitsattavaa materiaalia. Käsinhitsauksessa lisäaine syötetään 
hitsisulaan käsin. Lisäaine voidaan syöttää myös koneellisesti, jolloin 
puhutaan mekanisoidusta TIG-hitsauksesta.
EOF


# Test 1
# Description: xxx todo

echo "tensorit ne" | ./wordindex -a hash $testfile1 > $outfile
echo "tensorit: $testfile1 9 67" > $expected_output
echo "ne: $testfile1 4 64" >> $expected_output

diff $outfile $expected_output > /dev/null

if [ $? -eq 0 ]
	then echo "test 1 passed"
	else echo "test 1 failed"
fi

rm $testfile1 $testfile2 $outfile $expected_output

