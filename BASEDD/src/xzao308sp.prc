/*X  Fichier : $Id: xzao308sp.prc,v 1.2 1995/03/22 16:05:04 gaborit Exp $      Release : $Revision: 1.2 $        Date : $Date: 1995/03/22 16:05:04 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzao308sp.prc
------------------------------------------------------
* DESCRIPTION DE LA PROCEDURE :
*
* Lecture et ecriture de la configuration du reseau
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	22/03/95	: Creation (V 1.1, 1.2)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Suprimer un canal pour un local video existant
*
* Sequence d'appel
* SP    XZAO308_Suprimer_CanalVideo
*
* Arguments en entree
* XDY_Eqt	va_NumLTV_in
* XDY_Nom	va_NomCanal_in
* char(3)	va_CodeCanal_in
*
* Arguments en sortie
*
* Code retour
* XDC_OK
* XDC_NOK	: canal video inexistant
* XDC_ARG_INV	: arguments invalides
* <0 erreur sybase
*
* Conditions d'utilisation
* Tous les parametres d'entree doivent correspondre a un canal video
*
* Fonction
---------------------------------------------------*/

use PRC
go

create procedure XZAO;308
	@va_NumLTV_in		smallint = null,
	@va_NomCanal_in		T_NOM = null,
	@va_CodeCanal_in	char(3) = null
as
	if @va_NumLTV_in = 0 select @va_NumLTV_in = null
	if @va_NomCanal_in = XDC_CHAINE_VIDE select @va_NomCanal_in = null
	if @va_CodeCanal_in = XDC_CHAINE_VIDE select @va_CodeCanal_in = null

	/*A Controle la validite des parametres d'entree */
	if @va_NumLTV_in = null or @va_NomCanal_in = null or 
	   @va_CodeCanal_in = null
		return XDC_ARG_INV

	/*A Inserer le canal video */
	delete CFG..EQT_LTV_CAN 
	where	numero = @va_NumLTV_in and nom = @va_NomCanal_in and
		code = @va_CodeCanal_in

	if @@rowcount = 0
		return XDC_NOK

	return XDC_OK
go
