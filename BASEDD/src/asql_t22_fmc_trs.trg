/*E*/
/*  Fichier : $Id: asql_t22_fmc_trs.trg,v 1.3 1998/03/05 11:52:44 gaborit Exp $      Release : $Revision: 1.3 $        Date : $Date: 1998/03/05 11:52:44 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER asql_t22_fmc_trs.trg
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* 
* trigger sur la table FMC_TRS
*
------------------------------------------------------
* HISTORIQUE :
*
* Torregrossa  	16 Dec 97        : Creation
* Torregrossa  	25 Fev 98        : correction update ACT_GEN (1585) (1.2)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Trannsferer les actions d'une fmc vers une autre
*
* Code retour
* 
* n�ant
*
* Conditions d'utilisation
* base EXP
* 
---------------------------------------------------*/

use EXP
go

drop trigger fmc_trs_i_trg
go

create trigger fmc_trs_i_trg
on FMC_TRS
for insert
as

   /*A 
   ** D�claration des variables locales 
   */

   declare 	@vl_NumInit 		int,
   	  	@vl_CleInit		tinyint,
   	  	@vl_NumEvt		int,
   	  	@vl_CleEvt		tinyint,
   	  	@vl_Site		tinyint,
         @vl_SiteCI		tinyint
		
   /* rechercher le site local */
   select @vl_Site = numero
   from CFG..RES_DIS
   where serveur = @@servername

   /* initialisation variable locale */
   select @vl_SiteCI = numero from CFG..RES_DIS where type = XDC_TYPEM_SD
   
   /* lire la fmc concernee */
   select 	@vl_NumEvt = numero,
   		@vl_CleEvt = cle, 
   		@vl_NumInit = num_fmc_init,
   		@vl_CleInit = cle_fmc_init
   from inserted
   
   /* Transferer les actions locales de cette fmc : pas de replication */
   /* declenchee car trigger et table sur meme base */
   update EXP..ACT_GEN set evenement = @vl_NumInit, cle = @vl_CleInit
   where evenement = @vl_NumEvt and cle = @vl_CleEvt and sit != @vl_SiteCI

go
