/*E*/
/*  Fichier : $Id: xzas23sp.prc,v 1.10 1999/02/26 11:45:34 gaborit Exp $    Release : $Revision: 1.10 $       Date : $Date: 1999/02/26 11:45:34 $ 
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzas23sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Analyse 
*
------------------------------------------------------
* HISTORIQUE :
*
* T.F.  05/12/94        : Creation      (V 1.1)
* Guilhou	30/12/94	: enlever test sur prdebut a 0	V1.2
* B.G.  12/01/95        : pb valeurs <0      (V 1.4)
* B.G.  01/02/95        : modif pour prevus      (V 1.5)
* C.T.  01/03/95        : Recherche infos dans HIS si date de
*                         calcul > 1 mois pour CI (V 1.6)
* B.G.  01/09/95        : ajout de order by dans le resultat (1.7)
* B.G.  01/09/95        : suppression du return si pas de travaux (1.8)
* P.N	29/06/98        : separation EXP/HIS (1.9)
* P.N   05/02/99	: modif intergest dem/1724 1.10
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Calcul la gene travaux pour une section donnee a une heure donnee.
*
* Sequence d'appel
* SP    XZAS23_Gene_Travaux
*
* Arguments en entree :
* XDY_PR	@va_PR_DebSection_in	: PR du debut de la section 
* XDY_PR	@va_PR_FinSection_in    : PR de fin de la section  
* XDY_Octet	@va_NumAutoroute_in	: Numero de l'autoroute
* XDY_Datetime	@va_Horodate_in		: Date du calcul 
* XDY_Mot	@va_Vit_H_T_in		: Vitesse hors travaux (Km/h)
* XDY_Mot       @va_Vit_BAU_in		: Vitesse BAU neutralise (Km/h)
* XDY_Mot       @va_Vit_Voie_Red_in     : Vitesse voie reduite (Km/h)
* XDY_Mot       @va_Vit_BAS_in       	: Vitesse basculement (Km/h)
*
* Arguments en sortie :
* XDY_Octet     @va_Status_out          : Status retour de la procedure.
*
* Liste retournee :
* XDY_Entier	lg_Section	: Longueur de la section 
* XDY_Entier	perte_temps	: Perte de temps (en seconde)
* XDY_Entier	temps_theorique	: Temps de parcours sans travaux (en seconde)
* XDY_Entier    temps_reel 	: Temps avec travaux (en seconde)
* XDY_Sens	sens		: Sens de parcours
*
* Code retour
* XDC_OK
* XDC_NOK               : probleme d'acces aux donnees 
* XDC_ARG_INV           : parametres d'entree null, ou valeur non autorisee ou
*                         echangeur inexistante
* XDC_PAS_SYN           : pas de resultat.
*
* Conditions d'utilisation
* Une section est definie comme un PR de debut et un PR de fin
* situes sur la meme autoroute. Les travaux consideres a l'heure demandee sont
* en cours...
* La perte de temps = Temps de parcours avec travaux - Temps sans travaux
* La perte_temps, le temps_theorique et le temps_reel sont exprime en seconde
*
* Fonction
*
-------------------------------------------------------*/

use PRC
go

create procedure XZAS;23
	@va_PR_DebSection_in	T_PR		= null,
	@va_PR_FinSection_in    T_PR            = null,
	@va_NumAutoroute_in	tinyint		= null,
	@va_Horodate_in		datetime	= null,
	@va_Vit_H_T_in 		smallint	= null,
	@va_Vit_BAU_in         	smallint	= null,
	@va_Vit_Voie_Red_in    	smallint	= null,
	@va_Vit_BAS_in		smallint	= null,
	@va_Status_out          tinyint         = null output
as
	declare @vl_TypeEvenement	smallint,/* Type evenement "Travaaux"*/ 
		@vl_Numero              int,     /* Numero d'evenement */
		@vl_LgSection		float,	 /* Longueur de la section */
		@vl_Lg_BAU		float,	 /* Lgr BAU neutralisee */
		@vl_Lg_Voie_Red		float,	 /* Lgr voie reduite */
		@vl_Lg_BAS		float,	 /* Lgr de basculement */
		@vl_Lg_BAU_I            float,   /* Lgr BAU neutralisee (inv)*/
		@vl_Lg_Voie_Red_I       float,   /* Lgr voie reduite (inv)*/
		@vl_Lg_BAS_I            float,   /* Lgr de basculement (inv)*/
		@vl_TempsTheo		float,	 /* Temps theorique */
		@vl_TempsReel		float,	 /* Tps retard de parcours */
		@vl_TempsReel_I		float,	 /* Tps ret de parcours (inv)*/
		@vl_PerteTemps		float,   /* Perte de temps */
		@vl_Cle     		tinyint, /* Numero de cle */
		@vl_VR			tinyint, /* Voie rapide */
		@vl_VR_I                tinyint, /* Voie rapide sens inverse */
		@vl_VM2                 tinyint, /* Voie mediane 2 */
		@vl_VM2_I               tinyint, /* Voie mediane 2 ss inverse*/
		@vl_VM1                 tinyint, /* Voie mediane 1 */
		@vl_VM1_I               tinyint, /* Voie mediane 1 ss inverse*/
		@vl_VL			tinyint, /* Voie lente */
		@vl_VL_I		tinyint, /* Voie lente sens inverse */
		@vl_BAU			tinyint, /* BAU */
		@vl_BAU_I		tinyint, /* BAU sens inverse */
		@vl_Sens                tinyint, /* Sens de la voie */
		@vl_CodeRet	   	smallint,/* Code retour fonction */
		@vl_PR_DebTrav    	T_PR,	 /* PR de debut de travaux */
		@vl_PR_FinTrav          T_PR,    /* PR de fin de travaux */
		@vl_HorodateVal_Max     datetime,
		@vl_Status 		int,
		@vl_RechercheDsHis	bit

	/* Test Argument APPLIX (il n'existe pas d'equipement de type zero) */
	if @va_PR_FinSection_in = 0	select @va_PR_FinSection_in = null
	if @va_NumAutoroute_in  = 0	select @va_NumAutoroute_in  = null
	if @va_Vit_H_T_in	= 0     select @va_Vit_H_T_in	    = null
	if @va_Vit_BAU_in       = 0     select @va_Vit_BAU_in	    = null
	if @va_Vit_Voie_Red_in  = 0     select @va_Vit_Voie_Red_in  = null
	if @va_Vit_BAS_in	= 0     select @va_Vit_BAS_in	    = null

	/*A 
	** Controle des parametres en entree :
	*/
	if @va_PR_FinSection_in = null or
	   @va_NumAutoroute_in  = null or
	   @va_Horodate_in	= null or
	   @va_Vit_H_T_in	= null or
	   @va_Vit_BAU_in	= null or
	   @va_Vit_Voie_Red_in	= null or 
	   @va_Vit_BAS_in	= null
		return XDC_ARG_INV

	/*A
	** Test de coherence (PR de debut < PR de fin)
	*/
	if @va_PR_DebSection_in >= @va_PR_FinSection_in
	begin
		/* Inversion des parametres */
		select @vl_PR_DebTrav = @va_PR_DebSection_in
		select @va_PR_DebSection_in = @va_PR_FinSection_in
		select @va_PR_FinSection_in = @vl_PR_DebTrav 
	end

	/*A
	** initialisation variable locale 
	*/
	select @vl_RechercheDsHis = XDC_NON

#ifdef CI
	/*A
	** si la date donnee est anterieure d'un mois au moins
	** rechercher les infos dans la base HIS sinon dans la base EXP
	*/
	if @va_Horodate_in < (select dateadd(month, -1, getdate ()))
	begin
		select @vl_RechercheDsHis = XDC_OUI
		exec @vl_Status = SQL_HIS.PRC..XZAO;22
			@va_PR_DebSection_in	,
			@va_PR_FinSection_in    ,
			@va_NumAutoroute_in	,
			@va_Horodate_in		,
			@va_Vit_H_T_in 		,
			@va_Vit_BAU_in         	,
			@va_Vit_Voie_Red_in    	,
			@va_Vit_BAS_in		,
			@va_Status_out          output

		if @vl_Status = null
                        return XDC_PRC_INC
                else return @vl_Status
	end
#endif

#ifdef HIST
		select @vl_RechercheDsHis = XDC_OUI
#endif

	/*A
	** type d'evenement main courante "TRAVAUX"
	*/
	select @vl_TypeEvenement = 18
	
	/*A
	** Calcul de la longueur de la section.
	*/
	execute @vl_CodeRet = PRC..XZAO;22 @va_NumAutoroute_in,
				      	   @va_PR_DebSection_in,
				           @va_NumAutoroute_in,
				           @va_PR_FinSection_in,
				           @vl_LgSection out

	if @vl_CodeRet != XDC_OK
		return @vl_CodeRet 

	if @vl_LgSection = null
	begin
		select @va_Status_out = XDC_PAS_SYN
		return XDC_OK
	end

	/* Creation du table temporaire afin de concerver l'ensemble des */	
	/* fiches main courante de type travaux				 */
	create table #FMC_TRAV (	numero	int		null,
					cle	tinyint 	null,
					horodate_validation datetime   null)
	/*A
	** Recherche des fiches main courante de type "Travaux"
	** dont la date de debut est inferieure a la date recherchee 
	** et dont la date de fin n'est pas renseignee ou superieure a la date
	** de recherche (Travaux en cours).
	** Ces travaux doivent etre situes sur l'autoroute
	** demandee avec un PR de debut qui soit compris entre les PR de la
	** section consideree.
	*/
#ifndef HIST
		insert #FMC_TRAV
		select 	distinct GEN.numero,
			GEN.cle,
			HIS.horodate_validation
		from EXP..FMC_GEN GEN,EXP..FMC_HIS HIS
		where GEN.type = @vl_TypeEvenement
		and   GEN.datex = XDC_FAUX
		and   isnull(GEN.debut,GEN.debut_prevu) <= @va_Horodate_in
		and   (GEN.fin = null or
		       GEN.fin > @va_Horodate_in)
		and   HIS.autoroute = @va_NumAutoroute_in
		and   HIS.horodate_validation <= @va_Horodate_in
		and   HIS.cle = GEN.cle
		and   HIS.numero = GEN.numero
#else
		insert #FMC_TRAV
		select 	distinct GEN.numero,
			GEN.cle,
			HIS.horodate_validation
		from HIS..FMC_GEN GEN,HIS..FMC_HIS HIS
		where GEN.type = @vl_TypeEvenement
		and   GEN.datex = XDC_FAUX
		and   isnull(GEN.debut,GEN.debut_prevu) <= @va_Horodate_in
		and   (GEN.fin = null or
		       GEN.fin > @va_Horodate_in)
		and   HIS.autoroute = @va_NumAutoroute_in
		and   HIS.horodate_validation <= @va_Horodate_in
		and   HIS.cle = GEN.cle
		and   HIS.numero = GEN.numero
#endif


	/* Creation du table temporaire afin de concerver l'ensemble des */
	/* etapes resultats.						 */
	create table #GENE_RES(	lg_Section	int	null,
				perte_temps	int	null,
				temps_theorique int	null,
				temps_reel	int	null,
				sens		tinyint null
				)

	/* Mise a jour de longueur */
	select @vl_Lg_BAS = 0
	select @vl_Lg_BAS_I = 0
	select @vl_Lg_Voie_Red = 0
	select @vl_Lg_Voie_Red_I = 0
	select @vl_Lg_BAU = 0
	select @vl_Lg_BAU_I = 0

	/*A
	** Parcours de chacun des evenements "travaux" repondant aux criteres
	** requis.
	*/
	declare Travaux cursor for
		select distinct numero,cle
		from #FMC_TRAV

	open Travaux

	/* Positionnement sur le premier enregistrement */
	fetch Travaux into 	@vl_Numero,
				@vl_Cle

	/* Tant qu'il exite de evenements de ce type */ 
	while @@sqlstatus = 0
	begin

		/*A
		** Recherche de l'horodate maximale de validation.
		** Ceci de facon a avoir l'etat le plus recent.
		*/
		select @vl_HorodateVal_Max = max(horodate_validation)
		from #FMC_TRAV
		where numero = @vl_Numero
		and   cle    = @vl_Cle

		/*A
		** Recherche des informations concernant l'evenement 
		** selectionne.
		*/
#ifndef HIST
		begin
			/*B
			**Recherche du PR de debut de travaux ainsi que l'ensemble
			** des informations concernant chacune des voies.
			*/
			select 	@vl_PR_DebTrav = PR,
				@vl_Sens = 	sens,
				@vl_VR	=	VR,
				@vl_VM2 =	VM2,
				@vl_VM1 =	VM1,
				@vl_VL	=	VL,
				@vl_BAU	=	BAU_I,
				@vl_VR_I  =     VR_I,
				@vl_VM2_I =     VM2_I,
				@vl_VM1_I =     VM1_I,
				@vl_VL_I  =     VL_I,
				@vl_BAU_I =     BAU_I
			from EXP..FMC_HIS
			where numero = @vl_Numero
			and   cle    = @vl_Cle
			and   horodate_validation = @vl_HorodateVal_Max
	
			/*A
			** Si ces informations sont introuvables, alors l'evenement
			** suivant et selectionne.
			*/
			if @@rowcount = 0
			begin
				/* Passage a l'enregistrement suivant */
				fetch Travaux into      @vl_Numero,
							@vl_Cle
				continue
			end
				
			/* Recheche du PR de fin de travaux */ 
			select @vl_PR_FinTrav = localisation_tete
			from EXP..FMC_TRH
			where numero = @vl_Numero
			and   cle    = @vl_Cle
			and   horodate = @vl_HorodateVal_Max
	
			if @@rowcount = 0
			begin
				/* Passage a l'enregistrement suivant */
				fetch Travaux into      @vl_Numero,
							@vl_Cle
				continue
			end
		end
#else
		begin
			/*B
			**Recherche du PR de debut de travaux ainsi que l'ensemble
			** des informations concernant chacune des voies.
			*/
			select 	@vl_PR_DebTrav = PR,
				@vl_Sens = 	sens,
				@vl_VR	=	VR,
				@vl_VM2 =	VM2,
				@vl_VM1 =	VM1,
				@vl_VL	=	VL,
				@vl_BAU	=	BAU_I,
				@vl_VR_I  =     VR_I,
				@vl_VM2_I =     VM2_I,
				@vl_VM1_I =     VM1_I,
				@vl_VL_I  =     VL_I,
				@vl_BAU_I =     BAU_I
			from HIS..FMC_HIS
			where numero = @vl_Numero
			and   cle    = @vl_Cle
			and   horodate_validation = @vl_HorodateVal_Max
	
			/*A
			** Si ces informations sont introuvables, alors l'evenement
			** suivant et selectionne.
			*/
			if @@rowcount = 0
			begin
				/* Passage a l'enregistrement suivant */
				fetch Travaux into      @vl_Numero,
							@vl_Cle
				continue
			end
				
			/* Recheche du PR de fin de travaux */ 
			select @vl_PR_FinTrav = localisation_tete
			from HIS..FMC_TRH
			where numero = @vl_Numero
			and   cle    = @vl_Cle
			and   horodate = @vl_HorodateVal_Max
	
			if @@rowcount = 0
			begin
				/* Passage a l'enregistrement suivant */
				fetch Travaux into      @vl_Numero,
							@vl_Cle
				continue
			end
		end
#endif

		/*A racourcissement des travaux a la section */
		if @vl_PR_FinTrav>@va_PR_FinSection_in
			select @vl_PR_FinTrav=@va_PR_FinSection_in
		if @vl_PR_FinTrav<@va_PR_DebSection_in
			select @vl_PR_FinTrav=@va_PR_DebSection_in
		if @vl_PR_DebTrav>@va_PR_FinSection_in
			select @vl_PR_DebTrav=@va_PR_FinSection_in
		if @vl_PR_DebTrav<@va_PR_DebSection_in
			select @vl_PR_DebTrav=@va_PR_DebSection_in

		/*A
		** Test sur l'implication des travaux sur la circulation
		** (detection de :
		**	- Basculement
		**	- Voies reduites ou voies uniques
		**	- BAU neutralisee).
		** Mise a jour des longueurs de voies affectes par les 
		** travaux en fonction des cas detectes.
		*/

		/* Il y a basculement si ou moins une voie du sens oppose */
		/* est inversee (XDC_VOIE_SENS_INVERSE)			  */
		/* Pour un basculement, les deux sens sont affectee.	  */
		if ( 	(@vl_VR_I = XDC_VOIE_SENS_INVERSE) or
			(@vl_VM2_I = XDC_VOIE_SENS_INVERSE) or
			(@vl_VM1_I = XDC_VOIE_SENS_INVERSE) or
			(@vl_VL_I = XDC_VOIE_SENS_INVERSE) or
			(@vl_BAU_I = XDC_VOIE_SENS_INVERSE) or
			(@vl_VR = XDC_VOIE_SENS_INVERSE) or
			(@vl_VM2 = XDC_VOIE_SENS_INVERSE) or
			(@vl_VM1 = XDC_VOIE_SENS_INVERSE) or
			(@vl_VL = XDC_VOIE_SENS_INVERSE) or
			(@vl_BAU = XDC_VOIE_SENS_INVERSE))
		begin
		  select @vl_Lg_BAS = @vl_Lg_BAS +
			  abs(@vl_PR_FinTrav-@vl_PR_DebTrav)
		  select @vl_Lg_BAS_I = @vl_Lg_BAS_I +
			  abs(@vl_PR_FinTrav-@vl_PR_DebTrav)
		end
		/* Les voies voies sont reduites (XDC_VOIE_NEUTRALISEE ou */
		/* XDC_VOIE_BLOQUEE_NEUTRALISEE)			  */
		else if((@vl_VR = XDC_VOIE_NEUTRALISEE) or
			(@vl_VM2 = XDC_VOIE_NEUTRALISEE) or
			(@vl_VM1 = XDC_VOIE_NEUTRALISEE) or
			(@vl_VL= XDC_VOIE_NEUTRALISEE) or
			(@vl_VR = XDC_VOIE_BLOQUEE_NEUTRALISEE) or
			(@vl_VM2 = XDC_VOIE_BLOQUEE_NEUTRALISEE) or
			(@vl_VM1 = XDC_VOIE_BLOQUEE_NEUTRALISEE) or
			(@vl_VL= XDC_VOIE_BLOQUEE_NEUTRALISEE))
		begin
			if (@vl_Sens = XDC_SENS_SUD)
			  select @vl_Lg_Voie_Red = @vl_Lg_Voie_Red +
				abs(@vl_PR_FinTrav-@vl_PR_DebTrav)
			else if (@vl_Sens = XDC_SENS_NORD)
			  select @vl_Lg_Voie_Red_I = @vl_Lg_Voie_Red_I +
			    	abs(@vl_PR_FinTrav-@vl_PR_DebTrav)
		end
		/* La BAU est neutralisee (XDC_VOIE_NEUTRALISEE ou        */
		/* XDC_VOIE_BLOQUEE_NEUTRALISEE)                          */
		else if((@vl_BAU = XDC_VOIE_NEUTRALISEE) or
			(@vl_BAU = XDC_VOIE_BLOQUEE_NEUTRALISEE))
		begin
			if (@vl_Sens = XDC_SENS_SUD)
			  select @vl_Lg_BAU = @vl_Lg_BAU +
			 	abs(@vl_PR_FinTrav-@vl_PR_DebTrav)
			else if (@vl_Sens = XDC_SENS_NORD)
			  select @vl_Lg_BAU_I = @vl_Lg_BAU_I +
			    	abs(@vl_PR_FinTrav-@vl_PR_DebTrav)
	    	end

		/* Passage a l'enregistrement suivant */
		fetch Travaux into	@vl_Numero,
					@vl_Cle
	end

	/* Fermeture du curseur */
	close Travaux 

	/*A
	** Calcul du temps theorique en seconde (Pas de travaux sur la section).
	*/
	select @vl_TempsTheo	= 3.6*(@vl_LgSection / @va_Vit_H_T_in)

	/*A
	** Calcul du temps reel dans le sens normal en seconde.
	*/
	select @vl_TempsReel	= 3.6*(((@vl_LgSection -
					 @vl_Lg_BAS -
					 @vl_Lg_Voie_Red - 
					 @vl_Lg_BAU) / 
					 @va_Vit_H_T_in)	+
				       (@vl_Lg_BAS / @va_Vit_BAS_in +
					@vl_Lg_Voie_Red/ @va_Vit_Voie_Red_in +
					@vl_Lg_BAU/ @va_Vit_BAU_in))

	/*A
	** Calcul du temps reel dans le sens inverse en seconde.
	*/
	select @vl_TempsReel_I = 3.6*(((@vl_LgSection -
					@vl_Lg_BAS_I -
					@vl_Lg_Voie_Red_I -
					@vl_Lg_BAU_I) /
					@va_Vit_H_T_in)        +
				      (@vl_Lg_BAS_I / @va_Vit_BAS_in +
				       @vl_Lg_Voie_Red_I/ @va_Vit_Voie_Red_in +
				       @vl_Lg_BAU_I/ @va_Vit_BAU_in))

	/*A
	** Calcul de la perte de temps pour le sens normal et pour le
	** sens inverse.
	*/

	/* Sens normal. */
	select @vl_PerteTemps = @vl_TempsReel - @vl_TempsTheo

	insert #GENE_RES (
			lg_Section,
			perte_temps,
			temps_theorique,
			temps_reel,
			sens)
	values (	@vl_LgSection,
			@vl_PerteTemps,
			@vl_TempsTheo,
			@vl_TempsReel,
			XDC_SENS_SUD)

	/* Sens inverse. */
	select @vl_PerteTemps = @vl_TempsReel_I - @vl_TempsTheo

	insert #GENE_RES (
			lg_Section,
			perte_temps,
			temps_theorique,
			temps_reel,
			sens)
	values (        @vl_LgSection,
			@vl_PerteTemps,
			@vl_TempsTheo,
			@vl_TempsReel_I,
			XDC_SENS_NORD)
	/*A
	** Affichage du resultat (informations donnees au programme appelant)
	*/
	select * from #GENE_RES order by sens

	/* Suppression des tables temporaires */
	drop table #GENE_RES
	drop table #FMC_TRAV 

	/*A
	** Fin de la procedure
	*/
 	return XDC_OK
go
