/*E*/
/*  Fichier : $Id: xzae162sp.prc,v 1.3 2008/11/17 11:11:29 pc2dpdy Exp $      Release : $Revision: 1.3 $      Date : $Date: 2008/11/17 11:11:29 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae162sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module WEB : 
* recherche des troncons pour les evts WEB 
*
------------------------------------------------------
* HISTORIQUE :
*
* Cluchague     16/01/01  : Creation
* Cluchague     21/03/03  : Traitement du sens 0 (v1.2)
* JMG		22/08/08 : le texte passe a 200 caracteres 1.3 DEM 836
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* SP	XZAE162
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

if exists (select * from sysobjects where name = 'XZAE162' and type = 'P')
	drop procedure XZAE162
go

create procedure XZAE162
		@va_aut_in tinyint,
		@va_sens_in tinyint,
		@va_PR_in int,
		@va_pt_car_in tinyint,
		@va_num_pt_car_in tinyint,
		@va_horodate_in datetime,
		@va_texte_in char(200),
		@va_icone_in char(30),
		@va_priorite_in tinyint,
		@va_type_web_in tinyint
as

declare @vl_identifiant char(10),
        @vl_id char(10),
	@vl_sens tinyint,
	@vl_X int, @vl_Y int, @vl_facteurX int, @vl_facteurY int, @vl_rotation int,
	@vl_nom_vue char(100),@vl_niveau int

create table #WEB_ID (
	identifiant char(10) null,
	niveau tinyint null,
	nom_vue char(100) null,
	X int null,
	Y int null,
	facteurX int null,
	facteurY int null,
	rotation int null)

  select  @vl_sens=@va_sens_in   
  if      (@va_sens_in=XDC_SENS_SORTIE_NORD or
	  @va_sens_in=XDC_SENS_ENTREE_NORD)
  select  @vl_sens=XDC_SENS_NORD

  else if      (@va_sens_in=XDC_SENS_SORTIE_SUD or
	  @va_sens_in=XDC_SENS_ENTREE_SUD)
  select  @vl_sens=XDC_SENS_SUD
  else if      @va_sens_in=XDC_SENS_INCONNU
	  select @vl_sens=XDC_SENS_NORD

/*si evt sur aire*/
if @va_pt_car_in = XDC_POINT_CARACT_AIRE
begin
  insert into #WEB_ID
  select identifiant+"_a" ,
	 niveau,
	 nom_vue,
	 X,
	 Y,
	 facteurX,
	 facteurY,
	 rotation
  from CFG..WEB_AIR 
  where numero = @va_num_pt_car_in
end
/*si evt sur echangeur*/
else if @va_pt_car_in = XDC_POINT_CARACT_ECHANGEUR
begin
  insert into #WEB_ID
  select identifiant ,
	 niveau,
	 nom_vue,
	 X,
	 Y,
	 facteurX,
	 facteurY,
	 rotation
  from CFG..WEB_ECH 
  where numero = @va_num_pt_car_in and sens=@vl_sens
end
else
/* evt ailleurs*/
begin
  /*recherche du troncon de debut pour l evenement*/
  insert into #WEB_ID
  select identifiant ,
	 niveau,
	 nom_vue,
	 X,
	 Y,
         facteurX,
         facteurY,
	 rotation
  from CFG..WEB_TRO
  where autoroute = @va_aut_in
	and sens = @vl_sens
	and PR_debut <= @va_PR_in
	and PR_fin > @va_PR_in
end

/*curseur sur les identifiants recuperes*/
declare c1 cursor for
select identifiant,
	niveau,
	nom_vue,
	X,
	Y,
	facteurX,
	facteurY,
	rotation
from #WEB_ID

open c1

fetch c1 into @vl_identifiant,
		@vl_niveau,
		@vl_nom_vue,
		@vl_X,@vl_Y,@vl_facteurX,@vl_facteurY,@vl_rotation

while @@sqlstatus = 0
begin
/*regarde si l'evt est deja dans WEB_EVT*/
select @vl_id = identifiant from EXP..WEB_EVT where identifiant = @vl_identifiant
				and niveau = @vl_niveau
				and nom_vue = @vl_nom_vue
if @@rowcount = 0
begin
  /*insertion dans WEB_EVT*/
  insert EXP..WEB_EVT (identifiant,niveau,nom_vue,X,Y,facteurX,facteurY,rotation,icone,texte,horodate,type_web)
  values (@vl_identifiant,@vl_niveau,@vl_nom_vue,
	  @vl_X,@vl_Y,@vl_facteurX,@vl_facteurY,@vl_rotation,
	  null,null,null,@va_type_web_in)

  /*insertion dans WEB_TXT*/
  insert EXP..WEB_TXT (identifiant,niveau,nom_vue,icone,texte,horodate,priorite, flag)
  values (@vl_identifiant,@vl_niveau,@vl_nom_vue,
		@va_icone_in,@va_texte_in,@va_horodate_in,@va_priorite_in,0)
end
else
begin
  /*insertion uniquement dans WEB_TXT*/
  insert EXP..WEB_TXT (identifiant,niveau,nom_vue,icone,texte,horodate,priorite, flag)
  values (@vl_identifiant,@vl_niveau,@vl_nom_vue,
		@va_icone_in,@va_texte_in,@va_horodate_in,@va_priorite_in,0)
end

fetch c1 into @vl_identifiant,
		@vl_niveau,
		@vl_nom_vue,
		@vl_X,@vl_Y,@vl_facteurX,@vl_facteurY,@vl_rotation
end

close c1

drop table #WEB_ID


return (XDC_OK)

go
