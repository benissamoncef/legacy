/*E*/
/* Fichier : $Id: xzac1000sp.prc,v 1.1 2012/07/04 11:22:06 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2012/07/04 11:22:06 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac1000sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* VR	02/03/12 : Creation (DEM/1016)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Recapitule l'utilisation actuelle des PRV pour un evenement.
* 
* Sequence d'appel
* CL	XZAC1000_Recap_Info_PRV
* 
* Arguments en entree
* XDY_Horodate	va_Horodate_in
* XDY_Entier	va_NumEvt_in
* XDY_Octet	va_CleEvt_in
* 
* Arguments en sortie
* XDY_Octet	va_NumPRV_out
* char(45)	va_Message_out
* char(45)	va_Alternat_out
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
* select dans les tables ACTION et E_PMV 
* where evt, heure_succes<Horodate<heure_fin
-------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAC1000' and type = 'P')
	drop procedure XZAC1000
go

create procedure XZAC1000
	@va_Horodate_in		datetime	= null,
	@va_NumEvt_in		int		= null,
	@va_CleEvt_in		tinyint		= null
as
	declare @vl_Status int
	
	/*! transfo en parametres nulls bug applix */
	if @va_NumEvt_in = 0 select @va_NumEvt_in = null
	if @va_CleEvt_in = 0 select @va_CleEvt_in = null
	if @va_Horodate_in = XDC_DATE_NULLE select @va_Horodate_in = null

	/*A controle les parametres obligatoires */
	if @va_NumEvt_in = null or @va_CleEvt_in = null
		return XDC_ARG_INV

	/*A date nulle : date courante */

	if @va_Horodate_in=null select @va_Horodate_in=getdate()

	/*A rechercher les PRV utilises par cet evenement et les messages */
#ifdef CI
	/*! cas ou la procedure est creee au CI */

	/*A si la date donnee est anterieure d'un mois au moins */
 	/*A rechercher l'evenement dans la base HIS sinon dans  */
	/*A la base EXP (au niveau du CI)                       */
	if @va_Horodate_in < (select dateadd(month, -1, getdate ()))
		begin
			exec @vl_Status = SQL_HIS.PRC..XZAC1000
				@va_Horodate_in,@va_NumEvt_in,@va_CleEvt_in
			if @vl_Status = null
                        	return XDC_PRC_INC
                	else return @vl_Status
                end
	else
		select
			EXP..ACT_GEN.equipement,
			EXP..ACT_PRV.bandeau, EXP..ACT_PRV.vitesse 
		from EXP..ACT_GEN, EXP..ACT_PRV
		where	EXP..ACT_GEN.evenement = @va_NumEvt_in and 
			EXP..ACT_GEN.cle = @va_CleEvt_in and
			EXP..ACT_GEN.type = XDC_ACT_PRV and
			EXP..ACT_GEN.heure_succes <= @va_Horodate_in and
			(EXP..ACT_GEN.heure_fin is null or
			EXP..ACT_GEN.heure_fin >= @va_Horodate_in )  and
			EXP..ACT_PRV.actio = EXP..ACT_GEN.numero and
			EXP..ACT_PRV.sit = EXP..ACT_GEN.sit
#else
#ifdef HIST 	
		select
			HIS..ACT_GEN.equipement,
			HIS..ACT_PRV.bandeau, HIS..ACT_PRV.vitesse 
		from HIS..ACT_GEN, HIS..ACT_PRV
		where	HIS..ACT_GEN.evenement = @va_NumEvt_in and 
			HIS..ACT_GEN.cle = @va_CleEvt_in and
			HIS..ACT_GEN.type = XDC_ACT_PRV and
			HIS..ACT_GEN.heure_succes <= @va_Horodate_in and
			(HIS..ACT_GEN.heure_fin is null or
			HIS..ACT_GEN.heure_fin >= @va_Horodate_in )  and
			HIS..ACT_PRV.actio = HIS..ACT_GEN.numero and
			HIS..ACT_PRV.sit = HIS..ACT_GEN.sit

#else
	/*! cas ou la procedure est creee sur le serveur d'un district */

	select
		EXP..ACT_GEN.equipement,
			EXP..ACT_PRV.bandeau, EXP..ACT_PRV.vitesse 
	from EXP..ACT_GEN, EXP..ACT_PRV
	where	EXP..ACT_GEN.evenement = @va_NumEvt_in and 
		EXP..ACT_GEN.cle = @va_CleEvt_in and
		EXP..ACT_GEN.type = XDC_ACT_PRV and
		EXP..ACT_GEN.heure_succes <= @va_Horodate_in and
		(EXP..ACT_GEN.heure_fin is null or
		EXP..ACT_GEN.heure_fin >= @va_Horodate_in )  and
		EXP..ACT_PRV.actio = EXP..ACT_GEN.numero and
		EXP..ACT_PRV.sit = EXP..ACT_GEN.sit
#endif		
#endif		

	return XDC_OK
go
