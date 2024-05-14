/*E*/
/*Fichier : $Id: xzai04sp.prc,v 1.9 2012/08/06 14:36:22 gesconf Exp $      Release : $Revision: 1.9 $        Date :  $Date: 2012/08/06 14:36:22 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAI * FICHIER xzai04sp.prc
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
------------------------------------------------------*/

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

create procedure XZAI;04
  as 

/*
*
* SP	XZAI04SP
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
* Les messages sont tries successivement selon les trois criteres suivants :
*  1) messages dont la diffusion est a supprimer (evenement termine) ;
*  2) priorite du type d'evenement (avec evenements Strada classes en tete) ;
*  3) localisation de l'evenement (autoroute, sens, Pr), les evenements
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
		@vl_inversee		smallint,
		@vl_txt_msg_1		char(250),
		@vl_txt_msg_2		char(250),
		@vl_code_partenaire	char(10),
		@vl_ref_datex		char(50),

		@vl_heure_lance_fin	datetime,
		@vl_nom_type		T_NOM,
		@vl_nom_autoroute	T_NOM

	/* Donnees calculees */
	declare @vl_supprime		bit,
		@vl_priorite_evt	smallint,
		@vl_priorite_loc	smallint,
		@vl_type_et_loc		char(100)


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

		/* les criteres de tri */
		priorite_evt		smallint,
		priorite_loc		smallint,
		PR			int		NULL
	)


	/* Recherche des actions TFM en cours */
	declare Pointeur_action_tfm cursor for
	select	FMC.numero, FMC.cle, FMC.type, TYP.nom,
		ACT.numero, ACT.sit, ACT.heure_succes, ACT.heure_lancement_fin,
		TFM.nombre_diffusions, TFM.niveau,
		DER.autoroute, DER.PR, DER.sens
	from EXP..ACT_GEN ACT,
	     EXP..ACT_TFM TFM,
	     EXP..FMC_GEN FMC,
	     EXP..FMC_HIS_DER DER,
	     CFG..TYP_FMC TYP
	where ACT.heure_fin	= null
	  and TFM.actio 	= ACT.numero
	  and TFM.sit		= ACT.sit
	  and FMC.numero	= ACT.evenement
	  and FMC.cle		= ACT.cle
	  and DER.numero	= FMC.numero
	  and DER.cle		= FMC.cle
	  and TYP.numero	= FMC.type

	open Pointeur_action_tfm
	fetch Pointeur_action_tfm into
		@vl_num_evt, @vl_cle_evt, @vl_num_type, @vl_nom_type,
		@vl_num_action, @vl_site, @vl_heure_succes, @vl_heure_lance_fin,
		@vl_nb_diff, @vl_niveau,
		@vl_autoroute, @vl_PR, @vl_sens

	while @@sqlstatus = 0
	begin
		/* La diffusion doit se terminer ssi. le lancement fin est daté */
		if @vl_heure_lance_fin <> null
			select @vl_supprime = XDC_VRAI
		else
			select @vl_supprime = XDC_FAUX


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
			select @vl_nom_autoroute = nom from CFG..RES_AUT where numero = @vl_autoroute
			select @vl_inversee = inversee from CFG..RES_AUT_INV where numero = @vl_autoroute
			select @vl_type_et_loc = @vl_nom_type + ' ' + rtrim (@vl_nom_autoroute) + '-' +
			                                              convert (char(1), @vl_sens) + '-' +
		                                                      str (convert (float, @vl_PR) / 1000,7,3)

			/* et définir la priorité de localisation selon l'autoroute, puis le sens */
			select @vl_priorite_loc = 1000 * @vl_autoroute
			/* ranger A57 puis A52 avant l'A50, et l'A520 apres l'A50 */
			if @vl_autoroute = 8                           /* A57 */
				select @vl_priorite_loc = 2100
			else if @vl_autoroute = 6                      /* A52 */
				select @vl_priorite_loc = 2200
			else if @vl_autoroute = 7                      /* A520 */
				select @vl_priorite_loc = 3100

			/* donner la priorite aux evenements dans le sens Sud ou couvrant les deux sens */
			if @vl_sens in (XDC_SENS_INCONNU, XDC_SENS_SUD, XDC_SENS_SORTIE_SUD, XDC_SENS_ENTREE_SUD)
			begin
				select @vl_priorite_loc = @vl_priorite_loc + 1
				if (@vl_inversee = XDC_PR_INV)
					select @vl_PR = (- @vl_PR)
			end
			else
			begin
				select @vl_priorite_loc = @vl_priorite_loc + 2
				if (@vl_inversee = XDC_PR_NOR)
					select @vl_PR = (- @vl_PR)
			end
		end


		/* Stocker les données à retourner et passer à l'événement suivant */
		insert #TFM
		values ( @vl_num_evt, @vl_cle_evt, @vl_type_et_loc,
		         @vl_nb_diff, @vl_heure_succes,
		         @vl_supprime,
		         @vl_num_action, @vl_site, @vl_niveau,
		         null, null, null, null,
		         @vl_priorite_evt, @vl_priorite_loc, @vl_PR )

		fetch Pointeur_action_tfm into
			@vl_num_evt, @vl_cle_evt, @vl_num_type, @vl_nom_type,
			@vl_num_action, @vl_site, @vl_heure_succes, @vl_heure_lance_fin,
			@vl_nb_diff, @vl_niveau,
			@vl_autoroute, @vl_PR, @vl_sens
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
		-1,                   /* evenement inconnu : les classer en tete */
		0,                    /* autoroute inconnue : les classer en tete */
		0
	from EXP..TFM_DTX
	where supprime = 0


	select  numero, cle, type_et_loc,
		nombre_diffusions, heure_diffusion,
		supprime,
		action, sit, niveau,
		message_1, message_2, code_partenaire, reference_datex
	from #TFM
	order by supprime desc, priorite_evt, priorite_loc, PR

	return ( XDC_OK )
go
