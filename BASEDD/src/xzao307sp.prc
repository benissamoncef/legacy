/*X  Fichier : $Id: xzao307sp.prc,v 1.2 1995/03/22 16:05:02 gaborit Exp $      Release : $Revision: 1.2 $        Date : $Date: 1995/03/22 16:05:02 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzao307sp.prc
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
* inserer un canal pour un local video existant
*
* Sequence d'appel
* SP    XZAO307_Ecrire_CanalVideo
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
* XDC_NOK	: local video inexistant ou insertion impossible
* XDC_ARG_INV	: arguments invalides
* <0 erreur sybase
*
* Conditions d'utilisation
* Le nom du canal et son code doivent etre values et differents de " "
* La description donnee ne doit pas correspondre a un canal existant
* pour un meme local video
*
* Fonction
---------------------------------------------------*/

use PRC
go

create procedure XZAO;307
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

	/*A Verifier la presence du local video */
	if not exists ( select * from CFG..EQT_LTV where numero = @va_NumLTV_in)
		return XDC_NOK

	/*A Verifier l'absence d'un canal ayant la meme description que celui a inserer */
	if exists ( select * from CFG..EQT_LTV_CAN where numero = @va_NumLTV_in and 
							nom = @va_NomCanal_in and code = @va_CodeCanal_in)
		return XDC_NOK

	/*A Inserer le canal video */
	insert CFG..EQT_LTV_CAN (numero, nom, code)
	values( @va_NumLTV_in, @va_NomCanal_in, @va_CodeCanal_in)

	if @@rowcount != 1
		return XDC_NOK

	return XDC_OK
go
