/*E*/
/*  Fichier : $Id: xzaf10sp.prc,v 1.7 1995/06/01 18:52:44 gaborit Exp $      Release : $Revision: 1.7 $        Date : $Date: 1995/06/01 18:52:44 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzaf10sp.prc
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
* C.T.	20/01/95	: Modif clause de recherche des actions
*                         (ajout evt clos) (V 1.2)
* C.T.	31/01/95	: Modif filtre sur evenement (V 1.3)
* C.T.	31/01/95	: Modif filtre sur evenement (V 1.4)
* C.T.	06/02/95	: Vidage de la table des evts (V 1.5)
* C.T.	05/04/95	: Ajout de la prise en compte des evts meteo (V 1.6)
* C.T.	30/05/95	: Rechercher les actions avec l'heure
*                         de succes au lieu de celle de lancement (V 1.7)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* SP	XZAF10_Rechercher_Evts_Actions
* 
* Arguments en entree
* XDY_Horodate va_Horodate_in 
* 
* Arguments en sortie
* 
* Service rendu
* Rechercher tous les actions et evenements en cours a
* la date precisee et clos actuellement
*
* Code retour
* XDC_OK
* XDC_NOK	
* XDC_ARG_INV	: argument invalide	
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
*
---------------------------------------------------*/

use PRC
go

/* table de travail pour la formation */
if exists (select * from sysobjects where name = 'ACT_FRM' and type = 'U')
	drop table ACT_FRM
go

if exists (select * from sysobjects where name = 'EVT_FRM' and type = 'U')
	drop table EVT_FRM
go

create table EVT_FRM
(	numero				int,
	cle				tinyint,
	type				tinyint,
	horodate_validation		datetime,
	autoroute			T_AUTOROUTE	null,
	PR				T_PR	null,
	sens				T_SENS	null,
	point_caracteristique		tinyint	null,
	numero_point_caracteristique	tinyint	null,
	position_sur_aire_ou_echangeur	tinyint	null,
	VR				T_VOIE	null,
	VM2				T_VOIE	null,
	VM1				T_VOIE	null,
	VL				T_VOIE	null,
	BAU				T_VOIE	null,
	VR_I				T_VOIE	null,
	VM2_I				T_VOIE	null,
	VM1_I				T_VOIE	null,
	VL_I				T_VOIE	null,
	BAU_I				T_VOIE	null,
	spid				int
)
go

create table ACT_FRM
(	numero				int,
	sit				T_SITE,
	type				tinyint,
	evenement			int,
	cle				tinyint,
	spid				int
)
go

create procedure  XZAF;10
	@va_Horodate_in		datetime = null
as
declare @vl_NumEvt int, @vl_CleEvt tinyint, @vl_Type smallint,
	@vl_EvtInvalide bit

	/*A Test des paramÉtres d'appel */
	if @va_Horodate_in=null
		return XDC_ARG_INV
	
	/*A Supprimer les evenements restant de cette session */
	/*! utile en cas de probleme des synoptiques          */
	delete EVT_FRM where spid = @@spid
	delete ACT_FRM where spid = @@spid

	/*A Recherche des evenements en cours a la date precisee */
	/*A de type autre que delestage, deviation, operateur,   */
	/*A hors reseau                                          */
	select 
		numero,
		cle,
		type
	into #EVT
	from HIS..FMC_GEN
	where	type != XZAEC_FMC_Delestage and
		type != XZAEC_FMC_Deviation and
		type != XZAEC_FMC_PosteOperateur and
		type != XZAEC_FMC_EvtHorsReseau and
		type != XZAEC_FMC_FermetureEch and
		cloture > @va_Horodate_in and
		fin > @va_Horodate_in and
		(debut <= @va_Horodate_in or
		 (debut_prevu <= @va_Horodate_in and
		  type = XZAEC_FMC_Travaux))

	declare Pointeur_evt cursor
	for select 
		numero,
		cle,
		type
	from #EVT

	open Pointeur_evt
	fetch Pointeur_evt into @vl_NumEvt, @vl_CleEvt, @vl_Type
	while @@sqlstatus = 0
	begin
		select @vl_EvtInvalide = XDC_NON

		/* supprimer de la table des evenements clos a traiter en formation */
		/* les bouchons dont une des extremites n'est pas close             */
		if @vl_Type = XZAEC_FMC_QueueBouchon
		begin
			if not exists (select * from #EVT, HIS..FMC_GEN
					where HIS..FMC_GEN.numero = @vl_NumEvt and
						HIS..FMC_GEN.cle = @vl_CleEvt and
						#EVT.numero = HIS..FMC_GEN.cause and
						#EVT.cle = HIS..FMC_GEN.cle_cause)
				select @vl_EvtInvalide = XDC_OUI
		end
		else if @vl_Type = XZAEC_FMC_TeteBouchon
		begin
			if not exists (select * from #EVT, HIS..FMC_GEN
					where HIS..FMC_GEN.type = XZAEC_FMC_QueueBouchon and
						HIS..FMC_GEN.cause = @vl_NumEvt and
						HIS..FMC_GEN.cle_cause = @vl_CleEvt and
						#EVT.numero = HIS..FMC_GEN.numero and
						#EVT.cle = HIS..FMC_GEN.cle)
				select @vl_EvtInvalide = XDC_OUI
		end

		/* recherche l'historique de l'evenement juste avant la date */
		if @vl_EvtInvalide = XDC_NON
			insert EVT_FRM
			select 
				numero,
				cle,
				@vl_Type,
				horodate_validation,
				autoroute,
				PR,
				sens,
				point_caracteristique,
				numero_point_caracteristique,
				position_sur_aire_ou_echangeur,
				VR,
				VM2,
				VM1,
				VL,
				BAU,
				VR_I,
				VM2_I,
				VM1_I,
				VL_I,
				BAU_I,
				@@spid
			from HIS..FMC_HIS
			where	numero = @vl_NumEvt and cle = @vl_CleEvt and
				horodate_validation in 
				(select max(horodate_validation) from HIS..FMC_HIS
				 where numero = @vl_NumEvt and cle = @vl_CleEvt and
					horodate_validation <= @va_Horodate_in)

		/* passage a l'evenement suivant */
		fetch Pointeur_evt into @vl_NumEvt, @vl_CleEvt, @vl_Type
	end
	close Pointeur_evt


	/*A Recherche des actions en cours a la date precisee */
	insert ACT_FRM( numero, sit, type, evenement, cle, spid)
	select 
		HIS..ACT_GEN.numero,
		HIS..ACT_GEN.sit,
		HIS..ACT_GEN.type,
		HIS..ACT_GEN.evenement,
		HIS..ACT_GEN.cle,
		@@spid
	from HIS..ACT_GEN, EVT_FRM
	where 	HIS..ACT_GEN.evenement = EVT_FRM.numero and 
		HIS..ACT_GEN.cle = EVT_FRM.cle and
		HIS..ACT_GEN.heure_succes <= @va_Horodate_in and
		HIS..ACT_GEN.heure_fin > @va_Horodate_in 

	return XDC_OK
go
