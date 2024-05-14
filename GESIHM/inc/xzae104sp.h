/*E*/
/*  Fichier : $Id: xzae104sp.h,v 1.1 1996/09/03 15:53:41 verdier Exp $      Release : $Revision: 1.1 $        Date : $Date: 1996/09/03 15:53:41 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae104sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Efface le vehicule desire
------------------------------------------------------- 
* Sequence d'appel
* SP	XZAE104_FMC_Operateur_En_Cours
* 
* Code retour
* XDC_OK
* XDC_NOK : pas de complement de fiche main courante
* XDC_ARG_INV
* <0 erreur sybase 
* 
* 
* Fonction
* Suppression du vehicule desire dans EXP et HIS
-----------------------------------------------------*/

 
#define XZAE104_FMC_Operateur_En_Cours	"XZAE;104" 
