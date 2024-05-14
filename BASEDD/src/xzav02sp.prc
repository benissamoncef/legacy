/*E*/
/*Fichier : $Id: xzav02sp.prc,v 1.1 2012/07/04 11:38:25 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2012/07/04 11:38:25 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzav02.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* VR		09/02/12 : Creation (DEM/1016)
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzav02.h" */

/* static char *version = "@(#)xzav02.c	1.1 23/18/94 : xzav02" ; */

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

if exists (select * from sysobjects where name = 'XZAV02' and type = 'P')
	drop procedure XZAV02
go

create procedure XZAV02
   @va_Basedd_in                char(3),
   @va_Debut_Visu_in		datetime,
   @va_Fin_Visu_in		datetime,
   @va_Parametrage		bit,
   @va_Fonctionnement		bit,
   @va_Detection		bit,
   @va_Ajustement		bit,
   @va_Forcage			bit,
   @va_Alerte			bit,
   @va_Reveil			bit,
   @va_Validation		bit,
   @va_Application		bit,
   @va_simulation_in		tinyint
   as 

/* 
*
* SP    XZAV02SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd        va_Basedd_in
*  					va_Debut_Visu_in
*					va_Fin_Visu_in
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
* Affichage des donnees de la table REG_TRA pour une periode choisie
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* VR    08 Fev 2012    : Creation (DEM/1016)
------------------------------------------------------*/
declare @vl_typeParam int,
	@vl_typeFonc int,
	@vl_typeDetec int,
	@vl_typeAjus int,
	@vl_typeForc int,
	@vl_typeAlerte int,
	@vl_typeReveil int,
	@vl_typeValid int,
	@vl_typeAppli int
   
   
    /*A
    ** Test des parametres d'entree :
    **   Si un des parametres d'entree est egal a null alors XDC_NOK   
    */
if(@va_Parametrage = 1)
      select @vl_typeParam = 1
if(@va_Fonctionnement = 1)
      select @vl_typeFonc = 2
if(@va_Detection = 1)
      select @vl_typeDetec = 3
if(@va_Ajustement = 1)
      select @vl_typeAjus = 4
if(@va_Forcage = 1)
      select @vl_typeForc = 5
if(@va_Alerte = 1)
     select  @vl_typeAlerte = 6
if(@va_Reveil = 1)
     select  @vl_typeReveil = 7
if(@va_Validation = 1)
     select  @vl_typeValid = 8
if(@va_Application = 1)
     select  @vl_typeAppli = 9


    if @va_Basedd_in             = null 
	
		return XDC_ARG_INV
    else   
    
    begin
	
	    /*A
	    ** Verification que le serveur est bien celui du HIS
	    */
	  
#ifdef HIST
			    select horodate,
					   ZON.nom,
					   SCE.libelle,
					   REG.libelle,
					   COND.libelle,
					   numero_fmc,
					   cle_fmc,
					   alerte,
					   acq_alerte,
					   numero_station,
					   sens_station,
					   simulation,
					   type
		
					from   HIS..REG_TRA REG,
						CFG..REG_SCE SCE, 
						CFG..REG_CON COND,
						CFG..ZON_REG ZON
								
					where   REG.scenario = SCE.numero
					and		REG.condition *= COND.numero
					and 	REG.zone = ZON.numero
					and		REG.horodate >= @va_Debut_Visu_in 
					and 	REG.horodate  <= @va_Fin_Visu_in
					and 	REG.type in (@vl_typeParam, @vl_typeFonc, @vl_typeDetec, @vl_typeAjus, @vl_typeForc, @vl_typeAlerte, @vl_typeReveil, @vl_typeValid, @vl_typeAppli)
					
					order by horodate
				return XDC_OK
#else
			  
			  if @va_simulation_in = 1
			  
				    select horodate,
						   ZON.nom,
						   SCE.libelle,
						   REG.libelle,
						   COND.libelle,
						   numero_fmc,
						   cle_fmc,
						   alerte,
						   acq_alerte,
						   numero_station,
						   sens_station,
						   simulation,
						   type
				
					from    EXP..REG_TRA REG,
						CFG..REG_SCE SCE, 
						CFG..REG_CON COND,
						CFG..ZON_REG ZON
				
					where   REG.scenario = SCE.numero
					and	REG.condition *= COND.numero
					and 	REG.zone = ZON.numero
					and	REG.simulation = @va_simulation_in
					and 	REG.type in (@vl_typeParam, @vl_typeFonc, @vl_typeDetec, @vl_typeAjus, @vl_typeForc, @vl_typeAlerte, @vl_typeReveil, @vl_typeValid, @vl_typeAppli)
					
					order by horodate
				else 
				
					select horodate,
						   ZON.nom,
						   SCE.libelle,
						   REG.libelle,
						   COND.libelle,
						   numero_fmc,
						   cle_fmc,
						   alerte,
						   acq_alerte,
						   numero_station,
					   	   sens_station,
						   simulation,
						   type
				
					from    EXP..REG_TRA REG,
						CFG..REG_SCE SCE, 
						CFG..REG_CON COND,
						CFG..ZON_REG ZON
				
					where   REG.scenario = SCE.numero
					and	REG.condition *= COND.numero
					and 	REG.zone = ZON.numero
					and	REG.simulation = @va_simulation_in
					and REG.horodate >= @va_Debut_Visu_in 
					and 	REG.horodate  <= @va_Fin_Visu_in
					and 	REG.type in (@vl_typeParam, @vl_typeFonc, @vl_typeDetec, @vl_typeAjus, @vl_typeForc, @vl_typeAlerte, @vl_typeReveil, @vl_typeValid, @vl_typeAppli)
					
					order by horodate
#endif
			     
			    return XDC_OK
    end     
go
