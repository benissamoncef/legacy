								/*E*/
/* Fichier : $Id: xzat067sp.prc,v 1.6 2021/06/09 10:22:18 pc2dpdy Exp $        Release : $Revision: 1.6 $        Date : $Date: 2021/06/09 10:22:18 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzat067sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des etats d'un equipement PAU SONO
*
------------------------------------------------------
* HISTORIQUE :
*
* ABE	07/04/21 	: Creation PAU SONO DEM-SAE244 1.1
* ABE	09/06/21	: Fix Suppresion recherche dispo eqt DEM-SAE244 1.6
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
*
* Retourne l'utilisation reelle d'un PAU a une horodate donnee.
* 
* Sequence d'appel
* CL	XZAT067_Utilisation_PAU
* 
* Arguments en entree
* XDY_Horodate	va_Horodate_in
* XDY_Site	va_Site_in
* 
* Arguments en sortie
* XDY_Octet  	va_DispoPAU_out
* XDY_Entier	va_NumEvt_out
* XDY_Octet	va_CleEvt_out
* XDY_Octet	va_Priorite_out
* XDY_Entier	va_DistanceEvt_out
* XDY_Entier	va_Action_out
* XDY_Site	va_SiteAction_out
* 		va_Message_out
*		va_Autoroute_out
*		va_Sens_out
*		va_PrDebut_out
*		va_PrFin_out
* 
* Code retour
* XDC_OK
* XDC_NOK
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* La procedure stockee au CI recherche les informations 
* dans la base HIS si la date donnee est anterieure de plus d'un 

* mois tandis qu'au niveau des districts la recherche s'effectue 
* dans la base EXP.
* Le code de recherche des infos de la procedure au CI et aux districts 
* doit etre identique a l'exception de la base de recherche des infos
* et du test de la date en entree (< 1 mois)
* (toute modif dans la creation de la procedure doit etre 
*  effectuee pour le CI et les districts)
* 
* Fonction
* select dans les tables EQUIPEMENT, E_PAU, ACTION et 
* INDISPONIBILITE
------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAT067' and type = 'P')
	drop procedure XZAT067
go

create procedure XZAT067
	@va_Horodate_in			datetime	= null,
	@va_Site_in			T_SITE		= null,
	@va_DispoPAU_out		tinyint		= null		output,
	@va_NumEvt_out			int		= null		output,
	@va_CleEvt_out			tinyint		= null		output,
	@va_Priorite_out		tinyint		= null		output,
	@va_DistanceEvt_out		int		= null		output,
	@va_Action_out			int		= null		output,
	@va_SiteAction_out		T_SITE		= null		output,
	@va_Message_out			char(31)	= null		output,
	@va_Autoroute_out             	tinyint         = null          output,
	@va_Sens_out                  	tinyint         = null          output,
	@va_PrDebut_out              	int	        = null          output,
	@va_PrFin_out                  	int		= null          output

as
	declare @vl_HS bit, @vl_Critique bit, @vl_Majeure bit, @vl_Inhibe bit,
		@vl_Desactive bit,  @vl_Mineure bit, @vl_NbEnregistrements int,
		@vl_Status int
	/*! transformations bugs applix */
	if @va_Horodate_in = XDC_DATE_NULLE select @va_Horodate_in = null

	/*A test les parametres obligatoires */
	if @va_Site_in = null
		return XDC_ARG_INV


	/*A date nulle : date courante */
/***************************************************************************************************************************

ENLEVER LE DATEADD --> PROBLEME PF DE DEV

****************************************************************************************************************************/
#if defined (DEV)
	if @va_Horodate_in=null select @va_Horodate_in=dateadd(hour,+2,getdate())
#endif

if @va_Horodate_in = null select @va_Horodate_in = getdate()

#ifdef HIST
		/*B lecture des infos sur le PAU */
		select * into #ACT_GEN 
		from HIS..ACT_GEN
		where HIS..ACT_GEN.sit = @va_Site_in and
		      HIS..ACT_GEN.type = XDC_ACT_PAU and
		      HIS..ACT_GEN.heure_succes <= @va_Horodate_in and
		      ( HIS..ACT_GEN.heure_fin > @va_Horodate_in or
			HIS..ACT_GEN.heure_fin is null)
		
		select 
			@va_NumEvt_out		= #ACT_GEN.evenement,
			@va_CleEvt_out		= #ACT_GEN.cle,
			@va_Priorite_out	= #ACT_GEN.priorite,
			@va_DistanceEvt_out	= #ACT_GEN.distance_evt,
			@va_Action_out		= #ACT_GEN.numero,
			@va_SiteAction_out	= #ACT_GEN.sit,
			@va_Message_out		= HIS..ACT_PAU.message,
			@va_Autoroute_out	= HIS..ACT_PAU.autoroute,
			@va_Sens_out		= HIS..ACT_PAU.sens,
			@va_PrDebut_out		= HIS..ACT_PAU.PR_debut,
			@va_PrFin_out		= HIS..ACT_PAU.PR_fin
		from #ACT_GEN, HIS..ACT_PAU
		where   HIS..ACT_PAU.actio = #ACT_GEN.numero and 
			HIS..ACT_PAU.sit = #ACT_GEN.sit and #ACT_GEN.heure_fin is not null
	
		select @vl_NbEnregistrements = @@rowcount
		if @vl_NbEnregistrements = 0
		begin
		select 
			@va_NumEvt_out		= #ACT_GEN.evenement,
			@va_CleEvt_out		= #ACT_GEN.cle,
			@va_Priorite_out	= #ACT_GEN.priorite,
			@va_DistanceEvt_out	= #ACT_GEN.distance_evt,
			@va_Action_out		= #ACT_GEN.numero,
			@va_SiteAction_out	= #ACT_GEN.sit,
			@va_Message_out         = HIS..ACT_PAU.message,
			@va_Autoroute_out       = HIS..ACT_PAU.autoroute,
			@va_Sens_out            = HIS..ACT_PAU.sens,
			@va_PrDebut_out         = HIS..ACT_PAU.PR_debut,
			@va_PrFin_out           = HIS..ACT_PAU.PR_fin

		from #ACT_GEN, HIS..ACT_PAU
		where	HIS..ACT_PAU.actio = #ACT_GEN.numero and 
			HIS..ACT_PAU.sit = #ACT_GEN.sit and #ACT_GEN.heure_fin is null
		
		end
	
		if @vl_NbEnregistrements > 1
			/*! erreur dans la base */
			return XDC_NOK
	
#else
#ifdef CI
	select "CI"
	/*! cas ou la 								procedure est creee au CI */

	/*A si la date donnee est anterieure d'un mois au moins */
 	/*A rechercher l'evenement dans la base HIS sinon dans  */
	/*A la base EXP (au niveau du CI)                       */
	if @va_Horodate_in < (select dateadd(month, -1, getdate ()))
	begin
		exec @vl_Status = SQL_HIS.PRC..XZAT067
			@va_Horodate_in, 
			@va_Site_in,
			@va_DispoPAU_out	output, 
			@va_NumEvt_out		output,
			@va_CleEvt_out		output,
			@va_Priorite_out	output,
			@va_DistanceEvt_out	output,
			@va_Action_out		output,
			@va_SiteAction_out	output,
			@va_Message_out		output,
			@va_Autoroute_out	output,
			@va_Sens_out		output,
			@va_PrDebut_out		output,
			@va_PrFin_out		output
       		if @vl_Status = null
                     	return XDC_PRC_INC
           	else return @vl_Status
	end
	else								
	begin 
		/*B lecture des infos sur PAU */
		select * into #ACT_GEN 
		from EXP..ACT_GEN
		where sit = @va_Site_in and
		      type = XDC_ACT_PAU and
		      heure_succes <= @va_Horodate_in and
		      ( heure_fin > @va_Horodate_in or
			heure_fin is null)
		select 	@va_Site_in, @va_Horodate_in

		select 
			@va_NumEvt_out		= #ACT_GEN.evenement,
			@va_CleEvt_out		= #ACT_GEN.cle,
			@va_Priorite_out	= #ACT_GEN.priorite,
			@va_DistanceEvt_out	= #ACT_GEN.distance_evt,
			@va_Action_out		= #ACT_GEN.numero,
			@va_SiteAction_out	= #ACT_GEN.sit,
			@va_Message_out         = EXP..ACT_PAU.message,
			@va_Autoroute_out       = EXP..ACT_PAU.autoroute,
			@va_Sens_out            = EXP..ACT_PAU.sens,
			@va_PrDebut_out         = EXP..ACT_PAU.PR_debut,
			@va_PrFin_out           = EXP..ACT_PAU.PR_fin

		from EXP..ACT_PAU, #ACT_GEN
		where	#ACT_GEN.sit = @va_Site_in and #ACT_GEN.type = XDC_ACT_PAU and
			EXP..ACT_PAU.actio = #ACT_GEN.numero and 
			EXP..ACT_PAU.sit = #ACT_GEN.sit and #ACT_GEN.heure_succes <= @va_Horodate_in and (#ACT_GEN.heure_fin is null or #ACT_GEN.heure_fin > @va_Horodate_in) 
	
		select @vl_NbEnregistrements = @@rowcount
													
		if @vl_NbEnregistrements > 1
			/*! erreur dans la base */
			return XDC_NOK

	end
#else
	/*! cas ou la procedure est creee sur le serveur d'un district */

	/*A lecture des infos sur PAU */
	select * into #ACT_GEN 
	from EXP..ACT_GEN
	where sit = @va_Site_in and
		      type = XDC_ACT_PAU and
		      heure_succes <= @va_Horodate_in and
		      ( heure_fin > @va_Horodate_in or
			heure_fin is null)
		
	select 								
		@va_NumEvt_out		= #ACT_GEN.evenement,
		@va_CleEvt_out		= #ACT_GEN.cle,
		@va_Priorite_out	= #ACT_GEN.priorite,
		@va_DistanceEvt_out	= #ACT_GEN.distance_evt,
		@va_Action_out		= #ACT_GEN.numero,
		@va_SiteAction_out	= #ACT_GEN.sit,
		@va_Message_out         = EXP..ACT_PAU.message,
		@va_Autoroute_out       = EXP..ACT_PAU.autoroute,
		@va_Sens_out            = EXP..ACT_PAU.sens,
		@va_PrDebut_out         = EXP..ACT_PAU.PR_debut,
		@va_PrFin_out           = EXP..ACT_PAU.PR_fin

	from #ACT_GEN, EXP..ACT_PAU
	where	#ACT_GEN.sit = @va_Site_in and
		#ACT_GEN.type = XDC_ACT_PAU and
		EXP..ACT_PAU.actio = #ACT_GEN.numero and 
		EXP..ACT_PAU.sit = #ACT_GEN.sit and #ACT_GEN.heure_succes <= @va_Horodate_in and (#ACT_GEN.heure_fin is null or #ACT_GEN.heure_fin > @va_Horodate_in) 

	select @vl_NbEnregistrements = @@rowcount


	if @vl_NbEnregistrements > 1
		/*! erreur dans la base */
		return XDC_NOK

#endif
#endif

	/*A si l'PAU n'est pas utilise alors positionner toutes */
	/*A les chaines de caracteres retournees a " " pour eviter */
	/*A les problemes de retour de null, sauf le type du picto */
	/*A qui doit etre positionne a picto vide                  */
	/*! attention conserve la valuation a null pour evt sinon  */
	/*! conservation de la valeur du parametre au 2eme appel   */
	if @vl_NbEnregistrements = 0
		select	@va_NumEvt_out 		= null, 
			@va_CleEvt_out 		= null

	/*A calcul de l'etat de l'equipement */
	select @va_DispoPAU_out = 0


	return XDC_OK
go
