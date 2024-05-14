/*E*/
/*  Fichier : $Id: xzac65sp.prc,v 1.3 2020/10/13 17:31:15 pc2dpdy Exp $      Release : $Revision: 1.3 $        Date : $Date: 2020/10/13 17:31:15 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac65.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	04/09/96	: Creation
* C.P.	07/01/98	: Correction gestion multi-sites dans 
*			  les procs (1.2)
* LCL   25/03/20   	: MOVIS Ajout site local DEM-SAE93
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Modification du lien d'une action
* 
* Sequence d'appel
* SP	XZAC65_Modifier_Lien_Action
* 
* Arguments en entree
* XDY_Entier	va_NumAction_in
* XDY_Octet	va_Site_in
* XDY_Entier	va_NumEvt_in
* XDY_Octet	va_CleEvt_in
* 
* Arguments en sortie
* 
* Code retour
* XDC_OK
* XDC_NOK		: action ou evenement non trouve
* XDC_ARG_INV		: parametres d'entree null
* XDC_ACT_REFUSEE	: modif interdite sur ce site
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* 
* Fonction
* Verifier l'appartenance de l'action a ce site
* Verifier l'existence de l'evenement si le site n'est pas un PC simplifie
* modifier lien
* 
-----------------------------------------------------*/

use PRC
go

create procedure XZAC;65
	@va_NumAction_in		int		= null,
	@va_Site_in			T_SITE		= null,
	@va_NumEvt_in			int		= null,
	@va_CleEvt_in			tinyint		= null,
	@va_NomSiteLocal_in		char(2)		= null
as
	declare @vl_Site tinyint
		 
	/*! transfo en paramÉtres nulls bug applix */
	if @va_NumEvt_in = 0 select @va_NumEvt_in = null
	if @va_CleEvt_in = 0 select @va_CleEvt_in = null

	/*A controle des parametres obligatoires */
	if @va_NumEvt_in = null or @va_CleEvt_in = null 
		return XDC_ARG_INV

	/*A rechercher du site local */
        /*A recherche le site */
	/* MOVIS */
	if @va_NomSiteLocal_in = null
		select @vl_Site = numero 
	        from CFG..RES_DIS
	        where serveur = @@servername
	else
		select @vl_Site = numero 
	        from CFG..RES_DIS
		where code = @va_NomSiteLocal_in

        if @vl_Site is null
           	return XDC_NOK


	/*A tester que l'action a ete creation sur ce site */
	if @vl_Site != @va_Site_in
		return XDC_ACT_REFUSEE

#ifndef PC_SIMPL
	/* tester l'existance de l'evenement */
	if not exists ( select * from EXP..FMC_GEN 
			where numero = @va_NumEvt_in and cle = @va_CleEvt_in)
		return XDC_NOK
#endif

	update EXP..ACT_GEN  set evenement = @va_NumEvt_in, cle = @va_CleEvt_in
	where numero = @va_NumAction_in and sit = @va_Site_in

	if @@rowcount = 0
		return XDC_NOK
go
