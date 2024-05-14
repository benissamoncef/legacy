/*E*/
/*  Fichier : $Id: xzac90sp.prc,v 1.3 2020/11/23 13:11:45 pc2dpdy Exp $     Release : $Revision: 1.3 $        Date : $Date: 2020/11/23 13:11:45 $
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzac90.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	15/09/05	: Creation
* LCL   25/03/20   : MOVIS Ajout site local DEM-SAE93
------------------------------------------------------ */

/*X*/
/*-----------------------------------------------------
* Service rendu
* Indique la fin de la commande individuelle par l'operateur
* 
* Sequence d'appel
* SP	XZAC90_Fin_Commande_Manuelle_BAF
* 
* Arguments en entree
* XDY_Entier	va_NumAction_in
* XDY_Horodate	va_HeureSucces_in
* XDY_Horodate	va_HeureEchec_in
* char(4)	va_Scenario_in
* 
* Arguments en sortie
* 
* Code retour
* XDC_OK
* XDC_NOK	: actio non trouvee, procedure lancee sur serveur non prevu
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* Doit etre appelee apres XZAC03 ou XZAC51.
* Si l'operateur a choisi d'aboutir a un autre scenario que celui 
* initialement prevu, il peut l'indiquer dans Scenario.
* 
* Fonction
* Verifier l'existence de l'actio
* Update de la table ACTION, colonne heure_succes ou heure_echec 
* ou heure_fin, et eventuellement scenario
* S'il s'agit de heure succes ou heure echec, et si heure fin de 
* l'actio precedente sur le meme equipement n'est pas positionnee, le faire.
-----------------------------------------------------------------------*/

use PRC
go

drop procedure XZAC90
go

create procedure XZAC90
	@va_NumAction_in	int	= null,
	@va_HeureSucces_in	datetime = null,
	@va_HeureEchec_in	datetime = null,
	@va_Module11_in		tinyint	= null,
	@va_Module12_in		tinyint	= null,
	@va_Module15_in		tinyint	= null,
	@va_Module21_in		tinyint	= null,
	@va_Module22_in		tinyint	= null,
	@va_Module25_in		tinyint	= null,
	@va_CleAction_in	tinyint	= null
as
	declare @vl_IndicateurSuccesEchec bit, @vl_Site T_SITE, @vl_HeureFin datetime,
		@vl_NumEqt T_EQUIPEMENT, @vl_TypeEqt tinyint, @vl_Succes datetime, @vl_Echec datetime,
		@vl_Rowcount int

	/*! transfo en param�tres nulls bug applix */
	if @va_NumAction_in = 0 select @va_NumAction_in = null
	if @va_HeureSucces_in = XDC_DATE_NULLE select @va_HeureSucces_in = null
	if @va_HeureEchec_in = XDC_DATE_NULLE select @va_HeureEchec_in = null

	/*A controle des parametres */
	if @va_NumAction_in = null or 
	   (@va_HeureSucces_in = null and @va_HeureEchec_in = null) or
	   (@va_HeureSucces_in != null and @va_HeureEchec_in != null)
		return XDC_ARG_INV

	/* recherche le site local */
        /* MOVIS */
	if @va_CleAction_in = null
		select @vl_Site = numero 
	        from CFG..RES_DIS
	        where serveur = @@servername
	else
		select @vl_Site = @va_CleAction_in 

        if @vl_Site is null
                return XDC_NOK

	/*A verifier l'existance de l'actio */
	select 
		@vl_NumEqt = EXP..ACT_GEN.equipement,
		@vl_Succes = EXP..ACT_GEN.heure_succes,
		@vl_Echec = EXP..ACT_GEN.heure_echec,
		@vl_HeureFin = EXP..ACT_GEN.heure_fin,
		@vl_TypeEqt = EXP..ACT_GEN.type
	from EXP..ACT_GEN, EXP..ACT_BAF
	where	EXP..ACT_GEN.numero = @va_NumAction_in and 
		EXP..ACT_GEN.sit = @vl_Site and
		EXP..ACT_BAF.actio = @va_NumAction_in and 
		EXP..ACT_BAF.sit = @vl_Site
			
	if @@rowcount = 1
	begin
		/*B positionner l'indicateur de type de date (echec ou succes / fin) */
		if @vl_Succes is not null or @vl_Echec is not null
		begin
			select @vl_IndicateurSuccesEchec = XDC_NON
		end
		else	select @vl_IndicateurSuccesEchec = XDC_OUI
	end
	else return XDC_NOK

	/*A indiquer la fin de la commande */
	begin tran
		/*B positionner les dates pour l'actio */
		if @vl_IndicateurSuccesEchec = XDC_OUI
		begin
			/*B cas de mise a jour des date de succes ou echec */

			/*B positionner les dates pour l'actio */
			update EXP..ACT_GEN set 
				heure_succes = @va_HeureSucces_in,
				heure_echec = @va_HeureEchec_in
			where	numero = @va_NumAction_in and sit = @vl_Site and
				type = @vl_TypeEqt

			select @vl_Rowcount = @@rowcount

			/*B positionner le scenario */
			if @vl_Rowcount = 1 and @va_Module11_in is not null
			begin
				update EXP..ACT_BAF set 
					module11 = @va_Module11_in,
					module12 = @va_Module12_in,
					module15 = @va_Module15_in,
					module21 = @va_Module21_in,
					module22 = @va_Module22_in,
					module25 = @va_Module25_in
				where actio = @va_NumAction_in and sit = @vl_Site

				select @vl_Rowcount = @@rowcount
			end
		end
		else
		begin
			/*B cas de mise a jour de date de fin */
			if @vl_HeureFin is null
			begin
			   if @va_HeureSucces_in is not null
			   begin
			      update EXP..ACT_GEN set 
				  heure_fin = @va_HeureSucces_in
			      where  numero = @va_NumAction_in and sit = @vl_Site and
				  type = @vl_TypeEqt
			      select @vl_Rowcount = @@rowcount
			   end
			   else
			   begin
			      update EXP..ACT_GEN set 
				  heure_fin = @va_HeureEchec_in
			      where  numero = @va_NumAction_in and sit = @vl_Site and
				  type = @vl_TypeEqt
			      select @vl_Rowcount = @@rowcount
			   end
		        end
		        else
		        begin
		           select @vl_Rowcount = 1
		        end
		end

	commit tran

	if @vl_Rowcount != 1
		return XDC_NOK

	/*A positionner l'heure fin de l'actio precedente s'il s'agit */
	/*A d'un positionnement d'heure de succes ou echec sur l'actio */
	if @vl_IndicateurSuccesEchec = XDC_OUI
	begin
		update EXP..ACT_GEN set 
		heure_fin = isnull(@va_HeureSucces_in, @va_HeureEchec_in)
		where	equipement = @vl_NumEqt and sit = @vl_Site and 
			heure_fin is null and type = @vl_TypeEqt and
			numero != @va_NumAction_in
	end

	return XDC_OK

go
