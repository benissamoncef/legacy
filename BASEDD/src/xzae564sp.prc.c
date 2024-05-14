#include "/users/gesconf/migrazur/XDMICG/inc/xdc.h"
#include "/users/gesconf/migrazur/XDMICG/inc/xzaec.h"
#include "/users/gesconf/migrazur/XDMICG/inc/xzaac.h"
#include "/users/gesconf/migrazur/XDMICG/inc/xzacc.h"
#include "/users/gesconf/migrazur/XDMICG/inc/xzahc.h"
/*e*/
/* Fichier : $Id: xzae564sp.prc,v 1.5 2020/11/03 16:47:04 pc2dpdy Exp $      Release : $Revision: 1.5 $        Date : $Date: 2020/11/03 16:47:04 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae564sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* VR		30/01/12 : Creation (DEM/1016)
* JMG		23/07/12/ correction PR 1.2
* JMG		26/08/13 : scenario cible a -1 1.3
* JMG		22/10/13 : PR fin sens 1 1.4
* LCL   	22/04/20        : MOVIS Ajout site local pilotage DEM-SAE93
* PNI		15/06/21	: Suppression des doublons avant insertion 1.6
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzae564sp.h" */

/* static char *version = "@(#)xzae564.c	1.1 23/18/94 : xzae564" ; */

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

if exists (select * from sysobjects where name = 'XZAE564' and type = 'P')
	drop procedure XZAE564
go

create procedure XZAE564
   @va_Numero_in		int,
   @va_Cle_in		tinyint,
   @va_Scenario_in		smallint,
   @va_Contexte_in		smallint,
   @va_Indicateur_Horaire_in		smallint,
   @va_Mesure_in		smallint,
   @va_Transitoire_in		char(200),
   @va_Zone_in			smallint,
   @va_PosteOperateur_in   T_EQUIPEMENT,
   @va_Horodate_in			datetime = null, -- par defaut heure courante
   @va_Scenario_cible_in		smallint,
   @va_NomSiteLocal_in             char(2)         = null
   as 
   
/* 
*
* SP    XZAE564SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd        
*					va_Numero_in
*					va_Cle_in
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
* Mise a jour des donnees de la table FMC_REG
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* VR    13 Jan 2012    : Creation (DEM/1016)
------------------------------------------------------*/
declare @vl_Site T_SITE, @vl_typeSite tinyint,
	@va_ConfirmeSignale_in bit, @va_Validation_in datetime,
	@va_NumAutoroute_in T_AUTOROUTE, @va_PR_in int,
	@va_Sens_in tinyint, @va_TypePtCaracteristique_in tinyint, @va_PtCaracteristique_in tinyint,
	@vl_Position tinyint, @va_VR_in tinyint, @va_VM2_in tinyint, @va_VM1_in tinyint, @va_VL_in tinyint,
	@va_BAU_in tinyint, @va_VR_I_in tinyint, @va_VM2_I_in tinyint, @va_VM1_I_in tinyint, @va_VL_I_in tinyint,
	@va_BAU_I_in tinyint, @va_BAU_Etroite_in tinyint,  @va_BAU_I_Etroite_in tinyint, 
	@va_TypeAnterieur_in smallint, @va_Type_in smallint, @va_Longueur_in int,
	@vl_horodate datetime,
	@vl_last_horo datetime, @vl_scenario smallint,
	@vl_prf int, @vl_prd int

	/*! transfo en parametres nulls bug applix */
	if @va_Numero_in = 0 select @va_Numero_in = null
	if @va_Cle_in = 0 select @va_Cle_in = null

	if @va_Horodate_in = null select @va_Horodate_in = getdate()

	/*A controle parametres en entree */
	if @va_Numero_in  = null or @va_Cle_in = null
		return XDC_ARG_INV

	/*A recherche du site local */
        /* MOVIS */
        if @va_NomSiteLocal_in = null
		select @vl_Site = numero, @vl_typeSite = type
                from CFG..RES_DIS
                where serveur = @@servername
        else
		select @vl_Site = numero, @vl_typeSite = type
                from CFG..RES_DIS
                where code = @va_NomSiteLocal_in


	if @vl_typeSite = XDC_TYPEM_PCS
		return XDC_NOK

	select @vl_scenario = @va_Scenario_in
	if @va_Scenario_in =  -1
		select @vl_scenario = @va_Scenario_cible_in
	if @vl_scenario < 0
		select @vl_scenario = -@vl_scenario

	/*A verifier l'existance de l'evenement et la concordance */
	/*A du poste enrichisseur                                 */
	if (@va_PosteOperateur_in != 0)
	begin
		if not exists ( select * from EXP..FMC_GEN 
			where	numero = @va_Numero_in and 
				cle = @va_Cle_in and
				(poste_enrichisseur = @va_PosteOperateur_in or
				 poste_enrichisseur = null) and
				sit = @vl_Site)
			return XZAEC_FMC_PAS_ENR
	end

	select @vl_last_horo = max(horodate) from EXP..FMC_REG
	where numero = @va_Numero_in and
	cle = @va_Cle_in

	if @@rowcount = 0
	begin
		select  @vl_last_horo =  @va_Horodate_in
	end
	else
	begin
		select  @vl_last_horo = dateadd(second,1,@va_Horodate_in)
	end

	/*PATCH*/
	if @va_Scenario_cible_in < 0
begin
	delete EXP..FMC_REG where numero=@va_Numero_in and cle=@va_Cle_in and horodate= @vl_last_horo

	insert into EXP..FMC_REG (numero,
			   cle,
			   scenario, 
			   contexte,
			   mesure,
			   transitoire,
			   zone,
			   indicateur_horaire,
			   horodate,sit,
			   scenario_cible)
	values (@va_Numero_in,
			@va_Cle_in,
			@vl_scenario, 
			@va_Contexte_in, 
			@va_Mesure_in, 
			@va_Transitoire_in,
			@va_Zone_in, 
			@va_Indicateur_Horaire_in,
			@vl_last_horo,@vl_Site,
			-@va_Scenario_cible_in)
	return XDC_OK
end
	/*mise a jour de FMC_GEN*/
	update EXP..FMC_GEN
	set zz=zz+1
	where numero = @va_Numero_in and cle = @va_Cle_in

	/*recup du dernier enregistrement dans FMC_HIS*/
	select @va_ConfirmeSignale_in = confirme_ou_signale, 
		@va_NumAutoroute_in = autoroute,  @va_PR_in = PR,
		@va_Sens_in =sens, @va_TypePtCaracteristique_in = point_caracteristique, @va_PtCaracteristique_in = numero_point_caracteristique,
		@vl_Position = position_sur_aire_ou_echangeur, @va_VR_in = VR, @va_VM2_in = VM2, @va_VM1_in = VM1, @va_VL_in=VL,
		@va_BAU_in=BAU, @va_VR_I_in=VR_I, @va_VM2_I_in=VM2_I, @va_VM1_I_in=VM1_I, @va_VL_I_in=VL_I,
		@va_BAU_I_in=BAU_I, @va_BAU_Etroite_in=BAU_etroite,  @va_BAU_I_Etroite_in = BAU_I_etroite,
		@va_TypeAnterieur_in=type_anterieur, @va_Type_in=type_FMC_valide, @va_Longueur_in=longueur,
		@vl_horodate = horodate_validation
	from EXP..FMC_HIS
	where numero = @va_Numero_in and cle = @va_Cle_in
	and horodate_validation in (select max(horodate_validation) from EXP..FMC_HIS
		where numero = @va_Numero_in and cle = @va_Cle_in)

	/*A modification de la fiche Regulation */
	delete EXP..FMC_REG where numero=@va_Numero_in and cle=@va_Cle_in and horodate= @vl_last_horo
	insert into EXP..FMC_REG (numero,
			   cle,
			   scenario, 
			   contexte,
			   mesure,
			   transitoire,
			   zone,
			   indicateur_horaire,
			   horodate,sit,
			   scenario_cible)
	values (@va_Numero_in,
			@va_Cle_in,
			@vl_scenario, 
			@va_Contexte_in, 
			@va_Mesure_in, 
			@va_Transitoire_in,
			@va_Zone_in, 
			@va_Indicateur_Horaire_in,
			@vl_last_horo,@vl_Site,
			@va_Scenario_cible_in)
			 
	if @vl_horodate != @va_Horodate_in
	begin
		select @va_Sens_in = sens, @va_NumAutoroute_in = autoroute, @vl_prd = debut, @vl_prf = fin
		from CFG..ZON_REG
		where numero = @va_Zone_in

		if @va_Sens_in = XDC_SENS_SUD
			select @va_PR_in = @vl_prd
		else
			select @va_PR_in = @vl_prf
	
		insert EXP..FMC_HIS ( numero, cle, confirme_ou_signale,
			horodate_validation, autoroute, PR, sens, point_caracteristique,
			numero_point_caracteristique,position_sur_aire_ou_echangeur,
			VR, VM2, VM1, VL, BAU, VR_I, VM2_I, VM1_I, VL_I, BAU_I,
			BAU_etroite, BAU_I_etroite, sit,
			type_anterieur, type_FMC_valide, longueur)
		values( @va_Numero_in, @va_Cle_in,
			XDC_FMC_ETAT_TRAI, @vl_last_horo,
			@va_NumAutoroute_in, @va_PR_in,
			@va_Sens_in, @va_TypePtCaracteristique_in, @va_PtCaracteristique_in,
			@vl_Position, isnull(@va_VR_in, XDC_VOIE_INCONNUE), isnull(@va_VM2_in,XDC_VOIE_INCONNUE), 
			isnull(@va_VM1_in,XDC_VOIE_INCONNUE), isnull(@va_VL_in,XDC_VOIE_INCONNUE),
			isnull(@va_BAU_in, XDC_VOIE_INCONNUE), isnull(@va_VR_I_in, XDC_VOIE_INCONNUE), 
			isnull(@va_VM2_I_in, XDC_VOIE_INCONNUE), 
			isnull(@va_VM1_I_in, XDC_VOIE_INCONNUE),
			isnull(@va_VL_I_in, XDC_VOIE_INCONNUE),
			isnull(@va_BAU_I_in, XDC_VOIE_INCONNUE), @va_BAU_Etroite_in,  @va_BAU_I_Etroite_in, @vl_Site,
			XZAEC_FMC_Regulation, XZAEC_FMC_Regulation, @va_Longueur_in)
	end


go
