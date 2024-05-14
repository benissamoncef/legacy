#!/bin/ksh
# Fichier : $Id: xzag08.tst,v 1.1 1994/12/06 09:13:11 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/12/06 09:13:11 $
#-----------------------------------------------------
# STERIA *  PROJET MIGRAZUR 
#-----------------------------------------------------
# SOUS-SYSTEME  OPREDD
#-----------------------------------------------------
# FICHIER xzag08.tst
#-----------------------------------------------------
# DESCRIPTION DU TEST :
#
# xzag08.tst
#
#-----------------------------------------------------
# HISTORIQUE :
#
# C.T.	07/11/94	: Creation		(V 1.1)
#-----------------------------------------------------

echo "Sauvegarde des bases" > xzag08.res
echo "" >> xzag08.res

echo "TEST 1 : Sauvegarde de la base CFG" >> xzag08.res
../src/xzag08.ksh CI /dev/rmt/0mn VolCFG CFG trace 2000000 globale
echo "" >> xzag08.res
echo "         fichier de trace lu par l administrateur : " >> xzag08.res
cat trace >> xzag08.res
echo "" >> xzag08.res
rm trace

# reinserer la cassette
print "reinserer la cassette"
read -u

echo "TEST 2 : Sauvegarde de la base HIS" >> xzag08.res
../src/xzag08.ksh CI /dev/rmt/0mn VolHIS HIS trace 2000000 globale sauvegardeHISglob
echo "" >> xzag08.res
echo "         fichier de trace lu par l administrateur : " >> xzag08.res
cat trace >> xzag08.res
echo "" >> xzag08.res
rm trace

# reinserer la cassette
print "reinserer la cassette"
read -u

echo "TEST 3 : Sauvegarde des bases " >> xzag08.res
../src/xzag08.ksh CI /dev/rmt/0mn VolTou Tout trace 2000000 globale
echo "" >> xzag08.res
echo "         fichier de trace lu par l administrateur : " >> xzag08.res
cat trace >> xzag08.res
echo "" >> xzag08.res
rm trace

# reinserer la cassette
print "reinserer la cassette"
read -u

echo "TEST 4 : Sauvegarde incrementale de la base HIS " >> xzag08.res
../src/xzag08.ksh CI /dev/rmt/0mn VoSHIS HIS trace 2000000 incrementale
echo "" >> xzag08.res
echo "         fichier de trace lu par l administrateur : " >> xzag08.res
cat trace >> xzag08.res
echo "" >> xzag08.res
rm trace

echo "TEST 5 : Parametres erronees en entree" >> xzag08.res
../src/xzag08.ksh CI /dev/rmt/0mn VolTou BAS trace 2000000 globale >> xzag08.res
../src/xzag08.ksh DY /dev/rmt/0mn VolTou Tout trace 2000000 globale >> xzag08.res
echo "" >> xzag08.res

# reinserer la cassette
print "reinserer la cassette"
read -u

echo "TEST 6 : Peripherique incorrect" >> xzag08.res
../src/xzag08.ksh CI /dev/rmt/mn VolHIS HIS trace 2000000 globale
echo "" >> xzag08.res
echo "         fichier de trace lu par l administrateur : " >> xzag08.res
cat trace >> xzag08.res
echo "" >> xzag08.res
