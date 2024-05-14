/*E*/
/*  Fichier : $Id: xzae40sp.prc,v 1.36 2019/05/13 12:27:10 devgfi Exp $        $Revision: 1.36 $        $Date: 2019/05/13 12:27:10 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae40sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	29/08/94	: Creation
* C.T.	28/09/94	: Modif sur NomPtCar value a " " 
*                         si pas de point caracteristique
*                         de meme pour TypeAlerte
* C.T.  14/10/94	: Ajout des parametres de sortie
*                         de localisation de la cause,
*                         le site du commentaire et
*                         le critere de voie neutralisee
*                         a la place de bloquee dans cas
*                         travaux, basculement, action SIS
*                         V 1.6
* C.T.  20/10/94	: Modif commentaire entete procedure V 1.7
* C.T.  10/11/94	: Modif bugs applix V 1.8
*                         non teste
* C.T.	14/11/94	: Modif recherche des infos au CI (V 1.9)
*                         non teste
* C.T.	01/12/94	: Supprimer commentaires en double generes par sccs
*                         (V 1.10)
* C.T.  01/12/94	: Annuler le remplacement de
*                         voie neutralisee par bloquee
*                         dans cas travaux, basculement, action SIS
*                         non teste (V 1.11)
* B.G.  05/12/94	: ajout arguments numero et abreviation pour Pt Carac (1.12 et 13)
* B.G.  05/12/94	: null ou 0 pour pt car (1.14)
* B.G.  22/12/94	: modif lecture commentaire (1.14)
* B.G.  17/01/95	: modif date nulle (1.16)
* C.T.  15/06/95	: correction erreur recherche alerte (1.17)
* C.T.  15/11/95	: correction recherche sur commentaire (1.18)
* C.T.  23/01/96	: ajout du parametre position en sortie (1.19)
* C.T.  04/09/96	: ajout cas de pc simplifie : compilation condit. (V 1.20)
* C.T.  25/09/96	: ajout degats domaine (FMC10) (V 1.21)
* C.T.  01/10/96	: ajout type anterieur en parametre de sortie (FMC3) (V 1.22)
* JMG	14/10/96	: suppression du rowcount si recup du type de la cause au PC simplifie (1.23)
* C.T.  04/11/96	: ajout fausse alerte en parametre de sortie (FMC18) (V 1.24)
* C.T.  14/11/96	: ajout bau etroite en parametre de sortie (SAD4) (V 1.25)
* C.T.  11/12/96	: ajout longueur en parametre de sortie (FMC7) (V 1.26)
* C.T.  15/12/97	: ajout degrade en parametre de sortie (1531) (V 1.27)
* C.T.  27/02/98	: ajout com_fmc_init en parametre de sortie (1583) (V 1.28)
* C.P.	18/06/98	: Modif separation EXP et HIS (V 1.29)
* P.N.	13/01/99	: Modif intergestionnaire dem/1624 (V 1.30)
*  P.N.  19/03/99	: Correction intergestionnaire dem/1745 (1.33)
* JMG	09/04/99	: correction FMC_DAT sur PC N3 1.33
* JMG	27/04/00	: tri des commentaires par horodate decroissante (SAE 91) 1.34
* JMG	26/11/16 	: ajout majeur lot 23 1.35
* JPL	04/04/19	: Ajout de l'indicateur d'écoulement (DEM 1326)  1.36
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Retourne les champs generiques d'une fiche main 
* courante a l'heure precisee
* -----------------------------------------------------
* Sequence d'appel
* SP	XZAE40_Lire_Fiche_MC
* 
* Arguments en entree
* XDY_Horodate	va_Horodate_in : par defaut heure courante
* XDY_Entier	va_NumEvt_in
* XDY_Octet	va_CleEvt_in
* 
* Arguments en sortie
* XDY_Entier	va_Numero_out
* XDY_Octet	va_Cle_out
* XDY_Entier	va_Site_out
* XDY_Mot	va_Type_out
* XDY_Horodate	va_DebutPrevu_out
* XDY_Horodate	va_Debut_out
* XDY_Horodate	va_FinPrevu_out
* XDY_Horodate	va_Fin_out
* XDY_Horodate	va_Validation_out
* XDY_Entier	va_NumCause_out
* XDY_Octet	va_CleCause_out
* XDY_Octet	va_TypeCause_out
* XDY_Alerte	va_NumAlerte_out
* XDY_Horodate	va_HoroAlerte_out
* XDY_Nom	va_TypeAlerte_out
* XDY_Nom       va_Origine_out
* XDY_Booleen	va_Confirme_out
* XDY_Autoroute	va_Autoroute_out
* XDY_PR	va_PR_out
* XDY_Sens	va_Sens_out
* XDY_Octet	va_PointCar_out
* XDY_Nom	va_NomPtCar_out
* XDY_Voie	va_VR_out
* XDY_Voie	va_VM2_out
* XDY_Voie	va_VM1_out
* XDY_Voie	va_VL_out
* XDY_Voie	va_BAU_out
* XDY_Voie	va_VR_I_out
* XDY_Voie	va_VM2_I_out
* XDY_Voie	va_VM1_I_out
* XDY_Voie	va_VL_I_out
* XDY_Voie	va_BAU_I_out
* XDY_Autoroute	va_AutorouteCause_out
* XDY_PR	va_PRCause_out
* XDY_Sens	va_SensCause_out
* XDY_Horodate	va_HoroCause_out
* XDY_Octet	va_Position_out
* XDY_Octet	va_Degats_out
* XDY_Mot	va_TypeAnterieur_out
* XDY_Octet	va_FausseAlerte_out
* XDY_Octet	va_BAU_etroite_out
* XDY_Octet	va_BAU_I_etroite_out
* XDY_Entier	va_Longueur_out
* char(15)	va_AbrPtCar_out
* XDY_Octet	va_NumPtCar_out
* XDY_Octet	va_Degrade_out
* XDY_Entier	va_NumEvtInit_out
* XDY_Octet	va_CleEvtInit_out
* XDY_Octet	va_SiteAlerte_out
* XDY_Nom	va_ComEvtInit_out
* XDY_Octet	va_TypeEvtInit_out
* XDY_Autoroute	va_AutorouteEvtInit_out
* XDY_PR	va_PREvtInit_out
* XDY_Sens	va_SensEvtInit_out
* XDY_Horodate	va_HoroEvtInit_out
* XDY_Octet	va_Datex_out
* XDY_Octet	va_Trafic_out		
* XDY_Octet	va_Gravite_out		
* XDY_Booleen	va_Bloquant_out	
* XDY_Octet	va_Duree_out
* XDY_Octet	va_Majeur_out
* XDY_Octet	va_Ecoulement_out
*
*
* XDY_Octet	va_NumCommentaire_out
* XDY_Horodate	va_HorodateCommentaire_out
* XDY_Horodate	va_HoroPerimeCommentaire_out
* XDY_Commentaire	va_TexteCommentaire_out
* XDY_Site	va_Site_out
* 
* Code retour
* XDC_OK
* XDC_NOK	: fiche non trouvee
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* Penser a lire le complement fonction du type d'evenement
* En formation, preciser l'horodate
* Les informations concernant les commentaires sont retournes
* dans une liste
* 
* La procedure stockee au CI recherche les informations 
* dans la base HIS si la date donnee est anterieure de plus d'un 
* mois tandis qu'au niveau des districts la recherche s'effectue 
* dans la base EXP.
* Le code de creation de la procedure au CI et aux districts 
* doit etre identique a l'exception de la base de recherche des infos
* et du test de la date en entree (< 1 mois)
* (toute modif dans la creation de la procedure doit etre 
*  effectuee pour le CI et les districts)
* 
* 
* Fonction
* Select dans les tables EVENEMENT, EVENEMENT_HISTORIQUE, 
* ALERTE, COMMENTAIRES where numero evenement and horodate
----------------------------------------------------*/

use PRC
go

if exists (select 1 from sysobjects where name = 'XZAE40' and type = 'P')
	drop procedure XZAE40
go


create procedure XZAE40
	@va_Horodate_in		datetime , -- par defaut heure courante
	@va_NumEvt_in		int = null,
	@va_CleEvt_in		tinyint = null,
	@va_Numero_out		int	 	= null	output,
	@va_Cle_out		tinyint	 	= null	output,
	@va_Site_out		T_SITE	 	= null	output,
	@va_Type_out		tinyint	 	= null	output,	
	@va_DebutPrevu_out	datetime 	= null	output,
	@va_Debut_out		datetime 	= null	output,
	@va_FinPrevu_out	datetime 	= null	output,
	@va_Fin_out		datetime 	= null	output,
	@va_Validation_out	datetime 	= null	output,
	@va_NumCause_out	int	 	= null	output,
	@va_CleCause_out	tinyint	 	= null	output,
	@va_TypeCause_out	smallint 	= null	output,
	@va_NumAlerte_out	int	 	= null	output,
	@va_HoroAlerte_out	datetime 	= null	output,
	@va_TypeAlerte_out	T_NOM	 	= null	output,
	@va_Origine_out		T_NOM	 	= null	output,
	@va_Confirme_out	bit	 	= XDC_NON	output,
	@va_Autoroute_out	tinyint	 	= null	output,
	@va_PR_out		T_PR	 	= null	output,
	@va_Sens_out		tinyint	 	= null	output,
	@va_PointCar_out	tinyint	 	= null	output,
	@va_NomPtCar_out	T_NOM	 	= null	output,
	@va_VR_out		tinyint	 	= null	output,
	@va_VM2_out		tinyint	 	= null	output,
	@va_VM1_out		tinyint	 	= null	output,
	@va_VL_out		tinyint	 	= null	output,
	@va_BAU_out		tinyint	 	= null	output,
	@va_VR_I_out		tinyint	 	= null	output,
	@va_VM2_I_out		tinyint	 	= null	output,
	@va_VM1_I_out		tinyint	 	= null	output,
	@va_VL_I_out		tinyint	 	= null	output,
	@va_BAU_I_out		tinyint	 	= null	output,
	@va_AutorouteCause_out	tinyint	 	= null	output,
	@va_PRCause_out		T_PR	 	= null	output,
	@va_SensCause_out	tinyint	 	= null	output,
	@va_HoroCause_out	datetime 	= null	output,
	@va_Position_out	tinyint	 	= null	output,
	@va_Degats_out		tinyint	 	= null	output,
	@va_TypeAnterieur_out	smallint	= null	output,
	@va_FausseAlerte_out	tinyint	 	= null	output,
	@va_BAU_Etroite_out	tinyint		= null	output,
	@va_BAU_I_Etroite_out	tinyint		= null	output,
	@va_Longueur_out		int		= null	output,
	@va_AbrPtCar_out	char(15) 	= null	output,
	@va_NumPtCar_out	tinyint	 	= null	output,
	@va_Degrade_out		tinyint	 	= null	output,
	@va_NumEvtInit_out	int 		= null	output,
	@va_CleEvtInit_out	tinyint 	= null	output,
	@va_ComEvtInit_out	T_NOM	 	= null	output,
	@va_TypeEvtInit_out	smallint 	= null	output,
	@va_AutorouteEvtInit_out	tinyint	 	= null	output,
	@va_PREvtInit_out	T_PR	 	= null	output,
	@va_SensEvtInit_out	tinyint	 	= null	output,
	@va_HoroEvtInit_out	datetime 	= null	output,
	@va_SiteAlerte_out	tinyint	 	= null	output,
	@va_Datex_out		tinyint	 	= null	output,
	@va_Trafic_out		tinyint	 	= null	output,
	@va_Gravite_out		tinyint	 	= null	output,
	@va_Bloquant_out	bit	 	= XDC_NON	output,
	@va_Duree_out		tinyint	 	= null	output,
	@va_Majeur_out		tinyint         = null  output,
	@va_Ecoulement_out	tinyint		= null	output
as
	declare @vl_Horodate datetime, @vl_NumPointCar tinyint,
		@vl_Classe tinyint, @vl_Status int,@vl_DateCourante datetime,
		@vl_duree int

	/*! transfo en paramÉtres nulls bug applix */
	if @va_NumEvt_in = 0 select @va_NumEvt_in = null
	if @va_CleEvt_in = 0 select @va_CleEvt_in = null
	if @va_Horodate_in = XDC_DATE_NULLE select @va_Horodate_in = null

	/*A controle des arguments obligatoires */
	if @va_NumEvt_in = null or @va_CleEvt_in = null
		return XDC_ARG_INV

	/*A date servant a la recherche de la fiche main courante */
	if @va_Horodate_in = null
	begin
		select @va_Horodate_in = XDC_DATE_INFINIE
	end
	else
	begin
		select @va_Horodate_in = dateadd(second, 1, @va_Horodate_in)
	end

#ifdef HIST
select 
			@vl_Horodate = max(horodate_validation) 
		from HIS..FMC_HIS
		where	HIS..FMC_HIS.numero = @va_NumEvt_in and 
			HIS..FMC_HIS.cle = @va_CleEvt_in and 
			HIS..FMC_HIS.horodate_validation <= @va_Horodate_in
	
		if @@rowcount = 0
		begin
			return XDC_NOK
		end
	
		/*A lecture de la fiche main courante avec son historique */
		select 
			/* lecture des informations de FMC_GEN */
			@va_Numero_out		= HIS..FMC_GEN.numero,
			@va_Cle_out		= HIS..FMC_GEN.cle,
			@va_Site_out		= HIS..FMC_GEN.sit,
			@va_Type_out		= HIS..FMC_GEN.type,
			@va_DebutPrevu_out	= HIS..FMC_GEN.debut_prevu,
			@va_Debut_out		= HIS..FMC_GEN.debut,
			@va_FinPrevu_out	= HIS..FMC_GEN.fin_prevue,
			@va_Fin_out		= HIS..FMC_GEN.fin,
			@va_NumCause_out	= HIS..FMC_GEN.cause,
			@va_CleCause_out	= HIS..FMC_GEN.cle_cause,
			@va_NumAlerte_out	= HIS..FMC_GEN.alerte,
			@va_SiteAlerte_out	= HIS..FMC_GEN.sit_alerte,
			@va_Origine_out		= HIS..FMC_GEN.origine_creation,
			@va_Degats_out		= HIS..FMC_GEN.degats_domaine,
			@va_FausseAlerte_out	= HIS..FMC_GEN.fausse_alerte,
			@va_Degrade_out		= HIS..FMC_GEN.degrade,
			@va_NumEvtInit_out	= HIS..FMC_GEN.num_fmc_init,
			@va_CleEvtInit_out	= HIS..FMC_GEN.cle_fmc_init,
			@va_ComEvtInit_out	= HIS..FMC_GEN.com_fmc_init,
			@va_Datex_out		= HIS..FMC_GEN.datex,
		
			/* lecture de l'historique de la fiche main courante */
			@va_TypeAnterieur_out	= HIS..FMC_HIS.type_anterieur,
			@va_Validation_out	= HIS..FMC_HIS.horodate_validation,
			@va_Autoroute_out	= HIS..FMC_HIS.autoroute,
			@va_PR_out		= HIS..FMC_HIS.PR,
			@va_Sens_out		= HIS..FMC_HIS.sens,
			@va_PointCar_out	= HIS..FMC_HIS.point_caracteristique,
			@vl_NumPointCar		= HIS..FMC_HIS.numero_point_caracteristique,
			@va_Position_out	= HIS..FMC_HIS.position_sur_aire_ou_echangeur,
			@va_Confirme_out	= HIS..FMC_HIS.confirme_ou_signale,
			@va_VR_out		= HIS..FMC_HIS.VR,
			@va_VM2_out		= HIS..FMC_HIS.VM2,
			@va_VM1_out		= HIS..FMC_HIS.VM1,
			@va_VL_out		= HIS..FMC_HIS.VL,
			@va_BAU_out		= HIS..FMC_HIS.BAU,
			@va_VR_I_out		= HIS..FMC_HIS.VR_I,
			@va_VM2_I_out		= HIS..FMC_HIS.VM2_I,
			@va_VM1_I_out		= HIS..FMC_HIS.VM1_I,
			@va_VL_I_out		= HIS..FMC_HIS.VL_I,
			@va_BAU_I_out		= HIS..FMC_HIS.BAU_I,
			@va_BAU_Etroite_out	= HIS..FMC_HIS.BAU_etroite,
			@va_BAU_I_Etroite_out	= HIS..FMC_HIS.BAU_I_etroite,
			@va_Longueur_out	= HIS..FMC_HIS.longueur,
	
			@vl_Classe		= CFG..TYP_FMC.classe,
			@va_Majeur_out		= HIS..FMC_HIS.majeur,
			@va_Ecoulement_out	= HIS..FMC_HIS.ecoulement
		
		from HIS..FMC_GEN, HIS..FMC_HIS, CFG..TYP_FMC
		where	HIS..FMC_GEN.numero = @va_NumEvt_in and 
			HIS..FMC_GEN.cle = @va_CleEvt_in and 
			HIS..FMC_HIS.numero = @va_NumEvt_in and 
			HIS..FMC_HIS.cle = @va_CleEvt_in and 
			HIS..FMC_HIS.horodate_validation = @vl_Horodate and
			CFG..TYP_FMC.numero = HIS..FMC_GEN.type
	
		/* test si une fiche a ete trouvee sinon erreur */
		if @@rowcount = 0
		begin
			select @va_Confirme_out = XDC_NON
			return XDC_NOK
		end
	
		if @va_Datex_out=null
			select @va_Datex_out=XDC_FAUX
			
		/*A Recherche la localisation de la cause de l'evenement et */
		/*A la date de debut de la cause                            */
		if @va_NumCause_out != null and @va_CleCause_out != null
		begin
			select 
				@va_AutorouteCause_out	= HIS..FMC_HIS.autoroute,
				@va_PRCause_out		= HIS..FMC_HIS.PR,
				@va_SensCause_out	= HIS..FMC_HIS.sens,
				@va_HoroCause_out	= isnull(HIS..FMC_GEN.debut, 
								HIS..FMC_GEN.debut_prevu)
			from HIS..FMC_GEN, HIS..FMC_HIS
			where	HIS..FMC_GEN.numero = @va_NumCause_out and
				HIS..FMC_GEN.cle = @va_CleCause_out and
				HIS..FMC_HIS.numero = HIS..FMC_GEN.numero and
				HIS..FMC_HIS.cle = HIS..FMC_GEN.cle 
				group by HIS..FMC_GEN.numero, HIS..FMC_GEN.cle,
					 HIS..FMC_HIS.numero, HIS..FMC_HIS.cle
				having HIS..FMC_HIS.horodate_validation = 
					min(HIS..FMC_HIS.horodate_validation)
		end
	
		/*A Recherche la localisation de la fmc initiale et */
		/*A la date de debut de la fmc initiale */
		if @va_NumEvtInit_out != null and @va_CleEvtInit_out != null
		begin
			select 
				@va_TypeEvtInit_out	= HIS..FMC_HIS.type_FMC_valide,
				@va_AutorouteEvtInit_out	= HIS..FMC_HIS.autoroute,
				@va_PREvtInit_out	= HIS..FMC_HIS.PR,
				@va_SensEvtInit_out	= HIS..FMC_HIS.sens,
				@va_HoroEvtInit_out	= isnull(HIS..FMC_GEN.debut, 
								HIS..FMC_GEN.debut_prevu)
			from HIS..FMC_GEN, HIS..FMC_HIS
			where	HIS..FMC_GEN.numero = @va_NumEvtInit_out and
				HIS..FMC_GEN.cle = @va_CleEvtInit_out and
				HIS..FMC_HIS.numero = HIS..FMC_GEN.numero and
				HIS..FMC_HIS.cle = HIS..FMC_GEN.cle 
				group by HIS..FMC_GEN.numero, HIS..FMC_GEN.cle,
					 HIS..FMC_HIS.numero, HIS..FMC_HIS.cle
				having HIS..FMC_HIS.horodate_validation = 
					min(HIS..FMC_HIS.horodate_validation)
		end
	
		if @va_NumAlerte_out != null
		begin
			/*A Rechercher les informations de l'alerte */
			select
				@va_HoroAlerte_out	= HIS..ALT_EXP.horodate,
				@va_TypeAlerte_out	= CFG..TYP_ALT.nom
			from CFG..TYP_ALT, HIS..ALT_EXP
			where	HIS..ALT_EXP.numero = @va_NumAlerte_out and
				HIS..ALT_EXP.sit = @va_SiteAlerte_out and 
				CFG..TYP_ALT.numero = HIS..ALT_EXP.type
	
			if @@rowcount = 0
			begin
				select @va_TypeAlerte_out = XDC_CHAINE_VIDE
			end
		end
		/* valuation a chaine " " si pas de alerte */
		/* sinon pb avec open server et client     */
		else	select @va_TypeAlerte_out = XDC_CHAINE_VIDE
	
	
		/*A recherche des commentaires de la fiche main courante */
		select
			numero,
			horodate_validation,
			horodate_fin,
			texte,
			sit
		from HIS..FMC_COM
		where	evenement = @va_NumEvt_in and cle = @va_CleEvt_in and
			horodate_validation <= @va_Horodate_in 
		order by horodate_validation desc
	
		/*A Recherche le type de la cause de fiche main courante */
		if @va_NumCause_out != null
		begin
			select @va_TypeCause_out = type 
			from HIS..FMC_GEN 
			where numero = @va_NumCause_out and cle = @va_CleCause_out
	
			if @@rowcount = 0
				return XDC_NOK
		end
		
		/*A Recherche les attributs datex et du ftaifc et de la duree */
		if @va_Datex_out = XDC_VRAI 
		begin
			select
				@va_Gravite_out=gravite,
				@va_Trafic_out=trafic,
				@va_Bloquant_out=bloquant,
				@va_Duree_out=duree
			from HIS..FMC_DAT
			where numero= @va_NumEvt_in and 
				cle = @va_CleEvt_in 
			
		end
		else
		begin
			select @va_Bloquant_out=XDC_FAUX,@va_Gravite_out=null
			select @vl_duree=datediff(hour,@va_Debut_out,@va_Horodate_in)
			if @vl_duree<0 or @vl_duree>255
				select @va_Duree_out=255
			else
				select @va_Duree_out=@vl_duree
		end

		

#else

#ifdef CI
	/*! cas ou la procedure est creee au CI */

	/*A si la date donnee est anterieure d'un mois au moins */
 	/*A rechercher l'evenement dans la base HIS sinon dans  */
	/*A la base EXP (au niveau du CI)       
	                */
	if @va_Horodate_in < (select dateadd(month, -1, getdate ()))
	begin
           	exec @vl_Status = SQL_HIS.PRC..XZAE40 
	@va_Horodate_in		, 
	@va_NumEvt_in		,
	@va_CleEvt_in		,
	@va_Numero_out		output,
	@va_Cle_out		output,
	@va_Site_out		output,
	@va_Type_out		 output,	
	@va_DebutPrevu_out	 output,
	@va_Debut_out		 output,
	@va_FinPrevu_out	 output,
	@va_Fin_out		 output,
	@va_Validation_out	 output,
	@va_NumCause_out	output,
	@va_CleCause_out	 output,
	@va_TypeCause_out	 output,
	@va_NumAlerte_out	output,
	@va_HoroAlerte_out	 output,
	@va_TypeAlerte_out	output,
	@va_Origine_out		output,
	@va_Confirme_out	output,
	@va_Autoroute_out	 output,
	@va_PR_out		output,
	@va_Sens_out		 output,
	@va_PointCar_out	 output,
	@va_NomPtCar_out	output,
	@va_VR_out		 output,
	@va_VM2_out		 output,
	@va_VM1_out		 output,
	@va_VL_out		 output,
	@va_BAU_out		 output,
	@va_VR_I_out		 output,
	@va_VM2_I_out		 output,
	@va_VM1_I_out		 output,
	@va_VL_I_out		 output,
	@va_BAU_I_out		 output,
	@va_AutorouteCause_out	 output,
	@va_PRCause_out		output,
	@va_SensCause_out	 output,
	@va_HoroCause_out	 output,
	@va_Position_out	 output,
	@va_Degats_out		 output,
	@va_TypeAnterieur_out	output,
	@va_FausseAlerte_out	 output,
	@va_BAU_Etroite_out	output,
	@va_BAU_I_Etroite_out	output,
	@va_Longueur_out	output,
	@va_AbrPtCar_out	output,
	@va_NumPtCar_out	 output,
	@va_Degrade_out		 output,
	@va_NumEvtInit_out	output,
	@va_CleEvtInit_out	output,
	@va_ComEvtInit_out	output,
	@va_TypeEvtInit_out	 output,
	@va_AutorouteEvtInit_out	 output,
	@va_PREvtInit_out	output,
	@va_SensEvtInit_out	 output,
	@va_HoroEvtInit_out	 output,
	@va_SiteAlerte_out	 output,
	@va_Datex_out		output,
	@va_Trafic_out		output,
	@va_Gravite_out		output,
	@va_Bloquant_out	output,
	@va_Duree_out		output,
	@va_Majeur_out		output,
	@va_Ecoulement_out	output

             	if @vl_Status = null
                  	return XDC_PRC_INC
            	else return @vl_Status		
	end
	else
	begin
		/*A rechercher la derniere date de validation de  */
		/*A la partie historique de l'evenement anterieur */
		/*A a la date donnee                              */
		select 
			@vl_Horodate = max(horodate_validation) 
		from EXP..FMC_HIS
		where	EXP..FMC_HIS.numero = @va_NumEvt_in and 
			EXP..FMC_HIS.cle = @va_CleEvt_in and 
			EXP..FMC_HIS.horodate_validation <= @va_Horodate_in
	
		if @@rowcount = 0
		begin
			return XDC_NOK
		end
	
		/*A lecture de la fiche main courante avec son historique */
		select 
			/* lecture des informations de FMC_GEN */
			@va_Numero_out		= EXP..FMC_GEN.numero,
			@va_Cle_out		= EXP..FMC_GEN.cle,
			@va_Site_out		= EXP..FMC_GEN.sit,
			@va_Type_out		= EXP..FMC_GEN.type,
			@va_DebutPrevu_out	= EXP..FMC_GEN.debut_prevu,
			@va_Debut_out		= EXP..FMC_GEN.debut,
			@va_FinPrevu_out	= EXP..FMC_GEN.fin_prevue,
			@va_Fin_out		= EXP..FMC_GEN.fin,
			@va_NumCause_out	= EXP..FMC_GEN.cause,
			@va_CleCause_out	= EXP..FMC_GEN.cle_cause,
			@va_NumAlerte_out	= EXP..FMC_GEN.alerte,
			@va_SiteAlerte_out	= EXP..FMC_GEN.sit_alerte,
			@va_Origine_out		= EXP..FMC_GEN.origine_creation,
			@va_Degats_out		= EXP..FMC_GEN.degats_domaine,
			@va_FausseAlerte_out	= EXP..FMC_GEN.fausse_alerte,
			@va_Degrade_out		= EXP..FMC_GEN.degrade,
			@va_NumEvtInit_out	= EXP..FMC_GEN.num_fmc_init,
			@va_CleEvtInit_out	= EXP..FMC_GEN.cle_fmc_init,
			@va_ComEvtInit_out	= EXP..FMC_GEN.com_fmc_init,
			@va_Datex_out		= EXP..FMC_GEN.datex,
		
			/* lecture de l'historique de la fiche main courante */
			@va_TypeAnterieur_out	= EXP..FMC_HIS.type_anterieur,
			@va_Validation_out	= EXP..FMC_HIS.horodate_validation,
			@va_Autoroute_out	= EXP..FMC_HIS.autoroute,
			@va_PR_out		= EXP..FMC_HIS.PR,
			@va_Sens_out		= EXP..FMC_HIS.sens,
			@va_PointCar_out	= EXP..FMC_HIS.point_caracteristique,
			@vl_NumPointCar		= EXP..FMC_HIS.numero_point_caracteristique,
			@va_Position_out	= EXP..FMC_HIS.position_sur_aire_ou_echangeur,
			@va_Confirme_out	= EXP..FMC_HIS.confirme_ou_signale,
			@va_VR_out		= EXP..FMC_HIS.VR,
			@va_VM2_out		= EXP..FMC_HIS.VM2,
			@va_VM1_out		= EXP..FMC_HIS.VM1,
			@va_VL_out		= EXP..FMC_HIS.VL,
			@va_BAU_out		= EXP..FMC_HIS.BAU,
			@va_VR_I_out		= EXP..FMC_HIS.VR_I,
			@va_VM2_I_out		= EXP..FMC_HIS.VM2_I,
			@va_VM1_I_out		= EXP..FMC_HIS.VM1_I,
			@va_VL_I_out		= EXP..FMC_HIS.VL_I,
			@va_BAU_I_out		= EXP..FMC_HIS.BAU_I,
			@va_BAU_Etroite_out	= EXP..FMC_HIS.BAU_etroite,
			@va_BAU_I_Etroite_out	= EXP..FMC_HIS.BAU_I_etroite,
			@va_Longueur_out	= EXP..FMC_HIS.longueur,
	
			@vl_Classe		= CFG..TYP_FMC.classe,
			@va_Majeur_out		= EXP..FMC_HIS.majeur,
			@va_Ecoulement_out	= EXP..FMC_HIS.ecoulement
		
		from EXP..FMC_GEN, EXP..FMC_HIS, CFG..TYP_FMC
		where	EXP..FMC_GEN.numero = @va_NumEvt_in and 
			EXP..FMC_GEN.cle = @va_CleEvt_in and 
			EXP..FMC_HIS.numero = @va_NumEvt_in and 
			EXP..FMC_HIS.cle = @va_CleEvt_in and 
			EXP..FMC_HIS.horodate_validation = @vl_Horodate and
			CFG..TYP_FMC.numero = EXP..FMC_GEN.type
	
		/* test si une fiche a ete trouvee sinon erreur */
		if @@rowcount = 0
		begin
			select @va_Confirme_out = XDC_NON
			return XDC_NOK
		end
	
		if @va_Datex_out=null
			select @va_Datex_out=XDC_FAUX
			
		/*A Recherche la localisation de la cause de l'evenement et */
		/*A la date de debut de la cause                            */
		if @va_NumCause_out != null and @va_CleCause_out != null
		begin
			select 
				@va_AutorouteCause_out	= EXP..FMC_HIS.autoroute,
				@va_PRCause_out		= EXP..FMC_HIS.PR,
				@va_SensCause_out	= EXP..FMC_HIS.sens,
				@va_HoroCause_out	= isnull(EXP..FMC_GEN.debut, 
								EXP..FMC_GEN.debut_prevu)
			from EXP..FMC_GEN, EXP..FMC_HIS
			where	EXP..FMC_GEN.numero = @va_NumCause_out and
				EXP..FMC_GEN.cle = @va_CleCause_out and
				EXP..FMC_HIS.numero = EXP..FMC_GEN.numero and
				EXP..FMC_HIS.cle = EXP..FMC_GEN.cle 
				group by EXP..FMC_GEN.numero, EXP..FMC_GEN.cle,
					 EXP..FMC_HIS.numero, EXP..FMC_HIS.cle
				having EXP..FMC_HIS.horodate_validation = 
					min(EXP..FMC_HIS.horodate_validation)
		end
	
		/*A Recherche la localisation de la fmc initiale et */
		/*A la date de debut de la fmc initiale */
		if @va_NumEvtInit_out != null and @va_CleEvtInit_out != null
		begin
			select 
				@va_TypeEvtInit_out	= EXP..FMC_HIS.type_FMC_valide,
				@va_AutorouteEvtInit_out	= EXP..FMC_HIS.autoroute,
				@va_PREvtInit_out	= EXP..FMC_HIS.PR,
				@va_SensEvtInit_out	= EXP..FMC_HIS.sens,
				@va_HoroEvtInit_out	= isnull(EXP..FMC_GEN.debut, 
								EXP..FMC_GEN.debut_prevu)
			from EXP..FMC_GEN, EXP..FMC_HIS
			where	EXP..FMC_GEN.numero = @va_NumEvtInit_out and
				EXP..FMC_GEN.cle = @va_CleEvtInit_out and
				EXP..FMC_HIS.numero = EXP..FMC_GEN.numero and
				EXP..FMC_HIS.cle = EXP..FMC_GEN.cle 
				group by EXP..FMC_GEN.numero, EXP..FMC_GEN.cle,
					 EXP..FMC_HIS.numero, EXP..FMC_HIS.cle
				having EXP..FMC_HIS.horodate_validation = 
					min(EXP..FMC_HIS.horodate_validation)
		end
	
		if @va_NumAlerte_out != null
		begin
			/*A Rechercher les informations de l'alerte */
			select
				@va_HoroAlerte_out	= EXP..ALT_EXP.horodate,
				@va_TypeAlerte_out	= CFG..TYP_ALT.nom
			from CFG..TYP_ALT, EXP..ALT_EXP
			where	EXP..ALT_EXP.numero = @va_NumAlerte_out and
				EXP..ALT_EXP.sit = @va_SiteAlerte_out and 
				CFG..TYP_ALT.numero = EXP..ALT_EXP.type
	
			if @@rowcount = 0
			begin
				select @va_TypeAlerte_out = XDC_CHAINE_VIDE
			end
		end
		/* valuation a chaine " " si pas de alerte */
		/* sinon pb avec open server et client     */
		else	select @va_TypeAlerte_out = XDC_CHAINE_VIDE
	
	
		/*A recherche des commentaires de la fiche main courante */
		select
			numero,
			horodate_validation,
			horodate_fin,
			texte,
			sit
		from EXP..FMC_COM
		where	evenement = @va_NumEvt_in and cle = @va_CleEvt_in and
			horodate_validation <= @va_Horodate_in 
		order by horodate_validation desc
	
		/*A Recherche le type de la cause de fiche main courante */
		if @va_NumCause_out != null
		begin
			select @va_TypeCause_out = type 
			from EXP..FMC_GEN 
			where numero = @va_NumCause_out and cle = @va_CleCause_out
	
			if @@rowcount = 0
				return XDC_NOK
		end

		/*A Recherche les attributs datex et du ftaifc et de la duree */
		if @va_Datex_out = XDC_VRAI 
		begin
			select
				@va_Gravite_out=gravite,
				@va_Trafic_out=trafic,
				@va_Bloquant_out=bloquant,
				@va_Duree_out=duree
			from EXP..FMC_DAT
			where numero= @va_NumEvt_in and 
				cle = @va_CleEvt_in 
			
		end
		else
		begin
			exec PRC..XZAP;25 @va_Autoroute_out,@va_PR_out,@va_Sens_out,null,@va_Trafic_out	output
			select @va_Bloquant_out=XDC_FAUX,@va_Gravite_out=null
			if @va_FinPrevu_out=null
		        	select @vl_duree=datediff(hour,@va_Debut_out,getdate())
		        else
		        	select @vl_duree=datediff(hour,@va_Debut_out,@va_FinPrevu_out)
			if @vl_duree<=0 or @vl_duree>255
				select @va_Duree_out=255
			else
				select @va_Duree_out=@vl_duree
		end
		
	end
		
#else
	/*! cas ou la procedure est creee sur le serveur d'un district */

	/*A rechercher la derniere date de validation de  */
	/*A la partie historique de l'evenement anterieur */
	/*A a la date donnee                              */
	select 
		@vl_Horodate = max(horodate_validation) 
	from EXP..FMC_HIS
	where	EXP..FMC_HIS.numero = @va_NumEvt_in and 
		EXP..FMC_HIS.cle = @va_CleEvt_in and 
		EXP..FMC_HIS.horodate_validation <= @va_Horodate_in

	if @@rowcount = 0
	begin
		return XDC_NOK
	end

	/*A lecture de la fiche main courante avec son historique */
	select 
		/* lecture des informations de FMC_GEN */
		@va_Numero_out		= EXP..FMC_GEN.numero,
		@va_Cle_out		= EXP..FMC_GEN.cle,
		@va_Site_out		= EXP..FMC_GEN.sit,
		@va_Type_out		= EXP..FMC_GEN.type,
		@va_DebutPrevu_out	= EXP..FMC_GEN.debut_prevu,
		@va_Debut_out		= EXP..FMC_GEN.debut,
		@va_FinPrevu_out	= EXP..FMC_GEN.fin_prevue,
		@va_Fin_out		= EXP..FMC_GEN.fin,
		@va_NumCause_out	= EXP..FMC_GEN.cause,
		@va_CleCause_out	= EXP..FMC_GEN.cle_cause,
		@va_NumAlerte_out	= EXP..FMC_GEN.alerte,
		@va_SiteAlerte_out	= EXP..FMC_GEN.sit_alerte,
		@va_Origine_out		= EXP..FMC_GEN.origine_creation,
		@va_Degats_out		= EXP..FMC_GEN.degats_domaine,
		@va_FausseAlerte_out	= EXP..FMC_GEN.fausse_alerte,
		@va_Degrade_out		= EXP..FMC_GEN.degrade,
		@va_NumEvtInit_out	= EXP..FMC_GEN.num_fmc_init,
		@va_CleEvtInit_out	= EXP..FMC_GEN.cle_fmc_init,
		@va_ComEvtInit_out	= EXP..FMC_GEN.com_fmc_init,
		@va_Datex_out		= EXP..FMC_GEN.datex,
	
		/* lecture de l'historique de la fiche main courante */
		@va_TypeAnterieur_out	= EXP..FMC_HIS.type_anterieur,
		@va_Validation_out	= EXP..FMC_HIS.horodate_validation,
		@va_Autoroute_out	= EXP..FMC_HIS.autoroute,
		@va_PR_out		= EXP..FMC_HIS.PR,
		@va_Sens_out		= EXP..FMC_HIS.sens,
		@va_PointCar_out	= EXP..FMC_HIS.point_caracteristique,
		@vl_NumPointCar		= EXP..FMC_HIS.numero_point_caracteristique,
		@va_Position_out	= EXP..FMC_HIS.position_sur_aire_ou_echangeur,
		@va_Confirme_out	= EXP..FMC_HIS.confirme_ou_signale,
		@va_VR_out		= EXP..FMC_HIS.VR,
		@va_VM2_out		= EXP..FMC_HIS.VM2,
		@va_VM1_out		= EXP..FMC_HIS.VM1,
		@va_VL_out		= EXP..FMC_HIS.VL,
		@va_BAU_out		= EXP..FMC_HIS.BAU,
		@va_VR_I_out		= EXP..FMC_HIS.VR_I,
		@va_VM2_I_out		= EXP..FMC_HIS.VM2_I,
		@va_VM1_I_out		= EXP..FMC_HIS.VM1_I,
		@va_VL_I_out		= EXP..FMC_HIS.VL_I,
		@va_BAU_I_out		= EXP..FMC_HIS.BAU_I,
		@va_BAU_Etroite_out	= EXP..FMC_HIS.BAU_etroite,
		@va_BAU_I_Etroite_out	= EXP..FMC_HIS.BAU_I_etroite,
		@va_Longueur_out	= EXP..FMC_HIS.longueur,
		@vl_Classe		= CFG..TYP_FMC.classe,
		@va_Majeur_out		= EXP..FMC_HIS.majeur,
		@va_Ecoulement_out	= EXP..FMC_HIS.ecoulement
	from EXP..FMC_GEN, EXP..FMC_HIS, CFG..TYP_FMC
	where	EXP..FMC_GEN.numero = @va_NumEvt_in and 
		EXP..FMC_GEN.cle = @va_CleEvt_in and 
		EXP..FMC_HIS.numero = @va_NumEvt_in and 
		EXP..FMC_HIS.cle = @va_CleEvt_in and 
		EXP..FMC_HIS.horodate_validation = @vl_Horodate and
		CFG..TYP_FMC.numero = EXP..FMC_GEN.type

	/* test si une fiche a ete trouvee sinon erreur */
	if @@rowcount = 0
	begin
		select @va_Confirme_out = XDC_NON
		return XDC_NOK
	end

	if @va_Datex_out=null
		select @va_Datex_out=XDC_FAUX
			
	/*A Recherche la localisation de la cause de l'evenement et */
	/*A la date de debut de la cause                            */
	if @va_NumCause_out != null and @va_CleCause_out != null
	begin
		select 
			@va_AutorouteCause_out	= EXP..FMC_HIS.autoroute,
			@va_PRCause_out		= EXP..FMC_HIS.PR,
			@va_SensCause_out	= EXP..FMC_HIS.sens,
			@va_HoroCause_out	= isnull(EXP..FMC_GEN.debut, 
							EXP..FMC_GEN.debut_prevu)
		from EXP..FMC_GEN, EXP..FMC_HIS
		where	EXP..FMC_GEN.numero = @va_NumCause_out and
			EXP..FMC_GEN.cle = @va_CleCause_out and
			EXP..FMC_HIS.numero = EXP..FMC_GEN.numero and
			EXP..FMC_HIS.cle = EXP..FMC_GEN.cle 
			group by EXP..FMC_GEN.numero, EXP..FMC_GEN.cle,
				 EXP..FMC_HIS.numero, EXP..FMC_HIS.cle
			having EXP..FMC_HIS.horodate_validation = 
				min(EXP..FMC_HIS.horodate_validation)
	end

	/*A Recherche la localisation de la fmc initiale et */
	/*A la date de debut de la fmc initiale */
	if @va_NumEvtInit_out != null and @va_CleEvtInit_out != null
	begin
		select 
				@va_TypeEvtInit_out	= EXP..FMC_HIS.type_FMC_valide,
				@va_AutorouteEvtInit_out	= EXP..FMC_HIS.autoroute,
				@va_PREvtInit_out	= EXP..FMC_HIS.PR,
				@va_SensEvtInit_out	= EXP..FMC_HIS.sens,
				@va_HoroEvtInit_out	= isnull(EXP..FMC_GEN.debut, 
								EXP..FMC_GEN.debut_prevu)
		from EXP..FMC_GEN, EXP..FMC_HIS
		where	EXP..FMC_GEN.numero = @va_NumEvtInit_out and
			EXP..FMC_GEN.cle = @va_CleEvtInit_out and
			EXP..FMC_HIS.numero = EXP..FMC_GEN.numero and
			EXP..FMC_HIS.cle = EXP..FMC_GEN.cle 
			group by EXP..FMC_GEN.numero, EXP..FMC_GEN.cle,
				 EXP..FMC_HIS.numero, EXP..FMC_HIS.cle
			having EXP..FMC_HIS.horodate_validation = 
				min(EXP..FMC_HIS.horodate_validation)
	end
	
#ifdef PC_SIMPL
	select @va_TypeAlerte_out = XDC_CHAINE_VIDE
#else
	if @va_NumAlerte_out != null
	begin
		/*A Rechercher les informations de l'alerte */
		select
			@va_HoroAlerte_out	= EXP..ALT_EXP.horodate,
			@va_TypeAlerte_out	= CFG..TYP_ALT.nom
		from CFG..TYP_ALT, EXP..ALT_EXP
		where	EXP..ALT_EXP.numero = @va_NumAlerte_out and
			EXP..ALT_EXP.sit = @va_SiteAlerte_out and 
			CFG..TYP_ALT.numero = EXP..ALT_EXP.type

		if @@rowcount = 0
		begin
			select @va_TypeAlerte_out = XDC_CHAINE_VIDE
		end
	end
	/* valuation a chaine " " si pas de alerte */
	/* sinon pb avec open server et client     */
	else	select @va_TypeAlerte_out = XDC_CHAINE_VIDE
#endif

	/*A recherche des commentaires de la fiche main courante */
	select
		numero,
		horodate_validation,
		horodate_fin,
		texte,
		sit
	from EXP..FMC_COM
	where	evenement = @va_NumEvt_in and cle = @va_CleEvt_in and
		horodate_validation <= @va_Horodate_in 
	order by horodate_validation desc

	/*A Recherche le type de la cause de fiche main courante */
	if @va_NumCause_out != null
	begin
		select @va_TypeCause_out = type 
		from EXP..FMC_GEN 
		where numero = @va_NumCause_out and cle = @va_CleCause_out
#ifndef PC_SIMPL
		if @@rowcount = 0
			return XDC_NOK
#endif
	end
		
#ifndef PC_SIMPL
	/*A Recherche les attributs datex et du ftaifc et de la duree */
	if @va_Datex_out = XDC_VRAI 
	begin
		select
			@va_Gravite_out=gravite,
			@va_Trafic_out=trafic,
			@va_Bloquant_out=bloquant,
			@va_Duree_out=duree
		from EXP..FMC_DAT
		where numero= @va_NumEvt_in and 
			cle = @va_CleEvt_in 
			
	end
	else
	begin
		select @va_Trafic_out=0
		select @va_Bloquant_out=XDC_FAUX,@va_Gravite_out=null
		if @va_FinPrevu_out=null
		        select @vl_duree=datediff(hour,@va_Debut_out,getdate())
	 	else
		        select @vl_duree=datediff(hour,@va_Debut_out,@va_FinPrevu_out)
		if @vl_duree<0 or @vl_duree>255
			select @va_Duree_out=255
		else
			select @va_Duree_out=@vl_duree
	end
#endif

#endif
#endif

	/*A recherche du nom du point caracteristique */
	if isnull(@va_PointCar_out,0) != 0 and isnull(@vl_NumPointCar,0) != 0
	begin
		if @va_PointCar_out = XDC_POINT_CARACT_AIRE
		begin
			/*B recherche du nom du point caracteristique de type aire */
			select @va_NomPtCar_out = nom,
			       @va_NumPtCar_out = numero,
			       @va_AbrPtCar_out = abreviation
			from CFG..RES_AIR 
			where	numero = @vl_NumPointCar

			if @@rowcount = 0
				return XDC_NOK
		end
		else if @va_PointCar_out = XDC_POINT_CARACT_ECHANGEUR
		begin
			/*B recherche du nom du point caracteristique de type echangeur */
			select @va_NomPtCar_out = nom,
			       @va_NumPtCar_out = numero,
			       @va_AbrPtCar_out = abreviation
			from CFG..RES_ECH 
			where	numero = @vl_NumPointCar

			if @@rowcount = 0
				return XDC_NOK
		end
		else if @va_PointCar_out = XDC_POINT_CARACT_PEAGE
		begin
			/*B recherche du nom du point caracteristique de type peage */
			select @va_NomPtCar_out = nom,
			       @va_NumPtCar_out = numero,
			       @va_AbrPtCar_out = abreviation
			from CFG..RES_PEA 
			where	numero = @vl_NumPointCar

			if @@rowcount = 0
				return XDC_NOK
		end
		else if @va_PointCar_out = XDC_POINT_CARACT_LIT_ARRET
		begin
			/*B recherche du nom du point caracteristique de type lit d'arret */
			select @va_NomPtCar_out = nom,
			       @va_NumPtCar_out = @vl_NumPointCar,
			       @va_AbrPtCar_out = nom
			from CFG..EQT_GEN
			where	numero = @vl_NumPointCar and type = XDC_CAPT_LIT_ARRET 

			if @@rowcount = 0
				return XDC_NOK
		end
	end

	/*A valuation a chaine " " si pas de point caracteristique */
	/*! sinon pb avec open server et client                    */
	if isnull(@va_PointCar_out,0)=0 
	  or isnull(@vl_NumPointCar,0)=0
	  or @va_NomPtCar_out = null
	  or @va_AbrPtCar_out = null
		select @va_NomPtCar_out = XDC_CHAINE_VIDE,
		       @va_AbrPtCar_out = XDC_CHAINE_VIDE,
		       @va_NumPtCar_out = 0

	select @va_TypeAlerte_out,@va_Origine_out,@va_NomPtCar_out,@va_AbrPtCar_out
	return XDC_OK

go
