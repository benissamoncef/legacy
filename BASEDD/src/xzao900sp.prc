/*E*/
/* Fichier : $Id: xzao900sp.prc,v 1.1 2012/07/04 11:36:06 pc2dpdy Exp $        Release : $Revision: 1.1 $        Date : $Date: 2012/07/04 11:36:06 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzao900sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* VR		11/05/12 : Creation (DEM/1016)
------------------------------------------------------*/

/* static char *version = "@(#)xzao900.c	1.1 23/18/94 : xzao900" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC

go

if exists (select * from sysobjects where name = 'XZAO900' and type = 'P')
	drop procedure XZAO900
	
go

create procedure XZAO900
   @va_Basedd_in             char(3)
   as 

/* 
*
* SP    XZAO900SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd        va_Basedd_in
*
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf       va_Resultat_out
*
* CODE RETOUR :
*
* 0    XDC_OK
* 1    XDC_NOK 
*
*
* FONCTION :
*
* Import des donnees de la fenetre de configuration des scenarios
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* VR    11 Mai 2012    : Creation (DEM/1016)
------------------------------------------------------*/
declare
	@vl_id_sce smallint,
	@vl_libelle_sce varchar(60),
	@vl_nominal_sce bit,
	@vl_degrade_sce bit,
	@vl_niveau_sce smallint,
	@vl_inhibition_sce bit,
	@vl_zone_sce smallint,	   
    @vl_autoroute_prv varchar(50),
    @vl_num_prv smallint,
    @vl_pr_prv integer,
    @vl_sens_prv tinyint,
    @vl_vitesse_nomi_prv smallint, 
    @vl_critique_prv bit,
    @vl_type_prv tinyint,
    @vl_zone_prv smallint,
    @vl_bretelle_prv bit,
    @vl_rappel_nomi_prv bit,
    @vl_libelle_zone varchar(60),
    @vl_num_zone smallint,
    @vl_autoroute_zone smallint,
   	@vl_sens smallint,
	@vl_debut int,
	@vl_fin int,
	@vl_prv_cmd smallint, 
	@vl_sce_cmd smallint, 
	@vl_vitesse_prv smallint 
	
	
	/*A
    ** Creation d une table tampon
    */
   
   CREATE TABLE #SYNTHESE (
   base 				varchar(3)		null,
   zone_id 				smallint		null, 
   zone_libelle 		varchar(60)		null, 
   scenario_id 			smallint		null, 
   scenario_libelle 	varchar(60)		null,
   scenario_niveau 		smallint		null,
   scenario_nominal 	bit				not null,
   scenario_inhibition 	bit				not null,
   scenario_degrade 	bit				not null,
   prv_id 				smallint		null,
   prv_nom 				varchar(10)		null,
   prv_autoroute 		smallint		null,
   prv_pr 				integer			null,
   prv_sens 			tinyint			null,
   prv_vitesse 			smallint		null,
   prv_critique 		bit				not null,
   prv_vitesse_nominal 	smallint		null,
   prv_rappel_nominal 	smallint		null,
   prv_type 			tinyint			null,
   cond_id           	smallint		null,
   cond_type_horaire	smallint       	null,
   cond_jour            tinyint        	null,
   cond_horodate_debut	datetime       	null,
   cond_horodate_fin    datetime       	null,
   cond_heure_debut	 	smallint       	null,
   cond_minute_debut    smallint       	null,
   cond_heure_fin	 	smallint       	null,	
   cond_minute_fin	 	smallint       	null,
   cond_cond_alternative 	smallint       	null,
   cond_seuil		 	int            	null,
   cond_donnee_trafic   smallint       	null,
   cond_operateur	 	tinyint        	null,
   cond_scenario        smallint       	null,
   cond_conjonction1	smallint       	null,
   cond_conjonction2    smallint       	null,
   cond_conjonction3	smallint       	null,
   cond_conjonction4    smallint       	null,
   cond_station		 	smallint       	null,
   cond_zone		 	smallint 		null,
   cond_type_jour 	 	smallinT 		null,
   cond_libelle		 	varchar(100)	null,
   cond_sens		 	tinyint 		null,
   cond_condition_simple 	tinyint 		null
   )
	
    /*A
    ** Test des parametres d'entree :
    **   Si un des parametres d'entree est egal a null alors XDC_NOK   
    */
      
    if @va_Basedd_in             = null
		return XDC_ARG_INV

        /*A
        ** Si le serveur est celui du CI ou DS
        */
        
	   
	        /*A
	        **  Travail dans la base CFG
	        */   
	      
			if @va_Basedd_in = XDC_BASE_CFG
	            
				begin
				
				insert #SYNTHESE
				select 	@va_Basedd_in,
						ZON.numero,
						ZON.nom,
						SCE.numero,
						SCE.libelle,
						SCE.niveau,
						SCE.nominal,
						SCE.inhibition,
						SCE.degrade,
						PRV.numero,
						GEN.nom,
						GEN.autoroute,
						GEN.PR,
						GEN.sens,
						CMD.commande,
						CMD.prv_critique,
						PRV.vitesse_nominale,
						PRV.rappel_nominal,
						PRV.type,
						null,
						null,
						null,
						null,
						null,
						null,
						null,
						null,
						null,
						null,
						null,
						null,
						null,
						null,
						null,
						null,
						null,
						null,
						null,
						null,
						null,
						null,
						null,
						null		
				from CFG..ZON_REG ZON, 
				     CFG..REG_SCE SCE, 
				     CFG..REG_SCE_CMD CMD,
				     CFG..EQT_GEN GEN,
				     CFG..EQT_PRV PRV
				where ZON.numero = SCE.zone 
				  and SCE.numero = CMD.scenario
				  and CMD.prv = GEN.numero
				  and GEN.type = XDC_EQT_PRV
				  and PRV.numero = CMD.prv
	
	
				insert #SYNTHESE
				select 	@va_Basedd_in,
						ZON.numero,
						ZON.nom,
						SCE.numero,
						SCE.libelle,
						SCE.niveau,
						SCE.nominal,
						SCE.inhibition,
						SCE.degrade,
						null,
						null,
						null,
						null,
						null,
						null,
						0,
						null,
						null,
						null,
	    				CON.numero,
	    				CON.type_horaire,
						CON.jour,
				    	CON.horodate_debut,
						CON.horodate_fin,
				    	CON.heure_debut,
						CON.minute_debut,
						CON.heure_fin,
				    	CON.minute_fin,
						CON.cond_alternative,
				    	CON.seuil,
						CON.donnee_trafic,
				    	CON.operateur,
						CON.scenario,
				    	CON.conjonction1,
						CON.conjonction2,
				    	CON.conjonction3,
						CON.conjonction4,
				    	CON.station,
				    	CON.zone,
				    	CON.type_jour,
				    	CON.libelle,
				    	CON.sens,
				    	CON.condition_simple
				from CFG..ZON_REG ZON, 
				     CFG..REG_SCE SCE,
				     CFG..REG_ASS ASS,
				     CFG..REG_CON CON 
				where ZON.numero = SCE.zone 
				  and SCE.numero = ASS.scenario
				  and ASS.numero=CON.numero			 
	
				select * from #SYNTHESE
	           end  
   		
#ifdef CI		
		    /*A
	        **  Travail dans la base CFT
	        */   
	      
		else	if @va_Basedd_in = XDC_BASE_CFT
            
			begin
				
				insert #SYNTHESE
				select 	@va_Basedd_in,
						ZON.numero,
						ZON.nom,
						SCE.numero,
						SCE.libelle,
						SCE.niveau,
						SCE.nominal,
						SCE.inhibition,
						SCE.degrade,
						PRV.numero,
						GEN.nom,
						GEN.autoroute,
						GEN.PR,
						GEN.sens,
						CMD.commande,
						CMD.prv_critique,
						PRV.vitesse_nominale,
						PRV.rappel_nominal,
						PRV.type,
						null,
						null,
						null,
						null,
						null,
						null,
						null,
						null,
						null,
						null,
						null,
						null,
						null,
						null,
						null,
						null,
						null,
						null,
						null,
						null,
						null,
						null,
						null,
						null		
				from CFT..ZON_REG ZON, 
				     CFT..REG_SCE SCE, 
				     CFT..REG_SCE_CMD CMD,
				     CFT..EQT_GEN GEN,
				     CFT..EQT_PRV PRV
				where ZON.numero = SCE.zone 
				  and SCE.numero = CMD.scenario
				  and CMD.prv = GEN.numero
				  and GEN.type = XDC_EQT_PRV
				  and PRV.numero = CMD.prv
	
	
				insert #SYNTHESE
				select 	@va_Basedd_in,
						ZON.numero,
						ZON.nom,
						SCE.numero,
						SCE.libelle,
						SCE.niveau,
						SCE.nominal,
						SCE.inhibition,
						SCE.degrade,
						null,
						null,
						null,
						null,
						null,
						null,
						0,
						null,
						null,
						null,
	    				CON.numero,
	    				CON.type_horaire,
						CON.jour,
				    	CON.horodate_debut,
						CON.horodate_fin,
				    	CON.heure_debut,
						CON.minute_debut,
						CON.heure_fin,
				    	CON.minute_fin,
						CON.cond_alternative,
				    	CON.seuil,
						CON.donnee_trafic,
				    	CON.operateur,
						CON.scenario,
				    	CON.conjonction1,
						CON.conjonction2,
				    	CON.conjonction3,
						CON.conjonction4,
				    	CON.station,
				    	CON.zone,
				    	CON.type_jour,
				    	CON.libelle,
				    	CON.sens,
				    	CON.condition_simple
				from CFT..ZON_REG ZON, 
				     CFT..REG_SCE SCE,
				     CFT..REG_ASS ASS,
				     CFT..REG_CON CON 
				where ZON.numero = SCE.zone 
				  and SCE.numero = ASS.scenario
				  and ASS.numero=CON.numero			 
	
				select * from #SYNTHESE
           end  
#endif
	  
go  
