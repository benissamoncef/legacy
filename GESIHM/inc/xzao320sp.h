/*E*/
/*  Fichier : $Id: xzao320sp.h,v 1.1 1995/12/13 11:16:14 gaborit Exp $      Release : $Revision: 1.1 $        Date : $Date: 1995/12/13 11:16:14 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzao320sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* lecture et ecriture des descriptions des equipements 
* et du reseau
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	11/12/95	: Creation
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* SP	XZAO320_LISTE_VTL
* 
* Arguments en entree
* 
* Arguments en sortie
* 
* liste retournee :
* XDY_Mot	va_NumEqt
* XDY_Sens	va_Sens
* XDY_Vitesse	va_Vitesse

* Service rendu
* Recherche la liste des vitesses libres
* 
* Code retour
* XDC_OK
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* 
* Fonction
-----------------------------------------------------------------------*/

 
#define XZAO320_LISTE_VTL	"XZAO;320" 
