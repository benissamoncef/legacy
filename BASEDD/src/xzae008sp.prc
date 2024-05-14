/*E*/
/* Fichier : $Id: xzae008sp.prc,v 1.32 2020/11/03 16:11:26 pc2dpdy Exp $        $Revision: 1.32 $        $Date: 2020/11/03 16:11:26 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae008sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* GGY	26/05/23	: Creation => Correction Calcul Plan d'Action Consignation Bouchons (DEM-475)
* ABK	26/09/23 : Ajout district CA (DEM-483)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Cree une fiche main courante
* 
* Sequence d'appel
* SP	XZAE008_Creer_Fiche_MC
* 
* Arguments en entree
* XDY_Octet	va_TypeEvenement_in
* XDY_Machine	va_MachineOperateur_in
* XDY_Booleen	va_Prevision_ATraiter_in
* XDY_Horodate	va_HoroDebut_in
* XDY_Horodate	va_HoroDebutPrevu_in
* XDY_Mot	va_Site_creation_in
* XDY_Mot	va_Degrade_in
* 
* Arguments en sortie
* XDY_Entier	va_NumEvenement_out
* XDY_Octet	va_CleEvenement_out
* XDY_Entier	va_NumFicheAppel_out
* 
* Code retour
* XDC_OK
* XDC_NOK	: site local incorrect, insertion impossible, 
*                 type inconnu
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* Si l'evenement est a traiter, la machine operateur doit
* etre valuee 
* Si l'evenement n'est a traiter, la machine operateur doit
* etre a null (etat en prevision) 
* 
* Fonction
* Inserer dans la table EVENEMENT en indiquant le type 
* d'evenement et le site deduit local.
* Inserer dans la table de complement d'evenement en adequation 
* avec le type d'evenement.
* La fonction retourne le numero et la cle de l'evenement.
* Si l'evenement est a traiter, Insert dans la table a traiter
------------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAE008' and type = 'P')
	drop procedure XZAE008
go

create procedure XZAE008
			@va_TypeEvenement_in		smallint = null,
			@va_MachineOperateur_in		T_EQUIPEMENT = null,
			@va_Prevision_ATraiter_in	bit,
			@va_HoroDebut_in		datetime = null,
			@va_HoroDebutPrevu_in		datetime = null,
			@va_Site_creation_in		tinyint = null,
			@va_Degrade_in			tinyint = null,
			@va_NumEvenement_out		int 	= null	output,
			@va_CleEvenement_out		tinyint = null	output,
			@va_NumFicheAppel_out		int = null	output,
			@va_NumAlerte_in		int = null,
			@va_SiteAlerte_in		tinyint = null
as
	declare @vl_sqlserver char(6), 
			@vl_Status int

	/*! transfo en parametres nulls bug applix */
	if @va_Site_creation_in = 0 select @va_Site_creation_in = null
	if @va_TypeEvenement_in = 0 select @va_TypeEvenement_in = null
	if @va_MachineOperateur_in = 0 select @va_MachineOperateur_in = null
	if @va_NumAlerte_in = 0 select @va_NumAlerte_in = null
	if @va_SiteAlerte_in = 0 select @va_SiteAlerte_in = null
	if @va_HoroDebut_in = XDC_DATE_NULLE select @va_HoroDebut_in = null
	if @va_HoroDebutPrevu_in = XDC_DATE_NULLE select @va_HoroDebutPrevu_in = null

	/*A Controle des parametres obligatoires en entree */
	if @va_TypeEvenement_in = null or 
        (@va_HoroDebut_in = null and @va_HoroDebutPrevu_in = null) or
	    (@va_HoroDebut_in is not null and @va_HoroDebutPrevu_in is not null) or
	    (@va_Prevision_ATraiter_in = XDC_NON and @va_MachineOperateur_in != null)
	        return XDC_ARG_INV

	/*A recherche du site local servant de cle a l'evenement */
        select @vl_sqlserver = serveur_pra from CFG..RES_PRA where numero=@va_Site_creation_in

	if @vl_sqlserver is null
		return XDC_NOK

        if @vl_sqlserver = XDC_SQLVC
		exec @vl_Status = SQL_DM.PRC..XZAE08   @va_TypeEvenement_in,
                                                @va_MachineOperateur_in,
                                                @va_Prevision_ATraiter_in,
                                                @va_HoroDebut_in,
                                                @va_HoroDebutPrevu_in,
                                                @va_Site_creation_in,
                                                @va_Degrade_in,
                                                @va_NumEvenement_out output,
                                                @va_CleEvenement_out output,
                                                @va_NumFicheAppel_out output,
                                                @va_NumAlerte_in,
                                                @va_SiteAlerte_in
	else if @vl_sqlserver = XDC_SQLDP
		exec @vl_Status = SQL_DS.PRC..XZAE08   @va_TypeEvenement_in,
                                                @va_MachineOperateur_in,
                                                @va_Prevision_ATraiter_in,
                                                @va_HoroDebut_in,
                                                @va_HoroDebutPrevu_in,
                                                @va_Site_creation_in,
                                                @va_Degrade_in,
                                                @va_NumEvenement_out output,
                                                @va_CleEvenement_out output,
                                                @va_NumFicheAppel_out output,
                                                @va_NumAlerte_in,
                                                @va_SiteAlerte_in
	else if @vl_sqlserver = XDC_SQLCI
		exec @vl_Status = SQL_CI.PRC..XZAE08   @va_TypeEvenement_in,
                                                @va_MachineOperateur_in,
                                                @va_Prevision_ATraiter_in,
                                                @va_HoroDebut_in,
                                                @va_HoroDebutPrevu_in,
                                                @va_Site_creation_in,
                                                @va_Degrade_in,
                                                @va_NumEvenement_out output,
                                                @va_CleEvenement_out output,
                                                @va_NumFicheAppel_out output,
                                                @va_NumAlerte_in,
                                                @va_SiteAlerte_in
	else if @vl_sqlserver = XDC_SQLCA
		exec @vl_Status = SQL_CA.PRC..XZAE08   @va_TypeEvenement_in,
                                                @va_MachineOperateur_in,
                                                @va_Prevision_ATraiter_in,
                                                @va_HoroDebut_in,
                                                @va_HoroDebutPrevu_in,
                                                @va_Site_creation_in,
                                                @va_Degrade_in,
                                                @va_NumEvenement_out output,
                                                @va_CleEvenement_out output,
                                                @va_NumFicheAppel_out output,
                                                @va_NumAlerte_in,
                                                @va_SiteAlerte_in
	else 
                return XDC_NOK

	return @vl_Status
go
