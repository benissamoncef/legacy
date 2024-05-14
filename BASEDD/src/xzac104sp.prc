/*E*/
/*Fichier :  $Id: xzac104sp.prc,v 1.11 2021/03/08 14:37:57 pc2dpdy Exp $      Release : $Revision: 1.11 $        Date : $Date: 2021/03/08 14:37:57 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac104.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* AAZ	27/08/2007 : Creation DEM 663
* JBL   Oct 2008   : Correction mise a jour de l'action precedente
* JMG           02/10/09        : secto DEM887
* PNI           19/04/16 : Correction des appels dans PRC des XZAC sur les serveurs sites v1.5 DEM1159
*  JMG           03/03/17 : regio DEM1220
* LCL	29/06/17   : Modif PRA
* ABE   05-02-2020 : modifications int�gration ouvrage toulon
* LCL	25/05/20   : MOVIS Ajout site local DEM-SAE93
* LPE	12/10/20   : MOVIS DEM-SAE93 1.10
* ABE	05/01/21   : MOVIS ajout serveur PRA DEM-SAE93 1.11
* LCL	26/09/23   : Adaptations CNA DEM-483
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Action de configuration des equipements domaine Protection incendie par sequence.
* 
* Sequence d'appel
* SP	XZAC104_Commande_Seq_INC
* 
* Arguments en entree
* XDY_Entier	va_NumEvt_in
* XDY_Octet	    va_CleEvt_in
* XDY_Machine	va_PosteOperateur_in
* XDY_Eqt		va_NumEqt_in
* XDY_Horodate	va_HeureLancement_in
* XDY_Booleen	va_Manuelle_in
* XDY_Octet	    va_Priorite_in
* XDY_Entier	va_DistanceEvt_in
* XDY_Octet	    va_Sequence_in
*  
* Arguments en sortie
* XDY_Entier	va_NumAction_out
* 
* Code retour
* XDC_OK
* XDC_NOK	     : evenement non trouve, equipement indisponible, 
*                  insertion impossible, pb commande
* XDC_REV_PB_DCL : probleme pour declencher le reveil
* XDC_PRC_INC	 : ARRP03 inexistante
* XDC_ARG_INV	 : parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* L'execution de l'actio est realisee dans le 
* cas de commande automatique par sequence
* Si l'actio a ete proposee par le plan d'actions, 
* positionner Priorite et DistanceEvt, sinon, les mettre a 0
* Si l'operateur pilote equipement par equipement, 
* positionner Manuelle a XDC_Vrai (l'operateur doit 
* quand meme renseigner la sequence a laquelle il compte aboutir)
* Fonction
* Verifier l'existence de l'evenement
* Verifier l'existence de l'equipement non indisponible 
* dans le district ou l'actio est executee.
* 
* Inserer dans la table ACTION et la table AFFICHAGE_INC.
* Traitement sur l'actio precedente sur l'equipement a commander :
* 	Chercher l'actio precedente :
* 	SELECT numero FROM ACTION 
* 	WHERE equipement, type, heure_lancement_fin = null, 
* 		heure_lancement = all
* 		(SELECT max(heure_lancement) FROM ACTION 
* 		WHERE equipement, type)
* 	
* 	Positionner heure_lancement_fin a HeureLancement 
* 	pour l'actio precedente
* 	Si l'actio precedente trouvee est liee a un autre 
* 	evenement alors l'autre evenement doit 	etre reveille 
* 	pour vol (appeler XZAR03).
* 
* Dans le cas de commande automatique par sequence :
* 	ACRP44 : Appeler XZET03 pour commander l'equipement
-------------------------------------------------*/

use PRC
go

create procedure XZAC;104
	@va_NumEvt_in			    int		     = null,
	@va_CleEvt_in			    tinyint		 = null,
	@va_PosteOperateur_in		T_OPERATEUR	 = null,
	@va_TypEqt_in			    tinyint      = null,
	@va_NumEqt_in			    T_EQUIPEMENT = null,
	@va_HeureLancement_in		datetime	 = null,
	@va_Manuelle_in			    bit		      ,
	@va_Priorite_in			    tinyint		 = null,
	@va_DistanceEvt_in		    int		     = null,
	@va_Sequence_in			    tinyint		 = null,
	@va_NumeroAction_out		int		     = null	output,
	@va_NomSiteLocal_in             char(2)         = null,
	@va_distant_in 			int 		= XDC_NON
as
	declare @vl_Site tinyint, @vl_NumActionPrecedente int,
		@vl_NumEvtActionPrecedente int, @vl_CleEvtActionPrecedente tinyint,
		@vl_Status int, @vl_Texte char(100), @vl_NomEqt char(10),
		@vl_NomMachine char(10), @vl_NomSite char(2),
		@vl_PR T_PR, @vl_Sens T_SENS, @vl_Autoroute T_AUTOROUTE, 
		@vl_Intitule varchar(50), @vl_site_in T_SITE,
		@vl_serveur_pra char(6)	

	/*! transfo en param�tres nulls bug applix */
	if @va_NumEvt_in = 0 select @va_NumEvt_in = null
	if @va_CleEvt_in = 0 select @va_CleEvt_in = null
	if @va_PosteOperateur_in = 0 select @va_PosteOperateur_in = null
	if @va_NumEqt_in = 0 select @va_NumEqt_in = null
	if @va_Sequence_in = 0 select @va_Sequence_in = null
	if @va_DistanceEvt_in = XDC_DISTANCE_INCONNUE 
		select @va_DistanceEvt_in = null, @va_Priorite_in = null
	if @va_HeureLancement_in = XDC_DATE_NULLE select @va_HeureLancement_in = null
	if @va_NomSiteLocal_in = "" select @va_NomSiteLocal_in = null

	/*A controle des parametres obligatoires */
	if @va_NumEvt_in = null or @va_CleEvt_in = null or @va_NumEqt_in = null or
	   @va_PosteOperateur_in = null or @va_HeureLancement_in = null or @va_Sequence_in = null
		return XDC_ARG_INV

	/*A Recherche du site */
	/* MOVIS */
	if @va_NomSiteLocal_in = null
		select @vl_Site = numero 
    		from   CFG..RES_DIS
    		where  serveur = @@servername
	else
		if @va_distant_in = XDC_NON
		begin
			select @vl_Site = numero
	        	from CFG..RES_DIS
			where code = @va_NomSiteLocal_in
		end
		else
		begin
			select @vl_site_in= sit
			from    EXP..EQT_DSP
			where	equipement  = 	@va_NumEqt_in  and 
				type 		= 	@va_TypEqt_in  and
				dernier		=	1
			select @vl_Site = @vl_site_in
		end

    if @vl_Site is null
        return XDC_NOK
	
	/* modif ABE du 05-02-2020 : sp�cificit� toulon */
	select @vl_site_in= sit
	       from    EXP..EQT_DSP
			where	equipement  = 	@va_NumEqt_in  and 
					type 		= 	@va_TypEqt_in  and
					dernier		=	1  						
	if @vl_Site = XDC_CI and @va_distant_in = XDC_NON
	begin
		select @vl_serveur_pra=serveur_pra from CFG..RES_PRA where numero=@vl_site_in
		if @vl_serveur_pra= XDC_SQLVC
			exec @vl_Status=SQL_DM.PRC..XZAC;104 @va_NumEvt_in,@va_CleEvt_in,@va_PosteOperateur_in,@va_TypEqt_in,@va_NumEqt_in,@va_HeureLancement_in,@va_Manuelle_in,@va_Priorite_in,@va_DistanceEvt_in,@va_Sequence_in,@va_NumeroAction_out, @va_NomSiteLocal_in, XDC_OUI
		
		/* modif ABE du 05-02-2020 : sp�cificit� toulon */
		else if @vl_serveur_pra= XDC_SQLDP
			exec @vl_Status=SQL_DS.PRC..XZAC;104 @va_NumEvt_in,@va_CleEvt_in,@va_PosteOperateur_in,@va_TypEqt_in,@va_NumEqt_in,@va_HeureLancement_in,@va_Manuelle_in,@va_Priorite_in,@va_DistanceEvt_in,@va_Sequence_in,@va_NumeroAction_out, @va_NomSiteLocal_in, XDC_OUI
			
		else if @vl_serveur_pra= XDC_SQLCI
			exec @vl_Status=SQL_CI.PRC..XZAC;104 @va_NumEvt_in,@va_CleEvt_in,@va_PosteOperateur_in,@va_TypEqt_in,@va_NumEqt_in,@va_HeureLancement_in,@va_Manuelle_in,@va_Priorite_in,@va_DistanceEvt_in,@va_Sequence_in,@va_NumeroAction_out, @va_NomSiteLocal_in, XDC_OUI
		return @vl_Status
	end
	/* pas de traitement distant sur autres sites, tout s'effectue en local */


	/*A verifier l'existence de l'evenement */
	if not exists (	select numero 
	                from   EXP..FMC_GEN
			        where  numero = @va_NumEvt_in and
			               cle    = @va_CleEvt_in)
		return XDC_NOK

	/*A verifier la non indisponibilite de l'equipement et */
	/*A son existence sur ce district                      */
	if not exists ( select  equipement 
	                from    EXP..EQT_DSP
        			where	equipement = @va_NumEqt_in   and 
        			        type       = @va_TypEqt_in   and
        				    majeure    = XDC_NON         and 
        				    critique   = XDC_NON         and
        				    HS         = XDC_NON         and 
        				    inhibe     = XDC_NON         and
        				    desactive  = XDC_NON         and 
        				    sit        = @vl_Site)
		return XDC_NOK

	/*A calcul le numero d'actio a inserer */
	select @va_NumeroAction_out = isnull(max(numero), 0) + 1 
	from   EXP..ACT_GEN
	where  sit = @vl_Site

	/*A inserer l'actio PROTECTION INCENDIE */
	begin tran
		insert EXP..ACT_GEN ( numero,
		                      sit,
		                      evenement,
		                      cle,
		                      type,
		                      equipement,
		                      heure_lancement,
		                      priorite,
		                      prioritaire,
		                      distance_evt,
		                      operateur,
		                      sit_origine)
		             values ( @va_NumeroAction_out,
		                      @vl_Site,
		                      @va_NumEvt_in,
		                      @va_CleEvt_in,
		                      XDC_ACT_GTC_INC,
		                      @va_NumEqt_in,
		                      @va_HeureLancement_in,
		                      @va_Priorite_in, 
		                      XDC_NON,
		                      @va_DistanceEvt_in,
		                      @va_PosteOperateur_in,
		                      @vl_Site)

		insert EXP..ACT_GTC_INC ( actio,
		                          type,
		                          equipement,
		                          sit,
		                          sequence,
		                          manuelle)
		                 values ( @va_NumeroAction_out,
		                          @va_TypEqt_in,
		                          @va_NumEqt_in,
		                          @vl_Site, 
		                          @va_Sequence_in,
		                          @va_Manuelle_in )

	commit tran

	/*A commander l'equipement (le plus tot possible) */
	if @va_Manuelle_in = XDC_NON
	begin
		/* PRA
		if @@servername = XDC_SQLCI
		*/
		if @vl_Site = XDC_CI
			exec @vl_Status = TASRV_CI...ACRP44 @va_TypEqt_in,
												@va_NumEqt_in,
			                                    @va_Sequence_in,
			                                    @va_NumeroAction_out
		else /* PRA
		     if @@servername = XDC_SQLDP
		     */
		    if @vl_Site = XDC_DP
				exec @vl_Status = TASRV_DP...ACRP44 @va_TypEqt_in,
													@va_NumEqt_in, 
													@va_Sequence_in,
													@va_NumeroAction_out
		else /* PRA
		     if  @@servername = XDC_SQLVC
		     */
		    if @vl_Site = XDC_VC
				exec @vl_Status = TASRV_VC...ACRP44 @va_TypEqt_in,
													@va_NumEqt_in,
													@va_Sequence_in,
													@va_NumeroAction_out
		else 
		    if @vl_Site = XDC_CA
				exec @vl_Status = TASRV_CA...ACRP44 @va_TypEqt_in,
													@va_NumEqt_in,
													@va_Sequence_in,
													@va_NumeroAction_out
	
		if @vl_Status != XDC_OK
			return @vl_Status
	end

	/*A Rechercher l'actio precedente sur cet equipement non terminee */
	select @vl_NumActionPrecedente    = EXP..ACT_GEN.numero,
		   @vl_NumEvtActionPrecedente = EXP..ACT_GEN.evenement,
		   @vl_CleEvtActionPrecedente = EXP..ACT_GEN.cle,
		   @vl_Autoroute              = CFG..EQT_GEN.autoroute,
		   @vl_PR                     = CFG..EQT_GEN.PR,
		   @vl_Sens                   = CFG..EQT_GEN.sens
	from   EXP..ACT_GEN,
	       CFG..EQT_GEN
	where  EXP..ACT_GEN.equipement          = @va_NumEqt_in        and 
		   EXP..ACT_GEN.type                = XDC_ACT_GTC_INC      and
		   EXP..ACT_GEN.sit                 = @vl_Site             and 
		   EXP..ACT_GEN.numero             != @va_NumeroAction_out and
		   CFG..EQT_GEN.numero              = @va_NumEqt_in        and 
		   CFG..EQT_GEN.type                = @va_TypEqt_in        and
		   EXP..ACT_GEN.heure_lancement_fin is null                and
		   heure_lancement                  in ( select max(heure_lancement)
		                                         from   EXP..ACT_GEN
		                                         where	equipement =  @va_NumEqt_in   and
		                                                type       =  XDC_ACT_GTC_INC and
		                                                numero     != @va_NumeroAction_out )

	if @@rowcount = 1
	begin
		/*A Positionner l'heure de lancement de fin                     */
		update EXP..ACT_GEN 
		set    heure_lancement_fin = @va_HeureLancement_in
		where  numero = @vl_NumActionPrecedente and
		       sit    = @vl_Site
	
		/*A Reveiller si l'evenement liee a l'actio precedente est different */
		/*! on considere qu'aucun probleme de reveil puisse arriver          */
		if @vl_NumEvtActionPrecedente != @va_NumEvt_in or 
		   @vl_CleEvtActionPrecedente != @va_CleEvt_in
		begin
			/* nommage de l'equipement */
			exec XZAC;60 XDC_EQT_INC,
			             @vl_Autoroute,
			             @vl_PR,
			             @vl_Sens,
			             @vl_Intitule output

			/* texte du reveil */
			select @vl_Texte = XDC_REV_VOL_EQT + @vl_Intitule + XDC_REV_VOL_EQT_SUITE,
				@vl_NomMachine = null

			/* recherche du nom du site */
			select  @vl_NomSite = code
			from    CFG..RES_DIS
			where   numero = @vl_Site

			if @@rowcount != 1
				return XDC_NOK

			/* declencher le reveil pour le site local */
			if  @vl_Site = XDC_CI
				exec @vl_Status = TASRV_CI...ARRP03 @vl_NumEvtActionPrecedente, 
						                            @vl_CleEvtActionPrecedente,
						                            @vl_NomMachine,
						                            @vl_NomSite, @vl_Texte
			else if  @vl_Site = XDC_DP
				exec @vl_Status = TASRV_DP...ARRP03 @vl_NumEvtActionPrecedente,
				                                    @vl_CleEvtActionPrecedente,
				                                    @vl_NomMachine,
				                                    @vl_NomSite, @vl_Texte
			else if @vl_Site = XDC_VC
				exec @vl_Status = TASRV_VC...ARRP03 @vl_NumEvtActionPrecedente, 
						                            @vl_CleEvtActionPrecedente,
						                            @vl_NomMachine,
						                            @vl_NomSite, @vl_Texte
			else if @vl_Site = XDC_CA
				exec @vl_Status = TASRV_CA...ARRP03 @vl_NumEvtActionPrecedente, 
						                            @vl_CleEvtActionPrecedente,
						                            @vl_NomMachine,
						                            @vl_NomSite, @vl_Texte
			else return XDC_NOK

			if @vl_Status = null
				return XDC_PRC_INC
			else if @vl_Status != XDC_OK
				return XDC_REV_PB_DCL
		end
	end


	return XDC_OK
go
