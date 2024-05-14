/*E*/
/*  Fichier : $Id: asql_t12_fmc_trf.trg,v 1.3 2020/11/10 15:58:03 akkaakka Exp $      Release : $Revision: 1.3 $        Date : $Date: 2020/11/10 15:58:03 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  TRFEDD
------------------------------------------------------
* FICHIER asql_t12_fmc_trf.trg
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* 
* trigger sur la table FMC_TRF
*
------------------------------------------------------
* HISTORIQUE :
*
* B.G.  15/11/94        : Creation
* C.T.  09/02/95        : Suppression de la mise a jour de zz
*                         (V 1.3)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* met a jour la colonne version pour garantir la coherence
* en cas de retard de replication
*
* Code retour
* 
* n≈ant
*
* Conditions d'utilisation
* base EXP (et HIS au CI)
---------------------------------------------------*/

use EXP
go

drop trigger fmc_trf_iu_trg
go

create trigger fmc_trf_iu_trg
on FMC_TRF
for insert,update
as

/*A D≈claration des variables locales */

	declare @vl_del 	int,
		@vl_ins 	int
			
/*A r≈cup≈ration des valeurs ins≈r≈es */
		
	select	@vl_ins=zz
	
	from inserted
	
/*A r≈cup≈ration des valeurs supprim≈es */
		
	select	@vl_del=zz
	
	from deleted
	
/*A si l'insertion est faite par une proc */
/*A si l'insertion est faite par la replication */

	if @vl_ins<=@vl_del rollback trigger
go

#ifdef HIS

use HIS
go

drop trigger fmc_trf_iu_trg
go

create trigger fmc_trf_iu_trg
on FMC_TRF
for insert,update
as

/*A D≈claration des variables locales */

	declare @vl_del 	int,
		@vl_ins 	int
			
/*A r≈cup≈ration des valeurs ins≈r≈es */
		
	select	@vl_ins=zz
	
	from inserted
	
/*A r≈cup≈ration des valeurs supprim≈es */
		
	select	@vl_del=zz
	
	from deleted
	
/*A si l'insertion est faite par une proc */
/*A si l'insertion est faite par la replication */

	if @vl_ins<=@vl_del rollback trigger
go

#endif
