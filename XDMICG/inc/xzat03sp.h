/*E*/
/*  Fichier : $Id: xzat03sp.h,v 1.1 1994/11/03 14:54:01 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/03 14:54:01 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzat03sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des etats d'un equipement 
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	23/09/94	: Creation
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Positionne l'indicateur d'inhibition sur un equipement
* 
* Sequence d'appel
* SP	XZAT03_Inhiber_Eqt
* 
* Arguments en entree
* XDY_TypeEqt	va_Type_in
* XDY_Eqt	va_NumEqt_in
* 
* Arguments en sortie
* 
* Code retour
* XDC_OK
* XDC_NOK	: equipement non trouve 
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* code de ATRP18 si pb a son appel
* 
* Conditions d'utilisation
* Operateur District
* 
* Fonction
* insert INDISPONIBILITE
-----------------------------------------------------------*/

 
#define XZAT03_Inhiber_Eqt	"XZAT;03" 
