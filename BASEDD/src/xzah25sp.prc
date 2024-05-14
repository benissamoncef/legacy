/*E*/
/*  Fichier : @(#)xzah25sp.prc	1.3      Release : 1.3        Date : 11/08/95
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzah25sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* initialisation et mise a jour des voies pour equext
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.  08/12/94        : Creation      (V 1.1)
* C.T.  06/01/95        : Supprimer les tables temporaires      
*                         rajout la creation de 2 tables de 
*                         travail (V 1.2)
* B.G.	07/11/95	: modif test sur PR pour A57 et remplacement
			  'where !=' par 'where not in select' (1.3)
* C.P.	07/01/98	: Correction gestion multi-sites dans 
*			  les procs (V 1.4) (dem / 1536)
* P.N.  29/01/99        : Inhibition des DAI ds Bouchon EVOL18 dem/1731 v1.5 
* P.C   25/11/99        : Correction pour que les DAI inhibees aient un PR de debut ou de fin dans la zone 
* P.C   30/11/99        : Les DAI dont la zone de couverture englobe le bouchon doivent aussi etre inhibees
* P.N   03/12/99        : Suppression de l'inhibition des RADT pour les bouchons v1.7
* Hilmarcher 21/02/05   : voie BAU inhibee pour les DAI dans les bouchons DEM421 v1.8-v1.9
* P.N   24/06/05        : Ajout d'un arg pour ne pas supprimer les config de voie dans le second appel pour les basculement v1.10 DEM480
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Mise a jour l'etat des voies pour un evenement
*
* Sequence d'appel
* SP    XZAH25_Ecrire_Voies_Fmc
*
* Arguments en entree
* XDY_Entier	va_NumEvt_in
* XDY_Octet	va_CleEvt_in
* XDY_Autoroute	va_Autoroute_in
* XDY_PR	va_PRDebut_in
* XDY_PR	va_PRFin_in
* XDY_Sens	va_Sens_in
* XDY_Voie	va_VL_in
* XDY_Voie	va_VM1_in
* XDY_Voie	va_VM2_in
* XDY_Voie	va_VR_in
* XDY_Voie	va_BAU_in
* XDY_Voie	va_BAU_in
* XDYBooleen	va_basc_in
*
* Arguments en sortie
*
* Code retour
* XDC_OK        : pas de probleme
* XDC_NOK       : probleme a l'insertion dans la base de sauvegarde
*                 ou donnee non trouvee
* XDC_ARG_INV   : parametre d'entree null
*
* Conditions d'utilisation
* La procedure est appelee par XZAH;20
* Un evenement doit rester sur la meme chaussee (conserver le sens)
*
* Fonction
* insert ou update de VOI_DAI et VOI_RAD
-------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'DAI_EVT' and type = 'U')
	drop table DAI_EVT
go
if exists (select * from sysobjects where name = 'RAD_EVT' and type = 'U')
	drop table RAD_EVT
go

create table DAI_EVT
(
	numero                          smallint              not null,
	numero_analyseur                tinyint               not null,
	nombre_de_voies                 tinyint               not null,
	maitre                          smallint              not null,
	spid                            int                 not null
)
go

create table RAD_EVT
(
	numero                          smallint              not null,
	nombre_de_voies                 tinyint               not null,
	spid                            int                 not null
)
go

grant all on DAI_EVT to public
grant all on RAD_EVT to public
go

create procedure XZAH;25
	@va_NumEvt_in		int = null,
	@va_CleEvt_in		tinyint = null,
	@va_Autoroute_in	T_AUTOROUTE = null,
	@va_PRDebut_in		T_PR = null,
	@va_PRFin_in		T_PR = null,
	@va_Sens_in		T_SENS = null,
	@va_VL_in		T_VOIE = null,
	@va_VM1_in		T_VOIE = null,
	@va_VM2_in		T_VOIE = null,
	@va_VR_in		T_VOIE = null,
	@va_BAU_in		T_VOIE = null,
	@va_basc_in		bit = null
as
	declare @vl_Numero T_EQUIPEMENT, 
		@vl_NumEqt T_EQUIPEMENT, @vl_TypeEqt tinyint,
		@vl_NumMaitre T_EQUIPEMENT, @vl_TypeMaitre tinyint,
		@vl_NomMaitre char(10), @vl_NumDai T_EQUIPEMENT,
		@vl_NumeroAnal tinyint, @vl_NbVoies tinyint,
		@vl_MachineTrouvee bit, @vl_Site T_SITE,
		@vl_PR T_PR,
		@vl_Classe tinyint

	/*A controler les arguments en entree */
	if @va_NumEvt_in = null or @va_CleEvt_in = null or
	   @va_Autoroute_in = null or @va_PRDebut_in = null or
	   @va_PRFin_in = null or @va_Sens_in = null or
	   @va_VL_in = null or @va_VM1_in = null or
	   @va_VM2_in = null or @va_VR_in = null or
	   @va_BAU_in = null
		return XDC_ARG_INV

	/* recherche du site local */
	select @vl_Site = numero 
        from CFG..RES_DIS
        where serveur = @@servername

        if @vl_Site is null
                return XDC_NOK
	

	/*A mise dans l'ordre PRDebut<=PRFin pour A57 et pb config DAI */
	if @va_PRDebut_in>@va_PRFin_in
	begin
		select @vl_PR=@va_PRDebut_in
		select @va_PRDebut_in=@va_PRFin_in
		select @va_PRFin_in=@vl_PR
	end
	
	/*A recherche les DAI concernes par cet evenement */
	/*A sur le site local                             */
	insert DAI_EVT (numero, numero_analyseur, nombre_de_voies, maitre, spid)
	select
		CFG..EQT_ANA.numero,
		CFG..EQT_ANA.numero_analyseur,
		CFG..EQT_ANA.nombre_de_voies,
		CFG..EQT_GEN.maitre,
		@@spid
	from CFG..EQT_ANA, CFG..EQT_GEN, EXP..EQT_DSP
	where	CFG..EQT_GEN.type = XDC_EQT_ANA and
		EXP..EQT_DSP.equipement = CFG..EQT_GEN.numero and
		EXP..EQT_DSP.type = CFG..EQT_GEN.type and
		EXP..EQT_DSP.sit = @vl_Site and
		CFG..EQT_GEN.autoroute = @va_Autoroute_in and
		CFG..EQT_GEN.sens = @va_Sens_in and
		CFG..EQT_ANA.numero = CFG..EQT_GEN.numero and
		(
		  ( CFG..EQT_ANA.PR_fin between  @va_PRDebut_in and @va_PRFin_in ) or
		  ( CFG..EQT_GEN.PR     between  @va_PRDebut_in and @va_PRFin_in ) or
		  ( CFG..EQT_GEN.PR < @va_PRDebut_in and CFG..EQT_ANA.PR_fin > @va_PRFin_in ) or
		  ( CFG..EQT_GEN.PR > @va_PRFin_in   and CFG..EQT_ANA.PR_fin < @va_PRDebut_in ) or
		  ( CFG..EQT_GEN.PR > @va_PRDebut_in and CFG..EQT_ANA.PR_fin < @va_PRFin_in ) or
		  ( CFG..EQT_GEN.PR < @va_PRFin_in   and CFG..EQT_ANA.PR_fin > @va_PRDebut_in )
		) 

	/*A rechercher les RADT concernes par cet evenement */
	/*! un radt peut gerer plusieurs chaussees          */
	insert RAD_EVT (numero, nombre_de_voies, spid)
	select distinct
		CFG..EQT_PNT_MES.station,
		nombre_de_voies = max(CFG..EQT_PNT_MES.voie),
		@@spid
	from CFG..EQT_GEN, CFG..RES_CHA, CFG..EQT_PNT_MES,
	     EXP..EQT_DSP
	where	CFG..EQT_GEN.type = XDC_EQT_RAD and
		EXP..EQT_DSP.equipement = CFG..EQT_GEN.numero and
		EXP..EQT_DSP.type = CFG..EQT_GEN.type and
		EXP..EQT_DSP.sit = @vl_Site and
		CFG..EQT_GEN.autoroute = @va_Autoroute_in and
		CFG..RES_CHA.station = CFG..EQT_GEN.numero and
		CFG..RES_CHA.sens = @va_Sens_in and
		CFG..RES_CHA.PR >= @va_PRDebut_in and
		CFG..RES_CHA.PR <= @va_PRFin_in and
		CFG..EQT_PNT_MES.station = CFG..RES_CHA.station and
		CFG..EQT_PNT_MES.sens = CFG..RES_CHA.sens
	group by CFG..EQT_PNT_MES.station

	/*A recherche les DAI qui etaient deja concernes par cet evenement */
	/*A et qui ne le sont plus   sauf pour le second appel pour un basculement DEM480   */
	if @va_basc_in <> XDC_VRAI
	begin
		update EXP..VOI_DAI set 
			mode = XZAHC_VOIES_SUPP
		where	numero not in (select DAI_EVT.numero from DAI_EVT where spid = @@spid) and
			evenement = @va_NumEvt_in and
			cle = @va_CleEvt_in
	end

        /*A recherche de la classe d'evenement */
        select  @vl_Classe = CFG..TYP_FMC.classe
        from CFG..TYP_FMC, EXP..FMC_GEN
        where   EXP..FMC_GEN.numero = @va_NumEvt_in and
                EXP..FMC_GEN.cle = @va_CleEvt_in and
                CFG..TYP_FMC.numero = EXP..FMC_GEN.type

	/* Voie BAU inhibee pour les DAI dans un bouchon DEM421*/
	if @vl_Classe = XZAEC_ClasseBouchon
	select @va_BAU_in = XDC_VOIE_NEUTRALISEE

	/*A Memoriser l'etat des voies pour les DAI concernes */
	declare Pointeur_dai cursor
	for select numero, numero_analyseur, nombre_de_voies, maitre 
	from DAI_EVT
	where spid = @@spid
	open Pointeur_dai

	fetch Pointeur_dai into @vl_Numero, @vl_NumeroAnal, @vl_NbVoies, 
				@vl_NumDai
	while @@sqlstatus = 0
	begin
 		/* tester si l'evenement est nouveau ou non */
		if not exists ( select * from EXP..VOI_DAI
				where numero = @vl_Numero and
					evenement = @va_NumEvt_in and
					cle = @va_CleEvt_in and
					sens = @va_Sens_in)
		begin
			/*! si l'etat d'une des voies n'est pas soit neutralise, soit */
			/*! bloque neutralise, soit inverse alors aucun traitement    */
			/*! n'est a realiser pour les dai                             */
			if  @va_VL_in = XDC_VOIE_SENS_INVERSE or
	    		    @va_VL_in = XDC_VOIE_NEUTRALISEE or
	    		    @va_VL_in = XDC_VOIE_BLOQUEE or
			    @va_VL_in = XDC_VOIE_BLOQUEE_NEUTRALISEE or
	   		    @va_VM1_in = XDC_VOIE_SENS_INVERSE or
	    		    @va_VM1_in = XDC_VOIE_NEUTRALISEE or
	    		    @va_VM1_in = XDC_VOIE_BLOQUEE or
			    @va_VM1_in = XDC_VOIE_BLOQUEE_NEUTRALISEE or
	   		    @va_VM2_in = XDC_VOIE_SENS_INVERSE or
	    		    @va_VM2_in = XDC_VOIE_NEUTRALISEE or
	    		    @va_VM2_in = XDC_VOIE_BLOQUEE or
			    @va_VM2_in = XDC_VOIE_BLOQUEE_NEUTRALISEE or
	   		    @va_VR_in = XDC_VOIE_SENS_INVERSE or
	    		    @va_VR_in = XDC_VOIE_NEUTRALISEE or
	    		    @va_VR_in = XDC_VOIE_BLOQUEE or
			    @va_VR_in = XDC_VOIE_BLOQUEE_NEUTRALISEE or
	   		    @va_BAU_in = XDC_VOIE_SENS_INVERSE or
	    		    @va_BAU_in = XDC_VOIE_NEUTRALISEE or
	    		    @va_BAU_in = XDC_VOIE_BLOQUEE or
			    @va_BAU_in = XDC_VOIE_BLOQUEE_NEUTRALISEE
			begin
				select	@vl_NumEqt = @vl_Numero,
					@vl_TypeEqt = XDC_EQT_ANA, 
					@vl_MachineTrouvee = XDC_NON
	
				/* recherche du local technique auquel est */
				/* associe le dai                          */
				while @vl_MachineTrouvee = XDC_NON
				begin

					select
						@vl_NumMaitre = maitre,
						@vl_TypeMaitre = type_maitre
					from CFG..EQT_GEN
					where numero = @vl_NumEqt and type = @vl_TypeEqt
	
					if @@rowcount != 1
					begin
						delete RAD_EVT where spid = @@spid
						delete DAI_EVT where spid = @@spid
						return XDC_NOK
					end
	
					if @vl_TypeMaitre = XDC_EQT_MAC
					begin
						select @vl_NomMaitre = nom
						from CFG..EQT_GEN
						where	numero = @vl_NumMaitre and
							type = XDC_EQT_MAC
						break
					end
					else if @vl_NumMaitre = null
					begin
						delete RAD_EVT where spid = @@spid
						delete DAI_EVT where spid = @@spid
						return XDC_NOK
					end
					else	select @vl_NumEqt = @vl_NumMaitre, 
							@vl_TypeEqt = @vl_TypeMaitre
				end
	
				/* inserer un enregistrement correspondant */
				/* a cet analyseur                         */
				insert EXP..VOI_DAI (numero, evenement, cle,
				numero_analyseur, maitre, nombre_de_voies,
				sens, VL, VM1, VM2, VR, BAU, local_tech, mode)
				values( @vl_Numero, @va_NumEvt_in, @va_CleEvt_in,
				@vl_NumeroAnal, @vl_NumDai, @vl_NbVoies,
				@va_Sens_in, @va_VL_in, @va_VM1_in, @va_VM2_in,
				@va_VR_in, @va_BAU_in, @vl_NomMaitre, XZAHC_VOIES_MODIF)

				if @@rowcount != 1
				begin
					delete RAD_EVT where spid = @@spid
					delete DAI_EVT where spid = @@spid
					return XDC_NOK
				end
			end
		end
		else
		begin
			/* modifier l'enregistrement correspondant */
			/* a cet analyseur                         */
			update EXP..VOI_DAI set 
					nombre_de_voies = @vl_NbVoies,
					VL = @va_VL_in,
					VM1 = @va_VM1_in,
					VM2 = @va_VM2_in,
					VR = @va_VR_in,
					BAU = @va_BAU_in,
					mode = XZAHC_VOIES_MODIF
			where numero = @vl_Numero and 
				evenement = @va_NumEvt_in and
				cle = @va_CleEvt_in and
				sens = @va_Sens_in

			if @@rowcount != 1
			begin
				delete RAD_EVT where spid = @@spid
				delete DAI_EVT where spid = @@spid
				return XDC_NOK
			end
		end

		/* passage au dai suivant */
		fetch Pointeur_dai into @vl_Numero, @vl_NumeroAnal, @vl_NbVoies,
					@vl_NumDai
	end
	close Pointeur_dai

	/* vider la table des donnees utilisees dans cette procedure */
	delete DAI_EVT where spid = @@spid

        /*A recherche de la classe d'evenement 
	select  @vl_Classe = CFG..TYP_FMC.classe
	from CFG..TYP_FMC, EXP..FMC_GEN
	where   EXP..FMC_GEN.numero = @va_NumEvt_in and
		EXP..FMC_GEN.cle = @va_CleEvt_in and
		CFG..TYP_FMC.numero = EXP..FMC_GEN.type */

	/* Si l'evt est un bouchon on n'inhibe pas les RADT */
	if @vl_Classe = XZAEC_ClasseBouchon
	begin
		delete RAD_EVT where spid = @@spid
		return XDC_OK
	end

	
	/*A recherche les RADT qui etaient deja concernes par cet evenement */
	/*A et qui ne le sont plus   sauf pour le second appel pour un basculement DEM480   */
	if @va_basc_in <> XDC_VRAI
	begin
		update EXP..VOI_RAD set 
			mode = XZAHC_VOIES_SUPP
		where	numero not in (select RAD_EVT.numero from RAD_EVT where spid = @@spid) and
			evenement = @va_NumEvt_in and
			cle = @va_CleEvt_in and
			sens = @va_Sens_in
	end

	/*! pas de BAU pour un radt */
	select @va_BAU_in = XDC_VOIE_INEXISTANTE

	/*A Memoriser l'etat des voies pour les RADT concernes */
	declare Pointeur_radt cursor
	for select numero, nombre_de_voies from RAD_EVT
	where spid = @@spid
	open Pointeur_radt

	fetch Pointeur_radt into @vl_Numero, @vl_NbVoies
	while @@sqlstatus = 0
	begin
		/* tester si l'evenement est nouveau ou non */
		if not exists ( select * from EXP..VOI_RAD
				where	numero = @vl_Numero and
					evenement = @va_NumEvt_in and
					cle = @va_CleEvt_in and
					sens = @va_Sens_in)
		begin
			/*! si l'etat d'une des voies n'est pas soit neutralise, soit */
			/*! bloque neutralise, soit inverse alors aucun traitement    */
			/*! n'est a realiser pour les radt                            */
			if @va_VL_in = XDC_VOIE_SENS_INVERSE or
	    		    @va_VL_in = XDC_VOIE_NEUTRALISEE or
	    		    @va_VL_in = XDC_VOIE_BLOQUEE_NEUTRALISEE or
	   		    @va_VM1_in = XDC_VOIE_SENS_INVERSE or
	    		    @va_VM1_in = XDC_VOIE_NEUTRALISEE or
	    		    @va_VM1_in = XDC_VOIE_BLOQUEE_NEUTRALISEE or
	   		    @va_VM2_in = XDC_VOIE_SENS_INVERSE or
	    		    @va_VM2_in = XDC_VOIE_NEUTRALISEE or
	    		    @va_VM2_in = XDC_VOIE_BLOQUEE_NEUTRALISEE or
	   		    @va_VR_in = XDC_VOIE_SENS_INVERSE or
	    		    @va_VR_in = XDC_VOIE_NEUTRALISEE or
	    		    @va_VR_in = XDC_VOIE_BLOQUEE_NEUTRALISEE
			begin
				select	@vl_NumEqt = @vl_Numero,
					@vl_TypeEqt = XDC_EQT_RAD, 
					@vl_MachineTrouvee = XDC_NON
	
				/* recherche du local technique auquel est */
				/* associe le radt */
				while @vl_MachineTrouvee = XDC_NON
				begin
					select
						@vl_NumMaitre = maitre,
						@vl_TypeMaitre = type_maitre
					from CFG..EQT_GEN
					where numero = @vl_NumEqt and type = @vl_TypeEqt
	
					if @@rowcount != 1
					begin
						delete RAD_EVT where spid = @@spid
						return XDC_NOK
					end
	
					if @vl_TypeMaitre = XDC_EQT_MAC
					begin
						select @vl_NomMaitre = nom
						from CFG..EQT_GEN
						where	numero = @vl_NumMaitre and
							type = XDC_EQT_MAC
						break
					end
					else if @vl_NumMaitre = null
					begin
						delete RAD_EVT 
						where spid = @@spid
						return XDC_NOK
					end
					else	select @vl_NumEqt = @vl_NumMaitre, 
							@vl_TypeEqt = @vl_TypeMaitre
				end
	
				/* inserer un enregistrement correspondant */
				/* a cet analyseur                         */
				insert EXP..VOI_RAD (numero, sens, evenement, cle, 
				nombre_de_voies, VL, VM1, VM2, VR, BAU, local_tech, mode)
				values( @vl_Numero, @va_Sens_in, @va_NumEvt_in, 
				@va_CleEvt_in, @vl_NbVoies, @va_VL_in, @va_VM1_in, @va_VM2_in, @va_VR_in,
				@va_BAU_in, @vl_NomMaitre, XZAHC_VOIES_MODIF)

				if @@rowcount != 1
				begin
					delete RAD_EVT where spid = @@spid
					return XDC_NOK
				end
			end
		end
		else
		begin
			/* modifier l'enregistrement correspondant */
			/* a cet radt pour cet evenement           */
			update EXP..VOI_RAD set 
					nombre_de_voies = @vl_NbVoies,
					VL = @va_VL_in,
					VM1 = @va_VM1_in,
					VM2 = @va_VM2_in,
					VR = @va_VR_in,
					mode = XZAHC_VOIES_MODIF
			where numero = @vl_Numero and
				evenement = @va_NumEvt_in and
				cle = @va_CleEvt_in and
				sens = @va_Sens_in

			if @@rowcount != 1
			begin
				delete RAD_EVT where spid = @@spid
				return XDC_NOK
			end
		end

		/* passage au dai suivant */
		fetch Pointeur_radt into @vl_Numero, @vl_NbVoies
	end
	close Pointeur_radt

	/* vider la table des donnees utilisees dans cette procedure */
	delete RAD_EVT where spid = @@spid

	return XDC_OK
go
