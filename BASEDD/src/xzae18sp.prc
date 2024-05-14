/*E*/
/*  Fichier : $Id: xzae18sp.prc,v 1.10 2020/11/03 16:15:30 pc2dpdy Exp $      Release : $Revision: 1.10 $        Date : $Date: 2020/11/03 16:15:30 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae18sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	04/10/94	: Creation d'un bouchon	(V 1.1)
* C.T.	19/10/94	: Creation 		(V 1.2)
*			  remplace le poste operateur 
*                         par le site
* C.T.	10/11/94	: Modif bugs applix	(V 1.3)
* C.T.	24/05/95	: Ajout de la transmission de la
*                         tete et de la queue de bouchon
*                         pour un bouchon (V 1.4)
* C.T.	22/04/96	: Ajout le raffraichissement de la
*                         liste des evenements sur le site
*                         destinataire (1039) (V 1.5)
* C.T.	03/11/96	: Ajout en entree poste a rafraichir
*                         (optimisation liste a traiter) (V1.6)
* JMG	07/01/98	: gestion PC niveau 2 (dem/1536) 1.7
* JPL	12/01/09	: Degroupee;        (lie a DEM 848) 1.8
* JPL	14/09/09	: Ajout tete ou queue bouchon dans FMC_TRT si elles n'y sont pas 1.9
* LCL   22/04/20        : MOVIS Ajout site local pilotage DEM-SAE93
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Ajoute un evenement a la liste des evenements a traiter pour 
* le site concerne.
* 
* Sequence d'appel
* SP	XZAE18_Ajout_Liste_Evt_Traiter
* 
* Arguments en entree
* XDY_Site	va_Site_in
* XDY_Entier	va_NumEvt_in
* XDY_Octet	va_CleEvt_in
* 
* Arguments en sortie
* XDY_Booleen	@va_Resultat_out
* 
* Code retour
* XDC_OK
* XDC_NOK	: evenement non trouve, insertion impossible,
*                 site local inconnu
* XDC_ARG_INV	: parametres d'entree null
* 
* Conditions d'utilisation
* Le resultat indique si l'ajout a ete realise ou non
* (XDC_OUI/XDC_NON)
* Un evenement de type operateur n'est pas ajoute dans la liste 
* des evenements a traiter s'il n'est pas sur le site local
* L'ajout est realise si l'evenement n'est pas deja dans la liste
* 
* Fonction
* Insert dans la table A_TRAITER de l'evenements pour le 
* site 
----------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAE18' and type = 'P')
	drop procedure XZAE18
go


create procedure XZAE18
	@va_Site_in	T_SITE	= null,
	@va_NumEvt_in	int	= null,
	@va_CleEvt_in	tinyint	= null,
	@va_Resultat_out	bit = XDC_NON	output,
	@va_Poste_in	smallint = null, 	-- argument optionnel,
	@va_NomSiteLocal_in             char(2)         = null
as
	declare @vl_Type smallint, @vl_Site T_SITE,
		@vl_Cause int, @vl_CleCause tinyint,
		@vl_Evt int, @vl_CleEvt tinyint, 
		@vl_typeSite tinyint

	/*! transfo en param…tres nulls bug applix */
	if @va_Site_in = 0 select @va_Site_in = null
	if @va_NumEvt_in = 0 select @va_NumEvt_in = null
	if @va_CleEvt_in = 0 select @va_CleEvt_in = null
	
	/*A controle les parametres d'entree obligatoires */
	if @va_Site_in = null or @va_NumEvt_in = null or
	   @va_CleEvt_in = null
		return XDC_ARG_INV

	/*A tester l'existance de l'evenement */
	select
		@vl_Type = type,
		@vl_Cause = cause,
		@vl_CleCause = cle_cause
	from EXP..FMC_GEN
	where	numero = @va_NumEvt_in and cle = @va_CleEvt_in

	if @@rowcount != 1
		return XDC_NOK

	/* recherche du site local */
        /* MOVIS */
        if @va_NomSiteLocal_in = null
		select @vl_Site = numero, @vl_typeSite = type
                from CFG..RES_DIS
                where serveur = @@servername
        else
		select @vl_Site = numero, @vl_typeSite = type
                from CFG..RES_DIS
                where code = @va_NomSiteLocal_in

	if @vl_typeSite = XDC_TYPEM_PCS
		return XDC_NOK

	/*A Si l'evenement est de type operateur et n'est pas sur  */
	/*A le site local alors pas d'ajout dans la liste des      */
	/*A evenements a traiter                                   */
	if @vl_Type = XZAEC_FMC_PosteOperateur and @vl_Site != @va_Site_in
	begin
		select @va_Resultat_out = XDC_NON
		return XDC_OK
	end

	/*A inserer l'evenement dans la liste des evenements a traiter */
	/*A s'il n'existe pas dans la liste                            */
	if not exists ( select * from EXP..FMC_TRT
			where	evenement = @va_NumEvt_in and
				cle = @va_CleEvt_in and
				sit = @va_Site_in)
	begin
		insert EXP..FMC_TRT ( evenement, cle, sit)
		values(@va_NumEvt_in, @va_CleEvt_in, @va_Site_in)

		if @@rowcount != 1
			return XDC_NOK

		select @va_Resultat_out = XDC_OUI

		if @vl_Type = XZAEC_FMC_TeteBouchon
		begin
			/* recherche la queue de bouchon */
			select
				@vl_Evt = numero,
				@vl_CleEvt = cle
			from EXP..FMC_GEN
			where	cause = @va_NumEvt_in and 
				cle_cause = @va_CleEvt_in and
				type = XZAEC_FMC_QueueBouchon

			if @@rowcount = 1
			begin
				/* ajout de la queue de bouchon a traiter si elle ne l'est pas */
				if not exists ( select * from EXP..FMC_TRT
						where	evenement = @vl_Evt and
							cle = @vl_CleEvt and
							sit = @va_Site_in)
					insert EXP..FMC_TRT ( evenement, cle, sit)
					values(@vl_Evt, @vl_CleEvt, @va_Site_in)
			end
		end
		else if @vl_Type = XZAEC_FMC_QueueBouchon and 
			@vl_Cause is not null
		begin
			/* ajout de la tete de bouchon a traiter si elle ne l'est pas */
			if not exists ( select * from EXP..FMC_TRT
					where	evenement = @vl_Cause and
						cle = @vl_CleCause and
						sit = @va_Site_in)
				insert EXP..FMC_TRT ( evenement, cle, sit)
				values(@vl_Cause, @vl_CleCause, @va_Site_in)
		end
	end
	else	select @va_Resultat_out = XDC_NON

	/*A Rafraichir la liste destinataire DANS TOUS LES CAS */
	exec XZAR;03 0, 0, @va_Poste_in, " ", @va_Site_in

	return XDC_OK
go
