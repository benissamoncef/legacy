/*E*/
/*  Fichier : $Id: xzac88sp.prc,v 1.2 2020/11/26 16:57:10 devgtie Exp $     Release : $Revision: 1.2 $        Date : $Date: 2020/11/26 16:57:10 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac88sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* LCL	02/03/18 : Creation controleurs de feux CTRL_FEUX 1.1 DEM1284
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Recapitule l'utilisation actuelle des CFE pour un evenement.
* 
* Sequence d'appel
* CL	XZAC88_Recap_Info_CFE
* 
* Arguments en entree
* XDY_Horodate	va_Horodate_in
* XDY_Entier	va_NumEvt_in
* XDY_Octet	va_CleEvt_in
* 
* Arguments en sortie
* XDY_Octet	va_NumCFE_out
* XDY_Octet	va_PlanDeFeux_out
* 
* Code retour
* XDC_OK
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* Les informations sont retournees dans une liste
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
* select dans les tables ACTION et E_CFE 
* where evt, heure_succes<Horodate<heure_fin
-------------------------------------------------------*/

use PRC
go

drop procedure XZAC88
go

create procedure XZAC88
	@va_Horodate_in		datetime	= null,
	@va_NumEvt_in		int		= null,
	@va_CleEvt_in		tinyint		= null
as
	declare @vl_Status int
	
	/*! transfo en param�tres nulls bug applix */
	if @va_NumEvt_in = 0 select @va_NumEvt_in = null
	if @va_CleEvt_in = 0 select @va_CleEvt_in = null
	if @va_Horodate_in = XDC_DATE_NULLE select @va_Horodate_in = null

	/*A controle les parametres obligatoires */
	if @va_NumEvt_in = null or @va_CleEvt_in = null
		return XDC_ARG_INV

	/*A date nulle : date courante */

	if @va_Horodate_in=null select @va_Horodate_in=getdate()

	/*A rechercher les CFE utilises par cet evenement et les messages */
#ifdef CI
	/*! cas ou la procedure est creee au CI */

	/*A si la date donnee est anterieure d'un mois au moins */
 	/*A rechercher l'evenement dans la base HIS sinon dans  */
	/*A la base EXP (au niveau du CI)                       */
	if @va_Horodate_in < (select dateadd(month, -1, getdate ()))
		begin
			exec @vl_Status = SQL_HIS.PRC..XZAC88
				@va_Horodate_in,@va_NumEvt_in,@va_CleEvt_in
			if @vl_Status = null
                        	return XDC_PRC_INC
                	else return @vl_Status
                end
	else
		select
			EXP..ACT_GEN.equipement,
			EXP..ACT_CFE.plan_de_feux,
			EXP..ACT_CFE.libelle_plan
		from EXP..ACT_GEN, EXP..ACT_CFE
		where	EXP..ACT_GEN.evenement = @va_NumEvt_in and 
			EXP..ACT_GEN.cle = @va_CleEvt_in and
			EXP..ACT_GEN.type = XDC_ACT_CFE and
			EXP..ACT_GEN.heure_succes <= @va_Horodate_in and
			(EXP..ACT_GEN.heure_fin is null or
			EXP..ACT_GEN.heure_fin >= @va_Horodate_in )  and
			EXP..ACT_CFE.actio = EXP..ACT_GEN.numero and
			EXP..ACT_CFE.sit = EXP..ACT_GEN.sit
#else
#ifdef HIST 	
		select
			HIS..ACT_GEN.equipement,
			HIS..ACT_CFE.plan_de_feux,
			HIS..ACT_CFE.libelle_plan
		from HIS..ACT_GEN, HIS..ACT_CFE
		where	HIS..ACT_GEN.evenement = @va_NumEvt_in and 
			HIS..ACT_GEN.cle = @va_CleEvt_in and
			HIS..ACT_GEN.type = XDC_ACT_CFE and
			HIS..ACT_GEN.heure_succes <= @va_Horodate_in and
			(HIS..ACT_GEN.heure_fin is null or
			HIS..ACT_GEN.heure_fin >= @va_Horodate_in )  and
			HIS..ACT_CFE.actio = HIS..ACT_GEN.numero and
			HIS..ACT_CFE.sit = HIS..ACT_GEN.sit

#else
	/*! cas ou la procedure est creee sur le serveur d'un district */

	select
		EXP..ACT_GEN.equipement,
		EXP..ACT_CFE.plan_de_feux,
		EXP..ACT_CFE.libelle_plan
	from EXP..ACT_GEN, EXP..ACT_CFE
	where	EXP..ACT_GEN.evenement = @va_NumEvt_in and 
		EXP..ACT_GEN.cle = @va_CleEvt_in and
		EXP..ACT_GEN.type = XDC_ACT_CFE and
		EXP..ACT_GEN.heure_succes <= @va_Horodate_in and
		(EXP..ACT_GEN.heure_fin is null or
		EXP..ACT_GEN.heure_fin >= @va_Horodate_in )  and
		EXP..ACT_CFE.actio = EXP..ACT_GEN.numero and
		EXP..ACT_CFE.sit = EXP..ACT_GEN.sit
#endif		
#endif		

	return XDC_OK
go
