/*E*/
/* Fichier : $Id: xzao551sp.prc,v 1.1 2012/07/04 11:30:21 pc2dpdy Exp $        Release : $Revision: 1.1 $        Date : $Date: 2012/07/04 11:30:21 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzao551sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* VR		12/01/12 : Creation (DEM/1016)
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao551sp.h" */

/* static char *version = "@(#)xzao551.c	1.1 23/18/94 : xzao551" ; */

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

if exists (select * from sysobjects where name = 'XZAO551' and type = 'P')
	drop procedure XZAO551
go

create procedure XZAO551
   @va_Basedd_in             char(3),
   @va_NumeroScenario_in	smallint
   as 

/* 
*
* SP    XZAO550SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd        va_Basedd_in
*  					va_NumeroScenario_in
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf       va_Resultat_out
* 			        va_NumeroPRV_out     
* 			        va_Commande_out     
* 			        va_PRVCritique_out     
* CODE RETOUR :
*
* 0    XDC_OK
* 1    XDC_NOK 
*
*
* FONCTION :
*
* Affichage des donnees de la table REG_SCENARIO_CMD
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* VR    12 Jan 2012    : Creation (DEM/1016)
------------------------------------------------------*/

declare @vl_sens smallint
   
   
    /*A
    ** Test des parametres d'entree :
    **   Si un des parametres d'entree est egal a null alors XDC_NOK   
    */
      
	
	  
    if @va_Basedd_in             = null or
	   @va_NumeroScenario_in     = null
        
		return XDC_ARG_INV
   
if @va_Basedd_in = XDC_BASE_CFG
begin
	/*A
	** Lecture des donn?es dans REG_SCE_CMD
	*/
	
	select @vl_sens = GEN.sens
	from   CFG..REG_SCE_CMD CMD, CFG..EQT_GEN GEN, CFG..EQT_PRV PRV, CFG..REG_SCE SCEN
	where  scenario = @va_NumeroScenario_in
	and GEN.numero = prv
	and GEN.type = XDC_EQT_PRV
	and PRV.numero = CMD.prv
	and CMD.scenario = SCEN.numero
	and SCEN.zone = PRV.zone
		
	if @vl_sens = XDC_SENS_SUD OR @vl_sens = XDC_SENS_SORTIE_SUD OR @vl_sens = XDC_SENS_ENTREE_SUD
	begin
			
		select 
		CMD.prv, 
		CMD.scenario, 
		CMD.commande, 
		CMD.prv_critique,
		GEN.autoroute,
		GEN.PR,
		GEN.sens
		from   CFG..REG_SCE_CMD CMD, CFG..EQT_GEN GEN, CFG..EQT_PRV PRV, CFG..REG_SCE SCEN
		where  scenario = @va_NumeroScenario_in
		and GEN.numero = prv
		and GEN.type = XDC_EQT_PRV
		and PRV.numero = CMD.prv
		and CMD.scenario = SCEN.numero
		and SCEN.zone = PRV.zone
		ORDER BY GEN.PR ASC
				
	end
		
	if @vl_sens = XDC_SENS_NORD OR @vl_sens = XDC_SENS_SORTIE_NORD OR @vl_sens = XDC_SENS_ENTREE_NORD
	begin
		
		select 							
		CMD.prv, 
		CMD.scenario, 
		CMD.commande, 
		CMD.prv_critique,
		GEN.autoroute,
		GEN.PR,
		GEN.sens
		from   CFG..REG_SCE_CMD CMD, CFG..EQT_GEN GEN, CFG..EQT_PRV PRV, CFG..REG_SCE SCEN
		where  scenario = @va_NumeroScenario_in
		and GEN.numero = prv
		and GEN.type = XDC_EQT_PRV
		and PRV.numero = CMD.prv
		and CMD.scenario = SCEN.numero
		and SCEN.zone = PRV.zone
		ORDER BY GEN.PR DESC
			
	end
		
end   
				   
#ifdef CI      
else
begin

	select @vl_sens = sens
	from   CFT..REG_SCE_CMD CMD, CFT..EQT_GEN GEN, CFT..EQT_PRV PRV, CFT..REG_SCE SCEN
	where  scenario = @va_NumeroScenario_in
	and GEN.numero = prv
	and GEN.type = XDC_EQT_PRV
	and PRV.numero = CMD.prv
	and CMD.scenario = SCEN.numero
	and SCEN.zone = PRV.zone						   

	if @vl_sens = XDC_SENS_SUD OR @vl_sens = XDC_SENS_SORTIE_SUD OR @vl_sens = XDC_SENS_ENTREE_SUD
	begin
		
		select 
		prv, 
		scenario, 
		commande, 
		prv_critique,
		autoroute,
		PR,
		sens
		from   CFT..REG_SCE_CMD CMD, CFT..EQT_GEN GEN, CFT..EQT_PRV PRV, CFT..REG_SCE SCEN
		where  scenario = @va_NumeroScenario_in
		and GEN.numero = prv
		and GEN.type = XDC_EQT_PRV
		and PRV.numero = CMD.prv
		and CMD.scenario = SCEN.numero
		and SCEN.zone = PRV.zone
		ORDER BY PR ASC
			
	end
	
	if @vl_sens = XDC_SENS_NORD OR @vl_sens = XDC_SENS_SORTIE_NORD OR @vl_sens = XDC_SENS_ENTREE_NORD
	begin
		
		select 
		prv, 
		scenario, 
		commande, 
		prv_critique,
		autoroute,
		PR,
		sens
		from   CFT..REG_SCE_CMD CMD, CFT..EQT_GEN GEN, CFT..EQT_PRV PRV, CFT..REG_SCE SCEN
		where  scenario = @va_NumeroScenario_in
		and GEN.numero = prv
		and GEN.type = XDC_EQT_PRV
		and PRV.numero = CMD.prv
		and CMD.scenario = SCEN.numero
		and SCEN.zone = PRV.zone
		ORDER BY PR DESC
			
	end
	
end    

#endif
return XDC_OK
         
go  
