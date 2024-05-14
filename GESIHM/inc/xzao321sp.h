/*E*/
/*  Fichier : $Id: xzao321sp.h,v 1.1 1995/12/13 11:16:21 gaborit Exp $      Release : $Revision: 1.1 $        Date : $Date: 1995/12/13 11:16:21 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzao321sp.prc
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
* SP	XZAO321_Ecrire_Vitesse_Libre
* 
* Arguments en entree
* XDY_Mot	va_NumEqt_in
* XDY_Sens	va_Sens_in
* XDY_Vitesse	va_Vitesse_in
* 
* Arguments en sortie
* 
* Service rendu
* modifier  la vitesse libre d'une station ou l'initialiser
* 
* Code retour
* XDC_OK
* XDC_NOK	: si la station n'existe pas
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* 
* Fonction
-----------------------------------------------------------------------*/

 
#define XZAO321_Ecrire_Vitesse_Libre	"XZAO;321" 
