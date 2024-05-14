/*E*/
/*  Fichier : $Id: asql_t05_fmc_gen.trg,v 1.4 2020/11/10 15:57:56 akkaakka Exp $      Release : $Revision: 1.4 $        Date : $Date: 2020/11/10 15:57:56 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER asql_t05_fmc_gen.trg
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* 
* trigger sur la table FMC_GEN
*
------------------------------------------------------
* HISTORIQUE :
*
* B.G.  15/11/94        : Creation
* C.T.  09/02/95        : Suppression de la mise a jour de zz
*                         (V 1.3)
* GGY   09/03/23        : Ajout appel a AARPDATEX2 sur Insert/update (DEM-SAE438)
* LCL	20/06/23		: Ajout test sur poste enrichisseur : on n'envoie pas la FMC en Datex2 si c'est une reservation ou une liberation de FMC
* LCL	06/10/23 : Adaptations CNA DEM-483 : lecture site principal en base
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* met a jour la colonne version pour garantir la coherence
* en cas de retard de replication
*
* Code retour
* 
* neant
*
* Conditions d'utilisation
* base EXP (et HIS au CI)
---------------------------------------------------*/

#ifdef HIST

use HIS
go

drop trigger fmc_gen_iu_trg
go

create trigger fmc_gen_iu_trg
on FMC_GEN
for insert,update
as

/*A Declaration des variables locales */

	declare @vl_del 	int,
		@vl_ins 	int
			
/*A recuperation des valeurs inserees */
		
	select	@vl_ins=zz
	
	from inserted
	
/*A recuperation des valeurs supprimees */
		
	select	@vl_del=zz
	
	from deleted
	
/*A si l'insertion est faite par une proc */
/*A si l'insertion est faite par la replication */

	if @vl_ins<=@vl_del rollback trigger
go

#else

use EXP
go

drop trigger fmc_gen_iu_trg
go

create trigger fmc_gen_iu_trg
on FMC_GEN
for insert,update
as

/*A Declaration des variables locales */

	declare @vl_del 	int,
		@vl_ins 	 	int,
		@vl_site	    char(7),
		@vl_numero		int,
		@vl_cle			tinyint,
		@vl_poste_enrichisseur_new smallint,
		@vl_poste_enrichisseur_old smallint, 
		@vl_SiteCICA T_SITE

	/* Verification du site CI ou CA */
	select  @vl_SiteCICA = numero
		from CFG..RES_DIS
		where type = XDC_TYPEM_SD
			
/*A recuperation des valeurs inserees */
		
	select	@vl_ins=zz,
			@vl_numero=numero,
			@vl_cle=cle,
			@vl_poste_enrichisseur_new=poste_enrichisseur
	from inserted
	
/*A recuperation des valeurs supprimees */
		
	select	@vl_del=zz,
			@vl_poste_enrichisseur_old=poste_enrichisseur
	from deleted
	
/*A si l'insertion est faite par une proc */
/*A si l'insertion est faite par la replication */

	if @vl_ins<=@vl_del rollback trigger

	select @vl_site=serveur_pra from CFG..RES_PRA where numero = @vl_SiteCICA
	/* La creation d'une FMC passe par xzae08 puis par xzae11, on ne prends donc pas en compte la 1ere modification ==> @vl_ins > 1 */
	/* Ajout test poste enrichisseur : la reservation et la liberation de la FMC consiste à modifier uniquement le poste enrichisseur */
	/* il ne faut pas envoyer la FMC en Datex2 lors de ces operations */
	if @vl_site = @@servername and @vl_ins>1 and @vl_poste_enrichisseur_old=@vl_poste_enrichisseur_new
	begin
		if @vl_site = XDC_NOM_SITE_CI
			exec TASRV_CI...AARPDATEX2 	@vl_numero, @vl_cle
		else if @vl_site = XDC_NOM_SITE_CA
			exec TASRV_CA...AARPDATEX2 	@vl_numero, @vl_cle
	end

go

#endif
