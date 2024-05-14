/*E*/
/* Fichier : $Id: xzae560sp.prc,v 1.3 2020/11/03 16:46:49 pc2dpdy Exp $      Release : $Revision: 1.3 $        Date : $Date: 2020/11/03 16:46:49 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae560sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* VR		31/01/12 : Creation (DEM/1016)
* JMG		23/07/12 : correction PR 1.2
* LCL   	22/04/20        : MOVIS Ajout site local pilotage DEM-SAE93
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzae560sp.h" */

/* static char *version = "@(#)xzae560.c	1.1 23/18/94 : xzae560" ; */

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

if exists (select * from sysobjects where name = 'XZAE560' and type = 'P')
	drop procedure XZAE560
go

	create procedure XZAE560
	
	   @va_Autoroute_in			 	tinyint,
	   @va_Sens_in			 		tinyint,
	   @va_PR_Debut_in			 	int,
	   @va_Pr_Fin_in			 	int,
	   @va_Contexte_in			 	smallint,
	   @va_Scenario_in			 	smallint,
	   @va_Scenario_cible_in			 	smallint,
	   @va_IndicateurHoraire_in		bit,
	   @va_Mesure_in				smallint,
	   @va_Transitoire_in			char(200),
	   @va_Zone_in				smallint,
	   @va_numAlerte_in		int,
	   @va_Num_FMC_out         		int             	output,
	   @va_Cle_FMC_out         		tinyint             output,
	   @va_NomSiteLocal_in             char(2)         = null

	   as 

/* 
*
* SP    XZAE560SP
*
* PARAMETRES EN ENTREE :
*
*					va_Autoroute_in
*					va_Sens_in
*					va_PR_Debut_in
*					va_Pr_Fin_in
*					va_Contexte_in
*					va_Scenario_in
*					va_Scenario_cible_in
*					va_IndicateurHoraire_in
*					va_Mesure_in
*					va_Transitoire_in
*	
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf       va_Resultat_out    

* CODE RETOUR :
*
* 0    XDC_OK
* 1    XDC_NOK 
*
*
* FONCTION :
*
* Insertion dans la table FMC_REG
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* VR    13 Jan 2012    : Creation (DEM/1016)
------------------------------------------------------*/
declare
   
   /*A
   ** Declaration des variables locales
   */
   

   @vl_Site 	T_SITE,
   @vl_DateCourante datetime,
   @vl_Status int,
   @vl_PtCaracteristique tinyint, @vl_TypePtCaracteristique tinyint,
		@vl_Autoroute T_AUTOROUTE, @vl_PR T_PR, @vl_Sens T_SENS,
		@vl_Position tinyint, @vl_VR T_VOIE, @vl_VM2 T_VOIE, 
		@vl_VM1 T_VOIE, @vl_VL T_VOIE, @vl_BAU T_VOIE, 
		@vl_VR_I T_VOIE, @vl_VM2_I T_VOIE, @vl_VM1_I T_VOIE, 
		@vl_VL_I T_VOIE, @vl_BAU_I T_VOIE, @vl_ResultConfig int,
  		@vl_District T_DISTRICT,
		@vl_DistrictRecouvrement T_DISTRICT,
 		@vl_BAU_I_Etroite_in			tinyint,
		@vl_BAU_Etroite_in		tinyint
		
   	/*A test de la date de debut prevu */
	select @vl_DateCourante = getdate ()
	select @vl_DateCourante = dateadd(millisecond, -datepart(millisecond,@vl_DateCourante),@vl_DateCourante)
	

    begin
      
        	/*A recherche du site local servant de cle a l'evenement */
					
	        /* MOVIS */
        	if @va_NomSiteLocal_in = null
	                select  @vl_Site = numero
	                from CFG..RES_DIS
	                where serveur = @@servername
	        else
	                select  @vl_Site = numero
	                from CFG..RES_DIS
	                where code = @va_NomSiteLocal_in

			if @vl_Site is null
				return XDC_NOK			
											
			/*A
			** Creation de la fiche de Regulation
			*/					
			begin
				exec @vl_Status = XZAE08 XZAEC_FMC_Regulation, null, XDC_OUI,
						@vl_DateCourante, null,@vl_Site, XDC_NON, @va_Num_FMC_out output,
						@va_Cle_FMC_out output
		

				if @vl_Status = null
					select @vl_Status = XDC_PRC_INC
				if @vl_Status != XDC_OK
				begin
					return @vl_Status
				end
			end
			
		exec @vl_Status = XZAE66 @va_Autoroute_in, @va_PR_Debut_in, @va_Sens_in,
				null, XDC_POINT_CARACT_AUCUN,
				null, XZAEC_FMC_Regulation,@vl_Autoroute output,
				@vl_PR output, @vl_Sens output, @vl_PtCaracteristique output,
				@vl_TypePtCaracteristique output, @vl_Position output,
				@vl_VR output, @vl_VM2 output, @vl_VM1 output,
				@vl_VL output, @vl_BAU output, 
				@vl_VR_I output, @vl_VM2_I output, @vl_VM1_I output,
				@vl_VL_I output, @vl_BAU_I output, 
				@vl_BAU_Etroite_in output, @vl_BAU_I_Etroite_in output,
				@vl_District output, @vl_DistrictRecouvrement output,
				@vl_ResultConfig output
				
			/*A Validation de la fiche Regulation */
			exec @vl_Status = XZAE11 @va_Num_FMC_out, 
								@va_Cle_FMC_out, 
								null,
								XDC_FMC_ETAT_TRAI, /*va_Prevision_Traiter_Clos_in*/
								XZAEC_FMC_Regulation,
								null, /*va_DebutPrevu_in*/
								@vl_DateCourante, /*va_Debut_in*/
								null, 
								null, 
								@vl_DateCourante, /*va_Validation_in*/
								null, /*va_NumCause_in*/
								null, /*va_CleCause_in*/
								@va_numAlerte_in, 
								null, 
								null, 
								XZAEC_FMC_CONFIRMEE,
								@va_Autoroute_in, 
								@va_PR_Debut_in, 
								@va_Sens_in, 
								null, XDC_POINT_CARACT_AUCUN, null,
								@vl_VR, @vl_VM2, @vl_VM1, 
								@vl_VL, @vl_BAU, @vl_VR_I,
								@vl_VM2_I, @vl_VM1_I, @vl_VL_I, 
								@vl_BAU_I, null, null,
								XDC_FAUX, 
								XZAEC_FMC_Regulation, /*va_TypeAnterieur_in*/
								XDC_FAUX, /*fausse alerte*/
								null,
								null,
								@va_Cle_FMC_out,
								@va_NomSiteLocal_in
				
			if @vl_Status = null
				select @vl_Status = XDC_PRC_INC
			if @vl_Status != XDC_OK
				rollback tran


			/*A Ecriture du complement de la fiche Regulation */
			/*! le poste operateur est mis a 1 juste pour l'appel de la procedure */
			
			exec @vl_Status = XZAE564 @va_Num_FMC_out , @va_Cle_FMC_out , 
					@va_Scenario_in, @va_Contexte_in, 
					@va_IndicateurHoraire_in, @va_Mesure_in, 
					@va_Transitoire_in,@va_Zone_in,
					0,@vl_DateCourante	, @va_Scenario_cible_in, @va_NomSiteLocal_in
				select @vl_Status
					
			if @vl_Status = null
			
			begin
				select @vl_Status = XDC_PRC_INC
				return @vl_Status
			end
			
			else if @vl_Status != XDC_OK
			begin
				rollback tran
				return @vl_Status
			end
		

			commit tran
	
				
	end
	
	return @vl_Status
go
