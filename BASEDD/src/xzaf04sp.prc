/*E*/
/*  Fichier : $Id: xzaf04sp.prc,v 1.2 1995/11/02 11:48:17 gaborit Exp $      Release : $Revision: 1.2 $        Date : $Date: 1995/11/02 11:48:17 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzaf04sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des synoptiques pour animation
* de l'ihm pour la formation
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	11/01/95	: Creation 
* B.G.	30/10/95	: gestion du sens inverse A57 (1.2)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* SP	XZAF04_Neut_Voies_Evt
* 
* Arguments en entree
* XDY_Evt		va_NumEvt_in
* XDY_Autoroute		va_Autoroute_in
* XDY_PR		va_PR_in
* XDY_Sens		va_Sens_in
* XDY_Octet		va_DebutFin_in
* XDY_Octet		va_VR_in
* XDY_Octet		va_VM2_in_in
* XDY_Octet		va_VM1_in_in
* XDY_Octet		va_VL_in
* XDY_Octet		va_BAU_in
* XDY_Octet		va_VR_I_in
* XDY_Octet		va_VM2_I_in_in
* XDY_Octet		va_VM1_I_in_in
* XDY_Octet		va_VL_I_in
* XDY_Octet		va_BAU_I_in
* 
* Arguments en sortie
* 
* Service rendu
* Retourne la liste des voies neutralisees pour
* l'animation IHM pour un evenement
*
* Code retour
* XDC_OK
* XDC_NOK	
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
*
---------------------------------------------------*/

use PRC
go

create table #SYN_RAB_SEG
(
	evenement		int,
	cle			tinyint,
	debut_fin		tinyint,
	portion			smallint,
	translation		int,
	nvoie1			tinyint,
	vvoie1			tinyint,
	nvoie2			tinyint,
	vvoie2			tinyint,
	nvoie3			tinyint,
	vvoie3			tinyint,
	nvoie4			tinyint,
	vvoie4			tinyint,
	nvoie5			tinyint,
	vvoie5			tinyint,
	nvoie6			tinyint,
	vvoie6			tinyint,
	nvoie7			tinyint,
	vvoie7			tinyint,
	nvoie8			tinyint,
	vvoie8			tinyint,
	nvoie9			tinyint,
	vvoie9			tinyint,
	nvoie10			tinyint,
	vvoie10			tinyint
)
go

create procedure  XZAF;04
	@va_NumEvt_in		int		= null,
	@va_CleEvt_in		tinyint		= null,
	@va_Autoroute_in	T_AUTOROUTE	= null,
	@va_PR_in		T_PR		= null,
	@va_Sens_in		T_SENS		= null,
	@va_DebutFin_in		tinyint		= null,
	@va_VR_in               tinyint		= null,
	@va_VM2_in           	tinyint		= null,
	@va_VM1_in           	tinyint		= null,
	@va_VL_in               tinyint		= null,
	@va_BAU_in              tinyint		= null,
	@va_VR_I_in             tinyint		= null,
	@va_VM2_I_in         	tinyint		= null,
	@va_VM1_I_in         	tinyint		= null,
	@va_VL_I_in             tinyint		= null,
	@va_BAU_I_in            tinyint		= null
as

/*A D≈claration des variables locales */

declare @vl_Portion 		smallint,
	@vl_Translation 	int,
	@vl_NombreSud tinyint,
	@vl_NombreNord tinyint,
	@vl_nvoie1 tinyint ,
	@vl_vvoie1 tinyint ,
	@vl_nvoie2 tinyint ,
	@vl_vvoie2 tinyint ,
	@vl_nvoie3 tinyint ,
	@vl_vvoie3 tinyint ,
	@vl_nvoie4 tinyint ,
	@vl_vvoie4 tinyint ,
	@vl_nvoie5 tinyint ,
	@vl_vvoie5 tinyint ,
	@vl_nvoie6 tinyint ,
	@vl_vvoie6 tinyint ,
	@vl_nvoie7 tinyint ,
	@vl_vvoie7 tinyint ,
	@vl_nvoie8 tinyint ,
	@vl_vvoie8 tinyint ,
	@vl_nvoie9 tinyint ,
	@vl_vvoie9 tinyint ,
	@vl_nvoie10 tinyint ,
	@vl_vvoie10 tinyint ,
	@vl_voie tinyint
		
	/*A Test des informations nulles */
	if	@va_DebutFin_in=null or
		@va_Autoroute_in=null or
		@va_PR_in=null or
		@va_Sens_in=null or
		@va_VR_in=null or 
		@va_VM2_in=null or
		@va_VM1_in=null or
		@va_VL_in=null or
		@va_BAU_in=null or
		@va_VR_I_in=null or
		@va_VM2_I_in=null or
		@va_VM1_I_in=null or
		@va_VL_I_in=null or
		@va_BAU_I_in=null
	return XDC_NOK


	/*A Calcule portion, et translation */
	select	@vl_Portion=numero,
		@vl_Translation=10000*(@va_PR_in-PR_debut)/(PR_fin-PR_debut)
	from CFG..RES_POR
	where	autoroute=@va_Autoroute_in and
		PR_debut<@va_PR_in and
		PR_fin>=@va_PR_in
		
	if @@rowcount<>1
		return XDC_NOK


	/*A Initialise les variables voies */
	select	@vl_nvoie1 = XDC_VOIE_INEXISTANTE,
		@vl_vvoie1 = XDC_VOIE_INEXISTANTE,
		@vl_nvoie2 = XDC_VOIE_INEXISTANTE,
		@vl_vvoie2 = XDC_VOIE_INEXISTANTE,
		@vl_nvoie3 = XDC_VOIE_INEXISTANTE,
		@vl_vvoie3 = XDC_VOIE_INEXISTANTE,
		@vl_nvoie4 = XDC_VOIE_INEXISTANTE,
		@vl_vvoie4 = XDC_VOIE_INEXISTANTE,
		@vl_nvoie5 = XDC_VOIE_INEXISTANTE,
		@vl_vvoie5 = XDC_VOIE_INEXISTANTE,
		@vl_nvoie6 = XDC_VOIE_INEXISTANTE,
		@vl_vvoie6 = XDC_VOIE_INEXISTANTE,
		@vl_nvoie7 = XDC_VOIE_INEXISTANTE,
		@vl_vvoie7 = XDC_VOIE_INEXISTANTE,
		@vl_nvoie8 = XDC_VOIE_INEXISTANTE,
		@vl_vvoie8 = XDC_VOIE_INEXISTANTE,
		@vl_nvoie9 = XDC_VOIE_INEXISTANTE,
		@vl_vvoie9 = XDC_VOIE_INEXISTANTE,
		@vl_nvoie10 = XDC_VOIE_INEXISTANTE,
		@vl_vvoie10 = XDC_VOIE_INEXISTANTE

	/*A Convertit les arguments voies */
	if	@va_VR_in=XDC_VOIE_INCONNUE or
		@va_VR_in=XDC_VOIE_BAU_NORMALE or
		@va_VR_in=XDC_VOIE_BLOQUEE
	select	@va_VR_in=XDC_VOIE_INEXISTANTE
	
	if	@va_VR_in=XDC_VOIE_BLOQUEE_NEUTRALISEE
	select	@va_VR_in=XDC_VOIE_NEUTRALISEE

	if	@va_VM2_in=XDC_VOIE_INCONNUE or
		@va_VM2_in=XDC_VOIE_BAU_NORMALE or
		@va_VM2_in=XDC_VOIE_BLOQUEE
	select	@va_VM2_in=XDC_VOIE_INEXISTANTE
	
	if	@va_VM2_in=XDC_VOIE_BLOQUEE_NEUTRALISEE
	select	@va_VM2_in=XDC_VOIE_NEUTRALISEE

	if	@va_VM1_in=XDC_VOIE_INCONNUE or
		@va_VM1_in=XDC_VOIE_BAU_NORMALE or
		@va_VM1_in=XDC_VOIE_BLOQUEE
	select	@va_VM1_in=XDC_VOIE_INEXISTANTE
	
	if	@va_VM1_in=XDC_VOIE_BLOQUEE_NEUTRALISEE
	select	@va_VM1_in=XDC_VOIE_NEUTRALISEE

	if	@va_VL_in=XDC_VOIE_INCONNUE or
		@va_VL_in=XDC_VOIE_BAU_NORMALE or
		@va_VL_in=XDC_VOIE_BLOQUEE
	select	@va_VL_in=XDC_VOIE_INEXISTANTE
	
	if	@va_VL_in=XDC_VOIE_BLOQUEE_NEUTRALISEE
	select	@va_VL_in=XDC_VOIE_NEUTRALISEE

	if	@va_BAU_in=XDC_VOIE_INCONNUE or
		@va_BAU_in=XDC_VOIE_BAU_NORMALE or
		@va_BAU_in=XDC_VOIE_BLOQUEE
	select	@va_BAU_in=XDC_VOIE_INEXISTANTE
	
	if	@va_BAU_in=XDC_VOIE_BLOQUEE_NEUTRALISEE
	select	@va_BAU_in=XDC_VOIE_NEUTRALISEE

	if	@va_VR_I_in=XDC_VOIE_INCONNUE or
		@va_VR_I_in=XDC_VOIE_BAU_NORMALE or
		@va_VR_I_in=XDC_VOIE_BLOQUEE
	select	@va_VR_I_in=XDC_VOIE_INEXISTANTE
	
	if	@va_VR_I_in=XDC_VOIE_BLOQUEE_NEUTRALISEE
	select	@va_VR_I_in=XDC_VOIE_NEUTRALISEE

	if	@va_VM2_I_in=XDC_VOIE_INCONNUE or
		@va_VM2_I_in=XDC_VOIE_BAU_NORMALE or
		@va_VM2_I_in=XDC_VOIE_BLOQUEE
	select	@va_VM2_I_in=XDC_VOIE_INEXISTANTE
	
	if	@va_VM2_I_in=XDC_VOIE_BLOQUEE_NEUTRALISEE
	select	@va_VM2_I_in=XDC_VOIE_NEUTRALISEE

	if	@va_VM1_I_in=XDC_VOIE_INCONNUE or
		@va_VM1_I_in=XDC_VOIE_BAU_NORMALE or
		@va_VM1_I_in=XDC_VOIE_BLOQUEE
	select	@va_VM1_I_in=XDC_VOIE_INEXISTANTE
	
	if	@va_VM1_I_in=XDC_VOIE_BLOQUEE_NEUTRALISEE
	select	@va_VM1_I_in=XDC_VOIE_NEUTRALISEE

	if	@va_VL_I_in=XDC_VOIE_INCONNUE or
		@va_VL_I_in=XDC_VOIE_BAU_NORMALE or
		@va_VL_I_in=XDC_VOIE_BLOQUEE
	select	@va_VL_I_in=XDC_VOIE_INEXISTANTE
	
	if	@va_VL_I_in=XDC_VOIE_BLOQUEE_NEUTRALISEE
	select	@va_VL_I_in=XDC_VOIE_NEUTRALISEE

	if	@va_BAU_I_in=XDC_VOIE_INCONNUE or
		@va_BAU_I_in=XDC_VOIE_BAU_NORMALE or
		@va_BAU_I_in=XDC_VOIE_BLOQUEE
	select	@va_BAU_I_in=XDC_VOIE_INEXISTANTE
	
	if	@va_BAU_I_in=XDC_VOIE_BLOQUEE_NEUTRALISEE
	select	@va_BAU_I_in=XDC_VOIE_NEUTRALISEE


	/*A Calcule le nombre de voies pour chaque sens */
	select @vl_NombreSud=nombre_de_voies
	from CFG..RES_SEG
	where	portion=@vl_Portion and
		sens=XDC_SENS_SUD and
		PR_debut<=@va_PR_in
	having	PR_debut=max(PR_debut) and
		sens=XDC_SENS_SUD and
		PR_debut<=@va_PR_in and
		portion=@vl_Portion

	select @vl_NombreNord=nombre_de_voies
	from CFG..RES_SEG
	where	portion=@vl_Portion and
		sens=XDC_SENS_NORD and
		PR_debut<=@va_PR_in
	having	PR_debut=max(PR_debut) and
		sens=XDC_SENS_NORD and
		PR_debut<=@va_PR_in and
		portion=@vl_Portion

	/*A Affectation des voies si le sens est SUD et s'il s'agit
    	    d'un objet ponctuel ou d'un d≈but */

	if	@va_Sens_in=XDC_SENS_SUD and
		@va_DebutFin_in<>XZAHC_Fin
	begin

		/* Cas 1 voie sud */

		if @vl_NombreSud=1
		select	@vl_nvoie6=@va_VL_in,
			@vl_nvoie7=@va_BAU_in
		else

		/* Cas 2 voies sud */

		if @vl_NombreSud=2
		select	@vl_nvoie6=@va_VR_in,
			@vl_nvoie7=@va_VL_in,
			@vl_nvoie8=@va_BAU_in
		else

		/* Cas 3 voies sud */

		if @vl_NombreSud=3
		select	@vl_nvoie6=@va_VR_in,
			@vl_nvoie7=@va_VM1_in,
			@vl_nvoie8=@va_VL_in,
			@vl_nvoie9=@va_BAU_in
		else

		/* Cas 4 voies sud */

		if @vl_NombreSud=4
		select	@vl_nvoie6=@va_VR_in,
			@vl_nvoie7=@va_VM2_in,
			@vl_nvoie8=@va_VM1_in,
			@vl_nvoie9=@va_VL_in,
			@vl_nvoie10=@va_BAU_in

		/* Cas 1 voie nord */

		if @vl_NombreNord=1
		select	@vl_nvoie5=@va_VL_I_in,
			@vl_nvoie4=@va_BAU_I_in
		else

		/* Cas 2 voies nord */

		if @vl_NombreNord=2
		select	@vl_nvoie5=@va_VR_I_in,
			@vl_nvoie4=@va_VL_I_in,
			@vl_nvoie3=@va_BAU_I_in
		else

		/* Cas 3 voies nord */

		if @vl_NombreNord=3
		select	@vl_nvoie5=@va_VR_I_in,
			@vl_nvoie4=@va_VM1_I_in,
			@vl_nvoie3=@va_VL_I_in,
			@vl_nvoie2=@va_BAU_I_in
		else

		/* Cas 4 voies nord */

		if @vl_NombreNord=4
		select	@vl_nvoie5=@va_VR_I_in,
			@vl_nvoie4=@va_VM2_I_in,
			@vl_nvoie3=@va_VM1_I_in,
			@vl_nvoie2=@va_VL_I_in,
			@vl_nvoie1=@va_BAU_I_in

		/*B Affectation des voies avant si le sens est SUD et s'il s'agit
    		    d'un d≈but */
		if	@va_DebutFin_in=XZAHC_Debut
		begin

			/* Cas 1 voie sud */

			if @vl_NombreSud>=1
			select	@vl_vvoie6=XDC_VOIE_SENS_NORMAL

			/* Cas 2 voies sud */

			if @vl_NombreSud>=2
			select	@vl_vvoie7=XDC_VOIE_SENS_NORMAL

			/* Cas 3 voies sud */

			if @vl_NombreSud>=3
			select	@vl_vvoie8=XDC_VOIE_SENS_NORMAL

			/* Cas 4 voies sud */

			if @vl_NombreSud>=4
			select	@vl_vvoie9=XDC_VOIE_SENS_NORMAL

			/* Cas 1 voie nord */

			if @vl_NombreNord>=1
			select	@vl_vvoie5=XDC_VOIE_SENS_NORMAL

			/* Cas 2 voies nord */

			if @vl_NombreNord>=2
			select	@vl_vvoie4=XDC_VOIE_SENS_NORMAL

			/* Cas 3 voies nord */

			if @vl_NombreNord>=3
			select	@vl_vvoie3=XDC_VOIE_SENS_NORMAL

			/* Cas 4 voies nord */

			if @vl_NombreNord>=4
			select	@vl_vvoie2=XDC_VOIE_SENS_NORMAL
		end
	end

	/*A Affectation des voies si le sens est NORD et s'il s'agit
    	    d'un objet ponctuel ou d'un d≈but */

	else
	if	@va_Sens_in=XDC_SENS_NORD and
		@va_DebutFin_in<>XZAHC_Fin

	begin

		/* cas 1 voie sud */

		if @vl_NombreSud=1
			select	@vl_vvoie6=@va_VL_I_in,
				@vl_vvoie7=@va_BAU_I_in
		else

		/* Cas 2 voies sud */

		if @vl_NombreSud=2
			select	@vl_vvoie6=@va_VR_I_in,
				@vl_vvoie7=@va_VL_I_in,
				@vl_vvoie8=@va_BAU_I_in
		else

		/* Cas 3 voies sud */

		if @vl_NombreSud=3
			select	@vl_vvoie6=@va_VR_I_in,
				@vl_vvoie7=@va_VM1_I_in,
				@vl_vvoie8=@va_VL_I_in,
				@vl_vvoie9=@va_BAU_I_in
		else

		/* Cas 4 voies sud */

		if @vl_NombreSud=4
			select	@vl_vvoie6=@va_VR_I_in,
				@vl_vvoie7=@va_VM2_I_in,
				@vl_vvoie8=@va_VM1_I_in,
				@vl_vvoie9=@va_VL_I_in,
				@vl_vvoie10=@va_BAU_I_in

		/* Cas 1 voie nord */

		if @vl_NombreNord=1
			select	@vl_vvoie5=@va_VL_in,
				@vl_vvoie4=@va_BAU_in
		else

		/* Cas 2 voies nord */

		if @vl_NombreNord=2
			select	@vl_vvoie5=@va_VR_in,
				@vl_vvoie4=@va_VL_in,
				@vl_vvoie3=@va_BAU_in
		else

		/* Cas 3 voies nord */

		if @vl_NombreNord=3
			select	@vl_vvoie5=@va_VR_in,
				@vl_vvoie4=@va_VM1_in,
				@vl_vvoie3=@va_VL_in,
				@vl_vvoie2=@va_BAU_in
		else

		/* Cas 4 voies nord */

		if @vl_NombreNord=4
			select	@vl_vvoie5=@va_VR_in,
				@vl_vvoie4=@va_VM2_in,
				@vl_vvoie3=@va_VM1_in,
				@vl_vvoie2=@va_VL_in,
				@vl_vvoie1=@va_BAU_in

		/*B Affectation des voies avant si le sens est 
    		     NORD et s'il s'agit d'un d≈but */
		if @va_DebutFin_in=XZAHC_Debut
		begin

			/* Cas 1 voie sud */

			if @vl_NombreSud>=1
			select	@vl_nvoie6=XDC_VOIE_SENS_NORMAL

			/* Cas 2 voies sud */

			if @vl_NombreSud>=2
			select	@vl_nvoie7=XDC_VOIE_SENS_NORMAL

			/* Cas 3 voies sud */

			if @vl_NombreSud>=3
			select	@vl_nvoie8=XDC_VOIE_SENS_NORMAL

			/* Cas 4 voies sud */

			if @vl_NombreSud>=4
			select	@vl_nvoie9=XDC_VOIE_SENS_NORMAL

			/* Cas 1 voie nord */

			if @vl_NombreNord>=1
			select	@vl_nvoie5=XDC_VOIE_SENS_NORMAL

			/* Cas 2 voies nord */

			if @vl_NombreNord>=2
			select	@vl_nvoie4=XDC_VOIE_SENS_NORMAL

			/* Cas 3 voies nord */

			if @vl_NombreNord>=3
			select	@vl_nvoie3=XDC_VOIE_SENS_NORMAL

			/* Cas 4 voies nord */

			if @vl_NombreNord>=4
			select	@vl_nvoie2=XDC_VOIE_SENS_NORMAL
		end
	end

	/*A Affectation des voies si le sens est SUD et 
	     s'il s'agit d'une fin */

	else
	if @va_Sens_in=XDC_SENS_SUD and
	   @va_DebutFin_in=XZAHC_Fin
	begin

		/* cas 1 voie sud */

		if @vl_NombreSud=1
		select	@vl_vvoie6=@va_VL_in,
			@vl_vvoie7=@va_BAU_in
		else

		/* Cas 2 voies sud */

		if @vl_NombreSud=2
		select	@vl_vvoie6=@va_VR_in,
			@vl_vvoie7=@va_VL_in,
			@vl_vvoie8=@va_BAU_in
		else

		/* Cas 3 voies sud */

		if @vl_NombreSud=3
		select	@vl_vvoie6=@va_VR_in,
			@vl_vvoie7=@va_VM1_in,
			@vl_vvoie8=@va_VL_in,
			@vl_vvoie9=@va_BAU_in
		else

		/* Cas 4 voies sud */

		if @vl_NombreSud=4
		select	@vl_vvoie6=@va_VR_in,
			@vl_vvoie7=@va_VM2_in,
			@vl_vvoie8=@va_VM1_in,
			@vl_vvoie9=@va_VL_in,
			@vl_vvoie10=@va_BAU_in

		/* Cas 1 voie nord */

		if @vl_NombreNord=1
		select	@vl_vvoie5=@va_VL_I_in,
			@vl_vvoie4=@va_BAU_I_in
		else

		/* Cas 2 voies nord */

		if @vl_NombreNord=2
		select	@vl_vvoie5=@va_VR_I_in,
			@vl_vvoie4=@va_VL_I_in,
			@vl_vvoie3=@va_BAU_I_in
		else

		/* Cas 3 voies nord */

		if @vl_NombreNord=3
		select	@vl_vvoie5=@va_VR_I_in,
			@vl_vvoie4=@va_VM1_I_in,
			@vl_vvoie3=@va_VL_I_in,
			@vl_vvoie2=@va_BAU_I_in
		else

		/* Cas 4 voies nord */

		if @vl_NombreNord=4
		select	@vl_vvoie5=@va_VR_I_in,
			@vl_vvoie4=@va_VM2_I_in,
			@vl_vvoie3=@va_VM1_I_in,
			@vl_vvoie2=@va_VL_I_in,
			@vl_vvoie1=@va_BAU_I_in


		/*B Affectation des voies apres si le sens 
		    est SUD et s'il s'agit d'une fin */
    
		/* Cas 1 voie sud */

		if @vl_NombreSud>=1
		select	@vl_nvoie6=XDC_VOIE_SENS_NORMAL

		/* Cas 2 voies sud */

		if @vl_NombreSud>=2
		select	@vl_nvoie7=XDC_VOIE_SENS_NORMAL

		/* Cas 3 voies sud */

		if @vl_NombreSud>=3
		select	@vl_nvoie8=XDC_VOIE_SENS_NORMAL

		/* Cas 4 voies sud */

		if @vl_NombreSud>=4
		select	@vl_nvoie9=XDC_VOIE_SENS_NORMAL

		/* Cas 1 voie nord */

		if @vl_NombreNord>=1
		select	@vl_nvoie5=XDC_VOIE_SENS_NORMAL

		/* Cas 2 voies nord */

		if @vl_NombreNord>=2
		select	@vl_nvoie4=XDC_VOIE_SENS_NORMAL

		/* Cas 3 voies nord */

		if @vl_NombreNord>=3
		select	@vl_nvoie3=XDC_VOIE_SENS_NORMAL

		/* Cas 4 voies nord */

		if @vl_NombreNord>=4
		select	@vl_nvoie2=XDC_VOIE_SENS_NORMAL
	end

	/*A Affectation des voies si le sens est NORD 
	    et s'il s'agit d'une fin */

	else
	if	@va_Sens_in=XDC_SENS_NORD and
		@va_DebutFin_in=XZAHC_Fin

	begin

		/* Cas 1 voie sud */

		if @vl_NombreSud=1
		select	@vl_nvoie6=@va_VL_I_in,
			@vl_nvoie7=@va_BAU_I_in
		else

		/* Cas 2 voies sud */

		if @vl_NombreSud=2
		select	@vl_nvoie6=@va_VR_I_in,
			@vl_nvoie7=@va_VL_I_in,
			@vl_nvoie8=@va_BAU_I_in
		else

		/* Cas 3 voies sud */

		if @vl_NombreSud=3
		select	@vl_nvoie6=@va_VR_I_in,
			@vl_nvoie7=@va_VM1_I_in,
			@vl_nvoie8=@va_VL_I_in,
			@vl_nvoie9=@va_BAU_I_in
		else

		/* Cas 4 voies sud */

		if @vl_NombreSud=4
		select	@vl_nvoie6=@va_VR_I_in,
			@vl_nvoie7=@va_VM2_I_in,
			@vl_nvoie8=@va_VM1_I_in,
			@vl_nvoie9=@va_VL_I_in,
			@vl_nvoie10=@va_BAU_I_in

		/* Cas 1 voie nord */

		if @vl_NombreNord=1
		select	@vl_nvoie5=@va_VL_in,
			@vl_nvoie4=@va_BAU_in
		else

		/* Cas 2 voies nord */

		if @vl_NombreNord=2
		select	@vl_nvoie5=@va_VR_in,
			@vl_nvoie4=@va_VL_in,
			@vl_nvoie3=@va_BAU_in
		else

		/* Cas 3 voies nord */

		if @vl_NombreNord=3
		select	@vl_nvoie5=@va_VR_in,
			@vl_nvoie4=@va_VM1_in,
			@vl_nvoie3=@va_VL_in,
			@vl_nvoie2=@va_BAU_in
		else

		/* Cas 4 voies nord */

		if @vl_NombreNord=4
		select	@vl_nvoie5=@va_VR_in,
			@vl_nvoie4=@va_VM2_in,
			@vl_nvoie3=@va_VM1_in,
			@vl_nvoie2=@va_VL_in,
			@vl_nvoie1=@va_BAU_in

		/*B Affectation des voies apres si le sens est NORD 
    		    et s'il s'agit d'une fin */
    
		/* Cas 1 voie sud */

		if @vl_NombreSud>=1
		select	@vl_vvoie6=XDC_VOIE_SENS_NORMAL

		/* Cas 2 voies sud */

		if @vl_NombreSud>=2
		select	@vl_vvoie7=XDC_VOIE_SENS_NORMAL

		/* Cas 3 voies sud */

		if @vl_NombreSud>=3
		select	@vl_vvoie8=XDC_VOIE_SENS_NORMAL

		/* Cas 4 voies sud */

		if @vl_NombreSud>=4
		select	@vl_vvoie9=XDC_VOIE_SENS_NORMAL

		/* Cas 1 voie nord */

		if @vl_NombreNord>=1
		select	@vl_vvoie5=XDC_VOIE_SENS_NORMAL

		/* Cas 2 voies nord */

		if @vl_NombreNord>=2
		select	@vl_vvoie4=XDC_VOIE_SENS_NORMAL

		/* Cas 3 voies nord */

		if @vl_NombreNord>=3
		select	@vl_vvoie3=XDC_VOIE_SENS_NORMAL

		/* Cas 4 voies nord */

		if @vl_NombreNord>=4
		select	@vl_vvoie2=XDC_VOIE_SENS_NORMAL
	end

	/*A Toutes les voies sont positionn≈es en variable 
	    locale-> mise en table */

/*A Si l'autoroute est inversee (A57) echange des voies (1<-->10,2<-->9, etc) */

	if exists (	select * from CFG..RES_AUT_INV
			where	numero=@va_Autoroute_in
			  and	inversee=XDC_PR_INV )
	begin
		select	@vl_voie	= @vl_nvoie1
		select	@vl_nvoie1	= @vl_nvoie10
		select	@vl_nvoie10	= @vl_voie
		
		select	@vl_voie	= @vl_nvoie2
		select	@vl_nvoie2	= @vl_nvoie9
		select	@vl_nvoie9	= @vl_voie
		
		select	@vl_voie	= @vl_nvoie3
		select	@vl_nvoie3	= @vl_nvoie8
		select	@vl_nvoie8	= @vl_voie
		
		select	@vl_voie	= @vl_nvoie4
		select	@vl_nvoie4	= @vl_nvoie7
		select	@vl_nvoie7	= @vl_voie
		
		select	@vl_voie	= @vl_nvoie5
		select	@vl_nvoie5	= @vl_nvoie6
		select	@vl_nvoie6	= @vl_voie
		
		select	@vl_voie	= @vl_vvoie1
		select	@vl_vvoie1	= @vl_vvoie10
		select	@vl_vvoie10	= @vl_voie
		
		select	@vl_voie	= @vl_vvoie2
		select	@vl_vvoie2	= @vl_vvoie9
		select	@vl_vvoie9	= @vl_voie
		
		select	@vl_voie	= @vl_vvoie3
		select	@vl_vvoie3	= @vl_vvoie8
		select	@vl_vvoie8	= @vl_voie
		
		select	@vl_voie	= @vl_vvoie4
		select	@vl_vvoie4	= @vl_vvoie7
		select	@vl_vvoie7	= @vl_voie
		
		select	@vl_voie	= @vl_vvoie5
		select	@vl_vvoie5	= @vl_vvoie6
		select	@vl_vvoie6	= @vl_voie
		
		/*! si on se trouve sur l'A57, on renvoie translation entre -10001 et -1 */
		
		select @vl_Translation=@vl_Translation-10001
	end

	insert #SYN_RAB_SEG (
		evenement,
		cle,
		debut_fin,
		portion,
		translation,
		nvoie1,
		vvoie1,
		nvoie2,
		vvoie2,
		nvoie3,
		vvoie3,
		nvoie4,
		vvoie4,
		nvoie5,
		vvoie5,
		nvoie6,
		vvoie6,
		nvoie7,
		vvoie7,
		nvoie8,
		vvoie8,
		nvoie9,
		vvoie9,
		nvoie10,
		vvoie10
		)
	values	(
		@va_NumEvt_in,
		@va_CleEvt_in,
		convert(bit,@va_DebutFin_in),
		@vl_Portion,
		@vl_Translation,
		@vl_nvoie1,
		@vl_vvoie1,
		@vl_nvoie2,
		@vl_vvoie2,
		@vl_nvoie3,
		@vl_vvoie3,
		@vl_nvoie4,
		@vl_vvoie4,
		@vl_nvoie5,
		@vl_vvoie5,
		@vl_nvoie6,
		@vl_vvoie6,
		@vl_nvoie7,
		@vl_vvoie7,
		@vl_nvoie8,
		@vl_vvoie8,
		@vl_nvoie9,
		@vl_vvoie9,
		@vl_nvoie10,
		@vl_vvoie10
		)

	return XDC_OK
go

drop table #SYN_RAB_SEG
go
