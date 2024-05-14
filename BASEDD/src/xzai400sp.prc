/*E*/
/*Fichier : $Id: xzai400sp.prc,v 1.8 2022/01/10 13:34:34 gesconf Exp $      Release : $Revision: 1.8 $        Date :  $Date: 2022/01/10 13:34:34 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAI * FICHIER xzai400sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Interface Info TFM >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	27 Oct 1994	: Creation
* gaborit	28 dec 1994	: modif integration (1.2)
* niepceron	28 oct 1996	: Ajout du niv de qualif (dem/1248) (1.3)
* cluchague     03  07 2001     : Tri par supprime puis autoroute,PR,sens
* cluchague     10  07 2003     : ajout insertion ou autoroute est null pour synthese tfm sur fiche operateur (1.5)
* cluchague     23  09 2004     : ajout localisation apres le nom du type de la fmc (integre a la 1.5)
* F. Lizot      13 Fev 2005 ?   : Ajout des messages provenant de la table TFM_DTX (SAGA)  1.6
* F.Lizot  	27/06/2005 	:  Correction SAGA DEM473 1.7
* JPL		14/06/2007	: Tri des messages par ordre de priorite selon le type Fmc (DEM 654) 1.8
* JPL		27/07/2012	: Tri des messages selon le sens de l'autoroute pour l'A57 (DEM 1040) 1.9
* JMG		30/10/2014	: RVA lot 20 xzai400  1.1
* NDE		15/07/2015	: Liste avec bloc message VL sur BAU (DEM 1140) 1.2 1.3
* JPL		13/10/2015	: Message VL arretes sur point caracteristique en bloc Archive (DEM 1040 complement)  1.4
* PNI		16/07/2019	: jointure avec ACT_TFM sortie du cursor pour eviter deadlock DEM1343 1.5
* PNI		03/09/2019	: pour eviter deadlock DEM1343 1.7
* PNI		10/01/2022	: correctif en cas d'absence de données dans FMC_HIS_DER 1.8
* PNI		12/10/2022 : Mise en archivage des FMC Inc ss concessionnaire SAE-422
* PNI		02/02/2023 : Mise en archivage des FMC Autre & chaussée déteriorée SAE-457 SAE-422------------------------------------------------------*/

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*  Liste des messages a diffuser
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAI400' and type = 'P')
	drop procedure XZAI400
go

create procedure XZAI400
  as 

/*
*
* SP	XZAI400SP
*
* PARAMETRES EN ENTREE :
*
* PARAMETRES EN SORTIE :
*
* XZAITListe
*
* VALEUR RENDUE :
*
* Retourne a l'operateur TFM la liste des messages a diffuser ( intitule evt+numero
* de diffusions horodate premiere diffusion +flag "a supprimer")
*
* Les messages sont tries successivement selon les quatres criteres suivants :
*  1) messages a archiver : si le message concerne un veh en bau et qu'il a deja
*      ete diffuse
*  2) messages dont la diffusion est a supprimer (evenement termine) ;
*  3) priorite du type d'evenement (avec evenements Strada classes en tete) ;
*  4) localisation de l'evenement (autoroute, sens, Pr), les evenements
*      provenant des operateurs etant en tete de liste (autoroute nulle).
*
*
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK
*
* CONDITIONS D'UTILISATION :
*
* FONCTION :
*
* MECANISMES :
*
------------------------------------------------------*/

	/* Donnees extraites de la base */
	declare @vl_num_evt		int,
		@vl_cle_evt		tinyint,
		@vl_num_type		smallint,
		@vl_nb_diff		smallint,
		@vl_heure_succes	datetime,
		@vl_num_action		int,
		@vl_site		T_SITE,
		@vl_niveau		tinyint,
		@vl_autoroute		T_AUTOROUTE,
		@vl_PR			T_PR,
		@vl_sens		T_SENS,
		@vl_ptcar		tinyint,
		@vl_inversee		smallint,
		@vl_txt_msg_1		char(250),
		@vl_txt_msg_2		char(250),
		@vl_code_partenaire	char(10),
		@vl_ref_datex		char(50),
		@vl_vr			tinyint,
		@vl_vm1			tinyint,
		@vl_vm2			tinyint,
		@vl_vl			tinyint,
		@vl_bau			tinyint,

		@vl_heure_lance_fin	datetime,
		@vl_nom_type		T_NOM,
		@vl_nom_autoroute	T_NOM

	/* Donnees calculees */
	declare @vl_supprime		bit,
		@vl_priorite_arc	smallint,
		@vl_priorite_evt	smallint,
		@vl_priorite_loc	smallint,
		@vl_type_et_loc		char(100),
		@vl_deja_diffuse 	bit,
		@vl_nb			int,
		@vl_message_archive	bit

	select @vl_message_archive = XDC_FAUX

	/* Creer une table temporaire stockant les donnees retournees */
	create table #TFM
	(
		/* les donnees retournees */
		numero			int		NULL,
		cle			tinyint		NULL,
		type_et_loc		char(100)	NULL,
		nombre_diffusions	smallint	NULL,
		heure_diffusion		datetime	NULL,
		supprime		bit,
		action			int		NULL,
		sit			tinyint		NULL,
		niveau			tinyint		NULL,
		message_1		char(250)	NULL,	/* Modif SAGA */
		message_2		char(250)	NULL,	/* Modif SAGA */
		code_partenaire		char(10)	NULL,	/* Modif SAGA */
		reference_datex		char(50)	NULL,	/* Modif SAGA */
		vr			tinyint		NULL,
		vm1			tinyint		NULL,
		vm2			tinyint		NULL,
		vl			tinyint		NULL,
		bau			tinyint		NULL,
		
		/* les criteres de tri */
		priorite_arc		smallint,
		priorite_evt		smallint,
		priorite_loc		smallint,
		PR			int		NULL,
		type			smallint	NULL,
		deja_diffuse		bit
	)


	/* Recherche des actions TFM en cours */
	declare Pointeur_action_tfm cursor for
	select	
		ACT.numero, ACT.sit, ACT.heure_succes, ACT.heure_lancement_fin,ACT.evenement,ACT.cle
	from EXP..ACT_GEN ACT
	where ACT.type 		= XDC_ACT_TFM 
	  and ACT.heure_fin	= null

	open Pointeur_action_tfm
	fetch Pointeur_action_tfm into
		@vl_num_action, @vl_site, @vl_heure_succes, @vl_heure_lance_fin,@vl_num_evt,@vl_cle_evt

	while @@sqlstatus = 0
	begin
		/* La diffusion doit se terminer ssi. le lancement fin est daté */
		if @vl_heure_lance_fin <> null
			select @vl_supprime = XDC_VRAI
		else
			select @vl_supprime = XDC_FAUX

		select  @vl_num_type=FMC.type, @vl_nom_type=TYP.nom,
			@vl_autoroute=DER.autoroute, @vl_PR=DER.PR, @vl_sens=DER.sens, @vl_ptcar=DER.point_caracteristique,
			@vl_vr=DER.VR, @vl_vm1=DER.VM1, @vl_vm2=DER.VM2, @vl_vl=DER.VL, @vl_bau=DER.BAU
		from 
			EXP..FMC_GEN FMC,
			EXP..FMC_HIS_DER DER,
			CFG..TYP_FMC TYP
		where 
			  FMC.numero        = @vl_num_evt
			  and FMC.cle           = @vl_cle_evt
			  and DER.numero        = FMC.numero
			  and DER.cle           = FMC.cle
			  and TYP.numero        = FMC.type
		if @@rowcount = 0
		begin	
			select  @vl_autoroute=null
			select  @vl_num_type=FMC.type, @vl_nom_type=TYP.nom  from
			                        EXP..FMC_GEN FMC,
						CFG..TYP_FMC TYP
			where FMC.numero        = @vl_num_evt
			                          and FMC.cle           = @vl_cle_evt
						  and TYP.numero        = FMC.type
		end

		select @vl_nb_diff=nombre_diffusions,@vl_niveau=niveau
		from EXP..ACT_TFM
		where actio=@vl_num_action and sit= @vl_site
		

		if @vl_nb_diff > 0
			select @vl_deja_diffuse = XDC_VRAI
		else 
		begin
			select @vl_nb = count(*) from EXP..ACT_GEN ACT,  EXP..ACT_TFM TFM
	  where TFM.actio 	= ACT.numero
	  and TFM.sit		= ACT.sit
	  and ACT.evenement = @vl_num_evt
	  and ACT.cle =  @vl_cle_evt
			and TFM.nombre_diffusions > 0

			if  @vl_nb > 0
				select @vl_deja_diffuse = XDC_VRAI
			else
				select @vl_deja_diffuse = XDC_FAUX
		end

		/* Définir la priorité selon le type d'événement */
		if @vl_num_type in (
		             XZAEC_FMC_Accident,
		             XZAEC_FMC_Contresens,
		             XZAEC_FMC_VehFeu,
		             XZAEC_FMC_VehMarcheArr )
			select @vl_priorite_evt = 0
		else if @vl_num_type in (
		             XZAEC_FMC_Delestage,
		             XZAEC_FMC_Deviation,
		             XZAEC_FMC_PosteOperateur,
		             XZAEC_FMC_Meteo,
		             XZAEC_FMC_VehArrete,
		             XZAEC_FMC_VehPanne,
		             XZAEC_FMC_Escargot,
		             XZAEC_FMC_QueueBouchon,
		             XZAEC_FMC_FermetureEch,
		             XZAEC_FMC_Eboulement,
		             XZAEC_FMC_Incendie,
		             XZAEC_FMC_Inondation,
		             XZAEC_FMC_Obstacle,
		             XZAEC_FMC_FlaqueHuile,
		             XZAEC_FMC_AnimalErrant,
		             XZAEC_FMC_EvtHorsReseau,
		             XZAEC_FMC_Salage,
		             XZAEC_FMC_PietonChaussee,
		             XZAEC_FMC_Autre,
		             XZAEC_FMC_TraitChaussees,
		             XZAEC_FMC_EchDeconseille,
		             XZAEC_FMC_ChausseeGliss )
			select @vl_priorite_evt = 1
		else
			select @vl_priorite_evt = 2


		/* Si l'evenement provient d'un operateur */
		if @vl_num_type = XZAEC_FMC_PosteOperateur
		begin
			/* alors il n'a pas de localisation */
			select @vl_type_et_loc = @vl_nom_type + ' SYNTHESE TRANSMISE PAR LE CI'
			select @vl_priorite_loc = 0
		end
		else
		begin
			/* sinon construire le libelle de localisation */
			if  @vl_autoroute <> null
			begin	
			select @vl_nom_autoroute = nom from CFG..RES_AUT where numero = @vl_autoroute
			select @vl_inversee = inversee from CFG..RES_AUT_INV where numero = @vl_autoroute
			select @vl_type_et_loc = @vl_nom_type + ' ' + rtrim (@vl_nom_autoroute) + '-' +
			                                              convert (char(1), @vl_sens) + '-' +
		                                                      str (convert (float, @vl_PR) / 1000,7,3)
			end
			else
				select @vl_type_et_loc = @vl_nom_type + ' ' 

			/* et définir la priorité de localisation selon l'autoroute, puis le sens */
			if @vl_autoroute = 1 and @vl_sens in (XDC_SENS_INCONNU, XDC_SENS_SUD, XDC_SENS_SORTIE_SUD, XDC_SENS_ENTREE_SUD) /*A8*/
				select @vl_priorite_loc = 1000
			else if @vl_autoroute = 1 
				select @vl_priorite_loc = 2100
			else if @vl_autoroute = 2 /*A500*/
				select @vl_priorite_loc = 2000
			else if @vl_autoroute = 3 /*A50*/
				select @vl_priorite_loc = 4000
			else if @vl_autoroute = 4 /*A501*/
				select @vl_priorite_loc =  7000
			else if @vl_autoroute = 5 /*A51*/
				select @vl_priorite_loc =  8000
			else if @vl_autoroute = 6 /*A52*/
				select @vl_priorite_loc = 5000
			 else if @vl_autoroute = 7 /*A520*/
				select @vl_priorite_loc = 6000
			else if @vl_autoroute = 8 /*A57*/
				select @vl_priorite_loc = 3000
			else
				select @vl_priorite_loc = 9000

			/* donner la priorite aux evenements dans le sens Sud ou couvrant les deux sens */
			if @vl_sens in (XDC_SENS_INCONNU, XDC_SENS_SUD, XDC_SENS_SORTIE_SUD, XDC_SENS_ENTREE_SUD)
			begin
				if @vl_autoroute != 8
					select @vl_priorite_loc = @vl_priorite_loc + 1
				else
					select @vl_priorite_loc = @vl_priorite_loc + 2
				if (@vl_inversee = XDC_PR_INV)
					select @vl_PR = (- @vl_PR)
			end
			else
			begin
				if @vl_autoroute != 8
					select @vl_priorite_loc = @vl_priorite_loc + 2
				else
					select @vl_priorite_loc = @vl_priorite_loc + 1
				if (@vl_inversee = XDC_PR_NOR)
					select @vl_PR = (- @vl_PR)
			end
		end
		
		/*A marquer les messages pour vehicules sur BAU deja diffuses comme archives */
		select @vl_priorite_arc = XDC_TMA_BLOC_MSG

		if @vl_num_type in (XZAEC_FMC_VehArrete, XZAEC_FMC_VehPanne)
		   and @vl_deja_diffuse = XDC_VRAI
		   and ( @vl_ptcar = XDC_POINT_CARACT_AIRE  or
			( ( @vl_ptcar is NULL  or  @vl_ptcar = XDC_POINT_CARACT_AUCUN  or  @vl_ptcar = XDC_POINT_CARACT_PEAGE )
			   and @vl_bau in (XDC_VOIE_BLOQUEE, XDC_VOIE_BLOQUEE_NEUTRALISEE, XDC_VOIE_NEUTRALISEE)
			   and @vl_vr in (XDC_VOIE_INEXISTANTE, XDC_VOIE_SENS_NORMAL)
			   and @vl_vm1 in (XDC_VOIE_INEXISTANTE, XDC_VOIE_SENS_NORMAL)
			   and @vl_vm2 in (XDC_VOIE_INEXISTANTE, XDC_VOIE_SENS_NORMAL)
			   and @vl_vl in (XDC_VOIE_INEXISTANTE, XDC_VOIE_SENS_NORMAL)
			)
		       )
		begin
			if (select count(type_de_vehicule) from EXP..FMC_VEH
			    where numero=@vl_num_evt and cle=@vl_cle_evt
			      and numero_vehicule=1 and type_de_vehicule=XZAEC_VEH_VL) > 0
			begin
				select @vl_priorite_arc = XDC_TMA_BLOC_ARC
				select @vl_message_archive = XDC_VRAI
			end
		end

		if @vl_num_type = XZAEC_FMC_IncSsConces
		   and @vl_deja_diffuse = XDC_VRAI
		begin
			select @vl_priorite_arc = XDC_TMA_BLOC_ARC
			select @vl_message_archive = XDC_VRAI
		end
		if @vl_num_type = XZAEC_FMC_ChausseDeter
		   and @vl_deja_diffuse = XDC_VRAI
		begin
			select @vl_priorite_arc = XDC_TMA_BLOC_ARC
			select @vl_message_archive = XDC_VRAI
		end
		if @vl_num_type = XZAEC_FMC_Autre
		   and @vl_deja_diffuse = XDC_VRAI
		begin
			select @vl_priorite_arc = XDC_TMA_BLOC_ARC
			select @vl_message_archive = XDC_VRAI
		end

		/* Stocker les données à retourner et passer à l'événement suivant */
		insert #TFM
		values ( @vl_num_evt, @vl_cle_evt, @vl_type_et_loc,
		         @vl_nb_diff, @vl_heure_succes,
		         @vl_supprime,
		         @vl_num_action, @vl_site, @vl_niveau,
		         null, null, null, null,
			 null, null, null, null, null,
		         @vl_priorite_arc, @vl_priorite_evt, @vl_priorite_loc, @vl_PR , @vl_num_type, @vl_deja_diffuse)

		
		fetch Pointeur_action_tfm into
			@vl_num_action, @vl_site, @vl_heure_succes, @vl_heure_lance_fin,@vl_num_evt,@vl_cle_evt
	end
	close Pointeur_action_tfm


	/* Modif SAGA : ajout des messages de la table TFM_DTX */
	insert into #TFM
	select  0,
		0,
		'MESSAGE SAGA',
		nombre_diffusions,
		heure_diffusion,
		supprime,
		0,
		0,
		0,
		texte_message_1,
		texte_message_2,
		code_partenaire,
		reference_datex,
		0,
		0,
		0,
		0,
		0,
		0,
		-1,                   /* evenement inconnu : les classer en tete */
		0,                    /* autoroute inconnue : les classer en tete */
		0,
		0,
		XDC_FAUX
	from EXP..TFM_DTX
	where supprime = 0

	/* Ajout de l'entete des messages archives */
	-- if @vl_message_archive = XDC_VRAI
		insert into #TFM
		select  NULL,
			NULL,
			'Archivage',
			NULL,
			NULL,
			0,
			0,
			0,
			NULL,
			'',
			'',
			'',
			'',
			0,
			0,
			0,
			0,
			0,
			XDC_TMA_BLOC_ENT, /* priorite_arc = 1, place entre les messages a diffuser (0) et les messages archives (2) */
			-1,
			0,
			0,
			0,
			XDC_FAUX

	select  numero, cle, type_et_loc,
		nombre_diffusions, heure_diffusion,
		supprime,
		action, sit, niveau,
		message_1, message_2, code_partenaire, reference_datex,type,deja_diffuse, 
		priorite_arc
	from #TFM
	order by priorite_arc, supprime desc, priorite_loc, PR, priorite_evt

	return ( XDC_OK )
go
