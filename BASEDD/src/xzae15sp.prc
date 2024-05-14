/*E*/
/*  Fichier : $Id: xzae15sp.prc,v 1.22 2021/05/04 13:29:13 pc2dpdy Exp $        $Revision: 1.22 $        $Date: 2021/05/04 13:29:13 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae15sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.     	24/10/94	: Creation 	V 1.1
* C.T.     	10/11/94	: Modif bugs applix 	V 1.3
*                                 non teste
* C.T.     	16/11/94	: Autoriser de donner 
*                                 le site local pour transmettre 
*                                 non teste     V 1.4
* B.G.     	22/12/94	: modif reveil
* C.T.     	22/02/95	: modif zz 	V 1.6
* C.T.		14/03/95	: Modif longueur texte reveil (V 1.7)
* C.T.		24/03/95	: Transmettre les fiches vehicules
*                                 lors d'un accident (V 1.8)
* C.T.		23/06/95	: Modif controle appartenance a la 
*                                 liste a traiter (V 1.9)
* C.T.		22/04/96	: Ajout controle sur tete de bouchon pour
*                                 tjours envoyer le reveil sur queue de
*                                 bouchon (V 1.10) (1038)
* JMG		12/11/96	: ajout du parametre va_rafraichir_in (DEM/1205) 1.13
* JMG		07/01/98	: gestion PC niveau 2 (dem/1536) 1.14
* JPL		04/06/07	: Ajout classe Fmc Basculement pour travaux (DEM 647) 1.15
* JPL		01/09/08	: Texte de reveil marque urgent pour les Fmc Accident et Contresens (DEM 721) 1.16
* JMG		03/12/08	: ajout validation dans FMc_ACC 1.17 DEM/852
* JPL		12/01/09	: Degroupee; appel a XZAE18 renommee (lie a DEM 848) 1.18
* JPL		10/10/14	: Ajout du parametre 'sans reveil' (DEM 1101)  1.19
* JPL		15/03/19	: Plus de mise � jour du site en table compl�mentaire FMC_MNF (DEM 1327)  1.20
* LCL   	22/04/20        : MOVIS Ajout site local pilotage DEM-SAE93
* PNI 		25/07/22	: ajout le cas XZAEC_ClasseIncendie pour traiter FMC_INC SAE-398
* PNI		21/03/24	: suppression du test sur sit pour FMC_VEH (AVA sit peut être 43) SAE-518
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Confie le droit d'enrichir au site ou au poste operateur.
* 
* Sequence d'appel
* XZAE15_Transmettre_Droit_Enrichir
* 
* Arguments en entree
* XDY_Entier	va_NumEvt_in
* XDY_Octet	va_CleEvt_in
* XDY_Octet	va_Site_in
* XDY_Machine	va_PosteOperateur_in
* XDY_Entier	va_Sans_Rafraichir_in
* XDY_Entier	va_Sans_Reveil_in
*
* Arguments en sortie
* XDY_Entier	va_Resultat_out
* 
* Code retour
* XDC_OK
* XDC_NOK		: modif du droit impossible
* XDC_ARG_INV		: argument d'entree null, ou valeur incorrecte
* XDC_PRC_INC		: procedure stockee appelee n'existe pas en base
* < 0                   : erreur sybase
* 
* Conditions d'utilisation
* Soit le poste operateur, soit le site doit etre value a non null
* Pas l'autorisation de transmettre un evenement de type operateur
* Valeur prise par va_Resultat_out :
* - XZAEC_FMC_PAS_ENR	: pas le droit de transmettre l'enrichissement 
*                         (evenement pas sur le site local ou
*                          de type operateur)
* - XZAEC_FMC_OPR_INC	: operateur n'est pas en activite
* - XDC_OK		: le droit d'enrichir a ete transmis
* 
* Fonction
* Pour le transfert du droit d'enrichir a un site : 
* 	Update du champ site dans la table EVENEMENT, 
* 	EVENEMENT_HISTORIQUE et dans 
* 	la table de complement d'evenement (ou insert 
* 	distant puis delete local)
* Pour le tranfert du droit d'enrichir a un poste operateur du m�me site :
* 	Update du champ poste_enrichisseur dans la table EVENEMENT
---------------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAE15' and type = 'P')
	drop procedure XZAE15
go


create procedure XZAE15
	@va_NumEvt_in		int	= null,
	@va_CleEvt_in		tinyint	= null,
	@va_Site_in		T_SITE	= null,
	@va_PosteOperateur_in	T_EQUIPEMENT	= null,
	@va_Resultat_out	int	= null	output,
	@va_Sans_Rafraichir_in	int = null,
	@va_Sans_Reveil_in	int = null,
	@va_NomSiteLocal_in     char(2)         = null
as
	declare @vl_Site T_SITE, @vl_Erreur bit, @vl_Status int,
		@vl_PosteOperateur T_EQUIPEMENT, @vl_TexteReveil char(100),
		@vl_Evt int, @vl_CleEvt tinyint, @vl_typeSite tinyint,
		@vl_Type smallint, @vl_Classe tinyint, @vl_Enrichisseur T_EQUIPEMENT,
		@vl_ResultAjoutReveil bit, @vl_validation datetime

	/*! transfo en param�tres nulls bug applix */
	if @va_NumEvt_in = 0 select @va_NumEvt_in = null
	if @va_CleEvt_in = 0 select @va_CleEvt_in = null
	if @va_Site_in = 0 select @va_Site_in = null
	if @va_PosteOperateur_in = 0 select @va_PosteOperateur_in = null

	/*A controle les parametres obligatoires */
	if @va_NumEvt_in = null or @va_CleEvt_in = null or
	   (@va_PosteOperateur_in = null and @va_Site_in = null) or
	   (@va_PosteOperateur_in != null and @va_Site_in != null)
		return XDC_ARG_INV

	/* recherche du site local */
        /* MOVIS */
        if @va_NomSiteLocal_in = null
		select @vl_Site = numero, @vl_typeSite = type
                from CFG..RES_DIS
                where serveur = @@servername
        else
		select @vl_Site = numero, @vl_typeSite = type
                from CFG..RES_DIS
                where code = @va_NomSiteLocal_in

	if @vl_typeSite = XDC_TYPEM_PCS
	begin
	select "LCL1"
		return XDC_NOK
		end

	/* initialisation des variables */
	select	@vl_Erreur = XDC_NON,
		@va_Resultat_out = XDC_OK

	/*A verifier que l'existance de l'evenement a traiter sur le site local */
	select 
		@vl_Type = EXP..FMC_GEN.type,
		@vl_Classe = CFG..TYP_FMC.classe,
		@vl_Enrichisseur = EXP..FMC_GEN.poste_enrichisseur
	from EXP..FMC_GEN, CFG..TYP_FMC, EXP..FMC_TRT
	where	EXP..FMC_GEN.numero = @va_NumEvt_in and
		EXP..FMC_GEN.cle = @va_CleEvt_in and
		EXP..FMC_TRT.evenement = @va_NumEvt_in and
		EXP..FMC_TRT.cle = @va_CleEvt_in and
		EXP..FMC_TRT.sit = @vl_Site and
		CFG..TYP_FMC.numero = EXP..FMC_GEN.type and
		EXP..FMC_GEN.sit = @vl_Site

	if @@rowcount = 0
	begin
		select @va_Resultat_out =  XZAEC_FMC_PAS_ENR
		return XDC_OK
	end

	/* test que l'evenement n'est pas de type operateur */
	/* pas d'enrichissement autorise                    */
	if @vl_Classe = XZAEC_ClassePosteOper
	begin
		select @va_Resultat_out = XZAEC_FMC_PAS_ENR
		return XDC_OK
	end

	if @va_PosteOperateur_in != null or @va_Site_in = @vl_Site
	begin
		/*A cas : transmettre le droit d'enrichir a un autre operateur */

		/*A cas ou l'on transmet au site local : recherche d'un autre */
		/*A operateur present sur le meme site */
		if @va_Site_in = @vl_Site
		begin
			/* recherche un autre operateur present sur le meme site */
			select distinct
				@va_PosteOperateur_in = EXP..FMC_GEN.poste_enrichisseur
			from EXP..FMC_GEN, CFG..TYP_FMC
			where	EXP..FMC_GEN.sit = @va_Site_in and 
				CFG..TYP_FMC.numero = EXP..FMC_GEN.type and
				CFG..TYP_FMC.classe = XZAEC_ClassePosteOper and
				EXP..FMC_GEN.fin is null and 
				EXP..FMC_GEN.poste_enrichisseur is not null and
				EXP..FMC_GEN.poste_enrichisseur != @vl_Enrichisseur

			/* cas ou aucun operateur est present iautre que        */
			/* l'enrichisseur actuel : pas de transmission de droit */
			if @@rowcount = 0
			begin
				select @va_Resultat_out =  XZAEC_FMC_OPR_INC
				return XDC_OK
			end
		end
		/*A cas ou l'on transmet a un operateur precis sur le site local : */
		/*A tester si l'operateur est present */
		else if not exists (	select * from EXP..FMC_GEN, CFG..TYP_FMC
				where	EXP..FMC_GEN.poste_enrichisseur = @va_PosteOperateur_in and
					EXP..FMC_GEN.fin is null and
					CFG..TYP_FMC.numero = EXP..FMC_GEN.type and
					CFG..TYP_FMC.classe = XZAEC_ClassePosteOper and
					EXP..FMC_GEN.sit = @vl_Site)
		begin
			select @va_Resultat_out =  XZAEC_FMC_OPR_INC
			return XDC_OK
		end

		/*A transmettre le droit d'enrichir a un autre */
		/*A operateur sur le meme site                 */
		update EXP..FMC_GEN set 
			poste_enrichisseur = @va_PosteOperateur_in,
			zz = zz + 1
		where	numero = @va_NumEvt_in and cle =@va_CleEvt_in and
			sit = @vl_Site

		if @@rowcount != 1
		begin
		select "LCL2"
			return XDC_NOK
			end

		if @va_Sans_Reveil_in != XDC_VRAI
		begin
			/*A declencher un reveil, marque urgent selon le type d'evenement */
			select @vl_TexteReveil = XDC_REV_FMC_ENR
			if @vl_Type in (XZAEC_FMC_Accident, XZAEC_FMC_Contresens)
				select @vl_TexteReveil = XDC_REV_URGENT + @vl_TexteReveil
			exec @vl_Status = XZAR;03	@va_NumEvt_in, @va_CleEvt_in,
							@va_PosteOperateur_in, @vl_TexteReveil

			if @vl_Status = null
				return XDC_PRC_INC
			else if @vl_Status != XDC_OK
				return @vl_Status
		end
	end
	else
	begin
		/*A cas : transmettre le droit d'enrichir a un autre site */

		/*A transmettre le droit d'enrichir l'evenement a un autre site */
		begin tran evenement
			/*B modifier le site de la partie generique de l'evenement */
			update EXP..FMC_GEN set 
				poste_enrichisseur = null,
				sit = @va_Site_in,
				zz = zz + 1
			where	numero = @va_NumEvt_in and cle = @va_CleEvt_in and
				sit = @vl_Site
select "LCL01"
			if @@rowcount != 1
begin
select "LCL02"
				select @vl_Erreur = XDC_OUI
end
			else
			begin
				/*B modifier le site de la partie complement de    */
				/*B l'evenement dans le cas ou ce complement n'est */
				/*B historise                                      */
				if @vl_Classe = XZAEC_ClasseAccident
				begin
					/* modifier le site des vehicules impliques */
					/* dans l'accident                          */
					update EXP..FMC_VEH set
						sit = @va_Site_in,
						zz = zz + 1
					where	numero = @va_NumEvt_in and 
						cle =@va_CleEvt_in and
						sit = @vl_Site
					select @vl_validation=validation
					from EXP..FMC_ACC
					where numero = @va_NumEvt_in and
							cle =@va_CleEvt_in and
							sit = @vl_Site
					having validation = max (validation)

					update EXP..FMC_ACC set 
						sit = @va_Site_in,
						zz = zz + 1
					where	numero = @va_NumEvt_in and 
						cle =@va_CleEvt_in and
						sit = @vl_Site
						and validation = @vl_validation

				end				
				if @vl_Classe = XZAEC_ClasseIncendie
				begin
					/* modifier le site des vehicules impliques */
					/* dans l'accident                          */
					update EXP..FMC_INC set
						sit = @va_Site_in,
						zz = zz + 1
					where	numero = @va_NumEvt_in and 
						cle =@va_CleEvt_in and
						sit = @vl_Site
					end
				else if @vl_Classe = XZAEC_ClasseVehicule
				begin
--select "LCL002"
					update EXP..FMC_VEH set 
						sit = @va_Site_in,
						zz = zz + 1
					where	numero = @va_NumEvt_in and 
						cle =@va_CleEvt_in 
						/** SAE 518 and
						sit = @vl_Site */
				end
				else if @vl_Classe = XZAEC_ClasseDelestage
					update EXP..FMC_DEL set 
						sit = @va_Site_in,
						zz = zz + 1
					where	numero = @va_NumEvt_in and 
						cle =@va_CleEvt_in and
						sit = @vl_Site
				else if @vl_Classe = XZAEC_ClasseEchangeur
					update EXP..FMC_ECH set 
						sit = @va_Site_in,
						zz = zz + 1
					where	numero = @va_NumEvt_in and 
						cle =@va_CleEvt_in and
						sit = @vl_Site
				else if @vl_Classe = XZAEC_ClasseSousConcess
					update EXP..FMC_CNC set 
						sit = @va_Site_in,
						zz = zz + 1
					where	numero = @va_NumEvt_in and 
						cle =@va_CleEvt_in and
						sit = @vl_Site
				else if @vl_Classe = XZAEC_ClasseTravaux
					update EXP..FMC_TRF set 
						sit = @va_Site_in,
						zz = zz + 1
					where	numero = @va_NumEvt_in and 
						cle =@va_CleEvt_in and
						sit = @vl_Site
				else if @vl_Classe = XZAEC_ClasseBasculement
					update EXP..FMC_BAS set 
						sit = @va_Site_in,
						zz = zz + 1
					where	numero = @va_NumEvt_in and 
						cle =@va_CleEvt_in and
						sit = @vl_Site
				else if @vl_Classe = XZAEC_ClasseBasculTrav
				    begin
					update EXP..FMC_TRF set 
						sit = @va_Site_in,
						zz = zz + 1
					where	numero = @va_NumEvt_in and 
						cle =@va_CleEvt_in and
						sit = @vl_Site
					update EXP..FMC_BAS set 
						sit = @va_Site_in,
						zz = zz + 1
					where	numero = @va_NumEvt_in and 
						cle =@va_CleEvt_in and
						sit = @vl_Site
				    end
				else select @vl_Erreur = XDC_NON
				
				if @@rowcount != 1
begin
select "LCL03"
					select @vl_Erreur = XDC_OUI
end
			end

		if @vl_Erreur = XDC_OUI
		begin
			rollback tran evenement
			select "LCL3"
			return XDC_NOK
		end
		else	commit tran evenement

		/*A ajouter l'evenement dans la liste des evenements a traiter */
		/*A du site enrichisseur                                       */
		exec @vl_Status = XZAE18 @va_Site_in, @va_NumEvt_in, 
					@va_CleEvt_in, @vl_ResultAjoutReveil output

		if @vl_Status = null
			return XDC_PRC_INC
		else if @vl_Status != XDC_OK
			return @vl_Status


		if @va_Sans_Reveil_in != XDC_VRAI
		begin
			/* texte du reveil a envoyer, marque urgent selon le type d'evenement */
			select @vl_TexteReveil = XDC_REV_FMC_ENR
			if @vl_Type in (XZAEC_FMC_Accident, XZAEC_FMC_Contresens)
				select @vl_TexteReveil = XDC_REV_URGENT + @vl_TexteReveil

			/* creation de la table contenant les reveils a declencher (un par poste) */
			create table #REVEILS ( poste smallint)

			/*A recherche la liste des operateurs a reveiller sur le nouveau site */
			insert #REVEILS ( poste)
			select distinct
				EXP..FMC_GEN.poste_enrichisseur
			from EXP..FMC_GEN, CFG..TYP_FMC
			where	EXP..FMC_GEN.sit = @va_Site_in and 
				CFG..TYP_FMC.numero = EXP..FMC_GEN.type and
				CFG..TYP_FMC.classe = XZAEC_ClassePosteOper and
				EXP..FMC_GEN.fin is null and 
				EXP..FMC_GEN.poste_enrichisseur is not null

			/* recherche la queue de bouchon */
			if @vl_Type = XZAEC_FMC_TeteBouchon
			begin
				select
					@vl_Evt = numero,
					@vl_CleEvt = cle
				from EXP..FMC_GEN
				where	cause = @va_NumEvt_in and 
					cle_cause = @va_CleEvt_in

				if @@rowcount = 1
					select @va_NumEvt_in = @vl_Evt , @va_CleEvt_in = @vl_CleEvt 
			end

			/* creation des curseurs */
			declare Pointeur_reveil cursor
			for select poste
			from #REVEILS

			/*A declencher les reveils */
			open Pointeur_reveil
			fetch Pointeur_reveil into @vl_PosteOperateur
			while (@@sqlstatus = 0)
			begin
				/*B declencher un reveil par poste operateur en activite */
				exec @vl_Status = XZAR;03 @va_NumEvt_in, @va_CleEvt_in,
							@vl_PosteOperateur, @vl_TexteReveil
				if @vl_Status = null
					return XDC_PRC_INC
				else if @vl_Status != XDC_OK
					return @vl_Status

				/* poste suivant a prevenir */
				fetch Pointeur_reveil into @vl_PosteOperateur
			end

			close Pointeur_reveil
		end
	end

/*A on rafraichit la liste en local */
	
	if @va_Sans_Rafraichir_in != XDC_VRAI
		exec @vl_Status = XZAR;03 0, 0, null, " ", @vl_Site

	return XDC_OK
go
