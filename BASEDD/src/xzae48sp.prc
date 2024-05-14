/*E*/
/*  Fichier : $Id: xzae48sp.prc,v 1.10 2011/12/06 15:38:35 pc2dpdy Exp $      Release : $Revision: 1.10 $        Date : $Date: 2011/12/06 15:38:35 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae48sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	31/08/94	: Creation
* C.T.	12/08/94	: Modif position param entree
*                         (test ne fonctionne plus)
* C.T.  10/11/94	: Modif bugs applix V 1.4
* C.T.	14/11/94	: Modif recherche des infos au CI (V 1.5)
* C.T.	28/11/94	: Correction erreur de recherche voie (V 1.6)
*                         non teste
* B.G.	17/01/95	: modif date nulle (V 1.7)
* C.P.	17/06/98	: Modif separation EXP et HIS (V 1.8)
* JMG   11/10/08        : parametres en sortie 1.9 DEM/835 
* VR	05/10/11	: Ajout vitesse et vitesse_opposee 2.0 (DEM/1015)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Retourne les champs d'une fiche main courante basculement a l'heure precisee
* 
* Sequence d'appel
* SP	XZAE48_Lire_Fiche_Basculement
* 
* Arguments en entree
* XDY_Horodate		va_Horodate_in : par defaut heure courante 
* XDY_Entier		va_NumEvt_in
* XDY_Octet		va_CleEvt_in
* 
* Arguments en sortie
* XDY_PR		va_PR_out
* XDY_Entier		va_Longueur_out
* XDY_Octet		va_VR_out
* XDY_Octet		va_VM2_out
* XDY_Octet		va_VM1_out
* XDY_Octet		va_VL_out
* XDY_Octet		va_BAU_out
* XDY_Octet		va_VR_I_out
* XDY_Octet		va_VM2_I_out
* XDY_Octet		va_VM1_I_out
* XDY_Octet		va_VL_I_out
* XDY_Octet		va_BAU_I_out
* 
* Code retour
* XDC_OK
* XDC_NOK : pas de complement de fiche main courante
* XDC_ARG_INV
* < 0 erreur sybase
* 
* Conditions d'utilisation
* En formation, preciser l'horodate
* 
* Fonction
* Select dans la table BASCULEMENT
* where numero evenement and horodate
-------------------------------------------------*/

use PRC
go

drop procedure XZAE48
go

create procedure XZAE48
	@va_Horodate_in		datetime = null, -- par defaut heure courante
	@va_NumEvt_in		int = null,
	@va_CleEvt_in		tinyint = null,
	@va_PR_out		T_PR = null	output,
	@va_Longueur_out	int = null 	output,
	@va_VR_out		T_VOIE	= null	output,
	@va_VM2_out		T_VOIE	= null	output,
	@va_VM1_out		T_VOIE	= null	output,
	@va_VL_out		T_VOIE	= null	output,
	@va_BAU_out		T_VOIE	= null	output,
	@va_VR_I_out		T_VOIE	= null	output,
	@va_VM2_I_out		T_VOIE	= null	output,
	@va_VM1_I_out		T_VOIE	= null	output,
	@va_VL_I_out		T_VOIE	= null	output,
	@va_BAU_I_out		T_VOIE	= null	output,
	@va_vitesse_out		tinyint	= null	output,
	@va_vitesse_opposee_out	tinyint = null	output
as
	declare @vl_Autoroute	T_AUTOROUTE,
		@vl_PRDebut	T_PR,
		@vl_Status	int

	/*! transfo en paramÉtres nulls bug applix */
	if @va_NumEvt_in = 0 select @va_NumEvt_in = null
	if @va_CleEvt_in = 0 select @va_CleEvt_in = null
	if @va_Horodate_in = XDC_DATE_NULLE select @va_Horodate_in = null

	/*A test les parametres obligatoires */
	if @va_NumEvt_in = null or @va_CleEvt_in = null
		return XDC_ARG_INV

	/*A date courante a chercher si null */
	if @va_Horodate_in = null
		select  @va_Horodate_in = XDC_DATE_INFINIE

#ifdef HIST
begin
		/*B lecture des infos sur un basculement */
		select
			@va_PR_out	= localisation_PR_fin,
			@va_VR_out	= VR,
			@va_VM2_out	= VM2,
			@va_VM1_out	= VM1,
			@va_VL_out	= VL,
			@va_BAU_out	= BAU,
			@va_VR_I_out	= VR_I,
			@va_VM2_I_out	= VM2_I,
			@va_VM1_I_out	= VM1_I,
			@va_VL_I_out	= VL_I,
			@va_BAU_I_out	= BAU_I,
			@va_vitesse_out	= vitesse,
			@va_vitesse_opposee_out	= vitesse_opposee
		from HIS..FMC_BAS
		where numero = @va_NumEvt_in and cle = @va_CleEvt_in
	
		if @@rowcount = 0
			return XDC_NOK

		/*B recherche du dernier PR de debut et du numero d'autoroute */
		select
			@vl_Autoroute	= autoroute,
			@vl_PRDebut	= PR
		from HIS..FMC_HIS
		where	numero = @va_NumEvt_in and cle = @va_CleEvt_in and
			horodate_validation in 
				(select max(horodate_validation) from HIS..FMC_HIS
				where	numero = @va_NumEvt_in and 
					cle = @va_CleEvt_in and 
					horodate_validation <= @va_Horodate_in )
	end


#else
#ifdef CI
	/*! cas ou la procedure est creee au CI */

	/*A si la date donnee est anterieure d'un mois au moins */
 	/*A rechercher l'evenement dans la base HIS sinon dans  */
	/*A la base EXP (au niveau du CI)                       */
	if @va_Horodate_in < (select dateadd(month, -1, getdate ()))
	begin
		exec @vl_Status = SQL_HIS.PRC..XZAE;48
			@va_Horodate_in	, @va_NumEvt_in	, @va_CleEvt_in	, @va_PR_out output, @va_Longueur_out output,
			@va_VR_out output, @va_VM2_out output, @va_VM1_out output, @va_VL_out output, @va_BAU_out output, @va_VR_I_out output, 
			@va_VM2_I_out output, @va_VM1_I_out output, @va_VL_I_out output, @va_BAU_I_out output	
 		if @vl_Status = null
                       	 return XDC_PRC_INC
                else return @vl_Status
	end
	else
	begin
		/*B lecture des infos sur un basculement */
		select
			@va_PR_out	= localisation_PR_fin,
			@va_VR_out	= VR,
			@va_VM2_out	= VM2,
			@va_VM1_out	= VM1,
			@va_VL_out	= VL,
			@va_BAU_out	= BAU,
			@va_VR_I_out	= VR_I,
			@va_VM2_I_out	= VM2_I,
			@va_VM1_I_out	= VM1_I,
			@va_VL_I_out	= VL_I,
			@va_BAU_I_out	= BAU_I,
			@va_vitesse_out	= vitesse,
			@va_vitesse_opposee_out	= vitesse_opposee
		from EXP..FMC_BAS
		where numero = @va_NumEvt_in and cle = @va_CleEvt_in
	
		if @@rowcount = 0
			return XDC_NOK

		/*B recherche du dernier PR de debut et du numero d'autoroute */
		select
			@vl_Autoroute	= autoroute,
			@vl_PRDebut	= PR
		from EXP..FMC_HIS
		where	numero = @va_NumEvt_in and cle = @va_CleEvt_in and
			horodate_validation in 
				(select max(horodate_validation) from EXP..FMC_HIS
				where	numero = @va_NumEvt_in and 
					cle = @va_CleEvt_in and 
					horodate_validation <= @va_Horodate_in )
	end
#else
	/*! cas ou la procedure est creee sur le serveur d'un district */

	/*A lecture des infos sur un basculement */
	select
		@va_PR_out	= localisation_PR_fin,
		@va_VR_out	= VR,
		@va_VM2_out	= VM2,
		@va_VM1_out	= VM1,
		@va_VL_out	= VL,
		@va_BAU_out	= BAU,
		@va_VR_I_out	= VR_I,
		@va_VM2_I_out	= VM2_I,
		@va_VM1_I_out	= VM1_I,
		@va_VL_I_out	= VL_I,
		@va_BAU_I_out	= BAU_I,
		@va_vitesse_out	= vitesse,
		@va_vitesse_opposee_out	= vitesse_opposee
	from EXP..FMC_BAS
	where numero = @va_NumEvt_in and cle = @va_CleEvt_in

	if @@rowcount = 0
		return XDC_NOK

	/*A recherche du dernier PR de debut et du numero d'autoroute */
	select
		@vl_Autoroute	= autoroute,
		@vl_PRDebut	= PR
	from EXP..FMC_HIS
	where	numero = @va_NumEvt_in and cle = @va_CleEvt_in and
		horodate_validation in 
			(select max(horodate_validation) from EXP..FMC_HIS
			where	numero = @va_NumEvt_in and cle = @va_CleEvt_in and 
				horodate_validation <= @va_Horodate_in )
#endif
#endif

	if @@rowcount = 0
		return XDC_NOK

	/*A calcul de la longueur du basculement             */
	/*! basculement sur une seule autoroute  a confirmer */
	exec @vl_Status = XZAO;22 @vl_Autoroute, @vl_PRDebut, @vl_Autoroute, @va_PR_out, 
				 @va_Longueur_out output

	/*A test si la procedure s'est correctement deroulee */
	if @vl_Status != XDC_OK
		return @vl_Status	/*! voir si on retourne ce status ou XDC_NOK */


	return XDC_OK

go
