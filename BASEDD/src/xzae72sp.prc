/*E*/
/*  Fichier : $Id: xzae72sp.prc,v 1.10 2011/10/18 13:03:44 gesconf Exp $        Release : $Revision: 1.10 $        Date : $Date: 2011/10/18 13:03:44 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae72sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.  20/10/94	: Creation		V 1.1 
* C.T.	10/11/94	: Modif bugs applix	V 1.2
* C.T.	14/11/94	: Modif recherche des infos au CI (V 1.3)
* C.T.	02/12/94	: correction erreur having (V 1.4)
* JMG	05/11/96	: retourner heure de debut prevu (DEM/1281) (1.5)
* JMG	04/12/96	: corriger select dans HIS (DEM/1281) 1.6
* C.P.	17/06/98	: Modif separation EXP et HIS (V 1.7)
* JPL	21/07/10	: Au CI ou sur secteur, si l'evenement est introuvable, executer sur HIS (DEM 941) 1.8
* JPL	01/09/10	: ... et retour compte-rendu OK si l'evenement est trouve 1.9
* JPL	10/10/11	: Degroupee ; ajout retour de l'origine de la FMC (DEM 995)  1.10
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Retourne l'intitule d'une fiche main courante 
* 
* Sequence d'appel
* SP	XZAE72_Lire_Intitule_FMC
* 
* Arguments en entree
* XDY_Entier	va_NumEvt_in
* XDY_Octet	va_CleEvt_in
* 
* Arguments en sortie
* XDY_Mot	va_Type_out
* XDY_Horodate	va_HoroDebut_out
* XDY_Autoroute	va_NumAutoroute_out
* XDC_PR	va_PR_out
* XDC_Sens	va_Sens_out
* XDC_Octet	va_PtCaracteristique_out
* XDC_Octet	va_TypePtCaracteristique_out
* XDC_Octet	va_Position_out
* XDY_Horodate	va_HoroDebutPrevu_out 
* XDY_Nom	va_Origine_out
*
* Code retour
* XDC_OK	
* XDC_NOK	evenement pas trouve
* 
* Conditions d'utilisation
* La procedure stockee au CI recherche les informations 
* dans la base HIS si les infos sur l'evenement ne sont pas trouvees dans
* la base EXP tandis qu'au niveau des districts la recherche s'effectue 
* uniquement dans la base EXP.
* (toute modif dans la recherche d'infos sur un evenement doit etre 
*  effectuee pour le CI et les districts)
* 
* 
* Fonction
* recherche la derniere localisation dans la table FMC_HIS
* 
-------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAE72' and type = 'P')
	drop procedure XZAE72
go


create procedure XZAE72
	@va_NumEvt_in		int		= null,
	@va_CleEvt_in		tinyint		= null,
	@va_Type_out		smallint	= null	output,
	@va_HoroDebut_out	datetime	= null	output,
	@va_NumAutoroute_out	T_AUTOROUTE	= null	output,
	@va_PR_out		T_PR		= null	output,
	@va_Sens_out		T_SENS		= null	output,
	@va_PtCaracteristique_out	tinyint	= null	output,
	@va_TypePtCaracteristique_out	tinyint	= null	output,
	@va_Position_out	tinyint		= null	output,
	@va_HoroDebutPrevu_out	datetime	= null	output,
	@va_Origine_out		T_NOM		= null	output
as
	declare	@vl_NbEnrg	int,
		@vl_Status	int

	/*! transfo en param…tres nulls bug applix */
	if @va_NumEvt_in = 0 select @va_NumEvt_in = null
	if @va_CleEvt_in = 0 select @va_CleEvt_in = null

	/*A Controle les parametres obligatoires en entree */
	if @va_NumEvt_in = null or @va_CleEvt_in = null 
		return XDC_ARG_INV

	/*A rechercher la derniere localisation de l'evenement */

#ifdef HIST

	select
		@va_Type_out		= HIS..FMC_GEN.type,
		@va_HoroDebut_out	= HIS..FMC_GEN.debut,
		@va_NumAutoroute_out	= HIS..FMC_HIS.autoroute,
		@va_PR_out		= HIS..FMC_HIS.PR,
		@va_Sens_out		= HIS..FMC_HIS.sens,
		@va_PtCaracteristique_out= HIS..FMC_HIS.numero_point_caracteristique,
		@va_TypePtCaracteristique_out= HIS..FMC_HIS.point_caracteristique,
		@va_Position_out	= HIS..FMC_HIS.position_sur_aire_ou_echangeur,
		@va_HoroDebutPrevu_out  = HIS..FMC_GEN.debut_prevu,
		@va_Origine_out		= HIS..FMC_GEN.origine_creation
	from HIS..FMC_HIS, HIS..FMC_GEN
	where	HIS..FMC_HIS.numero = @va_NumEvt_in and 
		HIS..FMC_HIS.cle = @va_CleEvt_in and
		HIS..FMC_GEN.numero = @va_NumEvt_in and 
		HIS..FMC_GEN.cle = @va_CleEvt_in  and
		HIS..FMC_HIS.horodate_validation in ( select max(horodate_validation)
							from HIS..FMC_HIS
							where	numero = @va_NumEvt_in and
								cle = @va_CleEvt_in )
	select @vl_NbEnrg = @@rowcount

#else

	select
		@va_Type_out		= EXP..FMC_GEN.type,
		@va_HoroDebut_out	= EXP..FMC_GEN.debut,
		@va_NumAutoroute_out	= EXP..FMC_HIS.autoroute,
		@va_PR_out		= EXP..FMC_HIS.PR,
		@va_Sens_out		= EXP..FMC_HIS.sens,
		@va_PtCaracteristique_out= EXP..FMC_HIS.numero_point_caracteristique,
		@va_TypePtCaracteristique_out= EXP..FMC_HIS.point_caracteristique,
		@va_Position_out	= EXP..FMC_HIS.position_sur_aire_ou_echangeur,
		@va_HoroDebutPrevu_out	= EXP..FMC_GEN.debut_prevu,
		@va_Origine_out		= EXP..FMC_GEN.origine_creation
	from EXP..FMC_HIS, EXP..FMC_GEN
	where	EXP..FMC_HIS.numero = @va_NumEvt_in and 
		EXP..FMC_HIS.cle = @va_CleEvt_in and
		EXP..FMC_GEN.numero = @va_NumEvt_in and 
		EXP..FMC_GEN.cle = @va_CleEvt_in  and
		EXP..FMC_HIS.horodate_validation in ( select max(horodate_validation)
							from EXP..FMC_HIS
							where numero = @va_NumEvt_in and
								cle = @va_CleEvt_in )
	select @vl_NbEnrg = @@rowcount

	/*A si les infos ne sont pas trouvees     */
	/*A alors les rechercher dans la base HIS */
	if @vl_NbEnrg = 0
	begin
		exec @vl_Status = SQL_HIS.PRC..XZAE72
					@va_NumEvt_in,
					@va_CleEvt_in,
					@va_Type_out			output,
					@va_HoroDebut_out		output,
					@va_NumAutoroute_out		output,
					@va_PR_out			output,
					@va_Sens_out			output,
					@va_PtCaracteristique_out	output,
					@va_TypePtCaracteristique_out	output,
					@va_Position_out		output,
					@va_HoroDebutPrevu_out		output,
					@va_Origine_out			output
		if @vl_Status = null
			return XDC_PRC_INC
		else return @vl_Status
	end

#endif

	if @vl_NbEnrg != 1
		return XDC_NOK

	return XDC_OK
go
