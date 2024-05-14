/*E*/
/*  Fichier : $Id: xzae015sp.prc,v 1.22 2021/05/04 13:29:13 pc2dpdy Exp $        $Revision: 1.22 $        $Date: 2021/05/04 13:29:13 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae015sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* GGY	26/05/23	: Creation => Correction Calcul Plan d'Action Consignation Bouchons (DEM-475)
* ABK   26/09/23 	: Ajout district CA (DEM-483)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Confie le droit d'enrichir au site ou au poste operateur.
* 
* Sequence d'appel
* XZAE015_Transmettre_Droit_Enrichir
* 
* Arguments en entree
* XDY_Entier	va_NumEvt_in
* XDY_Octet	va_CleEvt_in
* XDY_Octet	va_Site_in
* XDY_Machine	va_PosteOperateur_in
* XDY_Entier	va_Sans_Rafraichir_in
* XDY_Entier	va_Sans_Reveil_in
*
* Arguments en sortie
* XDY_Entier	va_Resultat_out
* 
* Code retour
* XDC_OK
* XDC_NOK		: modif du droit impossible
* XDC_ARG_INV		: argument d'entree null, ou valeur incorrecte
* XDC_PRC_INC		: procedure stockee appelee n'existe pas en base
* < 0                   : erreur sybase
* 
* Conditions d'utilisation
* Soit le poste operateur, soit le site doit etre value a non null
* Pas l'autorisation de transmettre un evenement de type operateur
* Valeur prise par va_Resultat_out :
* - XZAEC_FMC_PAS_ENR	: pas le droit de transmettre l'enrichissement 
*                         (evenement pas sur le site local ou
*                          de type operateur)
* - XZAEC_FMC_OPR_INC	: operateur n'est pas en activite
* - XDC_OK		: le droit d'enrichir a ete transmis
* 
* Fonction
* Pour le transfert du droit d'enrichir a un site : 
* 	Update du champ site dans la table EVENEMENT, 
* 	EVENEMENT_HISTORIQUE et dans 
* 	la table de complement d'evenement (ou insert 
* 	distant puis delete local)
* Pour le tranfert du droit d'enrichir a un poste operateur du meme site :
* 	Update du champ poste_enrichisseur dans la table EVENEMENT
---------------------------------------------------------------*/
use PRC
go

if exists (select * from sysobjects where name = 'XZAE015' and type = 'P')
	drop procedure XZAE015
go

create procedure XZAE015
	@va_NumEvt_in		    int	            = null,
	@va_CleEvt_in		    tinyint	        = null,
	@va_Site_in		        T_SITE	        = null,
	@va_PosteOperateur_in	T_EQUIPEMENT    = null,
	@va_Resultat_out	    int             = null	output,
	@va_Sans_Rafraichir_in	int             = null,
	@va_Sans_Reveil_in	    int             = null,
	@va_NomSiteLocal_in     char(2)         = null
as
	declare @vl_sqlserver char(6), 
                @vl_Status int

	/*! transfo en parametres nulls bug applix */
	if @va_NumEvt_in         = 0 select @va_NumEvt_in = null
	if @va_CleEvt_in         = 0 select @va_CleEvt_in = null
	if @va_Site_in           = 0 select @va_Site_in = null
	if @va_PosteOperateur_in = 0 select @va_PosteOperateur_in = null

	/*A recherche du site local servant de cle a l'evenement */
        select @vl_sqlserver = serveur_pra from CFG..RES_PRA where numero=@va_CleEvt_in


        if @vl_sqlserver = XDC_SQLVC
		exec @vl_Status = SQL_DM.PRC..XZAE15    @va_NumEvt_in,
                                                @va_CleEvt_in,
                                                @va_Site_in,
                                                @va_PosteOperateur_in,
                                                @va_Resultat_out output,
                                                @va_Sans_Rafraichir_in,
                                                @va_Sans_Reveil_in,
                                                @va_NomSiteLocal_in
	else if @vl_sqlserver = XDC_SQLDP
		exec @vl_Status = SQL_DS.PRC..XZAE15    @va_NumEvt_in,
                                                @va_CleEvt_in,
                                                @va_Site_in,
                                                @va_PosteOperateur_in,
                                                @va_Resultat_out output,
                                                @va_Sans_Rafraichir_in,
                                                @va_Sans_Reveil_in,
                                                @va_NomSiteLocal_in
	else if @vl_sqlserver = XDC_SQLCI
		exec @vl_Status = SQL_CI.PRC..XZAE15    @va_NumEvt_in,
                                                @va_CleEvt_in,
                                                @va_Site_in,
                                                @va_PosteOperateur_in,
                                                @va_Resultat_out output,
                                                @va_Sans_Rafraichir_in,
                                                @va_Sans_Reveil_in,
                                                @va_NomSiteLocal_in
        else if @vl_sqlserver = XDC_SQLCA
		exec @vl_Status = SQL_CA.PRC..XZAE15    @va_NumEvt_in,
                                                @va_CleEvt_in,
                                                @va_Site_in,
                                                @va_PosteOperateur_in,
                                                @va_Resultat_out output,
                                                @va_Sans_Rafraichir_in,
                                                @va_Sans_Reveil_in,
                                                @va_NomSiteLocal_in
	else 
        return XDC_NOK

	return @vl_Status
go
