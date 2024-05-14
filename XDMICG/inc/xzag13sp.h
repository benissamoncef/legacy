/*E*/
/*  Fichier : $Id: xzag13sp.h,v 1.1 1994/11/16 11:24:26 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/16 11:24:26 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzag13sp.prc
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
* Purge des donnees de la base d'exploitation datant
* d'au moins la date precisee situees sur le site local
* sans aucun autre controle
* 
* Sequence d'appel
* SP	XZAG13_Purge_Base_Exploitation_Rapide
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
* La derniere action et le dernier evenement sur le site local
* crees sont pas supprimes (pour eviter un doublon dans la base HIS
* lors d'une nouvelle creation)
* 
* Fonction
* Suppression des lignes datant d'au moins la date precisee
* au niveau des evenements clos, des actions finies, des SAD,
* des etats d'equipement, des alertes, des mesures situes
* sur le site local.
-------------------------------------------------------*/

 
#define XZAG13_Purge_Base_Exploitation_Rapide	"XZAG;13" 
