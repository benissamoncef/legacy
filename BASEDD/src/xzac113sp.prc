/*E*/
/*Fichier :  $Id: xzac113sp.prc,v 1.2 2020/10/13 17:16:00 pc2dpdy Exp $      Release : $Revision: 1.2 $        Date : $Date: 2020/10/13 17:16:00 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac113sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* AAZ	27/08/2007	: Creation DEM 663
* LCL	30/03/20	: MOVIS Ajout site local DEM-SAE93 1.1
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Indique la fin de la commande individuelle par l'operateur
* 
* Sequence d'appel
* SP	XZAC113_Fin_Commande_Manuelle_ENR
* 
* Arguments en entree
* XDY_Entier	va_NumAction_in
* XDY_Horodate	va_HeureSucces_in
* XDY_Horodate	va_SuccesEchec_in
* XDY_Octet	    va_Sequence_in
* 
* Arguments en sortie
* 
* Code retour
* XDC_OK
* XDC_NOK	: action non trouvee, procedure lancee sur serveur non prevu
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* Doit etre appelee apres XZAC06 ou XZAC52.
* Si l'operateur a choisi d'aboutir a une autre sequence que celle 
* initialement prevue, il peut l'indiquer dans Sequence.
* 
* Fonction
* Verifier l'existence de l'action
* Update de la table ACTION, colonne heure_succes ou heure_echec 
* ou heure_fin, et eventuellement sequence.
* S'il s'agit de heure succes ou heure echec, et si heure fin de 
* l'action precedente sur le meme equipement n'est pas positionnee, le faire.
-------------------------------------------------------*/

use PRC
go

create procedure XZAC;113
	@va_NumAction_in	int	     = null,
	@va_HeureSucces_in	datetime = null,
	@va_HeureEchec_in	datetime = null,
	@va_Sequence_in		tinyint	 = null,
        @va_NomSiteLocal_in            char(2)         = null
as
	declare @vl_IndicateurSuccesEchec bit,
	        @vl_Site T_SITE,
	        @vl_HeureFin datetime,
		    @vl_NumEqt T_EQUIPEMENT,
		    @vl_Succes datetime,
		    @vl_Echec datetime,
		    @vl_Rowcount int

	/*! transfo en paramÉtres nulls bug applix */
	if @va_NumAction_in = 0 
	    select @va_NumAction_in = null
	if @va_Sequence_in = 0
	    select @va_Sequence_in = null
	if @va_HeureSucces_in = XDC_DATE_NULLE
	    select @va_HeureSucces_in = null
	if @va_HeureEchec_in = XDC_DATE_NULLE
	    select @va_HeureEchec_in = null
	if @va_NomSiteLocal_in = ""
	    select @va_NomSiteLocal_in = null

	/*A controle des parametres */
	if @va_NumAction_in = null or 
	   (@va_HeureSucces_in = null and
	    @va_HeureEchec_in = null) or
	   (@va_HeureSucces_in != null and
	    @va_HeureEchec_in != null)
        return XDC_ARG_INV

	/*A Recherche du site */
	/* MOVIS */
	if @va_NomSiteLocal_in = null
		select @vl_Site = numero 
		from   CFG..RES_DIS
        	where  serveur = @@servername
	else
		select @vl_Site = numero 
		from   CFG..RES_DIS
    		where code = @va_NomSiteLocal_in

    if @vl_Site is null
        return XDC_NOK
	
	/*A verifier l'existance de l'action */
	select @vl_NumEqt = EXP..ACT_GEN.equipement,
		   @vl_Succes = EXP..ACT_GEN.heure_succes,
		   @vl_Echec  = EXP..ACT_GEN.heure_echec
	from   EXP..ACT_GEN, EXP..ACT_GTC_ENR
	where  EXP..ACT_GEN.numero = @va_NumAction_in and 
		   EXP..ACT_GEN.type = XDC_ACT_GTC_ENR and
		   EXP..ACT_GEN.sit = @vl_Site and
		   EXP..ACT_GTC_ENR.actio = @va_NumAction_in and 
		   EXP..ACT_GTC_ENR.sit = @vl_Site
			
	if @@rowcount = 1
	begin
		/*B positionner l'indicateur de type de date (echec ou succes / fin) */
		if @vl_Succes is not null or 
		   @vl_Echec  is not null
		begin
			if @va_HeureSucces_in = null
				return XDC_ARG_INV

			select @vl_IndicateurSuccesEchec = XDC_NON
		end
		else	
		    select @vl_IndicateurSuccesEchec = XDC_OUI
	end
	else 
	    return XDC_NOK

	/*A indiquer la fin de la commande */
	begin tran
		/*B positionner les dates pour l'actio */
		if @vl_IndicateurSuccesEchec = XDC_OUI
		begin
			/*B cas de mise a jour des date de succes ou echec */

			/*B positionner les dates pour l'actio */
			update EXP..ACT_GEN
			set    heure_succes = @va_HeureSucces_in,
				   heure_echec = @va_HeureEchec_in
			where  numero = @va_NumAction_in and
			       sit    = @vl_Site and
				   type   = XDC_ACT_GTC_ENR

			select @vl_Rowcount = @@rowcount

			/*B positionner la sequence */
			if @vl_Rowcount    = 1 and
			   @va_Sequence_in is not null
			begin
				update EXP..ACT_GTC_ENR
				set    sequence = @va_Sequence_in
				where  actio = @va_NumAction_in and
				       sit = @vl_Site

				select @vl_Rowcount = @@rowcount
			end
		end
		else
		begin
			/*B cas de mise a jour de date de fin */
			update EXP..ACT_GEN 
			set    heure_fin = @va_HeureSucces_in
			where  numero = @va_NumAction_in and
			       sit    = @vl_Site and
				   type   = XDC_ACT_GTC_ENR

			select @vl_Rowcount = @@rowcount
		end

	commit tran

	if @vl_Rowcount != 1
		return XDC_NOK

	/*A positionner l'heure fin de l'actio precedente s'il s'agit */
	/*A d'un positionnement d'heure de succes ou echec sur l'actio */
	if @vl_IndicateurSuccesEchec = XDC_OUI
	begin
		update EXP..ACT_GEN
		set    heure_fin = isnull(@va_HeureSucces_in, @va_HeureEchec_in)
		where  equipement =  @vl_NumEqt and
		       sit        =  @vl_Site and 
			   heure_fin  is null and
			   type       =  XDC_ACT_GTC_ENR and
			   numero     != @va_NumAction_in
	end

	return XDC_OK

go
