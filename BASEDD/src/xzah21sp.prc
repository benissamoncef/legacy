/*E*/
/*  Fichier : $Id: xzah21sp.prc,v 1.4 1998/02/25 10:18:47 gaborit Exp $      Release : $Revision: 1.4 $        Date : $Date: 1998/02/25 10:18:47 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzah21sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* initialisation et mise a jour des voies pour equext
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.  30/11/94        : Creation      (V 1.1)
* C.T.  05/12/94        : Correction commentaire      (V 1.2)
* B.G.  11/01/95        : simplification select       (V 1.3)
* C.T.	23/02/98	: Modif default suite a passage en version SQL 11.0.3.2 (1.4)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Envoyer les etats des voies pour les radt
* et les dai pour le site local
*
* Sequence d'appel
* SP    XZAH21_Lire_Voies_Fmc
*
* Arguments en entree
* XDY_NomMachine	va_LocalTechnique_in
*
* Arguments en sortie
*
* liste des etats des voies des dai retournee :
* XDY_Eqt		va_Numero
* XDY_Octet		va_NumAnal
* XDY_Eqt		va_NumDAI
* XDY_Voie		va_BAU
* XDY_Voie		va_V1
* XDY_Voie		va_V2
* XDY_Voie		va_V3
* XDY_Voie		va_V4
* XDY_NomMachine	va_LocalTechnique
*
* liste des etats des voies des radt retournee :
* XDY_Eqt		va_Numero
* XDY_Sens		va_Sens
* XDY_Voie		va_BAU
* XDY_Voie		va_V1
* XDY_Voie		va_V2
* XDY_Voie		va_V3
* XDY_Voie		va_V4
* XDY_NomMachine	va_LocalTechnique
*
* Code retour
* XDC_OK        : pas de probleme
* XDC_NOK       : probleme a l'insertion dans la base de sauvegarde
*                 ou donnee non trouvee
* XDC_ARG_INV   : parametre d'entree null ou date demandee trop proche
*
* Conditions d'utilisation
* Si le nom du local technique n'est pas value alors seuls
* les etats des voies modifies sont retournes
* Sinon les etats de toutes les voies pour tous les dai et radt
* sont retournes
*
* Fonction
* select VOI_DAI et VOI_RAD
-------------------------------------------------------*/

use PRC
go

create procedure XZAH;21
	@va_LocalTechnique_in	char(10) = null
as
	declare @vl_NumEqt T_EQUIPEMENT, @vl_LocalTech char(10), @vl_NumLocalTech smallint,
		@vl_Mode tinyint, @vl_NumDai T_EQUIPEMENT,
		@vl_NumAnal tinyint, @vl_NbVoies tinyint, @vl_Sens T_SENS,
		@vl_VL T_VOIE, @vl_VM1 T_VOIE, @vl_VM2 T_VOIE,
		@vl_VR T_VOIE, @vl_BAU T_VOIE, @vl_Ajout int,
		@vl_Status int

	if @va_LocalTechnique_in = XDC_CHAINE_VIDE
		select @va_LocalTechnique_in = null

	/* creation d'une table de stockage des etats des analyseurs */
	create table #RES_ANA ( numero smallint, numero_analyseur tinyint,
			nombre_de_voies tinyint, maitre smallint,
			local_tech char(10),
			VR tinyint default XDC_VOIE_INEXISTANTE, 
			VM2 tinyint default XDC_VOIE_INEXISTANTE, 
			VM1 tinyint default XDC_VOIE_INEXISTANTE, 
			VL tinyint default XDC_VOIE_INEXISTANTE, 
			BAU tinyint default XDC_VOIE_INEXISTANTE, 
			V2 tinyint default XDC_VOIE_INEXISTANTE,
			V3 tinyint default XDC_VOIE_INEXISTANTE, 
			V4 tinyint default XDC_VOIE_INEXISTANTE)

	/* creation d'une table de stockage des etats des radt */
	create table #RES_RADT ( numero smallint, nombre_de_voies tinyint,
			sens tinyint, local_tech char(10),
			VR tinyint default XDC_VOIE_INEXISTANTE, 
			VM2 tinyint default XDC_VOIE_INEXISTANTE, 
			VM1 tinyint default XDC_VOIE_INEXISTANTE, 
			VL tinyint default XDC_VOIE_INEXISTANTE, 
			BAU tinyint default XDC_VOIE_INEXISTANTE, 
			V2 tinyint default XDC_VOIE_INEXISTANTE,
			V3 tinyint default XDC_VOIE_INEXISTANTE, 
			V4 tinyint default XDC_VOIE_INEXISTANTE)

	if @va_LocalTechnique_in = null
	begin
		/*A recherche toutes les modifications d'etats non lues */
		/*A pour les dai                                        */
		declare Pointeur_analyseur cursor
		for select distinct
			numero,
			numero_analyseur,
			nombre_de_voies,
			local_tech,
			mode,
			maitre
		from EXP..VOI_DAI
		where mode != XZAHC_VOIES_LUES

		/* parcours des modifications d'etats non lues */
		/* pour les dai                                */
		open Pointeur_analyseur
		fetch Pointeur_analyseur into @vl_NumEqt, @vl_NumAnal,
			@vl_NbVoies, @vl_LocalTech, @vl_Mode, @vl_NumDai
		while @@sqlstatus = 0
		begin
			if exists ( select * from #RES_ANA
				where numero = @vl_NumEqt)
				/* aucun traitement : c'est deja realise pour cet analyseur */
				select @vl_Mode = @vl_Mode
			else
			begin
				/* lecture des etats des voies de l'analyseur */
				exec @vl_Status = XZAH;22 @vl_NumEqt, @vl_Mode, @vl_NbVoies,
						@vl_VL output, @vl_VM1 output, 
						@vl_VM2 output, @vl_VR output,
						@vl_BAU output, @vl_Ajout output
				if @vl_Status = null
					return XDC_PRC_INC

				/* memorisation des etats des voies de l'analyseur */
				if @vl_Ajout = XDC_OUI
					insert #RES_ANA ( numero, numero_analyseur,
					nombre_de_voies, local_tech, maitre,
					VR, VM2, VM1, VL, BAU, V2, V3, V4)
					values ( @vl_NumEqt, @vl_NumAnal,
					@vl_NbVoies, @vl_LocalTech,
					@vl_NumDai, @vl_VR, @vl_VM2, @vl_VM1,
					@vl_VL, @vl_BAU, XDC_VOIE_INEXISTANTE,
					XDC_VOIE_INEXISTANTE, XDC_VOIE_INEXISTANTE)
			end

			/* passage a l'analyseur suivant */
			fetch Pointeur_analyseur into @vl_NumEqt, @vl_NumAnal,
				@vl_NbVoies, @vl_LocalTech, @vl_Mode, @vl_NumDai
		end
		close Pointeur_analyseur

		/*A recherche toutes les modifications d'etats non lues */
		/*A pour les radt                                        */
		declare Pointeur_radt cursor
		for select distinct
			numero,
			nombre_de_voies,
			sens,
			local_tech,
			mode
		from EXP..VOI_RAD
		where mode != XZAHC_VOIES_LUES

		/* parcours des modifications d'etats non lues */
		/* pour les radt                               */
		open Pointeur_radt
		fetch Pointeur_radt into @vl_NumEqt, @vl_NbVoies, 
			@vl_Sens, @vl_LocalTech, @vl_Mode
		while @@sqlstatus = 0
		begin
			if exists ( select * from #RES_RADT
				where numero = @vl_NumEqt and sens = @vl_Sens)
				/* aucun traitement : c'est deja realise pour cet radt */
				select @vl_Mode = @vl_Mode
			else
			begin
				/* lecture des etats des voies du radt */
				exec @vl_Status = XZAH;23 @vl_NumEqt, @vl_Mode, @vl_NbVoies,
						@vl_Sens, @vl_VL output, @vl_VM1 output, 
						@vl_VM2 output, @vl_VR output,
						@vl_Ajout output
				if @vl_Status = null
					return XDC_PRC_INC
				else if @vl_Status != XDC_OK
					return @vl_Status

				/* memorisation des etats des voies du radt */
				if @vl_Ajout = XDC_OUI
					insert #RES_RADT ( numero, nombre_de_voies,
					sens, local_tech, VR, VM2, VM1, VL, BAU,
					V2, V3, V4)
					values ( @vl_NumEqt, @vl_NbVoies, @vl_Sens, 
					@vl_LocalTech, @vl_VR, @vl_VM2, @vl_VM1, @vl_VL, 
					XDC_VOIE_INEXISTANTE, XDC_VOIE_INEXISTANTE,
					XDC_VOIE_INEXISTANTE, XDC_VOIE_INEXISTANTE)
			end

			/* passage au radt suivant */
			fetch Pointeur_radt into @vl_NumEqt, @vl_NbVoies, 
				@vl_Sens, @vl_LocalTech, @vl_Mode
		end
		close Pointeur_radt
	end
	else
	begin
		/*A recherche de tous les analyseurs     */
		/*A associes au local technique specifie */
		select @vl_NumLocalTech=numero
		from CFG..EQT_GEN
		where nom = @va_LocalTechnique_in and
		      type = XDC_EQT_MAC
		
		insert #RES_ANA ( numero, maitre, nombre_de_voies,
		numero_analyseur, local_tech,
		VR, VM2, VM1, VL, BAU, V2, V3, V4)
		select 
			a.numero,
			a.maitre,
			CFG..EQT_ANA.nombre_de_voies,
			CFG..EQT_ANA.numero_analyseur,
			@va_LocalTechnique_in,
			XDC_VOIE_INEXISTANTE,
			XDC_VOIE_INEXISTANTE,
			XDC_VOIE_INEXISTANTE,
			XDC_VOIE_INEXISTANTE,
			XDC_VOIE_INEXISTANTE,
			XDC_VOIE_INEXISTANTE,
			XDC_VOIE_INEXISTANTE,
			XDC_VOIE_INEXISTANTE
		from CFG..EQT_GEN d, CFG..EQT_GEN m,
			CFG..EQT_GEN a, CFG..EQT_ANA
		where d.type = XDC_EQT_DAI and 
			m.numero = d.maitre and
			m.type = d.type_maitre and
			m.type_maitre = XDC_EQT_MAC and
			@vl_NumLocalTech = m.maitre and 
			a.maitre = d.numero and
			a.type_maitre = XDC_EQT_DAI and
			CFG..EQT_ANA.numero = a.numero

		/*A rechercher l'etat des voies des analyseurs */
		/*A associes au local technique specifie       */
		update #RES_ANA set 
			BAU = XDC_VOIE_BAU_NORMALE,
			VL = XDC_VOIE_SENS_NORMAL
		where nombre_de_voies = 2

		update #RES_ANA set 
			BAU = XDC_VOIE_BAU_NORMALE,
			VL = XDC_VOIE_SENS_NORMAL,
			VR = XDC_VOIE_SENS_NORMAL
		where nombre_de_voies = 3

		update #RES_ANA set 
			BAU = XDC_VOIE_BAU_NORMALE,
			VL = XDC_VOIE_SENS_NORMAL,
			VR = XDC_VOIE_SENS_NORMAL,
			VM1 = XDC_VOIE_SENS_NORMAL
		where nombre_de_voies = 4

		update #RES_ANA set 
			BAU = XDC_VOIE_BAU_NORMALE,
			VL = XDC_VOIE_SENS_NORMAL,
			VR = XDC_VOIE_SENS_NORMAL,
			VM1 = XDC_VOIE_SENS_NORMAL,
			VM2 = XDC_VOIE_SENS_NORMAL
		where nombre_de_voies = 5

		/*A recherche toutes les modifications d'etats */
		/*A pour les dai dues a des evenements         */
		declare Pointeur_analyseur cursor
		for select distinct 
			numero, 
			nombre_de_voies, 
			mode
		from EXP..VOI_DAI
		where mode != XZAHC_VOIES_SUPP
		open Pointeur_analyseur

		fetch Pointeur_analyseur into @vl_NumEqt, 
				@vl_NbVoies, @vl_Mode
		while @@sqlstatus = 0
		begin
			/* recherche toutes les modifications d'etats */
			/* pour un analyseur dues a des evenements    */
			exec @vl_Status = XZAH;22 @vl_NumEqt, @vl_Mode, @vl_NbVoies,
					@vl_VL output, @vl_VM1 output, 
					@vl_VM2 output, @vl_VR output,
					@vl_BAU output, @vl_Ajout output
			if @vl_Status = null
				return XDC_PRC_INC
			else if @vl_Status != XDC_OK
				return @vl_Status

			/* memorisation des etats des voies de l'analyseur */
			if @vl_Ajout = XDC_OUI
				update #RES_ANA set
					VL = @vl_VL,
					VM1 = @vl_VM1,
					VM2 = @vl_VM2,
					VR = @vl_VR,
					BAU = @vl_BAU
				where numero = @vl_NumEqt 

			/* analyseur suivant */
			fetch Pointeur_analyseur into @vl_NumEqt, 
				@vl_NbVoies, @vl_Mode
		end
		close Pointeur_analyseur

		/*A recherche de tous les radt associes */
		/*A au local technique specifie         */
		insert #RES_RADT ( numero, nombre_de_voies,
		local_tech, sens, VR, VM2, VM1, VL, BAU,
		V2, V3, V4)
		select distinct
			r.numero,
			max(mes.voie),
			@va_LocalTechnique_in,
			mes.sens,
			XDC_VOIE_INEXISTANTE, 
			XDC_VOIE_INEXISTANTE, 
			XDC_VOIE_INEXISTANTE, 
			XDC_VOIE_INEXISTANTE, 
			XDC_VOIE_INEXISTANTE, 
			XDC_VOIE_INEXISTANTE, 
			XDC_VOIE_INEXISTANTE, 
			XDC_VOIE_INEXISTANTE
		from CFG..EQT_GEN r, CFG..EQT_GEN m,
			CFG..EQT_PNT_MES mes
		where r.type = XDC_EQT_RAD and 
			m.numero = r.maitre and
			m.type = r.type_maitre and
			m.type_maitre = XDC_EQT_MAC and
			@vl_NumLocalTech = m.maitre and 
			mes.station = r.numero and
			(mes.sens = XDC_SENS_SUD or
			 mes.sens = XDC_SENS_NORD)
		group by mes.station, mes.sens, r.numero

		/*A rechercher l'etat des voies des analyseurs */
		/*A associes au local technique specifie       */
		update #RES_RADT set 
			VL = XDC_VOIE_SENS_NORMAL
		where nombre_de_voies = 1

		update #RES_RADT set 
			VL = XDC_VOIE_SENS_NORMAL,
			VR = XDC_VOIE_SENS_NORMAL
		where nombre_de_voies = 2

		update #RES_RADT set 
			VL = XDC_VOIE_SENS_NORMAL,
			VR = XDC_VOIE_SENS_NORMAL,
			VM1 = XDC_VOIE_SENS_NORMAL
		where nombre_de_voies = 3

		update #RES_RADT set 
			VL = XDC_VOIE_SENS_NORMAL,
			VR = XDC_VOIE_SENS_NORMAL,
			VM1 = XDC_VOIE_SENS_NORMAL,
			VM2 = XDC_VOIE_SENS_NORMAL
		where nombre_de_voies = 4

		/*A recherche toutes les modifications d'etats */
		/*A pour les radt                              */
		declare Pointeur_radt cursor
		for select
			numero,
			nombre_de_voies,
			sens,
			mode
		from EXP..VOI_RAD
		where mode != XZAHC_VOIES_SUPP

		/* parcours des modifications d'etats */
		/* pour les radt                      */
		open Pointeur_radt
		fetch Pointeur_radt into @vl_NumEqt, @vl_NbVoies, 
			@vl_Sens, @vl_Mode
		while @@sqlstatus = 0
		begin
			/* lecture des etats des voies du radt */
			exec @vl_Status = XZAH;23 @vl_NumEqt, @vl_Mode, @vl_NbVoies,
					@vl_Sens, @vl_VL output, @vl_VM1 output, 
					@vl_VM2 output, @vl_VR output, @vl_Ajout output
			if @vl_Status = null
				return XDC_PRC_INC
			else if @vl_Status != XDC_OK
				return @vl_Status

			/* memorisation des etats des voies du radt */
			if @vl_Ajout = XDC_OUI
				update #RES_RADT set
					VL = @vl_VL,
					VM1 = @vl_VM1,
					VM2 = @vl_VM2,
					VR = @vl_VR
				where numero = @vl_NumEqt and sens = @vl_Sens

			/* passage au radt suivant */
			fetch Pointeur_radt into @vl_NumEqt, @vl_NbVoies, 
				@vl_Sens, @vl_Mode 
		end
		close Pointeur_radt
	end

	/*! marquer tous les etats des voies lus des radt */
	/*! et des dai pour ce local technique            */
	update EXP..VOI_DAI set mode = XZAHC_VOIES_LUES
	where mode = XZAHC_VOIES_MODIF and (@va_LocalTechnique_in is null or
		local_tech = @va_LocalTechnique_in)
	update EXP..VOI_RAD set mode = XZAHC_VOIES_LUES
	where mode = XZAHC_VOIES_MODIF and (@va_LocalTechnique_in is null or
		local_tech = @va_LocalTechnique_in)

	/*! supprimer les etats des voies qui ne sont plus modifies */
	/*! par un evenement                                        */
	delete EXP..VOI_DAI
	where mode = XZAHC_VOIES_SUPP and (@va_LocalTechnique_in is null or
		local_tech = @va_LocalTechnique_in)
	delete EXP..VOI_RAD
	where mode = XZAHC_VOIES_SUPP and (@va_LocalTechnique_in is null or
		local_tech = @va_LocalTechnique_in)

	/*A ordonner les voies pour l'envoi des etats selon la   */
	/*A regle suivante :                                     */
	/*A si un analyseur a :                                  */
	/*A - 2 voies : BAU, VL, (VM1, VM2, VR : a inexistante ) */
	/*A - 3 voies : BAU, VL, VR, (VM1, VM2 : a inexistante ) */
	/*A - 4 voies : BAU, VL, VM1, VR, (VM2 : a inexistante ) */
	/*A - 5 voies : BAU, VL, VM1, VM2, VR                    */
	update #RES_ANA set V2 = VR
	where nombre_de_voies = 3
	update #RES_ANA set V2 = VM1, V3 = VR
	where nombre_de_voies = 4
	update #RES_ANA set V2 = VM1, V3 = VM2, V4 = VR
	where nombre_de_voies = 5

	/*A ordonner les voies pour l'envoi des etats selon la   */
	/*A regle suivante :                                     */
	/*A si un radt a :                                       */
	/*A - 1 voies : BAU, VL, (VM1, VM2, VR : a inexistante ) */
	/*A - 2 voies : BAU, VL, VR, (VM1, VM2 : a inexistante ) */
	/*A - 3 voies : BAU, VL, VM1, VR, (VM2 : a inexistante ) */
	/*A - 4 voies : BAU, VL, VM1, VM2, VR                    */
	/*A la BAU est toujours inexistante                      */
	update #RES_RADT set V2 = VR
	where nombre_de_voies = 2
	update #RES_RADT set V2 = VM1, V3 = VR
	where nombre_de_voies = 3
	update #RES_RADT set V2 = VM1, V3 = VM2, V4 = VR
	where nombre_de_voies = 4

	/*A envoyer la liste des etats des voies des analyseurs */
	select
		numero,
		numero_analyseur,
		maitre,
		BAU,
		VL,
		V2,
		V3,
		V4,
		local_tech
	from #RES_ANA

	/*A envoyer la liste des etats des voies des radt */
	select
		numero,
		sens,
		BAU,
		VL,
		V2,
		V3,
		V4,
		local_tech
	from #RES_RADT

	return XDC_OK
go
