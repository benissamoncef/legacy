/*E*/
/*  Fichier : $Id: xzas34sp.prc,v 1.11 2010/09/01 19:23:53 gesconf Exp $     Release : $Revision: 1.11 $      Date : $Date: 2010/09/01 19:23:53 $ 
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzas34sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Analyse 
*
------------------------------------------------------
* HISTORIQUE :
*
* T.F.  09/12/94        : Creation      (V 1.1)
* B.G.  13/01/95        : modif integration      (V 1.3)
* C.T.  01/03/95        : Recherche infos dans HIS si date
*                         > 1 mois      (V 1.4)
* C.T.  12/02/96        : Les travaux prevus sont exclus si la date de fin
*                         prevus est depassee (V1.5)
* C.T.  26/08/96        : Prise en compte des voies bloquees dans le calcul (V1.6)
* JMG	10/09/96	: on retourne la longueur totale du reseau (V1.7)
* PN	29/06/98	: separation EXP/HIS (V1.8)
* PN    05/02/99	: modif intergest. dem/1724 v1.9
* JPL	11/06/07	: Prise en compte des Fmc Basculement pour Travaux (DEM 647) 1.10
* JPL	25/08/10	: Prise en compte des Fmc Zone de stockage (DEM 942) 1.11
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Calcul le pourcentage de voies coupees
*
* Sequence d'appel
* SP   XZAS34_Voies_Coupees
*
* Arguments en entree :
* XDY_Entier    @va_NumDistrict_in	: Numero de district
* XDY_Date	@va_Date_in		: Date calcul
*
* Arguments en sortie :
* XDY_Entier	@va_Pourcentage_out	: Pourcentage de voies coupees
* XDY_Entier	@va_Longueur_out	: longueur du reseau
*
* Liste retournee :
*
* Code retour
* XDC_OK
* XDC_NOK               : probleme d'acces aux donnees 
* XDC_ARG_INV           : parametres d'entree null, ou valeur non autorisee ou
*                         echangeur inexistante
* XDC_PAS_SYN           : pas de resultat. 
*
* Conditions d'utilisation
* - Si numero de district < 0 alors le calcul est fait pour tout le reseau
* - Le resultat "va_Pourcentage_out" = 1000 * (%) afin d'eviter l'utilisation
*   d'un float comme argument.
* - S'applique sur tous les types d'evenements FMC
*
* Fonction
*
-------------------------------------------------------*/

use PRC
go

create procedure XZAS;34
	@va_NumDistrict_in	int		= null,
	@va_Date_in             datetime	= null,
	@va_Pourcentage_out     int		= null	output,
	@va_Longueur_out	int             = null  output
as
	declare	@vl_NumPortion		int,	 /* Numero de portion */
		@vl_Numero		int,	 /* Numero de FMC */
		@vl_Cle			int,     /* Numero de cle */
		@vl_Portion_Debut	int,     /* Numero de portion */
		@vl_Portion_Fin		int,     /* Numero de portion */
		@vl_Longueur_Totale	int,	 /* Longueur totale de voies */
		@vl_Longueur_Coupees	int,     /* Lgr de voies coupees */ 
		@vl_PR_Debut		int,	 /* PR */
		@vl_PR_Fin              int,     /* PR */
		@vl_TypeEvenement       smallint,/* Type evenement "Travaaux"*/
		@vl_Sens_Debut		tinyint, /* Sens */
		@vl_Sens_Fin            tinyint, /* Sens */
		@vl_NbVoies_Debut	tinyint, /* Nombre de voies */
		@vl_NbVoies_Fin		tinyint, /* Nombre de voies */
		@vl_NbVoies_Retrecies	tinyint, /* Nombre de voies retrecies */
		@vl_Code_Retour		tinyint, /* Code retour fonction */
		@vl_VR                  tinyint, /* Voie rapide */
		@vl_VR_I                tinyint, /* Voie rapide sens inverse */
		@vl_VM2                 tinyint, /* Voie mediane 2 */
		@vl_VM2_I               tinyint, /* Voie mediane 2 ss inverse*/
		@vl_VM1                 tinyint, /* Voie mediane 1 */
		@vl_VM1_I               tinyint, /* Voie mediane 1 ss inverse*/
		@vl_VL                  tinyint, /* Voie lente */
		@vl_VL_I                tinyint, /* Voie lente sens inverse */
		@vl_BAU                 tinyint, /* BAU */
		@vl_BAU_I               tinyint, /* BAU sens inverse */
		@vl_Autoroute		tinyint, /* Autoroute */
		@vl_StatusAutoroute	tinyint, /* Status curseur Autoroute */
		@vl_StatusPortion	tinyint, /* Status curseur Portions */
		@vl_StatusSegments	tinyint, /* Status curseur Segments */
		@vl_HorodateVal_Max	datetime,/* Date de validation */
		@vl_Pourcentage		float,   /* variable intermediaire pour calcul */
		@vl_Status int,
		@vl_RechercheDsHis	bit	 /* Recherche des infos ds HIS sinon ds EXP */

	/* Test Argument APPLIX  */
	if @va_NumDistrict_in = 0    select @va_NumDistrict_in	= null

	/*A 
	** Controle des parametres en entree :
	*/
	if (@va_NumDistrict_in  = null) or
	   (@va_Date_in		= null)
		return XDC_ARG_INV

	/* initialisation de la variable locale */
	select @vl_RechercheDsHis = XDC_NON
#ifdef CI
	/*A 
	** si la date donnee est anterieure d'un mois au moins
	** rechercher les infos dans la base HIS pour le CI
	*/
	if @va_Date_in < (select dateadd(month, -1, getdate ()))
	begin
		select @vl_RechercheDsHis = XDC_OUI
		exec @vl_Status = SQL_HIS.PRC..XZAS;34
			@va_NumDistrict_in	,
			@va_Date_in             ,
			@va_Pourcentage_out     output,
			@va_Longueur_out	output

		if @vl_Status = null
                       	return XDC_PRC_INC
                else return @vl_Status
	end
#endif
#ifdef HIST
		select @vl_RechercheDsHis = XDC_OUI
#endif

	/* Liste des operations (travaux, stockage PL) finies apres la date ou encore en cours */
#ifndef HIST
		declare Travaux cursor for
		select      distinct	GEN.numero,
			    		GEN.cle
		from EXP..FMC_GEN GEN,EXP..FMC_HIS HIS
		where GEN.type in (XZAEC_FMC_Travaux, XZAEC_FMC_ZoneStockage, XZAEC_FMC_BasculTravaux)
		and   GEN.datex = XDC_FAUX
		and   isnull(GEN.debut,GEN.debut_prevu) <= @va_Date_in
		and   (isnull(GEN.fin,GEN.fin_prevue)   > @va_Date_in
		 or   isnull(GEN.fin,GEN.fin_prevue)    = null)
		and   HIS.horodate_validation <= @va_Date_in
		and   HIS.cle = GEN.cle
		and   HIS.numero = GEN.numero
#else
		declare Travaux cursor for
		select      distinct	GEN.numero,
			    		GEN.cle
		from HIS..FMC_GEN GEN,HIS..FMC_HIS HIS
		where GEN.type in (XZAEC_FMC_Travaux, XZAEC_FMC_ZoneStockage, XZAEC_FMC_BasculTravaux)
		and   GEN.datex = XDC_FAUX
		and   isnull(GEN.debut,GEN.debut_prevu) <= @va_Date_in
		and   (isnull(GEN.fin,GEN.fin_prevue)   > @va_Date_in
		 or   isnull(GEN.fin,GEN.fin_prevue)    = null)
		and   HIS.horodate_validation <= @va_Date_in
		and   HIS.cle = GEN.cle
		and   HIS.numero = GEN.numero
#endif

	/* Initialisation de la longueur totale du reseau avant calcul */
	select @vl_Longueur_Totale = 0

	/*A
	** Recherche des segments pour chaque portion d'une autoroute.
	*/
	 declare Segments cursor for
		 select  portion,
		 	 PR_debut,
			 sens,
			 nombre_de_voies
		 from CFG..RES_SEG

	open Segments

	/* Positionnement en debut de liste */
	fetch Segments into @vl_NumPortion,
		@vl_PR_Debut,
		@vl_Sens_Debut,
		@vl_NbVoies_Debut
			
	/* Tant qu'il existe des segments dans la liste */
	while @@sqlstatus = 0
	begin
		select @vl_PR_Fin=null
				
		select @vl_PR_Fin=min(PR_debut)
		from CFG..RES_SEG
		where portion=@vl_NumPortion
		and sens=@vl_Sens_Debut
		and PR_debut>@vl_PR_Debut
				  
		if @vl_PR_Fin=null select @vl_PR_Fin=PR_fin
				from CFG..RES_POR
				where numero=@vl_NumPortion
				
		select @vl_Longueur_Totale = @vl_Longueur_Totale +
						(@vl_PR_Fin-@vl_PR_Debut) *
						@vl_NbVoies_Debut
				
		/* Passage au segment suivant */
		fetch Segments into @vl_NumPortion,
				@vl_PR_Debut,
				@vl_Sens_Debut,
				@vl_NbVoies_Debut
	end /* Pour chaque segment */

	close Segments
				

	/*A
	** Calcul de la longueur totale de voies coupees pour tout le reseau
	** ou pour un district donne a l'heure demandee.
	*/
	select @vl_Longueur_Coupees = 0


	/* Positionnement sur le premier enregistrement */
	open Travaux
	fetch Travaux into      @vl_Numero,
				@vl_Cle

	/* Tant qu'il exite des evenements */
#ifndef HIST
	   while @@sqlstatus = 0
	   begin

		/*A
		** Recherche de l'horodate maximale de validation.
		*/
		select @vl_HorodateVal_Max = max(horodate_validation)
		from  EXP..FMC_HIS 
		where numero = @vl_Numero
		and   cle    = @vl_Cle
		and   horodate_validation <= @va_Date_in

		/* Recheche des informations Travaux */
		select  @vl_PR_Debut = HIS.PR,
			@vl_VR = VR,
			@vl_VR_I = VR_I,
			@vl_VM2 = VM2,
			@vl_VM2_I = VM2_I,
			@vl_VM1 = VM1,
			@vl_VM1_I = VM1_I,
			@vl_VL = VL,
			@vl_VL_I = VL_I,
			@vl_BAU = BAU,
			@vl_BAU_I = BAU_I
		from EXP..FMC_HIS HIS
		where HIS.numero = @vl_Numero
		and   HIS.cle    = @vl_Cle
		and   HIS.horodate_validation = @vl_HorodateVal_Max

		/* Recheche du PR de fin de travaux */
		select @vl_PR_Fin =  TRH.localisation_tete
		from EXP..FMC_TRH TRH
		where TRH.numero = @vl_Numero
		and   TRH.cle    = @vl_Cle
		and   TRH.horodate = @vl_HorodateVal_Max

		/* Recheche du nombre de voies retrecies pour chaque "Travaux"*/
		select @vl_NbVoies_Retrecies = 0

		/* Voie rapide */
		if((@vl_VR = XDC_VOIE_NEUTRALISEE) or
		   (@vl_VR = XDC_VOIE_BLOQUEE) or
		   (@vl_VR = XDC_VOIE_BLOQUEE_NEUTRALISEE))
		   select @vl_NbVoies_Retrecies = @vl_NbVoies_Retrecies + 1

		/* Voie mediane 2 */
		if ((@vl_VM2 = XDC_VOIE_NEUTRALISEE) or
		    (@vl_VM2 = XDC_VOIE_BLOQUEE) or
		    (@vl_VM2 = XDC_VOIE_BLOQUEE_NEUTRALISEE))
		    select @vl_NbVoies_Retrecies = @vl_NbVoies_Retrecies + 1

		/* Voie mediane 1 */
		if ((@vl_VM1 = XDC_VOIE_NEUTRALISEE) or
		    (@vl_VM1 = XDC_VOIE_BLOQUEE) or
		    (@vl_VM1 = XDC_VOIE_BLOQUEE_NEUTRALISEE))
		    select @vl_NbVoies_Retrecies = @vl_NbVoies_Retrecies + 1

		/* Voie lente */
		if ((@vl_VL= XDC_VOIE_NEUTRALISEE) or
		    (@vl_VL = XDC_VOIE_BLOQUEE) or
		    (@vl_VL= XDC_VOIE_BLOQUEE_NEUTRALISEE))
		    select @vl_NbVoies_Retrecies = @vl_NbVoies_Retrecies + 1

		/*A
		** Longueur voies coupees = Longueur des Travaux * nombre
		**                          de voies retrecies.
		*/
		select @vl_Longueur_Coupees = @vl_Longueur_Coupees +
					isnull((abs(@vl_PR_Fin - @vl_PR_Debut) *
					@vl_NbVoies_Retrecies),0)

		/* Positionnement sur l'enregistrement suivant */
		fetch Travaux into      @vl_Numero,
					@vl_Cle
	   end /* fin curseur travaux */
#else
	   while @@sqlstatus = 0
	   begin

		/*A
		** Recherche de l'horodate maximale de validation.
		*/
		select @vl_HorodateVal_Max = max(horodate_validation)
		from  HIS..FMC_HIS 
		where numero = @vl_Numero
		and   cle    = @vl_Cle
		and   horodate_validation <= @va_Date_in

		/* Recheche des informations Travaux */
		select  @vl_PR_Debut = HIS.PR,
			@vl_VR = VR,
			@vl_VR_I = VR_I,
			@vl_VM2 = VM2,
			@vl_VM2_I = VM2_I,
			@vl_VM1 = VM1,
			@vl_VM1_I = VM1_I,
			@vl_VL = VL,
			@vl_VL_I = VL_I,
			@vl_BAU = BAU,
			@vl_BAU_I = BAU_I
		from HIS..FMC_HIS HIS
		where HIS.numero = @vl_Numero
		and   HIS.cle    = @vl_Cle
		and   HIS.horodate_validation = @vl_HorodateVal_Max

		/* Recheche du PR de fin de travaux */
		select @vl_PR_Fin =  TRH.localisation_tete
		from HIS..FMC_TRH TRH
		where TRH.numero = @vl_Numero
		and   TRH.cle    = @vl_Cle
		and   TRH.horodate = @vl_HorodateVal_Max

		/* Recheche du nombre de voies retrecies pour chaque "Travaux"*/
		select @vl_NbVoies_Retrecies = 0

		/* Voie rapide */
		if((@vl_VR = XDC_VOIE_NEUTRALISEE) or
		   (@vl_VR = XDC_VOIE_BLOQUEE) or
		   (@vl_VR = XDC_VOIE_BLOQUEE_NEUTRALISEE))
		   select @vl_NbVoies_Retrecies = @vl_NbVoies_Retrecies + 1

		/* Voie mediane 2 */
		if ((@vl_VM2 = XDC_VOIE_NEUTRALISEE) or
		    (@vl_VM2 = XDC_VOIE_BLOQUEE) or
		    (@vl_VM2 = XDC_VOIE_BLOQUEE_NEUTRALISEE))
		    select @vl_NbVoies_Retrecies = @vl_NbVoies_Retrecies + 1

		/* Voie mediane 1 */
		if ((@vl_VM1 = XDC_VOIE_NEUTRALISEE) or
		    (@vl_VM1 = XDC_VOIE_BLOQUEE) or
		    (@vl_VM1 = XDC_VOIE_BLOQUEE_NEUTRALISEE))
		    select @vl_NbVoies_Retrecies = @vl_NbVoies_Retrecies + 1

		/* Voie lente */
		if ((@vl_VL= XDC_VOIE_NEUTRALISEE) or
		    (@vl_VL = XDC_VOIE_BLOQUEE) or
		    (@vl_VL= XDC_VOIE_BLOQUEE_NEUTRALISEE))
		    select @vl_NbVoies_Retrecies = @vl_NbVoies_Retrecies + 1

		/*A
		** Longueur voies coupees = Longueur des Travaux * nombre
		**                          de voies retrecies.
		*/
		select @vl_Longueur_Coupees = @vl_Longueur_Coupees +
					isnull((abs(@vl_PR_Fin - @vl_PR_Debut) *
					@vl_NbVoies_Retrecies),0)

		/* Positionnement sur l'enregistrement suivant */
		fetch Travaux into      @vl_Numero,
					@vl_Cle
	   end /* fin curseur travaux */
#endif

	close Travaux

	/*A
	** Calcul du pourcentage de voies coupees.
	*/
	if @vl_Longueur_Coupees != 0 and @vl_Longueur_Totale != 0
	begin
		select @vl_Pourcentage = (1000.0 * 100.0 * @vl_Longueur_Coupees) /
					@vl_Longueur_Totale
		/* le resultat est multiplie par 1000 de facon a avoir un */
		/* maximun de precision sans avoir recours a un float en  */
		/* argument de sortie.					  */
		select @va_Pourcentage_out=@vl_Pourcentage
	end
	else
		select @va_Pourcentage_out = 0

	select @va_Longueur_out=@vl_Longueur_Totale

	/*A
	** Fin de la procedure
	*/
 	return XDC_OK
go
