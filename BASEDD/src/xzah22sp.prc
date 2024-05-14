/*E*/
/*  Fichier : $Id: xzah22sp.prc,v 1.3 2007/09/19 16:06:18 gesconf Exp $      Release : $Revision: 1.3 $        Date : $Date: 2007/09/19 16:06:18 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzah22sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* initialisation et mise a jour des voies pour equext
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.  01/12/94        : Creation      (V 1.1)
* P.N.  29/04/99        : Inhibition des DAI ds Bouchon EVOL18 dem/1731 v1.2
* P.N.  19/09/07	: Requete specifique pour compter le nb d'evt inhibant un même analyseur DEM705 v1.3
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Envoyer les etats des voies de l'analyseur demande
* dans le cas ou au moins une modification a ete 
* provoque par un evenement
*
* Sequence d'appel
* SP    XZAH22_Lire_Voies_Analyseur
*
* Arguments en entree
* XDY_Eqt		va_Numero_in	: numero analyseur
* XDY_Octet		va_Mode_in	: mode des enregistrement a lire
* XDY_Octet		va_NbVoies_in	: nombre de voies de l'analyseur
*
* Arguments en sortie
* XDY_Voie		va_VL_out
* XDY_Voie		va_VM1_out
* XDY_Voie		va_VM2_out
* XDY_Voie		va_VR_out
* XDY_Voie		va_BAU_out
* XDY_int		va_Resultat_out : indique si nouveaux etats de voie
*
* Code retour
* XDC_OK        : pas de probleme
* XDC_ARG_INV   : parametre d'entree null
*
* Conditions d'utilisation
* La procedure est utilisee en interne par XZAH;21
* La presence de la voie BAU est consideree toujours vrai s'il
* y a plus d'une voie surveillees par l'analyseur
*
* Fonction
-------------------------------------------------------*/

use PRC
go
create procedure XZAH;22
	@va_NumEqt_in		T_EQUIPEMENT = null,
	@va_Mode_in		tinyint = null,
	@va_NbVoies_in		tinyint = null,
	@va_VL_out		T_VOIE = null output,
	@va_VM1_out		T_VOIE = null output,
	@va_VM2_out		T_VOIE = null output,
	@va_VR_out		T_VOIE = null output,
	@va_BAU_out		T_VOIE = null output,
	@va_Resultat_out	int = null output
as
	declare @vl_NbEvt int, @vl_Mode tinyint, @vl_VM1 T_VOIE,
		@vl_VM2 T_VOIE, @vl_VR T_VOIE, @vl_BAU T_VOIE,
		@vl_Status int

	/*A controle des parametres d'entree */
	if @va_NumEqt_in = null or @va_Mode_in = null or
	   @va_NbVoies_in = null  or @va_NbVoies_in > 5
		return XDC_ARG_INV

	/*A positionnement des voies d'apres la configuration des voies      */
	/*A de l'analyseur                                                   */
	/*! l'analyseur peut etre situe sur une section ou il y a plus ou    */
	/*! moins de voies que sur la localisation donnee par les evenements */
	if @va_NbVoies_in = 1
		select @va_VR_out = XDC_VOIE_INEXISTANTE, 
			@va_VM2_out = XDC_VOIE_INEXISTANTE, 
			@va_VM1_out = XDC_VOIE_INEXISTANTE,
			@va_VL_out = XDC_VOIE_SENS_NORMAL, 
			@va_BAU_out = XDC_VOIE_INEXISTANTE
	else if @va_NbVoies_in = 2
		select @va_VR_out = XDC_VOIE_INEXISTANTE, 
			@va_VM2_out = XDC_VOIE_INEXISTANTE, 
			@va_VM1_out = XDC_VOIE_INEXISTANTE,
			@va_VL_out = XDC_VOIE_SENS_NORMAL, 
			@va_BAU_out = XDC_VOIE_BAU_NORMALE
	else if @va_NbVoies_in = 3
		select @va_VR_out = XDC_VOIE_SENS_NORMAL, 
			@va_VM2_out = XDC_VOIE_INEXISTANTE, 
			@va_VM1_out = XDC_VOIE_INEXISTANTE,
			@va_VL_out = XDC_VOIE_SENS_NORMAL, 
			@va_BAU_out = XDC_VOIE_BAU_NORMALE
	else if @va_NbVoies_in = 4
		select @va_VR_out = XDC_VOIE_SENS_NORMAL, 
			@va_VM2_out = XDC_VOIE_INEXISTANTE, 
			@va_VM1_out = XDC_VOIE_SENS_NORMAL,
			@va_VL_out = XDC_VOIE_SENS_NORMAL, 
			@va_BAU_out = XDC_VOIE_BAU_NORMALE
	else if @va_NbVoies_in = 5
		select @va_VR_out = XDC_VOIE_SENS_NORMAL, 
			@va_VM2_out = XDC_VOIE_SENS_NORMAL, 
			@va_VM1_out = XDC_VOIE_SENS_NORMAL,
			@va_VL_out = XDC_VOIE_SENS_NORMAL, 
			@va_BAU_out = XDC_VOIE_BAU_NORMALE
	else return XDC_ARG_INV

	/*A rechercher l'etat exact des voies */
	if @va_Mode_in = XZAHC_VOIES_MODIF or @va_Mode_in = XZAHC_VOIES_LUES
	begin
		/*B etat de la voie BAU */
		select 
			@vl_BAU = BAU,
			@va_VL_out = VL,
			@vl_VM1 = VM1,
			@vl_VM2 = VM2,
			@vl_VR = VR
		from EXP..VOI_DAI
		where numero = @va_NumEqt_in and (mode = XZAHC_VOIES_MODIF or
			(@va_Mode_in = XZAHC_VOIES_LUES and mode = @va_Mode_in))

		select 
			
			@vl_NbEvt = count(BAU)
		from EXP..VOI_DAI
		where numero = @va_NumEqt_in and (mode = XZAHC_VOIES_MODIF or
			(@va_Mode_in = XZAHC_VOIES_LUES and mode = @va_Mode_in))


		if @vl_NbEvt = 0
		begin
			select @va_Resultat_out = XDC_NON
			return XDC_OK
		end

		/* dans le cas ou un seul evenement a modifie les etats */
		/* des voies, les nouveaux etats donnes par l'evenement */
		/* sont recuperes si les voies existent pour le radt    */
		if @vl_NbEvt = 1
		begin
			if @va_VM1_out != XDC_VOIE_INEXISTANTE
				select @va_VM1_out = @vl_VM1
			if @va_VM2_out != XDC_VOIE_INEXISTANTE
				select @va_VM2_out = @vl_VM2
			if @va_VR_out != XDC_VOIE_INEXISTANTE
				select @va_VR_out = @vl_VR
			if @va_BAU_out != XDC_VOIE_INEXISTANTE
				select @va_BAU_out = @vl_BAU

			select @va_Resultat_out = XDC_OUI
		end
		/* dans le cas ou plusieurs evenements ont modifies les etats  */
		/* des voies, la regle a applique pour deduire les etats est : */
		/* une voie neutralisee (avec ou sans blocage) est prioritaire */
		/* sur une voie inversee qui est prioritaire sur une voie      */
		/* normale (elle meme prioritaire sur une voie inexistante dans*/
		/* le cas ou il y a augmentation de nombre de voies)           */
		else
		begin
			/*B etat de la voie BAU */
			/*! dans le cas du mode deja lu : recherche d'abord dans */
			/*! les modifications des etats non deja lus             */
			if @va_BAU_out != XDC_VOIE_INEXISTANTE
			begin
				if exists ( select * from EXP..VOI_DAI
					where numero = @va_NumEqt_in and
						mode = XZAHC_VOIES_MODIF and
						(BAU = XDC_VOIE_BLOQUEE_NEUTRALISEE or
						BAU = XDC_VOIE_NEUTRALISEE or 
						BAU = XDC_VOIE_BLOQUEE))
					select @va_BAU_out = XDC_VOIE_NEUTRALISEE
				else if exists ( select * from EXP..VOI_DAI
					where numero = @va_NumEqt_in and
						mode = XZAHC_VOIES_MODIF and
						BAU = XDC_VOIE_SENS_INVERSE)
					select @va_BAU_out = XDC_VOIE_SENS_INVERSE
				else if exists ( select * from EXP..VOI_DAI
					where numero = @va_NumEqt_in and
						mode = XZAHC_VOIES_MODIF and
						BAU = XDC_VOIE_SENS_NORMAL)
					select @va_BAU_out = XDC_VOIE_SENS_NORMAL
				else if @va_Mode_in = XZAHC_VOIES_LUES 
				begin
					if exists ( select * from EXP..VOI_DAI
						where numero = @va_NumEqt_in and
							mode = @va_Mode_in and
							(BAU = XDC_VOIE_BLOQUEE_NEUTRALISEE or
							BAU = XDC_VOIE_NEUTRALISEE or
							BAU = XDC_VOIE_BLOQUEE))
						select @va_BAU_out = XDC_VOIE_NEUTRALISEE
					else if exists ( select * from EXP..VOI_DAI
						where numero = @va_NumEqt_in and
							mode = @va_Mode_in and
							BAU = XDC_VOIE_SENS_INVERSE)
						select @va_BAU_out = XDC_VOIE_SENS_INVERSE
					else if exists ( select * from EXP..VOI_DAI
						where numero = @va_NumEqt_in and
							mode = @va_Mode_in and
							BAU = XDC_VOIE_SENS_NORMAL)
						select @va_BAU_out = XDC_VOIE_SENS_NORMAL
				end
			end

			/*B etat de la voie lente */
			if exists ( select * from EXP..VOI_DAI
				where numero = @va_NumEqt_in and
					mode = XZAHC_VOIES_MODIF and
					(VL = XDC_VOIE_BLOQUEE_NEUTRALISEE or
					VL = XDC_VOIE_NEUTRALISEE or
					VL = XDC_VOIE_BLOQUEE))
				select @va_VL_out = XDC_VOIE_NEUTRALISEE
			else if exists ( select * from EXP..VOI_DAI
				where numero = @va_NumEqt_in and
					mode = XZAHC_VOIES_MODIF and
					VL = XDC_VOIE_SENS_INVERSE)
				select @va_VL_out = XDC_VOIE_SENS_INVERSE
			else if exists ( select * from EXP..VOI_DAI
				where numero = @va_NumEqt_in and
					mode = XZAHC_VOIES_MODIF and
					VL = XDC_VOIE_SENS_NORMAL)
				select @va_VL_out = XDC_VOIE_SENS_NORMAL
			else if @va_Mode_in = XZAHC_VOIES_LUES 
			begin
				if exists ( select * from EXP..VOI_DAI
					where numero = @va_NumEqt_in and
						mode = @va_Mode_in and
						(VL = XDC_VOIE_BLOQUEE_NEUTRALISEE or
						VL = XDC_VOIE_NEUTRALISEE or
						VL = XDC_VOIE_BLOQUEE))
					select @va_VL_out = XDC_VOIE_NEUTRALISEE
				else if exists ( select * from EXP..VOI_DAI
					where numero = @va_NumEqt_in and
						mode = @va_Mode_in and
						VL = XDC_VOIE_SENS_INVERSE)
					select @va_VL_out = XDC_VOIE_SENS_INVERSE
				else if exists ( select * from EXP..VOI_DAI
					where numero = @va_NumEqt_in and
						mode = @va_Mode_in and
						VL = XDC_VOIE_SENS_NORMAL)
					select @va_VL_out = XDC_VOIE_SENS_NORMAL
			end

			/*B etat de la voie rapide */
			if @va_VR_out != XDC_VOIE_INEXISTANTE
			begin
				if exists ( select * from EXP..VOI_DAI
					where numero = @va_NumEqt_in and
						mode = XZAHC_VOIES_MODIF and
						(VR = XDC_VOIE_BLOQUEE_NEUTRALISEE or
						VR = XDC_VOIE_NEUTRALISEE or
						VR = XDC_VOIE_BLOQUEE))
					select @va_VR_out = XDC_VOIE_NEUTRALISEE
				else if exists ( select * from EXP..VOI_DAI
					where numero = @va_NumEqt_in and
						mode = XZAHC_VOIES_MODIF and
						VR = XDC_VOIE_SENS_INVERSE)
					select @va_VR_out = XDC_VOIE_SENS_INVERSE
				else if exists ( select * from EXP..VOI_DAI
					where numero = @va_NumEqt_in and
						mode = XZAHC_VOIES_MODIF and
						VR = XDC_VOIE_SENS_NORMAL)
					select @va_VR_out = XDC_VOIE_SENS_NORMAL
				else if @va_Mode_in = XZAHC_VOIES_LUES 
				begin
					if exists ( select * from EXP..VOI_DAI
						where numero = @va_NumEqt_in and
							mode = @va_Mode_in and
							(VR = XDC_VOIE_BLOQUEE_NEUTRALISEE or
							VR = XDC_VOIE_NEUTRALISEE or
							VR = XDC_VOIE_BLOQUEE))
						select @va_VR_out = XDC_VOIE_NEUTRALISEE
					else if exists ( select * from EXP..VOI_DAI
						where numero = @va_NumEqt_in and
							mode = @va_Mode_in and
							VR = XDC_VOIE_SENS_INVERSE)
						select @va_VR_out = XDC_VOIE_SENS_INVERSE
					else if exists ( select * from EXP..VOI_DAI
						where numero = @va_NumEqt_in and
							mode = @va_Mode_in and
							VR = XDC_VOIE_SENS_NORMAL)
						select @va_VR_out = XDC_VOIE_SENS_NORMAL
				end
			end

			/*B etat de la voie mediane 1 */
			if @va_VM1_out != XDC_VOIE_INEXISTANTE
			begin
				if exists ( select * from EXP..VOI_DAI
					where numero = @va_NumEqt_in and
						mode = XZAHC_VOIES_MODIF and
						(VM1 = XDC_VOIE_BLOQUEE_NEUTRALISEE or
						VM1 = XDC_VOIE_NEUTRALISEE or
						VM1 = XDC_VOIE_BLOQUEE))
					select @va_VM1_out = XDC_VOIE_NEUTRALISEE
				else if exists ( select * from EXP..VOI_DAI
					where numero = @va_NumEqt_in and
						mode = XZAHC_VOIES_MODIF and
						VM1 = XDC_VOIE_SENS_INVERSE)
					select @va_VM1_out = XDC_VOIE_SENS_INVERSE
				else if exists ( select * from EXP..VOI_DAI
					where numero = @va_NumEqt_in and
						mode = XZAHC_VOIES_MODIF and
						VM1 = XDC_VOIE_SENS_NORMAL)
					select @va_VM1_out = XDC_VOIE_SENS_NORMAL
				else if @va_Mode_in = XZAHC_VOIES_LUES 
				begin
					if exists ( select * from EXP..VOI_DAI
						where numero = @va_NumEqt_in and
							mode = @va_Mode_in and
							(VM1 = XDC_VOIE_BLOQUEE_NEUTRALISEE or
							VM1 = XDC_VOIE_NEUTRALISEE or
							VM1 = XDC_VOIE_BLOQUEE))
						select @va_VM1_out = XDC_VOIE_NEUTRALISEE
					else if exists ( select * from EXP..VOI_DAI
						where numero = @va_NumEqt_in and
							mode = @va_Mode_in and
							VM1 = XDC_VOIE_SENS_INVERSE)
						select @va_VM1_out = XDC_VOIE_SENS_INVERSE
					else if exists ( select * from EXP..VOI_DAI
						where numero = @va_NumEqt_in and
							mode = @va_Mode_in and
							VM1 = XDC_VOIE_SENS_NORMAL)
						select @va_VM1_out = XDC_VOIE_SENS_NORMAL
				end
			end

			/*B etat de la voie mediane 2 */
			if @va_VM2_out != XDC_VOIE_INEXISTANTE
			begin
				if exists ( select * from EXP..VOI_DAI
					where numero = @va_NumEqt_in and
						mode = XZAHC_VOIES_MODIF and
						(VM2 = XDC_VOIE_BLOQUEE_NEUTRALISEE or
						VM2 = XDC_VOIE_NEUTRALISEE or
						VM2 = XDC_VOIE_BLOQUEE))
					select @va_VM2_out = XDC_VOIE_NEUTRALISEE
				else if exists ( select * from EXP..VOI_DAI
					where numero = @va_NumEqt_in and
						mode = XZAHC_VOIES_MODIF and
						VM2 = XDC_VOIE_SENS_INVERSE)
					select @va_VM2_out = XDC_VOIE_SENS_INVERSE
				else if exists ( select * from EXP..VOI_DAI
					where numero = @va_NumEqt_in and
						mode = XZAHC_VOIES_MODIF and
						VM2 = XDC_VOIE_SENS_NORMAL)
					select @va_VM2_out = XDC_VOIE_SENS_NORMAL
				else if @va_Mode_in = XZAHC_VOIES_LUES 
				begin
					if exists ( select * from EXP..VOI_DAI
						where numero = @va_NumEqt_in and
							mode = @va_Mode_in and
							(VM2 = XDC_VOIE_BLOQUEE_NEUTRALISEE or
							VM2 = XDC_VOIE_NEUTRALISEE or
							VM2 = XDC_VOIE_BLOQUEE))
						select @va_VM2_out = XDC_VOIE_NEUTRALISEE
					else if exists ( select * from EXP..VOI_DAI
						where numero = @va_NumEqt_in and
							mode = @va_Mode_in and
							VM2 = XDC_VOIE_SENS_INVERSE)
						select @va_VM2_out = XDC_VOIE_SENS_INVERSE
					else if exists ( select * from EXP..VOI_DAI
						where numero = @va_NumEqt_in and
							mode = @va_Mode_in and
							VM2 = XDC_VOIE_SENS_NORMAL)
						select @va_VM2_out = XDC_VOIE_SENS_NORMAL
				end
			end

		end

		select @va_Resultat_out = XDC_OUI
	end
	else if @va_Mode_in = XZAHC_VOIES_SUPP and
		not exists ( select * from EXP..VOI_DAI
			where numero = @va_NumEqt_in and 
				mode != XZAHC_VOIES_SUPP)
	begin
		/* les etats sont ceux de la configuration de l'analyseur */
		select @va_Resultat_out = XDC_OUI
	end
	else if @va_Mode_in = XZAHC_VOIES_SUPP and
		not exists ( select * from EXP..VOI_DAI
			where numero = @va_NumEqt_in and 
				mode = XZAHC_VOIES_MODIF)
	begin
		/* les etats sont ceux deja lus */
		select @vl_Mode = XZAHC_VOIES_LUES
		exec @vl_Status = XZAH;22 @va_NumEqt_in, @vl_Mode,
				@va_NbVoies_in, 
				@va_VL_out output, @va_VM1_out output,
				@va_VM2_out output, @va_VR_out output,
				@va_BAU_out output, @va_Resultat_out output
		return @vl_Status
	end
		/* les etats seront recherches plus tard avec les */
		/* enregistrements en mode modification des etats */
		/* modifies par evenement                         */
	else	select @va_Resultat_out = XDC_NON

	return XDC_OK
go
