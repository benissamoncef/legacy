/*E*/
/*  Fichier : $Id: xzac63sp.prc,v 1.8 2021/03/08 14:25:56 pc2dpdy Exp $      Release : $Revision: 1.8 $        Date : $Date: 2021/03/08 14:25:56 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac63sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	24/04/95	: Creation a partir xzac52
* C.P.	07/01/98	: Correction gestion multi-sites dans les procs (1.2)
* JMG	02/10/09        : secto DEM887 1.3
* PNI	19/04/16	: Correction des appels dans PRC des XZAC sur les serveurs sites v1.4 DEM1159
* JMG	03/03/17	: regio DEM1220
* LCL	03/07/17	: Modif PRA
* LCL	20/11/19	: MOVIS DEM-SAE93 1.6
* GRO	12/10/2020      : MOVIS DEM-SAE93 1.61
* CGR	05/01/21	: MOVIS ajout PRA DEM-SAE93 1.8
* LCL	28/09/23 : Adaptations CNA DEM-483	
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Remet le tunnel a l'etat normal
*
* Sequence d'appel
* SP	XZAC63_Commande_Fin_Tunnel
*
* Arguments en entree
* XDY_Eqt	va_NumeroTunnel_in
* XDY_Entier	va_Site_in
* XDY_Horodate	va_HeureLancementFin_in
* XDY_Booleen	va_Manuelle_in
*
* Arguments en sortie
* 
* Code retour
* XDC_OK
* XDC_NOK	: site incorrect
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
*
* Conditions d'utilisation
* La commande est executee si Manuelle est a XDC_Faux
*
* Fonction
* Inserer dans la table ACTION la date de fin de l'actio.
* Dans le cas de commande automatique :
* 	ACRP63 :	Appeler XZET03 pour commander l'equipement
-----------------------------------------------------------------------*/

use PRC
go

create procedure XZAC;63
	@va_NumeroTunnel_in		T_EQUIPEMENT = null,
	@va_Site_in			T_SITE = null,
	@va_HeureLancementFin_in	datetime = null,
	@va_Manuelle_in			bit,
	@va_NomSiteLocal_in		char(2)	= null,
	@va_distant_in 			int = XDC_NON			
as
	declare	@vl_NumeroAction int, @vl_Status int, @vl_Sequence tinyint,
		@vl_Commande int, @vl_Site tinyint,
		@vl_serveur_pra char(6)

	/*! transfo en param�tres nulls bug applix */
	if @va_NumeroTunnel_in = 0 select @va_NumeroTunnel_in = null
	if @va_Site_in = 0 select @va_Site_in = null
	if @va_HeureLancementFin_in = XDC_DATE_NULLE select @va_HeureLancementFin_in = null

	/*A controle des parametres obligatoires */
	if  @va_NumeroTunnel_in = null or @va_Site_in = null or 
	    @va_HeureLancementFin_in = null
		return XDC_ARG_INV

        /*A Determiner le site local */
        /* MOVIS */
	if @va_NomSiteLocal_in = null
	        select  @vl_Site = numero
	        from CFG..RES_DIS
	        where serveur = @@servername
	else
	begin    
                if @va_distant_in = XDC_NON
                begin
			select  @vl_Site = numero
			from CFG..RES_DIS
			where code = @va_NomSiteLocal_in
                end
                else
                begin
                        select @vl_Site = @va_Site_in
                end
        end

        if @vl_Site is null
                return XDC_NOK

	/* Init serveur PRA */
	select @vl_serveur_pra=serveur_pra from CFG..RES_PRA where numero=@va_Site_in

	if @vl_Site = XDC_CI and @va_distant_in = XDC_NON	
	/* Traitement au district d'exploitation pour Escota, en local sinon */
	begin
		if @vl_serveur_pra = XDC_SQLVC
			exec @vl_Status = SQL_DM.PRC..XZAC;63 @va_NumeroTunnel_in,  @va_Site_in, @va_HeureLancementFin_in,  @va_Manuelle_in, @va_NomSiteLocal_in, XDC_OUI 
		if @vl_serveur_pra = XDC_SQLDP
			exec @vl_Status = SQL_DS.PRC..XZAC;63 @va_NumeroTunnel_in,  @va_Site_in, @va_HeureLancementFin_in,  @va_Manuelle_in, @va_NomSiteLocal_in, XDC_OUI 
		if @vl_serveur_pra = XDC_SQLCI
			exec @vl_Status = SQL_CI.PRC..XZAC;63 @va_NumeroTunnel_in,  @va_Site_in, @va_HeureLancementFin_in,  @va_Manuelle_in, @va_NomSiteLocal_in, XDC_OUI 
		return @vl_Status		
	end


	/*A tester si le site est correct */
	if not exists ( select numero from CFG..RES_DIS
                                where numero = @va_Site_in and type <> XDC_TYPE_PCNIVEAU3 )
/*                                where serveur = @@servername and type <> XDC_TYPE_PCNIVEAU3 )
*/
		return XDC_NOK


	/*A Rechercher la derniere action en cours sur cet equipement */
	select @vl_NumeroAction = max(numero)
	from EXP..ACT_GEN
	where 	EXP..ACT_GEN.equipement = @va_NumeroTunnel_in and
		EXP..ACT_GEN.sit = @va_Site_in and
		EXP..ACT_GEN.type = XDC_ACT_TUNNEL

	if @@rowcount = 0
		select @vl_NumeroAction = 0

	/*A cas de commande automatique par sequence */
	if @va_Manuelle_in = XDC_NON
	begin
		/*A sequence a utiliser */
		select @vl_Sequence = XDC_SEQ_OUV_TUN
	
		/*A numero de commande a envoyer : - numero action */
		select @vl_Commande = - @vl_NumeroAction

		/*A arreter l'affichage tunnel */

		if @vl_Site = XDC_CI
			exec @vl_Status = TASRV_CI...ACRP06 @va_NumeroTunnel_in, @vl_Sequence,
				@vl_Commande

		else if @vl_Site = XDC_DP
			exec @vl_Status = TASRV_DP...ACRP06 @va_NumeroTunnel_in, @vl_Sequence,
				@vl_Commande
																							
		else if @vl_Site = XDC_VC
			exec @vl_Status = TASRV_VC...ACRP06 @va_NumeroTunnel_in, @vl_Sequence,
				@vl_Commande
	
		else if @vl_Site = XDC_CA
			exec @vl_Status = TASRV_CA...ACRP06 @va_NumeroTunnel_in, @vl_Sequence,
				@vl_Commande
	
		if @vl_Status != XDC_OK
			return @vl_Status 
	end

	/*A mettre a jour la date de lancement fin */
	if @vl_NumeroAction != 0
	begin
		update EXP..ACT_GEN set heure_lancement_fin = @va_HeureLancementFin_in
		where	numero = @vl_NumeroAction and sit = @va_Site_in and
			heure_lancement_fin is null
	end

	return XDC_OK

go
