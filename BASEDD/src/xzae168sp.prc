/*E*/
/*  Fichier : $Id: xzae168sp.prc,v 1.3 2011/09/20 16:12:49 gesconf Exp $      Release : $Revision: 1.3 $        Date :  $Date: 2011/09/20 16:12:49 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae168sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module WEB : 
*
* Ajouter un nouveau message dans WEB_CRI
*  (et marquer le precedent pour suppression)
*
------------------------------------------------------
* HISTORIQUE :
*
* Guilhou     16/01/01  : Creation
* Hilmarcher  08/07/03  : modif pour suppression message ant≈rieur avant insert v1.2
* JPL		12/09/11 : Support 2e portion de message pour longueur de 500 car. (DEM 993)  1.3
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* SP	XZAE168
* 
* Arguments en entree
*   char(200)	va_titre_in
*   XDY_Octet	va_delai_in
*   XDY_Octet	va_local_in
*   char(255)	va_message_in
*   char(255)	va_message2_in
* 
* Arguments en sortie
*
* aucun : 
* 
* Service rendu
* 
* Code retour
* XDC_OK
* XDC_NOK	:
* XDC_ARG_INV	: parametres d'entree null ou site incorrect
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* 
* Fonction
*  Recherche le numero d'identifiant de message maximal ;
*  marque ce message pour suppression et insere le nouveau message
*  avec l'identifiant immediatement superieur.
-----------------------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAE168' and type = 'P')
	drop procedure XZAE168
go


create procedure XZAE168
	@va_titre_in		char(200) = null,
	@va_delai_in		tinyint = null,
	@va_local_in		tinyint = null,
	@va_message_in		char(255)= null,
	@va_message2_in		char(255)= null
as

declare @vl_id int,
	@vl_date datetime

select @vl_date = getdate()

select @vl_id = max(identifiant) from EXP..WEB_CRI
if @@rowcount = 0 select @vl_id = 0
if @vl_id = NULL select @vl_id = 0

update EXP..WEB_CRI
set suppression=1 where identifiant=@vl_id

select @vl_id = @vl_id + 1
insert EXP..WEB_CRI (
	identifiant,
	suppression,
	local,
	delai,
	titre,
	message,
	message2,
	horodate
)
values (@vl_id,
	0,
	@va_local_in,
	@va_delai_in,
	@va_titre_in,
	@va_message_in,
	@va_message2_in,
	@vl_date)

return (XDC_OK)

go
