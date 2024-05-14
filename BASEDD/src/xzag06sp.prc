/*E*/
/*  Fichier : $Id: xzag06sp.prc,v 1.13 2019/01/17 18:29:39 pc2dpdy Exp $      Release : $Revision: 1.13 $        Date : $Date: 2019/01/17 18:29:39 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzag06sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Purge des bases
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	02/11/94	: Creation	(V 1.1)
* C.T.	02/11/94	: Modif ajout la purge de la table
*                         APL_HOR	(V 1.2)
*                         non teste
* C.T.	05/06/94	: Ajout de commit sur mesures (V 1.3)
* C.T.	03/05/96	: Supprimer les purge dans MES_NIV (V 1.4)
* C.T.	10/09/96	: Supprimer les purge dans MES_NIV (V 1.5)
* P.N.	09/10/96	: Ajouter les purge dans MES_KCS NEC et QPC (dem/1227) (V 1.6)
* P.N.	10/10/96	: Ajouter les purge dans MES_MET (dem/1232) (V 1.7)
* P.N.	23/02/97	: Ajouter les purge dans ZDP_HEU (dem/1396) (V 1.8)
* P.C.	08/03/02	: Correction pour HIS  V1.9
* JPL	13/06/07	: Purge des Fmc d'origine 'xxxESSAIxxx' ou 'xxxSUPPRIMERxxx' (DEM 653) 1.10
* JMG	22/09/11	: ALT_LOC DEM 979 1.11
* JMG	22/08/13 	: ajout REG_TRA 1.12
* JMG	15/08/18	: ajout GAB_TRA 1.13
* LCL	02/10/23	: purge PIT_LOG et REF_PIT_MESSAGE DEM-527
* LCL	11/10/23	: Correction purges Cockpit DEM-527
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Purge des donnees de la base d'historique datant
* d'au moins la date precisee
* 
* Sequence d'appel
* SP	XZAG06_Purge_Base_Historique
* 
* Arguments en entree
* XDY_Datetime		va_Horodate_in	: date de fin de purge
* 
* Arguments en sortie
* 
* Code retour
* XDC_OK
* XDC_ARG_INV	: parametre d'entree null
* XDC_PRC_INC	: procedure XZAG;11 ou XZAE;70 pas en base
* 
* Conditions d'utilisation
* 
* Fonction
* Suppression des lignes anterieures a la date donnee
* pour les evenements clos, les actions liees a ces evenements, 
* les etats d'equipement, les alertes, les mesures.
* (Pour un evenement, la date a considerer est celle de cloture.)
-------------------------------------------------------*/

use PRC
go

create procedure XZAG;06
	@va_Horodate_in		datetime = null
as
#ifdef HIST
	declare @vl_Status int, @vl_NumEvt int, @vl_CleEvt tinyint,
		@vl_NbConsequences int, @vl_DateCourante datetime,
		@vl_SuppressionEvt bit,@vl_TypeAcces bit, 
		@vl_ClasseEvt tinyint, @vl_Bouclage bit,
		@vl_horodateCockpit datetime

	/*A controle du parametre en entree */
	if @va_Horodate_in = null
		return XDC_ARG_INV

	/*A Rechercher dans la base d'historique la liste des evenements */
	/*A    - dont la date de cloture est anterieure a la date specifiee ou */
	/*A    - clotur�s et dont l'origine est 'Essai' ou 'Fmc a supprimer' */
	select 
		HIS..FMC_GEN.numero,
		HIS..FMC_GEN.cle,
		CFG..TYP_FMC.classe
	into #EVT_CLOS
	from HIS..FMC_GEN, CFG..TYP_FMC
	where CFG..TYP_FMC.numero = HIS..FMC_GEN.type
	  and (cloture <= @va_Horodate_in or
	       (cloture <> null and (origine_creation like '%ESSAI%' or
	                             origine_creation like '%SUPPRIMER%') ))

	if @@rowcount = 0
		return XDC_OK

	/* initilisation des variables locales */
	select	@vl_TypeAcces = XZAEC_MemoriserConseq,
		@vl_Bouclage = XDC_NON,
		@vl_DateCourante = getdate()

	declare Pointeur_evt_clos cursor
	for select numero, cle, classe 
	from #EVT_CLOS

	/*A Parcourir la liste de ces evenements en entier tant qu'au */
	/*A moins un evenement est supprime                           */
	open Pointeur_evt_clos
	fetch Pointeur_evt_clos into @vl_NumEvt, @vl_CleEvt, @vl_ClasseEvt
	while @@sqlstatus = 0
	begin
		/*B Rechercher dans les consequences d'un evenement de */
		/*B la liste                                           */
		exec @vl_Status = XZAE;70 @vl_NumEvt, @vl_CleEvt, 
					@vl_DateCourante, @vl_TypeAcces

		/* Rechercher le nombre de consequences */
		select @vl_NbConsequences = count(*)
		from LISTE_CONSEQ
		where spid = @@spid

		/* supprimer les consequences retournees par XZAE;70 */
		delete LISTE_CONSEQ where spid = @@spid

		/*B Si l'evenement lu n'a pas de consequences alors      */
		/*B effacer tout ce qui est relatif a cet evenement      */
		if @vl_NbConsequences = 0
		begin
			exec @vl_Status = XZAG;11 @vl_NumEvt, @vl_CleEvt, 
						@vl_ClasseEvt

			if @vl_Status = null
				return XDC_PRC_INC

			select @vl_Bouclage = XDC_OUI
		end

		/* lecture de l'evenement clos suivant datant d'au moins */
		/* la date precisee                                      */
		fetch Pointeur_evt_clos into @vl_NumEvt, @vl_CleEvt,
					@vl_ClasseEvt
		
		/* si la liste a ete parcouru en entier et au moins */
		/* un evenement a ete supprime de la liste          */
		/* recommencer l'operation sur la liste             */
		if @@sqlstatus = 2 and @vl_Bouclage = XDC_OUI
		begin
			/* fermer le curseur */
			close Pointeur_evt_clos

			/* repositionner l'indicateur de suppression */
			/* d'evenement a faux                        */
			select @vl_Bouclage = XDC_NON

			/* Rechercher les evenements restant dans la base   */
			/* d'historique, qui n'ont pas encore ete supprimes */
			delete #EVT_CLOS
			insert #EVT_CLOS (numero, cle, classe)
			select 
				HIS..FMC_GEN.numero,
				HIS..FMC_GEN.cle,
				CFG..TYP_FMC.classe
			from HIS..FMC_GEN, CFG..TYP_FMC
			where CFG..TYP_FMC.numero = HIS..FMC_GEN.type
			  and (cloture <= @va_Horodate_in or
			       (cloture <> null and (origine_creation like '%ESSAI%' or
			                             origine_creation like '%SUPPRIMER%') ))

			/* se repositionner sur le debut de la liste */
			open Pointeur_evt_clos
			fetch Pointeur_evt_clos into @vl_NumEvt, @vl_CleEvt,
					@vl_ClasseEvt
		end
	end

	/* fermeture definitive du curseur */
	close Pointeur_evt_clos

	/*A Effacer toutes les alertes non liees a un evenement datant */
	/*A de plus que la date donnee                                 */
	begin tran
	delete HIS..ALT_EXP where evenement is null and
				  horodate <= @va_Horodate_in
	commit tran

	begin tran
	delete HIS..ALT_LOC where numero not in (select numero from HIS..ALT_EXP)
	commit tran 
	/*A Effacer tous les etats de disponibilite des equipements   */
	/*A anterieurs a la date donnee et ne representant le dernier */
	/*A etat d'un equipement                                      */
	begin tran
	delete HIS..EQT_DSP where	horodate <= @va_Horodate_in and
				  	dernier = XDC_NON
	commit tran

	/*A Effacer toutes les mesures de trafic anterieures a la date specifiee */
	begin tran
	delete HIS..MES_TRA where horodate <= @va_Horodate_in
	commit tran
	begin tran
	delete HIS..MES_POI where horodate <= @va_Horodate_in
	commit tran
	begin tran
	delete HIS..MES_KCS where horodate <= @va_Horodate_in
	commit tran
	begin tran
	delete HIS..MES_NEC where horodate <= @va_Horodate_in
	commit tran
	begin tran
	delete HIS..MES_QPC where horodate <= @va_Horodate_in
	commit tran
	begin tran
	delete HIS..MES_MET where jour <= @va_Horodate_in
	commit tran
	begin tran
	delete HIS..ZDP_HEU where horodate <= @va_Horodate_in
	commit tran

	/* effacer les traces de regulation de trafic*/
	begin tran
	delete HIS..REG_TRA where horodate <= @va_Horodate_in
	commit tran
	begin tran
	delete HIS..GAB_TRA where horodate <= @va_Horodate_in
	commit tran
	/*A Effacer tous les appels horaire anterieurs a la date specifiee */
	begin tran
	delete HIS..APL_HOR where horodate <= @va_Horodate_in
	commit tran
	/* effacer toutes les données relatives a consignation Cockpit antérieures a la date specifiee */
	/* Purge des donnees Cockpit à 1 mois au lieu de 10 ans */
	select @vl_horodateCockpit = dateadd(month,-1,getdate())
	begin tran
	delete HIS..REF_PIT_MESSAGE where HIS..REF_PIT_MESSAGE.reference in (select reference from HIS..PIT_LOG where HIS..PIT_LOG.horodate <= @vl_horodateCockpit )
	delete HIS..FMC_REF_EXT where HIS..FMC_REF_EXT.reference in (select reference from HIS..PIT_LOG where HIS..PIT_LOG.horodate <= @vl_horodateCockpit )
	delete HIS..PIT_LOG where HIS..PIT_LOG.horodate <= @vl_horodateCockpit
	commit tran

	return XDC_OK
#else
	return XDC_NOK
#endif
go
