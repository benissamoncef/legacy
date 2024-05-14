#!/bin/ksh
# Fichier : $Id: xzag09.tst,v 1.1 1994/12/06 09:13:20 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/12/06 09:13:20 $
#-----------------------------------------------------
# STERIA *  PROJET MIGRAZUR 
#-----------------------------------------------------
# SOUS-SYSTEME  OPREDD
#-----------------------------------------------------
# FICHIER xzag09.tst
#-----------------------------------------------------
# DESCRIPTION DU TEST :
#
# xzag09.tst
#
#-----------------------------------------------------
# HISTORIQUE :
#
# C.T.	09/11/94	: Creation		(V 1.1)
#-----------------------------------------------------

echo "Restauration d'une base" > xzag09.res
echo "" >> xzag09.res

echo "TEST 1 : Restauration de la base SVG" >> xzag09.res
echo "         pas de nom de fichier sur la bande" >> xzag09.res
# sauvegarde SVG en 1 fois
../src/xzag07.ksh CI /dev/rmt/0mn VolRes globale SVG trace init 2000000 

# arret pour reinserer la cassette
echo "reinserer la cassette"
read -u 

../src/xzag09.ksh CI /dev/rmt/0mn VolRes SVG trace travail
echo "" >> xzag09.res
echo "         fichier de trace lu par l'administrateur : " >> xzag09.res
cat trace >> xzag09.res
echo "" >> xzag09.res
rm trace

# arret pour reinserer la cassette
echo "reinserer la cassette"
read -u 

echo "         avec nom de fichier sur la bande" >> xzag09.res
# sauvegarde SVG en 1 fois
../src/xzag07.ksh CI /dev/rmt/0mn VolRes globale SVG trace init 2000000 sauvegardeglobale

# arret pour reinserer la cassette
echo "reinserer la cassette"
read -u 

../src/xzag09.ksh CI /dev/rmt/0mn VolRes SVG trace travail sauvegardeglobale
echo "" >> xzag09.res
echo "         fichier de trace lu par l'administrateur : " >> xzag09.res
cat trace >> xzag09.res
echo "" >> xzag09.res
rm trace

# arret pour reinserer la cassette
echo "reinserer la cassette"
read -u 

echo "TEST 2 : Restauration de la base RSSD" >> xzag09.res
../src/xzag09.ksh CI /dev/rmt/0mn VolTot RSSD trace travail >> xzag09.res
echo "" >> xzag09.res

echo "TEST 3 : device incorrect " >> xzag09.res
../src/xzag09.ksh CI /dev/rmt/mn VolTot SVG trace travail 
echo "" >> xzag09.res
echo "         fichier de trace lu par l'administrateur : " >> xzag09.res
cat trace >> xzag09.res
echo "" >> xzag09.res
rm trace

# arret pour reinserer la cassette
echo "reinserer la cassette"
read -u 

echo "TEST 4 : Restauration de la base SVG avec nom de volume incorrect" >> xzag09.res
../src/xzag09.ksh CI /dev/rmt/0mn VolTot SVG trace travail fileTape
echo "" >> xzag09.res
echo "         fichier de trace lu par l administrateur : " >> xzag09.res
cat trace >> xzag09.res
echo "" >> xzag09.res
rm trace


