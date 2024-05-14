/*E*/
/*  Fichier : $Id: xzae0210sp.prc,v 1.8 2020/11/03 16:36:45 pc2dpdy Exp $        Release : $Revision: 1.8 $        Date : $Date: 2020/11/03 16:36:45 $
-------------------------------------------------------------------------------
* ESCOTA *  PROJET PASTRE
-------------------------------------------------------------------------------
* SOUS-SYSTEME  BASEDD
-------------------------------------------------------------------------------
* FICHIER xzae0210sp.prc
-------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Lecture / ecriture des fiches Main courante
*
-------------------------------------------------------------------------------
* HISTORIQUE :
*
* GGY	26/05/23	: Creation => Correction Calcul Plan d'Action Consignation Bouchons (DEM-475)
* ABK   26/09/23 	: Ajout district CA (DEM-483)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------------------------------
* SERVICE RENDU
*
*  Effectue la reservation ou la liberation d'une fiche Main courante
*  pour un poste operateur donne.
*
* -----------------------------------------------------------------------------
* SEQUENCE D'APPEL
*
* SP	XZAE0210_Reserver_FMC
*
* PARAMETRES EN ENTREE :
*
* XDY_Entier	va_NumEvt_in		: numero de l'evenement
* XDY_Octet	va_CleEvt_in		: cle de l'evenement
* XDY_Machine	va_PosteOperateur_in	: numero de la machine de l'operateur
* XDY_Booleen	va_Reserver_in		: vrai pour reserver, faux pour liberer
*
* PARAMETRES EN SORTIE :
*
* XDY_Booleen	va_Modifie_out		: Vrai ssi. l'operation a modifie les donnees
*
* VALEUR RENDUE :
*
* Aucune
*
* CODE RETOUR :
*
* XDC_OK
* XDC_NOK           : evenement inconnu ou de type incorrect pour l'operation
* XZAEC_FMC_PAS_ENR : evenement non modifiable par cet operateur
* XDC_ARG_INV       : parametre d'entree obligatoire manquant (Null) ou invalide
* <0                : erreur sybase
*
* CONDITIONS D'UTILISATION :
*
* FONCTION :
*
*  Reservation :
*    Si l'evenement n'a ete reserve par aucun autre poste operateur,
*    le marquer pour modification par ce poste ;
*
*  Liberation :
*    Si l'evenement a ete reserve par ce poste operateur,
*    oter la marque de reservation par ce poste.
*
-----------------------------------------------------------------------------*/
use PRC
go

if exists (select * from sysobjects where name = 'XZAE0210' and type = 'P')
	drop procedure XZAE0210
go

create procedure XZAE0210
	@va_NumEvt_in		    int		    = null,
	@va_CleEvt_in		    tinyint		= null,
	@va_PosteOperateur_in	T_OPERATEUR	= null,
	@va_Reserver_in		    bit		    = XDC_FAUX,
	@va_Modifie_out		    bit		    = XDC_NON	output,
	@va_NomSiteLocal_in     char(2)     = null
as

declare @vl_sqlserver	char(6), 
        @vl_Status		int

	/*A Valeur par defaut du parametre resultat */
	select @va_Modifie_out = XDC_NON

	/*A Transformation des parametres d'entree nulls (bug applix) */
	if @va_NumEvt_in = 0		    select @va_NumEvt_in = null
	if @va_CleEvt_in = 0		    select @va_CleEvt_in = null
	if @va_PosteOperateur_in = 0	select @va_PosteOperateur_in = null

	/* Verifier la validite des parametres d'entree obligatoires */
	if @va_NumEvt_in is null or @va_CleEvt_in is null or @va_PosteOperateur_in is null
		return XDC_ARG_INV

	if not exists (select nom from CFG..EQT_GEN where type = XDC_EQT_MAC and numero = @va_PosteOperateur_in)
		return XDC_ARG_INV

	/*A recherche du site local servant de cle a l'evenement */
    select @vl_sqlserver = serveur_pra from CFG..RES_PRA where numero=@va_CleEvt_in


    if @vl_sqlserver = XDC_SQLVC
		exec @vl_Status = SQL_DM.PRC..XZAE210   @va_NumEvt_in,
                                                @va_CleEvt_in,
                                                @va_PosteOperateur_in,
                                                @va_Reserver_in,
                                                @va_Modifie_out output,
                                                @va_NomSiteLocal_in
	else if @vl_sqlserver = XDC_SQLDP
		exec @vl_Status = SQL_DS.PRC..XZAE210   @va_NumEvt_in,
                                                @va_CleEvt_in,
                                                @va_PosteOperateur_in,
                                                @va_Reserver_in,
                                                @va_Modifie_out output,
                                                @va_NomSiteLocal_in
	else if @vl_sqlserver = XDC_SQLCI
		exec @vl_Status = SQL_CI.PRC..XZAE210   @va_NumEvt_in,
                                                @va_CleEvt_in,
                                                @va_PosteOperateur_in,
                                                @va_Reserver_in,
                                                @va_Modifie_out output,
                                                @va_NomSiteLocal_in
	else if @vl_sqlserver = XDC_SQLCA
		exec @vl_Status = SQL_CA.PRC..XZAE210   @va_NumEvt_in,
                                                @va_CleEvt_in,
                                                @va_PosteOperateur_in,
                                                @va_Reserver_in,
                                                @va_Modifie_out output,
                                                @va_NomSiteLocal_in
	else 
        return XDC_NOK

	return @vl_Status
go
