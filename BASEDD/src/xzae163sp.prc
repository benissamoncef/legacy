/*E*/
/*  Fichier : $Id: xzae163sp.prc,v 1.3 2010/08/31 17:11:22 gesconf Exp $      Release : $Revision: 1.3 $      Date : $Date: 2010/08/31 17:11:22 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae163sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module WEB : 
* calcul du texte associe a l icone avec la priorite
*
------------------------------------------------------
* HISTORIQUE :
*
* Cluchague     16/01/01  : Creation
* Cluchague     16/01/09  : V1.2 bug on trim avant de concatener (putain, 8 ans!)
* Niepceron	30/01/09  : v1.3 limitation à 5 concatenation pour éviter les débordements
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* SP	XZAE163
* 
* Arguments en entree
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
-----------------------------------------------------------------------*/

use PRC
go
drop proc XZAE163
go
create procedure XZAE163
as

declare @vl_identifiant char(10),
	@vl_nom_vue char(50),
	@vl_niveau int,
	@vl_icone char(50),
	@vl_texte char(255),
	@vl_horodate datetime,
	@vl_priorite tinyint,
        @vl_identifiant_prec char(10),
	@vl_nom_vue_prec char(50),
	@vl_niveau_prec int,
	@vl_texte_concatene char(255),
	@vl_nombre_concatene int


/*curseur sur les evts */
declare c1 cursor for
select identifiant,nom_vue,niveau,icone,texte,horodate,priorite
from EXP..WEB_TXT
order by identifiant,nom_vue,niveau,priorite

open c1

fetch c1 into  @vl_identifiant,@vl_nom_vue,@vl_niveau,@vl_icone,@vl_texte,@vl_horodate,@vl_priorite

/* raz des variables */
select @vl_identifiant_prec="0", @vl_niveau_prec=null , @vl_nom_vue_prec="0"
select @vl_texte_concatene=null , @vl_nombre_concatene=0

while ( @@sqlstatus = 0 )
begin
  /* est-ce le premier enregistrement correspondant a ce troncon que l'on traite ?*/
  if ( @vl_identifiant != @vl_identifiant_prec or @vl_nom_vue != @vl_nom_vue_prec or
       @vl_niveau != @vl_niveau_prec )
  begin
    /* il s'agit du premier enregistrement pour ce troncon */
    
    /* d'abord on sauvegarde en base le calcul precedent uniquement si identifiant_prec != "0" */
    if @vl_identifiant_prec != "0" 
    begin
      update EXP..WEB_EVT set texte=@vl_texte_concatene where 
        identifiant=@vl_identifiant_prec and nom_vue=@vl_nom_vue_prec and niveau=@vl_niveau_prec
    end

    /* initialisation des variables */
    select @vl_identifiant_prec=@vl_identifiant , @vl_nom_vue_prec=@vl_nom_vue ,
	   @vl_niveau_prec=@vl_niveau,@vl_texte_concatene=@vl_texte ,
	   @vl_nombre_concatene = 1
    update EXP..WEB_EVT set horodate=@vl_horodate, icone=@vl_icone, texte = @vl_texte_concatene where
	    identifiant=@vl_identifiant and nom_vue=@vl_nom_vue and niveau=@vl_niveau
  end

  else
  begin
    /* il s'agit d'un enregistrement pour un troncon en cours de traitement */
    /* on concatene simplement le texte (4 au maximum)                      */
    if ( @vl_nombre_concatene < 4 ) 
    begin
      select @vl_texte_concatene = rtrim(ltrim(@vl_texte_concatene)) + "\r\n"+ rtrim(ltrim(@vl_texte))
      select @vl_nombre_concatene = @vl_nombre_concatene+1
    end
  end

  fetch c1 into @vl_identifiant,@vl_nom_vue,@vl_niveau,@vl_icone,@vl_texte,@vl_horodate,@vl_priorite

end

close c1

return (XDC_OK)

go
