#Fichier : $Id: dedi_getref.sh,v 1.1 1995/05/31 08:41:37 gesconf Exp $      Release : $Revision: 1.1 $        Date : $Date: 1995/05/31 08:41:37 $
#------------------------------------------------------
# STERIA *  PROJET MIGRAZUR 
#------------------------------------------------------
# SOUS-SYSTEME  SAIDEC
#------------------------------------------------------
# MODULE DEDI * FICHIER dedi_makeref.sh
#------------------------------------------------------
# DESCRIPTION DU MODULE :
#
# shell utilise pour recuperer la reference des fichiers de la base 
# de connaissances  et des fichiers de donnees de l'editeur
#
#------------------------------------------------------
# HISTORIQUE :
#
# guilhou	30 mai 1994	: Creation
#------------------------------------------------------*/

ref_bdc=/users/gesconf/migrazur/reference/fichiers/bdc
ref_die=/users/gesconf/migrazur/reference/fichiers/die

ope_bdc=/produits/migrazur/appliSD/fichiers/bdc_dedi
ope_die=/produits/migrazur/appliSD/fichiers/die

# copie les versions de reference dans l'espace operationnelle
/bin/cp $ref_bdc/* $ope_bdc
/bin/cp $ref_die/* $ope_die


