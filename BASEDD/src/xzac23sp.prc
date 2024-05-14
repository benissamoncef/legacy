/*E*/
/*  Fichier : $Id: xzac23sp.prc,v 1.5 1998/09/22 13:22:03 gaborit Exp $      Release : $Revision: 1.5 $        Date : $Date: 1998/09/22 13:22:03 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac23sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	10/11/94	: Creation a partir de asrv_c23sp.prc
*                         ( V 1.1 )
* C.T.	14/11/94	: Modif recherche infos pour CI (V 1.2)
* B.G.	01/12/94	: ajout date courante si null (V 1.3)
* C.T.	04/10/96	: Rechercher les partenaires informes depuis
*                         le dernier retypage de fmc (FMC3) (V 1.4)
* C.P.	08/06/98	: Modif separation EXP et HIS (V 1.5)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Recherche les partenaires precedemment informes par 
* fax ou un appel telephonique ou bip pour un evenement.
* 
* Sequence d'appel
* CL	XZAC23_Rech_Partenaires_Informes
* 
* Arguments en entree
* XDY_Horodate	va_Horodate_in
* XDY_Entier	va_NumEvt_in
* XDY_Octet	va_CleEvt_in
* XDY_District	va_District_in
* 
* Arguments en sortie
* XDY_Octet	va_TypeAstreinte_out
* 
* Code retour
* XDC_OK
* XDC_NOK	: evenement non trouve
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* le parametre de sortie est retourne dans une liste
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
* select dans les tables ACTION et APPEL_TELEPHONIQUE_BIP_OU_FAX
----------------------------------------------------*/

use PRC
go

create procedure XZAC;23
	@va_Horodate_in		datetime	= null,
	@va_NumEvt_in		int		= null,
	@va_CleEvt_in		tinyint		= null,
	@va_District_in		tinyint		= null
as
	declare @vl_HorodateTypage datetime , @vl_Status int

	/*! transfo en paramÉtres nulls bug applix */
	if @va_NumEvt_in = 0 select @va_NumEvt_in = null
	if @va_CleEvt_in = 0 select @va_CleEvt_in = null
	if @va_District_in = 0 select @va_District_in = null
	if @va_Horodate_in = XDC_DATE_NULLE select @va_Horodate_in = null

	/*A controle des arguments obligatoires */
	if @va_NumEvt_in = null or
	   @va_CleEvt_in = null or @va_District_in = null
		return XDC_ARG_INV

	/*A date nulle : date courante */

	if @va_Horodate_in=null select @va_Horodate_in=getdate()

	/*A rechercher les astreintes appelees */
#ifdef CI
	/*! cas ou la procedure est creee au CI */

	/*A si la date donnee est anterieure d'un mois au moins */
 	/*A rechercher l'evenement dans la base HIS sinon dans  */
	/*A la base EXP (au niveau du CI)                       */
	if @va_Horodate_in < (select dateadd(month, -1, getdate ()))
		begin
			exec @vl_Status = SQL_HIS.PRC..XZAC;23
				@va_Horodate_in,@va_NumEvt_in,@va_CleEvt_in,@va_District_in
			if @vl_Status = null
                        	return XDC_PRC_INC
                	else return @vl_Status
                end
	
	else
	begin
		/*A Rechercher la date de retypage de fmc */
		select
			@vl_HorodateTypage = horodate_chgt_type
		from EXP..FMC_GEN
		where	numero = @va_NumEvt_in and cle = @va_CleEvt_in

		if @vl_HorodateTypage > @va_Horodate_in
			select @vl_HorodateTypage = null

		/*B retourner les astreintes appelees */
		select
			EXP..ACT_APL.type_d_astreinte
		from EXP..ACT_GEN, EXP..ACT_APL
		where	EXP..ACT_GEN.evenement = @va_NumEvt_in and 
			EXP..ACT_GEN.cle = @va_CleEvt_in and
			EXP..ACT_APL.actio = EXP..ACT_GEN.numero and 
			EXP..ACT_GEN.sit = @va_District_in and
			EXP..ACT_APL.sit = @va_District_in  and
			(@vl_HorodateTypage is null or EXP..ACT_GEN.heure_succes >= @vl_HorodateTypage) and
			EXP..ACT_GEN.heure_succes <= @va_Horodate_in
	end

#else
#ifdef HIST
	begin
		/*A Rechercher la date de retypage de fmc */
		select
			@vl_HorodateTypage = horodate_chgt_type
		from HIS..FMC_GEN
		where	numero = @va_NumEvt_in and cle = @va_CleEvt_in

		if @vl_HorodateTypage > @va_Horodate_in
			select @vl_HorodateTypage = null

		/*B retourner les astreintes appelees */
		select
			HIS..ACT_APL.type_d_astreinte
		from HIS..ACT_GEN, HIS..ACT_APL
		where	HIS..ACT_GEN.evenement = @va_NumEvt_in and 
			HIS..ACT_GEN.cle = @va_CleEvt_in and
			HIS..ACT_APL.actio = HIS..ACT_GEN.numero and 
			HIS..ACT_GEN.sit = @va_District_in and
			HIS..ACT_APL.sit = @va_District_in  and
			(@vl_HorodateTypage is null or HIS..ACT_GEN.heure_succes >= @vl_HorodateTypage) and
			HIS..ACT_GEN.heure_succes <= @va_Horodate_in
	end
	
#else
	/*! cas ou la procedure est creee sur le serveur d'un district */

	/*A Rechercher la date de retypage de fmc */
	select
		@vl_HorodateTypage = horodate_chgt_type
	from EXP..FMC_GEN
	where	numero = @va_NumEvt_in and cle = @va_CleEvt_in

	if @vl_HorodateTypage > @va_Horodate_in
		select @vl_HorodateTypage = null

	/*B retourner les astreintes appelees */
	select
		EXP..ACT_APL.type_d_astreinte
	from EXP..ACT_GEN, EXP..ACT_APL
	where	EXP..ACT_GEN.evenement = @va_NumEvt_in and 
		EXP..ACT_GEN.cle = @va_CleEvt_in and
		EXP..ACT_APL.actio = EXP..ACT_GEN.numero and 
		EXP..ACT_GEN.sit = @va_District_in and
		EXP..ACT_APL.sit = @va_District_in  and
		(@vl_HorodateTypage is null or EXP..ACT_GEN.heure_succes >= @vl_HorodateTypage) and
		EXP..ACT_GEN.heure_succes <= @va_Horodate_in
#endif
#endif

	return XDC_OK

go
