#Fichier : @(#)it_svgref.ksh	1.2      Release : 1.2        Date : 08/29/95
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
#* Niepceron     09 Nov   1995	: Deplacement de Reponse="" pour recup code erreur du tar v1.2
########################################################

cd /users/gesconf/migrazur/reference

tar cvf /dev/rmt/c201d3m /users/gesconf/migrazur/reference/fichiers 

if [ "$?" != "0" ]
then
   Reponse=""
   echo "La Sauvegarde a echoue"
   echo "Taper <Entree>"
   read  Reponse
   exit -1
else
   Reponse=""
   echo "La Sauvegarde a reussi"
   echo "Taper <Entree>"
   read  Reponse
   exit 0
fi

