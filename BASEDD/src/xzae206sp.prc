/*E*/
/* Fichier : $Id: xzae206sp.prc,v 1.13 2019/01/07 14:54:22 pc2dpdy Exp $        Release : $Revision: 1.13 $        Date : $Date: 2019/01/07 14:54:22 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae206sp.prc
------------------------------------------------------
* DESCRIPTION DE LA PROCEDURE :
*
* retourne la synth�se d'un evt ou operation
*
------------------------------------------------------
* HISTORIQUE :
*
* Hilmarcher 10/03/05: Creation proc idem xzae82sp.prc
*			mais insert dans FMC_LIS a la place de #SYNTHESE
*			+ param entree: poste v1.1 - v1.2
* JPL	18/04/07 : Ajout d'un argument aux appels XZAE80 (DEM 638) 1.3
* AAZ Nov 2007 : DEM 663
* JBL	Mai 2008	: DEM 663 ajout GTC_ECH 1.5&1.6
* JPL	08/06/09 : Ajout GTC_ECH sur districts (completer DEM 663); parametre TypeFmc de type T_NOM 1.7
* JPL	10/06/09 : Ajout du traitement des actions PMVA 1.8
* JPL	10/07/09 : Ajout du traitement des actions BAF, BAD 1.9
* JPL	01/09/09 : Unification EXP / HIS (deplacement section GTC_EXP) ; identique a la precedente 1.10
* JPL	02/09/09 : Ajout de la localisation des actions (DEM 519) 1.11
* JPL	14/11/12 : Ajout de l'action d'envoi des conditions de conduite (DEM 1032)  1.12
* JMG	08/08/18 : ligne PMV 18 car . suppression SAGA DEM1306 1.13
* GGY	28/04/23 : Augmentation taille vl_Bandeau a char(XDC_LGR_TXT_PICTO) et vl_pictogramme (XDC_LGR_NOM_PICTO) (DEM-473)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Ajout des actions dans la synthese d'un evenement
* a une date donnee
* 
* Sequence d'appel
* SP	XZAE206
* 
* Arguments en entree
* XDY_Entier    va_NumEvt_in
* XDY_Octet     va_CleEvt_in
* XDY_Nom	va_TypeFmc
* XDY_Entier	va_NumAct_in
* XDY_Octet     va_CleAct_in
* XDY_Octet     va_SiteOrigine_in
* XDY_Octet     va_Type_in
* XDC_Horodate	va_Heure_in
* XDY_Octet	va_TypeHeure_in
* XDY_Eqt	va_Eqt_in
* XDY_Autoroute	va_Autoroute_in
* XDY_PR	va_PR_in
* XDY_Sens	va_Sens_in
* XDY_Octet     va_NumPoste_in
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
* Cette procedure est utilis�e par la procedure XZAE204
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

if exists (select * from sysobjects where name = 'XZAE206' and type = 'P')
	drop procedure XZAE206
go


#ifdef HIST
	/*! CREATION DE LA PROCEDURE AU CI */

create procedure XZAE206
	@va_NumEvt_in		int		= null,
	@va_CleEvt_in           tinyint         = null,
	@va_TypeFmc		T_NOM		= null,
	@va_NumAct_in		int		= null,
	@va_CleAct_in		tinyint		= null,
	@va_SiteOrigine_in	tinyint		= null,
	@va_Type_in		tinyint		= null,
	@va_Heure_in		datetime	= null,
	@va_TypeHeure_in	tinyint		= null,
	@va_Eqt_in		T_EQUIPEMENT	= null,
	@va_Autoroute_in	T_AUTOROUTE	= null,
	@va_PR_in		T_PR		= null,
	@va_Sens_in		T_SENS		= null,
	@va_NumPoste_in		smallint	= null
as
	declare @vl_Ligne char(100), @vl_LignePicto char(80), @vl_NomEqt char(10), 
		@vl_DirectLigne char(18), @vl_Bandeau char(XDC_LGR_TXT_PICTO),
		@vl_TypeEqt tinyint, @vl_Interlocuteur smallint,
		@vl_TypeAstreinte tinyint, @vl_NomTypeAstreinte T_NOM,
		@vl_Diffusion smallint, @vl_Texte char(20), 
		@vl_Affichette varchar(250), @vl_PR T_PR, @vl_Sens T_SENS, 
		@vl_Gare T_NOM, @vl_Debut datetime, @vl_Fin datetime, 
		@vl_DebutBalisage datetime, @vl_FinBalisage datetime, 
		@vl_Balisage bit, @vl_NomInterlocuteur T_NOM, 
		@vl_Intervention varchar(25), @vl_Type tinyint, 
		@vl_Scenario char(4), @vl_NomScenario T_NOM, 
		@vl_Sequence tinyint, @vl_Depanneur T_NOM, 
		@vl_Longueur int,  @vl_Resultat char(25), @vl_Pictogramme char(XDC_LGR_NOM_PICTO),
		@vl_NomSequence T_NOM, @vl_Date char(10), @vl_Heure char(8),
		@vl_Action char(9), @vl_ChaineFixe char(20), 
		@vl_ChaineAction varchar(20), @vl_CaractDebut int,
		@vl_Numero varchar(6), @vl_PRDebBalisage T_PR,
		@vl_PRFinBalisage T_PR, @vl_localisation char(60),
		@vl_LibMod1 char(30), @vl_LibMod2 char(30), @vl_LibMod3 char(30),
		@vl_LibMod4 char(30), @vl_LibMod5 char(30),
		@vl_Annulation tinyint,
		@vl_texte1 char(255),@vl_Ligne1 char(18),@vl_Ligne2 char(18),@vl_Ligne3 char(18),@vl_Ligne4 char(18),
		@vl_Gravite tinyint,@vl_Trafic tinyint,@vl_index tinyint, @vl_autoroute tinyint,
		@vl_Scenario_PAL char(6), @vl_NomScenario_PAL char(50), @vl_numTypeFmc int,
		@vl_Scenario_BAD char(10),
		@vl_Mod1 tinyint, @vl_Mod2 tinyint, @vl_Mod3 tinyint, @vl_Mod4 tinyint,@vl_Mod6 tinyint, @vl_Mod5 tinyint

	/*A Controle des parametres d'entree obligatoire */
	if @va_NumAct_in = null or @va_CleAct_in = null or 
	   @va_Type_in = null or @va_Heure_in = null or @va_TypeHeure_in = null
		return XDC_ARG_INV

	/* conversion du numero d'action en chaine de caracteres */
	select @vl_ChaineAction = convert(char(20), @va_NumAct_in)

	/* calcul la position du premier caractere a prendre */
	select @vl_Longueur = char_length(@vl_ChaineAction), 
		@vl_ChaineFixe = @vl_ChaineAction
	if @vl_Longueur < XZAEC_NB_DIGITS_LIB_ACT
	begin
		select @vl_CaractDebut = 1
		select @vl_Numero = substring(@vl_ChaineFixe, @vl_CaractDebut, @vl_Longueur)
	end
	else    
	begin
		select @vl_CaractDebut = @vl_Longueur - (XZAEC_NB_DIGITS_LIB_ACT - 1)
		select @vl_Numero = substring(@vl_ChaineFixe, @vl_CaractDebut, XZAEC_NB_DIGITS_LIB_ACT)
	end

	/*A formatage de la date et du numero d'action */
	select	@vl_Date = convert(char(10), @va_Heure_in, 103),
		@vl_Heure = convert(char(8), @va_Heure_in, 8),
		@vl_Action = @vl_Numero + '-' + convert(char(2), @va_CleAct_in) + ' - ' + convert(char(2), @va_SiteOrigine_in)

        select @vl_numTypeFmc = type from HIS..FMC_GEN where numero=@va_NumEvt_in and cle=@va_CleEvt_in
	if @vl_numTypeFmc = XZAEC_FMC_QueueBouchon
		select @va_TypeFmc = XZAEC_SYN_FMC_QueueBouchon
	if @vl_numTypeFmc = XZAEC_FMC_TeteBouchon
		select @va_TypeFmc = XZAEC_SYN_FMC_TeteBouchon

	/* Mise en forme de la localisation de l'evenement */
	exec XZAE80	null, null, null, null,
			@va_Autoroute_in, @va_PR_in, @va_Sens_in,
			@vl_localisation output

	/*B Ajout d'une ligne "ACTION" datee mentionnant le numero et la localisation de l'evenement */
	select @vl_Ligne = @vl_Date + ' ' + @vl_Heure + ' N' +
				convert(char(6), @va_NumEvt_in) + '- ' + convert(char(2), @va_CleEvt_in) +
				' ' +  convert(varchar(60),@va_TypeFmc) + " - ACTION " + ' ' + @vl_localisation
	insert HIS..FMC_LIS(numero,cle,ligne,heure,poste)
	values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)


	/*A Creation de l'intitule de l'action dans la synthese */
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
	   @va_Type_in = XDC_ACT_BIP
	begin
		/*B recherche de l'interlocuteur pour un appel */
		select
			@vl_Interlocuteur = interlocuteur,
			@vl_TypeAstreinte = type_d_astreinte
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
			if @va_Type_in = XDC_ACT_FAX
				select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + XZAEC_SYN_ACT_FAX
			else if @va_Type_in = XDC_ACT_TEL
				select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + XZAEC_SYN_ACT_TEL
			else if @va_Type_in = XDC_ACT_BIP
				select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + XZAEC_SYN_ACT_BIP
			else return XDC_NOK
		end
		else
		begin
			if @va_Type_in = XDC_ACT_FAX
				select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + XZAEC_SYN_ACT_FAX_AST + @vl_NomTypeAstreinte
			else if @va_Type_in = XDC_ACT_TEL
				select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + XZAEC_SYN_ACT_TEL_AST + @vl_NomTypeAstreinte
			else if @va_Type_in = XDC_ACT_BIP
				select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + XZAEC_SYN_ACT_BIP_AST + @vl_NomTypeAstreinte
			else return XDC_NOK
		end

		/*B Ajout de l'intitule de l'action de type appel */
		insert HIS..FMC_LIS(numero,cle,ligne,heure,poste)
		values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)

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
			insert HIS..FMC_LIS(numero,cle,ligne,heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end
		else if (@va_TypeHeure_in = XZAEC_ACT_SUC)
		begin
			select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + XZAEC_SYN_ACT_REUSSIE
			insert HIS..FMC_LIS(numero,cle,ligne,heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end

	end

	else if @va_Type_in = XDC_ACT_CCH
	begin
		/*B Ajout de l'intitule de l'action de type CCH */
		select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + XZAEC_SYN_ACT_CCH
		insert HIS..FMC_LIS(numero,cle,ligne,heure,poste)
		values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)

		/*B resultat de l'action */
		if (@va_TypeHeure_in = XZAEC_ACT_ECHEC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_ECHEC
			insert HIS..FMC_LIS(numero,cle,ligne,heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end
		if (@va_TypeHeure_in = XZAEC_ACT_SUC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_SUC
			insert HIS..FMC_LIS(numero,cle,ligne,heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
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
		insert HIS..FMC_LIS(numero,cle,ligne,heure,poste)
		values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)

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
		insert HIS..FMC_LIS(numero,cle,ligne,heure,poste)
		values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)

		/*B resultat de l'action */
		if (@va_TypeHeure_in = XZAEC_ACT_ECHEC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_ECHEC
			insert HIS..FMC_LIS(numero,cle,ligne,heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end
		if (@va_TypeHeure_in = XZAEC_ACT_SUC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_SUC
			insert HIS..FMC_LIS(numero,cle,ligne,heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end

	end

        else if @va_Type_in =  XDC_ACT_BAF
        begin
                select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + XZAEC_SYN_ACT_BAF

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

                /* Ajout de l'intitule de l'action de type BAF */
                insert HIS..FMC_LIS(numero,cle,ligne,heure,poste)
                values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)

                /* recherche de l'etat des modules feux et barrieres de la BAF */
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
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_ACT_BAF_GLOBAL_DROIT
			insert HIS..FMC_LIS(numero,cle,ligne,heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end
		else if (@vl_Mod1!=0) and (@vl_Mod2!=0)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_ACT_BAF_FEUX_DROITS
			insert HIS..FMC_LIS(numero,cle,ligne,heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end
		else 
		begin
                	if (@vl_Mod1!=0)
                	begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_ACT_BAF_MOD1
				insert HIS..FMC_LIS(numero,cle,ligne,heure,poste)
				values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
                	end

                	if (@vl_Mod2!=0)
                	begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_ACT_BAF_MOD2
				insert HIS..FMC_LIS(numero,cle,ligne,heure,poste)
				values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
                	end

                	if (@vl_Mod3!=0)
                	begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_ACT_BAF_MOD3
				insert HIS..FMC_LIS(numero,cle,ligne,heure,poste)
				values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
                	end
		end


		if (@vl_Mod4!=0) and (@vl_Mod5!=0) and (@vl_Mod6!=0) 
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_ACT_BAF_GLOBAL_GAUCHE
			insert HIS..FMC_LIS(numero,cle,ligne,heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end
		else if (@vl_Mod4!=0) and (@vl_Mod5!=0)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_ACT_BAF_FEUX_GAUCHES
			insert HIS..FMC_LIS(numero,cle,ligne,heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end
		else 
		begin
                	if (@vl_Mod4!=0)
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_ACT_BAF_MOD4
				insert HIS..FMC_LIS(numero,cle,ligne,heure,poste)
				values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
                	end

                	if (@vl_Mod5!=0)
                	begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_ACT_BAF_MOD5
				insert HIS..FMC_LIS(numero,cle,ligne,heure,poste)
				values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
                	end

                	if (@vl_Mod6!=0)
                	begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_ACT_BAF_MOD6
				insert HIS..FMC_LIS(numero,cle,ligne,heure,poste)
				values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
                	end
		end

                /* resultat de l'action */
                if (@va_TypeHeure_in = XZAEC_ACT_ECHEC)
                begin
                        select @vl_Ligne = XZAEC_SYN_PAS_DATE +
                                           XZAEC_SYN_ACT_RESULT_ECHEC
			insert HIS..FMC_LIS(numero,cle,ligne,heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
                end
                if (@va_TypeHeure_in = XZAEC_ACT_SUC)
                begin
                        select @vl_Ligne = XZAEC_SYN_PAS_DATE +
                                           XZAEC_SYN_ACT_RESULT_SUC
			insert HIS..FMC_LIS(numero,cle,ligne,heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
                end

        end

        else if @va_Type_in = XDC_ACT_BAD
        begin
		select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + XZAEC_SYN_ACT_BAD

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

		/*B Ajout de l'intitule de l'action de type BAD */
                insert HIS..FMC_LIS(numero,cle,ligne,heure,poste)
                values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)

		/*B recherche du scenario de la BAD */
                select
                        @vl_Scenario_BAD = scenario
                from HIS..ACT_BAD
                where actio = @va_NumAct_in and sit = @va_CleAct_in

                if @@rowcount != 1
                        return XDC_NOK

                if @vl_Scenario_BAD != XDC_CHAINE_VIDE
                begin
/*                        select @vl_NomScenario = nom
                        from CFG..EQT_SCN_NAV
                        where scenario = @vl_Scenario_BAD*/

			select @vl_NomScenario = @vl_Scenario_BAD

                        if @@rowcount != 1
                                select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_ACT_SCN + @vl_Scenario_BAD
                        else    select @vl_Ligne = XZAEC_SYN_PAS_DATE +
                                           XZAEC_SYN_ACT_SCN + @vl_NomScenario
                end

                insert HIS..FMC_LIS(numero,cle,ligne,heure,poste)
                values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)

		/*B resultat de l'action */
                if (@va_TypeHeure_in = XZAEC_ACT_ECHEC)
                begin
                        select @vl_Ligne = XZAEC_SYN_PAS_DATE +
                                           XZAEC_SYN_ACT_RESULT_ECHEC
                        insert HIS..FMC_LIS(numero,cle,ligne,heure,poste)
                        values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
                end
                if (@va_TypeHeure_in = XZAEC_ACT_SUC)
                begin
                        select @vl_Ligne = XZAEC_SYN_PAS_DATE +
                                           XZAEC_SYN_ACT_RESULT_SUC
                        insert HIS..FMC_LIS(numero,cle,ligne,heure,poste)
                        values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
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
		insert HIS..FMC_LIS(numero,cle,ligne,heure,poste)
		values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)

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

		insert HIS..FMC_LIS(numero,cle,ligne,heure,poste)
		values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)

		/*B resultat de l'action */
		if (@va_TypeHeure_in = XZAEC_ACT_ECHEC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_ECHEC
			insert HIS..FMC_LIS(numero,cle,ligne,heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end
		if (@va_TypeHeure_in = XZAEC_ACT_SUC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_SUC
			insert HIS..FMC_LIS(numero,cle,ligne,heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
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
		insert HIS..FMC_LIS(numero,cle,ligne,heure,poste)
		values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)

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
		insert HIS..FMC_LIS(numero,cle,ligne,heure,poste)
		values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)

		insert HIS..FMC_LIS(numero,cle,ligne,heure,picto,poste)
		values (@va_NumEvt_in, @va_CleEvt_in,@vl_LignePicto, null,@vl_Pictogramme,@va_NumPoste_in)

		/*B ajout du texte du bandeau du PMV  dans la synthese si non null*/
		if (@vl_Bandeau!=null) and (@vl_Bandeau!=XDC_CHAINE_VIDE)
		begin   
			insert HIS..FMC_LIS(numero,cle,ligne,heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,
				XZAEC_SYN_PAS_DATE + XZAEC_SYN_ACT_PMV_BAND + @vl_Bandeau,
				null,@va_NumPoste_in)
		end

		/*B ajout du texte de 4eme ligne du PMV  dans la synthese si non null*/
		if (@vl_DirectLigne!=null) and (@vl_DirectLigne!=XDC_CHAINE_VIDE)
		begin   
			insert HIS..FMC_LIS(numero,cle,ligne,heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,
				XZAEC_SYN_PAS_DATE + XZAEC_SYN_ACT_PMV_TXT_4 + @vl_DirectLigne, 
				null,@va_NumPoste_in)
		end


		/*B resultat de l'action */
		if (@va_TypeHeure_in = XZAEC_ACT_ECHEC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_ECHEC
			insert HIS..FMC_LIS(numero,cle,ligne,heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end
		if (@va_TypeHeure_in = XZAEC_ACT_SUC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_SUC
			insert HIS..FMC_LIS(numero,cle,ligne,heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end

	end

	else if @va_Type_in = XDC_ACT_PMVA
	begin
		select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + XZAEC_SYN_ACT_PMVA

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

		/*B Ajout de l'intitule de l'action de type PMVA */
		insert HIS..FMC_LIS(numero,cle,ligne,heure,poste)
		values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)

		/*B recherche du texte du PMVA */
		select
			@vl_Ligne = XZAEC_SYN_PAS_DATE +
			            XZAEC_SYN_ACT_PMVA_TXT +
			            premiere_ligne + ' | ' + deuxieme_ligne +
			            ' | ' + troisieme_ligne
		from HIS..ACT_PMA
		where actio = @va_NumAct_in and sit = @va_CleAct_in

		if @@rowcount != 1
			return XDC_NOK

		/*B ajout du texte du PMVA dans la synthese */
		insert HIS..FMC_LIS(numero,cle,ligne,heure,poste)
		values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)

		/*B recherche du texte des 4eme et 5eme lignes du PMVA */
		select
			@vl_Ligne = XZAEC_SYN_PAS_DATE +
			            XZAEC_SYN_ACT_PMVA_TXT +
			            quatrieme_ligne + ' | ' + cinquieme_ligne
		from HIS..ACT_PMA
		where actio = @va_NumAct_in and sit = @va_CleAct_in

		if @@rowcount != 1
			return XDC_NOK

		insert HIS..FMC_LIS(numero,cle,ligne,heure,poste)
		values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)

		/*B resultat de l'action */
		if (@va_TypeHeure_in = XZAEC_ACT_ECHEC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
			                   XZAEC_SYN_ACT_RESULT_ECHEC
			insert HIS..FMC_LIS(numero,cle,ligne,heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
		end
		if (@va_TypeHeure_in = XZAEC_ACT_SUC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
			                   XZAEC_SYN_ACT_RESULT_SUC
			insert HIS..FMC_LIS(numero,cle,ligne,heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
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
		insert HIS..FMC_LIS(numero,cle,ligne,heure,poste)
		values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)

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

		insert HIS..FMC_LIS(numero,cle,ligne,heure,poste)
		values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)

		insert HIS..FMC_LIS(numero,cle,ligne,heure,picto,poste)
		values(@va_NumEvt_in, @va_CleEvt_in,@vl_LignePicto, null,@vl_Pictogramme,@va_NumPoste_in)

		/*B ajout du texte du bandeau du PMV  dans la synthese si non null*/
		if (@vl_Bandeau!=null) and (@vl_Bandeau!=XDC_CHAINE_VIDE)
		begin   
			insert HIS..FMC_LIS(numero,cle,ligne,heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,
				XZAEC_SYN_PAS_DATE + XZAEC_SYN_ACT_PMV_BAND + @vl_Bandeau, 
				null,@va_NumPoste_in)
		end

		/*B ajout du texte de 4eme ligne du PMV  dans la synthese si non null*/
		if (@vl_DirectLigne!=null) and (@vl_DirectLigne!=XDC_CHAINE_VIDE)
		begin   
			insert HIS..FMC_LIS(numero,cle,ligne,heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,
				XZAEC_SYN_PAS_DATE + XZAEC_SYN_ACT_PMV_TXT_4 + @vl_DirectLigne,
				null,@va_NumPoste_in)
		end


		/*B resultat de l'action */
		if (@va_TypeHeure_in = XZAEC_ACT_ECHEC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_ECHEC

			insert HIS..FMC_LIS(numero,cle,ligne,heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end
		if (@va_TypeHeure_in = XZAEC_ACT_SUC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_SUC
			insert HIS..FMC_LIS(numero,cle,ligne,heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
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

		insert HIS..FMC_LIS(numero,cle,ligne,heure,poste)
		values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)

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

				insert HIS..FMC_LIS(numero,cle,ligne,heure,poste)
				values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
			end
		end

		/*B resultat de l'action */
		if (@va_TypeHeure_in = XZAEC_ACT_ECHEC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_ECHEC
			insert HIS..FMC_LIS(numero,cle,ligne,heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end
		if (@va_TypeHeure_in = XZAEC_ACT_SUC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_SUC
			insert HIS..FMC_LIS(numero,cle,ligne,heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end

	end

	/* AAZ Ajout Domaine SIG, ECL, VEN, ENR et INC */

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

		insert HIS..FMC_LIS( numero,
		                     cle,
		                     ligne,
		                     heure,poste )
		            values ( @va_NumEvt_in,
		                     @va_CleEvt_in,
		                     @vl_Ligne, 
		                     null,
		                     @va_NumPoste_in)

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

				insert HIS..FMC_LIS( numero,
				                     cle,
				                     ligne,
				                     heure,
				                     poste )
				            values ( @va_NumEvt_in,
				                     @va_CleEvt_in,
				                     @vl_Ligne,
				                     null,
				                     @va_NumPoste_in)
			end
		end

		/*B resultat de l'action */
		if (@va_TypeHeure_in = XZAEC_ACT_ECHEC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_ECHEC
			insert HIS..FMC_LIS( numero,
			                     cle,
			                     ligne,
			                     heure,
			                     poste)
			            values ( @va_NumEvt_in,
			                     @va_CleEvt_in,
			                     @vl_Ligne,
			                     null,
			                     @va_NumPoste_in)
		end
		if (@va_TypeHeure_in = XZAEC_ACT_SUC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_SUC
			insert HIS..FMC_LIS( numero,
			                     cle,
			                     ligne,
			                     heure,
			                     poste)
			            values ( @va_NumEvt_in,
			                     @va_CleEvt_in,
			                     @vl_Ligne,
			                     null,
			                     @va_NumPoste_in)
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
			where  numero = @va_Eqt_in  and
			       type   = XDC_EQT_ECH

			if @@rowcount != 1
				return XDC_NOK

			select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + @vl_NomEqt
		end

		insert HIS..FMC_LIS( numero,
		                     cle,
		                     ligne,
		                     heure,poste)
                     values( @va_NumEvt_in,
                             @va_CleEvt_in,
                             @vl_Ligne,
                             null,
                             @va_NumPoste_in) 

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
			and type = XDC_EQT_ECH

			if @@rowcount = 1
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_SEQ + @vl_NomSequence

				insert HIS..FMC_LIS( numero,
				                     cle,
				                     ligne,
				                     heure,
				                     poste)
				             values( @va_NumEvt_in,
				                     @va_CleEvt_in,
				                     @vl_Ligne,
				                     null,
				                     @va_NumPoste_in)
			end
		end

		/*B resultat de l'action */
		if (@va_TypeHeure_in = XZAEC_ACT_ECHEC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_ECHEC
			insert HIS..FMC_LIS( numero,
			                     cle,
			                     ligne,
			                     heure,poste)
			             values( @va_NumEvt_in,
			                     @va_CleEvt_in,
			                     @vl_Ligne,
			                     null,
			                     @va_NumPoste_in)
		end
		if (@va_TypeHeure_in = XZAEC_ACT_SUC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_SUC
			insert HIS..FMC_LIS( numero,
			                     cle,
			                     ligne,
			                     heure,
			                     poste)
		  	             values( @va_NumEvt_in, 
		  	                     @va_CleEvt_in,
		  	                     @vl_Ligne,
		  	                     null,
		  	                     @va_NumPoste_in)	
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

		insert HIS..FMC_LIS( numero,
		                     cle,
		                     ligne,
		                     heure,poste )
		            values ( @va_NumEvt_in,
		                     @va_CleEvt_in,
		                     @vl_Ligne, 
		                     null,
		                     @va_NumPoste_in)

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

				insert HIS..FMC_LIS( numero,
				                     cle,
				                     ligne,
				                     heure,
				                     poste )
				            values ( @va_NumEvt_in,
				                     @va_CleEvt_in,
				                     @vl_Ligne,
				                     null,
				                     @va_NumPoste_in)
			end
		end

		/*B resultat de l'action */
		if (@va_TypeHeure_in = XZAEC_ACT_ECHEC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_ECHEC
			insert HIS..FMC_LIS( numero,
			                     cle,
			                     ligne,
			                     heure,
			                     poste)
			            values ( @va_NumEvt_in,
			                     @va_CleEvt_in,
			                     @vl_Ligne,
			                     null,
			                     @va_NumPoste_in)
		end
		if (@va_TypeHeure_in = XZAEC_ACT_SUC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_SUC
			insert HIS..FMC_LIS( numero,
			                     cle,
			                     ligne,
			                     heure,
			                     poste)
			            values ( @va_NumEvt_in,
			                     @va_CleEvt_in,
			                     @vl_Ligne,
			                     null,
			                     @va_NumPoste_in)
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

		insert HIS..FMC_LIS( numero,
		                     cle,
		                     ligne,
		                     heure,poste )
		            values ( @va_NumEvt_in,
		                     @va_CleEvt_in,
		                     @vl_Ligne, 
		                     null,
		                     @va_NumPoste_in)

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

				insert HIS..FMC_LIS( numero,
				                     cle,
				                     ligne,
				                     heure,
				                     poste )
				            values ( @va_NumEvt_in,
				                     @va_CleEvt_in,
				                     @vl_Ligne,
				                     null,
				                     @va_NumPoste_in)
			end
		end

		/*B resultat de l'action */
		if (@va_TypeHeure_in = XZAEC_ACT_ECHEC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_ECHEC
			insert HIS..FMC_LIS( numero,
			                     cle,
			                     ligne,
			                     heure,
			                     poste)
			            values ( @va_NumEvt_in,
			                     @va_CleEvt_in,
			                     @vl_Ligne,
			                     null,
			                     @va_NumPoste_in)
		end
		if (@va_TypeHeure_in = XZAEC_ACT_SUC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_SUC
			insert HIS..FMC_LIS( numero,
			                     cle,
			                     ligne,
			                     heure,
			                     poste)
			            values ( @va_NumEvt_in,
			                     @va_CleEvt_in,
			                     @vl_Ligne,
			                     null,
			                     @va_NumPoste_in)
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

		insert HIS..FMC_LIS( numero,
		                     cle,
		                     ligne,
		                     heure,poste )
		            values ( @va_NumEvt_in,
		                     @va_CleEvt_in,
		                     @vl_Ligne, 
		                     null,
		                     @va_NumPoste_in)

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

				insert HIS..FMC_LIS( numero,
				                     cle,
				                     ligne,
				                     heure,
				                     poste )
				            values ( @va_NumEvt_in,
				                     @va_CleEvt_in,
				                     @vl_Ligne,
				                     null,
				                     @va_NumPoste_in)
			end
		end

		/*B resultat de l'action */
		if (@va_TypeHeure_in = XZAEC_ACT_ECHEC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_ECHEC
			insert HIS..FMC_LIS( numero,
			                     cle,
			                     ligne,
			                     heure,
			                     poste)
			            values ( @va_NumEvt_in,
			                     @va_CleEvt_in,
			                     @vl_Ligne,
			                     null,
			                     @va_NumPoste_in)
		end
		if (@va_TypeHeure_in = XZAEC_ACT_SUC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_SUC
			insert HIS..FMC_LIS( numero,
			                     cle,
			                     ligne,
			                     heure,
			                     poste)
			            values ( @va_NumEvt_in,
			                     @va_CleEvt_in,
			                     @vl_Ligne,
			                     null,
			                     @va_NumPoste_in)
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

		insert HIS..FMC_LIS( numero,
		                     cle,
		                     ligne,
		                     heure,poste )
		            values ( @va_NumEvt_in,
		                     @va_CleEvt_in,
		                     @vl_Ligne, 
		                     null,
		                     @va_NumPoste_in)

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

				insert HIS..FMC_LIS( numero,
				                     cle,
				                     ligne,
				                     heure,
				                     poste )
				            values ( @va_NumEvt_in,
				                     @va_CleEvt_in,
				                     @vl_Ligne,
				                     null,
				                     @va_NumPoste_in)
			end
		end

		/*B resultat de l'action */
		if (@va_TypeHeure_in = XZAEC_ACT_ECHEC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_ECHEC
			insert HIS..FMC_LIS( numero,
			                     cle,
			                     ligne,
			                     heure,
			                     poste)
			            values ( @va_NumEvt_in,
			                     @va_CleEvt_in,
			                     @vl_Ligne,
			                     null,
			                     @va_NumPoste_in)
		end
		if (@va_TypeHeure_in = XZAEC_ACT_SUC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_SUC
			insert HIS..FMC_LIS( numero,
			                     cle,
			                     ligne,
			                     heure,
			                     poste)
			            values ( @va_NumEvt_in,
			                     @va_CleEvt_in,
			                     @vl_Ligne,
			                     null,
			                     @va_NumPoste_in)
		end

	end

	else if @va_Type_in = XDC_ACT_GTC_EXP
	begin
		/*B Ajout de l'intitule de l'action de type EXPLOITATION */
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

		insert HIS..FMC_LIS( numero,
		                     cle,
		                     ligne,
		                     heure,poste )
		            values ( @va_NumEvt_in,
		                     @va_CleEvt_in,
		                     @vl_Ligne, 
		                     null,
		                     @va_NumPoste_in)

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

				insert HIS..FMC_LIS( numero,
				                     cle,
				                     ligne,
				                     heure,
				                     poste )
				            values ( @va_NumEvt_in,
				                     @va_CleEvt_in,
				                     @vl_Ligne,
				                     null,
				                     @va_NumPoste_in)
			end
		end

		/*B resultat de l'action */
		if (@va_TypeHeure_in = XZAEC_ACT_ECHEC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_ECHEC
			insert HIS..FMC_LIS( numero,
			                     cle,
			                     ligne,
			                     heure,
			                     poste)
			            values ( @va_NumEvt_in,
			                     @va_CleEvt_in,
			                     @vl_Ligne,
			                     null,
			                     @va_NumPoste_in)
		end
		if (@va_TypeHeure_in = XZAEC_ACT_SUC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_SUC
			insert HIS..FMC_LIS( numero,
			                     cle,
			                     ligne,
			                     heure,
			                     poste)
			            values ( @va_NumEvt_in,
			                     @va_CleEvt_in,
			                     @vl_Ligne,
			                     null,
			                     @va_NumPoste_in)
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
			select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + XZAEC_SYN_ACT_ANNULEE

		insert HIS..FMC_LIS(numero,cle,ligne,heure,poste)
		values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)

		/*B ajout du nom du depanneur dans la synthese */
		if @vl_Depanneur != XDC_CHAINE_VIDE
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_DEP_NOM +
					   @vl_Depanneur

			insert HIS..FMC_LIS(numero,cle,ligne,heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end
		if @va_TypeHeure_in = XZAEC_ACT_FIN and @vl_Resultat is not null and
		   @vl_Resultat != XDC_CHAINE_VIDE
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULTAT +
					   @vl_Resultat

			insert HIS..FMC_LIS(numero,cle,ligne,heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end

	end

	else if @va_Type_in = XDC_ACT_AFF_GARE
	begin
		/* ajout de l'intitule de l'action de type affichage */
		select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + XZAEC_SYN_ACT_GAR

		insert HIS..FMC_LIS(numero,cle,ligne,heure,poste)
		values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)

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

		insert HIS..FMC_LIS(numero,cle,ligne,heure,poste)
		values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)

		/*B ajout du texte de l'affichette dans la synthese */
		if @vl_Affichette != XDC_CHAINE_VIDE
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_GAR_TXT +
					   substring(@vl_Affichette, 1, 50),
				@vl_Longueur = char_length(@vl_Affichette)

			insert HIS..FMC_LIS(numero,cle,ligne,heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)

			if @vl_Longueur > 50
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   	substring(@vl_Affichette, 51, 50)

				insert HIS..FMC_LIS(numero,cle,ligne,heure,poste)
				values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
			end

			if @vl_Longueur > 100
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   	substring(@vl_Affichette, 101, 50)

				insert HIS..FMC_LIS(numero,cle,ligne,heure,poste)
				values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
			end

			if @vl_Longueur > 150
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   	substring(@vl_Affichette, 151, 50)

				insert HIS..FMC_LIS(numero,cle,ligne,heure,poste)
				values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
			end

			if @vl_Longueur > 200
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   	substring(@vl_Affichette, 201, 50)

				insert HIS..FMC_LIS(numero,cle,ligne,heure,poste)
				values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
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
			select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + XZAEC_SYN_ACT_ANNULEE

		insert HIS..FMC_LIS(numero,cle,ligne,heure,poste)
		values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)

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

				insert HIS..FMC_LIS(numero,cle,ligne,heure,poste)
				values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
			end
			else
			begin
			/* texte correspondant a la pose du balisage */
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   convert(char(10), @vl_DebutBalisage, 103)+
					   ' ' + convert(char(8), @vl_DebutBalisage, 8)+
					   ' : ' + @vl_Intervention + ' ' +
					    XZAEC_SYN_ACT_BAL_POS

				insert HIS..FMC_LIS(numero,cle,ligne,heure,poste)
				values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
			end


			/* Localisation du balisage */
			if @vl_PRFinBalisage is not null
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_SIS_DEB_BAL +
					   convert(char(7),@vl_PRFinBalisage/1000.0)

				insert HIS..FMC_LIS(numero,cle,ligne,heure,poste)
				values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
			end
			if @vl_PRDebBalisage is not null
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_SIS_FIN_BAL +
					   convert(char(7),@vl_PRDebBalisage/1000.0)

				insert HIS..FMC_LIS(numero,cle,ligne,heure,poste)
				values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
			end

		end
		else if @vl_Intervention != XDC_CHAINE_VIDE
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_SIS_NOM +
					   @vl_Intervention

			insert HIS..FMC_LIS(numero,cle,ligne,heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
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
			select @vl_Ligne = rtrim(@vl_Ligne)+ ' ' + XZAEC_SYN_ACT_INT_POM
		else if @vl_Type = XDC_ACT_INT_GND
			select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + XZAEC_SYN_ACT_INT_GND

		if  @vl_Annulation = XDC_VRAI
			select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + XZAEC_SYN_ACT_ANNULEE
		/*B ajout de l'intitule de l'action  de type intervention */
		insert HIS..FMC_LIS(numero,cle,ligne,heure,poste)
		values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)

		if @vl_Intervention != XDC_CHAINE_VIDE
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_INT_NOM +
					   @vl_Intervention

			insert HIS..FMC_LIS(numero,cle,ligne,heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end
		end
	end

	else if @va_Type_in = XDC_ACT_PMV_REMORQUE
	begin
		/*B Ajout de l'intitule de l'action de type remorque */
		select @vl_Ligne = rtrim(@vl_Ligne)+ ' ' + XZAEC_SYN_ACT_RMQ
		insert HIS..FMC_LIS(numero,cle,ligne,heure,poste)
		values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)

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

			insert HIS..FMC_LIS(numero,cle,ligne,heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end

		/*B ajout du message dans la synthese */
		if @vl_Texte != XDC_CHAINE_VIDE
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RMQ_TXT +
					   substring(@vl_Texte, 1, 48)

			insert HIS..FMC_LIS(numero,cle,ligne,heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end

		/*B resultat de l'action  dans la synthese */
		if (@va_TypeHeure_in = XZAEC_ACT_ECHEC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_ECHEC
			insert HIS..FMC_LIS(numero,cle,ligne,heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end
		if (@va_TypeHeure_in = XZAEC_ACT_SUC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_SUC
			insert HIS..FMC_LIS(numero,cle,ligne,heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end

	end

	else if @va_Type_in = XDC_ACT_PMV_FOURGON
	begin
		/*B Ajout de l'intitule de l'action de type PMV sur fourgon */
		select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + XZAEC_SYN_ACT_FRG
		insert HIS..FMC_LIS(numero,cle,ligne,heure,poste)
		values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)

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

			insert HIS..FMC_LIS(numero,cle,ligne,heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end

		/*B ajout du message dans la synthese */
		if @vl_Texte != XDC_CHAINE_VIDE
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_FRG_TXT +
					   substring(@vl_Texte, 1, 20)

			insert HIS..FMC_LIS(numero,cle,ligne,heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end

		/*B resultat de l'action */
		if (@va_TypeHeure_in = XZAEC_ACT_ECHEC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_ECHEC
			insert HIS..FMC_LIS(numero,cle,ligne,heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end
		if (@va_TypeHeure_in = XZAEC_ACT_SUC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_SUC
			insert HIS..FMC_LIS(numero,cle,ligne,heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end

	end

	else if @va_Type_in = XDC_ACT_TFM
	begin
		/*B Ajout de l'intitule de l'action de type TFM */
		select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + XZAEC_SYN_ACT_TFM
		insert HIS..FMC_LIS(numero,cle,ligne,heure,poste)
		values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)

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

		insert HIS..FMC_LIS(numero,cle,ligne,heure,poste)
		values( @va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)

		select @vl_Fin = heure_fin 
		from HIS..ACT_GEN
		where numero = @va_NumAct_in and sit = @va_CleAct_in
			
		if @vl_Fin is not null
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
				   XZAEC_SYN_ACT_TFM_ARRET +
				   convert(char(10), @vl_Fin, 103)+' '+convert(char(8), @vl_Fin, 8)

			insert HIS..FMC_LIS(numero,cle,ligne,heure,poste)
			values( @va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end
	end

	else if @va_Type_in = XDC_ACT_EXPTFM
	begin
		/*B Ajout de l'intitule de l'action de type TFM */
		select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + XZAEC_SYN_ACT_EXPTFM
		insert HIS..FMC_LIS(numero,cle,ligne,heure,poste)
		values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)

		select @vl_Fin = heure_fin 
		from HIS..ACT_GEN
		where numero = @va_NumAct_in and sit = @va_CleAct_in
	end

	else if @va_Type_in = XDC_ACT_EXP
	begin
		/*B Ajout de l'intitule de l'action de type TFM */
		select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + XZAEC_SYN_ACT_EXP
		insert HIS..FMC_LIS(numero,cle,ligne,heure,poste)
		values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)

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
				insert HIS..FMC_LIS(numero,cle,ligne,heure,poste)
				values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
			end
			else if @vl_Gravite=1
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_GRAVITE + XZAEC_SYN_GRAVITE1
				insert HIS..FMC_LIS(numero,cle,ligne,heure,poste)
				values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
			end
			else if @vl_Gravite=2
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_GRAVITE + XZAEC_SYN_GRAVITE2
				insert HIS..FMC_LIS(numero,cle,ligne,heure,poste)
				values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
			end
			else if @vl_Gravite=3
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_GRAVITE + XZAEC_SYN_GRAVITE3
				insert HIS..FMC_LIS(numero,cle,ligne,heure,poste)
				values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
			end
		
			if @vl_Trafic=XDC_RADT_FLUIDE
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_TRAFIC + XZAEC_SYN_TRAFIC_FLUIDE
				insert HIS..FMC_LIS(numero,cle,ligne,heure,poste)
				values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
			end
			else if @vl_Trafic=XDC_RADT_PRESAT
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_TRAFIC + XZAEC_SYN_TRAFIC_PRESAT
				insert HIS..FMC_LIS(numero,cle,ligne,heure,poste)
				values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
			end
		 	if @vl_Trafic=XDC_RADT_SATURE
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_TRAFIC + XZAEC_SYN_TRAFIC_SATURE
				insert HIS..FMC_LIS(numero,cle,ligne,heure,poste)
				values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
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

		insert HIS..FMC_LIS(numero,cle,ligne,heure,poste)
		values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)

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

				insert HIS..FMC_LIS(numero,cle,ligne,heure,poste)
				values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
			end
		end


		/*B resultat de l'action */
		if (@va_TypeHeure_in = XZAEC_ACT_ECHEC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_ECHEC
			insert HIS..FMC_LIS(numero,cle,ligne,heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end
		if (@va_TypeHeure_in = XZAEC_ACT_SUC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_SUC
			insert HIS..FMC_LIS(numero,cle,ligne,heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end
	end
	else return XDC_NOK

	return XDC_OK

#else
	/*! CREATION DE LA PROCEDURE DANS UN DISTRICT */

create procedure XZAE206
	@va_NumEvt_in		int		= null,
	@va_CleEvt_in           tinyint         = null,
	@va_TypeFmc		T_NOM		= null,
	@va_NumAct_in		int		= null,
	@va_CleAct_in		tinyint		= null,
	@va_SiteOrigine_in	tinyint		= null,
	@va_Type_in		tinyint		= null,
	@va_Heure_in		datetime	= null,
	@va_TypeHeure_in	tinyint		= null,
	@va_Eqt_in		T_EQUIPEMENT	= null,
	@va_Autoroute_in	T_AUTOROUTE	= null,
	@va_PR_in		T_PR		= null,
	@va_Sens_in		T_SENS		= null,
	@va_NumPoste_in		smallint	= null
as
	declare @vl_Ligne char(100), @vl_LignePicto char(80), @vl_NomEqt char(10), 
		@vl_DirectLigne char(18), @vl_Bandeau char(XDC_LGR_TXT_PICTO),
		@vl_TypeEqt tinyint, @vl_Interlocuteur smallint,
		@vl_TypeAstreinte tinyint, @vl_NomTypeAstreinte T_NOM,
		@vl_Diffusion smallint, @vl_Texte char(20), 
		@vl_Affichette varchar(250), @vl_PR T_PR, @vl_Sens T_SENS, 
		@vl_Gare T_NOM, @vl_Debut datetime, @vl_Fin datetime, 
		@vl_DebutBalisage datetime, @vl_FinBalisage datetime, 
		@vl_Balisage bit, @vl_NomInterlocuteur T_NOM, 
		@vl_Intervention varchar(25), @vl_Type tinyint, 
		@vl_Scenario char(4), @vl_NomScenario T_NOM, 
		@vl_Sequence tinyint, @vl_Depanneur T_NOM, 
		@vl_Longueur int,  @vl_Resultat char(25), @vl_Pictogramme char(XDC_LGR_NOM_PICTO),
		@vl_NomSequence T_NOM, @vl_Date char(10), @vl_Heure char(8),
		@vl_Action char(9), @vl_ChaineFixe char(20), 
		@vl_ChaineAction varchar(20), @vl_CaractDebut int,
		@vl_Numero varchar(6), @vl_PRDebBalisage T_PR,
		@vl_PRFinBalisage T_PR, @vl_localisation char(60),
		@vl_LibMod1 char(30), @vl_LibMod2 char(30), @vl_LibMod3 char(30),
		@vl_LibMod4 char(30), @vl_LibMod5 char(30),
		@vl_Annulation tinyint,
		@vl_texte1 char(255),@vl_Ligne1 char(18),@vl_Ligne2 char(18),@vl_Ligne3 char(18),@vl_Ligne4 char(18),
		@vl_Gravite tinyint,@vl_Trafic tinyint,@vl_index tinyint, @vl_autoroute tinyint,
		@vl_Scenario_PAL char(6), @vl_NomScenario_PAL char(50), @vl_numTypeFmc int,
		@vl_Scenario_BAD char(10),
		@vl_Mod1 tinyint, @vl_Mod2 tinyint, @vl_Mod3 tinyint, @vl_Mod4 tinyint,@vl_Mod6 tinyint, @vl_Mod5 tinyint

	/*A Controle des parametres d'entree obligatoire */
	if @va_NumAct_in = null or @va_CleAct_in = null or 
	   @va_Type_in = null or @va_Heure_in = null or @va_TypeHeure_in = null
		return XDC_ARG_INV

	/* conversion du numero d'action en chaine de caracteres */
	select @vl_ChaineAction = convert(char(20), @va_NumAct_in)

	/* calcul la position du premier caractere a prendre */
	select @vl_Longueur = char_length(@vl_ChaineAction), 
		@vl_ChaineFixe = @vl_ChaineAction
	if @vl_Longueur < XZAEC_NB_DIGITS_LIB_ACT
	begin
		select @vl_CaractDebut = 1
		select @vl_Numero = substring(@vl_ChaineFixe, @vl_CaractDebut, @vl_Longueur)
	end
	else    
	begin
		select @vl_CaractDebut = @vl_Longueur - (XZAEC_NB_DIGITS_LIB_ACT - 1)
		select @vl_Numero = substring(@vl_ChaineFixe, @vl_CaractDebut, XZAEC_NB_DIGITS_LIB_ACT)
	end

	/*A formatage de la date et du numero d'action */
	select	@vl_Date = convert(char(10), @va_Heure_in, 103),
		@vl_Heure = convert(char(8), @va_Heure_in, 8),
		@vl_Action = @vl_Numero + '-' + convert(char(2), @va_CleAct_in) + ' - ' + convert(char(2), @va_SiteOrigine_in)

        select @vl_numTypeFmc = type from EXP..FMC_GEN where numero=@va_NumEvt_in and cle=@va_CleEvt_in
	if @vl_numTypeFmc = XZAEC_FMC_QueueBouchon
		select @va_TypeFmc = XZAEC_SYN_FMC_QueueBouchon
	if @vl_numTypeFmc = XZAEC_FMC_TeteBouchon
		select @va_TypeFmc = XZAEC_SYN_FMC_TeteBouchon

	/* Mise en forme de la localisation de l'evenement */
	exec XZAE80	null, null, null, null,
			@va_Autoroute_in, @va_PR_in, @va_Sens_in,
			@vl_localisation output

	/*B Ajout d'une ligne "ACTION" datee mentionnant le numero et la localisation de l'evenement */
	select @vl_Ligne = @vl_Date + ' ' + @vl_Heure + ' N' +
				convert(char(6), @va_NumEvt_in) + '- ' + convert(char(2), @va_CleEvt_in) +
				' ' +  convert(varchar(60),@va_TypeFmc) + " - ACTION " + ' ' + @vl_localisation
	insert EXP..FMC_LIS(numero,cle,ligne,heure,poste)
	values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)


	/*A Creation de l'intitule de l'action dans la synthese */
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
	   @va_Type_in = XDC_ACT_BIP
	begin
		/*B recherche de l'interlocuteur pour un appel */
		select
			@vl_Interlocuteur = interlocuteur,
			@vl_TypeAstreinte = type_d_astreinte
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
			if @va_Type_in = XDC_ACT_FAX
				select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + XZAEC_SYN_ACT_FAX
			else if @va_Type_in = XDC_ACT_TEL
				select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + XZAEC_SYN_ACT_TEL
			else if @va_Type_in = XDC_ACT_BIP
				select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + XZAEC_SYN_ACT_BIP
			else return XDC_NOK
		end
		else
		begin
			if @va_Type_in = XDC_ACT_FAX
				select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + XZAEC_SYN_ACT_FAX_AST + @vl_NomTypeAstreinte
			else if @va_Type_in = XDC_ACT_TEL
				select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + XZAEC_SYN_ACT_TEL_AST + @vl_NomTypeAstreinte
			else if @va_Type_in = XDC_ACT_BIP
				select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + XZAEC_SYN_ACT_BIP_AST + @vl_NomTypeAstreinte
			else return XDC_NOK
		end

		/*B Ajout de l'intitule de l'action de type appel */
		insert EXP..FMC_LIS(numero,cle,ligne,heure,poste)
		values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)

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
			insert EXP..FMC_LIS(numero,cle,ligne,heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end
		else if (@va_TypeHeure_in = XZAEC_ACT_SUC)
		begin
			select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + XZAEC_SYN_ACT_REUSSIE
			insert EXP..FMC_LIS(numero,cle,ligne,heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end

	end

	else if @va_Type_in = XDC_ACT_CCH
	begin
		/*B Ajout de l'intitule de l'action de type CCH */
		select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + XZAEC_SYN_ACT_CCH
		insert EXP..FMC_LIS(numero,cle,ligne,heure,poste)
		values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)

		/*B resultat de l'action */
		if (@va_TypeHeure_in = XZAEC_ACT_ECHEC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_ECHEC
			insert EXP..FMC_LIS(numero,cle,ligne,heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end
		if (@va_TypeHeure_in = XZAEC_ACT_SUC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_SUC
			insert EXP..FMC_LIS(numero,cle,ligne,heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
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
		insert EXP..FMC_LIS(numero,cle,ligne,heure,poste)
		values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)

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
		insert EXP..FMC_LIS(numero,cle,ligne,heure,poste)
		values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)

		/*B resultat de l'action */
		if (@va_TypeHeure_in = XZAEC_ACT_ECHEC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_ECHEC
			insert EXP..FMC_LIS(numero,cle,ligne,heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end
		if (@va_TypeHeure_in = XZAEC_ACT_SUC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_SUC
			insert EXP..FMC_LIS(numero,cle,ligne,heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end

	end

        else if @va_Type_in =  XDC_ACT_BAF
        begin
                select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + XZAEC_SYN_ACT_BAF

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

                /* Ajout de l'intitule de l'action de type BAF */
                insert EXP..FMC_LIS(numero,cle,ligne,heure,poste)
                values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)

                /* recherche de l'etat des modules feux et barrieres de la BAF */
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
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_ACT_BAF_GLOBAL_DROIT
			insert EXP..FMC_LIS(numero,cle,ligne,heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end
		else if (@vl_Mod1!=0) and (@vl_Mod2!=0)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_ACT_BAF_FEUX_DROITS
			insert EXP..FMC_LIS(numero,cle,ligne,heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end
		else 
		begin
                	if (@vl_Mod1!=0)
                	begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_ACT_BAF_MOD1
				insert EXP..FMC_LIS(numero,cle,ligne,heure,poste)
				values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
                	end

                	if (@vl_Mod2!=0)
                	begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_ACT_BAF_MOD2
				insert EXP..FMC_LIS(numero,cle,ligne,heure,poste)
				values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
                	end

                	if (@vl_Mod3!=0)
                	begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_ACT_BAF_MOD3
				insert EXP..FMC_LIS(numero,cle,ligne,heure,poste)
				values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
                	end
		end


		if (@vl_Mod4!=0) and (@vl_Mod5!=0) and (@vl_Mod6!=0) 
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_ACT_BAF_GLOBAL_GAUCHE
			insert EXP..FMC_LIS(numero,cle,ligne,heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end
		else if (@vl_Mod4!=0) and (@vl_Mod5!=0)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_ACT_BAF_FEUX_GAUCHES
			insert EXP..FMC_LIS(numero,cle,ligne,heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end
		else 
		begin
                	if (@vl_Mod4!=0)
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_ACT_BAF_MOD4
				insert EXP..FMC_LIS(numero,cle,ligne,heure,poste)
				values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
                	end

                	if (@vl_Mod5!=0)
                	begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_ACT_BAF_MOD5
				insert EXP..FMC_LIS(numero,cle,ligne,heure,poste)
				values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
                	end

                	if (@vl_Mod6!=0)
                	begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_ACT_BAF_MOD6
				insert EXP..FMC_LIS(numero,cle,ligne,heure,poste)
				values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
                	end
		end

                /* resultat de l'action */
                if (@va_TypeHeure_in = XZAEC_ACT_ECHEC)
                begin
                        select @vl_Ligne = XZAEC_SYN_PAS_DATE +
                                           XZAEC_SYN_ACT_RESULT_ECHEC
			insert EXP..FMC_LIS(numero,cle,ligne,heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
                end
                if (@va_TypeHeure_in = XZAEC_ACT_SUC)
                begin
                        select @vl_Ligne = XZAEC_SYN_PAS_DATE +
                                           XZAEC_SYN_ACT_RESULT_SUC
			insert EXP..FMC_LIS(numero,cle,ligne,heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
                end

        end

        else if @va_Type_in = XDC_ACT_BAD
        begin
		select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + XZAEC_SYN_ACT_BAD

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

		/*B Ajout de l'intitule de l'action de type BAD */
                insert EXP..FMC_LIS(numero,cle,ligne,heure,poste)
                values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)

		/*B recherche du scenario de la BAD */
                select
                        @vl_Scenario_BAD = scenario
                from EXP..ACT_BAD
                where actio = @va_NumAct_in and sit = @va_CleAct_in

                if @@rowcount != 1
                        return XDC_NOK

                if @vl_Scenario_BAD != XDC_CHAINE_VIDE
                begin
/*                        select @vl_NomScenario = nom
                        from CFG..EQT_SCN_NAV
                        where scenario = @vl_Scenario_BAD*/

			select @vl_NomScenario = @vl_Scenario_BAD

                        if @@rowcount != 1
                                select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_ACT_SCN + @vl_Scenario_BAD
                        else    select @vl_Ligne = XZAEC_SYN_PAS_DATE +
                                           XZAEC_SYN_ACT_SCN + @vl_NomScenario
                end

                insert EXP..FMC_LIS(numero,cle,ligne,heure,poste)
                values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)

		/*B resultat de l'action */
                if (@va_TypeHeure_in = XZAEC_ACT_ECHEC)
                begin
                        select @vl_Ligne = XZAEC_SYN_PAS_DATE +
                                           XZAEC_SYN_ACT_RESULT_ECHEC
                        insert EXP..FMC_LIS(numero,cle,ligne,heure,poste)
                        values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
                end
                if (@va_TypeHeure_in = XZAEC_ACT_SUC)
                begin
                        select @vl_Ligne = XZAEC_SYN_PAS_DATE +
                                           XZAEC_SYN_ACT_RESULT_SUC
                        insert EXP..FMC_LIS(numero,cle,ligne,heure,poste)
                        values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
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
		insert EXP..FMC_LIS(numero,cle,ligne,heure,poste)
		values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)

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

		insert EXP..FMC_LIS(numero,cle,ligne,heure,poste)
		values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)

		/*B resultat de l'action */
		if (@va_TypeHeure_in = XZAEC_ACT_ECHEC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_ECHEC
			insert EXP..FMC_LIS(numero,cle,ligne,heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end
		if (@va_TypeHeure_in = XZAEC_ACT_SUC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_SUC
			insert EXP..FMC_LIS(numero,cle,ligne,heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
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
		insert EXP..FMC_LIS(numero,cle,ligne,heure,poste)
		values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)

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
		insert EXP..FMC_LIS(numero,cle,ligne,heure,poste)
		values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)

		insert EXP..FMC_LIS(numero,cle,ligne,heure,picto,poste)
		values (@va_NumEvt_in, @va_CleEvt_in,@vl_LignePicto, null,@vl_Pictogramme,@va_NumPoste_in)

		/*B ajout du texte du bandeau du PMV  dans la synthese si non null*/
		if (@vl_Bandeau!=null) and (@vl_Bandeau!=XDC_CHAINE_VIDE)
		begin   
			insert EXP..FMC_LIS(numero,cle,ligne,heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,
				XZAEC_SYN_PAS_DATE + XZAEC_SYN_ACT_PMV_BAND + @vl_Bandeau,
				null,@va_NumPoste_in)
		end

		/*B ajout du texte de 4eme ligne du PMV  dans la synthese si non null*/
		if (@vl_DirectLigne!=null) and (@vl_DirectLigne!=XDC_CHAINE_VIDE)
		begin   
			insert EXP..FMC_LIS(numero,cle,ligne,heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,
				XZAEC_SYN_PAS_DATE + XZAEC_SYN_ACT_PMV_TXT_4 + @vl_DirectLigne, 
				null,@va_NumPoste_in)
		end


		/*B resultat de l'action */
		if (@va_TypeHeure_in = XZAEC_ACT_ECHEC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_ECHEC
			insert EXP..FMC_LIS(numero,cle,ligne,heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end
		if (@va_TypeHeure_in = XZAEC_ACT_SUC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_SUC
			insert EXP..FMC_LIS(numero,cle,ligne,heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end

	end

	else if @va_Type_in = XDC_ACT_PMVA
	begin
		select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + XZAEC_SYN_ACT_PMVA

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

		/*B Ajout de l'intitule de l'action de type PMVA */
		insert EXP..FMC_LIS(numero,cle,ligne,heure,poste)
		values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)

		/*B recherche du texte du PMVA */
		select
			@vl_Ligne = XZAEC_SYN_PAS_DATE +
			            XZAEC_SYN_ACT_PMVA_TXT +
			            premiere_ligne + ' | ' + deuxieme_ligne +
			            ' | ' + troisieme_ligne
		from EXP..ACT_PMA
		where actio = @va_NumAct_in and sit = @va_CleAct_in

		if @@rowcount != 1
			return XDC_NOK

		/*B ajout du texte du PMVA dans la synthese */
		insert EXP..FMC_LIS(numero,cle,ligne,heure,poste)
		values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)

		/*B recherche du texte des 4eme et 5eme lignes du PMVA */
		select
			@vl_Ligne = XZAEC_SYN_PAS_DATE +
			            XZAEC_SYN_ACT_PMVA_TXT +
			            quatrieme_ligne + ' | ' + cinquieme_ligne
		from EXP..ACT_PMA
		where actio = @va_NumAct_in and sit = @va_CleAct_in

		if @@rowcount != 1
			return XDC_NOK

		insert EXP..FMC_LIS(numero,cle,ligne,heure,poste)
		values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)

		/*B resultat de l'action */
		if (@va_TypeHeure_in = XZAEC_ACT_ECHEC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
			                   XZAEC_SYN_ACT_RESULT_ECHEC
			insert EXP..FMC_LIS(numero,cle,ligne,heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
		end
		if (@va_TypeHeure_in = XZAEC_ACT_SUC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
			                   XZAEC_SYN_ACT_RESULT_SUC
			insert EXP..FMC_LIS(numero,cle,ligne,heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
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
		insert EXP..FMC_LIS(numero,cle,ligne,heure,poste)
		values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)

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

		insert EXP..FMC_LIS(numero,cle,ligne,heure,poste)
		values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)

		insert EXP..FMC_LIS(numero,cle,ligne,heure,picto,poste)
		values(@va_NumEvt_in, @va_CleEvt_in,@vl_LignePicto, null,@vl_Pictogramme,@va_NumPoste_in)

		/*B ajout du texte du bandeau du PMV  dans la synthese si non null*/
		if (@vl_Bandeau!=null) and (@vl_Bandeau!=XDC_CHAINE_VIDE)
		begin   
			insert EXP..FMC_LIS(numero,cle,ligne,heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,
				XZAEC_SYN_PAS_DATE + XZAEC_SYN_ACT_PMV_BAND + @vl_Bandeau, 
				null,@va_NumPoste_in)
		end

		/*B ajout du texte de 4eme ligne du PMV  dans la synthese si non null*/
		if (@vl_DirectLigne!=null) and (@vl_DirectLigne!=XDC_CHAINE_VIDE)
		begin   
			insert EXP..FMC_LIS(numero,cle,ligne,heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,
				XZAEC_SYN_PAS_DATE + XZAEC_SYN_ACT_PMV_TXT_4 + @vl_DirectLigne,
				null,@va_NumPoste_in)
		end


		/*B resultat de l'action */
		if (@va_TypeHeure_in = XZAEC_ACT_ECHEC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_ECHEC

			insert EXP..FMC_LIS(numero,cle,ligne,heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end
		if (@va_TypeHeure_in = XZAEC_ACT_SUC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_SUC
			insert EXP..FMC_LIS(numero,cle,ligne,heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
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

		insert EXP..FMC_LIS(numero,cle,ligne,heure,poste)
		values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)

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

				insert EXP..FMC_LIS(numero,cle,ligne,heure,poste)
				values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
			end
		end

		/*B resultat de l'action */
		if (@va_TypeHeure_in = XZAEC_ACT_ECHEC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_ECHEC
			insert EXP..FMC_LIS(numero,cle,ligne,heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end
		if (@va_TypeHeure_in = XZAEC_ACT_SUC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_SUC
			insert EXP..FMC_LIS(numero,cle,ligne,heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end

	end

	/* AAZ Ajout Domaine SIG, ECL, VEN, ENR et INC */

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

		insert EXP..FMC_LIS( numero,
		                     cle,
		                     ligne,
		                     heure,poste )
		            values ( @va_NumEvt_in,
		                     @va_CleEvt_in,
		                     @vl_Ligne, 
		                     null,
		                     @va_NumPoste_in)

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
			where  numero = @vl_Sequence
			and type = XDC_EQT_TUBE

			if @@rowcount = 1
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_SEQ + @vl_NomSequence

				insert EXP..FMC_LIS( numero,
				                     cle,
				                     ligne,
				                     heure,
				                     poste )
				            values ( @va_NumEvt_in,
				                     @va_CleEvt_in,
				                     @vl_Ligne,
				                     null,
				                     @va_NumPoste_in)
			end
		end

		/*B resultat de l'action */
		if (@va_TypeHeure_in = XZAEC_ACT_ECHEC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_ECHEC
			insert EXP..FMC_LIS( numero,
			                     cle,
			                     ligne,
			                     heure,
			                     poste)
			            values ( @va_NumEvt_in,
			                     @va_CleEvt_in,
			                     @vl_Ligne,
			                     null,
			                     @va_NumPoste_in)
		end
		if (@va_TypeHeure_in = XZAEC_ACT_SUC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_SUC
			insert EXP..FMC_LIS( numero,
			                     cle,
			                     ligne,
			                     heure,
			                     poste)
			            values ( @va_NumEvt_in,
			                     @va_CleEvt_in,
			                     @vl_Ligne,
			                     null,
			                     @va_NumPoste_in)
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
			where  numero = @va_Eqt_in  and
			       type   = XDC_EQT_ECH

			if @@rowcount != 1
				return XDC_NOK

			select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + @vl_NomEqt
		end

		insert EXP..FMC_LIS( numero,
		                     cle,
		                     ligne,
		                     heure,poste)
                     values( @va_NumEvt_in,
                             @va_CleEvt_in,
                             @vl_Ligne,
                             null,
                             @va_NumPoste_in) 

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
			where  numero = @vl_Sequence
			and type = XDC_EQT_ECH

			if @@rowcount = 1
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_SEQ + @vl_NomSequence

				insert EXP..FMC_LIS( numero,
				                     cle,
				                     ligne,
				                     heure,
				                     poste)
				             values( @va_NumEvt_in,
				                     @va_CleEvt_in,
				                     @vl_Ligne,
				                     null,
				                     @va_NumPoste_in)
			end
		end

		/*B resultat de l'action */
		if (@va_TypeHeure_in = XZAEC_ACT_ECHEC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_ECHEC
			insert EXP..FMC_LIS( numero,
			                     cle,
			                     ligne,
			                     heure,poste)
			             values( @va_NumEvt_in,
			                     @va_CleEvt_in,
			                     @vl_Ligne,
			                     null,
			                     @va_NumPoste_in)
		end
		if (@va_TypeHeure_in = XZAEC_ACT_SUC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_SUC
			insert EXP..FMC_LIS( numero,
			                     cle,
			                     ligne,
			                     heure,
			                     poste)
		  	             values( @va_NumEvt_in, 
		  	                     @va_CleEvt_in,
		  	                     @vl_Ligne,
		  	                     null,
		  	                     @va_NumPoste_in)	
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

		insert EXP..FMC_LIS( numero,
		                     cle,
		                     ligne,
		                     heure,poste )
		            values ( @va_NumEvt_in,
		                     @va_CleEvt_in,
		                     @vl_Ligne, 
		                     null,
		                     @va_NumPoste_in)

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
			where  numero = @vl_Sequence
			and type = XDC_EQT_TUBE

			if @@rowcount = 1
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_SEQ + @vl_NomSequence

				insert EXP..FMC_LIS( numero,
				                     cle,
				                     ligne,
				                     heure,
				                     poste )
				            values ( @va_NumEvt_in,
				                     @va_CleEvt_in,
				                     @vl_Ligne,
				                     null,
				                     @va_NumPoste_in)
			end
		end

		/*B resultat de l'action */
		if (@va_TypeHeure_in = XZAEC_ACT_ECHEC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_ECHEC
			insert EXP..FMC_LIS( numero,
			                     cle,
			                     ligne,
			                     heure,
			                     poste)
			            values ( @va_NumEvt_in,
			                     @va_CleEvt_in,
			                     @vl_Ligne,
			                     null,
			                     @va_NumPoste_in)
		end
		if (@va_TypeHeure_in = XZAEC_ACT_SUC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_SUC
			insert EXP..FMC_LIS( numero,
			                     cle,
			                     ligne,
			                     heure,
			                     poste)
			            values ( @va_NumEvt_in,
			                     @va_CleEvt_in,
			                     @vl_Ligne,
			                     null,
			                     @va_NumPoste_in)
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

		insert EXP..FMC_LIS( numero,
		                     cle,
		                     ligne,
		                     heure,poste )
		            values ( @va_NumEvt_in,
		                     @va_CleEvt_in,
		                     @vl_Ligne, 
		                     null,
		                     @va_NumPoste_in)

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
			where  numero = @vl_Sequence
			and type = XDC_EQT_TUBE

			if @@rowcount = 1
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_SEQ + @vl_NomSequence

				insert EXP..FMC_LIS( numero,
				                     cle,
				                     ligne,
				                     heure,
				                     poste )
				            values ( @va_NumEvt_in,
				                     @va_CleEvt_in,
				                     @vl_Ligne,
				                     null,
				                     @va_NumPoste_in)
			end
		end

		/*B resultat de l'action */
		if (@va_TypeHeure_in = XZAEC_ACT_ECHEC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_ECHEC
			insert EXP..FMC_LIS( numero,
			                     cle,
			                     ligne,
			                     heure,
			                     poste)
			            values ( @va_NumEvt_in,
			                     @va_CleEvt_in,
			                     @vl_Ligne,
			                     null,
			                     @va_NumPoste_in)
		end
		if (@va_TypeHeure_in = XZAEC_ACT_SUC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_SUC
			insert EXP..FMC_LIS( numero,
			                     cle,
			                     ligne,
			                     heure,
			                     poste)
			            values ( @va_NumEvt_in,
			                     @va_CleEvt_in,
			                     @vl_Ligne,
			                     null,
			                     @va_NumPoste_in)
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

		insert EXP..FMC_LIS( numero,
		                     cle,
		                     ligne,
		                     heure,poste )
		            values ( @va_NumEvt_in,
		                     @va_CleEvt_in,
		                     @vl_Ligne, 
		                     null,
		                     @va_NumPoste_in)

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
			where  numero = @vl_Sequence
			and type = XDC_EQT_TUBE

			if @@rowcount = 1
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_SEQ + @vl_NomSequence

				insert EXP..FMC_LIS( numero,
				                     cle,
				                     ligne,
				                     heure,
				                     poste )
				            values ( @va_NumEvt_in,
				                     @va_CleEvt_in,
				                     @vl_Ligne,
				                     null,
				                     @va_NumPoste_in)
			end
		end

		/*B resultat de l'action */
		if (@va_TypeHeure_in = XZAEC_ACT_ECHEC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_ECHEC
			insert EXP..FMC_LIS( numero,
			                     cle,
			                     ligne,
			                     heure,
			                     poste)
			            values ( @va_NumEvt_in,
			                     @va_CleEvt_in,
			                     @vl_Ligne,
			                     null,
			                     @va_NumPoste_in)
		end
		if (@va_TypeHeure_in = XZAEC_ACT_SUC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_SUC
			insert EXP..FMC_LIS( numero,
			                     cle,
			                     ligne,
			                     heure,
			                     poste)
			            values ( @va_NumEvt_in,
			                     @va_CleEvt_in,
			                     @vl_Ligne,
			                     null,
			                     @va_NumPoste_in)
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

		insert EXP..FMC_LIS( numero,
		                     cle,
		                     ligne,
		                     heure,poste )
		            values ( @va_NumEvt_in,
		                     @va_CleEvt_in,
		                     @vl_Ligne, 
		                     null,
		                     @va_NumPoste_in)

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
			where  numero = @vl_Sequence
			and type = XDC_EQT_TUBE

			if @@rowcount = 1
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_SEQ + @vl_NomSequence

				insert EXP..FMC_LIS( numero,
				                     cle,
				                     ligne,
				                     heure,
				                     poste )
				            values ( @va_NumEvt_in,
				                     @va_CleEvt_in,
				                     @vl_Ligne,
				                     null,
				                     @va_NumPoste_in)
			end
		end

		/*B resultat de l'action */
		if (@va_TypeHeure_in = XZAEC_ACT_ECHEC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_ECHEC
			insert EXP..FMC_LIS( numero,
			                     cle,
			                     ligne,
			                     heure,
			                     poste)
			            values ( @va_NumEvt_in,
			                     @va_CleEvt_in,
			                     @vl_Ligne,
			                     null,
			                     @va_NumPoste_in)
		end
		if (@va_TypeHeure_in = XZAEC_ACT_SUC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_SUC
			insert EXP..FMC_LIS( numero,
			                     cle,
			                     ligne,
			                     heure,
			                     poste)
			            values ( @va_NumEvt_in,
			                     @va_CleEvt_in,
			                     @vl_Ligne,
			                     null,
			                     @va_NumPoste_in)
		end

	end

	else if @va_Type_in = XDC_ACT_GTC_EXP
	begin
		/*B Ajout de l'intitule de l'action de type EXPLOITATION */
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

		insert EXP..FMC_LIS( numero,
		                     cle,
		                     ligne,
		                     heure,poste )
		            values ( @va_NumEvt_in,
		                     @va_CleEvt_in,
		                     @vl_Ligne, 
		                     null,
		                     @va_NumPoste_in)

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
			where  numero = @vl_Sequence
			and type = XDC_EQT_TUBE

			if @@rowcount = 1
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_SEQ + @vl_NomSequence

				insert EXP..FMC_LIS( numero,
				                     cle,
				                     ligne,
				                     heure,
				                     poste )
				            values ( @va_NumEvt_in,
				                     @va_CleEvt_in,
				                     @vl_Ligne,
				                     null,
				                     @va_NumPoste_in)
			end
		end

		/*B resultat de l'action */
		if (@va_TypeHeure_in = XZAEC_ACT_ECHEC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_ECHEC
			insert EXP..FMC_LIS( numero,
			                     cle,
			                     ligne,
			                     heure,
			                     poste)
			            values ( @va_NumEvt_in,
			                     @va_CleEvt_in,
			                     @vl_Ligne,
			                     null,
			                     @va_NumPoste_in)
		end
		if (@va_TypeHeure_in = XZAEC_ACT_SUC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_SUC
			insert EXP..FMC_LIS( numero,
			                     cle,
			                     ligne,
			                     heure,
			                     poste)
			            values ( @va_NumEvt_in,
			                     @va_CleEvt_in,
			                     @vl_Ligne,
			                     null,
			                     @va_NumPoste_in)
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

		insert EXP..FMC_LIS(numero,cle,ligne,heure,poste)
		values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)

		/*B ajout du nom du depanneur dans la synthese */
		if @vl_Depanneur != XDC_CHAINE_VIDE
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_DEP_NOM +
					   @vl_Depanneur

			insert EXP..FMC_LIS(numero,cle,ligne,heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end
		if @va_TypeHeure_in = XZAEC_ACT_FIN and @vl_Resultat is not null and
		   @vl_Resultat != XDC_CHAINE_VIDE
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULTAT +
					   @vl_Resultat

			insert EXP..FMC_LIS(numero,cle,ligne,heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end

	end

	else if @va_Type_in = XDC_ACT_AFF_GARE
	begin
		/* ajout de l'intitule de l'action de type affichage */
		select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + XZAEC_SYN_ACT_GAR

		insert EXP..FMC_LIS(numero,cle,ligne,heure,poste)
		values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)

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

		insert EXP..FMC_LIS(numero,cle,ligne,heure,poste)
		values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)

		/*B ajout du texte de l'affichette dans la synthese */
		if @vl_Affichette != XDC_CHAINE_VIDE
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_GAR_TXT +
					   substring(@vl_Affichette, 1, 50),
				@vl_Longueur = char_length(@vl_Affichette)

			insert EXP..FMC_LIS(numero,cle,ligne,heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)

			if @vl_Longueur > 50
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   	substring(@vl_Affichette, 51, 50)

				insert EXP..FMC_LIS(numero,cle,ligne,heure,poste)
				values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
			end

			if @vl_Longueur > 100
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   	substring(@vl_Affichette, 101, 50)

				insert EXP..FMC_LIS(numero,cle,ligne,heure,poste)
				values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
			end

			if @vl_Longueur > 150
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   	substring(@vl_Affichette, 151, 50)

				insert EXP..FMC_LIS(numero,cle,ligne,heure,poste)
				values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
			end

			if @vl_Longueur > 200
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   	substring(@vl_Affichette, 201, 50)

				insert EXP..FMC_LIS(numero,cle,ligne,heure,poste)
				values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
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

		insert EXP..FMC_LIS(numero,cle,ligne,heure,poste)
		values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)

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

				insert EXP..FMC_LIS(numero,cle,ligne,heure,poste)
				values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
			end
			else
			begin
			/* texte correspondant a la pose du balisage */
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   convert(char(10), @vl_DebutBalisage, 103)+
					   ' ' + convert(char(8), @vl_DebutBalisage, 8)+
					   ' : ' + @vl_Intervention + ' ' +
					    XZAEC_SYN_ACT_BAL_POS

				insert EXP..FMC_LIS(numero,cle,ligne,heure,poste)
				values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
			end


			/* Localisation du balisage */
			if @vl_PRFinBalisage is not null
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_SIS_DEB_BAL +
					   convert(char(7),@vl_PRFinBalisage/1000.0)

				insert EXP..FMC_LIS(numero,cle,ligne,heure,poste)
				values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
			end
			if @vl_PRDebBalisage is not null
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_SIS_FIN_BAL +
					   convert(char(7),@vl_PRDebBalisage/1000.0)

				insert EXP..FMC_LIS(numero,cle,ligne,heure,poste)
				values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
			end

		end
		else if @vl_Intervention != XDC_CHAINE_VIDE
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_SIS_NOM +
					   @vl_Intervention

			insert EXP..FMC_LIS(numero,cle,ligne,heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
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
		insert EXP..FMC_LIS(numero,cle,ligne,heure,poste)
		values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)

		if @vl_Intervention != XDC_CHAINE_VIDE
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_INT_NOM +
					   @vl_Intervention

			insert EXP..FMC_LIS(numero,cle,ligne,heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end
		end
	end

	else if @va_Type_in = XDC_ACT_PMV_REMORQUE
	begin
		/*B Ajout de l'intitule de l'action de type remorque */
		select @vl_Ligne = rtrim(@vl_Ligne)+ ' ' + XZAEC_SYN_ACT_RMQ
		insert EXP..FMC_LIS(numero,cle,ligne,heure,poste)
		values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)

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

			insert EXP..FMC_LIS(numero,cle,ligne,heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end

		/*B ajout du message dans la synthese */
		if @vl_Texte != XDC_CHAINE_VIDE
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RMQ_TXT +
					   substring(@vl_Texte, 1, 48)

			insert EXP..FMC_LIS(numero,cle,ligne,heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end

		/*B resultat de l'action  dans la synthese */
		if (@va_TypeHeure_in = XZAEC_ACT_ECHEC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_ECHEC
			insert EXP..FMC_LIS(numero,cle,ligne,heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end
		if (@va_TypeHeure_in = XZAEC_ACT_SUC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_SUC
			insert EXP..FMC_LIS(numero,cle,ligne,heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end

	end

	else if @va_Type_in = XDC_ACT_PMV_FOURGON
	begin
		/*B Ajout de l'intitule de l'action de type PMV sur fourgon */
		select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + XZAEC_SYN_ACT_FRG
		insert EXP..FMC_LIS(numero,cle,ligne,heure,poste)
		values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)

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

			insert EXP..FMC_LIS(numero,cle,ligne,heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end

		/*B ajout du message dans la synthese */
		if @vl_Texte != XDC_CHAINE_VIDE
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_FRG_TXT +
					   substring(@vl_Texte, 1, 20)

			insert EXP..FMC_LIS(numero,cle,ligne,heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end

		/*B resultat de l'action */
		if (@va_TypeHeure_in = XZAEC_ACT_ECHEC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_ECHEC
			insert EXP..FMC_LIS(numero,cle,ligne,heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end
		if (@va_TypeHeure_in = XZAEC_ACT_SUC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_SUC
			insert EXP..FMC_LIS(numero,cle,ligne,heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end

	end

	else if @va_Type_in = XDC_ACT_TFM
	begin
		/*B Ajout de l'intitule de l'action de type TFM */
		select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + XZAEC_SYN_ACT_TFM
		insert EXP..FMC_LIS(numero,cle,ligne,heure,poste)
		values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)

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

		insert EXP..FMC_LIS(numero,cle,ligne,heure,poste)
		values( @va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)

		select @vl_Fin = heure_fin 
		from EXP..ACT_GEN
		where numero = @va_NumAct_in and sit = @va_CleAct_in
			
		if @vl_Fin is not null
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
				   XZAEC_SYN_ACT_TFM_ARRET +
				   convert(char(10), @vl_Fin, 103)+' '+convert(char(8), @vl_Fin, 8)

			insert EXP..FMC_LIS(numero,cle,ligne,heure,poste)
			values( @va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end
	end

	else if @va_Type_in = XDC_ACT_EXPTFM
	begin
		/*B Ajout de l'intitule de l'action de type TFM */
		select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + XZAEC_SYN_ACT_EXPTFM
		insert EXP..FMC_LIS(numero,cle,ligne,heure,poste)
		values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)

		select @vl_Fin = heure_fin 
		from EXP..ACT_GEN
		where numero = @va_NumAct_in and sit = @va_CleAct_in
	end

	else if @va_Type_in = XDC_ACT_EXP
	begin
		/*B Ajout de l'intitule de l'action de type TFM */
		select @vl_Ligne = rtrim(@vl_Ligne) + ' ' + XZAEC_SYN_ACT_EXP
		insert EXP..FMC_LIS(numero,cle,ligne,heure,poste)
		values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)

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
				insert EXP..FMC_LIS(numero,cle,ligne,heure,poste)
				values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
			end
			else if @vl_Gravite=1
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_GRAVITE + XZAEC_SYN_GRAVITE1
				insert EXP..FMC_LIS(numero,cle,ligne,heure,poste)
				values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
			end
			else if @vl_Gravite=2
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_GRAVITE + XZAEC_SYN_GRAVITE2
				insert EXP..FMC_LIS(numero,cle,ligne,heure,poste)
				values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
			end
			else if @vl_Gravite=3
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_GRAVITE + XZAEC_SYN_GRAVITE3
				insert EXP..FMC_LIS(numero,cle,ligne,heure,poste)
				values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
			end
		
			if @vl_Trafic=XDC_RADT_FLUIDE
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_TRAFIC + XZAEC_SYN_TRAFIC_FLUIDE
				insert EXP..FMC_LIS(numero,cle,ligne,heure,poste)
				values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
			end
			else if @vl_Trafic=XDC_RADT_PRESAT
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_TRAFIC + XZAEC_SYN_TRAFIC_PRESAT
				insert EXP..FMC_LIS(numero,cle,ligne,heure,poste)
				values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
			end
		 	if @vl_Trafic=XDC_RADT_SATURE
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_TRAFIC + XZAEC_SYN_TRAFIC_SATURE
				insert EXP..FMC_LIS(numero,cle,ligne,heure,poste)
				values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
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

		insert EXP..FMC_LIS(numero,cle,ligne,heure,poste)
		values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)

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

				insert EXP..FMC_LIS(numero,cle,ligne,heure,poste)
				values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
			end
		end


		/*B resultat de l'action */
		if (@va_TypeHeure_in = XZAEC_ACT_ECHEC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_ECHEC
			insert EXP..FMC_LIS(numero,cle,ligne,heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end
		if (@va_TypeHeure_in = XZAEC_ACT_SUC)
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_ACT_RESULT_SUC
			insert EXP..FMC_LIS(numero,cle,ligne,heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end
	end
	else return XDC_NOK

	return XDC_OK
#endif

go
