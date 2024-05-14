/*E*/
/*  Fichier : $Id: xzae180sp.prc,v 1.1 2007/03/26 15:18:07 gesconf Exp $      Release : $Revision: 1.1 $        Date : $Date: 2007/03/26 15:18:07 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae180sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Inserer  un morceau de texte message WEB Crise
* 
* Sequence d'appel
* SP	XZAE180_Lit_Texte_Crise
* 
* Arguments en entree
* XDY_Entier		va_Ident_in
* 
* Arguments en sortie
* 
* Code retour
* XDC_OK
* XDC_NOK	: 
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* Il faut que l'actio existe deja : utiliser avant XZAC19
* 
* Fonction
* Inserer dans la table WEB_TXT_CRI

-------------------------------------------------------*/

use PRC
go

create procedure XZAE180
	@va_Ident_in	int	= null,
	@va_Texte_in		T_TEXTE	= null
as
	declare @vl_Morceau smallint, @vl_Status int,
		@vl_Texte T_TEXTE

	/*! transfo en paramÉtres nulls bug applix */
	if @va_Ident_in = 0 select @va_Ident_in = null

	/*A controle des parametres obligatoires */
	if @va_Ident_in = null 
		return XDC_ARG_INV


	/*A verifier l'existence de l'actio */
	if not exists ( select * from EXP..WEB_CRI
			where identifiant = @va_Ident_in)
		return XDC_NOK

	if not exists (select * from EXP..WEB_TXT_CRI
			where identifiant = @va_Ident_in)
		select " "
	else
		select texte from EXP..WEB_TXT_CRI
		where identifiant = @va_Ident_in
		order by morceau asc
	return XDC_OK

go
