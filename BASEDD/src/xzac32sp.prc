/*E*/
/*  Fichier : $Id: xzac32sp.prc,v 1.10 2010/08/30 15:58:18 gesconf Exp $      Release : $Revision: 1.10 $        Date : $Date: 2010/08/30 15:58:18 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac32sp.prc
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
* C.T.	28/03/95	: Ajout PR de debut et de fin de balisage (V 1.6)
* JMG	18/11/96	: ajout annulation et sous_type astreinte en sortie (DEM/1306) 1.7
* JMG	18/12/96	: le numero d'action est traite en int 1.8
* C.P.	10/06/98	: Separation EXP et HIS (V 1.9)
* JPL	20/07/10	: Au CI ou sur secteur, si l'action est introuvable, executer sur HIS (DEM 941) 1.10
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Lecture d'une intervention de la patrouille Escota
* 
* Sequence d'appel
* SP	XZAC32_Lire_Intervention_SIS
* 
* Arguments en entree
* XDY_Entier		va_NumeroAction_in
* XDY_Octet		va_Site_in
* 
* Arguments en sortie
* XDY_Nom		va_NomPatrouille_out
* XDY_Booleen		va_PoseBalisage_out
* XY_Horodate		va_DebutBalisage_out
* XY_Horodate		va_FinBalisage_out
* XY_Horodate		va_HoroLancement_out
* XY_Horodate		va_HoroFin_out
* XDY_Operateur		va_Operateur_out
* XY_Horodate		va_HoroSucces_out
* XDY_PR		va_PRDebut_out
* XDY_PR		va_PRFin_out
* XDY_Nom		va_SousTypeAstreinte_out
* tinyint		va_Annulation_out
* XY_Horodate		va_HoroAnnulation_out
*
* Code retour
* XDC_OK
* XDC_NOK	: actio non trouvee
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
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
* 
* Fonction
* Select dans les tables ACTION, INTERVENTION_PATROUILLE
-----------------------------------------------------------------------*/

use PRC
go

create procedure  XZAC;32
	@va_NumeroAction_in	int		= null,
	@va_Site_in		T_SITE		= null,
	@va_NomPatrouille_out	T_NOM		= null		output,
	@va_PoseBalisage_out	bit		= XDC_NON	output,
	@va_DebutBalisage_out	datetime	= null		output,
	@va_FinBalisage_out	datetime	= null		output,
	@va_HoroLancement_out	datetime	= null		output,
	@va_HoroFin_out		datetime	= null		output,
	@va_Operateur_out	T_OPERATEUR	= null		output,
	@va_HoroSucces_out	datetime	= null		output,
	@va_PRDebut_out		T_PR		= null		output,
	@va_PRFin_out		T_PR		= null		output,
	@va_SousTypeAstreinte_out T_NOM		= null		output,
	@va_Annulation_out	tinyint		= null		output,
	@va_HoroAnnulation_out	datetime	= null		output
as
	declare	@vl_comp	smallint,
		@vl_num		int,
		@vl_Status	int

	/*! transfo en paramÉtres nulls bug applix */
	if @va_NumeroAction_in = 0 select @va_NumeroAction_in = null
	if @va_Site_in = 0 select @va_Site_in = null

	/*A Controle des parametres d'entree */
	if @va_NumeroAction_in = null or @va_Site_in = null
		return XDC_ARG_INV

	select @vl_num = null

	/*A lecture de l'action */

#ifdef HIST

		select
			@va_NomPatrouille_out	= HIS..ACT_SIS.nom_patrouille,
			@va_PoseBalisage_out	= HIS..ACT_SIS.pose_balisage,
			@va_DebutBalisage_out	= HIS..ACT_SIS.debut_balisage,
			@va_FinBalisage_out	= HIS..ACT_SIS.fin_balisage,
			@va_PRDebut_out		= HIS..ACT_SIS.PR_debut,
			@va_PRFin_out		= HIS..ACT_SIS.PR_fin,
			@va_HoroLancement_out	= HIS..ACT_GEN.heure_lancement,
			@va_HoroFin_out		= HIS..ACT_GEN.heure_fin,
			@va_Operateur_out	= HIS..ACT_GEN.operateur,
			@va_HoroSucces_out	= HIS..ACT_GEN.heure_succes,
			@va_Annulation_out      = HIS..ACT_SIS.annulation_intervention,
			@vl_comp                = HIS..ACT_GEN.competence,
			@vl_num                 = HIS..ACT_GEN.numero
		from HIS..ACT_GEN, HIS..ACT_SIS
		where	HIS..ACT_GEN.numero = @va_NumeroAction_in and 
			HIS..ACT_GEN.type = XDC_ACT_PAT_SIS and
			HIS..ACT_GEN.sit = @va_Site_in and
			HIS..ACT_SIS.actio = @va_NumeroAction_in and 
			HIS..ACT_SIS.sit = @va_Site_in

		if @vl_comp != null
			select @va_SousTypeAstreinte_out = sous_type_d_astreinte
			from CFG..ANN_AST_CMP
			where numero = @vl_comp
		else
			select @va_SousTypeAstreinte_out = null

#else

	select
		@va_NomPatrouille_out	= EXP..ACT_SIS.nom_patrouille,
		@va_PoseBalisage_out	= EXP..ACT_SIS.pose_balisage,
		@va_DebutBalisage_out	= EXP..ACT_SIS.debut_balisage,
		@va_FinBalisage_out	= EXP..ACT_SIS.fin_balisage,
		@va_PRDebut_out		= EXP..ACT_SIS.PR_debut,
		@va_PRFin_out		= EXP..ACT_SIS.PR_fin,
		@va_HoroLancement_out	= EXP..ACT_GEN.heure_lancement,
		@va_HoroFin_out		= EXP..ACT_GEN.heure_fin,
		@va_Operateur_out	= EXP..ACT_GEN.operateur,
		@va_HoroSucces_out	= EXP..ACT_GEN.heure_succes,
		@va_Annulation_out 	= EXP..ACT_SIS.annulation_intervention,
		@vl_comp 		= EXP..ACT_GEN.competence,
		@vl_num			= EXP..ACT_GEN.numero
	from EXP..ACT_GEN, EXP..ACT_SIS
	where	EXP..ACT_GEN.numero = @va_NumeroAction_in and 
		EXP..ACT_GEN.type = XDC_ACT_PAT_SIS and
		EXP..ACT_GEN.sit = @va_Site_in and
		EXP..ACT_SIS.actio = @va_NumeroAction_in and 
		EXP..ACT_SIS.sit = @va_Site_in

	/*A si les infos ne sont pas trouvees     */
	/*A alors les rechercher dans la base HIS */
	if @@rowcount = 0
	begin
		exec @vl_Status = SQL_HIS.PRC..XZAC;32
					@va_NumeroAction_in,
					@va_Site_in,
					@va_NomPatrouille_out		output,
					@va_PoseBalisage_out		output,
					@va_DebutBalisage_out		output,
					@va_FinBalisage_out		output,
					@va_HoroLancement_out		output,
					@va_HoroFin_out			output,
					@va_Operateur_out		output,
					@va_HoroSucces_out		output,
					@va_PRDebut_out			output,
					@va_PRFin_out			output,
					@va_SousTypeAstreinte_out	output,
					@va_Annulation_out		output,
					@va_HoroAnnulation_out		output
		if @vl_Status = null
			return XDC_PRC_INC
		else return @vl_Status
	end


	if @vl_comp != null
		select @va_SousTypeAstreinte_out = sous_type_d_astreinte
		from CFG..ANN_AST_CMP
		where numero = @vl_comp
	else
		select @va_SousTypeAstreinte_out = null

#endif

	if @vl_num = null
		return XDC_NOK

	if @va_Annulation_out = XDC_VRAI
	begin
		select @va_HoroAnnulation_out = @va_HoroFin_out
		select @va_HoroFin_out = null
	end
	else
		select @va_HoroAnnulation_out = null

	return XDC_OK
go
