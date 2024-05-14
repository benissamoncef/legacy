/*E*/
/*  Fichier : @(#)xzas80sp.prc	1.0      Release : 1.0        Date : 10/01/01
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzas84sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module WEB : Gestion de la couleur des troncons web (evt) 
*
------------------------------------------------------
* HISTORIQUE :
*
* Cluchague     16/01/01  : Creation
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* SP	XZAS84
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
*         Remplit simplement le champ etat de la table WEB_TRO en
*         pour tous les troncons satisfaisant au parametres d'entree
*         avec la couleur passee en parametre d'entree
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

if exists (select * from sysobjects where name = 'XZAS84' and type = 'P')

    drop procedure XZAS84

go

create procedure XZAS84
@va_autoroute tinyint,
@va_sens      tinyint,
@va_PR_debut      int,
@va_PR_fin        int,
@va_etat      tinyint

as

declare @vl_PR int

/* si le PR de debut est apres le pr de fin on inverse les valeurs */
if @va_PR_debut > @va_PR_fin select @vl_PR = @va_PR_debut, @va_PR_debut=@va_PR_fin, @va_PR_fin=@vl_PR

/* si l'etat est noir, on ecrase le precedent etat au noir */
if ( @va_etat = XDC_WEB_NOIR )
begin
  update CFG..WEB_TRO set etat=XDC_WEB_NOIR where autoroute=@va_autoroute and sens=@va_sens and 
		      ( (PR_debut<=@va_PR_debut and PR_fin> @va_PR_debut) or
			(PR_debut>=@va_PR_debut and PR_fin<=@va_PR_fin  ) or
			(PR_debut< @va_PR_fin   and PR_fin>=@va_PR_fin  )
                      )
end

/* si l'etat est rouge, on ecrase le precedent etat au rouge uniquement s'il n'etait pas noir */
else if ( @va_etat = XDC_WEB_ROUGE )
begin
  update CFG..WEB_TRO set etat=XDC_WEB_ROUGE where autoroute=@va_autoroute and sens=@va_sens and 
		      ( (PR_debut<=@va_PR_debut and PR_fin> @va_PR_debut) or
			(PR_debut>=@va_PR_debut and PR_fin<=@va_PR_fin  ) or
			(PR_debut< @va_PR_fin   and PR_fin>=@va_PR_fin  )
                      ) and
		      etat != XDC_WEB_NOIR
end


/* si l'etat est orange, on ecrase le precedent etat a orange uniquement s'il n'etait pas noir
   ni rouge */
else if ( @va_etat = XDC_WEB_ORANGE )
begin
  update CFG..WEB_TRO set etat=XDC_WEB_ORANGE where autoroute=@va_autoroute and sens=@va_sens and 
		      ( (PR_debut<=@va_PR_debut and PR_fin> @va_PR_debut) or
			(PR_debut>=@va_PR_debut and PR_fin<=@va_PR_fin  ) or
			(PR_debut< @va_PR_fin   and PR_fin>=@va_PR_fin  )
                      ) and
		      etat not in ( XDC_WEB_NOIR , XDC_WEB_ROUGE )
end


/* si l'etat est vert on ne fait rien */

return (XDC_OK)

go

