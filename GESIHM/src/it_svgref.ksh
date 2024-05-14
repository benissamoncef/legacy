#Fichier : $Id: it_svgref.ksh,v 1.2 1995/08/29 18:06:05 niepceron Exp $      Release : $Revision: 1.2 $        Date : $Date: 1995/08/29 18:06:05 $
#------------------------------------------------------
# STERIA *  PROJET MIGRAZUR 
#------------------------------------------------------
#* SOUS-SYSTEME  GESIHM
#------------------------------------------------------
#* MODULE MT * FICHIER  it_svgref.ksh
#------------------------------------------------------
#* DESCRIPTION DU MODULE :
#*
#*  Sauvegarde de l'arborescence /users/migrazur/reference/fichiers
#*  sur bande DAT
#*   
#------------------------------------------------------
#* HISTORIQUE :
#*
#* Niepceron     29 Aout  1995     : Creation
########################################################

cd /users/gesconf/migrazur/reference

tar cvf /dev/rmt/c201d3m /users/gesconf/migrazur/reference/fichiers >/tmp/it_svgref.log 2>&1

if [ "$?" != "0" ]
then
   echo "La Sauvegarde a echoue"
   more /tmp/it_svgref.log
   exit -1
else
   echo "La Sauvegarde a reussi"
   more /tmp/it_svgref.log
   exit 0
fi

