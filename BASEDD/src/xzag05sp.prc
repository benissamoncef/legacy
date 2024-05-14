/*E*/
/*  Fichier : $Id: xzag05sp.prc,v 1.26 2020/09/22 08:03:04 pc2dpdy Exp $      Release : $Revision: 1.26 $        Date : $Date: 2020/09/22 08:03:04 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzag05sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Purge des bases
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	27/10/94	: Creation	(V 1.1)
* C.T.	03/11/94	: Modif ajout de la purge de la table
*                         APL_FAX	(V 1.2)
*                         non teste
* C.T.	16/11/94	: Le dernier evenement ou action cree
*                         ne sont pas purges (V 1.3)
*                         non teste
* C.T.	05/06/95	: Ajout commit sur delete (V 1.4)
*                         et modif appel a XZAG10
* P.N.	09/10/96	: Ajout des tables MES_KCS NEC QPC (dem/1227) (V 1.6)
* P.N.	23/02/97	: Ajout des tables ZDP_6MN 1.7
* C.P.	07/01/98	: Correction gestion multi-sites dans 
*			  les procs (V 1.8) (dem / 1536)
* P.N.	27/01/99	: Ajouter SAD_EXP (V1.19) + ADA_SIT ADA_TXT_SYN MES_ADA dem/1724
* C.H.  27/02/03        : Ajout des ifndef PC_SIMPL pour que la purge soit compilee sur PC niv 3 (V1.10)
* C.H.  06/03/03        : Ajout purge de la table FMC_HIS_DER sauf sur PC niv 3 (V1.10)
* C.H   06/03/03        : Ajout purge de la table FMC_ACT (V1.10)  
* C.H.  17/03/03        : Ajout insert dans table PURGE_ESSAI pour avoir les traces du passage de la proc
*                         dans PC3 1.11
* JMG	27/04/04	: AJOUT PURGE SAD_PAL 1.12
* JMG	13/06/05	: ajout purge TFM_DTX (SAGA) 1.13
* PNI	12/07/06	: ajout PMA, BAF,BAD dem576 v1.15
* JPL	13/06/07	: Purge des Fmc d'origine 'xxxESSAIxxx' ou 'xxxSUPPRIMERxxx' (DEM 653) 1.16
* JMG	07/09/09	: purge sur ancien numero de secteur aussi 1.17 DEM887
* JMG	18/09/11	: purge ALT_LOC 1.18 DEM 980
* VR	12/12/11	: Ajout PRV	(DEM/1016) 1.18
* JMG	12/06/13	: ajout REG_TRA 1.21
* JMG 	08/03/16	: purge alarmes inactives FACTIS 1.22
* JMG	06/07/17	: ajout SAD_SAGA 1.23
* LCL	02/03/18	: ajout controleurs de feux CTRL_FEUX 1.24 DEM1284
* JMG	15/08/18	: ajout GAB_TRA 1.25
* LPE	24/07/20	: ajout TDP_FCD DEM-SAE152 1.26
* LCL	02/10/23	: purge PIT_LOG et REF_PIT_MESSAGE DEM-527
* LCL	11/10/23	: Correction purges Cockpit DEM-527
* GGY	11/12/23	: Ajout PIC (DEM-483)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Purge des donnees de la base d'exploitation datant
* d'au moins la date precisee
* 
* Sequence d'appel
* SP	XZAG05_Purge_Base_Exploitation
* 
* Arguments en entree
* XDY_Datetime	va_Horodate_in	: date de fin de purge
* 
* Arguments en sortie
* 
* Code retour
* XDC_OK
* XDC_NOK	: site local inconnu
* XDC_ARG_INV	: parametre d'entree null
* XDC_PRC_INC	: procedure XZAG;10 ou XZAE;70 pas en base
* 
* Conditions d'utilisation
* 
* Fonction
* Suppression des lignes vielles de plus d'un mois
* au niveau des evenements, des actions, des SAD,
* des etats d'equipement, des alertes, des mesures.
* (Pour un evenement, la date a considerer est celle de cloture.)
-------------------------------------------------------*/

use PRC
go

create procedure XZAG;05
	@va_Horodate_in		datetime = null
as
	declare @vl_Status int, @vl_NumEvt int, @vl_CleEvt tinyint,
		@vl_NbConsequences int, @vl_DateCourante datetime,
		@vl_SuppressionEvt bit,@vl_TypeAcces bit, @vl_Site T_SITE,
		@vl_SiteEvt T_SITE, @vl_ClasseEvt tinyint, @vl_Bouclage bit,
		@vl_SiteAncien T_SITE

	/*A controle du parametre en entree */
	if @va_Horodate_in = null
		return XDC_ARG_INV

	/*A Recherche du numero du site local */
	select @vl_Site = numero 
        from CFG..RES_DIS
        where serveur = @@servername

	if @vl_Site=17
		select @vl_SiteAncien=21
	else if @vl_Site=15
		select @vl_SiteAncien=11
	else if @vl_Site=18
		select @vl_SiteAncien=24
	else if @vl_Site=16
		select @vl_SiteAncien=13
	else
		select @vl_SiteAncien=@vl_Site

        if @vl_Site is null
           	return XDC_NOK

	/*A Rechercher la liste des evenements sur tous les sites */
	/*A    - dont la date de cloture est anterieure a la date specifiee ou */
	/*A    - clotur�s et dont l'origine est 'Essai' ou 'Fmc a supprimer' */
	select 
		EXP..FMC_GEN.numero,
		EXP..FMC_GEN.cle,
		EXP..FMC_GEN.sit,
		CFG..TYP_FMC.classe
	into #EVT_CLOS
	from EXP..FMC_GEN, CFG..TYP_FMC
	where CFG..TYP_FMC.numero = EXP..FMC_GEN.type
	  and	(cloture <= @va_Horodate_in or
		 (cloture <> null and (origine_creation like '%ESSAI%' or
		                       origine_creation like '%SUPPRIMER%') ))

	/*! Enlever de la liste le dernier evenement cree sur le site */
	/*! local s'il a ete selectionne                              */
	if @@rowcount > 0
		delete #EVT_CLOS 
		where (cle = @vl_Site or cle = @vl_SiteAncien) 
				and numero in ( select max(EXP..FMC_GEN.numero)
						from EXP..FMC_GEN
						where EXP..FMC_GEN.cle = @vl_Site or
							EXP..FMC_GEN.cle = @vl_SiteAncien)
		

	/* initilisation des variables locales */
	select	@vl_TypeAcces = XZAEC_MemoriserConseq,
		@vl_Bouclage = XDC_NON,
		@vl_DateCourante = getdate()

	declare Pointeur_evt_clos cursor
	for select numero, cle, sit, classe 
	from #EVT_CLOS

	/*A Parcourir la liste de ces evenements en entier tant qu'au */
	/*A moins un evenement est supprime                           */
	open Pointeur_evt_clos
	fetch Pointeur_evt_clos into @vl_NumEvt, @vl_CleEvt, @vl_SiteEvt, 
				@vl_ClasseEvt
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
		/*B appartenant au site local et l'evenement s'il est sur*/
		/*B le site local                                        */
		if @vl_NbConsequences = 0
		begin
			exec @vl_Status = XZAG;10 @vl_NumEvt, @vl_CleEvt, 
						@vl_SuppressionEvt output

			if @vl_Status = null
				return XDC_PRC_INC

			if @vl_SuppressionEvt = XDC_OUI
				select @vl_Bouclage = XDC_OUI
		end

		/* lecture de l'evenement clos suivant */
		fetch Pointeur_evt_clos into @vl_NumEvt, @vl_CleEvt,
					@vl_SiteEvt, @vl_ClasseEvt
		
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

			/* Rechercher les evenements restant sur le site */
			/* local, qui n'ont pas encore ete supprimes */
			delete #EVT_CLOS
			insert #EVT_CLOS (numero, cle, sit, classe)
			select 
				EXP..FMC_GEN.numero,
				EXP..FMC_GEN.cle,
				EXP..FMC_GEN.sit,
				CFG..TYP_FMC.classe
			from EXP..FMC_GEN, CFG..TYP_FMC
			where	(sit = @vl_Site or sit = @vl_SiteAncien)
			  and	CFG..TYP_FMC.numero = EXP..FMC_GEN.type
			  and	(cloture <= @va_Horodate_in or
				 (cloture <> null and (origine_creation like '%ESSAI%' or
				                       origine_creation like '%SUPPRIMER%') ))

			/* se repositionner sur le debut de la liste */
			open Pointeur_evt_clos
			fetch Pointeur_evt_clos into @vl_NumEvt, @vl_CleEvt,
					@vl_SiteEvt, @vl_ClasseEvt
		end
	end

	/* fermeture definitive du curseur */
	close Pointeur_evt_clos
#ifndef PC_SIMPL
	/*A Rechercher la liste de tous les SAD datant d'au moins */
	/*A la date donnee                                        */
	select numero
	into #PLN_SUP
	from EXP..SAD_PLN 
	where	horodate <= @va_Horodate_in

	/*A Effacer la partie generique des plans d'actions et */
	/*A le complement associe lies a cet evenement         */
	begin tran
	delete	EXP..SAD_APL
	where	numero in (select numero from #PLN_SUP) 
	commit tran
	begin tran
	delete	EXP..SAD_DST_FAX
	where	numero in (select numero from #PLN_SUP) 
	commit tran
	begin tran
	delete	EXP..SAD_ECH 
	where	numero in (select numero from #PLN_SUP) 
	commit tran
	begin tran
	delete	EXP..SAD_FAX
	where	numero in (select numero from #PLN_SUP) 
	commit tran
	begin tran
	delete	EXP..SAD_MSG_TFM
	where	numero in (select numero from #PLN_SUP) 
	commit tran
	begin tran
	delete	EXP..SAD_NAV
	where	numero in (select numero from #PLN_SUP) 
	commit tran
	begin tran
	delete	EXP..SAD_PMV
	where	numero in (select numero from #PLN_SUP) 
	commit tran
	begin tran
	delete  EXP..SAD_SAGA
	where   numero in (select numero from #PLN_SUP)
	commit tran
	delete	EXP..SAD_PRV
	where	numero in (select numero from #PLN_SUP) 
	commit tran
	delete	EXP..SAD_PIC
	where	numero in (select numero from #PLN_SUP) 
	commit tran
	delete	EXP..SAD_CFE
	where	numero in (select numero from #PLN_SUP) 
	commit tran
	begin tran
	delete	EXP..SAD_BAF
	where	numero in (select numero from #PLN_SUP) 
	commit tran
	commit tran
	begin tran
	delete	EXP..SAD_PMA
	where	numero in (select numero from #PLN_SUP) 
	commit tran
	commit tran
	begin tran
	delete	EXP..SAD_BAD
	where	numero in (select numero from #PLN_SUP) 
	commit tran
	begin tran
	delete	EXP..SAD_TFM 
	where	numero in (select numero from #PLN_SUP) 
	commit tran
	begin tran
	delete	EXP..SAD_TUB
	where	numero in (select numero from #PLN_SUP) 
	commit tran
	begin tran
	delete	EXP..SAD_TXT_FAX
	where	numero in (select numero from #PLN_SUP) 
	commit tran
	begin tran
	delete	EXP..SAD_TXT_TFM
	where	numero in (select numero from #PLN_SUP) 
	commit tran
	delete EXP..SAD_PAL
        where numero in (select numero from #PLN_SUP)
        commit tran
#ifdef CI
	begin tran
	delete	EXP..SAD_EXP
	where	numero in (select numero from #PLN_SUP) 
	commit tran
#endif 
	begin tran
	delete	EXP..SAD_PLN
	where	numero in (select numero from #PLN_SUP) 
	commit tran
#endif

	/*A Effacer toutes les alertes non liees a un evenement datant */
	/*A de plus que la date donnee sur le site local               */
	begin tran
	delete EXP..ALT_LOC where sit  = @vl_Site
				and numero in (select numero from EXP..ALT_EXP where evenement is null and sit = @vl_Site
					and horodate <= @va_Horodate_in and
					qualification != XDC_ALT_VRAIE_EVT and
					qualification != XDC_ALT_CONF_EVT)

	delete EXP..ALT_EXP where evenement is null and (sit = @vl_Site or sit = @vl_SiteAncien) 
				and horodate <= @va_Horodate_in and
				  qualification != XDC_ALT_VRAIE_EVT and
				  qualification != XDC_ALT_CONF_EVT
	commit tran
	/*C.H.*/
	/*A Purge de la table FMC_ACT */
	/*A sans condition sur le site car table non repliquee */
	/*A les inserts et updates se font par trigger */
	/*A a partir de la table ACT_GEN */
	begin tran
	delete EXP..FMC_ACT where numero not in
	(select numero from EXP..FMC_GEN where cloture is null or cloture > @va_Horodate_in)
	commit tran
	/*C.H.*/

#ifdef CI
	/*A Effacer tous les attribut datex non liees a un evenement datant */
	/*A de plus que la date donnee sur le site local               */
	begin tran
	delete EXP..ADA_TXT_SYN where reference in ( select reference
							from EXP..ADA_SIT
							where evenement is null and
							horodate <= @va_Horodate_in)
	commit tran
	begin tran
	delete EXP..ADA_SIT where evenement is null and
				  horodate <= @va_Horodate_in 
	commit tran
#endif

	/*A Effacer les alarmes inactives anterieurs a la date donnee*/
	begin tran
	delete EXP..ALM_EQT where date_dernier_changement <= @va_Horodate_in and etat = XDC_FAUX
	commit tran

	/*A Effacer tous les etats de disponibilite des equipements   */
	/*A anterieurs a la date donnee et ne representant le dernier */
	/*A etat d'un equipement sur le site local                    */
	begin tran
	delete EXP..EQT_DSP where (	sit = @vl_Site or sit = @vl_SiteAncien) and 
					horodate <= @va_Horodate_in and
				  	dernier = XDC_NON
	commit tran

	/*A Effacer toutes les mesures de trafic anterieures a la date */
	/*A donnee sur le site local                                   */
	begin tran
	delete EXP..MES_TRA where (sit = @vl_Site or sit = @vl_SiteAncien) and horodate <= @va_Horodate_in
	commit tran
	begin tran
	delete EXP..MES_POI where (sit = @vl_Site or sit = @vl_SiteAncien) and horodate <= @va_Horodate_in
	commit tran
#ifdef CI
	begin tran
	delete EXP..MES_ADA where horodate <= @va_Horodate_in
	commit tran
#endif
	begin tran
	delete EXP..MES_NIV where (sit = @vl_Site or sit = @vl_SiteAncien) and horodate <= @va_Horodate_in
	commit tran
#ifndef PC_SIMPL	
	begin tran
	delete EXP..ZDP_6MN where (sit = @vl_Site or sit = @vl_SiteAncien) and horodate <= @va_Horodate_in
	commit tran
	begin tran
	delete EXP..TDP_FCD where horodate <= @va_Horodate_in
	commit tran
#endif
	begin tran
	delete EXP..REG_TRA where horodate <= @va_Horodate_in
	commit tran
	begin tran
	delete EXP..GAB_TRA where horodate <= @va_Horodate_in
	commit tran
	begin tran
	delete EXP..REF_PIT_MESSAGE where EXP..REF_PIT_MESSAGE.reference in (select reference from EXP..PIT_LOG where EXP..PIT_LOG.horodate <= @va_Horodate_in )
	delete EXP..FMC_REF_EXT where EXP..FMC_REF_EXT.reference in (select reference from EXP..PIT_LOG where EXP..PIT_LOG.horodate <= @va_Horodate_in )
	delete EXP..PIT_LOG where EXP..PIT_LOG.horodate <= @va_Horodate_in
	commit tran
#ifndef CI
	begin tran
	delete EXP..MES_KCS where (sit = @vl_Site or sit = @vl_SiteAncien) and horodate <= @va_Horodate_in
	commit tran
	begin tran
	delete EXP..MES_NEC where (sit = @vl_Site or sit = @vl_SiteAncien) and horodate <= @va_Horodate_in
	commit tran
	begin tran
	delete EXP..MES_QPC where (sit = @vl_Site or sit = @vl_SiteAncien) and horodate <= @va_Horodate_in
	commit tran
#endif
#ifndef PC_SIMPL
	/*A Effacer tous les appels par fax anterieures a la date */
	/*A donnee                                                */
	begin tran
	delete EXP..APL_FAX where horodate <= @va_Horodate_in
	commit tran
	/*C.H*/
	/*A purge de la table FMC_HIS_DER sans condition sur le site */
	/*A car table non repliquee */
	begin tran
	delete EXP..FMC_HIS_DER where numero not in
	(select numero from EXP..FMC_GEN where cloture is null or cloture > @va_Horodate_in) 
	commit tran
	/*C.H*/
#endif
#ifdef CI
        /*A Effacer tous les messages trafic FM de TFM_DTX anterieures a la date donnee */
        /*A et qui ont supprime=1  (modif SAGA)                                         */
        begin tran
        delete EXP..TFM_DTX where supprime=1 and heure_diffusion <= @va_Horodate_in
        commit tran
#endif
	return XDC_OK
go
