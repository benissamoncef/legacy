/*E*/
/*  Fichier : $Id: xzac22sp.prc,v 1.4 1998/09/22 13:22:02 gaborit Exp $      Release : $Revision: 1.4 $        Date : $Date: 1998/09/22 13:22:02 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac22sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	10/11/94	: Creation a partir de asrv_c22sp.prc
*                         ( V 1.1 )
* C.T.	14/11/94	: Modif recherche infos pour CI (V 1.2)
* B.G.	01/12/94	: ajout date courante si null (V 1.3)
* C.P.	08/06/98	: Modif separation EXP et HIS (V 1.4)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Recherche les intervenants presents sur les lieux d'un evenement 
* (secours, SIS, depanneur)
* 
* Sequence d'appel
* CL	XZAC22_Rech_Intervenants_Sur_Place
* 
* Arguments en entree
* XDY_Horodate	va_Horodate_in
* XDY_Entier	va_NumEvt_in
* XDY_Octet	va_CleEvt_in
* 
* Arguments en sortie
* XDY_Booleen	va_SIS_out
* XDY_Booleen	va_Secours_out
* XDY_Booleen	va_Depanneur_out
* 
* Code retour
* XDC_OK
* XDC_NOK	: evenement non trouve
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
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
* select dans ACTION where type=XDC_ACT_PAT_SIS,
* XDC_ACT_INTERVENTION ou XDC_ACT_DEPANNEUR et verifiant
* heure_succes < Horodate < heure_fin
---------------------------------------------------------*/

use PRC
go

create procedure XZAC;22
	@va_Horodate_in		datetime	= null,
	@va_NumEvt_in		int		= null,
	@va_CleEvt_in		tinyint		= null,
	@va_SIS_out		bit		output,
	@va_Secours_out		bit		output,
	@va_Depanneur_out	bit		output
as
	declare @vl_numero int,@vl_Status int	

	/*A initialisation des parametres de sortie par defaut */
	select	@va_SIS_out = XDC_NON, 
		@va_Secours_out = XDC_NON, 
		@va_Depanneur_out = XDC_NON

	/*! transfo en paramÉtres nulls bug applix */
	if @va_NumEvt_in = 0 select @va_NumEvt_in = null
	if @va_CleEvt_in = 0 select @va_CleEvt_in = null
	if @va_Horodate_in = XDC_DATE_NULLE select @va_Horodate_in = null

	/*A controle les parametres obligatoires */
	if @va_NumEvt_in = null or @va_CleEvt_in = null
		return XDC_ARG_INV

	/*A date nulle : date courante */

	if @va_Horodate_in=null select @va_Horodate_in=getdate()

#ifdef CI
	/*! cas ou la procedure est creee au CI */

	/*A si la date donnee est anterieure d'un mois au moins */
 	/*A rechercher l'evenement dans la base HIS sinon dans  */
	/*A la base EXP (au niveau du CI)                       */
	if @va_Horodate_in < (select dateadd(month, -1, getdate ()))
		begin
			exec @vl_Status = SQL_HIS.PRC..XZAC;22
				@va_Horodate_in,@va_NumEvt_in,@va_CleEvt_in,@va_SIS_out output,@va_Secours_out output,@va_Depanneur_out output
			if @vl_Status = null
                        	return XDC_PRC_INC
                	else return @vl_Status
                end

	else
		begin
			/*B recherche des actions de type SIS */
			select @vl_numero = numero
			from EXP..ACT_GEN
			where	type = XDC_ACT_PAT_SIS and
				heure_succes <= @va_Horodate_in and
				(heure_fin is null or
				heure_fin >= @va_Horodate_in ) and
				evenement = @va_NumEvt_in and cle = @va_CleEvt_in
	
			if @@rowcount != 0
				select @va_SIS_out = XDC_OUI
	
			/*B recherche des actions de type intervention */
			select @vl_numero = numero
			from EXP..ACT_GEN
			where	type = XDC_ACT_INTERVENTION and
				heure_succes <= @va_Horodate_in and
				(heure_fin is null or
				heure_fin >= @va_Horodate_in ) and
				evenement = @va_NumEvt_in and cle = @va_CleEvt_in
	
			if @@rowcount != 0
				select @va_Secours_out = XDC_OUI
	
			/*B recherche des actions de type depanneur */
			select @vl_numero = numero
			from EXP..ACT_GEN
			where	type = XDC_ACT_DEPANNEUR and 
				heure_succes <= @va_Horodate_in and
				(heure_fin is null or
				heure_fin >= @va_Horodate_in ) and
				evenement = @va_NumEvt_in and cle = @va_CleEvt_in
		end
#else
#ifdef HIST
	begin
		/*B recherche des actions de type SIS */
		select @vl_numero = numero
		from HIS..ACT_GEN
		where	type = XDC_ACT_PAT_SIS and
			heure_succes <= @va_Horodate_in and
			(heure_fin is null or
			heure_fin >= @va_Horodate_in ) and
			evenement = @va_NumEvt_in and cle = @va_CleEvt_in
	
		if @@rowcount != 0
			select @va_SIS_out = XDC_OUI
	
		/*B recherche des actions de type intervention */
		select @vl_numero = numero
		from HIS..ACT_GEN
		where	type = XDC_ACT_INTERVENTION and
			heure_succes <= @va_Horodate_in and
			(heure_fin is null or
			heure_fin >= @va_Horodate_in ) and
			evenement = @va_NumEvt_in and cle = @va_CleEvt_in
	
		if @@rowcount != 0
			select @va_Secours_out = XDC_OUI
	
		/*B recherche des actions de type depanneur */
		select @vl_numero = numero
		from HIS..ACT_GEN
		where	type = XDC_ACT_DEPANNEUR and 
			heure_succes <= @va_Horodate_in and
			(heure_fin is null or
			heure_fin >= @va_Horodate_in ) and
			evenement = @va_NumEvt_in and cle = @va_CleEvt_in
	end
#else
	/*! cas ou la procedure est creee sur le serveur d'un district */

	/*A recherche des actions de type SIS */
	select @vl_numero = numero
	from EXP..ACT_GEN
	where	type = XDC_ACT_PAT_SIS and
		heure_succes <= @va_Horodate_in and
		(heure_fin is null or
		heure_fin >= @va_Horodate_in ) and
		evenement = @va_NumEvt_in and cle = @va_CleEvt_in

	if @@rowcount != 0
		select @va_SIS_out = XDC_OUI

	/*A recherche des actions de type intervention */
	select @vl_numero = numero
	from EXP..ACT_GEN
	where	type = XDC_ACT_INTERVENTION and
		heure_succes <= @va_Horodate_in and
		(heure_fin is null or
		heure_fin >= @va_Horodate_in ) and
		evenement = @va_NumEvt_in and cle = @va_CleEvt_in

	if @@rowcount != 0
		select @va_Secours_out = XDC_OUI

	/*A recherche des actions de type depanneur */
	select @vl_numero = numero
	from EXP..ACT_GEN
	where	type = XDC_ACT_DEPANNEUR and 
		heure_succes <= @va_Horodate_in and
		(heure_fin is null or
		heure_fin >= @va_Horodate_in ) and
		evenement = @va_NumEvt_in and cle = @va_CleEvt_in
#endif
#endif

	if @@rowcount != 0
		select @va_Depanneur_out = XDC_OUI

	return XDC_OK

go
