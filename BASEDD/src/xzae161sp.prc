/*E*/
/*  Fichier : $Id: xzae161sp.prc,v 1.2 2009/10/05 10:40:21 gesconf Exp $      Release : $Revision: 1.2 $      Date : $Date: 2009/10/05 10:40:21 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae161sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module WEB : Recherche des PR de sortie et reinjection pour une FMC
*              delestage ou deviation passee en parametre d'entree
* Remarque :   Il subsiste un probleme dans le cas ou les echangeurs de sortie 
* et de reinjection ne sont pas sur les memes autoroutes, car on ne sait pas rechercher les
* troncons situes entre ces 2 PR sans traiter les noeuds et cie.
*
------------------------------------------------------
* HISTORIQUE :
*
* Cluchague     16/01/01  : Creation
* Cluchague     07/01/09  : V1.2 on ne retourne un resultat que si les ech de sortie et reinject sont
			    sur la meme autoroute. Sinon on retourne un PR_sortie=PR_reinjection pour
			    ne pas colorer les troncons (1=rouge 2=noir 3=orange 4=vert)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* SP	XZAE161
* 
* Arguments en entree
*  int numero_fmc    
*  tinyint cle 
* 
* Arguments en sortie
*
* aucun : 
* 
* Service rendu
*         retourne les PR de sortie et de reinjection associes a une FMC de type 
	  delestage ou deviation passe en parametre d'entree
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
drop proc XZAE161
go
create procedure XZAE161
        @va_numero_fmc int ,
	@va_cle_fmc    tinyint   ,
	@va_PR_sortie  int=null output,
	@va_PR_reinjection int=null output
as

declare @vl_numero_plan    smallint     ,
        @vl_PR_sortie      int          ,
	@vl_PR_reinjection int          ,
	@vl_portion_sortie smallint,
	@vl_portion_reinjection smallint,
	@vl_aut_sortie tinyint,
	@vl_aut_reinjection tinyint

select @vl_numero_plan=plan_associe from EXP..FMC_DEL 
				    where numero=@va_numero_fmc and cle=@va_cle_fmc

if ( @@rowcount  != 1 )  return XDC_NOK
if ( @vl_numero_plan is null or @vl_numero_plan=0) return XDC_ARG_INV


  select @vl_PR_sortie=E1.PR, @vl_PR_reinjection=E2.PR,
	 @vl_portion_sortie=E1.portion, @vl_portion_reinjection=E2.portion
     from CFG..PLN_SCR P, CFG..RES_ECH E1 ,CFG..RES_ECH E2 
     where E1.numero=P.echangeur_sortie and E2.numero=P.echangeur_reinjection and 
	   P.numero =  @vl_numero_plan


/* recherche des autoroutes des echangeurs de sortie et reinjection */
select @vl_aut_sortie = autoroute from CFG..RES_POR where numero=@vl_portion_sortie
select @vl_aut_reinjection= autoroute from CFG..RES_POR where numero=@vl_portion_reinjection
if @vl_aut_sortie != @vl_aut_reinjection 
begin
  select @vl_PR_sortie=0,
	 @vl_PR_reinjection=0
end

if @vl_PR_sortie <= @vl_PR_reinjection
select @va_PR_sortie=@vl_PR_sortie, @va_PR_reinjection=@vl_PR_reinjection
else
select @va_PR_sortie=@vl_PR_reinjection,@va_PR_reinjection=@vl_PR_sortie

return (XDC_OK)

go
