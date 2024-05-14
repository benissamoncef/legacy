/*E*/
/*  Fichier : $Id: xzag35sp.prc,v 1.3 2007/03/26 15:27:07 gesconf Exp $      Release : $Revision: 1.3 $        Date : $Date: 2007/03/26 15:27:07 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzag35sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Purge des bases
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	07/02/96	: Creation	(V 1.1)
* C.T.	27/03/96	: Correction pamaretre d'appel XZAE;68	(V 1.2)
* P.N.	24/08/05	: Prise en cpte du spid pour select LISTE_CONSEQ v1.3 DEM501
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Tester si la purge d'un evenement incluant dans une boucle
* est possible
* 
* Sequence d'appel
* SP	XZAG35_Verif_Purge_Evt_Bouclage
* 
* Arguments en entree
* XDY_Entier	va_NumEvt_in	: numero evt
* XDY_Octet	va_CleEvt_in	: cle evt
* char(3)	va_Base_in	: base a purger
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
-------------------------------------------------------*/

use PRC
go

create procedure XZAG;35
	@va_Horodate_in		datetime = null,
	@va_NumEvt_in		int = null,
	@va_CleEvt_in		tinyint = null,
	@va_Base_in		char(3) = null,
	@va_EvtAPurger_out	bit output
as
	declare @vl_Status int, @vl_Conseq int, @vl_CleConseq tinyint,
		@vl_NbConsequences int, @vl_DateCourante datetime,
		@vl_TypeAcces tinyint, @vl_Nombre int

	/*A controle du parametre en entree */
	if @va_NumEvt_in = null or @va_CleEvt_in = null or 
	   @va_Horodate_in = null or @va_Base_in = null
		return XDC_ARG_INV

	if @va_Base_in = XDC_BASE_EXP
		select @vl_DateCourante = getdate(),
			@va_EvtAPurger_out = XDC_VRAI 
	else	select @vl_DateCourante = dateadd(year,-4, getdate()),
			@va_EvtAPurger_out = XDC_VRAI 

	/*B Rechercher l'arbre des consequences de l'evenement */
	select @vl_TypeAcces = XZAEC_PasEffacDonnees
	exec @vl_Status = XZAE;68 @va_NumEvt_in, @va_CleEvt_in, 
				@vl_DateCourante, @vl_TypeAcces

	/*B evenement inclus dans un bouclage :                            */
	/*B tester si tous les evenements inclus dans le meme bouclage ont */
	/*B aucune autre consequence n'appartenant pas au bouclage         */
	if exists (select * from LISTE_CONSEQ 
			where numero = @va_NumEvt_in and cle = @va_CleEvt_in and spid = @@spid)
	begin
		/* memoriser la liste des consequences */
		select * 
		into #COPIE_CONSEQ
		from LISTE_CONSEQ
		where spid = @@spid

		/* vider la table de stockage de l'arbre */
		delete LISTE_CONSEQ
		where spid = @@spid

		declare Pointeur_bouclage cursor
		for select
			numero,
			cle
		from #COPIE_CONSEQ

		open Pointeur_bouclage
		fetch Pointeur_bouclage into @vl_Conseq, @vl_CleConseq
		while @va_EvtAPurger_out = XDC_VRAI and @@sqlstatus = 0
		begin
			if @vl_Conseq = @va_NumEvt_in and @vl_CleConseq = @va_CleEvt_in
				select @va_EvtAPurger_out = XDC_VRAI 
			else
			begin
				/* tester si chaque consequence est close */
				/* et a au plus une consequence et qui appartient au bouclage */
				if exists (select * from EXP..FMC_GEN
					where numero = @vl_Conseq and cle = @vl_CleConseq and
						cloture <= @va_Horodate_in)
				begin
					exec @vl_Status = XZAE;70 @vl_Conseq, @vl_CleConseq, 
								@vl_DateCourante, XZAEC_MemoriserConseq
				
					if @vl_Status = XDC_OK
					begin
						select @vl_NbConsequences = count(*)
						from LISTE_CONSEQ
						where spid = @@spid
		
						if @vl_NbConsequences > 1 
							select @va_EvtAPurger_out = XDC_FAUX 
						else
						begin
							select @vl_Nombre = count(*)
							from LISTE_CONSEQ, #COPIE_CONSEQ
							where	LISTE_CONSEQ.spid = @@spid and
								LISTE_CONSEQ.numero = #COPIE_CONSEQ.numero and 
								LISTE_CONSEQ.cle = #COPIE_CONSEQ.cle

							if @vl_Nombre = 0
								select @va_EvtAPurger_out = XDC_FAUX 
						end
					end
					else	select @va_EvtAPurger_out = XDC_FAUX 
				end
				else	select @va_EvtAPurger_out = XDC_FAUX 
			end

			/* vider la table de stockage de l'arbre */
			delete LISTE_CONSEQ
			where spid = @@spid

			fetch Pointeur_bouclage into @vl_Conseq, @vl_CleConseq
		end
		close Pointeur_bouclage
	end
	else	select @va_EvtAPurger_out = XDC_FAUX 

	/* vider la table de stockage de l'arbre */
	delete LISTE_CONSEQ
	where spid = @@spid

	return XDC_OK
go
