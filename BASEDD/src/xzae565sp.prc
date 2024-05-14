/*E*/
/* Fichier : $Id: xzae565sp.prc,v 1.4 2020/11/03 16:47:19 pc2dpdy Exp $      Release : $Revision: 1.4 $        Date : $Date: 2020/11/03 16:47:19 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae565sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* VR	08/03/12	: Creation (DEM/1016)
* PNI	25/04/19	: Correctif syntaxe 1.3
* LCL   22/04/20        : MOVIS Ajout site local pilotage DEM-SAE93
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Retourne les champs d'un complement de fiche main courante 
* de type regulation
* 
* Sequence d'appel
* SP	XZAE565_Lire_FMC_Regulation
* 
* Arguments en entree
* XDY_Horodate		va_Horodate_in : par defaut heure courante(non utilisee)
* XDY_Entier	va_Numero_in
* XDY_Entier  va_Cle_in
* 
* Arguments en sortie
*	XDY_Entier	va_Numero_in
*	XDY_Entier  va_Cle_in
*	XDY_Entier	va_Contexte_out
*	XDY_Entier	va_Scenario_out
*	XDY_Entier	va_Scenario_cible_out
*	XDY_Entier	va_Mesure_out
*	XDY_Entier	va_Indicateur_Horaire_out
*	char*		va_Transitoire_out
* 
* Code retour
* XDC_OK
* XDC_NOK : pas de complement de fiche main courante
* XDC_ARG_INV
* <0 erreur sybase 
* 
* Conditions d'utilisation
* En formation, preciser l'horodate
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
* Select dans les tables FMC_REG
* where numero numero and horodate
------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAE565' and type = 'P')
	drop procedure XZAE565
go

	
create procedure XZAE565
   @va_Horodate_in				datetime 		= null, -- par defaut heure courante
   @va_Numero_in				int,
   @va_Cle_in					tinyint,
   @va_NomSiteLocal_in             		char(2)         = null
   as 
   
   declare 
   @vl_Horodate datetime,
   @vl_Status int, 
   @va_Contexte_out  			int				,
   @va_Scenario_out 			int			,
   @va_Scenario_cible_out		int,
   @va_Mesure_out 				int				,
   @va_Indicateur_Horaire_out	int			,
   @va_Transitoire_out 			varchar(200)		,
   @va_Zone_out					int,
   @vl_Site				tinyint
   
	/*! transfo en parametres nulls bug applix */
	if @va_Numero_in = 0 select @va_Numero_in = null
	if @va_Cle_in = 0 select @va_Cle_in		  = null
	if @va_Horodate_in = XDC_DATE_NULLE select @va_Horodate_in = null

	/*A controle des parametres d'entree obligatoire */
	if @va_Numero_in = null or @va_Cle_in = null
		return XDC_ARG_INV

	/*A prendre l'heure courante dans le cas ou elle est nulle */
	if @va_Horodate_in = null
		select @va_Horodate_in = XDC_DATE_INFINIE


#ifdef HIST
        /* MOVIS */
        if @va_NomSiteLocal_in = null
                select  @vl_Site = numero
                from CFG..RES_DIS
                where serveur = @@servername
        else
                select  @vl_Site = numero
                from CFG..RES_DIS
                where code = @va_NomSiteLocal_in

	/* PRA
	if @@servername = XDC_SQLHIS
	*/
	if @vl_Site = XDC_HIS
	begin
		/*B lecture des informations Regulation */
		select
			@va_Contexte_out			= contexte,
			@va_Scenario_out			= scenario,
			@va_Scenario_cible_out		= scenario_cible,
			@va_Mesure_out				= mesure,
			@va_Indicateur_Horaire_out	= indicateur_horaire,
			@va_Transitoire_out			= transitoire,
			@va_Zone_out				= zone
		from HIS..FMC_REG
		where	numero = @va_Numero_in and cle = @va_Cle_in and
			horodate in ( 	select max(horodate) from HIS..FMC_REG
					where 	numero = @va_Numero_in and cle = @va_Cle_in and
						horodate <= @va_Horodate_in  and scenario is not null)
	
		/*B test si un seul complement */
		if @@rowcount != 1
			return XDC_NOK
	end

#else
#ifdef CI
	/*! cas ou la procedure est creee au CI */

	/*A si la date donnee est anterieure d'un mois au moins */
 	/*A rechercher l'evenement dans la base HIS sinon dans  */
	/*A la base EXP (au niveau du CI)                       */
	if @va_Horodate_in < (select dateadd(month, -1, getdate ()))
		begin
			exec @vl_Status = SQL_HIS.PRC..XZAE565
			@va_Horodate_in, @va_Numero_in, @va_Cle_in, @va_Contexte_out output, @va_Scenario_out output, @va_Scenario_cible_out output,@va_Mesure_out output,
			@va_Indicateur_Horaire_out output, @va_Transitoire_out output, @va_Zone_out output
			if @vl_Status = null
                        return XDC_PRC_INC
                else return @vl_Status
		end
	else
	begin
		/*B lecture des informations*/
		select
			@va_Contexte_out				= contexte,
			@va_Scenario_out				= scenario,
			@va_Scenario_cible_out			= scenario_cible,
			@va_Mesure_out					= mesure,
			@va_Indicateur_Horaire_out		= indicateur_horaire,
			@va_Transitoire_out				= transitoire,
			@va_Zone_out					= zone
		from EXP..FMC_REG
		where	numero = @va_Numero_in and cle = @va_Cle_in and
			horodate in ( 	select max(horodate) from EXP..FMC_REG
					where 	numero = @va_Numero_in and cle = @va_Cle_in and
							horodate <= @va_Horodate_in and scenario is not null)
	
		/*B test si un seul est trouve */
		if @@rowcount != 1
			return XDC_NOK
	
	end
#else
	/*! cas ou la procedure est creee sur le serveur d'un district */
	/*A lecture des informations */
	select
			@va_Contexte_out			= contexte,
			@va_Scenario_out			= scenario,
			@va_Scenario_cible_out		= scenario_cible,
			@va_Mesure_out				= mesure,
			@va_Indicateur_Horaire_out	= indicateur_horaire,
			@va_Transitoire_out			= transitoire,
			@va_Zone_out				= zone
	from EXP..FMC_REG
	where	numero = @va_Numero_in and cle = @va_Cle_in and
		horodate in ( 	select max(horodate) from EXP..FMC_REG
				where 	numero = @va_Numero_in and cle = @va_Cle_in and
					horodate <= @va_Horodate_in  and scenario_cible is not null)

	/*A test si un seul complement est trouve */
	if @@rowcount != 1
		return XDC_NOK

	
#endif
#endif

	select
			@va_Contexte_out			,
			@va_Scenario_out			,
			@va_Scenario_cible_out			,
			@va_Mesure_out				,
			@va_Indicateur_Horaire_out	,
			@va_Transitoire_out			,
			@va_Zone_out		
	return XDC_OK
go



   
