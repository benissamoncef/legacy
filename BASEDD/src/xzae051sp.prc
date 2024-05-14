/*E*/
/*  Fichier : $Id: xzae051sp.prc,v 1.12 2019/04/23 11:07:30 pc2dpdy Exp $      Release : $Revision: 1.12 $        Date : $Date: 2019/04/23 11:07:30 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae051.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* GGY	01/06/23	: Creation => Correction Calcul Plan d'Action Consignation Bouchons (DEM-475)
* ABK   26/09/23 	: Ajout district CA (DEM-483)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Retourne les champs d'une fiche main courante bouchon a l'heure precisee
* 
* Sequence d'appel
* SP	XZAE051_Lire_Fiche_Bouchon
* 
* Arguments en entree
* XDY_Horodate		va_Horodate_in : par defaut heure courante(non utilisee)
* XDY_Entier		va_NumEvt_in
* XDY_Octet		va_CleEvt_in
* 
* Arguments en sortie
* XDY_PR		va_PRQueue_out
* XDY_Octet		va_Type_out
* XDY_Entier		va_Longueur_out
* XDY_Entier		va_NumEvt_out
* XDY_Octet		va_CleEvt_out
* 
* Code retour
* XDC_OK
* XDC_NOK : pas de complement de fiche main courante
* XDC_ARG_INV
* < 0 erreur sybase
* 
* Conditions d'utilisation
* En formation, preciser l'horodate
* Penser a lire les 2 fiches generiques tete et queue 
* de bouchon avant de lire le complement.
* Si l'evenement en entree correspond a une tete, le numero 
* d'evenement en sortie est ceui de la queue et inversement
* 
* La procedure stockee au CI recherche les informations 
* dans la base HIS si la date donnee est anterieure de plus d'un 
* mois tandis qu'au niveau des districts la recherche s'effectue 
* dans la base EXP.
* Le code de creation de la procedure au CI et aux districts 
* doit etre identique a l'exception de la base de recherche des infos
* et du test de la date en entree (< 1 mois)
* (toute modif dans la creation de la procedure doit etre 
*  effectuee pour le CI et les districts)
* 
* 
* Fonction
* Select dans la table QUEUE_DE_BOUCHON
* where numero and horodate
* Calcul de la distance entre PR tete et PR queue.
-----------------------------------------------------*/

use PRC
go

if exists (select 1 from sysobjects where name = 'XZAE051' and type = 'P')
	drop procedure XZAE051
go

create procedure XZAE051
	@va_Horodate_in		datetime = null, -- par defaut heure courante
	@va_NumEvt_in		int = null,
	@va_CleEvt_in		tinyint = null,
	@va_PRQueue_out		T_PR = null	output,
	@va_Type_out		tinyint = null	output,
	@va_Longueur_out	int = null	output,
	@va_NumEvt_out		int = null	output,
	@va_CleEvt_out		tinyint = null	output
as
	declare @vl_sqlserver char(6), 
			@vl_Status int

	/*! transfo en parametres nulls bug applix */
	if @va_NumEvt_in = 0 select @va_NumEvt_in = null
	if @va_CleEvt_in = 0 select @va_CleEvt_in = null
	if @va_Horodate_in = XDC_DATE_NULLE select @va_Horodate_in = null

	/*A test la presence des parametres obligatoires */
	if @va_NumEvt_in = null or @va_CleEvt_in = null
		return XDC_ARG_INV

	/*A recherche du site local servant de cle a l'evenement */
    select @vl_sqlserver = serveur_pra from CFG..RES_PRA where numero=@va_CleEvt_in

	if @vl_sqlserver is null
		return XDC_NOK

    if @vl_sqlserver = XDC_SQLVC
		exec @vl_Status = SQL_DM.PRC..XZAE;51   @va_Horodate_in,
                                                @va_NumEvt_in,
                                                @va_CleEvt_in,
                                                @va_PRQueue_out output,
                                                @va_Type_out output,
                                                @va_Longueur_out output,
                                                @va_NumEvt_out output,
                                                @va_CleEvt_out output

	else if @vl_sqlserver = XDC_SQLDP
		exec @vl_Status = SQL_DS.PRC..XZAE;51   @va_Horodate_in,
                                                @va_NumEvt_in,
                                                @va_CleEvt_in,
                                                @va_PRQueue_out output,
                                                @va_Type_out output,
                                                @va_Longueur_out output,
                                                @va_NumEvt_out output,
                                                @va_CleEvt_out output

	else if @vl_sqlserver = XDC_SQLCI
		exec @vl_Status = SQL_CI.PRC..XZAE;51   @va_Horodate_in,
                                                @va_NumEvt_in,
                                                @va_CleEvt_in,
                                                @va_PRQueue_out output,
                                                @va_Type_out output,
                                                @va_Longueur_out output,
                                                @va_NumEvt_out output,
                                                @va_CleEvt_out output
	else if @vl_sqlserver = XDC_SQLCA
		exec @vl_Status = SQL_CA.PRC..XZAE;51   @va_Horodate_in,
                                                @va_NumEvt_in,
                                                @va_CleEvt_in,
                                                @va_PRQueue_out output,
                                                @va_Type_out output,
                                                @va_Longueur_out output,
                                                @va_NumEvt_out output,
                                                @va_CleEvt_out output

	else 
                return XDC_NOK

	return @vl_Status
go
