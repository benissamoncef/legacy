/*E*/
/* Fichier : $Id: xzae205sp.prc,v 1.9 2019/10/01 19:28:15 devgfi Exp $        $Revision: 1.9 $        $Date: 2019/10/01 19:28:15 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae205p.prc
------------------------------------------------------
* DESCRIPTION DE LA PROCEDURE :
*
* retourne la synthÉse d'un evt ou operation
*
------------------------------------------------------
* HISTORIQUE :
*
* Hilmarcher 10/03/05:  Creation proc = xzae81 mais avec appel 
* 			a FMC_LIS a la place de #SYNTHESE
* CHI   25/04/05: modif pour ajout grele sur la chaussee DEM456 v1.2
* CHI   20/07/05        : ajout classe nature de l'obstacle DEM487 v1.3
* JPL	18/04/07 : Ajout d'un argument aux appels XZAE80 (DEM 638) 1.4
* JPL	15/05/07 : Prise en compte FMCs Traitement chaussees et Chaussee glissante (DEM 644,646) 1.5
* JPL	07/06/07 : Pas de visibilite pour Traitement chaussees et Chaussee glissante (DEM 644,646) 1.6
* JPL	03/09/10 : Prise en compte Fmc Basculemement pour Travaux (DEM 647) 1.7
* JPL	03/09/10 : Prise en compte Fmc Zone de stockage (DEM 942) 1.8
* JPL	01/10/19 : Basculement pour travaux : indiquer la présence d'un bouchon mobile (DEM 1346)  1.9
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Ajouter le complement d'une fiche courante dans 
* la synthese dans le cas d'evenement historisant
* son complement
* 
* Sequence d'appel
* SP	XZAE205
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
* XDY_Octet     va_NumPoste_in
* 
* Arguments en sortie
* 
* Code retour
* XDC_OK	
* XDC_NOK	evenement pas trouve
* < 0          : erreur sybase
* 
* Conditions d'utilisation
* Cette procedure est utilisÅe par la procedure XZAE204
* Elle est interne a la base de donnÅe.
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

if exists (select * from sysobjects where name = 'XZAE205' and type = 'P')
	drop procedure XZAE205
go


#ifdef HIST
	/*! CREATION DE LA PROCEDURE AU CI */

create procedure XZAE205
	@va_NumEvt_in		int		= null,
	@va_CleEvt_in		tinyint		= null,
	@va_Classe_in		tinyint		= null,
	@va_Type_in		smallint	= null,
	@va_HoroValidation_in	datetime	= null,
	@va_Autoroute_in	T_AUTOROUTE	= null,
	@va_PR_in		T_PR		= null,
	@va_Sens_in		T_SENS		= null,
	@va_NumPoste_in		smallint	= null
as
	declare @vl_Status int, @vl_PRTete T_PR, @vl_PRFin T_PR,
		@vl_Visibilite smallint, @vl_TypePerturb tinyint,
		@vl_nature_obstacle char(100),
		@vl_LongVeh smallint, @vl_Vitesse tinyint,
		@vl_Contenu char(25), @vl_Hauteur smallint,
		@vl_Largeur smallint, @vl_Poids smallint,
		@vl_TypeBouchon tinyint, @vl_Longueur int,
		@vl_NumEvtExtremite int, @vl_CleEvtExtremite tinyint,
		@vl_VoiesRet bit, @vl_SensTete T_SENS, @vl_SensQueue T_SENS,
		@vl_Ligne char(92), @vl_localisation char(60),
		@vl_PresenceBouchon tinyint

	if @va_Classe_in = XZAEC_ClasseMeteo
	begin
		/*A recherche des informations sur la meteo */
		select
			@vl_PRFin = PR,
			@vl_Visibilite = visibilite,
			@vl_TypePerturb = type_de_perturbation
		from HIS..FMC_MET
		where   numero = @va_NumEvt_in and cle = @va_CleEvt_in and
			horodate = @va_HoroValidation_in

		/*A Ajout des informations dans la synthese */
		if @vl_TypePerturb != null
		begin
			if @vl_TypePerturb = XZAEC_MET_ChuteNeige
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
						   XZAEC_SYN_CHUT_NEI
			else if @vl_TypePerturb = XZAEC_MET_NeigeChaus
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_NEI_CHA
			else if @vl_TypePerturb = XZAEC_MET_Pluie
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_PLU
			else if @vl_TypePerturb = XZAEC_MET_VentFort
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_VEN
			else if @vl_TypePerturb = XZAEC_MET_Brouillard
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_BROUIL
			else if @vl_TypePerturb = XZAEC_MET_BrouillardGiv
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_BRO_GIV
			else if @vl_TypePerturb = XZAEC_MET_Grele
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_GRELE 
			else if @vl_TypePerturb =  XZAEC_MET_GreleChaus
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_GREL_CH
			else if @vl_TypePerturb = XZAEC_MET_Verglas
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_VERG
			else if @vl_TypePerturb = XZAEC_MET_Autre
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_AUT
			/* cas d'un evenement Chaussee Glissante */
			else if @vl_TypePerturb = XZAEC_CH_GLIS_Neige
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_NEI_CHA
			else if @vl_TypePerturb = XZAEC_CH_GLIS_Verglas
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_VERG
			else if @vl_TypePerturb = XZAEC_CH_GLIS_Autre
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_AUT
			else if @vl_TypePerturb = XZAEC_CH_GLIS_Grele
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_GREL_CH
			else if @vl_TypePerturb = XZAEC_CH_GLIS_Huile
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_CHS_HUILE
			else if @vl_TypePerturb = XZAEC_CH_GLIS_Carburant
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_CHS_CARBURANT
			else if @vl_TypePerturb = XZAEC_CH_GLIS_Gasoil
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_CHS_GASOIL
			/* cas d'un evenement Traitement Chaussees */
			else if @vl_TypePerturb = XZAEC_TRT_CH_Salage
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_TRT_SALAGE
			else if @vl_TypePerturb = XZAEC_TRT_CH_Deneigement
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_TRT_DENEIG
			else return XDC_NOK
			
			insert HIS..FMC_LIS (numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in,@va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end

		if @vl_PRFin != null
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					XZAEC_SYN_FIN_MET + 
					convert(char(7), @vl_PRFin/1000.0) 
			
			insert HIS..FMC_LIS (numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in,@va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end

		if @va_Type_in = XZAEC_FMC_Meteo
		begin
			if @vl_Visibilite != null
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						XZAEC_SYN_VIS_MET + 
						convert(varchar(8), @vl_Visibilite/1000.0)
				insert HIS..FMC_LIS (numero,cle,ligne, heure,poste)
				values(@va_NumEvt_in,@va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
			end
		end
	end
        /*DEM487*/
        else if @va_Classe_in = XZAEC_ClasseNature
        begin
                /*A recherche des informations sur la nature de l'obstacle */
                select
                        @vl_nature_obstacle = nature
                from HIS..FMC_NAT
                where   numero = @va_NumEvt_in and cle = @va_CleEvt_in and
                        horodate = @va_HoroValidation_in

                select @vl_Ligne = XZAEC_SYN_PAS_DATE +
                                XZAEC_SYN_NATURE + @vl_nature_obstacle
                insert HIS..FMC_LIS (numero,cle,ligne, heure,poste)
                values(@va_NumEvt_in,@va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)

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
		from HIS..FMC_VLN
		where	numero = @va_NumEvt_in and cle = @va_CleEvt_in and
		horodate = @va_HoroValidation_in

		/*A Ajout des informations dans la synthese */
		if @vl_LongVeh != null
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					XZAEC_SYN_LON_VEH + 
					convert(varchar(8), @vl_LongVeh)
			insert HIS..FMC_LIS (numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in,@va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end

		if @vl_Vitesse != null
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					XZAEC_SYN_VIT_VEH + 
					convert(varchar(8), @vl_Vitesse)
			insert HIS..FMC_LIS (numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in,@va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end

		if @va_Type_in = XZAEC_FMC_ConvoiExc or @va_Type_in = XZAEC_FMC_ConvoiMil
		begin
			if @vl_Hauteur != null
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						XZAEC_SYN_HAU_VEH + 
						convert(varchar(8), @vl_Hauteur)
				insert HIS..FMC_LIS (numero,cle,ligne, heure,poste)
				values(@va_NumEvt_in,@va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
			end
	
			if @vl_Largeur != null
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						XZAEC_SYN_LRG_VEH + 
						convert(varchar(8), @vl_Largeur)
				insert HIS..FMC_LIS (numero,cle,ligne, heure,poste)
				values(@va_NumEvt_in,@va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
			end
	
			if @vl_Poids != null
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						XZAEC_SYN_POI_VEH + 
						convert(varchar(8), @vl_Poids)
				insert HIS..FMC_LIS (numero,cle,ligne, heure,poste)
				values(@va_NumEvt_in,@va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
			end

			if @vl_Contenu != null and @vl_Contenu != XDC_CHAINE_VIDE
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						XZAEC_SYN_CNT_VEH + 
						convert(varchar(25), @vl_Contenu)
				insert HIS..FMC_LIS (numero,cle,ligne, heure,poste)
				values(@va_NumEvt_in,@va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
			end
		end
	end
	else if @va_Classe_in in (XZAEC_ClasseTravaux, XZAEC_ClasseBasculTrav)
	begin
		/*A recherche les informations sur les travaux */
		select
			@vl_PRTete		= localisation_tete,
			@vl_VoiesRet		= voies_retrecies,
			@vl_PresenceBouchon	= bouchon
		from HIS..FMC_TRH
		where	HIS..FMC_TRH.numero = @va_NumEvt_in and 
			HIS..FMC_TRH.cle = @va_CleEvt_in  and
			HIS..FMC_TRH.horodate = @va_HoroValidation_in

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
			insert HIS..FMC_LIS (numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in,@va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end

		if @va_Type_in = XZAEC_FMC_ZoneStockage
		begin
			/*A ajout l'etat du stockage dans la synthese */
			if @vl_VoiesRet = XDC_OUI
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_STK_ENCOURS
			else
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_STK_PREPAR
			insert HIS..FMC_LIS (numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in,@va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end
		else
		if @vl_VoiesRet = XDC_OUI
		begin
			/*A ajout l'existance de voie retrecie dans la synthese */
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_VOI_RET 
			insert HIS..FMC_LIS (numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in,@va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end

		/* Pour un Basculement pour travaux, indiquer la présence d'un bouchon mobile */
		if @va_Type_in = XZAEC_FMC_BasculTravaux
		begin
			if @vl_PresenceBouchon = XDC_VRAI
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_BOU_MOBILE
				insert HIS..FMC_LIS (numero,cle,ligne,heure,poste)
				values (@va_NumEvt_in, @va_CleEvt_in, @vl_Ligne, null, @va_NumPoste_in)
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
			select @vl_PRFin = PR, @vl_SensQueue = sens from HIS..FMC_HIS
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
			select @vl_SensTete = sens from HIS..FMC_HIS
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

		/*A ajout de la localisation de la tete de bouchon */
		select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_BOU_TET + 
					@vl_localisation
                insert HIS..FMC_LIS (numero,cle,ligne, heure,poste)
                values(@va_NumEvt_in,@va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)

		/* mise en forme de la localisation de la queue de bouchon */
		exec XZAE80	null, null, null, null,
				@va_Autoroute_in, @vl_PRFin, @vl_SensQueue, 
				@vl_localisation output

		/*A ajout de la localisation de la queue de bouchon */
		select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_BOU_QUE + 
					@vl_localisation
 
                insert HIS..FMC_LIS (numero,cle,ligne, heure,poste)
                values(@va_NumEvt_in,@va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)


		/*A ajout de la longueur du bouchon dans la synthese */
		select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_BOU_LON +
					convert(varchar(7), @vl_Longueur/1000.0)

                insert HIS..FMC_LIS (numero,cle,ligne, heure,poste)
                values(@va_NumEvt_in,@va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)

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

                insert HIS..FMC_LIS (numero,cle,ligne, heure,poste)
                values(@va_NumEvt_in,@va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)

	end
	else if	(@va_Type_in = XZAEC_FMC_Escargot)
	begin
		/*A recherche les informations sur les travaux */
		select
			@vl_Longueur	= longueur
		from HIS..FMC_HIS
		where	numero = @va_NumEvt_in and 
			cle = @va_CleEvt_in  and
			horodate_validation = @va_HoroValidation_in

		/*A ajout de la localisation de la tete de bouchon */
		select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_LON_ESC + 
					convert(varchar(7), @vl_Longueur/1000.0)
                insert HIS..FMC_LIS (numero,cle,ligne, heure,poste)
                values(@va_NumEvt_in,@va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)

	end

	return XDC_OK

#else
	/*! CREATION DE LA PROCEDURE DANS UN DISTRICT */

create procedure XZAE205
	@va_NumEvt_in		int		= null,
	@va_CleEvt_in		tinyint		= null,
	@va_Classe_in		tinyint		= null,
	@va_Type_in		smallint	= null,
	@va_HoroValidation_in	datetime	= null,
	@va_Autoroute_in	T_AUTOROUTE	= null,
	@va_PR_in		T_PR		= null,
	@va_Sens_in		T_SENS		= null,
	@va_NumPoste_in		smallint	= null
as
	declare @vl_Status int, @vl_PRTete T_PR, @vl_PRFin T_PR,
		@vl_Visibilite smallint, @vl_TypePerturb tinyint,
		@vl_nature_obstacle char(100),
		@vl_LongVeh smallint, @vl_Vitesse tinyint,
		@vl_Contenu char(25), @vl_Hauteur smallint,
		@vl_Largeur smallint, @vl_Poids smallint,
		@vl_TypeBouchon tinyint, @vl_Longueur int,
		@vl_NumEvtExtremite int, @vl_CleEvtExtremite tinyint,
		@vl_VoiesRet bit, @vl_SensTete T_SENS, @vl_SensQueue T_SENS,
		@vl_Ligne char(92), @vl_localisation char(60),
		@vl_PresenceBouchon tinyint

	if @va_Classe_in = XZAEC_ClasseMeteo
	begin
		/*A recherche des informations sur la meteo */
		select
			@vl_PRFin = PR,
			@vl_Visibilite = visibilite,
			@vl_TypePerturb = type_de_perturbation
		from EXP..FMC_MET
		where   numero = @va_NumEvt_in and cle = @va_CleEvt_in and
			horodate = @va_HoroValidation_in

		/*A Ajout des informations dans la synthese */
		if @vl_TypePerturb != null
		begin
			if @vl_TypePerturb = XZAEC_MET_ChuteNeige
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
						   XZAEC_SYN_CHUT_NEI
			else if @vl_TypePerturb = XZAEC_MET_NeigeChaus
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_NEI_CHA
			else if @vl_TypePerturb = XZAEC_MET_Pluie
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_PLU
			else if @vl_TypePerturb = XZAEC_MET_VentFort
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_VEN
			else if @vl_TypePerturb = XZAEC_MET_Brouillard
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_BROUIL
			else if @vl_TypePerturb = XZAEC_MET_BrouillardGiv
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_BRO_GIV
			else if @vl_TypePerturb = XZAEC_MET_Grele
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_GRELE 
			else if @vl_TypePerturb =  XZAEC_MET_GreleChaus
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_GREL_CH
			else if @vl_TypePerturb = XZAEC_MET_Verglas
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_VERG
			else if @vl_TypePerturb = XZAEC_MET_Autre
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_AUT
			/* cas d'un evenement Chaussee Glissante */
			else if @vl_TypePerturb = XZAEC_CH_GLIS_Neige
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_NEI_CHA
			else if @vl_TypePerturb = XZAEC_CH_GLIS_Verglas
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_VERG
			else if @vl_TypePerturb = XZAEC_CH_GLIS_Autre
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_AUT
			else if @vl_TypePerturb = XZAEC_CH_GLIS_Grele
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_GREL_CH
			else if @vl_TypePerturb = XZAEC_CH_GLIS_Huile
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_CHS_HUILE
			else if @vl_TypePerturb = XZAEC_CH_GLIS_Carburant
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_CHS_CARBURANT
			else if @vl_TypePerturb = XZAEC_CH_GLIS_Gasoil
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_CHS_GASOIL
			/* cas d'un evenement Traitement Chaussees */
			else if @vl_TypePerturb = XZAEC_TRT_CH_Salage
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_TRT_SALAGE
			else if @vl_TypePerturb = XZAEC_TRT_CH_Deneigement
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_TRT_DENEIG
			else return XDC_NOK
				
                        insert EXP..FMC_LIS (numero,cle,ligne, heure,poste)
                        values(@va_NumEvt_in,@va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)

		end

		if @vl_PRFin != null
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					XZAEC_SYN_FIN_MET + 
					convert(char(7), @vl_PRFin/1000.0) 
			
                insert EXP..FMC_LIS (numero,cle,ligne, heure,poste)
                values(@va_NumEvt_in,@va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
	
		end

		if @va_Type_in = XZAEC_FMC_Meteo
		begin
			if @vl_Visibilite != null
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						XZAEC_SYN_VIS_MET + 
						convert(varchar(8), @vl_Visibilite/1000.0)
			insert EXP..FMC_LIS (numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in,@va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
			end
		end
	end
	/*DEM487*/
        else if @va_Classe_in = XZAEC_ClasseNature
        begin
                /*A recherche des informations sur la nature de l'obstacle */
                select
                        @vl_nature_obstacle = nature
                from EXP..FMC_NAT
                where   numero = @va_NumEvt_in and cle = @va_CleEvt_in and
                        horodate = @va_HoroValidation_in

                select @vl_Ligne = XZAEC_SYN_PAS_DATE +
                                XZAEC_SYN_NATURE + @vl_nature_obstacle
                insert EXP..FMC_LIS (numero,cle,ligne, heure,poste)
                values(@va_NumEvt_in,@va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
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
		from EXP..FMC_VLN
		where	numero = @va_NumEvt_in and cle = @va_CleEvt_in and
		horodate = @va_HoroValidation_in

		/*A Ajout des informations dans la synthese */
		if @vl_LongVeh != null
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					XZAEC_SYN_LON_VEH + 
					convert(varchar(8), @vl_LongVeh)
               insert EXP..FMC_LIS (numero,cle,ligne, heure,poste)
               values(@va_NumEvt_in,@va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
	
		end

		if @vl_Vitesse != null
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					XZAEC_SYN_VIT_VEH + 
					convert(varchar(8), @vl_Vitesse)
	        insert EXP..FMC_LIS (numero,cle,ligne, heure,poste)
                values(@va_NumEvt_in,@va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
	
		end

		if @va_Type_in = XZAEC_FMC_ConvoiExc or @va_Type_in = XZAEC_FMC_ConvoiMil
		begin
			if @vl_Hauteur != null
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						XZAEC_SYN_HAU_VEH + 
						convert(varchar(8), @vl_Hauteur)
	              		insert EXP..FMC_LIS (numero,cle,ligne, heure,poste)
                		values(@va_NumEvt_in,@va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
			
			end
	
			if @vl_Largeur != null
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						XZAEC_SYN_LRG_VEH + 
						convert(varchar(8), @vl_Largeur)
	                	insert EXP..FMC_LIS (numero,cle,ligne, heure,poste)
                		values(@va_NumEvt_in,@va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
			end
	
			if @vl_Poids != null
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						XZAEC_SYN_POI_VEH + 
						convert(varchar(8), @vl_Poids)
	                	insert EXP..FMC_LIS (numero,cle,ligne, heure,poste)
                		values(@va_NumEvt_in,@va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
			end
	
			if @vl_Contenu != null and @vl_Contenu != XDC_CHAINE_VIDE
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						XZAEC_SYN_CNT_VEH + 
						convert(varchar(25), @vl_Contenu)
	                	insert EXP..FMC_LIS (numero,cle,ligne, heure,poste)
                		values(@va_NumEvt_in,@va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
			end
		end
	end
	else if @va_Classe_in in (XZAEC_ClasseTravaux, XZAEC_ClasseBasculTrav)
	begin
		/*A recherche les informations sur les travaux */
		select
			@vl_PRTete		= localisation_tete,
			@vl_VoiesRet		= voies_retrecies,
			@vl_PresenceBouchon	= bouchon
		from EXP..FMC_TRH
		where	EXP..FMC_TRH.numero = @va_NumEvt_in and 
			EXP..FMC_TRH.cle = @va_CleEvt_in  and
			EXP..FMC_TRH.horodate = @va_HoroValidation_in

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
                	insert EXP..FMC_LIS (numero,cle,ligne, heure,poste)
                	values(@va_NumEvt_in,@va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end

		if @va_Type_in = XZAEC_FMC_ZoneStockage
		begin
			/*A ajout l'etat du stockage dans la synthese */
			if @vl_VoiesRet = XDC_OUI
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_STK_ENCOURS
			else
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_STK_PREPAR
			insert EXP..FMC_LIS (numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in,@va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end
		else
		if @vl_VoiesRet = XDC_OUI
		begin
			/*A ajout l'existance de voie retrecie dans la synthese */
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_VOI_RET 
	                insert EXP..FMC_LIS (numero,cle,ligne, heure,poste)
                	values(@va_NumEvt_in,@va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end

		/* Pour un Basculement pour travaux, indiquer la présence d'un bouchon mobile */
		if @va_Type_in = XZAEC_FMC_BasculTravaux
		begin
			if @vl_PresenceBouchon = XDC_VRAI
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_BOU_MOBILE
				insert EXP..FMC_LIS (numero,cle,ligne,heure,poste)
				values (@va_NumEvt_in, @va_CleEvt_in, @vl_Ligne, null, @va_NumPoste_in)
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
			select @vl_PRFin = PR, @vl_SensQueue = sens from EXP..FMC_HIS
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
			select @vl_SensTete = sens from EXP..FMC_HIS
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

		/*A ajout de la localisation de la tete de bouchon */
		select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_BOU_TET + 
					@vl_localisation
                insert EXP..FMC_LIS (numero,cle,ligne, heure,poste)
                values(@va_NumEvt_in,@va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)


		/* mise en forme de la localisation de la queue de bouchon */
		exec XZAE80	null, null, null, null,
				@va_Autoroute_in, @vl_PRFin, @vl_SensQueue, 
				@vl_localisation output

		/*A ajout de la localisation de la queue de bouchon */
		select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_BOU_QUE + 
					@vl_localisation
                insert EXP..FMC_LIS (numero,cle,ligne, heure,poste)
                values(@va_NumEvt_in,@va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)

		/*A ajout de la longueur du bouchon dans la synthese */
		select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_BOU_LON +
					convert(varchar(7), @vl_Longueur/1000.0)

                insert EXP..FMC_LIS (numero,cle,ligne, heure,poste)
                values(@va_NumEvt_in,@va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)

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

		insert EXP..FMC_LIS (numero,cle,ligne, heure,poste)
                values(@va_NumEvt_in,@va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)

	end
	else if	(@va_Type_in = XZAEC_FMC_Escargot)
	begin
		/*A recherche les informations sur les travaux */
		select
			@vl_Longueur	= longueur
		from EXP..FMC_HIS
		where	numero = @va_NumEvt_in and 
			cle = @va_CleEvt_in  and
			horodate_validation = @va_HoroValidation_in

		/*A ajout de la localisation de la tete de bouchon */
		select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_LON_ESC + 
					convert(varchar(7), @vl_Longueur/1000.0)
	        insert EXP..FMC_LIS (numero,cle,ligne, heure,poste)
                values(@va_NumEvt_in,@va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
	end

	return XDC_OK
#endif

go
