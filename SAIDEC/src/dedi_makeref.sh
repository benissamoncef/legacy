#Fichier : $Id: dedi_makeref.sh,v 1.1 1995/05/31 08:41:46 gesconf Exp $      Release : $Revision: 1.1 $        Date : $Date: 1995/05/31 08:41:46 $
#------------------------------------------------------
# STERIA *  PROJET MIGRAZUR 
#------------------------------------------------------
# SOUS-SYSTEME  SAIDEC
#------------------------------------------------------
# MODULE DEDI * FICHIER dedi_makeref.sh
#------------------------------------------------------
# DESCRIPTION DU MODULE :
#
# shell utilise pour mettre en refrence les fichiers de la base 
# de connaissances  et les fichiers de donnees de l'editeur
#
#------------------------------------------------------
# HISTORIQUE :
#
# guilhou	30 mai 1994	: Creation
#------------------------------------------------------*/

dedi_bdc=/produits/migrazur/appliSD/fichiers/bdc_dedi
dedi_die=/produits/migrazur/appliSD/fichiers/die


ref_bdc=/users/gesconf/migrazur/reference/fichiers/bdc
ref_die=/users/gesconf/migrazur/reference/fichiers/die

ges_bdc=/users/gesconf/migrazur/fichiers/bdc
ges_die=/users/gesconf/migrazur/fichiers/die



# copie les fichiers generes par dedi dans la reference 
/bin/cp $dedi_bdc/* $ref_bdc
/bin/cp $dedi_die/* $ref_die


/bin/cp $dedi_bdc/* $ges_bdc
/bin/cp $dedi_die/* $ges_die
