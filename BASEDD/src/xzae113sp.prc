/*X  Fichier : @(#)xzae113sp.prc	1.2      Release : 1.2        Date : 01/07/98
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae113sp.prc
------------------------------------------------------
* DESCRIPTION DE LA PROCEDURE :
*
* Lecture et ecriture de la configuration du reseau
*
------------------------------------------------------
* HISTORIQUE :
*
* PCL	09/07/03	: CREATION   1.2
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Rechercher la  fmc operateur en cours
* correspondante au numero de poste passe en parametre
*
* Sequence d'appel
* SP    XZAE113_FMC_Operateur_En_Cours
*
* Arguments en entree
* numero de poste SAE      
*
* Arguments en sortie
*
* XDY_Entier	numero
* XDY_Octet	cle
*
* Code retour
* XDC_OK
* <0 erreur sybase
*
* Conditions d'utilisation
*
* Fonction
---------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAE113' and type = 'P')
	drop procedure XZAE113
go

create procedure XZAE113
	@va_poste_in smallint,
	@va_numero_out int = null output,
	@va_cle_out    tinyint=null output
as

	/*A rechercher la  fmc operateur en cours correspondante */
	select
		@va_numero_out = numero,
		@va_cle_out = cle
	from EXP..FMC_GEN
	where type = XZAEC_FMC_PosteOperateur and fin is null and cloture is null and poste_enrichisseur=@va_poste_in

	return XDC_OK
go
