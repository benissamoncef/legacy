/*E*/
/*  Fichier : $Id: xzac35sp.prc,v 1.11 2010/08/30 15:58:18 gesconf Exp $      Release : $Revision: 1.11 $        Date : $Date: 2010/08/30 15:58:18 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac35sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	08/09/94	: Creation
* C.T.	13/09/94	: Modif parametre en entree
*                         (HoroLancFin remplace par HoroFin)
* C.T.	10/11/94	: Modif bugs applix (V 1.3)
* C.T.	14/11/94	: Modif recherche infos pour CI (V 1.4)
* C.T.	09/12/94	: Modif clause de lecture d'action (V 1.5)
*                         non teste
* P.V.  11/07/96	: Ajout colonnes (V1.6) (DEM76)
* P.N.  19/09/96	: Correction sur horo depart  - arrivee v1.7
* JMG	02/12/96	: flag indiquant HIS ou EXP (DEM/1207) 1.8
* JMG	04/12/96	: ajout site responsable (DEM/1256) 1.9
* C.P.	10/06/98	: Separation EXP et HIS (V 1.10)
* JPL	21/07/10	: Au CI ou sur secteur, si l'action est introuvable, executer sur HIS (DEM 941) 1.11
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Lecture d'une intervention d'un depanneur
* 
* Sequence d'appel
* SP	XZAC35_Lire_Intervention_Depanneur
* 
* Arguments en entree
* XDY_Entier		va_NumAction_in
* XDY_Octet		va_Site_in
*
* Arguments en sortie
* XDY_Nom		va_NomDepanneur_out
* XDY_Nom		va_Resultat_out
* XY_Horodate		va_HoroAppel_out
* XY_Horodate		va_HoroArrivee_out
* XDY_Operateur		va_Operateur_out
* XY_Horodate		va_HoroDepart_out
* XY_Horodate		va_HoroAnnul_out
* XDY_Octet		va_renfort_out
* XDY_Octet		va_patrouille_presente_out
* XDY_Octet		va_garag_contact_pc_out
* XDY_Octet		va_annul_intervention_out
* XDY_Entier		va_code_type_astreinte_out
* XDY_Entier		va_interlocuteur_out
* XDY_Entier		va_fmc_purgee_out
* XDY_Octet		va_site_responsable
*
* Code retour
* XDC_OK
* XDC_NOK
* 
* Conditions d'utilisation
* La procedure stockee au CI recherche les informations 
* dans la base HIS si les informations n'existent pas dans
* EXP tandis qu'au niveau des districts la recherche s'effectue 
* dans la base EXP.
* Le code de recherche d'infos de la procedure au CI et aux districts 
* doit etre identique a l'exception de la base de recherche des infos
* (toute modif dans la creation de la procedure doit etre 
*  effectuee pour le CI et les districts)
*
* Suite a un ajout de colonnes dans ACT_DEP, les champs renfort, patrouille_presente,
* garagiste_doit_contacter_pc, annulation_intervention n'ont pu etre codes en booleens.
* Des controles sont donc necessaires dans le code appelant cette procedure pour que les
* valeurs de ces champs soient 0 ou 1 uniquement.
* 
* Fonction
* Select dans les tables ACTION, INTERVENTION_DEPANNEUR
------------------------------------------------*/

use PRC
go

create procedure XZAC;35
	@va_NumAction_in		int	    = null,
	@va_Site_in			T_SITE	    = null,
	@va_NomDepanneur_out		T_NOM	    = null	output,
	@va_Resultat_out		T_NOM	    = null	output,
	@va_HoroAppel_out		datetime    = null	output,
	@va_HoroArrivee_out		datetime    = null	output,
	@va_Operateur_out		T_OPERATEUR = null	output,
	@va_HoroDepart_out		datetime    = null	output,
	@va_HoroAnnul_out		datetime    = null	output,
	@va_renfort_out			tinyint	    = null  	output,
	@va_patrouille_presente_out 	tinyint	    = null  	output,
	@va_garag_contact_pc_out 	tinyint     = null 	output,
	@va_annul_intervention_out 	tinyint     = null  	output,
	@va_code_type_astreinte_out 	int	    = null 	output,
	@va_interlocuteur_out 		int	    = null 	output,
	@va_fmc_purgee_out		tinyint	    = null	output,
	@va_site_responsable_out	T_SITE	    = null	output
as

	declare	@vl_Status	int

	/*! transfo en param…tres nulls bug applix */
	if @va_NumAction_in = 0 select @va_NumAction_in = null
	if @va_Site_in = 0 select @va_Site_in = null

	/*A Controle des parametres d'entree */
	if @va_NumAction_in = null or @va_Site_in = null
		return XDC_ARG_INV

	/*A lecture de l'action */

#ifdef HIST
		select @va_fmc_purgee_out=XDC_VRAI

		select
			@va_HoroAppel_out		    = HIS..ACT_GEN.heure_lancement,
			@va_HoroDepart_out		    = HIS..ACT_GEN.heure_fin,
			@va_HoroArrivee_out		    = HIS..ACT_GEN.heure_succes,
			@va_NomDepanneur_out		    = HIS..ACT_DEP.nom,
			@va_Resultat_out		    = HIS..ACT_DEP.resultat,
			@va_Operateur_out		    = HIS..ACT_GEN.operateur,
			@va_renfort_out                     = HIS..ACT_DEP.renfort,
			@va_patrouille_presente_out         = HIS..ACT_DEP.patrouille_presente,
			@va_garag_contact_pc_out 	    = HIS..ACT_DEP.garagiste_doit_contacter_pc,
			@va_annul_intervention_out	    = HIS..ACT_DEP.annulation_intervention,
			@va_code_type_astreinte_out         = HIS..ACT_DEP.code_type_astreinte,
			@va_interlocuteur_out               = HIS..ACT_DEP.interlocuteur,
			@va_site_responsable_out	    = HIS..ACT_DEP.site_responsable
		from 	HIS..ACT_GEN, HIS..ACT_DEP
		where 	HIS..ACT_GEN.numero = @va_NumAction_in and 
			HIS..ACT_GEN.type = XDC_ACT_DEPANNEUR and
			HIS..ACT_GEN.sit = @va_Site_in and
			HIS..ACT_DEP.actio = @va_NumAction_in and 
			HIS..ACT_DEP.sit = @va_Site_in

	if @@rowcount != 1
		return XDC_NOK

#else
 
	select
		@va_HoroAppel_out		    = EXP..ACT_GEN.heure_lancement,
		@va_HoroDepart_out		    = EXP..ACT_GEN.heure_fin,
		@va_HoroArrivee_out	   	    = EXP..ACT_GEN.heure_succes,
		@va_NomDepanneur_out		    = EXP..ACT_DEP.nom,
		@va_Resultat_out		    = EXP..ACT_DEP.resultat,
		@va_Operateur_out		    = EXP..ACT_GEN.operateur,
		@va_renfort_out			    = EXP..ACT_DEP.renfort,
		@va_patrouille_presente_out 	    = EXP..ACT_DEP.patrouille_presente,
		@va_garag_contact_pc_out    	    = EXP..ACT_DEP.garagiste_doit_contacter_pc,
		@va_annul_intervention_out          = EXP..ACT_DEP.annulation_intervention,
		@va_code_type_astreinte_out	    = EXP..ACT_DEP.code_type_astreinte,
		@va_interlocuteur_out		    = EXP..ACT_DEP.interlocuteur,
		@va_site_responsable_out	    = EXP..ACT_DEP.site_responsable
	from 	EXP..ACT_GEN, EXP..ACT_DEP
	where 	EXP..ACT_GEN.numero = @va_NumAction_in and 
		EXP..ACT_GEN.type = XDC_ACT_DEPANNEUR and
		EXP..ACT_GEN.sit = @va_Site_in and
		EXP..ACT_DEP.actio = @va_NumAction_in and 
		EXP..ACT_DEP.sit = @va_Site_in

	/*A si les infos ne sont pas trouvees    */
 	/*A alors le rechercher dans la base HIS */
	if @@rowcount = 0
	begin
		exec @vl_Status = SQL_HIS.PRC..XZAC;35
				@va_NumAction_in,
				@va_Site_in,
				@va_NomDepanneur_out 	output,
				@va_Resultat_out	output,
				@va_HoroAppel_out	output,
				@va_HoroArrivee_out	output,
				@va_Operateur_out	output,
				@va_HoroDepart_out	output,
				@va_HoroAnnul_out	output,
				@va_renfort_out		output,
				@va_patrouille_presente_out 	output,
				@va_garag_contact_pc_out	output,
				@va_annul_intervention_out	output,
				@va_code_type_astreinte_out	output,
				@va_interlocuteur_out 		output,
				@va_fmc_purgee_out		output,
				@va_site_responsable_out	output
		if @vl_Status = null
			return XDC_PRC_INC
		else return @vl_Status
	end

	select @va_fmc_purgee_out = XDC_FAUX

#endif

	if @va_annul_intervention_out = XDC_VRAI
	begin
		select @va_HoroAnnul_out = @va_HoroDepart_out
		select @va_HoroDepart_out = null
	end
	else
		select @va_HoroAnnul_out = null

	return XDC_OK
go
