/*E*/
/*  Fichier : $Id: xzag14sp.h,v 1.1 1994/11/16 11:24:19 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/16 11:24:19 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzag14sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Purge des bases
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	15/11/94	: Creation	(V 1.1)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Purge des donnees de la base d'historique datant
* d'au moins la date precisee situees sur le site local
* sans aucun autre controle
* 
* Sequence d'appel
* SP	XZAG14_Purge_Base_Historique_Rapide
* 
* Arguments en entree
* XDY_Datetime	va_Horodate_in	: date de fin de purge
* XDY_Octet	va_Mode_in	: XZAGC_IMP_ENR/XZAGC_IMP_NBR
* 
* Arguments en sortie
* XDC_Booleen 	va_Resultat_out : XDC_OUI si suppression realisee
* 
* Code retour
* XDC_OK
* XDC_ARG_INV	: parametre d'entree null
* 
* Conditions d'utilisation
* Supprimer tout jusqu'a la date "va_Horodate_in" en
* imprimant le nombre de suppression par table (XZAGC_IMP_NBR)
* ou l'enregistrement supprime (XZAGC_IMP_ENR)
* (par defaut c'est le nombre de suppression par table qui est
* affiche)
* 
* Fonction
* Suppression des lignes datant d'au moins la date precisee
* au niveau des evenements clos, des actions finies, 
* des etats d'equipement, des alertes, des mesures situes
* sur tous les sites.
-------------------------------------------------------*/

 
#define XZAG14_Purge_Base_Historique_Rapide	"XZAG;14" 
