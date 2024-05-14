/*E*/
/*  Fichier : @(#)xzae553.prc	1.0      Release : 1.0        Date : 01/04/04
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae553.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des evenements
*
------------------------------------------------------
* HISTORIQUE :
*
* F.Lizot  14/04/2004 :  Creation
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Cree un evennement DATEX associe a une FMC ESCOTA
* 
* Sequence d'appel
* SP	XZAE553_Creer_Evenement_DATEX
* 
* Arguments en entree
* char(10)      va_emetteur_in
* char(35)      va_reference_in
* XDY_Entier	va_evenement_in
* XDY_Octet     va_cle_in
*
* Arguments en sortie
* 
* Code retour
* XDC_OK
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* 
* Fonction
* Insere dans la table ADA_SIT un evenement liee a la FMC ESCOTA passe en parametres
* 
-----------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAE553' and type = 'P')
	drop procedure XZAE553
go

create procedure XZAE553
	@va_emetteur_in         char(10)	= null,
	@va_reference_in        char(35)	= null,
	@va_evenement_in	int		= 0,
	@va_cle_in	        tinyint		= 0

as
	/*A controle des parametres obligatoires */
	if @va_emetteur_in = null or @va_reference_in = null or @va_evenement_in = 0 or @va_cle_in = 0
		return XDC_ARG_INV

	if not exists (select * from EXP..ADA_SIT where emetteur=@va_emetteur_in and reference=@va_reference_in)
	if exists (select * from EXP..FMC_GEN where numero=@va_evenement_in and cle=@va_cle_in)
	   insert into EXP..ADA_SIT (emetteur,reference,type,evenement,cle)
	   values (@va_emetteur_in,@va_reference_in,"FMC ESCOTA",@va_evenement_in,@va_cle_in)

	return XDC_OK
go
