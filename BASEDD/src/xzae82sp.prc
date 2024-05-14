/*E*/
/*  Fichier : $Id: xzae82sp.prc,v 1.55 2021/07/20 11:13:35 pc2dpdy Exp $      Release : $Revision: 1.55 $        Date : $Date: 2021/07/20 11:13:35 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae82sp.prc
------------------------------------------------------
* DESCRIPTION DE LA PROCEDURE :
*
* retourne la synth�se d'un evt ou operation
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.  14/10/94	: Creation	V 1.1 
* C.T.	14/11/94	: Modif recherche des infos au CI (V 1.2)
* C.T.	14/02/95	: Modif format des dates (V 1.3)
*                         rajout du numero d'action pour la synthese
* C.T.	15/02/95	: Correction erreur compile (V 1.4)
* C.T.	15/02/95	: Correction erreur compile (V 1.5)
* C.T.	15/02/95	: Correction erreur compile (V 1.6)
* C.T.	16/02/95	: Ajout du nom de la gare de peage (V 1.8)
* C.T.	14/03/95	: Modif longueur champ affichette 
*                         et nombre de lignes affichees (V 1.10)
* C.T.	22/03/95	: Ajout les PR de debut et fin de balisage (V 1.11)
* C.T.  07/04/95	: Positionner la config des voies (V 1.12)
* C.T.  02/08/95	: Ajout du picto PMV, du resultat de l'action (V 1.13)
* C.T.  01/09/95	: Modif test sur action tunnel (V 1.14)
* C.T.  07/09/95	: Modif ligne sur action tunnel (V 1.15)
*                         et test sur action echangeur
* C.T.  02/10/95	: Modif libelle sens pour A57 (V 1.16)
* C.T.  16/10/95	: Correction erreur de compile au PC (V 1.17)
* C.T.  09/11/95	: Correction texte PMV (V 1.18)
* C.T.  16/11/95	: Ajout resultat du depanneur, modification pictogramme (V 1.19)
* C.T.  30/01/96	: numero action sur 6 digits (V1.20)
* P.V.  14/08/96	: ajout lecture 4eme ligne des pmv (V1.21) (ANA30)
* C.T.  26/09/96	: correction lecture 4eme ligne des pmv dans HIS (V1.22) (ANA30)
* C.T.  13/12/96	: suppression code d'erreur si on trouve pas l'interlocuteur (V1.24)
* C.T.  18/03/97	: suppression code d'erreur si on trouve pas l'intervention (V1.25)
* C.T.  16/01/98	: Ajout action sur PAL, FAC, FAU (V1.26)
* C.T.	23/02/98	: Modif default dans creation SYNTHESE suite a passage en version SQL 11.0.3.2 (1.27)
* JMG	03/03/98	: ajout sit_origine dans les actions (dem/1605) 1.28
* C.T.	03/03/98	: ajout bandeau dans les actions PMV (dem/1607) 1.28
			  correction sur 1607 1.29
* PN	29/06/98	: separation EXP/HIS 1.30
* PN	29/06/98	: Ajout annulation pour intervention dem/1500 1.31
* P.N	12/01/99	: Modifs intergestionnaire dem/1724 (1.32)
			  Correction fin et debut balisage SIS dem/1726
* JMG	21/09/99	: ajout autoroute dans ACT_RMQ 1.33
*			  plus de troncature des messages dans la synthese (SAE146) 
* Hilmarcher 02/01/04   : modif pour plus de troncature des actions venant de EXP (DEM 313, FI 37) v1.34
*                         et autres bugs ...
* JMG	29/04/04	: PAL par scenario 1.35
* JMG	25/10/05	: ajout PMVA 1.36
* PFE	09/11/05	: BUG remplacer HIS..ACT_PMA par EXP..ACT_PMA (ligne 1794 et ligne 1806) 1.36 
* JPL	18/04/07	: Ajout d'un argument aux appels XZAE80 (DEM 638). Renommage 1.37
* AAZ   27/08/2007  : Ajout domaine DEM 663
* JBL   Mai 2008	: DEM 663 Ajout GTC_ECH
* JPL	09/06/09	: Ordre des sections; suppression differences mineures His/Exp 1.40
* JPL	09/06/09	: Ajout GTC_ECH sur serveur Historique (completer DEM 663) 1.41
* JPL	10/07/09	: BAF, BAD : traitement identique sur serveurs His/Exp 1.42
* JMG	15/02/11	: traitement action appel raio 1.43
* LCL	20/07/11	: ajout du rappel client 1.44
* VR	12/12/11 	: Ajout PRV (DEM/1016)
* JPL	14/11/12	: Ajout action d'envoi des conditions de conduite (DEM 1032)  1.46
* JMG	28/11/16	: ajout SAGA 1.47 DEM1191
* JMG	17/08/17	: correction sur @vl_LignePicto dans le cas PMV SAGA 1.48
* PNI	19/09/17	: ajout site origine s'il est different DEM1246 1.49
* LCL	02/03/18	: ajout controleurs de feux CTRL_FEUX 1.50 DEM1284 
* JMG	08/08/18	: ligne PMV 18 car. suppression SAGA DEM1306 et SMS LOT25 DEM1301 1.51
* ABE	07/04/21	: Ajout PAU DEM-SAE244 1.52
* ABE	17/12/20	: Ajout IMU DEM-SAE155 1.55
* ABE	18/11/21	: Ajout colonne categorie pour les filtres de la synthèse Magistra DEM-SAE311 1.56
* PNI	28/11/22	: Suppression du troncage du numero de action + @vl_Numero varchar(7) SAE-441
* GGY	14/11/23	: Ajout PIC (DEM483)
-----------------------------------------------------*/


/*X*/
/*-----------------------------------------------------
* Service rendu
* Ajout des actions dans la synthese d'un evenement
* a une date donnee
* 
* Sequence d'appel
* SP	XZAE82_Synthese_Actions_FMC
* 
* Arguments en entree
* XDY_Entier	va_NumEvt_in
* char(2)	va_CleEvt_in
* 		va_SiteOrigine_in
* char(10)	va_Type_in
* XDC_Horodate	va_Heure_in
* XDC_Octet	va_TypeHeure_in
* XDC_Eqt	va_Eqt_in								
* XDY_Autoroute	va_Autoroute_in
* 
* Arguments en sortie
* 
* Code retour
* XDC_OK	
* XDC_NOK	action non trouvee ou informations
*               complementaires
* XDC_ARG_INV
* < 0          : erreur sybase
* 
* Conditions d'utilisation
* Cette procedure est utilis�e par la procedure XZAE;02
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
* 
-------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAE82' and type = 'P')
	drop procedure XZAE82
go


/* table temporaire partagee avec la procedure XZAE02 */
create table #SYNTHESE
(
	ligne	char(100) not null,
	heure	datetime null,
	VR	tinyint	null,	-- default	XDC_VOIE_INCONNUE,
	VM2	tinyint	null,	-- default	XDC_VOIE_INCONNUE,
	VM1	tinyint	null,	-- default	XDC_VOIE_INCONNUE,
	VL	tinyint	null,	-- default	XDC_VOIE_INCONNUE,
	BAU	tinyint	null,	-- default	XDC_VOIE_INCONNUE,
	VR_I	tinyint	null,	-- default	XDC_VOIE_INCONNUE,
	VM2_I	tinyint	null,	-- default	XDC_VOIE_INCONNUE,
	VM1_I	tinyint	null,	-- default	XDC_VOIE_INCONNUE,
	VL_I	tinyint	null,	-- default	XDC_VOIE_INCONNUE,
	BAU_I	tinyint	null,	-- default	XDC_VOIE_INCONNUE,
	BAU_etroite	tinyint null,	-- default	XDC_FAUX,
	BAU_I_etroite	tinyint null,	-- default	XDC_FAUX,
	picto	char(4) null,
	categorie tinyint null
)
go

#ifdef HIST
	/*! CREATION DE LA PROCEDURE AU CI */

create procedure XZAE82
	@va_NumAct_in		int		= null,
	@va_CleAct_in		tinyint		= null,
	@va_SiteOrigine_in	tinyint		= null,
	@va_Type_in		tinyint		= null,
	@va_Heure_in		datetime	= null,
	@va_TypeHeure_in	tinyint		= null,
	@va_Eqt_in		T_EQUIPEMENT	= null,
	@va_Autoroute_in	T_AUTOROUTE	= null
as
	declare @vl_Ligne char(100), @vl_LignePicto char(80), @vl_NomEqt char(10), 
		@vl_DirectLigne char(18), @vl_Bandeau char(10),
		@vl_TypeEqt tinyint, @vl_Interlocuteur smallint,
		@vl_TypeAstreinte tinyint, @vl_NomTypeAstreinte T_NOM,
		@vl_Diffusion smallint, @vl_Texte char(20), 
		@vl_Affichette varchar(250), @vl_PR T_PR, @vl_Sens T_SENS, 
		@vl_Gare T_NOM, @vl_Debut datetime, @vl_Fin datetime, 
		@vl_DebutBalisage datetime, @vl_FinBalisage datetime, 
		@vl_Balisage bit, @vl_NomInterlocuteur T_NOM, 
		@vl_Intervention varchar(25), @vl_Type tinyint, 
		@vl_Scenario char(10), @vl_NomScenario T_NOM, 
		@vl_Sequence tinyint, @vl_Depanneur T_NOM, 
		@vl_Longueur int,  @vl_Resultat char(25), @vl_Pictogramme char(10),
		@vl_NomSequence T_NOM, @vl_Date char(10), @vl_Heure char(8),
		@vl_Action char(12), @vl_ChaineFixe char(20), 
		@vl_ChaineAction varchar(20), @vl_CaractDebut int,
		@vl_Numero varchar(7), @vl_PRDebBalisage T_PR,
		@vl_PRFinBalisage T_PR, @vl_localisation char(60),
		@vl_LibMod1 char(30), @vl_LibMod2 char(30), @vl_LibMod3 char(30),
		@vl_LibMod4 char(30), @vl_LibMod5 char(30),
		@vl_Annulation tinyint,
		@vl_texte1 char(255),@vl_Ligne1 char(18),@vl_Ligne2 char(18),@vl_Ligne3 char(18),@vl_Ligne4 char(18),
		@vl_Gravite tinyint,@vl_Trafic tinyint,@vl_index tinyint, @vl_autoroute tinyint,
		@vl_Scenario_PAL char(6), @vl_NomScenario_PAL char(50),
		@vl_Mod1 tinyint, @vl_Mod2 tinyint, @vl_Mod3 tinyint, @vl_Mod4 tinyint,@vl_Mod6 tinyint, @vl_Mod5 tinyint,
		@vl_numero_telephonique char(100),@vl_barriere smallint, @vl_scenario smallint,
		@vl_l1 char(18), @vl_l2 char(18), @vl_l3 char(18), @vl_l4 char(18), @vl_l5 char(18),
		@vl_type_saga tinyint, @vl_sms tinyint


	/*A Controle des parametres d'entree obligatoire */
	if @va_NumAct_in = null or @va_CleAct_in = null or 
	   @va_Type_in = null or @va_Heure_in = null or @va_TypeHeure_in = null
		return XDC_ARG_INV

	/* conversion du numero d'action en chaine de caracteres */
	select @vl_ChaineAction = convert(char(20), @va_NumAct_in)

	/* calcul la position du premier caractere a prendre */
	select @vl_Longueur = char_length(@vl_ChaineAction), 
		@vl_ChaineFixe = @vl_ChaineAction
	/****************** DEM SAE-441
	if @vl_Longueur < XZAEC_NB_DIGITS_LIB_ACT
	begin
	*******************/

	select @vl_CaractDebut = 1
	select @vl_Numero = substring(@vl_ChaineFixe, @vl_CaractDebut, @vl_Longueur)
	
	/**************  DEM SAE-441
	end
	else    
	begin
		select @vl_CaractDebut = @vl_Longueur - (XZAEC_NB_DIGITS_LIB_ACT - 1)
		select @vl_Numero = substring(@vl_ChaineFixe, @vl_CaractDebut, XZAEC_NB_DIGITS_LIB_ACT)
	end
	****************************/

	/*A formatage de la date et du numero d'action */
	select	@vl_Date = convert(char(10), @va_Heure_in, 103),
		@vl_Heure = convert(char(8), @va_Heure_in, 8)

	if @va_CleAct_in != @va_SiteOrigine_in
	begin
		select	@vl_Action = @vl_Numero + '-' + convert(char(2), @va_CleAct_in) + ' ' + convert(char(2), @va_SiteOrigine_in)
	end
	else
	begin
		select	@vl_Action = @vl_Numero + '-' + convert(char(2), @va_CleAct_in) + '   ' 
	end

	/*A Creation l'intitule de l'action  dans la synthese */
	if @va_TypeHeure_in = XZAEC_ACT_SUC
		select @vl_Ligne = @vl_Date + ' ' + @vl_Heure +
				' ' + @vl_Action + XZAEC_SYN_ACT_DEB 
	else
	if @va_TypeHeure_in = XZAEC_ACT_ECHEC and ( @va_Type_in = XDC_ACT_EXP or @va_Type_in = XDC_ACT_EXPTFM)
		select @vl_Ligne = @vl_Date + ' ' + @vl_Heure +
				' ' + @vl_Action + XZAEC_SYN_ACT_ECHEC
	else	select @vl_Ligne = @vl_Date + ' ' + @vl_Heure +
				' ' + @vl_Action + XZAEC_SYN_ACT_FIN 

	/*A recherche les informations complementaires relatives */
	/*A a une action                                         */
	if @va_Type_in = XDC_ACT_APPEL or @va_Type_in = XDC_ACT_FAX or
	   @va_Type_in = XDC_ACT_BIP or @va_Type_in = XDC_APPEL_RADIO
	begin
		/*B recherche de l'interlocuteur pour un appel */
		select
			@vl_Interlocuteur = interlocuteur,
			@vl_TypeAstreinte = type_d_astreinte,
			@vl_numero_telephonique = numero_telephonique,
			@vl_sms = sms
		from HIS..ACT_APL
		where actio = @va_NumAct_in and sit = @va_CleAct_in

		if @@rowcount != 1
			return XDC_NOK

		if @vl_TypeAstreinte is not null
			select @vl_NomTypeAstreinte = nom
			from CFG..TYP_AST
			where numero = @vl_TypeAstreinte

		if @vl_TypeAstreinte is null or @vl_NomTypeAstreinte is null
		begin
			if @va_Type_in = XDC_ACT_FAX and @vl_sms =1
				select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + XZAEC_SYN_ACT_SMS
			else if @va_Type_in = XDC_ACT_FAX
				select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + XZAEC_SYN_ACT_FAX
			else if @va_Type_in = XDC_ACT_TEL
				select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + XZAEC_SYN_ACT_TEL
			else if @va_Type_in = XDC_ACT_BIP
				select @vl_Ligne = rtrim(@vl_Ligne) + ' '  + XZAEC_SYN_ACT_BIP
			else if @va_Type_in = XDC_APPEL_RADIO
				select @vl_Ligne = rtrim(@vl_Ligne) + ' '  + XZAEC_SYN_ACT_RADIO
			else return XDC_NOK
		end
		else
		begin
			if @va_Type_in = XDC_ACT_FAX and @vl_sms =1
				select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + XZAEC_SYN_ACT_SMS_AST + @vl_NomTypeAstreinte
			else if @va_Type_in = XDC_ACT_FAX
				select @vl_Ligne = rtrim(@vl_Ligne) + ' '  + XZAEC_SYN_ACT_FAX_AST + @vl_NomTypeAstreinte
			else if @va_Type_in = XDC_ACT_TEL
				if @vl_TypeAstreinte = XDC_AST_CLIENT
					select @vl_Ligne = rtrim(@vl_Ligne) + ' '  + XZAEC_SYN_ACT_TEL_AST + XDC_LIB_AST_CLIENT + ' ' + substring(@vl_numero_telephonique,2,4)
				else
					select @vl_Ligne = rtrim(@vl_Ligne) + ' '  + XZAEC_SYN_ACT_TEL_AST + @vl_NomTypeAstreinte
			else if @va_Type_in = XDC_ACT_BIP
				select @vl_Ligne = rtrim(@vl_Ligne) + ' '  + XZAEC_SYN_ACT_BIP_AST + @vl_NomTypeAstreinte
			else if @va_Type_in = XDC_APPEL_RADIO
				select @vl_Ligne = rtrim(@vl_Ligne) + ' '  + XZAEC_SYN_ACT_RADIO + @vl_NomTypeAstreinte
			else return XDC_NOK
		end

		/*B Ajout de l'intitule de l'action de type appel */
		insert #SYNTHESE(ligne, heure, categorie)
		values(@vl_Ligne, null, XDC_CAT_ACT_APPEL)

		if @vl_Interlocuteur != null 
		begin
			select @vl_NomInterlocuteur = nom
			from CFG..ANN_FIC
			where numero = @vl_Interlocuteur

			if @@rowcount != 1
				select @vl_Ligne = XZAEC_SYN_PAS_DATE
			else	select @vl_Ligne = XZAEC_SYN_PAS_DATE + @vl_NomInterlocuteur
		end
		else	select @vl_Ligne = XZAEC_SYN_PAS_DATE 

		/*B Ajout de l'interlocuteur et du resultat de l'action */
		if (@va_TypeHeure_in = XZAEC_ACT_ECHEC)
		begin
			select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + XZAEC_SYN_ACT_ECHOUEE
			insert #SYNTHESE (ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_APPEL)
		end
		else if (@va_TypeHeure_in = XZAEC_ACT_SUC)
		begin
			select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + XZAEC_SYN_ACT_REUSSIE
			insert #SYNTHESE (ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_APPEL)
		end

	end
	else if @va_Type_in = XDC_ACT_CCH
	begin
		select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + XZAEC_SYN_ACT_CCH

		/*B Ajout de l'intitule de l'action de type CCH */
		insert #SYNTHESE (ligne, heure, categorie)
		values(@vl_Ligne, null, XDC_CAT_ACT_APPEL)

		/*B resultat de l'action */
		if (@va_TypeHeure_in = XZAEC_ACT_ECHEC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_ECHEC
			insert #SYNTHESE (ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_APPEL)
		end
		if (@va_TypeHeure_in = XZAEC_ACT_SUC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_SUC
			insert #SYNTHESE (ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_APPEL)
		end

	end
	else if @va_Type_in = XDC_ACT_NAV or @va_Type_in = XDC_ACT_FAC or @va_Type_in = XDC_ACT_FAU
	begin
		if @va_Type_in = XDC_ACT_NAV
			select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + XZAEC_SYN_ACT_NAV
		else if @va_Type_in = XDC_ACT_FAC
			select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + XZAEC_SYN_ACT_FAC
		else
			select @vl_Ligne = rtrim(@vl_Ligne) + ' '  + XZAEC_SYN_ACT_FAU

		/* ajouter le nom de l'equipement a l'intitule */
		if @va_Eqt_in != null
		begin
			select @vl_NomEqt = nom
			from CFG..EQT_GEN
			where numero = @va_Eqt_in and type = @va_Type_in

			if @@rowcount != 1
				return XDC_NOK

			select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + @vl_NomEqt
		end

		/*B Ajout de l'intitule de l'action de type NAV */
		insert #SYNTHESE (ligne, heure, categorie)
		values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)

		/*B recherche de le scenario de la NAV */
		select
			@vl_Scenario = scenario
		from HIS..ACT_NAV
		where actio = @va_NumAct_in and sit = @va_CleAct_in

		if @@rowcount != 1
			return XDC_NOK

		if @vl_Scenario != XDC_CHAINE_VIDE 
		begin
			select @vl_NomScenario = nom
			from CFG..EQT_SCN_NAV
			where scenario = @vl_Scenario

			if @@rowcount != 1
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_ACT_SCN
			else	select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_SCN + @vl_NomScenario
		end

		/*B ajout du scenario de la NAV  dans la synthese */
		insert #SYNTHESE (ligne, heure, categorie)
		values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)

		/*B resultat de l'action */
		if (@va_TypeHeure_in = XZAEC_ACT_ECHEC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_ECHEC
			insert #SYNTHESE (ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
		end
		if (@va_TypeHeure_in = XZAEC_ACT_SUC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_SUC
			insert #SYNTHESE (ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
		end

	end

        else if @va_Type_in =  XDC_ACT_BAF
        begin
                select @vl_Ligne = @vl_Ligne + XZAEC_SYN_ACT_BAF

                if @va_Eqt_in != null
                begin
                        select @vl_NomEqt = nom
                        from CFG..EQT_GEN
                        where numero = @va_Eqt_in and type = @va_Type_in

                        if @@rowcount != 1
                                return XDC_NOK

                        select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + @vl_NomEqt
                end

                insert #SYNTHESE (ligne, heure, categorie)
                values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)

                select
                        @vl_Mod1 = module11,
                        @vl_Mod2 = module12,
                        @vl_Mod3 = module15,
                        @vl_Mod4 = module21,
                        @vl_Mod5 = module22,
			@vl_Mod6 =module25
                from HIS..ACT_BAF
                where actio = @va_NumAct_in and sit = @va_CleAct_in

                if @@rowcount != 1
                        return XDC_NOK

		if (@vl_Mod1!=0) and (@vl_Mod2!=0) and (@vl_Mod3!=0) 
		begin
			insert #SYNTHESE (ligne, heure, categorie)
			values(XZAEC_SYN_PAS_DATE +XZAEC_SYN_ACT_BAF_GLOBAL_DROIT,null, XDC_CAT_ACT_COMMANDE)
		end
		else if (@vl_Mod1!=0) and (@vl_Mod2!=0)
		begin
			insert #SYNTHESE (ligne, heure, categorie)
			values(XZAEC_SYN_PAS_DATE + XZAEC_SYN_ACT_BAF_FEUX_DROITS,null, XDC_CAT_ACT_COMMANDE)
		end
		else 
		begin
                	if (@vl_Mod1!=0)
                	begin
                        	insert #SYNTHESE (ligne, heure, categorie)
                        	values(XZAEC_SYN_PAS_DATE + XZAEC_SYN_ACT_BAF_MOD1 , null, XDC_CAT_ACT_COMMANDE)
                	end

                	if (@vl_Mod2!=0)
                	begin
                       		insert #SYNTHESE (ligne, heure, categorie)
                        	values(XZAEC_SYN_PAS_DATE + XZAEC_SYN_ACT_BAF_MOD2 , null, XDC_CAT_ACT_COMMANDE)
                	end

                	if (@vl_Mod3!=0)
                	begin
                        	insert #SYNTHESE (ligne, heure, categorie)
                        	values(XZAEC_SYN_PAS_DATE + XZAEC_SYN_ACT_BAF_MOD3 , null, XDC_CAT_ACT_COMMANDE)
                	end
		end


		if (@vl_Mod4!=0) and (@vl_Mod5!=0) and (@vl_Mod6!=0) 
		begin
			insert #SYNTHESE (ligne, heure, categorie)
			values(XZAEC_SYN_PAS_DATE +XZAEC_SYN_ACT_BAF_GLOBAL_GAUCHE,null, XDC_CAT_ACT_COMMANDE)
		end
		else if (@vl_Mod4!=0) and (@vl_Mod5!=0)
		begin
			insert #SYNTHESE (ligne, heure, categorie)
			values(XZAEC_SYN_PAS_DATE + XZAEC_SYN_ACT_BAF_FEUX_GAUCHES,null, XDC_CAT_ACT_COMMANDE)
		end
		else 
		begin
                	if (@vl_Mod4!=0)
			begin
                        	insert #SYNTHESE (ligne, heure, categorie)
                        	values(XZAEC_SYN_PAS_DATE + XZAEC_SYN_ACT_BAF_MOD4 , null, XDC_CAT_ACT_COMMANDE)
                	end

                	if (@vl_Mod5!=0)
                	begin
                        	insert #SYNTHESE (ligne, heure, categorie)
                        	values(XZAEC_SYN_PAS_DATE + XZAEC_SYN_ACT_BAF_MOD5 , null, XDC_CAT_ACT_COMMANDE)
                	end

                	if (@vl_Mod6!=0)
                	begin
                        	insert #SYNTHESE (ligne, heure, categorie)
                        	values(XZAEC_SYN_PAS_DATE + XZAEC_SYN_ACT_BAF_MOD6 , null, XDC_CAT_ACT_COMMANDE)
                	end
		end

                if (@va_TypeHeure_in = XZAEC_ACT_ECHEC)
                begin
                        select @vl_Ligne = XZAEC_SYN_PAS_DATE +
                                           XZAEC_SYN_ACT_RESULT_ECHEC
                        insert #SYNTHESE (ligne, heure, categorie)
                        values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
                end
                if (@va_TypeHeure_in = XZAEC_ACT_SUC)
                begin
                        select @vl_Ligne = XZAEC_SYN_PAS_DATE +
                                           XZAEC_SYN_ACT_RESULT_SUC
                        insert #SYNTHESE (ligne, heure, categorie)
                        values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
                end

        end

        else if @va_Type_in = XDC_ACT_BAD
        begin
                        select @vl_Ligne = @vl_Ligne + XZAEC_SYN_ACT_BAD

                if @va_Eqt_in != null
                begin
                        select @vl_NomEqt = nom
                        from CFG..EQT_GEN
                        where numero = @va_Eqt_in and type = @va_Type_in

                        if @@rowcount != 1
                                return XDC_NOK

                        select @vl_Ligne = @vl_Ligne + ' ' + @vl_NomEqt
                end
                insert #SYNTHESE (ligne, heure, categorie)
                values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)

                select
                        @vl_Scenario = scenario
                from HIS..ACT_BAD
                where actio = @va_NumAct_in and sit = @va_CleAct_in

                if @@rowcount != 1
                        return XDC_NOK

                if @vl_Scenario != XDC_CHAINE_VIDE
                begin
/*                        select @vl_NomScenario = nom
                        from CFG..EQT_SCN_NAV
                        where scenario = @vl_Scenario*/

			select @vl_NomScenario = @vl_Scenario

                        if @@rowcount != 1
                                select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_ACT_SCN + @vl_Scenario
                        else    select @vl_Ligne = XZAEC_SYN_PAS_DATE +
                                           XZAEC_SYN_ACT_SCN + @vl_NomScenario
                end

                insert #SYNTHESE (ligne, heure, categorie)
                values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)

                if (@va_TypeHeure_in = XZAEC_ACT_ECHEC)
                begin
                        select @vl_Ligne = XZAEC_SYN_PAS_DATE +
                                           XZAEC_SYN_ACT_RESULT_ECHEC
                        insert #SYNTHESE (ligne, heure, categorie)
                        values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
                end
                if (@va_TypeHeure_in = XZAEC_ACT_SUC)
                begin
                        select @vl_Ligne = XZAEC_SYN_PAS_DATE +
                                           XZAEC_SYN_ACT_RESULT_SUC
                        insert #SYNTHESE (ligne, heure, categorie)
                        values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
                end

        end

	else if @va_Type_in =  XDC_ACT_PAL
	begin
		select @vl_Ligne = rtrim(@vl_Ligne) + ' '  + XZAEC_SYN_ACT_PAL

		/* ajouter le nom de l'equipement a l'intitule */
		if @va_Eqt_in != null
		begin
			select @vl_NomEqt = nom
			from CFG..EQT_GEN
			where numero = @va_Eqt_in and type = @va_Type_in

			if @@rowcount != 1
				return XDC_NOK

			select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + @vl_NomEqt
		end

		/*B Ajout de l'intitule de l'action de type PAL */
		insert #SYNTHESE (ligne, heure, categorie)
		values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)

		/*B recherche de le libelle des modules du panneau d'alerte */
		select
			@vl_Scenario_PAL = scenario,
			@vl_NomScenario_PAL = nom_scenario
		from HIS..ACT_PAL
		where actio = @va_NumAct_in and sit = @va_CleAct_in

		if @@rowcount != 1
			return XDC_NOK

		select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_SCN + @vl_NomScenario_PAL
					   + ' ' + @vl_Scenario_PAL

		insert #SYNTHESE (ligne, heure, categorie)
		values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)

		/*B resultat de l'action */
		if (@va_TypeHeure_in = XZAEC_ACT_ECHEC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_ECHEC
			insert #SYNTHESE (ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
		end
		if (@va_TypeHeure_in = XZAEC_ACT_SUC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_SUC
			insert #SYNTHESE (ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
		end

	end

	else if @va_Type_in = XDC_ACT_SAGA
	begin
		select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + XZAEC_SYN_ACT_SAGA

		/*rechercher le sous type SAGA*/
		select @vl_type_saga= type_saga from CFG..EQT_SAGA where numero = @va_Eqt_in
		if @@rowcount != 1
			return XDC_NOK
		
		/* ajouter le nom de l'equipement a l'intitule */
		if @va_Eqt_in != null
		begin
			select @vl_NomEqt = nom
			from CFG..EQT_GEN
			where numero = @va_Eqt_in and type = @va_Type_in

			if @@rowcount != 1
				return XDC_NOK

			select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + @vl_NomEqt
		end

		/*B Ajout de l'intitule de l'action de type PMV */
		insert #SYNTHESE (ligne, heure, categorie)
		values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)


		/*recupere les infos de l action*/
		select @vl_l1 = premiere_ligne, @vl_l2 = deuxieme_ligne, @vl_l3 = troisieme_ligne,
			@vl_l4=quatrieme_ligne, @vl_l5 = cinquieme_ligne,
			@vl_Pictogramme = picto,
			@vl_Bandeau = bandeau,
			@vl_barriere = barriere, @vl_scenario = scenario
		from HIS..ACT_SAGA
		where actio = @va_NumAct_in and sit = @va_CleAct_in
		if @@rowcount != 1
			return XDC_NOK

		select @vl_LignePicto =  XZAEC_SYN_PAS_DATE +
				    XZAEC_SYN_ACT_PMV_PCT
		if @vl_type_saga = XDC_SAGA_BAF or @vl_type_saga = XDC_SAGA_BRA
		begin
			insert #SYNTHESE (ligne, heure, categorie)
			values(XZAEC_SYN_PAS_DATE + XZAEC_SYN_ACT_BAD_MOD2, null, XDC_CAT_ACT_COMMANDE)
		end
		else if @vl_type_saga = XDC_SAGA_META_EQT
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_SCEN +  ' '  + convert(char(10),@vl_scenario)
			insert #SYNTHESE (ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
		end
		else if @vl_type_saga = XDC_SAGA_PMVA
		begin
			select
			 @vl_Ligne = XZAEC_SYN_PAS_DATE +
				XZAEC_SYN_ACT_PMVA_TXT +
				@vl_l1 + ' | ' + @vl_l2 + ' | ' + @vl_l3 + ' | ' + @vl_l4 + ' | ' + @vl_l5
			insert #SYNTHESE (ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
		end
		else if @vl_type_saga = XDC_SAGA_PMV_1_18
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_ACT_PMV_TXT + @vl_l1
			insert #SYNTHESE (ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
		end
		else if @vl_type_saga = XDC_SAGA_PMV_2_18 or @vl_type_saga = XDC_SAGA_PMV_2_15
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_ACT_PMV_TXT + @vl_l1 + ' | ' + @vl_l2
			insert #SYNTHESE (ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
		end
		else if @vl_type_saga = XDC_SAGA_PMV_3_15
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_ACT_PMV_TXT + @vl_l1 + ' | ' + @vl_l2 + ' | ' + @vl_l3 
			insert #SYNTHESE (ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
		end
		else if @vl_type_saga = XDC_SAGA_PMV_2_18_P
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_ACT_PMV_TXT + @vl_l1 + ' | ' + @vl_l2
			insert #SYNTHESE (ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
			insert #SYNTHESE(ligne, heure, picto, categorie)
			values(@vl_LignePicto, null, @vl_Pictogramme, XDC_CAT_ACT_COMMANDE)
		end
		else if @vl_type_saga = XDC_SAGA_PMV_3_15_P
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_ACT_PMV_TXT + @vl_l1 + ' | ' + @vl_l2 + ' | ' + @vl_l3
			insert #SYNTHESE (ligne, heure, categorie)
			values(isnull(@vl_Ligne,""), null, XDC_CAT_ACT_COMMANDE)
			insert #SYNTHESE(ligne, heure, picto, categorie)
			values(isnull(@vl_LignePicto,""), null, @vl_Pictogramme, XDC_CAT_ACT_COMMANDE)
		end

		/*B resultat de l'action */
		if (@va_TypeHeure_in = XZAEC_ACT_ECHEC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_ECHEC
			insert #SYNTHESE (ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
		end
		if (@va_TypeHeure_in = XZAEC_ACT_SUC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_SUC
			insert #SYNTHESE (ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
		end

	end

	else if @va_Type_in =  XDC_ACT_CFE
        begin
                select @vl_Ligne = rtrim(@vl_Ligne) + ' ' +XZAEC_SYN_ACT_CFE
                if @va_Eqt_in != null
                begin
                        select @vl_NomEqt = nom
                        from CFG..EQT_GEN
                        where numero = @va_Eqt_in and type = @va_Type_in
                        if @@rowcount != 1
                                return XDC_NOK
                        select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + @vl_NomEqt
                end

                insert #SYNTHESE (ligne, heure, categorie)
                values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)

                select
                        @vl_Ligne = XZAEC_SYN_PAS_DATE +
                                    XZAEC_SYN_ACT_CFE_TXT +
                                    libelle_plan 
                from HIS..ACT_CFE
                where actio = @va_NumAct_in and sit = @va_CleAct_in

                if @@rowcount != 1
                        return XDC_NOK

                insert #SYNTHESE (ligne, heure, categorie)
                values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)

                if (@va_TypeHeure_in = XZAEC_ACT_ECHEC)
                begin
                        select @vl_Ligne = XZAEC_SYN_PAS_DATE +
                                           XZAEC_SYN_ACT_RESULT_ECHEC
                        insert #SYNTHESE (ligne, heure, categorie)
                        values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
                end
                if (@va_TypeHeure_in = XZAEC_ACT_SUC)
                begin
                        select @vl_Ligne = XZAEC_SYN_PAS_DATE +
                                           XZAEC_SYN_ACT_RESULT_SUC
                        insert #SYNTHESE (ligne, heure, categorie)
                        values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
                end
        end

	else if @va_Type_in =  XDC_ACT_IMU 
        begin 
                select @vl_Ligne = rtrim(@vl_Ligne) + ' ' +XZAEC_SYN_ACT_IMU 
                if @va_Eqt_in != null 
                begin 
                        select @vl_NomEqt = nom 
                        from CFG..EQT_GEN 
                        where numero = @va_Eqt_in and type = @va_Type_in 
                        if @@rowcount != 1 
                                return XDC_NOK 
                        select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + @vl_NomEqt 
                end 
 
                insert #SYNTHESE (ligne, heure, categorie) 
                values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE) 
 
                select 
                        @vl_Ligne = XZAEC_SYN_PAS_DATE + 
                                    XZAEC_SYN_ACT_IMU_TXT + 
                                    "chaine:" + chaine + " intervalle:" + convert(char(6),intervalle) + " Nb cycles:" + convert(char(6),cycles)								
                from HIS..ACT_IMU 
                where actio = @va_NumAct_in and sit = @va_CleAct_in 
 
                if @@rowcount != 1 
                        return XDC_NOK 
 
                insert #SYNTHESE (ligne, heure, categorie) 
                values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE) 								
 
                if (@va_TypeHeure_in = XZAEC_ACT_ECHEC) 
                begin 
                        select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
                                           XZAEC_SYN_ACT_RESULT_ECHEC 
                        insert #SYNTHESE (ligne, heure, categorie) 
                        values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE) 
                end 
                if (@va_TypeHeure_in = XZAEC_ACT_SUC) 
                begin 
                        select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
                                           XZAEC_SYN_ACT_RESULT_SUC 
                        insert #SYNTHESE (ligne, heure, categorie) 
                        values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE) 
                end 
        end

	else if @va_Type_in =  XDC_ACT_PAU 
        begin 
                select @vl_Ligne = rtrim(@vl_Ligne) + ' ' +XZAEC_SYN_ACT_PAU 
                if @va_Eqt_in != null 
                begin 
                        select @vl_NomEqt = nom 
                        from CFG..EQT_GEN 
                        where numero = @va_Eqt_in and type = XDC_EQT_PAU 
                        if @@rowcount != 1 
                                return XDC_NOK 
                        select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + @vl_NomEqt 
                end 
 
                insert #SYNTHESE (ligne, heure, categorie) 
                values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE) 
 
                select 
                        @vl_Ligne = XZAEC_SYN_PAS_DATE + 
                                XZAEC_SYN_ACT_PAU_TXT + 
                                message + " " + convert(char(2),autoroute) + " " + 
				convert(char(1),sens) + " " +
				convert(char(6),PR_debut) + " au " +
				convert(char(6),PR_fin) + " " +
				convert(char(2),PAUErreur) + "PAU en erreur " +
				convert(char(2), PAUPilotes)+ " PAU Pilotes"

                from HIS..ACT_PAU												 
                where actio = @va_NumAct_in and sit = @va_CleAct_in 
 
                if @@rowcount != 1 
                        return XDC_NOK 
 
                insert #SYNTHESE (ligne, heure, categorie) 
                values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE) 								
 
                if (@va_TypeHeure_in = XZAEC_ACT_ECHEC) 
                begin 
                        select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
                                           XZAEC_SYN_ACT_RESULT_ECHEC 
                        insert #SYNTHESE (ligne, heure, categorie) 
                        values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE) 
                end 
                if (@va_TypeHeure_in = XZAEC_ACT_SUC) 
                begin 
                        select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
                                           XZAEC_SYN_ACT_RESULT_SUC 
                        insert #SYNTHESE (ligne, heure, categorie) 
                        values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE) 
                end 
        end

	else if @va_Type_in =  XDC_ACT_PMV
	begin
		select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + XZAEC_SYN_ACT_PMV
		
		/* ajouter le nom de l'equipement a l'intitule */
		if @va_Eqt_in != null
		begin
			select @vl_NomEqt = nom
			from CFG..EQT_GEN
			where numero = @va_Eqt_in and type = @va_Type_in

			if @@rowcount != 1
				return XDC_NOK

			select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + @vl_NomEqt
		end

		/*B Ajout de l'intitule de l'action de type PMV */
		insert #SYNTHESE (ligne, heure, categorie)
		values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)

		/*B recherche de le texte du PMV */
		/*B et du texte du PMV 4eme ligne*/
		select
			@vl_Ligne = XZAEC_SYN_PAS_DATE +
				    XZAEC_SYN_ACT_PMV_TXT +
				    premiere_ligne + ' | ' + deuxieme_ligne + 
				    ' | ' + troisieme_ligne, 
			@vl_LignePicto =  XZAEC_SYN_PAS_DATE +
				    XZAEC_SYN_ACT_PMV_PCT, 
			@vl_Pictogramme = pictogramme,
			@vl_Bandeau = bandeau,
			@vl_DirectLigne = direction_ligne
		from HIS..ACT_PMV
		where actio = @va_NumAct_in and sit = @va_CleAct_in

		if @@rowcount != 1
			return XDC_NOK

		/*B ajout du texte du PMV  dans la synthese */
		insert #SYNTHESE (ligne, heure, categorie)
		values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
		insert #SYNTHESE(ligne, heure, picto, categorie)
		values(@vl_LignePicto, null, @vl_Pictogramme, XDC_CAT_ACT_COMMANDE)

		/*B ajout du texte du bandeau du PMV  dans la synthese si non null*/
		if (@vl_Bandeau!=null) and (@vl_Bandeau!=XDC_CHAINE_VIDE)
		begin   
			insert #SYNTHESE (ligne, heure, categorie)
			values(XZAEC_SYN_PAS_DATE + XZAEC_SYN_ACT_PMV_BAND + @vl_Bandeau, null, XDC_CAT_ACT_COMMANDE)
		end

		/*B ajout du texte de 4eme ligne du PMV  dans la synthese si non null*/
		if (@vl_DirectLigne!=null) and (@vl_DirectLigne!=XDC_CHAINE_VIDE)
		begin   
			insert #SYNTHESE (ligne, heure, categorie)
			values(XZAEC_SYN_PAS_DATE + XZAEC_SYN_ACT_PMV_TXT_4 + @vl_DirectLigne, null, XDC_CAT_ACT_COMMANDE)
		end


		/*B resultat de l'action */
		if (@va_TypeHeure_in = XZAEC_ACT_ECHEC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_ECHEC
			insert #SYNTHESE (ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
		end
		if (@va_TypeHeure_in = XZAEC_ACT_SUC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_SUC
			insert #SYNTHESE (ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
		end

	end

        else if @va_Type_in =  XDC_ACT_PMVA
        begin
                select @vl_Ligne = rtrim(@vl_Ligne) + ' ' +XZAEC_SYN_ACT_PMVA
                if @va_Eqt_in != null
                begin
                        select @vl_NomEqt = nom
                        from CFG..EQT_GEN
                        where numero = @va_Eqt_in and type = @va_Type_in
                        if @@rowcount != 1
                                return XDC_NOK
                        select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + @vl_NomEqt
                end

                insert #SYNTHESE (ligne, heure, categorie)
                values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)

                select
                        @vl_Ligne = XZAEC_SYN_PAS_DATE +
                                    XZAEC_SYN_ACT_PMVA_TXT +
                                    premiere_ligne + ' | ' + deuxieme_ligne +
                                    ' | ' + troisieme_ligne
                from HIS..ACT_PMA
                where actio = @va_NumAct_in and sit = @va_CleAct_in

                if @@rowcount != 1
                        return XDC_NOK

                insert #SYNTHESE (ligne, heure, categorie)
                values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)

                select
                        @vl_Ligne = XZAEC_SYN_PAS_DATE +
                                    XZAEC_SYN_ACT_PMVA_TXT +
                                    quatrieme_ligne + ' | ' + cinquieme_ligne
                from HIS..ACT_PMA
                where actio = @va_NumAct_in and sit = @va_CleAct_in

                if @@rowcount != 1
                        return XDC_NOK

                insert #SYNTHESE (ligne, heure, categorie)
                values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)

                if (@va_TypeHeure_in = XZAEC_ACT_ECHEC)
                begin
                        select @vl_Ligne = XZAEC_SYN_PAS_DATE +
                                           XZAEC_SYN_ACT_RESULT_ECHEC
                        insert #SYNTHESE (ligne, heure, categorie)
                        values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
                end
                if (@va_TypeHeure_in = XZAEC_ACT_SUC)
                begin
                        select @vl_Ligne = XZAEC_SYN_PAS_DATE +
                                           XZAEC_SYN_ACT_RESULT_SUC
                        insert #SYNTHESE (ligne, heure, categorie)
                        values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
                end
        end

        else if @va_Type_in =  XDC_ACT_PRV
        begin
                select @vl_Ligne = rtrim(@vl_Ligne) + ' ' +XZAEC_SYN_ACT_PRV
                if @va_Eqt_in != null
                begin
                        select @vl_NomEqt = nom
                        from CFG..EQT_GEN
                        where numero = @va_Eqt_in and type = @va_Type_in
                        if @@rowcount != 1
                                return XDC_NOK
                        select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + @vl_NomEqt
                end

                insert #SYNTHESE (ligne, heure, categorie)
                values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)

                select
                        @vl_Ligne = XZAEC_SYN_PAS_DATE +
                                    XZAEC_SYN_ACT_PRV_TXT +
                                    vitesse + ' | ' + bandeau
                from HIS..ACT_PRV
                where actio = @va_NumAct_in and sit = @va_CleAct_in

                if @@rowcount != 1
                        return XDC_NOK

                insert #SYNTHESE (ligne, heure, categorie)
                values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)

                if (@va_TypeHeure_in = XZAEC_ACT_ECHEC)
                begin
                        select @vl_Ligne = XZAEC_SYN_PAS_DATE +
                                           XZAEC_SYN_ACT_RESULT_ECHEC
                        insert #SYNTHESE (ligne, heure, categorie)
                        values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
                end
                if (@va_TypeHeure_in = XZAEC_ACT_SUC)
                begin
                        select @vl_Ligne = XZAEC_SYN_PAS_DATE +
                                           XZAEC_SYN_ACT_RESULT_SUC
                        insert #SYNTHESE (ligne, heure, categorie)
                        values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
                end
        end

	else if @va_Type_in =  XDC_ACT_PIC
	begin
		select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + XZAEC_SYN_ACT_PIC
		if @va_Eqt_in != null
		begin
			select @vl_NomEqt = nom
				from CFG..EQT_GEN
					where numero = @va_Eqt_in and type = @va_Type_in
			if @@rowcount != 1
				return XDC_NOK
			select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + @vl_NomEqt
		end
		
		insert #SYNTHESE (ligne, heure, categorie)
			values (@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
		
		select	@vl_Ligne = XZAEC_SYN_PAS_DATE +
				XZAEC_SYN_ACT_PIC_TXT +
				pictogramme + ' | ' + bandeau
			from HIS..ACT_PIC
		where actio = @va_NumAct_in and sit = @va_CleAct_in
		
		if @@rowcount != 1
		        return XDC_NOK
		
		insert #SYNTHESE (ligne, heure, categorie)
			values (@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
		
		if (@va_TypeHeure_in = XZAEC_ACT_ECHEC)
		begin
			select @vl_Ligne =	XZAEC_SYN_PAS_DATE +
								XZAEC_SYN_ACT_RESULT_ECHEC
			insert #SYNTHESE (ligne, heure, categorie)
				values (@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
		end
		if (@va_TypeHeure_in = XZAEC_ACT_SUC)
		begin
			select @vl_Ligne =	XZAEC_SYN_PAS_DATE +
								XZAEC_SYN_ACT_RESULT_SUC
			insert #SYNTHESE (ligne, heure, categorie)
				values (@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
		end
	end

	else if @va_Type_in =  XDC_ACT_PMVDAT
	begin
		select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + XZAEC_SYN_ACT_PMVDAT

		/* ajouter le nom de l'equipement a l'intitule */
		select @vl_NomEqt = PMV,
			@vl_texte1=texte1
		from HIS..ACT_PMV_ADA
		where actio = @va_NumAct_in and sit = @va_CleAct_in

		if @@rowcount != 1
			return XDC_NOK

		select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + @vl_NomEqt

		select @vl_index=patindex("%/%",@vl_texte1)
		select @vl_Pictogramme = substring(substring(@vl_texte1,1,@vl_index-1),1,4)
		select @vl_texte1 = substring(@vl_texte1,@vl_index+1,char_length(@vl_texte1))
		
		select @vl_index=patindex("%/%",@vl_texte1)
		select @vl_Bandeau = substring(substring(@vl_texte1,1,@vl_index-1),1,6)
		select @vl_texte1 = substring(@vl_texte1,@vl_index+1,char_length(@vl_texte1))
		
		select @vl_index=patindex("%/%",@vl_texte1)
		select @vl_Ligne1 = substring(substring(@vl_texte1,1,@vl_index-1),1,15)
		select @vl_texte1 = substring(@vl_texte1,@vl_index+1,char_length(@vl_texte1))
		
		select @vl_index=patindex("%/%",@vl_texte1)
		select @vl_Ligne2 = substring(substring(@vl_texte1,1,@vl_index-1),1,15)
		select @vl_texte1 = substring(@vl_texte1,@vl_index+1,char_length(@vl_texte1))
		
		select @vl_index=patindex("%/%",@vl_texte1)
		select @vl_Ligne3 = substring(substring(@vl_texte1,1,@vl_index-1),1,15)
		select @vl_Ligne4 = substring(substring(@vl_texte1,@vl_index+1,char_length(@vl_texte1)),1,15)
		
		
		/*B Ajout de l'intitule de l'action de type PMV */
		insert #SYNTHESE (ligne, heure, categorie)
		values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)

		/*B recherche de le texte du PMV */
		/*B et du texte du PMV 4eme ligne*/
		select
			@vl_Ligne = substring(XZAEC_SYN_PAS_DATE +
				    XZAEC_SYN_ACT_PMV_TXT + @vl_Ligne1 + "|" + @vl_Ligne2 + 
				    "|" + @vl_Ligne3 + "|" + @vl_Ligne4,1,100),
			@vl_LignePicto =  XZAEC_SYN_PAS_DATE +
				    XZAEC_SYN_ACT_PMV_PCT

		if @@rowcount != 1
			return XDC_NOK

		/*B ajout du texte du PMV  dans la synthese */
		insert #SYNTHESE (ligne, heure, categorie)
		values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
		insert #SYNTHESE(ligne, heure, picto, categorie)
		values(@vl_LignePicto, null, @vl_Pictogramme, XDC_CAT_ACT_COMMANDE)

		/*B ajout du texte du bandeau du PMV  dans la synthese si non null*/
		if (@vl_Bandeau!=null) and (@vl_Bandeau!=XDC_CHAINE_VIDE)
		begin   
			insert #SYNTHESE (ligne, heure, categorie)
			values(XZAEC_SYN_PAS_DATE + XZAEC_SYN_ACT_PMV_BAND + @vl_Bandeau, null, XDC_CAT_ACT_COMMANDE)
		end

		/*B ajout du texte de 4eme ligne du PMV  dans la synthese si non null*/
		if (@vl_DirectLigne!=null) and (@vl_DirectLigne!=XDC_CHAINE_VIDE)
		begin   
			insert #SYNTHESE (ligne, heure, categorie)
			values(XZAEC_SYN_PAS_DATE + XZAEC_SYN_ACT_PMV_TXT_4 + @vl_DirectLigne, null, XDC_CAT_ACT_COMMANDE)
		end


		/*B resultat de l'action */
		if (@va_TypeHeure_in = XZAEC_ACT_ECHEC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_ECHEC
			insert #SYNTHESE (ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
		end
		if (@va_TypeHeure_in = XZAEC_ACT_SUC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_SUC
			insert #SYNTHESE (ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
		end

	end

	else if @va_Type_in = XDC_ACT_TUNNEL
	begin
		/*B Ajout de l'intitule de l'action de type TUB */
		select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + XZAEC_SYN_ACT_TUB

		/* ajouter le nom de l'equipement a l'intitule */
		if @va_Eqt_in != null
		begin
			select @vl_NomEqt = nom
			from CFG..EQT_GEN
			where numero = @va_Eqt_in and type = @va_Type_in

			if @@rowcount != 1
				return XDC_NOK

			select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + @vl_NomEqt
		end

		insert #SYNTHESE (ligne, heure, categorie)
		values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)

		/*B recherche de la sequence du tunnel */
		select
			@vl_Sequence = sequence
		from HIS..ACT_TUB
		where actio = @va_NumAct_in and sit = @va_CleAct_in

		/*B ajout de la sequence du tunnel dans la synthese */
		if @vl_Sequence != null 
		begin
			select @vl_NomSequence = nom
			from CFG..EQT_SEQ_TUB
			where numero = @vl_Sequence

			if @@rowcount = 1
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_SEQ + @vl_NomSequence

				insert #SYNTHESE (ligne, heure, categorie)
				values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
			end
		end

		/*B resultat de l'action */
		if (@va_TypeHeure_in = XZAEC_ACT_ECHEC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_ECHEC
			insert #SYNTHESE (ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
		end
		if (@va_TypeHeure_in = XZAEC_ACT_SUC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_SUC
			insert #SYNTHESE (ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
		end

	end

    /* AAZ 27/08/2007 */

    else if @va_Type_in = XDC_ACT_GTC_SIG
    begin
        /*B Ajout de l'intitule de l'action de type SIGNALISATION */
        select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + XZAEC_SYN_ACT_GTC_SIG

        /* ajouter le nom de l'equipement a l'intitule */
        if @va_Eqt_in != null
        begin
            select @vl_NomEqt = nom
            from   CFG..EQT_GEN
            where  numero = @va_Eqt_in and
                   type   = XDC_EQT_TUBE

            if @@rowcount != 1
                return XDC_NOK

            select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + @vl_NomEqt
        end

        insert #SYNTHESE( ligne,
                          heure,
						  categorie)
                  values( @vl_Ligne,
                          null,
						  XDC_CAT_ACT_COMMANDE)

        /*B recherche de la sequence du tunnel */
        select @vl_Sequence = sequence
        from   HIS..ACT_GTC_SIG
        where  actio = @va_NumAct_in and
               sit   = @va_CleAct_in

        /*B ajout de la sequence du tunnel dans la synthese */
        if @vl_Sequence != null 
        begin
            select @vl_NomSequence = nom
            from   CFG..EQT_SEQ_GTC_SIG
            where  numero = @vl_Sequence
			and type = XDC_EQT_TUBE
			
            if @@rowcount = 1
            begin
                select @vl_Ligne = XZAEC_SYN_PAS_DATE +
                                   XZAEC_SYN_ACT_SEQ + @vl_NomSequence

                insert #SYNTHESE( ligne,
                                  heure,
								  categorie)
                          values( @vl_Ligne,
                                  null,
								  XDC_CAT_ACT_COMMANDE)
            end
        end

        /*B resultat de l'action */
        if (@va_TypeHeure_in = XZAEC_ACT_ECHEC)
        begin
            select @vl_Ligne = XZAEC_SYN_PAS_DATE +
                               XZAEC_SYN_ACT_RESULT_ECHEC
            insert #SYNTHESE( ligne,
                              heure,
							  categorie)
                      values( @vl_Ligne,
                              null,
							  XDC_CAT_ACT_COMMANDE)
        end
        if (@va_TypeHeure_in = XZAEC_ACT_SUC)
        begin
            select @vl_Ligne = XZAEC_SYN_PAS_DATE +
                               XZAEC_SYN_ACT_RESULT_SUC
            insert #SYNTHESE( ligne,
                              heure,
							  categorie)
                      values( @vl_Ligne,
                              null,
							  XDC_CAT_ACT_COMMANDE)
        end

    end

    else if @va_Type_in = XDC_ACT_GTC_SIG_ECH
    begin
        /*B Ajout de l'intitule de l'action de type SIGNALISATION */
        select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + XZAEC_SYN_ACT_GTC_SIG_ECH

        /* ajouter le nom de l'equipement a l'intitule */
        if @va_Eqt_in != null
        begin
            select @vl_NomEqt = nom
            from   CFG..EQT_GEN
            where  numero = @va_Eqt_in and
                   type   = XDC_EQT_ECH

            if @@rowcount != 1
                return XDC_NOK

            select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + @vl_NomEqt
        end

        insert #SYNTHESE( ligne,
                          heure,
						  categorie)
                  values( @vl_Ligne,
                          null,
						  XDC_CAT_ACT_COMMANDE)

        /*B recherche de la sequence du tunnel */
        select @vl_Sequence = sequence
        from   HIS..ACT_GTC_SIG
        where  actio = @va_NumAct_in and
               sit   = @va_CleAct_in

        /*B ajout de la sequence du tunnel dans la synthese */
        if @vl_Sequence != null 
        begin
            select @vl_NomSequence = nom
            from   CFG..EQT_SEQ_GTC_SIG
            where numero = @vl_Sequence
			and type = XDC_EQT_ECH

            if @@rowcount = 1
            begin
                select @vl_Ligne = XZAEC_SYN_PAS_DATE +
                                   XZAEC_SYN_ACT_SEQ + @vl_NomSequence

                insert #SYNTHESE( ligne,
                                  heure,
								  categorie)
                          values( @vl_Ligne,
                                  null,
								  XDC_CAT_ACT_COMMANDE)
            end
        end

        /*B resultat de l'action */
        if (@va_TypeHeure_in = XZAEC_ACT_ECHEC)
        begin
            select @vl_Ligne = XZAEC_SYN_PAS_DATE +
                               XZAEC_SYN_ACT_RESULT_ECHEC
            insert #SYNTHESE( ligne,
                              heure,
							  categorie)
                      values( @vl_Ligne,
                              null,
							  XDC_CAT_ACT_COMMANDE)
        end
        if (@va_TypeHeure_in = XZAEC_ACT_SUC)
        begin
            select @vl_Ligne = XZAEC_SYN_PAS_DATE +
                               XZAEC_SYN_ACT_RESULT_SUC
            insert #SYNTHESE( ligne,
                              heure,
							  categorie)
                      values( @vl_Ligne,
                              null,
							  XDC_CAT_ACT_COMMANDE)
        end

    end

    else if @va_Type_in = XDC_ACT_GTC_EXP
    begin
        /*B Ajout de l'intitule de l'action de type SIGNALISATION */
        select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + XZAEC_SYN_ACT_GTC_EXP

        /* ajouter le nom de l'equipement a l'intitule */
        if @va_Eqt_in != null
        begin
            select @vl_NomEqt = nom
            from   CFG..EQT_GEN
            where  numero = @va_Eqt_in and
                   type   = XDC_EQT_TUBE

            if @@rowcount != 1
                return XDC_NOK

            select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + @vl_NomEqt
        end

        insert #SYNTHESE( ligne,
                          heure,
						  categorie)
                  values( @vl_Ligne,
                          null,
						  XDC_CAT_ACT_COMMANDE)

        /*B recherche de la sequence du tunnel */
        select @vl_Sequence = sequence
        from   HIS..ACT_GTC_EXP
        where  actio = @va_NumAct_in and
               sit   = @va_CleAct_in

        /*B ajout de la sequence du tunnel dans la synthese */
        if @vl_Sequence != null 
        begin
            select @vl_NomSequence = nom
            from   CFG..EQT_SEQ_GTC_EXP
            where  numero = @vl_Sequence
			and type = XDC_EQT_TUBE

            if @@rowcount = 1
            begin
                select @vl_Ligne = XZAEC_SYN_PAS_DATE +
                                   XZAEC_SYN_ACT_SEQ + @vl_NomSequence

                insert #SYNTHESE( ligne,
                                  heure,
								  categorie)
                          values( @vl_Ligne,
                                  null,
						  		XDC_CAT_ACT_COMMANDE)
            end
        end

        /*B resultat de l'action */
        if (@va_TypeHeure_in = XZAEC_ACT_ECHEC)
        begin
            select @vl_Ligne = XZAEC_SYN_PAS_DATE +
                               XZAEC_SYN_ACT_RESULT_ECHEC
            insert #SYNTHESE( ligne,
                              heure,
							  categorie)
                      values( @vl_Ligne,
                              null,
						  	  XDC_CAT_ACT_COMMANDE)
        end
        if (@va_TypeHeure_in = XZAEC_ACT_SUC)
        begin
            select @vl_Ligne = XZAEC_SYN_PAS_DATE +
                               XZAEC_SYN_ACT_RESULT_SUC
            insert #SYNTHESE( ligne,
                              heure,
							  categorie)
                      values( @vl_Ligne,
                              null,
							  XDC_CAT_ACT_COMMANDE)
        end

    end	

    else if @va_Type_in = XDC_ACT_GTC_ECL
    begin
        /*B Ajout de l'intitule de l'action de type ECLAIRAGE */
        select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + XZAEC_SYN_ACT_GTC_ECL

        /* ajouter le nom de l'equipement a l'intitule */
        if @va_Eqt_in != null
        begin
            select @vl_NomEqt = nom
            from   CFG..EQT_GEN
            where  numero = @va_Eqt_in and
                   type   = XDC_EQT_TUBE

            if @@rowcount != 1
                return XDC_NOK

            select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + @vl_NomEqt
        end

        insert #SYNTHESE( ligne,
                          heure,
						  categorie)
                  values( @vl_Ligne,
                          null,
						  XDC_CAT_ACT_COMMANDE)

        /*B recherche de la sequence du tunnel */
        select @vl_Sequence = sequence
        from   HIS..ACT_GTC_ECL
        where  actio = @va_NumAct_in and
               sit   = @va_CleAct_in

        /*B ajout de la sequence du tunnel dans la synthese */
        if @vl_Sequence != null 
        begin
            select @vl_NomSequence = nom
            from   CFG..EQT_SEQ_GTC_ECL
            where  numero = @vl_Sequence
			and type = XDC_EQT_TUBE

            if @@rowcount = 1
            begin
                select @vl_Ligne = XZAEC_SYN_PAS_DATE +
                                   XZAEC_SYN_ACT_SEQ + @vl_NomSequence

                insert #SYNTHESE( ligne,
                                  heure,
								  categorie)
                          values( @vl_Ligne,
                                  null,
								  XDC_CAT_ACT_COMMANDE)
            end
        end

        /*B resultat de l'action */
        if (@va_TypeHeure_in = XZAEC_ACT_ECHEC)
        begin
            select @vl_Ligne = XZAEC_SYN_PAS_DATE +
                               XZAEC_SYN_ACT_RESULT_ECHEC
            insert #SYNTHESE( ligne,
                              heure,
							  categorie)
                      values( @vl_Ligne,
                              null,
							  XDC_CAT_ACT_COMMANDE)
        end
        if (@va_TypeHeure_in = XZAEC_ACT_SUC)
        begin
            select @vl_Ligne = XZAEC_SYN_PAS_DATE +
                               XZAEC_SYN_ACT_RESULT_SUC
            insert #SYNTHESE( ligne,
                              heure,
							  categorie)
                      values( @vl_Ligne,
                              null,
							  XDC_CAT_ACT_COMMANDE)
        end

    end

    else if @va_Type_in = XDC_ACT_GTC_VEN
    begin
        /*B Ajout de l'intitule de l'action de type VENTILATION */
        select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + XZAEC_SYN_ACT_GTC_VEN

        /* ajouter le nom de l'equipement a l'intitule */
        if @va_Eqt_in != null
        begin
            select @vl_NomEqt = nom
            from   CFG..EQT_GEN
            where  numero = @va_Eqt_in and
                   type   = XDC_EQT_TUBE

            if @@rowcount != 1
                return XDC_NOK

            select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + @vl_NomEqt
        end

        insert #SYNTHESE( ligne,
                          heure,
						  categorie)
                  values( @vl_Ligne,
                          null,
						  XDC_CAT_ACT_COMMANDE)

        /*B recherche de la sequence du tunnel */
        select @vl_Sequence = sequence
        from   HIS..ACT_GTC_VEN
        where  actio = @va_NumAct_in and
               sit   = @va_CleAct_in

        /*B ajout de la sequence du tunnel dans la synthese */
        if @vl_Sequence != null 
        begin
            select @vl_NomSequence = nom
            from   CFG..EQT_SEQ_GTC_VEN
            where  numero = @vl_Sequence
			and type = XDC_EQT_TUBE

            if @@rowcount = 1
            begin
                select @vl_Ligne = XZAEC_SYN_PAS_DATE +
                                   XZAEC_SYN_ACT_SEQ + @vl_NomSequence

                insert #SYNTHESE( ligne,
                                  heure,
								  categorie)
                          values( @vl_Ligne,
                                  null,
								  XDC_CAT_ACT_COMMANDE)
            end
        end

        /*B resultat de l'action */
        if (@va_TypeHeure_in = XZAEC_ACT_ECHEC)
        begin
            select @vl_Ligne = XZAEC_SYN_PAS_DATE +
                               XZAEC_SYN_ACT_RESULT_ECHEC
            insert #SYNTHESE( ligne,
                              heure,
							  categorie)
                      values( @vl_Ligne,
                              null,
							  XDC_CAT_ACT_COMMANDE)
        end
        if (@va_TypeHeure_in = XZAEC_ACT_SUC)
        begin
            select @vl_Ligne = XZAEC_SYN_PAS_DATE +
                               XZAEC_SYN_ACT_RESULT_SUC
            insert #SYNTHESE( ligne,
                              heure,
							  categorie)
                      values( @vl_Ligne,
                              null,
							  XDC_CAT_ACT_COMMANDE)
        end

    end

    else if @va_Type_in = XDC_ACT_GTC_ENR
    begin
        /*B Ajout de l'intitule de l'action de type ENERGIE */
        select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + XZAEC_SYN_ACT_GTC_ENR

        /* ajouter le nom de l'equipement a l'intitule */
        if @va_Eqt_in != null
        begin
            select @vl_NomEqt = nom
            from   CFG..EQT_GEN
            where  numero = @va_Eqt_in and
                   type   = XDC_EQT_TUBE

            if @@rowcount != 1
                return XDC_NOK

            select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + @vl_NomEqt
        end

        insert #SYNTHESE( ligne,
                          heure,
						  categorie)
                  values( @vl_Ligne,
                          null,
						  XDC_CAT_ACT_COMMANDE)

        /*B recherche de la sequence du tunnel */
        select @vl_Sequence = sequence
        from   HIS..ACT_GTC_ENR
        where  actio = @va_NumAct_in and
               sit   = @va_CleAct_in

        /*B ajout de la sequence du tunnel dans la synthese */
        if @vl_Sequence != null 
        begin
            select @vl_NomSequence = nom
            from   CFG..EQT_SEQ_GTC_ENR
            where  numero = @vl_Sequence
			and type = XDC_EQT_TUBE

            if @@rowcount = 1
            begin
                select @vl_Ligne = XZAEC_SYN_PAS_DATE +
                                   XZAEC_SYN_ACT_SEQ + @vl_NomSequence

                insert #SYNTHESE( ligne,
                                  heure,
								  categorie)
                          values( @vl_Ligne,
                                  null,
								  XDC_CAT_ACT_COMMANDE)
            end
        end

        /*B resultat de l'action */
        if (@va_TypeHeure_in = XZAEC_ACT_ECHEC)
        begin
            select @vl_Ligne = XZAEC_SYN_PAS_DATE +
                               XZAEC_SYN_ACT_RESULT_ECHEC
            insert #SYNTHESE( ligne,
                              heure,
							  categorie)
                      values( @vl_Ligne,
                              null,
							  XDC_CAT_ACT_COMMANDE)
        end
        if (@va_TypeHeure_in = XZAEC_ACT_SUC)
        begin
            select @vl_Ligne = XZAEC_SYN_PAS_DATE +
                               XZAEC_SYN_ACT_RESULT_SUC
            insert #SYNTHESE( ligne,
                              heure,
							  categorie)
                      values( @vl_Ligne,
                              null,
							  XDC_CAT_ACT_COMMANDE)
        end

    end

    else if @va_Type_in = XDC_ACT_GTC_INC
    begin
        /*B Ajout de l'intitule de l'action de type PROTECTION INCENDIE */
        select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + XZAEC_SYN_ACT_GTC_INC

        /* ajouter le nom de l'equipement a l'intitule */
        if @va_Eqt_in != null
        begin
            select @vl_NomEqt = nom
            from   CFG..EQT_GEN
            where  numero = @va_Eqt_in and
                   type   = XDC_EQT_TUBE

            if @@rowcount != 1
                return XDC_NOK

            select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + @vl_NomEqt
        end

        insert #SYNTHESE( ligne,
                          heure,
						  categorie)
                  values( @vl_Ligne,
                          null,
						  XDC_CAT_ACT_COMMANDE)

        /*B recherche de la sequence du tunnel */
        select @vl_Sequence = sequence
        from   HIS..ACT_GTC_INC
        where  actio = @va_NumAct_in and
               sit   = @va_CleAct_in

        /*B ajout de la sequence du tunnel dans la synthese */
        if @vl_Sequence != null 
        begin
            select @vl_NomSequence = nom
            from   CFG..EQT_SEQ_GTC_INC
            where  numero = @vl_Sequence
			and type = XDC_EQT_TUBE

            if @@rowcount = 1
            begin
                select @vl_Ligne = XZAEC_SYN_PAS_DATE +
                                   XZAEC_SYN_ACT_SEQ + @vl_NomSequence

                insert #SYNTHESE( ligne,
                                  heure,
								  categorie)
                          values( @vl_Ligne,
                                  null,
								  XDC_CAT_ACT_COMMANDE)
            end
        end

        /*B resultat de l'action */
        if (@va_TypeHeure_in = XZAEC_ACT_ECHEC)
        begin
            select @vl_Ligne = XZAEC_SYN_PAS_DATE +
                               XZAEC_SYN_ACT_RESULT_ECHEC
            insert #SYNTHESE( ligne,
                              heure,
							  categorie)
                      values( @vl_Ligne,
                              null,
							  XDC_CAT_ACT_COMMANDE)
        end
        if (@va_TypeHeure_in = XZAEC_ACT_SUC)
        begin
            select @vl_Ligne = XZAEC_SYN_PAS_DATE +
                               XZAEC_SYN_ACT_RESULT_SUC
            insert #SYNTHESE( ligne,
                              heure,
							  categorie)
                      values( @vl_Ligne,
                              null,
							  XDC_CAT_ACT_COMMANDE)
        end

    end

    /* AAZ */

	else if @va_Type_in = XDC_ACT_DEPANNEUR
	begin
		/*B Ajout de l'intitule de l'action de type depanneur */
		if @va_TypeHeure_in = XZAEC_ACT_FIN
			select @vl_Ligne = @vl_Date + ' ' + @vl_Heure +
					   ' ' + @vl_Action + XZAEC_SYN_ACT_DPT + 
				   	   XZAEC_SYN_ACT_DEP
		else	select @vl_Ligne = @vl_Date + ' ' + @vl_Heure +
					   ' ' + @vl_Action + XZAEC_SYN_ACT_ARR + 
				   	   XZAEC_SYN_ACT_DEP

		/*B recherche le nom du depanneur et annulation */
		select
			@vl_Depanneur = nom,
			@vl_Resultat = resultat,
			@vl_Annulation = annulation_intervention
		from HIS..ACT_DEP
		where actio = @va_NumAct_in and sit = @va_CleAct_in

		if @vl_Annulation = XDC_VRAI
			select @vl_Ligne = rtrim(@vl_Ligne) + ' '  + XZAEC_SYN_ACT_ANNULEE

		insert #SYNTHESE (ligne, heure, categorie)
		values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)

		/*B ajout du nom du depanneur dans la synthese */
		if @vl_Depanneur != XDC_CHAINE_VIDE
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_DEP_NOM +
					   @vl_Depanneur

			insert #SYNTHESE (ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
		end
		if @va_TypeHeure_in = XZAEC_ACT_FIN and @vl_Resultat is not null and
		   @vl_Resultat != XDC_CHAINE_VIDE
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULTAT +
					   @vl_Resultat

			insert #SYNTHESE (ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
		end

	end

	else if @va_Type_in = XDC_ACT_AFF_GARE
	begin
		/* ajout de l'intitule de l'action de type affichage */
		select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + XZAEC_SYN_ACT_GAR

		insert #SYNTHESE (ligne, heure, categorie)
		values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)

		/*B recherche les informations sur l'affichage en gare */
		select 
			@vl_Gare = nom_gare,
			@vl_Affichette = affichette,
			@vl_Debut = horodate_debut,
			@vl_Fin = horodate_fin
		from HIS..ACT_GAR
		where actio = @va_NumAct_in and sit = @va_CleAct_in

		if @@rowcount != 1
			return XDC_NOK

		/*B ajout du nom de la gare de peage dans la synthese */
		select @vl_Ligne = XZAEC_SYN_PAS_DATE +
				   XZAEC_SYN_ACT_GAR_NOM +
				   @vl_Gare

		insert #SYNTHESE (ligne, heure, categorie)
		values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)

		/*B ajout du texte de l'affichette dans la synthese */
		if @vl_Affichette != XDC_CHAINE_VIDE
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_GAR_TXT +
					   substring(@vl_Affichette, 1, 50),
				@vl_Longueur = char_length(@vl_Affichette)

			insert #SYNTHESE (ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)

			if @vl_Longueur > 50
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   	substring(@vl_Affichette, 51, 50)

				insert #SYNTHESE (ligne, heure, categorie)
				values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
			end

			if @vl_Longueur > 100
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   	substring(@vl_Affichette, 101, 50)

				insert #SYNTHESE (ligne, heure, categorie)
				values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
			end

			if @vl_Longueur > 150
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   	substring(@vl_Affichette, 151, 50)

				insert #SYNTHESE (ligne, heure, categorie)
				values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
			end

			if @vl_Longueur > 200
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   	substring(@vl_Affichette, 201, 50)

				insert #SYNTHESE (ligne, heure, categorie)
				values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
			end
		end
	end

	else if @va_Type_in = XDC_ACT_PAT_SIS
	begin
		/*B ajout de l'intitule de l'action  de type SIS */
		if @va_TypeHeure_in = XZAEC_ACT_FIN
			select @vl_Ligne = @vl_Date + ' ' + @vl_Heure +
					   ' ' + @vl_Action + XZAEC_SYN_ACT_DPT
		else	select @vl_Ligne = @vl_Date + ' ' + @vl_Heure +
					   ' ' + @vl_Action + XZAEC_SYN_ACT_ARR

		select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + XZAEC_SYN_ACT_SIS

		/*B recherche les informations sur le balisage */
		select
			@vl_Intervention = nom_patrouille,
			@vl_DebutBalisage = debut_balisage,
			@vl_FinBalisage = fin_balisage,
			@vl_Balisage = pose_balisage,
			@vl_PRDebBalisage = PR_debut,
			@vl_PRFinBalisage = PR_fin,
			@vl_Annulation = annulation_intervention
		from HIS..ACT_SIS
		where actio = @va_NumAct_in and sit = @va_CleAct_in

		if @@rowcount != 1
			return XDC_NOK

		if @vl_Annulation = XDC_VRAI
			select @vl_Ligne = rtrim(@vl_Ligne) + ' '  + XZAEC_SYN_ACT_ANNULEE

		insert #SYNTHESE (ligne, heure, categorie)
		values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)

		/*B ajout des informations sur le balisage dans la synthese */
		if @vl_Balisage = XDC_OUI and @vl_DebutBalisage != null
		begin
			/* texte correspondant a l'enlevement du balisage */
			if @vl_FinBalisage != null and
			   @va_TypeHeure_in = XZAEC_ACT_FIN
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   convert(char(10), @vl_FinBalisage, 103) +
					   ' ' + convert(char(8), @vl_FinBalisage, 8) +
					   ' : ' + @vl_Intervention + ' ' +
					   XZAEC_SYN_ACT_BAL_OTE

				insert #SYNTHESE (ligne, heure, categorie)
				values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
			end
			else
			begin
			/* texte correspondant a la pose du balisage */
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   convert(char(10), @vl_DebutBalisage, 103)+
					   ' ' + convert(char(8), @vl_DebutBalisage, 8)+
					   ' : ' + @vl_Intervention + ' ' +
					    XZAEC_SYN_ACT_BAL_POS

				insert #SYNTHESE (ligne, heure, categorie)
				values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
			end


			/* Localisation du balisage */
			if @vl_PRFinBalisage is not null
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_SIS_DEB_BAL +
					   convert(char(7),@vl_PRFinBalisage/1000.0)

				insert #SYNTHESE (ligne, heure, categorie)
				values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
			end
			if @vl_PRDebBalisage is not null
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_SIS_FIN_BAL +
					   convert(char(7),@vl_PRDebBalisage/1000.0)

				insert #SYNTHESE (ligne, heure, categorie)
				values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
			end

		end
		else if @vl_Intervention != XDC_CHAINE_VIDE
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_SIS_NOM +
					   @vl_Intervention

			insert #SYNTHESE (ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
		end
	end

	else if @va_Type_in = XDC_ACT_INTERVENTION
	begin
		/* recherche le nom de la patrouille et son type */
		select 
			@vl_Intervention = nom,
			@vl_Type = type,
			@vl_Annulation = annulation_intervention
		from HIS..ACT_INT
		where actio = @va_NumAct_in and sit = @va_CleAct_in

		if @@rowcount = 1
		begin
		/* creation de l'intitule de l'action */
		if @va_TypeHeure_in = XZAEC_ACT_FIN
			select @vl_Ligne = @vl_Date + ' ' + @vl_Heure +
					   ' ' + @vl_Action + XZAEC_SYN_ACT_DPT
		else	select @vl_Ligne = @vl_Date + ' ' + @vl_Heure +
					   ' ' + @vl_Action + XZAEC_SYN_ACT_ARR

		if @vl_Type = XDC_ACT_INT_POM
			select @vl_Ligne = rtrim(@vl_Ligne) + ' '  + XZAEC_SYN_ACT_INT_POM
		else if @vl_Type = XDC_ACT_INT_GND
			select @vl_Ligne = rtrim(@vl_Ligne) + ' '  + XZAEC_SYN_ACT_INT_GND

		if  @vl_Annulation = XDC_VRAI
			select @vl_Ligne = rtrim(@vl_Ligne) + ' '  + XZAEC_SYN_ACT_ANNULEE
		/*B ajout de l'intitule de l'action  de type intervention */
		insert #SYNTHESE (ligne, heure, categorie)
		values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)

		if @vl_Intervention != XDC_CHAINE_VIDE
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_INT_NOM +
					   @vl_Intervention

			insert #SYNTHESE (ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
		end
		end
	end

	else if @va_Type_in = XDC_ACT_PMV_REMORQUE
	begin
		/*B Ajout de l'intitule de l'action de type remorque */
		select @vl_Ligne = rtrim(@vl_Ligne) + ' '  + XZAEC_SYN_ACT_RMQ
		insert #SYNTHESE (ligne, heure, categorie)
		values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)

		/*B recherche les informations sur PMV sur remorque */
		select
			@vl_Texte	= message,
			@vl_PR		= PR,
			@vl_Sens	= sens,
			@vl_autoroute 	= autoroute
		from HIS..ACT_RMQ
		where actio = @va_NumAct_in and sit = @va_CleAct_in

		if @@rowcount != 1
			return XDC_NOK

		/*B ajout de la localisation du pmv sur remorque dans la synthese */
		if (@va_TypeHeure_in = XZAEC_ACT_SUC or @va_TypeHeure_in = XZAEC_ACT_ECHEC) and 
		   @vl_PR != null and @vl_Sens != null
		begin

			/* mise en forme de la localisation */
			exec XZAE80	null, null, null, null,
					@vl_autoroute, @vl_PR, @vl_Sens,
					@vl_localisation output

			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RMQ_LOC +
					   @vl_localisation

			insert #SYNTHESE (ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
		end

		/*B ajout du message dans la synthese */
		if @vl_Texte != XDC_CHAINE_VIDE
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RMQ_TXT +
					   substring(@vl_Texte, 1, 48)

			insert #SYNTHESE (ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
		end

		/*B resultat de l'action  dans la synthese */
		if (@va_TypeHeure_in = XZAEC_ACT_ECHEC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_ECHEC
			insert #SYNTHESE (ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
		end
		if (@va_TypeHeure_in = XZAEC_ACT_SUC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_SUC
			insert #SYNTHESE (ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
		end

	end

	else if @va_Type_in = XDC_ACT_PMV_FOURGON
	begin
		/*B Ajout de l'intitule de l'action de type PMV sur fourgon */
		select @vl_Ligne = rtrim(@vl_Ligne) + ' '  + XZAEC_SYN_ACT_FRG
		insert #SYNTHESE (ligne, heure, categorie)
		values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)

		/*B recherche les informations sur PMV sur remorque */
		select
			@vl_Texte	= message,
			@vl_PR		= PR,
			@vl_Sens	= sens,
			@vl_autoroute	= autoroute
		from HIS..ACT_FRG
		where actio = @va_NumAct_in and sit = @va_CleAct_in

		if @@rowcount != 1
			return XDC_NOK

		/*B ajout de la localisation du pmv sur remorque dans la synthese */
		if @va_TypeHeure_in = XZAEC_ACT_SUC and @vl_PR != null and
		   @vl_Sens != null
		begin
			/* mise en forme de la localisation */
			exec XZAE80	null, null, null, null,
					@vl_autoroute, @vl_PR, @vl_Sens,
					@vl_localisation output

			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_FRG_LOC +
					   @vl_localisation

			insert #SYNTHESE (ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
		end

		/*B ajout du message dans la synthese */
		if @vl_Texte != XDC_CHAINE_VIDE
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_FRG_TXT +
					   substring(@vl_Texte, 1, 20)

			insert #SYNTHESE (ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
		end

		/*B resultat de l'action */
		if (@va_TypeHeure_in = XZAEC_ACT_ECHEC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_ECHEC
			insert #SYNTHESE (ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
		end
		if (@va_TypeHeure_in = XZAEC_ACT_SUC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_SUC
			insert #SYNTHESE (ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
		end

	end

	else if @va_Type_in = XDC_ACT_TFM
	begin
		/*B Ajout de l'intitule de l'action de type TFM */
		select @vl_Ligne = rtrim(@vl_Ligne) + ' '  + XZAEC_SYN_ACT_TFM
		insert #SYNTHESE (ligne, heure, categorie)
		values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)

		/*B recherche nombre de diffusions */
		select
			@vl_Diffusion = isnull(nombre_diffusions, 0)
		from HIS..ACT_TFM
		where actio = @va_NumAct_in and sit = @va_CleAct_in

		if @@rowcount != 1
			return XDC_NOK

		/*B ajout du nombre de diffusions dans la synthese */
		select @vl_Ligne = XZAEC_SYN_PAS_DATE +
				   XZAEC_SYN_ACT_TFM_DIFF +
				   convert(varchar(8), @vl_Diffusion)

		insert #SYNTHESE (ligne, heure, categorie)
		values( @vl_Ligne, null, XDC_CAT_ACT_COMMANDE)

		select @vl_Fin = heure_fin 
		from HIS..ACT_GEN
		where numero = @va_NumAct_in and sit = @va_CleAct_in
			
		if @vl_Fin is not null
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
				   XZAEC_SYN_ACT_TFM_ARRET +
				   convert(char(10), @vl_Fin, 103)+' '+convert(char(8), @vl_Fin, 8)

			insert #SYNTHESE (ligne, heure, categorie)
			values( @vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
		end
	end

	else if @va_Type_in = XDC_ACT_EXPTFM
	begin
		/*B Ajout de l'intitule de l'action de type TFM */
		select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + XZAEC_SYN_ACT_EXPTFM
		insert #SYNTHESE (ligne, heure, categorie)
		values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)

		select @vl_Fin = heure_fin 
		from HIS..ACT_GEN
		where numero = @va_NumAct_in and sit = @va_CleAct_in
	end

	else if @va_Type_in = XDC_ACT_EXP
	begin
		/*B Ajout de l'intitule de l'action de type TFM */
		select @vl_Ligne = rtrim(@vl_Ligne) + ' '  + XZAEC_SYN_ACT_EXP
		insert #SYNTHESE (ligne, heure, categorie)
		values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)

		if (@va_TypeHeure_in = XZAEC_ACT_SUC)
		begin
			/*B recherche gravite et trafic */
			select
				@vl_Gravite = gravite,
				@vl_Trafic = trafic
			from HIS..ACT_EXP_ADA
			where actio = @va_NumAct_in and sit = @va_CleAct_in
		
			if @vl_Gravite=0
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_GRAVITE + XZAEC_SYN_GRAVITE0
				insert #SYNTHESE (ligne, heure, categorie)
				values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
			end
			else if @vl_Gravite=1
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_GRAVITE + XZAEC_SYN_GRAVITE1
				insert #SYNTHESE (ligne, heure, categorie)
				values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
			end
			else if @vl_Gravite=2
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_GRAVITE + XZAEC_SYN_GRAVITE2
				insert #SYNTHESE (ligne, heure, categorie)
				values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
			end
			else if @vl_Gravite=3
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_GRAVITE + XZAEC_SYN_GRAVITE3
				insert #SYNTHESE (ligne, heure, categorie)
				values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
			end
		
			if @vl_Trafic=XDC_RADT_FLUIDE
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_TRAFIC + XZAEC_SYN_TRAFIC_FLUIDE
				insert #SYNTHESE (ligne, heure, categorie)
				values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
			end
			else if @vl_Trafic=XDC_RADT_PRESAT
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_TRAFIC + XZAEC_SYN_TRAFIC_PRESAT
				insert #SYNTHESE (ligne, heure, categorie)
				values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
			end
		 	if @vl_Trafic=XDC_RADT_SATURE
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_TRAFIC + XZAEC_SYN_TRAFIC_SATURE
				insert #SYNTHESE (ligne, heure, categorie)
				values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
			end
		end
	end

	else if @va_Type_in = XDC_ACT_ECHANGEUR
	begin
		/*B Ajout de l'intitule de l'action de type echangeur */
		select @vl_Ligne = rtrim(@vl_Ligne) + ' '  + XZAEC_SYN_ACT_ECH

		/* ajouter le nom de l'equipement a l'intitule */
		if @va_Eqt_in != null
		begin
			select @vl_NomEqt = nom
			from CFG..EQT_GEN
			where numero = @va_Eqt_in and type = @va_Type_in

			if @@rowcount != 1
				return XDC_NOK

			select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + @vl_NomEqt
		end

		insert #SYNTHESE (ligne, heure, categorie)
		values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)

		/*B recherche de la sequence de l'echangeur */
		select
			@vl_Sequence = sequence
		from HIS..ACT_ECH
		where actio = @va_NumAct_in and sit = @va_CleAct_in

		/*B ajout de la sequence de l'echangeur dans la synthese */
		if @vl_Sequence != null 
		begin
			select @vl_NomSequence = nom
			from CFG..EQT_SEQ_ECH
			where numero = @vl_Sequence

			if @@rowcount = 1
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_SEQ + @vl_NomSequence

				insert #SYNTHESE (ligne, heure, categorie)
				values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
			end
		end


		/*B resultat de l'action */
		if (@va_TypeHeure_in = XZAEC_ACT_ECHEC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_ECHEC
			insert #SYNTHESE (ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
		end
		if (@va_TypeHeure_in = XZAEC_ACT_SUC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_SUC
			insert #SYNTHESE (ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
		end
	end
	else return XDC_NOK

	return XDC_OK

#else
	/*! CREATION DE LA PROCEDURE DANS UN DISTRICT */

create procedure XZAE82
	@va_NumAct_in		int		= null,
	@va_CleAct_in		tinyint		= null,
	@va_SiteOrigine_in	tinyint		= null,
	@va_Type_in		tinyint		= null,
	@va_Heure_in		datetime	= null,
	@va_TypeHeure_in	tinyint		= null,
	@va_Eqt_in		T_EQUIPEMENT	= null,
	@va_Autoroute_in	T_AUTOROUTE	= null
as
	declare @vl_Ligne char(100), @vl_LignePicto char(80), @vl_NomEqt char(10), 
		@vl_DirectLigne char(18), @vl_Bandeau char(10),
		@vl_TypeEqt tinyint, @vl_Interlocuteur smallint,
		@vl_TypeAstreinte tinyint, @vl_NomTypeAstreinte T_NOM,
		@vl_Diffusion smallint, @vl_Texte char(20), 
		@vl_Affichette varchar(250), @vl_PR T_PR, @vl_Sens T_SENS, 
		@vl_Gare T_NOM, @vl_Debut datetime, @vl_Fin datetime, 
		@vl_DebutBalisage datetime, @vl_FinBalisage datetime, 
		@vl_Balisage bit, @vl_NomInterlocuteur T_NOM, 
		@vl_Intervention varchar(25), @vl_Type tinyint, 
		@vl_Scenario char(10), @vl_NomScenario T_NOM, 
		@vl_Sequence tinyint, @vl_Depanneur T_NOM, 
		@vl_Longueur int,  @vl_Resultat char(25), @vl_Pictogramme char(10),
		@vl_NomSequence T_NOM, @vl_Date char(10), @vl_Heure char(8),
		@vl_Action char(12), @vl_ChaineFixe char(20), 
		@vl_ChaineAction varchar(20), @vl_CaractDebut int,
		@vl_Numero varchar(7), @vl_PRDebBalisage T_PR,
		@vl_PRFinBalisage T_PR, @vl_localisation char(60),
		@vl_LibMod1 char(30), @vl_LibMod2 char(30), @vl_LibMod3 char(30),
		@vl_LibMod4 char(30), @vl_LibMod5 char(30),
		@vl_Annulation tinyint,
		@vl_texte1 char(255),@vl_Ligne1 char(18),@vl_Ligne2 char(18),@vl_Ligne3 char(18),@vl_Ligne4 char(18),
		@vl_Gravite tinyint,@vl_Trafic tinyint,@vl_index tinyint, @vl_autoroute tinyint,
		@vl_Scenario_PAL char(6), @vl_NomScenario_PAL char(50),
		@vl_Mod1 tinyint, @vl_Mod2 tinyint, @vl_Mod3 tinyint, @vl_Mod4 tinyint,@vl_Mod6 tinyint, @vl_Mod5 tinyint,
		@vl_numero_telephonique char(100),
		@vl_type_saga tinyint,@vl_sms tinyint,
		@vl_barriere smallint, @vl_scenario smallint,
		@vl_l1 char(18), @vl_l2 char(18), @vl_l3 char(18), @vl_l4 char(18), @vl_l5 char(18)


	/*A Controle des parametres d'entree obligatoire */
	if @va_NumAct_in = null or @va_CleAct_in = null or 
	   @va_Type_in = null or @va_Heure_in = null or @va_TypeHeure_in = null
		return XDC_ARG_INV

	/* conversion du numero d'action en chaine de caracteres */
	select @vl_ChaineAction = convert(char(20), @va_NumAct_in)

	/* calcul la position du premier caractere a prendre */
	select @vl_Longueur = char_length(@vl_ChaineAction), 
		@vl_ChaineFixe = @vl_ChaineAction
	/****************** DEM SAE-441
	if @vl_Longueur < XZAEC_NB_DIGITS_LIB_ACT
	begin
	********************************/

		select @vl_CaractDebut = 1
		select @vl_Numero = substring(@vl_ChaineFixe, @vl_CaractDebut, @vl_Longueur)
		
	/****************** DEM SAE-441
	end
	else    
	begin
		select @vl_CaractDebut = @vl_Longueur - (XZAEC_NB_DIGITS_LIB_ACT - 1)
		select @vl_Numero = substring(@vl_ChaineFixe, @vl_CaractDebut, XZAEC_NB_DIGITS_LIB_ACT)
	end
	*******************************/
	/*A formatage de la date et du numero d'action */
	select	@vl_Date = convert(char(10), @va_Heure_in, 103),
		@vl_Heure = convert(char(8), @va_Heure_in, 8)
	
	if @va_CleAct_in != @va_SiteOrigine_in
	begin
		select	@vl_Action = @vl_Numero + '-' + convert(char(2), @va_CleAct_in) + ' ' + convert(char(2), @va_SiteOrigine_in)
	end
	else
	begin
		select	@vl_Action = @vl_Numero + '-' + convert(char(2), @va_CleAct_in) + '   ' 
	end

	/*A Creation l'intitule de l'action  dans la synthese */
	if @va_TypeHeure_in = XZAEC_ACT_SUC
		select @vl_Ligne = @vl_Date + ' ' + @vl_Heure +
				' ' + @vl_Action + XZAEC_SYN_ACT_DEB 
	else
	if @va_TypeHeure_in = XZAEC_ACT_ECHEC and ( @va_Type_in = XDC_ACT_EXP or @va_Type_in = XDC_ACT_EXPTFM)
		select @vl_Ligne = @vl_Date + ' ' + @vl_Heure +
				' ' + @vl_Action + XZAEC_SYN_ACT_ECHEC
	else	select @vl_Ligne = @vl_Date + ' ' + @vl_Heure +
				' ' + @vl_Action + XZAEC_SYN_ACT_FIN 
				
	/*A recherche les informations complementaires relatives */
	/*A a une action                                         */
	if @va_Type_in = XDC_ACT_APPEL or @va_Type_in = XDC_ACT_FAX or
	   @va_Type_in = XDC_ACT_BIP or @va_Type_in = XDC_APPEL_RADIO
	begin
		/*B recherche de l'interlocuteur pour un appel */
		select
			@vl_Interlocuteur = interlocuteur,
			@vl_TypeAstreinte = type_d_astreinte,
			@vl_numero_telephonique = numero_telephonique,
			@vl_sms = sms
		from EXP..ACT_APL
		where actio = @va_NumAct_in and sit = @va_CleAct_in

		if @@rowcount != 1
			return XDC_NOK

		if @vl_TypeAstreinte is not null
			select @vl_NomTypeAstreinte = nom
			from CFG..TYP_AST
			where numero = @vl_TypeAstreinte

		if @vl_TypeAstreinte is null or @vl_NomTypeAstreinte is null
		begin
			if @va_Type_in = XDC_ACT_FAX and @vl_sms =1
				select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + XZAEC_SYN_ACT_SMS
			else if @va_Type_in = XDC_ACT_FAX
				select @vl_Ligne = rtrim(@vl_Ligne)+ ' ' + XZAEC_SYN_ACT_FAX
			else if @va_Type_in = XDC_ACT_TEL
				select @vl_Ligne = rtrim(@vl_Ligne)+ ' ' + XZAEC_SYN_ACT_TEL
			else if @va_Type_in = XDC_ACT_BIP
				select @vl_Ligne = rtrim(@vl_Ligne)+ ' ' + XZAEC_SYN_ACT_BIP
			else if @va_Type_in = XDC_APPEL_RADIO
				select @vl_Ligne = rtrim(@vl_Ligne)+ ' ' + XZAEC_SYN_ACT_RADIO
			else return XDC_NOK
		end
		else
		begin
			if @va_Type_in = XDC_ACT_FAX and @vl_sms =1
				select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + XZAEC_SYN_ACT_SMS_AST + @vl_NomTypeAstreinte
			else if @va_Type_in = XDC_ACT_FAX
				select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + XZAEC_SYN_ACT_FAX_AST + @vl_NomTypeAstreinte
			else if @va_Type_in = XDC_ACT_TEL
				if @vl_TypeAstreinte = XDC_AST_CLIENT
					select @vl_Ligne = rtrim(@vl_Ligne) + ' '  + XZAEC_SYN_ACT_TEL_AST + XDC_LIB_AST_CLIENT + ' ' + substring(@vl_numero_telephonique,2,4)
				else
					select @vl_Ligne = rtrim(@vl_Ligne) + ' '  + XZAEC_SYN_ACT_TEL_AST + @vl_NomTypeAstreinte
			else if @va_Type_in = XDC_ACT_BIP
				select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + XZAEC_SYN_ACT_BIP_AST + @vl_NomTypeAstreinte
			else if @va_Type_in = XDC_APPEL_RADIO
				select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + XZAEC_SYN_ACT_RADIO + @vl_NomTypeAstreinte
			else return XDC_NOK
		end

		/*B Ajout de l'intitule de l'action de type appel */
		insert #SYNTHESE (ligne, heure, categorie)
		values(@vl_Ligne, null, XDC_CAT_ACT_APPEL)

		if @vl_Interlocuteur != null 
		begin
			select @vl_NomInterlocuteur = nom
			from CFG..ANN_FIC
			where numero = @vl_Interlocuteur

			if @@rowcount != 1
				select @vl_Ligne = XZAEC_SYN_PAS_DATE
			else	select @vl_Ligne = XZAEC_SYN_PAS_DATE + @vl_NomInterlocuteur
		end
		else	select @vl_Ligne = XZAEC_SYN_PAS_DATE 

		/*B Ajout de l'interlocuteur et du resultat de l'action */
		if (@va_TypeHeure_in = XZAEC_ACT_ECHEC)
		begin
			select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + XZAEC_SYN_ACT_ECHOUEE
			insert #SYNTHESE (ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_APPEL)
		end
		else if (@va_TypeHeure_in = XZAEC_ACT_SUC)
		begin
			select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + XZAEC_SYN_ACT_REUSSIE
			insert #SYNTHESE (ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_APPEL)
		end

	end
	else if @va_Type_in = XDC_ACT_CCH
	begin
		select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + XZAEC_SYN_ACT_CCH

		/*B Ajout de l'intitule de l'action de type CCH */
		insert #SYNTHESE (ligne, heure, categorie)
		values(@vl_Ligne, null, XDC_CAT_ACT_APPEL)

		/*B resultat de l'action */
		if (@va_TypeHeure_in = XZAEC_ACT_ECHEC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_ECHEC
			insert #SYNTHESE (ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_APPEL)
		end
		if (@va_TypeHeure_in = XZAEC_ACT_SUC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_SUC
			insert #SYNTHESE (ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_APPEL)
		end

	end

	else if @va_Type_in = XDC_ACT_NAV or @va_Type_in = XDC_ACT_FAC or @va_Type_in = XDC_ACT_FAU
	begin
		if @va_Type_in = XDC_ACT_NAV
			select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + XZAEC_SYN_ACT_NAV
		else if @va_Type_in = XDC_ACT_FAC
			select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + XZAEC_SYN_ACT_FAC
		else
			select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + XZAEC_SYN_ACT_FAU

		/* ajouter le nom de l'equipement a l'intitule */
		if @va_Eqt_in != null
		begin
			select @vl_NomEqt = nom
			from CFG..EQT_GEN
			where numero = @va_Eqt_in and type = @va_Type_in

			if @@rowcount != 1
				return XDC_NOK

			select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + @vl_NomEqt
		end

		/*B Ajout de l'intitule de l'action de type NAV */
		insert #SYNTHESE (ligne, heure, categorie)
		values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)

		/*B recherche de le scenario de la NAV */
		select
			@vl_Scenario = scenario
		from EXP..ACT_NAV
		where actio = @va_NumAct_in and sit = @va_CleAct_in

		if @@rowcount != 1
			return XDC_NOK

		if @vl_Scenario != XDC_CHAINE_VIDE 
		begin
			select @vl_NomScenario = nom
			from CFG..EQT_SCN_NAV
			where scenario = @vl_Scenario

			if @@rowcount != 1
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_ACT_SCN
			else	select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_SCN + @vl_NomScenario
		end

		/*B ajout du scenario de la NAV  dans la synthese */
		insert #SYNTHESE (ligne, heure, categorie)
		values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)

		/*B resultat de l'action */
		if (@va_TypeHeure_in = XZAEC_ACT_ECHEC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_ECHEC
			insert #SYNTHESE (ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
		end
		if (@va_TypeHeure_in = XZAEC_ACT_SUC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_SUC
			insert #SYNTHESE (ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
		end

	end

        else if @va_Type_in =  XDC_ACT_BAF
        begin
                select @vl_Ligne = @vl_Ligne + XZAEC_SYN_ACT_BAF

                if @va_Eqt_in != null
                begin
                        select @vl_NomEqt = nom
                        from CFG..EQT_GEN
                        where numero = @va_Eqt_in and type = @va_Type_in

                        if @@rowcount != 1
                                return XDC_NOK

                        select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + @vl_NomEqt
                end

                insert #SYNTHESE (ligne, heure, categorie)
                values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)

                select
                        @vl_Mod1 = module11,
                        @vl_Mod2 = module12,
                        @vl_Mod3 = module15,
                        @vl_Mod4 = module21,
                        @vl_Mod5 = module22,
			@vl_Mod6 =module25
                from EXP..ACT_BAF
                where actio = @va_NumAct_in and sit = @va_CleAct_in

                if @@rowcount != 1
                        return XDC_NOK

		if (@vl_Mod1!=0) and (@vl_Mod2!=0) and (@vl_Mod3!=0) 
		begin
			insert #SYNTHESE (ligne, heure, categorie)
			values(XZAEC_SYN_PAS_DATE +XZAEC_SYN_ACT_BAF_GLOBAL_DROIT,null, XDC_CAT_ACT_COMMANDE)
		end
		else if (@vl_Mod1!=0) and (@vl_Mod2!=0)
		begin
			insert #SYNTHESE (ligne, heure, categorie)
			values(XZAEC_SYN_PAS_DATE + XZAEC_SYN_ACT_BAF_FEUX_DROITS,null, XDC_CAT_ACT_COMMANDE)
		end
		else 
		begin
                	if (@vl_Mod1!=0)
                	begin
                        	insert #SYNTHESE (ligne, heure, categorie)
                        	values(XZAEC_SYN_PAS_DATE + XZAEC_SYN_ACT_BAF_MOD1 , null, XDC_CAT_ACT_COMMANDE)
                	end

                	if (@vl_Mod2!=0)
                	begin
                       		insert #SYNTHESE (ligne, heure, categorie)
                        	values(XZAEC_SYN_PAS_DATE + XZAEC_SYN_ACT_BAF_MOD2 , null, XDC_CAT_ACT_COMMANDE)
                	end

                	if (@vl_Mod3!=0)
                	begin
                        	insert #SYNTHESE (ligne, heure, categorie)
                        	values(XZAEC_SYN_PAS_DATE + XZAEC_SYN_ACT_BAF_MOD3 , null, XDC_CAT_ACT_COMMANDE)
                	end
		end


		if (@vl_Mod4!=0) and (@vl_Mod5!=0) and (@vl_Mod6!=0) 
		begin
			insert #SYNTHESE (ligne, heure, categorie)
			values(XZAEC_SYN_PAS_DATE +XZAEC_SYN_ACT_BAF_GLOBAL_GAUCHE,null, XDC_CAT_ACT_COMMANDE)
		end
		else if (@vl_Mod4!=0) and (@vl_Mod5!=0)
		begin
			insert #SYNTHESE (ligne, heure, categorie)
			values(XZAEC_SYN_PAS_DATE + XZAEC_SYN_ACT_BAF_FEUX_GAUCHES,null, XDC_CAT_ACT_COMMANDE)
		end
		else 
		begin
                	if (@vl_Mod4!=0)
			begin
                        	insert #SYNTHESE (ligne, heure, categorie)
                        	values(XZAEC_SYN_PAS_DATE + XZAEC_SYN_ACT_BAF_MOD4 , null, XDC_CAT_ACT_COMMANDE)
                	end

                	if (@vl_Mod5!=0)
                	begin
                        	insert #SYNTHESE (ligne, heure, categorie)
                        	values(XZAEC_SYN_PAS_DATE + XZAEC_SYN_ACT_BAF_MOD5 , null, XDC_CAT_ACT_COMMANDE)
                	end

                	if (@vl_Mod6!=0)
                	begin
                        	insert #SYNTHESE (ligne, heure, categorie)
                        	values(XZAEC_SYN_PAS_DATE + XZAEC_SYN_ACT_BAF_MOD6 , null, XDC_CAT_ACT_COMMANDE)
                	end
		end

                if (@va_TypeHeure_in = XZAEC_ACT_ECHEC)
                begin
                        select @vl_Ligne = XZAEC_SYN_PAS_DATE +
                                           XZAEC_SYN_ACT_RESULT_ECHEC
                        insert #SYNTHESE (ligne, heure, categorie)
                        values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
                end
                if (@va_TypeHeure_in = XZAEC_ACT_SUC)
                begin
                        select @vl_Ligne = XZAEC_SYN_PAS_DATE +
                                           XZAEC_SYN_ACT_RESULT_SUC
                        insert #SYNTHESE (ligne, heure, categorie)
                        values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
                end

        end

        else if @va_Type_in = XDC_ACT_BAD
        begin
                        select @vl_Ligne = @vl_Ligne + XZAEC_SYN_ACT_BAD

                if @va_Eqt_in != null
                begin
                        select @vl_NomEqt = nom
                        from CFG..EQT_GEN
                        where numero = @va_Eqt_in and type = @va_Type_in

                        if @@rowcount != 1
                                return XDC_NOK

                        select @vl_Ligne = @vl_Ligne + ' ' + @vl_NomEqt
                end
                insert #SYNTHESE (ligne, heure, categorie)
                values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)

                select
                        @vl_Scenario = scenario
                from EXP..ACT_BAD
                where actio = @va_NumAct_in and sit = @va_CleAct_in

                if @@rowcount != 1
                        return XDC_NOK

                if @vl_Scenario != XDC_CHAINE_VIDE
                begin
/*                        select @vl_NomScenario = nom
                        from CFG..EQT_SCN_NAV
                        where scenario = @vl_Scenario*/

			select @vl_NomScenario = @vl_Scenario

                        if @@rowcount != 1
                                select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_ACT_SCN + @vl_Scenario
                        else    select @vl_Ligne = XZAEC_SYN_PAS_DATE +
                                           XZAEC_SYN_ACT_SCN + @vl_NomScenario
                end

                insert #SYNTHESE (ligne, heure, categorie)
                values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)

                if (@va_TypeHeure_in = XZAEC_ACT_ECHEC)
                begin
                        select @vl_Ligne = XZAEC_SYN_PAS_DATE +
                                           XZAEC_SYN_ACT_RESULT_ECHEC
                        insert #SYNTHESE (ligne, heure, categorie)
                        values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
                end
                if (@va_TypeHeure_in = XZAEC_ACT_SUC)
                begin
                        select @vl_Ligne = XZAEC_SYN_PAS_DATE +
                                           XZAEC_SYN_ACT_RESULT_SUC
                        insert #SYNTHESE (ligne, heure, categorie)
                        values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
                end

        end
	else if @va_Type_in =  XDC_ACT_PAL
	begin
		select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + XZAEC_SYN_ACT_PAL

		/* ajouter le nom de l'equipement a l'intitule */
		if @va_Eqt_in != null
		begin
			select @vl_NomEqt = nom
			from CFG..EQT_GEN
			where numero = @va_Eqt_in and type = @va_Type_in

			if @@rowcount != 1
				return XDC_NOK

			select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + @vl_NomEqt
		end

		/*B Ajout de l'intitule de l'action de type PAL */
		insert #SYNTHESE (ligne, heure, categorie)
		values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)

		/*B recherche de le libelle des modules du panneau d'alerte */
		select
			@vl_Scenario_PAL = scenario,
			@vl_NomScenario_PAL = nom_scenario
		from EXP..ACT_PAL
		where actio = @va_NumAct_in and sit = @va_CleAct_in

		if @@rowcount != 1
			return XDC_NOK

		select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_SCN + @vl_NomScenario_PAL
					   + ' ' + @vl_Scenario_PAL

		insert #SYNTHESE (ligne, heure, categorie)
		values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)

		/*B resultat de l'action */
		if (@va_TypeHeure_in = XZAEC_ACT_ECHEC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_ECHEC
			insert #SYNTHESE (ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
		end
		if (@va_TypeHeure_in = XZAEC_ACT_SUC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_SUC
			insert #SYNTHESE (ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
		end

	end

	else if @va_Type_in = XDC_ACT_SAGA
	begin
		select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + XZAEC_SYN_ACT_SAGA

		/*rechercher le sous type SAGA*/
		select @vl_type_saga= type_saga from CFG..EQT_SAGA where numero = @va_Eqt_in
		if @@rowcount != 1
			return XDC_NOK
		
		/*JG*/
		/* ajouter le nom de l'equipement a l'intitule */
		if @va_Eqt_in != null
		begin
			select @vl_NomEqt = nom
			from CFG..EQT_GEN
			where numero = @va_Eqt_in and type = @va_Type_in

			if @@rowcount != 1
				return XDC_NOK

			select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + @vl_NomEqt
		end

		/*B Ajout de l'intitule de l'action de type PMV */
		insert #SYNTHESE (ligne, heure, categorie)
		values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)

		/*recupere les infos de l action*/
		select @vl_l1 = premiere_ligne, @vl_l2 = deuxieme_ligne, @vl_l3 = troisieme_ligne,
			@vl_l4=quatrieme_ligne, @vl_l5 = cinquieme_ligne,
			@vl_Pictogramme = picto,
			@vl_Bandeau = bandeau,
			@vl_barriere = barriere, @vl_scenario = scenario
		from EXP..ACT_SAGA
		where actio = @va_NumAct_in and sit = @va_CleAct_in
		if @@rowcount != 1
			return XDC_NOK

		select @vl_LignePicto =  XZAEC_SYN_PAS_DATE +
				    XZAEC_SYN_ACT_PMV_PCT

		if @vl_type_saga = XDC_SAGA_BAF or @vl_type_saga = XDC_SAGA_BRA
		begin
			insert #SYNTHESE (ligne, heure, categorie)
			values(XZAEC_SYN_PAS_DATE + XZAEC_SYN_ACT_BAD_MOD2, null, XDC_CAT_ACT_COMMANDE)
		end
		else if @vl_type_saga = XDC_SAGA_META_EQT
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_SCEN + ' ' + convert (char(10),@vl_scenario)
			insert #SYNTHESE (ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
		end
		else if @vl_type_saga = XDC_SAGA_PMVA
		begin
			select
			 @vl_Ligne = XZAEC_SYN_PAS_DATE +
				XZAEC_SYN_ACT_PMVA_TXT +
				@vl_l1 + ' | ' + @vl_l2 + ' | ' + @vl_l3 + ' | ' + @vl_l4 + ' | ' + @vl_l5
			insert #SYNTHESE (ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
		end
		else if @vl_type_saga = XDC_SAGA_PMV_1_18
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_ACT_PMV_TXT + @vl_l1
			insert #SYNTHESE (ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
		end
		else if @vl_type_saga = XDC_SAGA_PMV_2_18 or @vl_type_saga = XDC_SAGA_PMV_2_15
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_ACT_PMV_TXT + @vl_l1 + ' | ' + @vl_l2
			insert #SYNTHESE (ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
		end
		else if @vl_type_saga = XDC_SAGA_PMV_3_15
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_ACT_PMV_TXT + @vl_l1 + ' | ' + @vl_l2 + ' | ' + @vl_l3 
			insert #SYNTHESE (ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
		end
		else if @vl_type_saga = XDC_SAGA_PMV_2_18_P
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_ACT_PMV_TXT + @vl_l1 + ' | ' + @vl_l2
			insert #SYNTHESE (ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
			insert #SYNTHESE(ligne, heure, picto, categorie)
			values(@vl_LignePicto, null, @vl_Pictogramme, XDC_CAT_ACT_COMMANDE)
		end
		else if @vl_type_saga = XDC_SAGA_PMV_3_15_P
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_ACT_PMV_TXT + @vl_l1 + ' | ' + @vl_l2 + ' | ' + @vl_l3
			insert #SYNTHESE (ligne, heure, categorie)
			values(isnull(@vl_Ligne,""), null, XDC_CAT_ACT_COMMANDE)
			insert #SYNTHESE(ligne, heure, picto, categorie)
			values(isnull(@vl_LignePicto,""), null, @vl_Pictogramme, XDC_CAT_ACT_COMMANDE)
		end

		/*B resultat de l'action */
		if (@va_TypeHeure_in = XZAEC_ACT_ECHEC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_ECHEC
			insert #SYNTHESE (ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
		end
		if (@va_TypeHeure_in = XZAEC_ACT_SUC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_SUC
			insert #SYNTHESE (ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
		end
	end

	else if @va_Type_in =  XDC_ACT_PMV
	begin
		select @vl_Ligne = rtrim(@vl_Ligne) + ' ' +XZAEC_SYN_ACT_PMV
		
		/* ajouter le nom de l'equipement a l'intitule */
		if @va_Eqt_in != null
		begin
			select @vl_NomEqt = nom
			from CFG..EQT_GEN
			where numero = @va_Eqt_in and type = @va_Type_in

			if @@rowcount != 1
				return XDC_NOK

			select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + @vl_NomEqt
		end

		/*B Ajout de l'intitule de l'action de type PMV */
		insert #SYNTHESE (ligne, heure, categorie)
		values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)

		/*B recherche de le texte du PMV */
		/*B et du texte du PMV 4eme ligne*/
		select
			@vl_Ligne = XZAEC_SYN_PAS_DATE +
				    XZAEC_SYN_ACT_PMV_TXT +
				    premiere_ligne + ' | ' + deuxieme_ligne + 
				    ' | ' + troisieme_ligne, 
			@vl_LignePicto =  XZAEC_SYN_PAS_DATE +
				    XZAEC_SYN_ACT_PMV_PCT, 
			@vl_Pictogramme = pictogramme,
			@vl_Bandeau = bandeau,
			@vl_DirectLigne = direction_ligne
		from EXP..ACT_PMV
		where actio = @va_NumAct_in and sit = @va_CleAct_in

		if @@rowcount != 1
			return XDC_NOK

		/*B ajout du texte du PMV  dans la synthese */
		insert #SYNTHESE (ligne, heure, categorie)
		values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
		insert #SYNTHESE(ligne, heure, picto, categorie)
		values(@vl_LignePicto, null, @vl_Pictogramme, XDC_CAT_ACT_COMMANDE)

		/*B ajout du texte du bandeau du PMV  dans la synthese si non null*/
		if (@vl_Bandeau!=null) and (@vl_Bandeau!=XDC_CHAINE_VIDE)
		begin   
			insert #SYNTHESE (ligne, heure, categorie)
			values(XZAEC_SYN_PAS_DATE + XZAEC_SYN_ACT_PMV_BAND + @vl_Bandeau, null, XDC_CAT_ACT_COMMANDE)
		end

		/*B ajout du texte de 4eme ligne du PMV  dans la synthese si non null*/
		if (@vl_DirectLigne!=null) and (@vl_DirectLigne!=XDC_CHAINE_VIDE)
		begin   
			insert #SYNTHESE (ligne, heure, categorie)
			values(XZAEC_SYN_PAS_DATE + XZAEC_SYN_ACT_PMV_TXT_4 + @vl_DirectLigne, null, XDC_CAT_ACT_COMMANDE)
		end


		/*B resultat de l'action */
		if (@va_TypeHeure_in = XZAEC_ACT_ECHEC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_ECHEC
			insert #SYNTHESE (ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
		end
		if (@va_TypeHeure_in = XZAEC_ACT_SUC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_SUC
			insert #SYNTHESE (ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
		end

	end

        else if @va_Type_in =  XDC_ACT_PMVA
        begin
                select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + XZAEC_SYN_ACT_PMVA
                if @va_Eqt_in != null
                begin
                        select @vl_NomEqt = nom
                        from CFG..EQT_GEN
                        where numero = @va_Eqt_in and type = @va_Type_in
                        if @@rowcount != 1
                                return XDC_NOK
                        select @vl_Ligne = rtrim(@vl_Ligne)+ ' ' + @vl_NomEqt
                end

                insert #SYNTHESE (ligne, heure, categorie)
                values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)

                select
                        @vl_Ligne = XZAEC_SYN_PAS_DATE +
                                    XZAEC_SYN_ACT_PMVA_TXT +
                                    premiere_ligne + ' | ' + deuxieme_ligne +
                                    ' | ' + troisieme_ligne
                from EXP..ACT_PMA
                where actio = @va_NumAct_in and sit = @va_CleAct_in

                if @@rowcount != 1
                        return XDC_NOK

                insert #SYNTHESE (ligne, heure, categorie)
                values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)

                select
                        @vl_Ligne = XZAEC_SYN_PAS_DATE +
                                    XZAEC_SYN_ACT_PMVA_TXT +
                                    quatrieme_ligne + ' | ' + cinquieme_ligne
                from EXP..ACT_PMA
                where actio = @va_NumAct_in and sit = @va_CleAct_in

                if @@rowcount != 1
                        return XDC_NOK

                insert #SYNTHESE (ligne, heure, categorie)
                values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)

                if (@va_TypeHeure_in = XZAEC_ACT_ECHEC)
                begin
                        select @vl_Ligne = XZAEC_SYN_PAS_DATE +
                                           XZAEC_SYN_ACT_RESULT_ECHEC
                        insert #SYNTHESE (ligne, heure, categorie)
                        values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
                end
                if (@va_TypeHeure_in = XZAEC_ACT_SUC)
                begin
                        select @vl_Ligne = XZAEC_SYN_PAS_DATE +
                                           XZAEC_SYN_ACT_RESULT_SUC
                        insert #SYNTHESE (ligne, heure, categorie)
                        values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
                end
        end

	else if @va_Type_in =  XDC_ACT_CFE
        begin
                select @vl_Ligne = rtrim(@vl_Ligne) + ' ' +XZAEC_SYN_ACT_CFE
                if @va_Eqt_in != null
                begin
                        select @vl_NomEqt = nom
                        from CFG..EQT_GEN
                        where numero = @va_Eqt_in and type = @va_Type_in
                        if @@rowcount != 1
                                return XDC_NOK
                        select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + @vl_NomEqt
                end

                insert #SYNTHESE (ligne, heure, categorie)
                values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)

                select
                        @vl_Ligne = XZAEC_SYN_PAS_DATE +
                                    XZAEC_SYN_ACT_CFE_TXT +
                                    libelle_plan 
                from EXP..ACT_CFE
                where actio = @va_NumAct_in and sit = @va_CleAct_in

                if @@rowcount != 1
                        return XDC_NOK

                insert #SYNTHESE (ligne, heure, categorie)
                values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)

                if (@va_TypeHeure_in = XZAEC_ACT_ECHEC)
                begin
                        select @vl_Ligne = XZAEC_SYN_PAS_DATE +
                                           XZAEC_SYN_ACT_RESULT_ECHEC
                        insert #SYNTHESE (ligne, heure, categorie)
                        values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
                end
                if (@va_TypeHeure_in = XZAEC_ACT_SUC)
                begin
                        select @vl_Ligne = XZAEC_SYN_PAS_DATE +
                                           XZAEC_SYN_ACT_RESULT_SUC
                        insert #SYNTHESE (ligne, heure, categorie)
                        values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
                end
        end

	else if @va_Type_in =  XDC_ACT_IMU 
        begin 
                select @vl_Ligne = rtrim(@vl_Ligne) + ' ' +XZAEC_SYN_ACT_IMU 
                if @va_Eqt_in != null 
                begin 
                        select @vl_NomEqt = nom 
                        from CFG..EQT_GEN 
                        where numero = @va_Eqt_in and type = @va_Type_in 
                        if @@rowcount != 1 
                                return XDC_NOK 
                        select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + @vl_NomEqt 
                end 
 
                insert #SYNTHESE (ligne, heure, categorie) 
                values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE) 
 
                select 
                        @vl_Ligne = XZAEC_SYN_PAS_DATE + 
                                    XZAEC_SYN_ACT_IMU_TXT + 
                                    "chaine:" + chaine + " intervalle:" + convert(char(6),intervalle) + " Nb cycles:" + convert(char(6),cycles)								
                from EXP..ACT_IMU 
                where actio = @va_NumAct_in and sit = @va_CleAct_in 
 
                if @@rowcount != 1 
                        return XDC_NOK 
 
                insert #SYNTHESE (ligne, heure, categorie) 
                values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE) 								
 
                if (@va_TypeHeure_in = XZAEC_ACT_ECHEC) 
                begin 
                        select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
                                           XZAEC_SYN_ACT_RESULT_ECHEC 
                        insert #SYNTHESE (ligne, heure, categorie) 
                        values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE) 
                end 
                if (@va_TypeHeure_in = XZAEC_ACT_SUC) 
                begin 
                        select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
                                           XZAEC_SYN_ACT_RESULT_SUC 
                        insert #SYNTHESE (ligne, heure, categorie) 
                        values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE) 
                end 
        end

	else if @va_Type_in =  XDC_ACT_PAU 
        begin 
                select @vl_Ligne = rtrim(@vl_Ligne) + ' ' +XZAEC_SYN_ACT_PAU 
                if @va_Eqt_in != null 
                begin 
                        select @vl_NomEqt = nom 
                        from CFG..EQT_GEN 
                        where numero = @va_Eqt_in and type = XDC_EQT_PAU 
                        if @@rowcount != 1 
                                return XDC_NOK 
                        select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + @vl_NomEqt 
                end 
 
                insert #SYNTHESE (ligne, heure, categorie) 
                values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE) 
 
                select 
                        @vl_Ligne = XZAEC_SYN_PAS_DATE + 
                                XZAEC_SYN_ACT_PAU_TXT + 
                                message + " " + convert(char(2),autoroute) + " " + 
				convert(char(1),sens) + " " +
				convert(char(6),PR_debut) + " au " +
				convert(char(6),PR_fin) + " " +
				convert(char(2),PAUErreur) + "PAU en erreur " +
				convert(char(2), PAUPilotes)+ " PAU Pilotes"

                from EXP..ACT_PAU												 
                where actio = @va_NumAct_in and sit = @va_CleAct_in 
 
                if @@rowcount != 1 
                        return XDC_NOK 
 
                insert #SYNTHESE (ligne, heure, categorie) 
                values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE) 								
 
                if (@va_TypeHeure_in = XZAEC_ACT_ECHEC) 
                begin 
                        select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
                                           XZAEC_SYN_ACT_RESULT_ECHEC 
                        insert #SYNTHESE (ligne, heure, categorie) 
                        values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE) 
                end 
                if (@va_TypeHeure_in = XZAEC_ACT_SUC) 
                begin 
                        select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
                                           XZAEC_SYN_ACT_RESULT_SUC 
                        insert #SYNTHESE (ligne, heure, categorie) 
                        values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE) 
                end 
        end
		else if @va_Type_in =  XDC_ACT_PRV
        
		begin
                select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + XZAEC_SYN_ACT_PRV
                if @va_Eqt_in != null
                begin
                        select @vl_NomEqt = nom
                        from CFG..EQT_GEN
                        where numero = @va_Eqt_in and type = @va_Type_in
                        if @@rowcount != 1
                                return XDC_NOK
                        select @vl_Ligne = rtrim(@vl_Ligne)+ ' ' + @vl_NomEqt
                end

                insert #SYNTHESE (ligne, heure, categorie)
                values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)

                select
                        @vl_Ligne = XZAEC_SYN_PAS_DATE +
                                    XZAEC_SYN_ACT_PRV_TXT +
                                    vitesse + ' | ' + bandeau
                from EXP..ACT_PRV
                where actio = @va_NumAct_in and sit = @va_CleAct_in

                if @@rowcount != 1
                        return XDC_NOK

                insert #SYNTHESE (ligne, heure, categorie)
                values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)

                if (@va_TypeHeure_in = XZAEC_ACT_ECHEC)
                begin
                        select @vl_Ligne = XZAEC_SYN_PAS_DATE +
                                           XZAEC_SYN_ACT_RESULT_ECHEC
                        insert #SYNTHESE (ligne, heure, categorie)
                        values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
                end
                if (@va_TypeHeure_in = XZAEC_ACT_SUC)
                begin
                        select @vl_Ligne = XZAEC_SYN_PAS_DATE +
                                           XZAEC_SYN_ACT_RESULT_SUC
                        insert #SYNTHESE (ligne, heure, categorie)
                        values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
                end
        end

	else if @va_Type_in =  XDC_ACT_PIC
	begin
		select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + XZAEC_SYN_ACT_PIC
		if @va_Eqt_in != null
		begin
			select @vl_NomEqt = nom
				from CFG..EQT_GEN
					where numero = @va_Eqt_in and type = @va_Type_in
			if @@rowcount != 1
			        return XDC_NOK
			select @vl_Ligne = rtrim(@vl_Ligne)+ ' ' + @vl_NomEqt
		end

		insert #SYNTHESE (ligne, heure, categorie)
			values (@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)

		select @vl_Ligne =	XZAEC_SYN_PAS_DATE +
							XZAEC_SYN_ACT_PIC_TXT +
							pictogramme + ' | ' + bandeau
			from EXP..ACT_PIC
				where actio = @va_NumAct_in and sit = @va_CleAct_in

		if @@rowcount != 1
			return XDC_NOK

		insert #SYNTHESE (ligne, heure, categorie)
			values (@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)

		if (@va_TypeHeure_in = XZAEC_ACT_ECHEC)
		begin
			select @vl_Ligne =	XZAEC_SYN_PAS_DATE +
								XZAEC_SYN_ACT_RESULT_ECHEC
			insert #SYNTHESE (ligne, heure, categorie)
				values (@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
		end
		if (@va_TypeHeure_in = XZAEC_ACT_SUC)
		begin
			select @vl_Ligne =	XZAEC_SYN_PAS_DATE +
								XZAEC_SYN_ACT_RESULT_SUC
			insert #SYNTHESE (ligne, heure, categorie)
				values (@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
		end
	end

	else if @va_Type_in =  XDC_ACT_PMVDAT
	begin
		select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + XZAEC_SYN_ACT_PMVDAT

		/* ajouter le nom de l'equipement a l'intitule */
		select @vl_NomEqt = PMV,
			@vl_texte1=texte1
		from EXP..ACT_PMV_ADA
		where actio = @va_NumAct_in and sit = @va_CleAct_in

		if @@rowcount != 1
			return XDC_NOK

		select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + @vl_NomEqt

		select @vl_index=patindex("%/%",@vl_texte1)
		select @vl_Pictogramme = substring(substring(@vl_texte1,1,@vl_index-1),1,4)
		select @vl_texte1 = substring(@vl_texte1,@vl_index+1,char_length(@vl_texte1))
		
		select @vl_index=patindex("%/%",@vl_texte1)
		select @vl_Bandeau = substring(substring(@vl_texte1,1,@vl_index-1),1,6)
		select @vl_texte1 = substring(@vl_texte1,@vl_index+1,char_length(@vl_texte1))
		
		select @vl_index=patindex("%/%",@vl_texte1)
		select @vl_Ligne1 = substring(substring(@vl_texte1,1,@vl_index-1),1,15)
		select @vl_texte1 = substring(@vl_texte1,@vl_index+1,char_length(@vl_texte1))
		
		select @vl_index=patindex("%/%",@vl_texte1)
		select @vl_Ligne2 = substring(substring(@vl_texte1,1,@vl_index-1),1,15)
		select @vl_texte1 = substring(@vl_texte1,@vl_index+1,char_length(@vl_texte1))
		
		select @vl_index=patindex("%/%",@vl_texte1)
		select @vl_Ligne3 = substring(substring(@vl_texte1,1,@vl_index-1),1,15)
		select @vl_Ligne4 = substring(substring(@vl_texte1,@vl_index+1,char_length(@vl_texte1)),1,15)
		
		
		/*B Ajout de l'intitule de l'action de type PMV */
		insert #SYNTHESE (ligne, heure, categorie)
		values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)

		/*B recherche de le texte du PMV */
		/*B et du texte du PMV 4eme ligne*/
		select
			@vl_Ligne = substring(XZAEC_SYN_PAS_DATE +
				    XZAEC_SYN_ACT_PMV_TXT + @vl_Ligne1 + "|" + @vl_Ligne2 + 
				    "|" + @vl_Ligne3 + "|" + @vl_Ligne4,1,100),
			@vl_LignePicto =  XZAEC_SYN_PAS_DATE +
				    XZAEC_SYN_ACT_PMV_PCT

		if @@rowcount != 1
			return XDC_NOK

		/*B ajout du texte du PMV  dans la synthese */
		insert #SYNTHESE (ligne, heure, categorie)
		values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
		insert #SYNTHESE(ligne, heure, picto, categorie)
		values(@vl_LignePicto, null, @vl_Pictogramme, XDC_CAT_ACT_COMMANDE)

		/*B ajout du texte du bandeau du PMV  dans la synthese si non null*/
		if (@vl_Bandeau!=null) and (@vl_Bandeau!=XDC_CHAINE_VIDE)
		begin   
			insert #SYNTHESE (ligne, heure, categorie)
			values(XZAEC_SYN_PAS_DATE + XZAEC_SYN_ACT_PMV_BAND + @vl_Bandeau, null, XDC_CAT_ACT_COMMANDE)
		end

		/*B ajout du texte de 4eme ligne du PMV  dans la synthese si non null*/
		if (@vl_DirectLigne!=null) and (@vl_DirectLigne!=XDC_CHAINE_VIDE)
		begin   
			insert #SYNTHESE (ligne, heure, categorie)
			values(XZAEC_SYN_PAS_DATE + XZAEC_SYN_ACT_PMV_TXT_4 + @vl_DirectLigne, null, XDC_CAT_ACT_COMMANDE)
		end


		/*B resultat de l'action */
		if (@va_TypeHeure_in = XZAEC_ACT_ECHEC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_ECHEC
			insert #SYNTHESE (ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
		end
		if (@va_TypeHeure_in = XZAEC_ACT_SUC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_SUC
			insert #SYNTHESE (ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
		end

	end

	else if @va_Type_in = XDC_ACT_TUNNEL
	begin
		/*B Ajout de l'intitule de l'action de type TUB */
		select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + XZAEC_SYN_ACT_TUB

		/* ajouter le nom de l'equipement a l'intitule */
		if @va_Eqt_in != null
		begin
			select @vl_NomEqt = nom
			from CFG..EQT_GEN
			where numero = @va_Eqt_in and type = @va_Type_in

			if @@rowcount != 1
				return XDC_NOK

			select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + @vl_NomEqt
		end

		insert #SYNTHESE (ligne, heure, categorie)
		values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)

		/*B recherche de la sequence du tunnel */
		select
			@vl_Sequence = sequence
		from EXP..ACT_TUB
		where actio = @va_NumAct_in and sit = @va_CleAct_in

		/*B ajout de la sequence du tunnel dans la synthese */
		if @vl_Sequence != null 
		begin
			select @vl_NomSequence = nom
			from CFG..EQT_SEQ_TUB
			where numero = @vl_Sequence

			if @@rowcount = 1
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_SEQ + @vl_NomSequence

				insert #SYNTHESE (ligne, heure, categorie)
				values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
			end
		end

		/*B resultat de l'action */
		if (@va_TypeHeure_in = XZAEC_ACT_ECHEC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_ECHEC
			insert #SYNTHESE (ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
		end
		if (@va_TypeHeure_in = XZAEC_ACT_SUC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_SUC
			insert #SYNTHESE (ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
		end

	end

    /* AAZ 27/08/2007 */

    else if @va_Type_in = XDC_ACT_GTC_SIG
    begin
        /*B Ajout de l'intitule de l'action de type SIGNALISATION */
        select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + XZAEC_SYN_ACT_GTC_SIG

        /* ajouter le nom de l'equipement a l'intitule */
        if @va_Eqt_in != null
        begin
            select @vl_NomEqt = nom
            from   CFG..EQT_GEN
            where  numero = @va_Eqt_in and
                   type   = XDC_EQT_TUBE

            if @@rowcount != 1
                return XDC_NOK

            select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + @vl_NomEqt
        end

        insert #SYNTHESE( ligne,
                          heure,
						  categorie)
                  values( @vl_Ligne,
                          null,
						  XDC_CAT_ACT_COMMANDE)

        /*B recherche de la sequence du tunnel */
        select @vl_Sequence = sequence
        from   EXP..ACT_GTC_SIG
        where  actio = @va_NumAct_in and
               sit   = @va_CleAct_in

        /*B ajout de la sequence du tunnel dans la synthese */
        if @vl_Sequence != null 
        begin
            select @vl_NomSequence = nom
            from   CFG..EQT_SEQ_GTC_SIG
            where numero = @vl_Sequence
			and type = XDC_EQT_TUBE

            if @@rowcount = 1
            begin
                select @vl_Ligne = XZAEC_SYN_PAS_DATE +
                                   XZAEC_SYN_ACT_SEQ + @vl_NomSequence

                insert #SYNTHESE( ligne,
                                  heure,
								  categorie)
                          values( @vl_Ligne,
                                  null,
						  		  XDC_CAT_ACT_COMMANDE)
            end
        end

        /*B resultat de l'action */
        if (@va_TypeHeure_in = XZAEC_ACT_ECHEC)
        begin
            select @vl_Ligne = XZAEC_SYN_PAS_DATE +
                               XZAEC_SYN_ACT_RESULT_ECHEC
            insert #SYNTHESE( ligne,
                              heure,
							  categorie)
                      values( @vl_Ligne,
                              null,
						  	  XDC_CAT_ACT_COMMANDE)
        end
        if (@va_TypeHeure_in = XZAEC_ACT_SUC)
        begin
            select @vl_Ligne = XZAEC_SYN_PAS_DATE +
                               XZAEC_SYN_ACT_RESULT_SUC
            insert #SYNTHESE( ligne,
                              heure,
							  categorie)
                      values( @vl_Ligne,
                              null,
						  	  XDC_CAT_ACT_COMMANDE)
        end

    end
	
    else if @va_Type_in = XDC_ACT_GTC_SIG_ECH
    begin
        /*B Ajout de l'intitule de l'action de type SIGNALISATION */
        select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + XZAEC_SYN_ACT_GTC_SIG_ECH

        /* ajouter le nom de l'equipement a l'intitule */
        if @va_Eqt_in != null
        begin
            select @vl_NomEqt = nom
            from   CFG..EQT_GEN
            where  numero = @va_Eqt_in and
                   type   = XDC_EQT_ECH

            if @@rowcount != 1
                return XDC_NOK

            select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + @vl_NomEqt
        end

        insert #SYNTHESE( ligne,
                          heure,
						  categorie)
                  values( @vl_Ligne,
                          null,
						  XDC_CAT_ACT_COMMANDE)

        /*B recherche de la sequence du tunnel */
        select @vl_Sequence = sequence
        from   EXP..ACT_GTC_SIG
        where  actio = @va_NumAct_in and
               sit   = @va_CleAct_in

        /*B ajout de la sequence du tunnel dans la synthese */
        if @vl_Sequence != null 
        begin
            select @vl_NomSequence = nom
            from   CFG..EQT_SEQ_GTC_SIG
            where numero = @vl_Sequence
			and type = XDC_EQT_ECH

            if @@rowcount = 1
            begin
                select @vl_Ligne = XZAEC_SYN_PAS_DATE +
                                   XZAEC_SYN_ACT_SEQ + @vl_NomSequence

                insert #SYNTHESE( ligne,
                                  heure,
								  categorie)
                          values( @vl_Ligne,
                                  null,
								  XDC_CAT_ACT_COMMANDE)
            end
        end

        /*B resultat de l'action */
        if (@va_TypeHeure_in = XZAEC_ACT_ECHEC)
        begin
            select @vl_Ligne = XZAEC_SYN_PAS_DATE +
                               XZAEC_SYN_ACT_RESULT_ECHEC
            insert #SYNTHESE( ligne,
                              heure,
							  categorie)
                      values( @vl_Ligne,
                              null,
							  XDC_CAT_ACT_COMMANDE)
        end
        if (@va_TypeHeure_in = XZAEC_ACT_SUC)
        begin
            select @vl_Ligne = XZAEC_SYN_PAS_DATE +
                               XZAEC_SYN_ACT_RESULT_SUC
            insert #SYNTHESE( ligne,
                              heure,
							  categorie)
                      values( @vl_Ligne,
                              null,
							  XDC_CAT_ACT_COMMANDE)
        end

    end
	
	else if @va_Type_in = XDC_ACT_GTC_EXP
    begin
        /*B Ajout de l'intitule de l'action de type SIGNALISATION */
        select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + XZAEC_SYN_ACT_GTC_EXP

        /* ajouter le nom de l'equipement a l'intitule */
        if @va_Eqt_in != null
        begin
            select @vl_NomEqt = nom
            from   CFG..EQT_GEN
            where  numero = @va_Eqt_in and
                   type   = XDC_EQT_TUBE

            if @@rowcount != 1
                return XDC_NOK

            select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + @vl_NomEqt
        end

        insert #SYNTHESE( ligne,
                          heure,
						  categorie)
                  values( @vl_Ligne,
                          null,
						  XDC_CAT_ACT_COMMANDE)

        /*B recherche de la sequence du tunnel */
        select @vl_Sequence = sequence
        from   EXP..ACT_GTC_EXP
        where  actio = @va_NumAct_in and
               sit   = @va_CleAct_in

        /*B ajout de la sequence du tunnel dans la synthese */
        if @vl_Sequence != null 
        begin
            select @vl_NomSequence = nom
            from   CFG..EQT_SEQ_GTC_EXP
            where numero = @vl_Sequence
			and type = XDC_EQT_TUBE

            if @@rowcount = 1
            begin
                select @vl_Ligne = XZAEC_SYN_PAS_DATE +
                                   XZAEC_SYN_ACT_SEQ + @vl_NomSequence

                insert #SYNTHESE( ligne,
                                  heure,
								  categorie)
                          values( @vl_Ligne,
                                  null,
								  XDC_CAT_ACT_COMMANDE)
            end
        end

        /*B resultat de l'action */
        if (@va_TypeHeure_in = XZAEC_ACT_ECHEC)
        begin
            select @vl_Ligne = XZAEC_SYN_PAS_DATE +
                               XZAEC_SYN_ACT_RESULT_ECHEC
            insert #SYNTHESE( ligne,
                              heure,
							  categorie)
                      values( @vl_Ligne,
                              null,
							  XDC_CAT_ACT_COMMANDE)
        end
        if (@va_TypeHeure_in = XZAEC_ACT_SUC)
        begin
            select @vl_Ligne = XZAEC_SYN_PAS_DATE +
                               XZAEC_SYN_ACT_RESULT_SUC
            insert #SYNTHESE( ligne,
                              heure,
							  categorie)
                      values( @vl_Ligne,
                              null,
							  XDC_CAT_ACT_COMMANDE)
        end

    end

    else if @va_Type_in = XDC_ACT_GTC_ECL
    begin
        /*B Ajout de l'intitule de l'action de type ECLAIRAGE */
        select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + XZAEC_SYN_ACT_GTC_ECL

        /* ajouter le nom de l'equipement a l'intitule */
        if @va_Eqt_in != null
        begin
            select @vl_NomEqt = nom
            from   CFG..EQT_GEN
            where  numero = @va_Eqt_in and
                   type   = XDC_EQT_TUBE

            if @@rowcount != 1
                return XDC_NOK

            select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + @vl_NomEqt
        end

        insert #SYNTHESE( ligne,
                          heure,
						  categorie)
                  values( @vl_Ligne,
                          null,
						  XDC_CAT_ACT_COMMANDE)

        /*B recherche de la sequence du tunnel */
        select @vl_Sequence = sequence
        from   EXP..ACT_GTC_ECL
        where  actio = @va_NumAct_in and
               sit   = @va_CleAct_in 

        /*B ajout de la sequence du tunnel dans la synthese */
        if @vl_Sequence != null 
        begin
            select @vl_NomSequence = nom
            from   CFG..EQT_SEQ_GTC_ECL
            where numero = @vl_Sequence
			and type = XDC_EQT_TUBE

            if @@rowcount = 1
            begin
                select @vl_Ligne = XZAEC_SYN_PAS_DATE +
                                   XZAEC_SYN_ACT_SEQ + @vl_NomSequence

                insert #SYNTHESE( ligne,
                                  heure,
								  categorie)
                          values( @vl_Ligne,
                                  null,
								  XDC_CAT_ACT_COMMANDE)
            end
        end

        /*B resultat de l'action */
        if (@va_TypeHeure_in = XZAEC_ACT_ECHEC)
        begin
            select @vl_Ligne = XZAEC_SYN_PAS_DATE +
                               XZAEC_SYN_ACT_RESULT_ECHEC
            insert #SYNTHESE( ligne,
                              heure,
							  categorie)
                      values( @vl_Ligne,
                              null,
							  XDC_CAT_ACT_COMMANDE)
        end
        if (@va_TypeHeure_in = XZAEC_ACT_SUC)
        begin
            select @vl_Ligne = XZAEC_SYN_PAS_DATE +
                               XZAEC_SYN_ACT_RESULT_SUC
            insert #SYNTHESE( ligne,
                              heure,
							  categorie)
                      values( @vl_Ligne,
                              null,
							  XDC_CAT_ACT_COMMANDE)
        end

    end

    else if @va_Type_in = XDC_ACT_GTC_VEN
    begin
        /*B Ajout de l'intitule de l'action de type VENTILATION */
        select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + XZAEC_SYN_ACT_GTC_VEN

        /* ajouter le nom de l'equipement a l'intitule */
        if @va_Eqt_in != null
        begin
            select @vl_NomEqt = nom
            from   CFG..EQT_GEN
            where  numero = @va_Eqt_in and
                   type   = XDC_EQT_TUBE

            if @@rowcount != 1
                return XDC_NOK

            select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + @vl_NomEqt
        end

        insert #SYNTHESE( ligne,
                          heure,
						  categorie)
                  values( @vl_Ligne,
                          null,
						  XDC_CAT_ACT_COMMANDE)

        /*B recherche de la sequence du tunnel */
        select @vl_Sequence = sequence
        from   EXP..ACT_GTC_VEN
        where  actio = @va_NumAct_in and
               sit   = @va_CleAct_in

        /*B ajout de la sequence du tunnel dans la synthese */
        if @vl_Sequence != null 
        begin
            select @vl_NomSequence = nom
            from   CFG..EQT_SEQ_GTC_VEN
            where numero = @vl_Sequence
			and type = XDC_EQT_TUBE

            if @@rowcount = 1
            begin
                select @vl_Ligne = XZAEC_SYN_PAS_DATE +
                                   XZAEC_SYN_ACT_SEQ + @vl_NomSequence

                insert #SYNTHESE( ligne,
                                  heure,
								  categorie)
                          values( @vl_Ligne,
                                  null,
								  XDC_CAT_ACT_COMMANDE)
            end
        end

        /*B resultat de l'action */
        if (@va_TypeHeure_in = XZAEC_ACT_ECHEC)
        begin
            select @vl_Ligne = XZAEC_SYN_PAS_DATE +
                               XZAEC_SYN_ACT_RESULT_ECHEC
            insert #SYNTHESE( ligne,
                              heure,
							  categorie)
                      values( @vl_Ligne,
                              null,
							  XDC_CAT_ACT_COMMANDE)
        end
        if (@va_TypeHeure_in = XZAEC_ACT_SUC)
        begin
            select @vl_Ligne = XZAEC_SYN_PAS_DATE +
                               XZAEC_SYN_ACT_RESULT_SUC
            insert #SYNTHESE( ligne,
                              heure,
							  categorie)
                      values( @vl_Ligne,
                              null,
							  XDC_CAT_ACT_COMMANDE)
        end

    end

    else if @va_Type_in = XDC_ACT_GTC_ENR
    begin
        /*B Ajout de l'intitule de l'action de type ENERGIE */
        select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + XZAEC_SYN_ACT_GTC_ENR

        /* ajouter le nom de l'equipement a l'intitule */
        if @va_Eqt_in != null
        begin
            select @vl_NomEqt = nom
            from   CFG..EQT_GEN
            where  numero = @va_Eqt_in and
                   type   = XDC_EQT_TUBE

            if @@rowcount != 1
                return XDC_NOK

            select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + @vl_NomEqt
        end

        insert #SYNTHESE( ligne,
                          heure,
						  categorie)
                  values( @vl_Ligne,
                          null,
						  XDC_CAT_ACT_COMMANDE)

        /*B recherche de la sequence du tunnel */
        select @vl_Sequence = sequence
        from   EXP..ACT_GTC_ENR
        where  actio = @va_NumAct_in and
               sit   = @va_CleAct_in

        /*B ajout de la sequence du tunnel dans la synthese */
        if @vl_Sequence != null 
        begin
            select @vl_NomSequence = nom
            from   CFG..EQT_SEQ_GTC_ENR
            where numero = @vl_Sequence
			and type = XDC_EQT_TUBE

            if @@rowcount = 1
            begin
                select @vl_Ligne = XZAEC_SYN_PAS_DATE +
                                   XZAEC_SYN_ACT_SEQ + @vl_NomSequence

                insert #SYNTHESE( ligne,
                                  heure,
								  categorie)
                          values( @vl_Ligne,
                                  null,
								  XDC_CAT_ACT_COMMANDE)
            end
        end

        /*B resultat de l'action */
        if (@va_TypeHeure_in = XZAEC_ACT_ECHEC)
        begin
            select @vl_Ligne = XZAEC_SYN_PAS_DATE +
                               XZAEC_SYN_ACT_RESULT_ECHEC
            insert #SYNTHESE( ligne,
                              heure,
							  categorie)
                      values( @vl_Ligne,
                              null,
							  XDC_CAT_ACT_COMMANDE)
        end
        if (@va_TypeHeure_in = XZAEC_ACT_SUC)
        begin
            select @vl_Ligne = XZAEC_SYN_PAS_DATE +
                               XZAEC_SYN_ACT_RESULT_SUC
            insert #SYNTHESE( ligne,
                              heure,
							  categorie)
                      values( @vl_Ligne,
                              null,
							  XDC_CAT_ACT_COMMANDE)
        end

    end

    else if @va_Type_in = XDC_ACT_GTC_INC
    begin
        /*B Ajout de l'intitule de l'action de type PROTECTION INCENDIE */
        select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + XZAEC_SYN_ACT_GTC_INC

        /* ajouter le nom de l'equipement a l'intitule */
        if @va_Eqt_in != null
        begin
            select @vl_NomEqt = nom
            from   CFG..EQT_GEN
            where  numero = @va_Eqt_in and
                   type   = XDC_EQT_TUBE

            if @@rowcount != 1
                return XDC_NOK

            select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + @vl_NomEqt
        end

        insert #SYNTHESE( ligne,
                          heure,
						  categorie)
                  values( @vl_Ligne,
                          null,
						  XDC_CAT_ACT_COMMANDE)

        /*B recherche de la sequence du tunnel */
        select @vl_Sequence = sequence
        from   EXP..ACT_GTC_INC
        where  actio = @va_NumAct_in and
               sit   = @va_CleAct_in

        /*B ajout de la sequence du tunnel dans la synthese */
        if @vl_Sequence != null 
        begin
            select @vl_NomSequence = nom
            from   CFG..EQT_SEQ_GTC_INC
            where numero = @vl_Sequence
			and type = XDC_EQT_TUBE

            if @@rowcount = 1
            begin
                select @vl_Ligne = XZAEC_SYN_PAS_DATE +
                                   XZAEC_SYN_ACT_SEQ + @vl_NomSequence

                insert #SYNTHESE( ligne,
                                  heure,
								  categorie)
                          values( @vl_Ligne,
                                  null,
								  XDC_CAT_ACT_COMMANDE)
            end
        end

        /*B resultat de l'action */
        if (@va_TypeHeure_in = XZAEC_ACT_ECHEC)
        begin
            select @vl_Ligne = XZAEC_SYN_PAS_DATE +
                               XZAEC_SYN_ACT_RESULT_ECHEC
            insert #SYNTHESE( ligne,
                              heure,
							  categorie)
                      values( @vl_Ligne,
                              null,
							  XDC_CAT_ACT_COMMANDE)
        end
        if (@va_TypeHeure_in = XZAEC_ACT_SUC)
        begin
            select @vl_Ligne = XZAEC_SYN_PAS_DATE +
                               XZAEC_SYN_ACT_RESULT_SUC
            insert #SYNTHESE( ligne,
                              heure,
							  categorie)
                      values( @vl_Ligne,
                              null,
							  XDC_CAT_ACT_COMMANDE)
        end

    end

    /* AAZ */

	else if @va_Type_in = XDC_ACT_DEPANNEUR
	begin
		/*B Ajout de l'intitule de l'action de type depanneur */
		if @va_TypeHeure_in = XZAEC_ACT_FIN
			select @vl_Ligne = @vl_Date + ' ' + @vl_Heure +
					   ' ' + @vl_Action + XZAEC_SYN_ACT_DPT + 
				   	   XZAEC_SYN_ACT_DEP
		else	select @vl_Ligne = @vl_Date + ' ' + @vl_Heure +
					   ' ' + @vl_Action + XZAEC_SYN_ACT_ARR + 
				   	   XZAEC_SYN_ACT_DEP

		/*B recherche le nom du depanneur et annulation */
		select
			@vl_Depanneur = nom,
			@vl_Resultat = resultat,
			@vl_Annulation = annulation_intervention
		from EXP..ACT_DEP
		where actio = @va_NumAct_in and sit = @va_CleAct_in

		if @vl_Annulation = XDC_VRAI
			select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + XZAEC_SYN_ACT_ANNULEE

		insert #SYNTHESE (ligne, heure, categorie)
		values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)

		/*B ajout du nom du depanneur dans la synthese */
		if @vl_Depanneur != XDC_CHAINE_VIDE
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_DEP_NOM +
					   @vl_Depanneur

			insert #SYNTHESE (ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
		end
		if @va_TypeHeure_in = XZAEC_ACT_FIN and @vl_Resultat is not null and
		   @vl_Resultat != XDC_CHAINE_VIDE
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULTAT +
					   @vl_Resultat

			insert #SYNTHESE (ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
		end

	end

	else if @va_Type_in = XDC_ACT_AFF_GARE
	begin
		/* ajout de l'intitule de l'action de type affichage */
		select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + XZAEC_SYN_ACT_GAR

		insert #SYNTHESE (ligne, heure, categorie)
		values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)

		/*B recherche les informations sur l'affichage en gare */
		select 
			@vl_Gare = nom_gare,
			@vl_Affichette = affichette,
			@vl_Debut = horodate_debut,
			@vl_Fin = horodate_fin
		from EXP..ACT_GAR
		where actio = @va_NumAct_in and sit = @va_CleAct_in

		if @@rowcount != 1
			return XDC_NOK

		/*B ajout du nom de la gare de peage dans la synthese */
		select @vl_Ligne = XZAEC_SYN_PAS_DATE +
				   XZAEC_SYN_ACT_GAR_NOM +
				   @vl_Gare

		insert #SYNTHESE (ligne, heure, categorie)
		values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)

		/*B ajout du texte de l'affichette dans la synthese */
		if @vl_Affichette != XDC_CHAINE_VIDE
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_GAR_TXT +
					   substring(@vl_Affichette, 1, 50),
				@vl_Longueur = char_length(@vl_Affichette)

			insert #SYNTHESE (ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)

			if @vl_Longueur > 50
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   	substring(@vl_Affichette, 51, 50)

				insert #SYNTHESE (ligne, heure, categorie)
				values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
			end

			if @vl_Longueur > 100
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   	substring(@vl_Affichette, 101, 50)

				insert #SYNTHESE (ligne, heure, categorie)
				values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
			end

			if @vl_Longueur > 150
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   	substring(@vl_Affichette, 151, 50)

				insert #SYNTHESE (ligne, heure, categorie)
				values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
			end

			if @vl_Longueur > 200
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   	substring(@vl_Affichette, 201, 50)

				insert #SYNTHESE (ligne, heure, categorie)
				values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
								end
		end
	end

	else if @va_Type_in = XDC_ACT_PAT_SIS
	begin
		/*B ajout de l'intitule de l'action  de type SIS */
		if @va_TypeHeure_in = XZAEC_ACT_FIN
			select @vl_Ligne = @vl_Date + ' ' + @vl_Heure +
					   ' ' + @vl_Action + XZAEC_SYN_ACT_DPT
		else	select @vl_Ligne = @vl_Date + ' ' + @vl_Heure +
					   ' ' + @vl_Action + XZAEC_SYN_ACT_ARR

		select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + XZAEC_SYN_ACT_SIS

		/*B recherche les informations sur le balisage */
		select
			@vl_Intervention = nom_patrouille,
			@vl_DebutBalisage = debut_balisage,
			@vl_FinBalisage = fin_balisage,
			@vl_Balisage = pose_balisage,
			@vl_PRDebBalisage = PR_debut,
			@vl_PRFinBalisage = PR_fin,
			@vl_Annulation = annulation_intervention
		from EXP..ACT_SIS
		where actio = @va_NumAct_in and sit = @va_CleAct_in

		if @@rowcount != 1
			return XDC_NOK

		if @vl_Annulation = XDC_VRAI
			select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + XZAEC_SYN_ACT_ANNULEE

		insert #SYNTHESE (ligne, heure, categorie)
		values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)

		/*B ajout des informations sur le balisage dans la synthese */
		if @vl_Balisage = XDC_OUI and @vl_DebutBalisage != null
		begin
			/* texte correspondant a l'enlevement du balisage */
			if @vl_FinBalisage != null and
			   @va_TypeHeure_in = XZAEC_ACT_FIN
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   convert(char(10), @vl_FinBalisage, 103) +
					   ' ' + convert(char(8), @vl_FinBalisage, 8) +
					   ' : ' + @vl_Intervention + ' ' +
					   XZAEC_SYN_ACT_BAL_OTE

				insert #SYNTHESE (ligne, heure, categorie)
				values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
			end
			else
			begin
			/* texte correspondant a la pose du balisage */
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   convert(char(10), @vl_DebutBalisage, 103)+
					   ' ' + convert(char(8), @vl_DebutBalisage, 8)+
					   ' : ' + @vl_Intervention + ' ' +
					    XZAEC_SYN_ACT_BAL_POS

				insert #SYNTHESE (ligne, heure, categorie)
				values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
			end


			/* Localisation du balisage */
			if @vl_PRFinBalisage is not null
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_SIS_DEB_BAL +
					   convert(char(7),@vl_PRFinBalisage/1000.0)

				insert #SYNTHESE (ligne, heure, categorie)
				values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
			end
			if @vl_PRDebBalisage is not null
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_SIS_FIN_BAL +
					   convert(char(7),@vl_PRDebBalisage/1000.0)

				insert #SYNTHESE (ligne, heure, categorie)
				values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
			end

		end
		else if @vl_Intervention != XDC_CHAINE_VIDE
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_SIS_NOM +
					   @vl_Intervention

			insert #SYNTHESE (ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
		end
	end

	else if @va_Type_in = XDC_ACT_INTERVENTION
	begin
		/* recherche le nom de la patrouille et son type */
		select 
			@vl_Intervention = nom,
			@vl_Type = type,
			@vl_Annulation = annulation_intervention
		from EXP..ACT_INT
		where actio = @va_NumAct_in and sit = @va_CleAct_in

		if @@rowcount = 1
		begin
		/* creation de l'intitule de l'action */
		if @va_TypeHeure_in = XZAEC_ACT_FIN
			select @vl_Ligne = @vl_Date + ' ' + @vl_Heure +
					   ' ' + @vl_Action + XZAEC_SYN_ACT_DPT
		else	select @vl_Ligne = @vl_Date + ' ' + @vl_Heure +
					   ' ' + @vl_Action + XZAEC_SYN_ACT_ARR

		if @vl_Type = XDC_ACT_INT_POM
			select @vl_Ligne = rtrim(@vl_Ligne)+ ' ' + XZAEC_SYN_ACT_INT_POM
		else if @vl_Type = XDC_ACT_INT_GND
			select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + XZAEC_SYN_ACT_INT_GND

		if  @vl_Annulation = XDC_VRAI
			select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + XZAEC_SYN_ACT_ANNULEE
		/*B ajout de l'intitule de l'action  de type intervention */
		insert #SYNTHESE (ligne, heure, categorie)
		values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)

		if @vl_Intervention != XDC_CHAINE_VIDE
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_INT_NOM +
					   @vl_Intervention

			insert #SYNTHESE (ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
		end
		end
	end

	else if @va_Type_in = XDC_ACT_PMV_REMORQUE
	begin
		/*B Ajout de l'intitule de l'action de type remorque */
		select @vl_Ligne = rtrim(@vl_Ligne)+ ' ' + XZAEC_SYN_ACT_RMQ
		insert #SYNTHESE (ligne, heure, categorie)
		values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)

		/*B recherche les informations sur PMV sur remorque */
		select
			@vl_Texte	= message,
			@vl_PR		= PR,
			@vl_Sens	= sens,
			@vl_autoroute 	= autoroute
		from EXP..ACT_RMQ
		where actio = @va_NumAct_in and sit = @va_CleAct_in

		if @@rowcount != 1
			return XDC_NOK

		/*B ajout de la localisation du pmv sur remorque dans la synthese */
		if (@va_TypeHeure_in = XZAEC_ACT_SUC or @va_TypeHeure_in = XZAEC_ACT_ECHEC) and 
		   @vl_PR != null and @vl_Sens != null
		begin

			/* mise en forme de la localisation */
			exec XZAE80	null, null, null, null,
					@vl_autoroute, @vl_PR, @vl_Sens,
					@vl_localisation output

			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RMQ_LOC +
					   @vl_localisation

			insert #SYNTHESE (ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
		end

		/*B ajout du message dans la synthese */
		if @vl_Texte != XDC_CHAINE_VIDE
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RMQ_TXT +
					   substring(@vl_Texte, 1, 48)

			insert #SYNTHESE (ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
		end

		/*B resultat de l'action  dans la synthese */
		if (@va_TypeHeure_in = XZAEC_ACT_ECHEC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_ECHEC
			insert #SYNTHESE (ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
		end
		if (@va_TypeHeure_in = XZAEC_ACT_SUC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_SUC
			insert #SYNTHESE (ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
		end

	end

	else if @va_Type_in = XDC_ACT_PMV_FOURGON
	begin
		/*B Ajout de l'intitule de l'action de type PMV sur fourgon */
		select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + XZAEC_SYN_ACT_FRG
		insert #SYNTHESE (ligne, heure, categorie)
		values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)

		/*B recherche les informations sur PMV sur remorque */
		select
			@vl_Texte	= message,
			@vl_PR		= PR,
			@vl_Sens	= sens,
			@vl_autoroute	= autoroute
		from EXP..ACT_FRG
		where actio = @va_NumAct_in and sit = @va_CleAct_in

		if @@rowcount != 1
			return XDC_NOK

		/*B ajout de la localisation du pmv sur remorque dans la synthese */
		if @va_TypeHeure_in = XZAEC_ACT_SUC and @vl_PR != null and
		   @vl_Sens != null
		begin
			/* mise en forme de la localisation */
			exec XZAE80	null, null, null, null,
					@vl_autoroute, @vl_PR, @vl_Sens,
					@vl_localisation output

			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_FRG_LOC +
					   @vl_localisation

			insert #SYNTHESE (ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
		end

		/*B ajout du message dans la synthese */
		if @vl_Texte != XDC_CHAINE_VIDE
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_FRG_TXT +
					   substring(@vl_Texte, 1, 20)

			insert #SYNTHESE (ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
		end

		/*B resultat de l'action */
		if (@va_TypeHeure_in = XZAEC_ACT_ECHEC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_ECHEC
			insert #SYNTHESE (ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
		end
		if (@va_TypeHeure_in = XZAEC_ACT_SUC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_SUC
			insert #SYNTHESE (ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
		end

	end

	else if @va_Type_in = XDC_ACT_TFM
	begin
		/*B Ajout de l'intitule de l'action de type TFM */
		select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + XZAEC_SYN_ACT_TFM
		insert #SYNTHESE (ligne, heure, categorie)
		values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)

		/*B recherche nombre de diffusions */
		select
			@vl_Diffusion = isnull(nombre_diffusions, 0)
		from EXP..ACT_TFM
		where actio = @va_NumAct_in and sit = @va_CleAct_in

		if @@rowcount != 1
			return XDC_NOK

		/*B ajout du nombre de diffusions dans la synthese */
		select @vl_Ligne = XZAEC_SYN_PAS_DATE +
				   XZAEC_SYN_ACT_TFM_DIFF +
				   convert(varchar(8), @vl_Diffusion)

		insert #SYNTHESE (ligne, heure, categorie)
		values( @vl_Ligne, null, XDC_CAT_ACT_COMMANDE)

		select @vl_Fin = heure_fin 
		from EXP..ACT_GEN
		where numero = @va_NumAct_in and sit = @va_CleAct_in
			
		if @vl_Fin is not null
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
				   XZAEC_SYN_ACT_TFM_ARRET +
				   convert(char(10), @vl_Fin, 103)+' '+convert(char(8), @vl_Fin, 8)

			insert #SYNTHESE (ligne, heure, categorie)
			values( @vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
		end
	end

	else if @va_Type_in = XDC_ACT_EXPTFM
	begin
		/*B Ajout de l'intitule de l'action de type TFM */
		select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + XZAEC_SYN_ACT_EXPTFM
		insert #SYNTHESE (ligne, heure, categorie)
		values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)

		select @vl_Fin = heure_fin 
		from EXP..ACT_GEN
		where numero = @va_NumAct_in and sit = @va_CleAct_in
	end

	else if @va_Type_in = XDC_ACT_EXP
	begin
		/*B Ajout de l'intitule de l'action de type TFM */
		select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + XZAEC_SYN_ACT_EXP
		insert #SYNTHESE (ligne, heure, categorie)
		values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)

		if (@va_TypeHeure_in = XZAEC_ACT_SUC)
		begin
			/*B recherche gravite et trafic */
			select
				@vl_Gravite = gravite,
				@vl_Trafic = trafic
			from EXP..ACT_EXP_ADA
			where actio = @va_NumAct_in and sit = @va_CleAct_in
		
			if @vl_Gravite=0
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_GRAVITE + XZAEC_SYN_GRAVITE0
				insert #SYNTHESE (ligne, heure, categorie)
				values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
			end
			else if @vl_Gravite=1
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_GRAVITE + XZAEC_SYN_GRAVITE1
				insert #SYNTHESE (ligne, heure, categorie)
				values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
			end
			else if @vl_Gravite=2
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_GRAVITE + XZAEC_SYN_GRAVITE2
				insert #SYNTHESE (ligne, heure, categorie)
				values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
			end
			else if @vl_Gravite=3
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_GRAVITE + XZAEC_SYN_GRAVITE3
				insert #SYNTHESE (ligne, heure, categorie)
				values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
			end
		
			if @vl_Trafic=XDC_RADT_FLUIDE
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_TRAFIC + XZAEC_SYN_TRAFIC_FLUIDE
				insert #SYNTHESE (ligne, heure, categorie)
				values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
			end
			else if @vl_Trafic=XDC_RADT_PRESAT
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_TRAFIC + XZAEC_SYN_TRAFIC_PRESAT
				insert #SYNTHESE (ligne, heure, categorie)
				values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
			end
		 	if @vl_Trafic=XDC_RADT_SATURE
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_TRAFIC + XZAEC_SYN_TRAFIC_SATURE
				insert #SYNTHESE (ligne, heure, categorie)
				values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
			end
		end
	end

	else if @va_Type_in = XDC_ACT_ECHANGEUR
	begin
		/*B Ajout de l'intitule de l'action de type echangeur */
		select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + XZAEC_SYN_ACT_ECH

		/* ajouter le nom de l'equipement a l'intitule */
		if @va_Eqt_in != null
		begin
			select @vl_NomEqt = nom
			from CFG..EQT_GEN
			where numero = @va_Eqt_in and type = @va_Type_in

			if @@rowcount != 1
				return XDC_NOK

			select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + @vl_NomEqt
		end

		insert #SYNTHESE (ligne, heure, categorie)
		values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)

		/*B recherche de la sequence de l'echangeur */
		select
			@vl_Sequence = sequence
		from EXP..ACT_ECH
		where actio = @va_NumAct_in and sit = @va_CleAct_in

		/*B ajout de la sequence de l'echangeur dans la synthese */
		if @vl_Sequence != null 
		begin
			select @vl_NomSequence = nom
			from CFG..EQT_SEQ_ECH
			where numero = @vl_Sequence

			if @@rowcount = 1
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_SEQ + @vl_NomSequence

				insert #SYNTHESE (ligne, heure, categorie)
				values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
			end
		end


		/*B resultat de l'action */
		if (@va_TypeHeure_in = XZAEC_ACT_ECHEC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_ECHEC
			insert #SYNTHESE (ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
		end
		if (@va_TypeHeure_in = XZAEC_ACT_SUC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_SUC
			insert #SYNTHESE (ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_COMMANDE)
		end
	end
	else return XDC_NOK

	return XDC_OK

#endif

go

/* supprimer la table */
drop table #SYNTHESE
go
