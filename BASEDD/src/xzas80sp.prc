/*E*/
/*  Fichier : @(#)xzas80sp.prc	1.0      Release : 1.0        Date : 10/01/01
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzas80sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module WEB : Gestion de la couleur des troncons web      
*
------------------------------------------------------
* HISTORIQUE :
*
* Cluchague     16/01/01  : Creation
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* SP	XZAS;80
* 
* Arguments en entree
* smallint	station
* tinyint	sens
* 
* Arguments en sortie
*
* aucun : 
* 
* Service rendu
*         Remplit simplement le champ etat de la table WEB_TRO
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
-----------------------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAS80' and type = 'P')

    drop procedure XZAS80

go

create procedure XZAS80
as

declare @vl_station             smallint,
	@vl_sens    	        tinyint,
	@vl_niveau    		tinyint

/* mise a 0 de l'etat de tous les troncons */ 
update CFG..WEB_TRO set etat=XDC_WEB_VERT

/* declaration du curseur */
declare pointeur cursor
  for select station,sens,niveau
      from EXP..MES_NIV_DER

/* ouverture du curseur */
open pointeur
fetch pointeur into @vl_station,@vl_sens,@vl_niveau

while (@@sqlstatus = 0)
begin
    /* si le niveau est sature, on ecrase l'etat du troncon a rouge */
    if @vl_niveau = XDC_RADT_SATURE  
      update CFG..WEB_TRO set etat=XDC_WEB_ROUGE where 
      sens = @vl_sens  and 
      ( station1 = @vl_station or
	station2 = @vl_station or
	station3 = @vl_station or
	station4 = @vl_station or
	station5 = @vl_station )

    /* si le niveau est pre sat, on ecrase l'etat du troncon a orange s'il n'est pas deja rouge */
    else if @vl_niveau = XDC_RADT_PRESAT
      update CFG..WEB_TRO set etat=XDC_WEB_ORANGE where
      sens = @vl_sens  and 
      etat != XDC_WEB_ROUGE and
      ( station1 = @vl_station or
	station2 = @vl_station or
	station3 = @vl_station or
	station4 = @vl_station or
	station5 = @vl_station)

  fetch pointeur into @vl_station,@vl_sens,@vl_niveau

end

close pointeur

return (XDC_OK)
go

