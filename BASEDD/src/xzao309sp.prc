/*X  Fichier : $Id: xzao309sp.prc,v 1.2 1995/04/12 18:37:20 gaborit Exp $      Release : $Revision: 1.2 $        Date : $Date: 1995/04/12 18:37:20 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzao309sp.prc
------------------------------------------------------
* DESCRIPTION DE LA PROCEDURE :
*
* Lecture et ecriture de la configuration du reseau
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	22/03/95	: Creation
* C.T.	11/04/95	: Retourner tous les canaux si
*                         pas de lt precise (V 1.2)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* retourne la liste des listes des canaux pour un local video
*
* Sequence d'appel
* SP    XZAO309_Liste_Canaux_LTV
*
* Arguments en entree
* XDY_Eqt	va_NumLTV_in
*
* Arguments en sortie
*
* Liste retournee :
* XDY_Nom		nomEqt
* char(3)		code
*
* Code retour
* XDC_OK
* XDC_ARG_INV	: parametre d'entree non value
* <0 erreur sybase
*
* Conditions d'utilisation
*
* Fonction
---------------------------------------------------*/

use PRC
go

create procedure XZAO;309
	@va_NumLTV_in	smallint = null
as
	if @va_NumLTV_in = 0 select @va_NumLTV_in = null

	/*A Recherche la liste des canaux du local video */
	if @va_NumLTV_in is not null
		select
			nom,
			code
		from CFG..EQT_LTV_CAN
		where numero = @va_NumLTV_in
	else

		select distinct
			nom,
			code
		from CFG..EQT_LTV_CAN

	return XDC_OK
go
