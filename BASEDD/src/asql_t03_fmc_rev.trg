/*E*/
/*  Fichier : $Id: asql_t03_fmc_rev.trg,v 1.14 2020/11/03 16:44:23 pc2dpdy Exp $      Release : $Revision: 1.14 $        Date : $Date: 2020/11/03 16:44:23 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER asql_t03_fmc_rev.trg
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* 
* trigger sur la table FMC_REV
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic  		03 Nov 94	: Creation
* gaborit       11 nov 94	: ajout de drop trigger (1.2)
* gaborit       20 dec 94	: numero machine -> nom (1.3)
* niepceron     03 Mar 97	: correction char(25) en char(100) 
								pour texte reveil(1.4)
* torregrossa   02 Mar 97	: Ajout des sites pc2 (1.5)
* JMG           02/10/09	: secto DEM887 1.6
* JMG			09/03/2017	: REGIONALISATION - DEM1220 1.7
* PNI			26/09/2017	: REGIONALISATION : suppression des anciens sites - DEM1220 1.8-1.10
* XXX			06/02/2018	: Utilisation du nom du serveur en table RES_DIS  1.11
* PNI			23/10/2018	: Reference a TASRV_DM  1.12 1.13
* LCL			03/11/2020	: MOVIS 1.14
* GGY			26/09/23	: Ajout district CA (DEM-483)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Cree un reveil et previent  l'IHM sur insert ou update
*
* Code retour
* 
* n�ant
*
* Conditions d'utilisation
* base EXP
---------------------------------------------------*/

use EXP
go

drop trigger fmc_rev_iu_trg
go

create trigger fmc_rev_iu_trg
on FMC_REV
for insert,update
as

   /*A 
   ** D�claration des variables locales 
   */

   declare 	@vl_Site 			tinyint,
			@vl_NumEvt			int,
			@vl_CleEvt			tinyint,
			@vl_PosteOperateur	smallint,
			@vl_TexteReveil		char(100),
			@vl_serveur			char(10),
			@vl_NomMachine		char(10),
			@vl_NomSite			char(2)
		
   
   select 	@vl_NumEvt = evenement,
   		@vl_CleEvt = cle, 
   		@vl_PosteOperateur = machine_operateur,
   		@vl_TexteReveil = texte,
   		@vl_Site = sit 
   from inserted
   
   select @vl_NomMachine=nom from CFG..EQT_GEN
   where numero=@vl_PosteOperateur
     and type=XDC_EQT_MAC

   select @vl_NomSite=code, @vl_serveur= serveur from CFG..RES_DIS
   where serveur = @@servername

   
   if @vl_serveur = XDC_SQLCI
   begin
      
      select @vl_Site = XDC_CI
      
      	 exec TASRV_CI...ARRP03 	@vl_NumEvt, 
					@vl_CleEvt, 
					@vl_NomMachine,
					@vl_NomSite, 
					@vl_TexteReveil
			
         delete FMC_REV
      
   end
   
   else

   if @vl_serveur = XDC_SQLCA
   begin
      
      select @vl_Site = XDC_CA
      
      	 exec TASRV_CA...ARRP03 	@vl_NumEvt, 
					@vl_CleEvt, 
					@vl_NomMachine,
					@vl_NomSite, 
					@vl_TexteReveil
			
         delete FMC_REV
      
   end
   
   else

   if @vl_serveur = XDC_SQLVC
   begin
      
      select @vl_Site = XDC_VC
   
      	 exec TASRV_DM...ARRP03 	@vl_NumEvt, 
					@vl_CleEvt, 
					@vl_NomMachine,
					@vl_NomSite, 
					@vl_TexteReveil
	 delete FMC_REV
      
   end    
   
   else
   
   
   

   if @vl_serveur = XDC_SQLDP
   begin
      
      select @vl_Site = XDC_DP
   
      	 exec TASRV_DP...ARRP03 	@vl_NumEvt, 
					@vl_CleEvt, 
					@vl_NomMachine,
					@vl_NomSite, 
					@vl_TexteReveil
	 delete FMC_REV
       		
   end	
go
