/*E*/
/*  Fichier : $Id: xzae171sp.prc,v 1.1 2004/07/16 17:13:05 gesconf Exp $      Release : $Revision: 1.1 $      Date : $Date: 2004/07/16 17:13:05 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae171sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module NOMADE : Retourne la liste des 10 derniers messages tfm  pour nomade
* Remarque :                                                                   
*
------------------------------------------------------
* HISTORIQUE :
*
* Cluchague     07/12/01  : Creation
-----------------------------------------------------*/
/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* SP	XZAE171
* 
* Arguments en entree
* 
* Arguments en sortie
*
* aucun : 
* 
* Service rendu
*         retourne les renseignements necessaires a l'etablissement des messages TFM
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

if exists (select * from sysobjects where name = 'XZAE171' and type = 'P')
	drop procedure XZAE171
go

create procedure XZAE171
as

declare
  @vl_texte_concatene        char(255),
  @vl_morceau                smallint ,
  @vl_texte                  char(250),
  @vl_derniere_action_tfm    int

   create table #TMP_LISTE_TFM
      (
	message      char(255) null
      )

/************************************************************************************************************************/
/************************************************************************************************************************/
/*                                      recuperation des infos en base                                                  */
/************************************************************************************************************************/
/************************************************************************************************************************/
select @vl_derniere_action_tfm = max(actio) from EXP..ACT_TXT_TFM

declare messages cursor
for select morceau,rtrim(texte) from EXP..ACT_TXT_TFM where actio between (@vl_derniere_action_tfm-10) and
   @vl_derniere_action_tfm order by actio desc , morceau

open messages
fetch messages into @vl_morceau, @vl_texte
select @vl_texte_concatene = null
/* tant qu'il y a des enregistrements */
while @@sqlstatus = 0
begin
  /* si premier morceau nouveau message */
  if ( @vl_morceau = 1 )
  begin
    insert #TMP_LISTE_TFM values ("********************************************************************************")
  end
    insert #TMP_LISTE_TFM values (@vl_texte)

  fetch messages into @vl_morceau, @vl_texte
end
close messages
select  rtrim(ltrim(message)) from #TMP_LISTE_TFM

drop table #TMP_LISTE_TFM

return (XDC_OK)

go
