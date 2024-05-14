/*E*/
/*  Fichier : $Id: xzae150sp.prc,v 1.1 1999/02/26 14:22:16 gaborit Exp $      Release : $Revision: 1.1 $        Date : $Date: 1999/02/26 14:22:16 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae150sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
* strada : infos d'une situation strada
------------------------------------------------------
* HISTORIQUE :
*
* Niepceron	12 Jan 1999 : creation
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
*
*
* Sequence d'appel
* SP   XZAE150_Message_Datex
*
* Arguments en entree
*
* Arguments en sortie
*
* Code retour
* XDC_OK
* <0 erreur sybase (voir Transact SQL p 13-15)
*
* Conditions d'utilisation
*
* Fonction
* Lecture des des infos obligatoires pour generer
* un fichier STRADA
-------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAE150' and type = 'P')
	drop procedure XZAE150
go

create procedure XZAE150
	@va_NumEvt_in 		int=null,
	@va_CleEvt_in 		int=null,
	@va_emetteur_out		T_NOM=null output,
	@va_Texte1_out 		char(250)=null output,
	@va_Texte2_out 		char(100)=null output
	
as
declare @vl_emetteur 		char(10),
	@vl_reference  	char(35)
	 
	 
	
	if @va_NumEvt_in  = null or @va_CleEvt_in = null
	   return XDC_ARG_INV
	 
	 
	select
		@vl_emetteur =emetteur,
		@vl_reference =reference
	from EXP..ADA_SIT
	where evenement=@va_NumEvt_in and cle=@va_CleEvt_in 
	
	select
		@va_Texte1_out =texte1,
		@va_Texte2_out =texte2
	from EXP..ADA_SIT
	where emetteur= @vl_emetteur and lien=@vl_reference
	
	select @va_emetteur_out=rtrim(nom)
	from CFG..RES_DIS
	where serveur=@vl_emetteur
	
	return XDC_OK
go

