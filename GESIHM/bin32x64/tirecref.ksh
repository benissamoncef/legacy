#!/bin/ksh
#  FICHIER: $Id: tirecref.ksh,v 1.1 1995/09/01 11:57:49 niepceron Exp $      Release : $Revision: 1.1 $        Date : $Date: 1995/09/01 11:57:49 $
#------------------------------------------------------
# STERIA *  PROJET MIGRAZUR 
#------------------------------------------------------
#* SOUS-SYSTEME  GESIHM
#------------------------------------------------------
#* MODULE MT * FICHIER it_recref.ksh
#------------------------------------------------------
#* DESCRIPTION DU MODULE :
#*
#*  Recuperation de l'arborescence /users/migrazur/reference/fichiers
#*  a partir d'une bande DAT
#*   
#------------------------------------------------------
#* HISTORIQUE :
#*
#* Niepceron     29 Aout  1995     : Creation
########################################################

cd /users/gesconf/migrazur/reference

tar xvf /dev/rmt/c201d3m 

if [ "$?" != "0" ]
then
   echo "La Recuperation a echoue"
   print "Taper <Entree>"
   read Reponse
   exit -1
else
   echo "La Recuperation a reussi"
   print "Taper <Entree>"
   read Reponse
   exit 0 
fi

