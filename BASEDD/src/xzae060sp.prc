/*E*/
/*  Fichier : $Id: xzae060sp.prc,v 1.10 2020/11/03 16:53:20 pc2dpdy Exp $      Release : $Revision: 1.10 $        Date : $Date: 2020/11/03 16:53:20 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae060sp.prc
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
* Modifier une fiche de bouchon relative a un evenement 
* de queue de bouchon existant
* 
* Sequence d'appel
* SP	XZAE060_Ecrire_Fiche_Bouchon
* 
* Arguments en entree
* XDY_Entier		va_NumEvenement_in
* XDY_Octet		va_CleEvenement_in
* XDY_Octet		va_Type_in
* XDY_Horodate		va_Horodate_in
* XDY_Eqt		va_PosteOperateur_in
* XDY_Octet		va_Previson_Traiter_Clos_in
* 
* Arguments en sortie
* 
* Code retour
* XDC_OK
* XDC_NOK	: evenement non trouve, insert ou update impossible,
*                 poste enrichisseur different
* XDC_ARG_INV	: parametres d'entree null
* 
* Conditions d'utilisation
* la date passee en parametre d'entree doit etre ensuite passee
* a la procedure de validation de la fiche main courante (XZAE11)
* 
* Fonction
* Modifier dans la table QUEUE_BOUCHON
-------------------------------------------------------*/
use PRC
go

if exists (select * from sysobjects where name = 'XZAE060' and type = 'P')
	drop procedure XZAE060
go

create procedure XZAE060
	@va_NumEvenement_in	            int	        = null,
	@va_CleEvenement_in	            tinyint	    = null,
	@va_Type_in		                tinyint     = null,
	@va_Horodate_in		            datetime    = null,
	@va_PosteOperateur_in	        T_EQUIPEMENT,
	@va_Prevision_Traiter_Clos_in   tinyint     = null,
	@va_NomSiteLocal_in             char(2)     = null
as
	declare @vl_sqlserver char(6), 
          	@vl_Status int

	/*! transfo en parametres nulls bug applix */
	if @va_NumEvenement_in = 0 select @va_NumEvenement_in = null
	if @va_CleEvenement_in = 0 select @va_CleEvenement_in = null
	if @va_Type_in = 0 select @va_Type_in = null
	if @va_PosteOperateur_in = 0 select @va_PosteOperateur_in = null
	if @va_Prevision_Traiter_Clos_in = 0 select @va_Prevision_Traiter_Clos_in = null
	if @va_Horodate_in = XDC_DATE_NULLE select @va_Horodate_in = null

	/*A controle parametres en entree */
	if @va_NumEvenement_in  = null or @va_CleEvenement_in = null or @va_Horodate_in = null or @va_PosteOperateur_in = null or @va_Prevision_Traiter_Clos_in = null
		return XDC_ARG_INV

	/*A recherche du site local servant de cle a l'evenement */
    select @vl_sqlserver = serveur_pra from CFG..RES_PRA where numero=@va_CleEvenement_in

    if @vl_sqlserver = XDC_SQLVC
		exec @vl_Status = SQL_DM.PRC..XZAE;60   @va_NumEvenement_in,
                                                @va_CleEvenement_in,
                                                @va_Type_in,
                                                @va_Horodate_in,
                                                @va_PosteOperateur_in,
                                                @va_Prevision_Traiter_Clos_in,
                                                @va_NomSiteLocal_in
	else if @vl_sqlserver = XDC_SQLDP
		exec @vl_Status = SQL_DS.PRC..XZAE;60   @va_NumEvenement_in,
                                                @va_CleEvenement_in,
                                                @va_Type_in,
                                                @va_Horodate_in,
                                                @va_PosteOperateur_in,
                                                @va_Prevision_Traiter_Clos_in,
                                                @va_NomSiteLocal_in
	else if @vl_sqlserver = XDC_SQLCI
		exec @vl_Status = SQL_CI.PRC..XZAE;60   @va_NumEvenement_in,
                                                @va_CleEvenement_in,
                                                @va_Type_in,
                                                @va_Horodate_in,
                                                @va_PosteOperateur_in,
                                                @va_Prevision_Traiter_Clos_in,
                                                @va_NomSiteLocal_in
	else if @vl_sqlserver = XDC_SQLCA
		exec @vl_Status = SQL_CA.PRC..XZAE;60   @va_NumEvenement_in,
                                                @va_CleEvenement_in,
                                                @va_Type_in,
                                                @va_Horodate_in,
                                                @va_PosteOperateur_in,
                                                @va_Prevision_Traiter_Clos_in,
                                                @va_NomSiteLocal_in
	else 
        return XDC_NOK

	return @vl_Status
go
