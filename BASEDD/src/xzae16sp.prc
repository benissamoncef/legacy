/*E*/
/*  Fichier : $Id: xzae16sp.prc,v 1.6 2020/11/03 16:14:30 pc2dpdy Exp $        Release : $Revision: 1.6 $        Date : $Date: 2020/11/03 16:14:30 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae16sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* B.G.     	19/12/94	: Creation 	(1.1)
* JMG		23/09/96	: marche aussi sur pc simplifie (1.2)
* JMG		26/09/06	: compil conditionnelle sur pc simplifie (1.3)
* JMG		07/01/98	: gestion multi-sites (dem/1536) 1.4
* LCL   	22/04/20        : MOVIS Ajout site local pilotage DEM-SAE93
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Teste le droit d'achever la fiche main courante / aux actions non finies.
* 
* Sequence d'appel
* XZAE16_Tester_Achever_FMC
* 
* Arguments en entree
* XDY_Entier	va_NumEvt_in
* XDY_Octet	va_CleEvt_in
* XDY_Machine	va_PosteOperateur_in	: numero de machine
* 
* Arguments en sortie
* XDY_Entier	va_Resultat_out : XDC_OK si achevenement de l'evt est ok
* 
* Code retour
* XDC_OK
* XDC_NOK		: evenement inexistant, site local inconnu
* XDC_ARG_INV		: argument d'entree null, ou valeur incorrecte
* 
* Conditions d'utilisation
* va_Resultat_out vaut :
*   XDC_OK
*   XZAEC_FMC_ACT_NON_FIN : toutes les actions relatives a l'evenement ne sont 
*   	      		    terminees 
* Fonction
* 	-verifier que toutes les actions du site liees 
* 	 a la FMC sont finies, sinon refuser
-------------------------------------------------------------*/

use PRC
go

create procedure XZAE;16
	@va_NumEvt_in		int = null,
	@va_CleEvt_in		tinyint = null,
	@va_PosteOperateur_in	T_EQUIPEMENT = null,
	@va_Resultat_out	int = null	output,
	@va_NomSiteLocal_in             char(2)         = null
as
	declare @vl_Site T_SITE, @vl_Classe tinyint,
		@vl_SiteFmc T_SITE, @vl_Fin datetime,
		@vl_PosteEnrichisseur T_EQUIPEMENT

	/*! transfo en param…tres nulls bug applix */
	if @va_NumEvt_in = 0 select @va_NumEvt_in = null
	if @va_CleEvt_in = 0 select @va_CleEvt_in = null
	if @va_PosteOperateur_in = 0 select @va_PosteOperateur_in = null

	/*A controle des parametres obligatoires */
	if @va_NumEvt_in = null or @va_CleEvt_in = null
	   	return XDC_ARG_INV

	/* recherche du site local */
        /* MOVIS */
        if @va_NomSiteLocal_in = null
                select  @vl_Site = numero
                from CFG..RES_DIS
                where serveur = @@servername
        else
                select  @vl_Site = numero
                from CFG..RES_DIS
                where code = @va_NomSiteLocal_in

	/*A verifier que toutes les actions relatives a l'evenement */
	/*A sont terminees  sur le site local                       */
	if exists (	select * from EXP..ACT_GEN
			where	evenement = @va_NumEvt_in and
				cle = @va_CleEvt_in and
				heure_fin is null and
				sit=@vl_Site)
	begin
		select @va_Resultat_out = XZAEC_FMC_ACT_NON_FIN
		return XDC_OK
	end
#ifndef PC_SIMPL
	/*A verifier l'existance de l'evenement sur le site local */
	select 
		@vl_Fin			= EXP..FMC_GEN.fin,
		@vl_PosteEnrichisseur	= EXP..FMC_GEN.poste_enrichisseur,
		@vl_Classe		= CFG..TYP_FMC.classe,
		@vl_SiteFmc		= EXP..FMC_GEN.sit
	from EXP..FMC_GEN, CFG..TYP_FMC
	where	EXP..FMC_GEN.numero = @va_NumEvt_in and 
		EXP..FMC_GEN.cle = @va_CleEvt_in and
		CFG..TYP_FMC.numero = EXP..FMC_GEN.type

	if @@rowcount != 1
	begin
		return XDC_NOK
	end
#endif

	select @va_Resultat_out = XDC_OK

	return XDC_OK
go
