/*E*/
/*  Fichier : $Id: xzas31sp.prc,v 1.9 1999/02/26 11:45:43 gaborit Exp $    Release : $Revision: 1.9 $      Date : $Date: 1999/02/26 11:45:43 $ 
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzas31sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Analyse 
*
------------------------------------------------------
* HISTORIQUE :
*
* T.F.  08/12/94	: Creation      (V 1.1)
* T.F.	2/01/95		: Ajout d'un code de retour comme argument. (V 1.2)
* C.T.  01/03/95	: Au CI, recherche ds la base HIS si pas trouve dans EXP
*                         (V 1.3)
* B.G.  05/09/1995	: modif recette site (1.4)
* B.G.  11/09/1995	: correction in au lieu = dans sous select (1.5)
* B.G.  11/09/1995	: correction concernant les messages jamais diffuses (1.6)
* C.T.	23/02/1998	: Modif default suite a passage en version SQL 11.0.3.2 (1.7)
* PN	29/06/1998	: separation EXP/HIS (1.8)
* PN	05/02/1999	: Modif intergest. dem/1724 1.9
* GGY	26/09/23	: Ajout district CA (DEM483)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Retourne des informations radio pour un evenement.
*
* Sequence d'appel
* SP   XZAS31_Info_Radio 
*
* Arguments en entree :
* Un evenement est compose d'un numero et d'une cle
* XDY_Entier    @va_Numero_in		: Numero de l'evement
* XDY_Mot	@va_Cle_in		: Valeur de la cle
*
* Arguments en sortie :
* XDY_Octet     @va_Status_out          : Status retour de la procedure
*
* Liste retournee :
* XDY_Entier 	Num_action	: Numero de l'action de type radio (message k)
* XDY_Entier 	Nb_diffusion	: Le nombre de fois ou le message a ete diffuse (Nd(k))
* XDY_Octet	Deb_Evnt	: Premier message emis si = 0 (1 sinon)
* XDY_Octet	Fin_Evnt	: Dernier message emis si = 0 (1 sinon)
* XDY_Datetime	Heure_succes    : Heure d'emission du message TD(k)
*
* Code retour
* XDC_OK		: fin de procedure normale
* XDC_NOK               : probleme d'acces aux donnees 
* XDC_ARG_INV           : parametres d'entree null, ou valeur non autorisee ou
*                         echangeur inexistante
* XDC_PAS_SYN		: pas d'action radio
*
* Conditions d'utilisation
* Utilise pour le calcul de l'efficacite intrinseque et absolue RADIO
*
* Fonction
*
-------------------------------------------------------*/

use PRC
go

create procedure XZAS;31
	@va_Numero_in		int		= null,
	@va_Cle_in		smallint	= null,
	@va_Status_out          tinyint         = null output
as
	declare	@vl_Numero		int,	 /* Numero d'action */
		@vl_Indicateur		tinyint, /* Indicateur dernier msg */
		@vl_DateDernierMsg	datetime,/* Date dernier message */	
		@vl_Status			int,
		@vl_RechercheDsHis	bit, 
		@vl_Site			tinyint

	/* Test Argument APPLIX  */
	if @va_Numero_in = 0    select @va_Numero_in	= null
	if @va_Cle_in = 0	select @va_Cle_in	= null

	/*A 
	** Controle des parametres en entree :
	*/
	if (@va_Numero_in       = null) or
	   (@va_Cle_in		= null)
		return XDC_ARG_INV

	/* Initialisation des arguments de sortie */
	select	@va_Status_out = XDC_OK,
		@vl_RechercheDsHis = XDC_NON

	/* Creation d'un table temeporaire pour concerver les informations */
	/* TFM.							    	   */
	create table #INFO_TRAF	(
				Num_action	int	 null,
				Nb_diffusion	int	 null,
				Deb_Evnt	tinyint  default 1,
				Fin_Evnt	tinyint  default 1,
				Heure_succes	datetime null
				)

	/* Informations FMC message Traffic FM */
	/*A
	** Recherche de toutes les actions correspondants a l'emission
	** d'un message d'information TFM pour l'evenement demande (FMC)
	*/
	/* L'heure de succes correspond a la date de premiere difusion du*/
	/* message.	*/

#ifdef CI
	insert #INFO_TRAF (	Num_action,
				Heure_succes,
				Deb_Evnt,
				Fin_Evnt)
	select 	numero,
		heure_succes,
		1,
		1
	from EXP..ACT_GEN
	where 	type 		= XDC_ACT_TFM
	and	evenement	= @va_Numero_in
	and	cle		= @va_Cle_in 
	and     heure_succes    <>null

	if @@rowcount = 0
	begin
		select @vl_RechercheDsHis = XDC_OUI
		exec @vl_Status = SQL_HIS.PRC..XZAS;31
			@va_Numero_in		,
			@va_Cle_in		,
			@va_Status_out      output

		if @vl_Status = null
                       	return XDC_PRC_INC
                else return @vl_Status
	end
#else
#ifdef HIST
		select @vl_RechercheDsHis = XDC_OUI

		insert #INFO_TRAF (	Num_action,
					Heure_succes,
					Deb_Evnt,
					Fin_Evnt)
		select 	HIS..ACT_GEN.numero,
			heure_succes,
			1,
			1
		from HIS..ACT_GEN, HIS..FMC_GEN
		where 	HIS..ACT_GEN.type 		= XDC_ACT_TFM
		and	HIS..ACT_GEN.evenement	= @va_Numero_in
		and	HIS..ACT_GEN.cle		= @va_Cle_in 
		and	HIS..ACT_GEN.heure_succes	<>null
		and 	 HIS..ACT_GEN.evenement  = HIS..FMC_GEN.numero
		and 	HIS..ACT_GEN.cle =  HIS..FMC_GEN.cle
		and 	datex=XDC_FAUX

		if @@rowcount = 0
		begin
			select @va_Status_out = XDC_PAS_SYN
			return XDC_OK
		end
#else
	insert #INFO_TRAF (	Num_action,
				Heure_succes,
				Deb_Evnt,
				Fin_Evnt)
	select 	EXP..ACT_GEN.numero,
		heure_succes,
		1,
		1
	from EXP..ACT_GEN, EXP..FMC_GEN
	where 	EXP..ACT_GEN.type 		= XDC_ACT_TFM
	and	EXP..ACT_GEN.evenement	= @va_Numero_in
	and	EXP..ACT_GEN.cle		= @va_Cle_in 
	and     EXP..ACT_GEN.heure_succes    <>null
	and	EXP..ACT_GEN.evenement    =  EXP..FMC_GEN.numero
	and 	EXP..ACT_GEN.cle                = EXP..FMC_GEN.cle
	and	datex=XDC_FAUX

	/* S'il n'y a pas de message radio, il n'est pas necessaire */
	/* de poursuivre.					    */
	if @@rowcount = 0
	begin
		select @va_Status_out = XDC_PAS_SYN
		return XDC_OK
	end
#endif
#endif
	/* Recherche du dernier message qui a ete envoye pour   */
	/* cet evenement. (date de succes la plus recente)      */
	/* Message anononcant la fin de l'evenement.		*/
	set rowcount 1
	update #INFO_TRAF
	set Fin_Evnt = 0
	where Heure_succes in (  select max(Heure_succes)
				from #INFO_TRAF)

	/* Recherche du premier message qui a ete envoye pour   */
	/* cet evenement. (date de succes la plus ancienne)     */
	/* Premier message annoncant l'evenement.		*/
	update #INFO_TRAF
	set Deb_Evnt = 0
	where Heure_succes in (  select min(Heure_succes)
				from #INFO_TRAF)
	set rowcount 0
	/* Pour chaque message, il peut y avoir plusieurs diffusions. */
	/*A
	** Recherche du nombre de diffusions pour chacun des messages radio.
	*/
	/* Verification du site */
	select  @vl_Site = numero
		from CFG..RES_DIS
		where serveur = @@servername and type = XDC_TYPEM_SD

	if @@rowcount = 1
	begin
		/* Recherche du nombre de diffusion du message */
#ifndef HIST
		update #INFO_TRAF
		set Nb_diffusion = nombre_diffusions
		from EXP..ACT_TFM
		where actio = Num_action 
		  and   sit = @vl_Site
#else
		update #INFO_TRAF
		set Nb_diffusion = nombre_diffusions
		from HIS..ACT_TFM
		where actio = Num_action 
		  and   sit = @vl_Site
#endif
	end

	/*A
	** Affichage du resultat
	*/
	select * from #INFO_TRAF
	order by Heure_succes

	/* Destruction de la table temporaire */
	drop table #INFO_TRAF

	/*A
	** Fin de la procedure
	*/
 	return XDC_OK
go
