#!/bin/ksh
# Fichier : $Id: xzag07.tst,v 1.1 1994/12/06 09:13:03 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/12/06 09:13:03 $
#-----------------------------------------------------
# STERIA *  PROJET MIGRAZUR 
#-----------------------------------------------------
# SOUS-SYSTEME  OPREDD
#-----------------------------------------------------
# FICHIER xzag07.tst
#-----------------------------------------------------
# DESCRIPTION DU TEST :
#
# xzag07.tst
#
#-----------------------------------------------------
# HISTORIQUE :
#
# C.T.	07/11/94	: Creation		(V 1.1)
#-----------------------------------------------------

echo "Sauvegarde d'une base" > xzag07.res
echo "" >> xzag07.res

echo "TEST 1 : Sauvegarde de la base CFG" >> xzag07.res
../src/xzag07.ksh CI /dev/rmt/0mn VolTot globale CFG trace ajoutdebut 2000000
echo "" >> xzag07.res
echo "         fichier de trace lu par l'administrateur : " >> xzag07.res
cat trace >> xzag07.res
echo "" >> xzag07.res
rm trace

echo "TEST 2 : Sauvegarde de la base RSSD" >> xzag07.res
../src/xzag07.ksh CI /dev/rmt/0mn VolTot globale RSSD trace ajout 2000000
echo "" >> xzag07.res
echo "         fichier de trace lu par l administrateur : " >> xzag07.res
cat trace >> xzag07.res
echo "" >> xzag07.res
rm trace

echo "TEST 3 : Sauvegarde de la base SVG " >> xzag07.res
../src/xzag07.ksh CI /dev/rmt/0mn VolTot globale SVG trace ajoutfin 200000
echo "" >> xzag07.res
echo "         fichier de trace lu par l'administrateur : " >> xzag07.res
cat trace >> xzag07.res
echo "" >> xzag07.res
rm trace

# arret pour reinserer la cassette
echo "reinserer la cassette"
read -u 

echo "TEST 4 : Sauvegarde incrementale de la base SVG " >> xzag07.res
../src/xzag07.ksh CI /dev/rmt/0mn VolTot incrementale SVG trace init 450 SVGincrementale
echo "" >> xzag07.res
echo "         fichier de trace lu par l administrateur : " >> xzag07.res
cat trace >> xzag07.res
echo "" >> xzag07.res
rm trace

# arret pour reinserer la cassette
echo "reinserer la cassette"
read -u 

echo "TEST 5 : Parametres erronees en entree" >> xzag07.res
../src/xzag07.ksh CI /dev/rmt/0mn VolBAS globale BAS trace 2000000 >> xzag07.res
../src/xzag07.ksh DY /dev/rmt/0mn VolS globale SVG trace 2000000 >> xzag07.res
rm trace

echo "TEST 6 : Peripherique incorrect" >> xzag07.res
../src/xzag07.ksh CI /dev/rmt/pm VolHIS globale HIS trace init 2000000
echo "" >> xzag07.res
echo "         fichier de trace lu par l administrateur : " >> xzag07.res
cat trace >> xzag07.res
echo "" >> xzag07.res
rm trace

# arret pour reinserer la cassette en lecture seule
echo "reinserer la cassette en lecture seule"
read -u

echo "TEST 7 : bande en lecture seule" >> xzag07.res
../src/xzag07.ksh CI /dev/rmt/0mn VolHIS globale HIS trace init 2000000
echo "" >> xzag07.res
echo "         fichier de trace lu par l administrateur : " >> xzag07.res
cat trace >> xzag07.res
echo "" >> xzag07.res
rm trace

