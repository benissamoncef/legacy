/*E*/
/*  Fichier : $Id: xzac70sp.prc,v 1.3 2015/09/22 09:10:04 gesconf Exp $      Release : $Revision: 1.3 $        Date : $Date: 2015/09/22 09:10:04 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac70sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
* strada : fournit les infos sur les messages TFM a exporter
------------------------------------------------------
* HISTORIQUE :
*
* Niepceron 	27/11/1998 : creation
* Niepceron	03/03/1999 : suppression des msg tfm vide v1.2
* PNI		18/09/2015 : n'inserer que les action ayant une date de succes ou de fin v1.3 DEM1145
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
*
*
* Sequence d'appel
* SP   XZAC70_Recherche_Derniers_TFM
*
* Arguments en entree
*
* Arguments en sortie
*
* Code retour
* XDC_OK
* <0 erreur sybase (voir Transact SQL p 13-15)
*
* Conditions d'utilisation
*
* Fonction
* Liste les infos TFM a exporter
-------------------------------------------------------*/

use PRC
go
drop procedure XZAC70
go

create procedure XZAC70
	
as
declare	@vl_actio int, @vl_sit tinyint,
	@vl_niveau tinyint,
	@vl_succes_lancement datetime,
	@vl_texte1 char(255),
	@vl_texte2 char(255)
 
	create table #TFM ( actio int,niveau tinyint null, succes datetime null, fin datetime null, texte1 char(255) null, texte2 char(255) null, fmc int, cle tinyint)
	create table #TFM_VID ( actio int, sit tinyint)	

	/* creation des curseurs */
	declare Pointeur_action cursor
	for select actio,sit
	from EXP..TFM_ADA

	open Pointeur_action
	fetch Pointeur_action into  @vl_actio, @vl_sit
		
	while (@@sqlstatus = 0)
	begin
		select @vl_texte1=texte from EXP..ACT_TXT_TFM 
		where actio=@vl_actio and sit = @vl_sit and morceau=1
		select @vl_texte2=texte from EXP..ACT_TXT_TFM 
		where actio=@vl_actio and sit = @vl_sit and morceau=2
		select @vl_niveau=niveau from EXP..ACT_TFM 
		where actio=@vl_actio and sit = @vl_sit
		select @vl_succes_lancement=heure_succes
		from EXP..ACT_GEN
		where numero=@vl_actio and sit = @vl_sit
		if @vl_succes_lancement=null
			select @vl_succes_lancement=heure_lancement
			from EXP..ACT_GEN
			where numero=@vl_actio and sit = @vl_sit
		
		if  not ( @vl_texte1 is null )
		begin
			insert into #TFM 
			select numero,@vl_niveau,@vl_succes_lancement,heure_fin,@vl_texte1,@vl_texte2,evenement,cle
			from EXP..ACT_GEN
			where numero=@vl_actio and sit = @vl_sit and (heure_succes is not null or heure_fin is not null)
		end
		else
		begin
			insert #TFM_VID (actio,sit) values (@vl_actio, @vl_sit)
		end

		fetch Pointeur_action into  @vl_actio, @vl_sit
	end
	
	close Pointeur_action
	
	delete EXP..TFM_ADA where actio not in (select actio from #TFM_VID )
	
	select * from #TFM
	
	return XDC_OK
go

