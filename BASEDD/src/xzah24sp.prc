/*E*/
/*  Fichier : $Id: xzah24sp.prc,v 1.2 1994/12/30 11:06:17 gaborit Exp $      Release : $Revision: 1.2 $        Date : $Date: 1994/12/30 11:06:17 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzah24sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des synoptiques pour animation
* de l'ihm
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.  06/12/94        : Cr≈ation
* B.G.  26/12/94        : Modif (1.2)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Prepare l'animation IHM (voies invers≈es pour un basculement 
* avec un bouchon)
*
* Sequence d'appel
* SP    XZAH24_Animer_Bouchon_Inverse
*
* Arguments en entree
* XDY_Entier            va_NumBouchon_in
* XDY_Octet             va_CleBouchon_in
* XDY_PR                va_PRBouchonMin_in
* XDY_PR                va_PRBouchonMax_in
* XDY_Entier            va_NumBascul_in
* XDY_Octet             va_CleBascul_in
* XDY_PR                va_PRBasculMin_in
* XDY_PR                va_PRBasculMax_in
* XDY_Voie              va_VR_in
* XDY_Voie              va_VM2_in
* XDY_Voie              va_VM1_in
* XDY_Voie              va_VL_in
* XDY_Voie              va_BAU_in
* XDY_Autoroute         va_Autoroute_in
* XDY_Sens              va_SensBascul_in
*
*
* Arguments en sortie
*
* Code retour
* XDC_OK
* XDC_ARG_INV	: argument null
*
* Conditions d'utilisation
* Les etats des voies passes en argument sont ceux du sens inverse
* c-a-d va_SensBascul_in
* Les PR sont donnes dans le sens croissant
* La procedure est appel≈e par XZAH;19
*
------------------------------------------------------*/

use PRC
go

create procedure XZAH;24
	@va_NumBouchon_in	int	= null,
	@va_CleBouchon_in	tinyint = null,
	@va_PRBouchonMin_in	T_PR	= null,
	@va_PRBouchonMax_in	T_PR	= null,
	@va_TypeBouchon_in	tinyint	= null,
	@va_NumBascul_in	int	= null,
	@va_CleBascul_in	tinyint = null,
	@va_PRBasculMin_in	T_PR	= null,
	@va_PRBasculMax_in	T_PR	= null,
	@va_VR_in               T_VOIE	= null,
	@va_VM2_in              T_VOIE	= null,
	@va_VM1_in              T_VOIE	= null,
	@va_VL_in               T_VOIE	= null,
	@va_BAU_in              T_VOIE	= null,
	@va_Autoroute_in	T_AUTOROUTE = null,
	@va_SensBascul_in	T_SENS	= null
as
	declare @vl_Portion smallint, @vl_PRDebutSeg T_PR, 
		@vl_PRFinSeg T_PR,
		@vl_PRDebutPortion T_PR, @vl_PRFinPortion T_PR, 
		@vl_NbVoies tinyint, @vl_Translation int,
		@vl_Extension int

	/*A Controle des parametres d'entree */
	if @va_NumBouchon_in = null or @va_CleBouchon_in = null or
	   @va_PRBouchonMin_in = null or @va_PRBouchonMax_in = null or
	   @va_TypeBouchon_in = null or @va_NumBascul_in = null or
	   @va_CleBascul_in = null or @va_PRBasculMin_in = null or
	   @va_PRBasculMax_in = null or @va_VR_in = null or
	   @va_VM2_in = null or @va_VM1_in = null or
	   @va_VL_in = null or @va_BAU_in = null or
	   @va_Autoroute_in = null or @va_SensBascul_in = null
		return XDC_ARG_INV

	/*A Ramene PRBouchonMin et Max a leur valeur utile (recouvrant */
	/* le basculement  */
	
	if @va_PRBouchonMin_in<@va_PRBasculMin_in
	select @va_PRBouchonMin_in=@va_PRBasculMin_in
	
	if @va_PRBouchonMax_in>@va_PRBasculMax_in
	select @va_PRBouchonMax_in=@va_PRBasculMax_in
	
	/*A Rechercher des segments concern≈s par le bouchon   */
	/*A et etant sur le cote oppose (cote du basculement)  */
	declare Pointeur_segment cursor
	for select
		CFG..RES_SEG.portion,
		CFG..RES_SEG.PR_debut,
		CFG..RES_POR.PR_debut,
		CFG..RES_POR.PR_fin,
		CFG..RES_SEG.nombre_de_voies
	from CFG..RES_POR, CFG..RES_SEG
	where CFG..RES_POR.autoroute = @va_Autoroute_in and
		CFG..RES_SEG.portion = CFG..RES_POR.numero and
		CFG..RES_SEG.sens = @va_SensBascul_in and
		CFG..RES_SEG.PR_debut < @va_PRBouchonMax_in and
		CFG..RES_POR.PR_fin > @va_PRBouchonMin_in

	/*A Pour tous les segments concern≈s, inserer les voies inversees */
	open Pointeur_segment
	fetch Pointeur_segment into @vl_Portion, @vl_PRDebutSeg, @vl_PRDebutPortion, 
			@vl_PRFinPortion, @vl_NbVoies
	while @@sqlstatus = 0
	begin
		/* Recherche le PR de fin : debut du suivant ou */
		/* fin de portion                               */
		select @vl_PRFinSeg = min(PR_debut)
		from CFG..RES_SEG
		where portion=@vl_Portion and sens = @va_SensBascul_in and
			PR_debut > @vl_PRDebutSeg

		if @vl_PRFinSeg is null
			select @vl_PRFinSeg = @vl_PRFinPortion

		/* raccourcit le segment » la zone du basculement */
		if @vl_PRFinSeg > @va_PRBouchonMax_in
			select @vl_PRFinSeg = @va_PRBouchonMax_in

		/* Prendre le segment s'il est dans la zone du basculement */
		/* et dans la zone du bouchon                              */
		if @vl_PRDebutSeg < @va_PRBouchonMax_in and
		   @vl_PRFinSeg > @va_PRBouchonMin_in
		begin
			/*B calculs des coefficients de translation et */
			/*B extension par segment                      */
			if @vl_PRDebutSeg < @va_PRBouchonMin_in
				select @vl_Translation = 10000 *
						((@va_PRBouchonMin_in +
						 @vl_PRFinSeg)/2 - 
						 @vl_PRDebutPortion)/ 
						 (@vl_PRFinPortion - 
						 @vl_PRDebutPortion),
					@vl_Extension = 10000 *
						(@vl_PRFinSeg -
						 @va_PRBouchonMin_in) / 
						 (@vl_PRFinPortion - 
						 @vl_PRDebutPortion)
			else	select @vl_Translation = 10000 *
						((@vl_PRDebutSeg +
						 @vl_PRFinSeg)/ 2 -
						 @vl_PRDebutPortion)/ 
						 (@vl_PRFinPortion - 
						 @vl_PRDebutPortion),
					@vl_Extension = 10000 *
						(@vl_PRFinSeg -
						 @vl_PRDebutSeg) / 
						 (@vl_PRFinPortion - 
						 @vl_PRDebutPortion)

			/*B ins…re les voies inversees par segment */
			/* on traite en premier la BAU */
			if @va_BAU_in=XDC_VOIE_SENS_INVERSE
				insert	EXP..SYN_BAS_SEG ( evenement_basc, cle_basc, 
					evenement_bou, cle_bou, portion, PR, sens, voie, 
					ordre, translation, extension, niveau)
				values ( @va_NumBascul_in, @va_CleBascul_in,
					@va_NumBouchon_in, @va_CleBouchon_in,
					@vl_Portion, @vl_PRDebutSeg, @va_SensBascul_in,
					6-@vl_NbVoies, XDC_AJOUT,
					@vl_Translation, @vl_Extension, @va_TypeBouchon_in)
	
			/* cas 1 voie */
	
			if @vl_NbVoies=1
			begin
				if @va_VL_in=XDC_VOIE_SENS_INVERSE
					insert	EXP..SYN_BAS_SEG ( evenement_basc, cle_basc, 
						evenement_bou, cle_bou, portion, PR, sens, 
						voie, ordre,translation, extension, niveau)
					values ( @va_NumBascul_in, @va_CleBascul_in,
						@va_NumBouchon_in, @va_CleBouchon_in,
						@vl_Portion, @vl_PRDebutSeg, @va_SensBascul_in,
						6, XDC_AJOUT,
						@vl_Translation, @vl_Extension, @va_TypeBouchon_in)
			end
			else
	
			/* cas 2 voie */
	
			if @vl_NbVoies=2
			begin
				if @va_VR_in=XDC_VOIE_SENS_INVERSE
					insert	EXP..SYN_BAS_SEG ( evenement_basc, cle_basc, 
						evenement_bou, cle_bou, portion, PR, sens, 
						voie, ordre,translation, extension, niveau)
					values ( @va_NumBascul_in, @va_CleBascul_in,
						@va_NumBouchon_in, @va_CleBouchon_in,
						@vl_Portion, @vl_PRDebutSeg, @va_SensBascul_in,
						6, XDC_AJOUT,
						@vl_Translation, @vl_Extension, @va_TypeBouchon_in)

				if @va_VL_in=XDC_VOIE_SENS_INVERSE
					insert	EXP..SYN_BAS_SEG ( evenement_basc, cle_basc, 
						evenement_bou, cle_bou, portion, PR, sens, 
						voie, ordre,translation, extension, niveau)
					values ( @va_NumBascul_in, @va_CleBascul_in,
						@va_NumBouchon_in, @va_CleBouchon_in,
						@vl_Portion, @vl_PRDebutSeg, @va_SensBascul_in,
						5, XDC_AJOUT,
						@vl_Translation, @vl_Extension, @va_TypeBouchon_in)
			end
			else
	
			/* cas 3 voie */
	
			if @vl_NbVoies=3
			begin
				if @va_VR_in=XDC_VOIE_SENS_INVERSE
					insert	EXP..SYN_BAS_SEG ( evenement_basc, cle_basc, 
						evenement_bou, cle_bou, portion, PR, sens, 
						voie, ordre,translation, extension, niveau)
					values ( @va_NumBascul_in, @va_CleBascul_in,
						@va_NumBouchon_in, @va_CleBouchon_in,
						@vl_Portion, @vl_PRDebutSeg, @va_SensBascul_in,
						6, XDC_AJOUT,
						@vl_Translation, @vl_Extension, @va_TypeBouchon_in)

				if @va_VM1_in=XDC_VOIE_SENS_INVERSE
					insert	EXP..SYN_BAS_SEG ( evenement_basc, cle_basc, 
						evenement_bou, cle_bou, portion, PR, sens, 
						voie, ordre,translation, extension, niveau)
					values ( @va_NumBascul_in, @va_CleBascul_in,
						@va_NumBouchon_in, @va_CleBouchon_in,
						@vl_Portion, @vl_PRDebutSeg, @va_SensBascul_in,
						5, XDC_AJOUT,
						@vl_Translation, @vl_Extension, @va_TypeBouchon_in)

				if @va_VL_in=XDC_VOIE_SENS_INVERSE
					insert	EXP..SYN_BAS_SEG ( evenement_basc, cle_basc, 
						evenement_bou, cle_bou, portion, PR, sens, 
						voie, ordre,translation, extension, niveau)
					values ( @va_NumBascul_in, @va_CleBascul_in,
						@va_NumBouchon_in, @va_CleBouchon_in,
						@vl_Portion, @vl_PRDebutSeg, @va_SensBascul_in,
						4, XDC_AJOUT,
						@vl_Translation, @vl_Extension, @va_TypeBouchon_in)
			end
			else
	
			/* cas 4 voie */
	
			if @vl_NbVoies=4
			begin
				if @va_VR_in=XDC_VOIE_SENS_INVERSE
					insert	EXP..SYN_BAS_SEG ( evenement_basc, cle_basc, 
						evenement_bou, cle_bou, portion, PR, sens, 
						voie, ordre,translation, extension, niveau)
					values ( @va_NumBascul_in, @va_CleBascul_in,
						@va_NumBouchon_in, @va_CleBouchon_in,
						@vl_Portion, @vl_PRDebutSeg, @va_SensBascul_in,
						6, XDC_AJOUT,
						@vl_Translation, @vl_Extension, @va_TypeBouchon_in)

				if @va_VM2_in=XDC_VOIE_SENS_INVERSE
					insert	EXP..SYN_BAS_SEG ( evenement_basc, cle_basc, 
						evenement_bou, cle_bou, portion, PR, sens, 
						voie, ordre,translation, extension, niveau)
					values ( @va_NumBascul_in, @va_CleBascul_in,
						@va_NumBouchon_in, @va_CleBouchon_in,
						@vl_Portion, @vl_PRDebutSeg, @va_SensBascul_in,
						5, XDC_AJOUT,
						@vl_Translation, @vl_Extension, @va_TypeBouchon_in)

				if @va_VM1_in=XDC_VOIE_SENS_INVERSE
					insert	EXP..SYN_BAS_SEG ( evenement_basc, cle_basc, 
						evenement_bou, cle_bou, portion, PR, sens, 
						voie, ordre,translation, extension, niveau)
					values ( @va_NumBascul_in, @va_CleBascul_in,
						@va_NumBouchon_in, @va_CleBouchon_in,
						@vl_Portion, @vl_PRDebutSeg, @va_SensBascul_in,
						4, XDC_AJOUT,
						@vl_Translation, @vl_Extension, @va_TypeBouchon_in)

				if @va_VL_in=XDC_VOIE_SENS_INVERSE
					insert	EXP..SYN_BAS_SEG ( evenement_basc, cle_basc, 
						evenement_bou, cle_bou, portion, PR, sens, 
						voie, ordre,translation, extension, niveau)
					values ( @va_NumBascul_in, @va_CleBascul_in,
						@va_NumBouchon_in, @va_CleBouchon_in,
						@vl_Portion, @vl_PRDebutSeg, @va_SensBascul_in,
						3, XDC_AJOUT,
						@vl_Translation, @vl_Extension, @va_TypeBouchon_in)
			end
		end
		fetch Pointeur_segment into @vl_Portion, @vl_PRDebutSeg, @vl_PRDebutPortion, 
				@vl_PRFinPortion, @vl_NbVoies
	end

	return XDC_OK
go
