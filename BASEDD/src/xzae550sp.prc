/*E*/
/*  Fichier : @(#)xzae550.prc	1.0      Release : 1.0        Date : 01/04/04
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae550.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des evenements
*
------------------------------------------------------
* HISTORIQUE :
*
* F.Lizot  01/04/2004 :  Creation
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Recherche la FMC associee a un evennement DATEX
* Retourne 0,0 si la FMX n'existe pas
* 
* Sequence d'appel
* SP	XZAE550_Chercher_FMC_DATEX
* 
* Arguments en entree
* char(10)      va_emetteur_in
* char(35)      va_reference_in
*
* Arguments en sortie
* XDY_Entier	va_evenement_out
* XDY_Octet     va_cle_out
* 
* Code retour
* XDC_OK
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* 
* Fonction
* Recherche la FMC associee a un evennement DATEX
* Retourne 0,0 si la FMX n'existe pas
* 
-----------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAE550' and type = 'P')
	drop procedure XZAE550
go

create procedure XZAE550
	@va_emetteur_in         char(10)	= null,
	@va_reference_in        char(35)	= null,
	@va_evenement_out	int		= null	output,
	@va_cle_out	        tinyint		= null	output

as
	/*A controle des parametres obligatoires */
	if @va_emetteur_in = null or @va_reference_in = null 
		return XDC_ARG_INV

        /* Recherche de la FMC associee */
	select @va_evenement_out=evenement,@va_cle_out=cle from EXP..ADA_SIT
	where emetteur=@va_emetteur_in and reference=@va_reference_in and evenement<>null and cle<>null
	if @va_evenement_out=null
	   select @va_evenement_out=0
	if @va_cle_out=null
	   select @va_cle_out=0

	return XDC_OK
go
