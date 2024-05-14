/*E*/
/*  Fichier : $Id: xzae88sp.h,v 1.1 1995/08/04 18:06:39 gaborit Exp $      Release : $Revision: 1.1 $        Date : $Date: 1995/08/04 18:06:39 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae88sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* lecture et ecriture des fmc
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	03/08/95	: Creation
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Construire l'intitule pour la synthese des travaux 
* 
* Sequence d'appel
* SP	XZAE88_Intitule_Synthes_Trv
* 
* Arguments en entree
* XDY_Booleen	va_Prevision_in
* XDY_Autoroute	va_Autoroute_in
* XDC_PR	va_PRTete_in
* XDC_PR	va_PRQueue_in
* XDC_Sens	va_Sens_in
* 
* Arguments en sortie
* char(63)	va_Intitule_out
* 
* Code retour
* XDC_OK
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* 
* Fonction
-----------------------------------------------------------------------*/

 
#define XZAE88_Intitule_Synthes_Trv	"XZAE;88" 
