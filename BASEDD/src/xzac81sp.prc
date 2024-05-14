/*E*/
/*  Fichier : $Id: xzac81sp.prc,v 1.5 2016/04/20 08:28:09 gesconf Exp $        Release : $Revision: 1.5 $        Date : $Date: 2016/04/20 08:28:09 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac81sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
* strada : fournit les infos sur les actions a exporter
------------------------------------------------------
* HISTORIQUE :
*
* F. Lizot	04 Mar 2004 : creation
* JMG	20/09/05 : on ne renvoie que les actions en succes ou fin
* JPL	09/01/12 : Changement libelle "message Trafic FM" en "message RVA" (DEM 1009)  1.3
* PNI	05/03/15 : suppression dans le texte renvoyer des infos complémentaire (No tel, id fourgon) 1.4 DEM1118
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
*
*
* Sequence d'appel
* SP   XZAC81_Rechercher_Dernieres_Actions
*
* Arguments en entree
*
* Arguments en sortie
*
* Code retour
* XDC_OK
* <0 erreur sybase (voir Transact SQL p 13-15)
*
* Conditions d'utilisation
*
* Fonction
* Liste les actions a exporter
-------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAC81' and type = 'P')
	drop procedure XZAC81
go


create procedure XZAC81
	
as
declare	@vl_actio int, @vl_sit tinyint, @vl_type tinyint, @vl_texte char(250)
 
	create table #ACT (actio int,
			   niveau tinyint,
			   succes datetime null, 
			   fin datetime null, 
			   texte char(250),
			   fmc int, 
			   cle tinyint)
	

	/* creation des curseurs */
	declare Pointeur_action cursor
	for select EXP..ADA_ACT.actio, EXP..ADA_ACT.sit, EXP..ACT_GEN.type
	from EXP..ADA_ACT,EXP..ACT_GEN
	where EXP..ACT_GEN.numero = EXP..ADA_ACT.actio and EXP..ACT_GEN.sit = EXP..ADA_ACT.sit
	
	open Pointeur_action
	fetch Pointeur_action into  @vl_actio, @vl_sit, @vl_type
		
	while (@@sqlstatus = 0)
	begin
		if @vl_type = XDC_ACT_PAL
		   select @vl_texte="pilotage PAL " + convert(varchar(7), PAL)
		   from EXP..ACT_PAL
		   where actio = @vl_actio and sit = @vl_sit
		else if @vl_type = XDC_ACT_NAV
		   select @vl_texte="pilotage NAV " + convert(varchar(7), NAV)
		   from EXP..ACT_NAV
		   where actio = @vl_actio and sit = @vl_sit
		else if @vl_type = XDC_ACT_PMV
		   select @vl_texte="affichage PMV " + convert(varchar(7), PMV) + premiere_ligne +"/"+ deuxieme_ligne + "/" + troisieme_ligne
		   from EXP..ACT_PMV
		   where actio = @vl_actio and sit = @vl_sit
		else if @vl_type = XDC_ACT_TUNNEL
		   select @vl_texte="pilotage tunnel " + convert(varchar(7), tube)
		   from EXP..ACT_TUB
		   where actio = @vl_actio and sit = @vl_sit
		else if @vl_type = XDC_ACT_DEPANNEUR
		   select @vl_texte="appel depanneur " /* + nom*/
		   from EXP..ACT_DEP
		   where actio = @vl_actio and sit = @vl_sit
		else if @vl_type = XDC_ACT_INTERVENTION
		   select @vl_texte="intervention " /*+ nom*/
		   from EXP..ACT_INT
		   where actio = @vl_actio and sit = @vl_sit
		else if @vl_type = XDC_APPEL_RADIO
		   select @vl_texte="Appel Radio"
		else if @vl_type = XDC_ACT_FAX or @vl_type = XDC_ACT_BIP or @vl_type = XDC_ACT_TEL
		   select @vl_texte="appel " /*+ numero_telephonique*/
		   from EXP..ACT_APL
		   where actio = @vl_actio and sit = @vl_sit
		else if @vl_type = XDC_ACT_ECHANGEUR
		   select @vl_texte="pilotage echangeur " /*+ convert(varchar(7), demi_echangeur)*/
		   from EXP..ACT_ECH
		   where actio = @vl_actio and sit = @vl_sit
		else if @vl_type = XDC_ACT_FAX
		   select @vl_texte="envoi fax"
		else if @vl_type = XDC_ACT_PMV_FOURGON
		   select @vl_texte="message fourgon " /*+ convert(varchar(7), PR) + " " + message*/
		   from EXP..ACT_FRG
		   where actio = @vl_actio and sit = @vl_sit
		else if @vl_type = XDC_ACT_AFF_GARE
		   select @vl_texte="affichage gare " + nom_gare + " " + affichette
		   from EXP..ACT_GAR
		   where actio = @vl_actio and sit = @vl_sit
		else if @vl_type = XDC_ACT_PMV_REMORQUE
		   select @vl_texte="message PMV remorque " /*+ convert(varchar(7), PR) + " " + message*/
		   from EXP..ACT_RMQ
		   where actio = @vl_actio and sit = @vl_sit
		else if @vl_type = XDC_ACT_PAT_SIS
		   select @vl_texte="envoi patrouille " /*+ nom_patrouille*/
		   from EXP..ACT_SIS
		   where actio = @vl_actio and sit = @vl_sit
		else if @vl_type = XDC_ACT_TFM
		   select @vl_texte="diffusion message RVA"
		else
		   select @vl_texte=""


		if @vl_texte!=""
		begin
		insert into #ACT 
		select numero,sit,heure_succes,heure_fin,
			@vl_texte,evenement,cle
		from EXP..ACT_GEN
		where numero=@vl_actio and sit = @vl_sit
		and (heure_succes is not null or heure_fin is not null)
		end

		fetch Pointeur_action into  @vl_actio, @vl_sit, @vl_type
	end
	
	close Pointeur_action
	
	delete EXP..ADA_ACT
	
	select * from #ACT
	
	return XDC_OK
go

