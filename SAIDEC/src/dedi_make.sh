#Fichier : $Id: dedi_make.sh,v 1.3 1995/11/15 14:39:08 gesconf Exp $      Release : $Revision: 1.3 $        Date : $Date: 1995/11/15 14:39:08 $
#------------------------------------------------------
# STERIA *  PROJET MIGRAZUR 
#------------------------------------------------------
# SOUS-SYSTEME  SAIDEC
#------------------------------------------------------
# MODULE DEDI_MAKE * FICHIER dedi_make.sh
#------------------------------------------------------
# DESCRIPTION DU MODULE :
#
# shell utilise pour generer les fichiers de la base 
# de connaissances a partir des sorties de l'editeur
#
#------------------------------------------------------
# HISTORIQUE :
#
# thomas	30 nov 1994	: Creation
# thomas	15 nov 1995	: Modif path bdc (1.2)
# thomas	15 nov 1995	: Modif path bdc (1.3)
#------------------------------------------------------*/

bdc=../modif/fichiers/bdc

# verifie le nombre d'arguments
if (test $# != 4)
then
	echo "syntaxe : dedi_make.sh Entete PrefixeFichierIn NomFichierOut Mode"
	exit 1
fi

# detruit le fichier cible et le fichier de travail
/bin/rm -f $bdc/$3
/bin/rm -f $bdc/tmp_$2

# concatene l'entete et les fichiers elementaires dans le fichier de travail
# supprime les lignes vides
cat $1 $bdc/$2*.tie | sed "/^$/d" >> $bdc/tmp_$2

# detruit les fichiers elementaires et le fichier d'entete
/bin/rm -f $bdc/$2*.tie
/bin/rm -f $1

# renomme le fichier de travail en fichier cible
/bin/mv -f $bdc/tmp_$2 $bdc/$3

# si on a genere des fichiers texte
if (test $4 = 2)
then
  #on les imprime au fur et a mesure de leur generation
  lp -olandscape -ohalf $bdc/$3
fi

