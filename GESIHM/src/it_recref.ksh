#!/bin/ksh
#Fichier : $Id: it_recref.ksh,v 1.2 1995/08/29 18:06:07 niepceron Exp $      Release : $Revision: 1.2 $        Date : $Date: 1995/08/29 18:06:07 $
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

tar xvf /dev/rmt/c201d3m >/tmp/it_recref.log 2>&1

if [ "$?" != "0" ]
then
   echo "La Recuperation a echoue"
   more /tmp/it_recref.log
   exit -1
else
   echo "La Recuperation a reussi"
   more /tmp/it_recref.log
   exit 0 
fi

