/*E*/
/*Fichier :  $Id: xzag01sp.prc,v 1.20 2021/05/04 13:29:13 pc2dpdy Exp $      Release : $Revision: 1.20 $        Date : $Date: 2021/05/04 13:29:13 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzag01sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Purge des bases
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	02/11/94	: Creation	(V 1.1)
* C.T.	02/11/94	: Modif ajout sauvegarde des appels
*                         horaire	(V 1.2)
*                         non teste
* C.T.	11/01/95	: Modif ajout premiere disponibilite (V 1.3)
*                         (pb de performance)
*                         non teste
* C.T.	21/03/95	: Ajout de transactions pour les delete (V 1.4)
* C.T.	21/03/95	: Ajout de transactions pour les insert mesures (V 1.5)
* C.T.	03/05/96	: Supprimer les actions sur MES_NIV (V1.6)
* P.N.	09/10/96	: Ajouter les actions sur MES_KCS QPC et NEC (dem/1227) (V1.7)
* P.N.	10/10/96	: Ajouter les actions sur MES_MET (dem/1232) (V1.8)
* P.N.  24/02/97	: Ajouter les actions sur ZDP_6MN ZDP_HEU (dem/1396) (1.9)
* C.T.	02/02/98	: Ajouter les actions sur ACT_PAL (V1.10)
* P.N.	27/01/99	: Ajouter les actions sur ACT_EXP ACT_EXPTFM ACT_PMVDAT et FMC_DAT (V1.11)
* P.C   06/02/02        : correction pour HIS V1.8
* JPL	11/04/07	: Prise en compte table FMC_NAT pour la classe Nature (DEM 639) V1.13
* AAZ   28/08/2007  : Ajout action domaine SIG, ECL, VEN, ENR et INC DEM 663
* JMG	18/09/11 : ajout ALT_LOC  1.15
* VR	12/12/11	: Ajout PRV(DEM/1016) V1.16
* JPL	13/09/12	: Prise en compte table FMC_INC pour la classe Incendie (DEM 1037)  1.17
* LCL	02/03/18	: Ajout controleurs de feux CTRL_FEUX 1.18 DEM1284
* ABE	17/12/20	: Ajout IMU DEM-SAE155 1.19
* ABE	07/04/21	: Ajout PAU SONO DEM-SAE244 1.20
* GGY	11/12/23	: Ajout PIC (DEM-483)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Transfere les donnees du mois dans la base de sauvegarde
* /restauration
* 
* Sequence d'appel
* SP	XZAG01_Transfert_Mois
* 
* Arguments en entree
* XDY_Mot	va_Mois_in
* XDY_Mot	va_Annee_in
* 
* Arguments en sortie
* 
* Code retour
* XDC_OK	: probleme a l'insertion dans la base de sauvegarde
*                 ou donnee non trouvee
* XDC_ARG_INV	: parametre d'entree null ou date demandee trop proche
* XDC_PRC_INC	: procedure appelee inexistante en base
* 
* Conditions d'utilisation
* - Il faut que la date courante soit au moins superieure a
* au mois specifie plus 7 jours sinon erreur de parametre
* - Le mois vaut de 1 a 12
* - L'annee vaut au moins 1900
* - La periode de sauvegarde est du debut de mois - 7 jours a la
* la fin du mois + 7 jours
* 
* Fonction
* Copier de la base historique dans la base sauvegarde/restauration
* les evenements clos durant le mois specifie et les actions relatives
* a ces evenements, les etats des equipements et les mesures de trafic.
-------------------------------------------------------*/

use PRC
go

create procedure XZAG;01
	@va_Mois_in	smallint = null,
	@va_Annee_in	smallint = null
as
#ifdef HIST
	declare @vl_Status int, @vl_NumEvt int, @vl_CleEvt tinyint,
		@vl_NumCause int, @vl_CleCause tinyint,
		@vl_NumEqt smallint, @vl_TypeEqt tinyint,
		@vl_Horodate1900 datetime, @vl_DateCourante datetime,
		@vl_HorodateDebut datetime, @vl_HorodateFin datetime, 
		@vl_ClasseEvt tinyint, @vl_DeltaJours int

	/*A controle du parametre en entree */
	if @va_Mois_in = null or @va_Annee_in = null or 
	   @va_Mois_in > 12 or @va_Mois_in < 1 or @va_Annee_in < 1900
		return XDC_ARG_INV

	/* date de debut et de fin de sauvegarde */
	select	@vl_DateCourante = getdate(),
		@vl_Horodate1900 = dateadd(year, @va_Annee_in - 1900, "01/01/1900 00:00:00")
	select	@vl_HorodateDebut = dateadd(month, @va_Mois_in - 1, @vl_Horodate1900),
		@vl_HorodateFin = dateadd(month, @va_Mois_in, @vl_Horodate1900)
	select	@vl_DeltaJours = datediff(day, @vl_HorodateFin, @vl_DateCourante)
	select	@vl_HorodateDebut = dateadd(day, -7, @vl_HorodateDebut),
		@vl_HorodateFin = dateadd(day, 7, @vl_HorodateFin)

	/*A Controle que la date specifiee est anterieure a la date courante */
	/*A moins 7 jours                                                    */
	if @vl_DeltaJours < 7
		return XDC_ARG_INV

	/*A Effacer toutes les donnees actuellement dans la base sauvegarde */
	begin tran
		delete SVG..FMC_GEN
	commit tran
	begin tran
		delete SVG..FMC_HIS
	commit tran
	begin tran
		delete SVG..FMC_ACC
	commit tran
	begin tran
		delete SVG..FMC_VEH
	commit tran
	begin tran
		delete SVG..FMC_VLN
	commit tran
	begin tran
		delete SVG..FMC_BOU
	commit tran
	begin tran
		delete SVG..FMC_DEL
	commit tran
	begin tran
		delete SVG..FMC_BAS
	commit tran
	begin tran
		delete SVG..FMC_OPR
	commit tran
	begin tran
		delete SVG..FMC_MET
	commit tran
	begin tran
		delete SVG..FMC_TRF
	commit tran
	begin tran
		delete SVG..FMC_TRH
	commit tran
	begin tran
		delete SVG..FMC_MNF
	commit tran
	begin tran
		delete SVG..FMC_ECH
	commit tran
	begin tran
		delete SVG..FMC_CNC
	commit tran
	begin tran
		delete SVG..FMC_NAT
	commit tran
	begin tran
		delete SVG..FMC_INC
	commit tran
	begin tran
		delete SVG..FMC_COM
	commit tran
	begin tran
		delete SVG..FMC_DAT
	commit tran
	begin tran
		delete SVG..ACT_GEN
	commit tran
	begin tran
		delete SVG..ACT_APL
	commit tran
	begin tran
		delete SVG..ACT_TFM
	commit tran
	begin tran
		delete SVG..ACT_TXT_TFM
	commit tran
	begin tran
		delete SVG..ACT_PMV
	commit tran
	begin tran
		delete SVG..ACT_PRV
	commit tran
	begin tran
		delete SVG..ACT_PIC
	commit tran
	begin tran
		delete SVG..ACT_PAL
	commit tran
	begin tran
		delete SVG..ACT_EXP_ADA
	commit tran
	begin tran
		delete SVG..ACT_TFM_EXP
	commit tran
	begin tran
		delete SVG..ACT_PMV_ADA
	commit tran
	begin tran
		delete SVG..ACT_FRG
	commit tran
	begin tran
		delete SVG..ACT_RMQ
	commit tran
	begin tran
		delete SVG..ACT_NAV
	commit tran
	begin tran
		delete SVG..ACT_TUB
	commit tran
	begin tran
		delete SVG..ACT_ECH
	commit tran
	begin tran
		delete SVG..ACT_INT
	commit tran
	begin tran
		delete SVG..ACT_SIS
	commit tran
	begin tran
		delete SVG..ACT_DEP
	commit tran
	begin tran
		delete SVG..ACT_GAR
	commit tran
	begin tran
		delete SVG..ACT_FAX
	commit tran
	begin tran
		delete SVG..ALT_EXP
	commit tran
	begin tran
		delete SVG..ALT_LOC
	commit tran
	begin tran
		delete SVG..EQT_DSP
	commit tran
	begin tran
		delete SVG..MES_TRA
	commit tran
	begin tran
		delete SVG..MES_POI
	commit tran
	begin tran
		delete SVG..MES_KCS
	commit tran
	begin tran
		delete SVG..MES_NEC
	commit tran
	begin tran
		delete SVG..MES_QPC
	commit tran
	begin tran
		delete SVG..MES_MET
	commit tran
	begin tran
		delete SVG..ZDP_HEU
	commit tran
	/* AAZ 28/08/2007 Ajout suppression action domaine */
	begin tran
		delete SVG..ACT_GTC_SIG
	commit tran
	begin tran
		delete SVG..ACT_GTC_EXP
	commit tran
	begin tran
		delete SVG..ACT_GTC_ECL
	commit tran
	begin tran
		delete SVG..ACT_GTC_VEN
	commit tran
	begin tran
		delete SVG..ACT_GTC_ENR
	commit tran
	begin tran
		delete SVG..ACT_GTC_INC
	commit tran
    /* AAZ */
	begin tran
		delete SVG..ACT_CFE
	commit tran
	begin tran
		delete SVG..ACT_IMU
	commit tran
	begin tran
		delete SVG..ACT_PAU
	commit tran

	/*A Rechercher la liste des evenements clos dans la base d'histo dont */
	/*A la cloture a ete faite durant le mois specifie                    */
	select 
		HIS..FMC_GEN.numero,
		HIS..FMC_GEN.cle,
		HIS..FMC_GEN.cause,
		HIS..FMC_GEN.cle_cause,
		CFG..TYP_FMC.classe
	into #EVT_DU_MOIS
	from HIS..FMC_GEN, CFG..TYP_FMC
	where	HIS..FMC_GEN.cloture >= @vl_HorodateDebut and
		HIS..FMC_GEN.cloture < @vl_HorodateFin and
		CFG..TYP_FMC.numero = HIS..FMC_GEN.type

	declare Pointeur_evt_clos cursor
	for select numero, cle, classe, cause, cle_cause 
	from #EVT_DU_MOIS

	/*A Parcourir la liste de ces evenements */
	open Pointeur_evt_clos
	fetch Pointeur_evt_clos into @vl_NumEvt, @vl_CleEvt, @vl_ClasseEvt, @vl_NumCause,
					@vl_CleCause
	while @@sqlstatus = 0
	begin
		/* Si l'evenement est une consequence et que la cause fait      */
		/* partie de la liste des evenements clos pour le mois specifie */
		/* alors positionner a null la cause pour la procedure de       */
		/* sauvegarde de l'evenement dans la base historique            */
		/* (la cause est automatiquement sauvegardee avant ou apres cet */
		/*  appel)                                                      */
		if @vl_CleCause != null and @vl_NumCause != null and
		   exists (	select * from #EVT_DU_MOIS
				where numero = @vl_NumCause and cle = @vl_CleCause)
			select @vl_CleCause = null, @vl_NumCause = null

		/*B Sauvegarder l'evenement dans la base historique */
		exec @vl_Status = XZAG;12 @vl_NumEvt, @vl_CleEvt, @vl_ClasseEvt,
					@vl_NumCause, @vl_CleCause

		if @vl_Status = null
			return XDC_PRC_INC
		else if @vl_Status != XDC_OK
			return @vl_Status

		/* lecture de l'evenement clos suivant du mois specifie */
		fetch Pointeur_evt_clos into @vl_NumEvt, @vl_CleEvt,
					@vl_ClasseEvt, @vl_NumCause, @vl_CleCause
	end

	/* fermeture definitive du curseur */
	close Pointeur_evt_clos

	/*A Sauvegarder tous les etats de disponibilite des equipements du */
	/*A mois specifie                                                  */
	begin tran
	insert SVG..EQT_DSP
	select * from HIS..EQT_DSP 
	where (horodate < @vl_HorodateFin and horodate > @vl_HorodateDebut)
	commit tran

	/* ajouter la derniere disponibilite des equipements avant */
	/* la date de debut */
	declare Pointeur_evt cursor
	for select 
		numero,
		type
	from CFG..EQT_GEN

	open Pointeur_evt
	fetch Pointeur_evt into @vl_NumEqt, @vl_TypeEqt
	while @@sqlstatus = 0
	begin
		insert SVG..EQT_DSP
		select * from HIS..EQT_DSP
		where equipement = @vl_NumEqt and
			type = @vl_TypeEqt and horodate in
			(select max(horodate) from HIS..EQT_DSP
			 where horodate <= @vl_HorodateDebut and
				equipement = @vl_NumEqt and
				type = @vl_TypeEqt)
		
		fetch Pointeur_evt into @vl_NumEqt, @vl_TypeEqt
	end
	close Pointeur_evt

	/*A Sauvegarder toutes les mesures de trafic du mois specifie */
	begin tran
	insert SVG..MES_TRA
	select * from HIS..MES_TRA 
	where horodate < @vl_HorodateFin and horodate >= @vl_HorodateDebut
	commit tran
	begin tran
	insert SVG..MES_POI
	select * from HIS..MES_POI 
	where horodate < @vl_HorodateFin and horodate >= @vl_HorodateDebut
	commit tran
	begin tran
	insert SVG..MES_KCS
	select * from HIS..MES_KCS 
	where horodate < @vl_HorodateFin and horodate >= @vl_HorodateDebut
	commit tran
	begin tran
	insert SVG..MES_NEC
	select * from HIS..MES_NEC 
	where horodate < @vl_HorodateFin and horodate >= @vl_HorodateDebut
	commit tran
	begin tran
	insert SVG..MES_QPC
	select * from HIS..MES_QPC 
	where horodate < @vl_HorodateFin and horodate >= @vl_HorodateDebut
	commit tran
	begin tran
	insert SVG..MES_MET
	select * from HIS..MES_MET 
	where jour < @vl_HorodateFin and jour >= @vl_HorodateDebut
	commit tran
        insert SVG..ZDP_HEU
	select * from HIS..ZDP_HEU
	where horodate < @vl_HorodateFin and horodate >= @vl_HorodateDebut
	commit tran

	/*A Sauvegarder les appels horaire du mois specifie */
	begin tran
	insert SVG..APL_HOR
	select * from HIS..APL_HOR
	where horodate < @vl_HorodateFin and horodate >= @vl_HorodateDebut
	commit tran

	return XDC_OK
#else
	return XDC_NOK
#endif
go
