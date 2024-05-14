/*E*/
/*  Fichier : $Id: xzag25sp.prc,v 1.5 2007/07/05 14:37:39 gesconf Exp $      Release : $Revision: 1.5 $        Date : $Date: 2007/07/05 14:37:39 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzag25sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Purge des bases
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	04/09/95	: Creation	(V 1.1)
* C.T.	08/02/96	: Ajout de la purge des evenements inclus 
*                         dans une boucle simple (lien cause) (V 1.2)
* PN	03/09/98	: separation EXP/HIS v1.3 dem/1696
* Hilmarcher 17/02/05   : proc xzag;35 non compilee sur HIS
* JPL	13/06/07	: Purge des Fmc d'origine 'xxxESSAIxxx' ou 'xxxSUPPRIMERxxx' (DEM 653) 1.5
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Purge des donnees de la base d'historique datant
* d'au moins la date precisee
* 
* Sequence d'appel
* SP	XZAG25_Purge_Base_Historique
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

create procedure XZAG;25
	@va_Horodate_in		datetime = null
as
#ifdef HIST
	declare @vl_Status int, @vl_NumEvt int, @vl_CleEvt tinyint,
		@vl_NbConsequences int, @vl_DateCourante datetime,
		@vl_SuppressionEvt bit,@vl_TypeAcces bit, @vl_EvtAEffacer bit

	/*A controle du parametre en entree */
	if @va_Horodate_in = null
		return XDC_ARG_INV

	create table #LISTE_PURGE (numero int, cle tinyint)

	/*A Rechercher dans la base d'historique la liste des evenements */
	/*A    - dont la date de cloture est anterieure a la date specifiee ou */
	/*A    - cloturés et dont l'origine est 'Essai' ou 'Fmc a supprimer' */
	select 
		HIS..FMC_GEN.numero,
		HIS..FMC_GEN.cle
	into #EVT_CLOS
	from HIS..FMC_GEN
	where cloture <= @va_Horodate_in
	   or (cloture <> null and (origine_creation like '%ESSAI%' or
	                            origine_creation like '%SUPPRIMER%'))

	if @@rowcount = 0
		return XDC_OK

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
		if @vl_NbConsequences = 0
		begin
			insert #LISTE_PURGE (numero, cle)
			values(@vl_NumEvt, @vl_CleEvt)
		end
		
#ifndef HIST
		/*B si l'evenement est dans une boucle simple */
		/*B alors l'evenement est effacable           */
		else if @vl_NbConsequences = 1
		begin
			exec @vl_Status = XZAG;35 @va_Horodate_in, @vl_NumEvt, @vl_CleEvt, XDC_BASE_HIS, @vl_EvtAEffacer output

			if @vl_Status = XDC_OK and @vl_EvtAEffacer = XDC_VRAI
				insert #LISTE_PURGE (numero, cle)
				values(@vl_NumEvt, @vl_CleEvt)
		end
#endif
		/* lecture de l'evenement clos suivant datant d'au moins */
		/* la date precisee                                      */
		fetch Pointeur_evt_clos into @vl_NumEvt, @vl_CleEvt
	end

	/* fermeture definitive du curseur */
	close Pointeur_evt_clos

	/*A Retourner la liste des evenements a purger */
	select
		numero,
		cle
	from #LISTE_PURGE

	return XDC_OK
#else
	return XDC_NOK
#endif
go
