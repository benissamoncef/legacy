/*E*/
/*  Fichier : $Id: xzag15sp.prc,v 1.15 2012/09/26 10:06:49 gesconf Exp $      Release : $Revision: 1.15 $        Date : $Date: 2012/09/26 10:06:49 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzag15sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Purge des bases
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	16/06/95	: Creation	(V 1.1)
* C.T.	08/02/96	: Ajout de la purge des evenements inclus 
*                         dans une boucle simple (lien cause) (V 1.2)
* C.T.	03/09/96	: Ajout cas de purge des evenements sur PC simplifie
*                         et ajout au CI stockage dans la table FMC_PUR des fmc 
*                         a purger (V 1.3)
* C.T.	20/11/97	: Modif clause de recherche des fmc operateur du pc simplifie
* C.P.	07/01/98	: Correction gestion multi-sites dans 
*			  les procs (V 1.5) (dem / 1536)
* C.T.	04/02/98	: Correction erreur de compil. V1.6
* P.N	02/10/98	: Correction pour PCS il manquait le go final 1.7
* P.N.  14/10/98	: Correction pour ne pas supprimer le dernier no de fmc au PCS
			  Suppression de residus dans FMC_TRT 1.8
* P.N.  27/01/99	: Ajout suppression de ADA_SIT non lie a un evt 1.9
* C.H.  21/03/03        : Modification de la proc pour obtenir une liste des evenements a purger
			  complete meme pour PC3
* C.H.  21/03/03        : Ajout purge des tables FMC_HIS_DER et FMC_ACT suivant conditions 
* C.H.  25/03/03        : Suppression des traces et des insert ds tables PURGE_ESSAI_15 v1.8
* C.H  			: Ajout purge FMC_HIS_DER v1.10
* JPL	11/07/07	: Purge des Fmc d'origine 'xxxESSAIxxx' ou 'xxxSUPPRIMERxxx' (DEM 653) 1.11
* JMG	07/09/09	: purge sur ancien numero de secteur aussi 1.12 DEM887
* JPL	10/11/09	: Purge des Fmc crees mais non validees (rien dans FMC_HIS) (DEM 799) 1.13
* JPL	10/11/09	: Purge FMC_TRT, FMC_HIS_DER, FMC_ACT: recherche sur critere de cle manquant 1.14
* PNI	26/09/12	: Suppression du delete de FMC_HIS_DER (ajouter dedans l'xzag10) DEM1052 1.15
* ABK   26/09/23 	: Ajout district CA (DEM-483)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Retourner la liste des evenements a purger des donnees 
* de la base d'exploitation datant
* d'au moins la date precisee
* 
* Sequence d'appel
* SP	XZAG15_Liste_Purge_Evts_Base_Exploitation
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
* XDC_PRC_INC	: XZAE;70 pas en base
* 
* Conditions d'utilisation
* 
* Fonction
* (Pour un evenement, la date a considerer est celle de cloture.)
* pour les pc simplifies, les fmc operateur qui ont une date de cloture
* inferieure a la date donnee et les fmc a purger
-------------------------------------------------------*/

use PRC
go

create procedure XZAG;15
	@va_Horodate_in		datetime = null
as
	declare @vl_Status int, @vl_NumEvt int, @vl_CleEvt tinyint,
		@vl_NbConsequences int, @vl_DateCourante datetime,
		@vl_EvtAEffacer bit, @vl_TypeAcces bit, @vl_Site T_SITE, @vl_SiteAncien T_SITE,
		@vl_SiteMaitre T_SITE


	/*A controle du parametre en entree */
	if @va_Horodate_in = null
		return XDC_ARG_INV

	/*A Recherche du numero du site local */
	select @vl_Site = numero 
        from CFG..RES_DIS
        where serveur = @@servername

        if @vl_Site is null
           	return XDC_NOK

	/* initialisation variable locale */
	select @vl_SiteMaitre = numero from CFG..RES_DIS where type = XDC_TYPEM_SD

	select @vl_SiteAncien=@vl_Site	

#ifndef PC_SIMPL
	/* Suppression des residus ds FMC_TRT : suppression des evenements non en cours */
	/*!!!!! Attention aux evenements acheves au CI et encore presents sur site !!!!!*/
	delete EXP..FMC_TRT where cle = @vl_Site and not exists (
		select * from EXP..FMC_GEN
		where numero = EXP..FMC_TRT.evenement and cle = EXP..FMC_TRT.cle and
			(cloture is null or sit <> @vl_SiteMaitre) )

	delete EXP..FMC_TRT where cle = @vl_SiteAncien and not exists (
		select * from EXP..FMC_GEN
		where numero = EXP..FMC_TRT.evenement and cle = EXP..FMC_TRT.cle and
			(cloture is null or sit <> @vl_SiteMaitre) )
#endif
	
	/*A Rechercher la liste des evenements sur tous les sites dont */
	/*A la cloture datant de plus de la date precisee              */
	select 
		EXP..FMC_GEN.numero,
		EXP..FMC_GEN.cle
	into #EVT_CLOS
	from EXP..FMC_GEN
	where cloture <= @va_Horodate_in
	   or (cloture <> null and (origine_creation like '%ESSAI%' or
	                            origine_creation like '%SUPPRIMER%'))
	   or not exists (select * from EXP..FMC_HIS
			   where numero = EXP..FMC_GEN.numero
			   and	 cle	= EXP..FMC_GEN.cle)

	/*! Enlever de la liste le dernier evenement cree sur le site */
	/*! local s'il a ete selectionne                              */
	if @@rowcount > 0
	begin
		delete #EVT_CLOS 
		where cle = @vl_Site and numero in ( select max(EXP..FMC_GEN.numero)
						from EXP..FMC_GEN
						where EXP..FMC_GEN.cle = @vl_Site)
		delete #EVT_CLOS 
		where cle = @vl_SiteAncien and numero in ( select max(EXP..FMC_GEN.numero)
						from EXP..FMC_GEN
						where EXP..FMC_GEN.cle = @vl_SiteAncien)
	end
	
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


	delete EXP..FMC_PUR
#else
	create table #LISTE_PURGE (numero int, cle tinyint)
#endif

	/* initilisation des variables locales */
	select	@vl_TypeAcces = XZAEC_MemoriserConseq,
		@vl_DateCourante = getdate()

	declare Pointeur_evt_clos cursor
	for select numero, cle
	from #EVT_CLOS

	/*A Parcourir la liste de ces evenements en entier tant qu'au */
	/*A moins un evenement est supprime                           */
	open Pointeur_evt_clos
	fetch Pointeur_evt_clos into @vl_NumEvt, @vl_CleEvt

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
#ifdef CI
			insert EXP..FMC_PUR (numero, cle)
			values(@vl_NumEvt, @vl_CleEvt)
#else
			insert #LISTE_PURGE (numero, cle)
			values(@vl_NumEvt, @vl_CleEvt)
#endif
		end
		/*B si l'evenement est dans une boucle simple */
		/*B alors l'evenement est effacable           */
		else if @vl_NbConsequences = 1
		begin
			exec @vl_Status = XZAG;35 @va_Horodate_in, @vl_NumEvt, @vl_CleEvt, XDC_BASE_EXP, @vl_EvtAEffacer output

			if @vl_Status = XDC_OK and @vl_EvtAEffacer = XDC_VRAI
#ifdef CI
				insert EXP..FMC_PUR (numero, cle)
				values(@vl_NumEvt, @vl_CleEvt)
#else
				insert #LISTE_PURGE (numero, cle)
				values(@vl_NumEvt, @vl_CleEvt)
#endif
		end

		/* lecture de l'evenement clos suivant datant d'au moins */
		/* la date precisee                                      */
		fetch Pointeur_evt_clos into @vl_NumEvt, @vl_CleEvt
	end

	/* fermeture definitive du curseur */
	close Pointeur_evt_clos

	/*A Retourner la liste des evenements a purger */
#ifdef CI
	select numero, cle
	from EXP..FMC_PUR
#else
	select numero, cle
	from #LISTE_PURGE
#endif
/*---------------------------C.H Purge de la table FMC_ACT uniquement sur PC3 car table deja purgee sur autres PC par xzag19 */
#ifdef PC_SIMPL
	begin tran
	delete EXP..FMC_ACT where not exists (
		select * from EXP..FMC_GEN
		where numero = EXP..FMC_ACT.numero and cle = EXP..FMC_ACT.cle and
			(cloture is null or cloture > @va_Horodate_in) )
	commit tran
#endif
/*C.H. --------------------------------------Fin Purge---------------------------------------------- */

	return XDC_OK
go
