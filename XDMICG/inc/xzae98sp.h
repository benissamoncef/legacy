/*E*/
/*  Fichier : $Id: xzae98sp.h,v 1.2 2019/06/05 16:28:44 gesconf Exp $      Release : $Revision: 1.2 $        Date : $Date: 2019/06/05 16:28:44 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae98sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* P.V.  28/08/96: Creation
* PNI	05/06/19 :  degroupage 1.2
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Efface le vehicule desire
------------------------------------------------------- 
* Sequence d'appel
* SP	XZAE98_Supprime_Vehicule
* 
* Arguments en entree
* XDY_Entier		va_NumEvt_in
* XDY_Octet		va_CleEvt_in
* XDY_Mot		va_NumVeh_in
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

 
#define XZAE98_Supprime_Vehicule	"XZAE98" 
