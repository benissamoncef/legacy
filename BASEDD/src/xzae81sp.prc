/*E*/
/*  Fichier : $Id: xzae81sp.prc,v 1.27 2019/10/01 19:30:54 devgfi Exp $        $Revision: 1.27 $        $Date: 2019/10/01 19:30:54 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae81p.prc
------------------------------------------------------
* DESCRIPTION DE LA PROCEDURE :
*
* retourne la synthese d'un evt ou operation
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.  14/10/94	: Creation	V 1.1 
* C.T.	14/11/94	: Modif recherche des infos au CI (V 1.2)
* C.T.	14/02/95	: Modif le retour de XZAE;51 (V 1.3)
*                         (PRFin en PRTete)
* C.T.	10/03/95	: Modif le retour de XZAE;51 (V 1.4)
*                         pour le district (PRFin en PRTete)
* C.T.  17/02/95	: Modif longueur ligne 1.5 
* C.T.  07/04/95	: Positionner la config des voies (V 1.6)
* C.T.  05/09/95	: Correction du sens extremite bouchon (V 1.7)
* C.T.  17/11/95	: Inversion longueur et type de bouchon (V 1.8)
* C.T.  08/10/96	: Ajout largeur,hauteur,contenu,poids pour vln (V 1.9) (FMC6)
* C.T.  11/12/96	: Ajout longueur pour escargot (V 1.10) (FMC7)
* C.T.	23/02/98	: Modif default dans creation #SYNTHESE suite a passage en version SQL 11.0.3.2 (1.11)
* C.P.	17/06/98	: Modif separation EXP et HIS (V 1.12)
* P.N.	28/02/05 	: Ajout du cas en accordeon v1.13 DEM428
* CHI   13/04/05        : Ajout sous type meteo "Grele sur chaussee" et "Chaussee mouillee" v1.14 DEM456
* CHI   20/07/05        : ajout classe nature de l'obstacle DEM487 v1.15
* JPL	18/04/07	: Ajout d'un argument aux appels XZAE80 (DEM 638). Renommage v1.16
* JPL	15/05/07	: Prise en compte FMCs Traitement chaussees et Chaussee glissante (DEM 644,646) 1.17
* JPL	07/06/07	: Pas de visibilite pour Traitement Chaussees et Chaussee glissante (DEM 644,646) 1.18
* JPL	10/07/07	: Ajouter les informations Travaux pour un Basculement pour Travaux (DEM 647) 1.19
* JPL	03/09/10	: Prise en compte Fmc Zone de stockage dans la classe Travaux (DEM 942) 1.20
* JPL	21/11/12	: Reunification EXP et HIS  1.21
* JPL	21/11/12	: Ajout des conditions de conduite pour FMC Meteo (DEM 1032)  1.22
* JPL	18/12/12	: Complement de l'indicateur de precipitations (DEM 1032)  1.23
* JMG	08/03/18	: ajout personnel 1.24
* JPL	18/03/19	: Nommage de la base des donnees d'exploitation  1.25
* JPL	18/03/19	: Classe Manifestation historis�e : ajout donn�es de la classe � chaque validation (DEM 1327)  1.26
* JPL	01/10/19	: Basculement pour travaux : indiquer la pr�sence d'un bouchon mobile (DEM 1346)  1.27
* ABE	18/11/21	: Ajout colonne categorie pour les filtres de la synthèse Magistra DEM-SAE311 1.28
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Ajouter le complement d'une fiche courante dans 
* la synthese dans le cas d'evenement historisant
* son complement
* 
* Sequence d'appel
* SP	XZAE81_Intitule_FMC
* 
* Arguments en entree
* XDY_Entier	va_NumEvt_in
* XDC_Octet	va_CleEvt_in
* XDC_Octet	va_Classe_in
* XDC_Mot	va_Type_in
* XDY_Horodate	va_HoroValidation
* XDY_Autoroute	va_Autoroute_in
* XDC_PR	va_PR_in
* XDC_Sens	va_Sens_in
* 
* Arguments en sortie
* 
* Code retour
* XDC_OK	
* XDC_NOK	evenement pas trouve
* < 0          : erreur sybase
* 
* Conditions d'utilisation
* Cette procedure est utilis�e par la procedure XZAE02
* Elle est interne a la base de donn�e.
* 
* La procedure stockee au CI recherche les informations 
* dans la base HIS tandis qu'au niveau des districts la 
* recherche s'effectue dans la base EXP.
* Le code de creation de la procedure au CI et aux districts 
* doit etre identique a l'exception de la base de recherche des infos
* (toute modif dans la creation de la procedure doit etre 
*  effectuee pour le CI et les districts)
* 
* 
* Fonction
* Si l'evenement est de classe Meteo, Vehicule Lent,
* Bouchon, Travaux alors 
* 	rechercher le complement pour l'ajouter a la 
*       synthese de l'evenement
-------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAE81' and type = 'P')
	drop procedure XZAE81
go


#ifdef HIST
	#define BDD_XPL HIS
#else
	#define BDD_XPL EXP
#endif


/* table temporaire partagee avec la procedure XZAE02 (pour compilation) */
create table #SYNTHESE
(
	ligne	char(120) not null,
	heure	datetime null,
	VR	tinyint	null,		-- default	XDC_VOIE_INCONNUE,
	VM2	tinyint	null,		-- default	XDC_VOIE_INCONNUE,
	VM1	tinyint	null,		-- default	XDC_VOIE_INCONNUE,
	VL	tinyint	null,		-- default	XDC_VOIE_INCONNUE,
	BAU	tinyint	null,		-- default	XDC_VOIE_INCONNUE,
	VR_I	tinyint	null,		-- default	XDC_VOIE_INCONNUE,
	VM2_I	tinyint	null,		-- default	XDC_VOIE_INCONNUE,
	VM1_I	tinyint	null,		-- default	XDC_VOIE_INCONNUE,
	VL_I	tinyint	null,		-- default	XDC_VOIE_INCONNUE,
	BAU_I	tinyint	null,		-- default	XDC_VOIE_INCONNUE,
	BAU_etroite	tinyint null,	-- default      XDC_FAUX,
	BAU_I_etroite   tinyint null,	-- default      XDC_FAUX,
	picto   char(4) null,
	categorie tinyint null
)
go


create procedure XZAE81
	@va_NumEvt_in		int		= null,
	@va_CleEvt_in		tinyint		= null,
	@va_Classe_in		tinyint		= null,
	@va_Type_in		smallint	= null,
	@va_HoroValidation_in	datetime	= null,
	@va_Autoroute_in	T_AUTOROUTE	= null,
	@va_PR_in		T_PR		= null,
	@va_Sens_in		T_SENS		= null
as
	declare @vl_Status int,
		@vl_PRTete T_PR, @vl_PRFin T_PR,
		@vl_nature_obstacle char(100),
		@vl_LongVeh smallint, @vl_Vitesse tinyint,
		@vl_Contenu char(25), @vl_Hauteur smallint,
		@vl_Largeur smallint, @vl_Poids smallint,
		@vl_TypeBouchon tinyint, @vl_Longueur int,
		@vl_NumEvtExtremite int, @vl_CleEvtExtremite tinyint,
		@vl_VoiesRet bit, @vl_SensTete T_SENS, @vl_SensQueue T_SENS,
		@vl_Nombre smallint, @vl_Perception bit, @vl_Violence bit,
		@vl_Ligne char(120), @vl_localisation char(60),
		@vl_texte varchar(100),
		@vl_personnel tinyint, @vl_PresenceBouchon tinyint,
		@vl_libelle_localisation_tunnel char(30), @vl_libelle_localisation_tunnel_tet char(30)

	if @va_Classe_in = XZAEC_ClasseMeteo
	begin
		/*A recherche des informations sur la meteo */
		declare	@vl_Visibilite smallint,
			@vl_TypePerturb tinyint, @vl_TypeLibPerturb smallint,
			@vl_txt_pr_deb char(8), @vl_txt_pr_fin char(8), @vl_precipit varchar(3),
			@vl_VR char(2), @vl_VM2 char(2), @vl_VM1 char(2), @vl_VL char(2), @vl_BAU char(2),
			@vl_VR_I char(2), @vl_VM2_I char(2), @vl_VM1_I char(2), @vl_VL_I char(2), @vl_BAU_I char(2)

		select
			@vl_PRFin = PR,
			@vl_Visibilite = visibilite,
			@vl_TypePerturb = type_de_perturbation
		from BDD_XPL..FMC_MET
		where   numero = @va_NumEvt_in and cle = @va_CleEvt_in and
			horodate = @va_HoroValidation_in

		/*A Ajout des informations dans la synthese */
		if @vl_TypePerturb != null
		begin
			if @va_Type_in = XZAEC_FMC_Meteo
				select @vl_TypeLibPerturb = XDC_LIB_TYPE_PERTURBATION
			else if @va_Type_in = XZAEC_FMC_ChausseeGliss
				select @vl_TypeLibPerturb = XDC_TYPE_LIB_CHAUSSEE_GLISS
			else if @va_Type_in = XZAEC_FMC_TraitChaussees
				select @vl_TypeLibPerturb = XDC_TYPE_LIB_TRAIT_CHAUSSEE

			select	@vl_texte = libelle from CFG..LIB_PRE
			where	notyp = @vl_TypeLibPerturb and code = @vl_TypePerturb
			if @@rowcount = 0
				return XZAEC_FMC_ERR_DON_VAL

			select @vl_Ligne = XZAEC_SYN_PAS_DATE + 'type : ' + @vl_texte

--			if @vl_TypePerturb = XZAEC_MET_ChuteNeige
--				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_CHUT_NEI
--			else if @vl_TypePerturb = XZAEC_MET_NeigeChaus
--				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_NEI_CHA
--			else if @vl_TypePerturb = XZAEC_MET_Pluie
--				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_PLU
--			else if @vl_TypePerturb = XZAEC_MET_VentFort
--				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_VEN
--			else if @vl_TypePerturb = XZAEC_MET_Brouillard
--				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_BROUIL
--			else if @vl_TypePerturb = XZAEC_MET_BrouillardGiv
--				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_BRO_GIV
--			else if @vl_TypePerturb = XZAEC_MET_Grele
--				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_GRELE 
--			else if @vl_TypePerturb = XZAEC_MET_Verglas
--				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_VERG
--			else if @vl_TypePerturb = XZAEC_MET_GreleChaus
--				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_GREL_CH
--			else if @vl_TypePerturb = XZAEC_MET_ChausMouill
--				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_CH_MOUI
--			else if @vl_TypePerturb = XZAEC_MET_Autre
--				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_AUT
--			/* cas d'un evenement Chaussee Glissante */
--			else if @vl_TypePerturb = XZAEC_CH_GLIS_Neige
--				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_NEI_CHA
--			else if @vl_TypePerturb = XZAEC_CH_GLIS_Verglas
--				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_VERG
--			else if @vl_TypePerturb = XZAEC_CH_GLIS_Autre
--				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_AUT
--			else if @vl_TypePerturb = XZAEC_CH_GLIS_Grele
--				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_GREL_CH
--			else if @vl_TypePerturb = XZAEC_CH_GLIS_Huile
--				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_CHS_HUILE
--			else if @vl_TypePerturb = XZAEC_CH_GLIS_Carburant
--				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_CHS_CARBURANT
--			else if @vl_TypePerturb = XZAEC_CH_GLIS_Gasoil
--				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_CHS_GASOIL
--			/* cas d'un evenement Traitement Chaussees */
--			else if @vl_TypePerturb = XZAEC_TRT_CH_Salage
--				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_TRT_SALAGE
--			else if @vl_TypePerturb = XZAEC_TRT_CH_Deneigement
--				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_TRT_DENEIG
--			else return XDC_NOK

			insert #SYNTHESE (ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
		end

		if @vl_PRFin != null
		begin
			select @vl_texte = convert (char(8), convert (numeric(8,3), @vl_PRFin / 1000.0))
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_FIN_MET +  @vl_texte

			insert #SYNTHESE (ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
		end

		if @va_Type_in = XZAEC_FMC_Meteo
		begin
			if (@vl_Visibilite != null)  and  (@vl_Visibilite != XDC_VISIBILITE_INCONNUE)
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_VIS_MET + 
				                      convert (varchar(3), @vl_Visibilite) + 'm'
				insert #SYNTHESE (ligne, heure, categorie)
				values(@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
			end

			/*A recherche des informations sur les conditions de conduite */
			declare Pointeur_CCH cursor for
			select	convert (varchar(8), convert (numeric(8,3), PR_debut / 1000.0)),
				convert (varchar(8), convert (numeric(8,3), PR_fin / 1000.0)),
				case when CC.precipitations = XDC_VRAI then "Oui" else "Non" end,
				LP1.libelle, LP2.libelle, LP3.libelle, LP4.libelle, LP5.libelle,
				LP6.libelle, LP7.libelle, LP8.libelle, LP9.libelle, LP10.libelle
			from	BDD_XPL..FMC_MET_CCH CC,
				CFG..LIB_PRE LP1, CFG..LIB_PRE LP2, CFG..LIB_PRE LP3, CFG..LIB_PRE LP4, CFG..LIB_PRE LP5,
				CFG..LIB_PRE LP6, CFG..LIB_PRE LP7, CFG..LIB_PRE LP8, CFG..LIB_PRE LP9, CFG..LIB_PRE LP10
			where	CC.numero = @va_NumEvt_in  and  CC.cle = @va_CleEvt_in  and
				CC.horodate_validation = @va_HoroValidation_in  and
				LP1.notyp = XDC_LIB_TYPE_CCH  and  LP1.code = CC.cch_VR  and
				LP2.notyp = XDC_LIB_TYPE_CCH  and  LP2.code = CC.cch_VM2  and
				LP3.notyp = XDC_LIB_TYPE_CCH  and  LP3.code = CC.cch_VM1  and
				LP4.notyp = XDC_LIB_TYPE_CCH  and  LP4.code = CC.cch_VL  and
				LP5.notyp = XDC_LIB_TYPE_CCH  and  LP5.code = CC.cch_BAU  and
				LP6.notyp = XDC_LIB_TYPE_CCH  and  LP6.code = CC.cch_VR_I  and
				LP7.notyp = XDC_LIB_TYPE_CCH  and  LP7.code = CC.cch_VM2_I  and
				LP8.notyp = XDC_LIB_TYPE_CCH  and  LP8.code = CC.cch_VM1_I  and
				LP9.notyp = XDC_LIB_TYPE_CCH  and  LP9.code = CC.cch_VL_I  and
				LP10.notyp = XDC_LIB_TYPE_CCH  and  LP10.code = CC.cch_BAU_I
			order by PR_debut

			open Pointeur_CCH
			fetch Pointeur_CCH
			into	@vl_txt_pr_deb, @vl_txt_pr_fin, @vl_precipit,
				@vl_VR, @vl_VM2, @vl_VM1, @vl_VL, @vl_BAU,
				@vl_VR_I, @vl_VM2_I, @vl_VM1_I, @vl_VL_I, @vl_BAU_I
			select @vl_Status = @@sqlstatus

			if @vl_Status = 0
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
				                   "Conditions de conduite :          Sens 2                 Sens 1"
				insert #SYNTHESE (ligne, heure, categorie)	values(@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
				                   "                           BAU  VL VM1 VM2  VR     VR VM2 VM1  VL BAU   "
				                      + XZAEC_SYN_PRECIPIT
				insert #SYNTHESE (ligne, heure, categorie)	values(@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
			end
			while @vl_Status = 0
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + "   de " + @vl_txt_pr_deb +
				                   XZAEC_SYN_TXT_A + @vl_txt_pr_fin + "    " +
				                   @vl_BAU_I + "  " + @vl_VL_I + "  " + @vl_VM1_I + "  " +
				                   @vl_VM2_I + "  " + @vl_VR_I + "  |  " +
				                   @vl_VR + "  " + @vl_VM2 + "  " + @vl_VM1 + "  " +
				                   @vl_VL + "  " + @vl_BAU + "        " + @vl_precipit
				insert #SYNTHESE (ligne, heure, categorie)	values(@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)

				fetch Pointeur_CCH
				into	@vl_txt_pr_deb, @vl_txt_pr_fin, @vl_precipit,
					@vl_VR, @vl_VM2, @vl_VM1, @vl_VL, @vl_BAU,
					@vl_VR_I, @vl_VM2_I, @vl_VM1_I, @vl_VL_I, @vl_BAU_I
				select @vl_Status = @@sqlstatus
			end
			close Pointeur_CCH
		end

	end
	else if @va_Classe_in = XZAEC_ClasseNature
	begin
		/*A recherche des informations sur la nature de l'obstacle */
		select
			@vl_nature_obstacle = nature
		from BDD_XPL..FMC_NAT
		where 	numero = @va_NumEvt_in and cle = @va_CleEvt_in and
			horodate = @va_HoroValidation_in

		select @vl_Ligne = XZAEC_SYN_PAS_DATE +
				XZAEC_SYN_NATURE + @vl_nature_obstacle
		insert #SYNTHESE (ligne, heure, categorie)
		values(@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)

	end
	else if @va_Classe_in = XZAEC_ClasseVehLent
	begin
		/*A recherche les informations sur un vehicule lent */
		select
			@vl_LongVeh = longueur,
			@vl_Vitesse = vitesse,
			@vl_Hauteur = hauteur,
			@vl_Largeur = largeur,
			@vl_Poids = poids,
			@vl_Contenu = contenu
		from BDD_XPL..FMC_VLN
		where	numero = @va_NumEvt_in and cle = @va_CleEvt_in and
		horodate = @va_HoroValidation_in

		/*A Ajout des informations dans la synthese */
		if @vl_LongVeh != null
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					XZAEC_SYN_LON_VEH + 
					convert(varchar(8), @vl_LongVeh)
			insert #SYNTHESE (ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
		end

		if @vl_Vitesse != null
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					XZAEC_SYN_VIT_VEH + 
					convert(varchar(8), @vl_Vitesse)
			insert #SYNTHESE (ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
		end

		if @va_Type_in = XZAEC_FMC_ConvoiExc or @va_Type_in = XZAEC_FMC_ConvoiMil
		begin
			if @vl_Hauteur != null
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						XZAEC_SYN_HAU_VEH + 
						convert(varchar(8), @vl_Hauteur)
				insert #SYNTHESE (ligne, heure, categorie)
				values(@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
			end
	
			if @vl_Largeur != null
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						XZAEC_SYN_LRG_VEH + 
						convert(varchar(8), @vl_Largeur)
				insert #SYNTHESE (ligne, heure, categorie)
				values(@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
			end
	
			if @vl_Poids != null
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						XZAEC_SYN_POI_VEH + 
						convert(varchar(8), @vl_Poids)
				insert #SYNTHESE (ligne, heure, categorie)
				values(@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
			end
	
			if @vl_Contenu != null and @vl_Contenu != XDC_CHAINE_VIDE
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						XZAEC_SYN_CNT_VEH + 
						convert(varchar(25), @vl_Contenu)
				insert #SYNTHESE (ligne, heure, categorie)
				values(@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
			end
		end

	end
	else if @va_Classe_in in (XZAEC_ClasseTravaux, XZAEC_ClasseBasculTrav)
	begin
		/*A recherche les informations sur les travaux */
		select
			@vl_PRTete		= localisation_tete,
			@vl_VoiesRet		= voies_retrecies,
			@vl_personnel		= personnel,
			@vl_PresenceBouchon	= bouchon
		from BDD_XPL..FMC_TRH
		where	numero = @va_NumEvt_in and 
			cle = @va_CleEvt_in  and
			horodate = @va_HoroValidation_in

		/*A ajout la localisation dans la synthese */
		if @vl_PRTete != null
		begin
			if @va_Type_in = XZAEC_FMC_ZoneStockage
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_STK_PRT + 
						   convert(char(7), @vl_PRTete/1000.0)
			else
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_PR_TET + 
						   convert(char(7), @vl_PRTete/1000.0)
			insert #SYNTHESE (ligne, heure, categorie)
			values (@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
		end

		if @va_Type_in = XZAEC_FMC_ZoneStockage
		begin
			/*A ajout l'etat du stockage dans la synthese */
			if @vl_VoiesRet = XDC_OUI
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_STK_ENCOURS
			else
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_STK_PREPAR
			insert #SYNTHESE (ligne, heure, categorie)
			values (@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
		end
		else
		if @vl_VoiesRet = XDC_OUI
		begin
			/*A ajout l'existance de voie retrecie dans la synthese */
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_VOI_RET 
			insert #SYNTHESE (ligne, heure, categorie)
			values (@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
		end

		if @vl_personnel = 1
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_PERSONNEL
			insert #SYNTHESE (ligne, heure, categorie)
			values (@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
		end

		/* Pour un Basculement pour travaux, indiquer la pr�sence d'un bouchon mobile */
		if @va_Type_in = XZAEC_FMC_BasculTravaux
		begin
			if @vl_PresenceBouchon = XDC_OUI
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_BOU_MOBILE
				insert #SYNTHESE (ligne, heure, categorie)
				values (@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
			end
		end
	end
	else if @va_Classe_in = XZAEC_ClasseBouchon or
		(@va_Classe_in = XZAEC_ClasseGenerique and 
		 @va_Type_in = XZAEC_FMC_TeteBouchon)
	begin
		/*A recherche les informations sur le bouchon */
		exec @vl_Status = XZAE;51 @va_HoroValidation_in, @va_NumEvt_in,
					@va_CleEvt_in, @vl_PRTete output,
					@vl_TypeBouchon output, 
					@vl_Longueur output,
					@vl_NumEvtExtremite output, 
					@vl_CleEvtExtremite output

		if @vl_Status != XDC_OK
			return XDC_NOK

		/*A recherche le PR de queue de bouchon */
		if @va_Classe_in = XZAEC_ClasseGenerique
		begin
			/* recherche le PR de queue de bouchon */
			select @vl_PRFin = PR, @vl_SensQueue = sens
			from BDD_XPL..FMC_HIS
			where	numero = @vl_NumEvtExtremite and 
				cle = @vl_CleEvtExtremite and
				horodate_validation <=  @va_HoroValidation_in 
			group by numero, cle
			having horodate_validation = max(horodate_validation)

			if @@rowcount != 1
				return XDC_NOK

			/* recherche le sens de la tete de bouchon */
			select @vl_SensTete = @va_Sens_in
		end
		else	
		begin
			/* recherche le sens de tete de bouchon */
			select @vl_SensTete = sens
			from BDD_XPL..FMC_HIS
			where	numero = @vl_NumEvtExtremite and 
				cle = @vl_CleEvtExtremite and
				horodate_validation <=  @va_HoroValidation_in 
			group by numero, cle
			having horodate_validation = max(horodate_validation)

			select @vl_PRFin = @va_PR_in, @vl_SensQueue = @va_Sens_in
		end

		/* mise en forme de la localisation de la tete de bouchon */
		exec XZAE80	null, null, null, null,
				@va_Autoroute_in, @vl_PRTete, @vl_SensTete, 
				@vl_localisation output

		/* recherche si la fmc est dans un tunnel, ajoute le libelle de la niche/IS proche */
		exec PRC..XZAO508 @va_Autoroute_in, @vl_PRTete, @vl_SensTete, @vl_libelle_localisation_tunnel_tet output

		/*A ajout de la localisation de la tete de bouchon */
		select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_BOU_TET + 
					rtrim(@vl_localisation) + " " + @vl_libelle_localisation_tunnel_tet
		insert #SYNTHESE (ligne, heure, categorie)
		values(@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)

		/* mise en forme de la localisation de la queue de bouchon */
		exec XZAE80	null, null, null, null,
				@va_Autoroute_in, @vl_PRFin, @vl_SensQueue, 
				@vl_localisation output

		/* recherche si la fmc est dans un tunnel, ajoute le libelle de la niche/IS proche */
		exec PRC..XZAO508 @va_Autoroute_in, @vl_PRFin, @vl_SensQueue, @vl_libelle_localisation_tunnel output

		/*A ajout de la localisation de la queue de bouchon */
		select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_BOU_QUE + 
					rtrim(@vl_localisation) + " " + @vl_libelle_localisation_tunnel
		insert #SYNTHESE (ligne, heure, categorie)
		values(@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)

		/*A ajout de la longueur du bouchon dans la synthese */
		select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_BOU_LON +
					convert(varchar(7), @vl_Longueur/1000.0)

		insert #SYNTHESE (ligne, heure, categorie)
		values(@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)

		/*A ajout du type du bouchon */
		if @vl_TypeBouchon = XZAEC_BOU_Dense
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
					   XZAEC_SYN_BOU_DNS
		else if @vl_TypeBouchon = XZAEC_BOU_Accordeon
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
					   XZAEC_SYN_BOU_ACD
		else if @vl_TypeBouchon = XZAEC_BOU_Ralenti
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
					   XZAEC_SYN_BOU_RAL
		else if @vl_TypeBouchon = XZAEC_BOU_Bouche
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
					   XZAEC_SYN_BOU_BOU
		else if @vl_TypeBouchon = XZAEC_BOU_Bloque
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
					   XZAEC_SYN_BOU_BLQ
		else return XDC_NOK

		insert #SYNTHESE (ligne, heure, categorie)
		values(@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)

	end

	else if @va_Classe_in = XZAEC_ClasseManif
	begin
		/* recherche les informations specifiques du type Escargot */
		if	(@va_Type_in = XZAEC_FMC_Escargot)
		begin
			select
				@vl_Longueur	= longueur
			from BDD_XPL..FMC_HIS
			where	numero = @va_NumEvt_in and 
				cle = @va_CleEvt_in  and
				horodate_validation = @va_HoroValidation_in

			/*A ajout de la longueur */
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_LON_ESC + 
						convert(varchar(7), @vl_Longueur/1000.0)
			insert #SYNTHESE (ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)

		end

		/* recherche les informations sur la manifestation */
		exec @vl_Status = XZAE;47 @va_HoroValidation_in, @va_NumEvt_in, @va_CleEvt_in,
					@vl_Nombre output, 
					@vl_Perception output,
					@vl_Violence output
		if @vl_Status != XDC_OK
			return XZAEC_FMC_PAS_DONNEES

		/* Ajout des informations dans la synthese */
		if @vl_Nombre != null
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					XZAEC_SYN_NBR_MNF + 
					convert(varchar(8), @vl_Nombre)
			insert #SYNTHESE (ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
		end
		if @vl_Violence = XDC_OUI
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					XZAEC_SYN_VIO_MNF 
			insert #SYNTHESE (ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
		end
		if @vl_Perception = XZAEC_FMC_PAS_PER
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					XZAEC_SYN_PAS_PER_PEA
			insert #SYNTHESE (ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
		end

	end

	return XDC_OK
go

drop table #SYNTHESE
go
