/*E*/
/*  Fichier : $Id: xzat08sp.prc,v 1.8 2013/10/15 09:10:05 gesconf Exp $      Release : $Revision: 1.8 $        Date : $Date: 2013/10/15 09:10:05 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzat08sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des etats d'un equipement 
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	26/09/94	: Creation
* C.T.	10/11/94	: Modif bugs applix V 1.3
* C.T.  15/11/94          changement du nom de fichier V 1.1
* C.T.	15/11/94	: Modif recherche des infos au CI (V 1.2)
*                         et ajout du numero d'action
* C.T.	29/11/94	: Modif interface fonction ajout site action (V 1.3)
*                         non teste
* B.G.	01/12/94	: ajout date courante si null (V 1.4)
* C.T.	07/02/95	: Modif clause select sur heure_fin exclue (V 1.5)
* C.P.	23/06/98	: Modif separation EXP et HIS (V 1.6)
* P.N.	15/10/98	: Correction etat dispo du PMV au CI (1.7)
* PNI	11/10/13	: SAPIENS AU CI : on considÈre l'utilisation de l'eqt ‡ partir de l'heure_lancement ou heure_succes DEM 1074 1.8
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Retourne l'utilisation reelle d'un ≈changeur » une horodate donn≈e.
* 
* S≈quence d'appel
* CL	XZAT08_Utilisation_Echangeur
* 
* Arguments en entr≈e
* XDY_Horodate	va_Horodate_in : date comprise entre le succes et la 
*                                fin de l'action correspondante
* XDY_Eqt	va_NumEqt_in
* 
* Arguments en sortie
* XDY_Octet	va_DispoEchangeur_out
* XDY_Octet	va_Sequence_out
* XDY_Evt	va_NumEvt_out
* XDY_Evt	va_CleEvt_out
* XDY_Octet	va_Priorite_out
* XDY_Entier	va_DistanceEvt_out
* XDY_Entier	va_Action_out
* XDY_Site	va_SiteAction_out
* 
* Code retour
* XDC_OK
* XDC_NOK
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* La procedure stockee au CI recherche les informations 
* dans la base HIS si la date donnee est anterieure de plus d'un 
* mois tandis qu'au niveau des districts la recherche s'effectue 
* dans la base EXP.
* Le code de recherche des infos de la procedure au CI et aux districts 
* doit etre identique a l'exception de la base de recherche des infos
* et du test de la date en entree (< 1 mois)
* (toute modif dans la creation de la procedure doit etre 
*  effectuee pour le CI et les districts)
* 
* 
* Fonction
* select dans les tables EQUIPEMENT, AFFICHAGE_ECHANGEUR, 
* ACTION et INDISPONIBILITE
------------------------------------------------------------*/

use PRC
go

create procedure XZAT;08
	@va_Horodate_in			datetime	= null,
	@va_NumEqt_in			T_EQUIPEMENT	= null,
	@va_DispoEchangeur_out		tinyint		= null		output,
	@va_Sequence_out		tinyint		= null		output,
	@va_NumEvt_out			int		= null		output,
	@va_CleEvt_out			tinyint		= null		output,
	@va_Priorite_out		tinyint		= null		output,
	@va_DistanceEvt_out		int		= null		output,
	@va_Action_out			int		= null		output,
	@va_SiteAction_out		T_SITE		= null		output
as
	declare @vl_HS bit, @vl_Critique bit, @vl_Majeure bit, @vl_Inhibe bit,
		@vl_Desactive bit, @vl_Mineure bit, @vl_NbEnregistrements int
		, @vl_Status int

	/*! transformations bugs applix */
	if @va_NumEqt_in = 0 select @va_NumEqt_in = null
	if @va_Horodate_in = XDC_DATE_NULLE select @va_Horodate_in = null


	/*A test les parametres obligatoires */
	if @va_NumEqt_in = null
		return XDC_ARG_INV

	/*A date nulle : date courante */

	if @va_Horodate_in=null select @va_Horodate_in=getdate()

#ifdef HIST

	/*! cas ou la procedure est creee au HIS */

		/*B lecture des infos sur le tunnel */
		select 
			@va_Sequence_out	= HIS..ACT_ECH.sequence,
			@va_NumEvt_out		= HIS..ACT_GEN.evenement,
			@va_CleEvt_out		= HIS..ACT_GEN.cle,
			@va_Priorite_out	= HIS..ACT_GEN.priorite,
			@va_DistanceEvt_out	= HIS..ACT_GEN.distance_evt,
			@va_Action_out		= HIS..ACT_GEN.numero,
			@va_SiteAction_out	= HIS..ACT_GEN.sit
		from HIS..ACT_ECH, HIS..ACT_GEN
		where	HIS..ACT_GEN.equipement = @va_NumEqt_in and 
			HIS..ACT_GEN.type = XDC_ACT_ECHANGEUR and
			HIS..ACT_ECH.actio = HIS..ACT_GEN.numero and 
			HIS..ACT_ECH.sit = HIS..ACT_GEN.sit and
			HIS..ACT_GEN.heure_succes <= @va_Horodate_in and
			 HIS..ACT_GEN.heure_fin > @va_Horodate_in
	
		select @vl_NbEnregistrements = @@rowcount
		if @vl_NbEnregistrements = 0
		begin
		select 
			@va_Sequence_out	= HIS..ACT_ECH.sequence,
			@va_NumEvt_out		= HIS..ACT_GEN.evenement,
			@va_CleEvt_out		= HIS..ACT_GEN.cle,
			@va_Priorite_out	= HIS..ACT_GEN.priorite,
			@va_DistanceEvt_out	= HIS..ACT_GEN.distance_evt,
			@va_Action_out		= HIS..ACT_GEN.numero,
			@va_SiteAction_out	= HIS..ACT_GEN.sit
		from HIS..ACT_ECH, HIS..ACT_GEN
		where	HIS..ACT_GEN.equipement = @va_NumEqt_in and 
			HIS..ACT_GEN.type = XDC_ACT_ECHANGEUR and
			HIS..ACT_ECH.actio = HIS..ACT_GEN.numero and 
			HIS..ACT_ECH.sit = HIS..ACT_GEN.sit and
			HIS..ACT_GEN.heure_succes <= @va_Horodate_in and
			 HIS..ACT_GEN.heure_fin is null
		end
	
		if @vl_NbEnregistrements > 1
			/*! erreur dans la base */
			return XDC_NOK
		else if @vl_NbEnregistrements = 0
			/*! si l'echangeur n'est pas utilise alors positionner a null */
			/*! evt sinon probleme de conservation de la valeur de l'     */
			/*! appel precedent  */
			select	@va_NumEvt_out 		= null, 
				@va_CleEvt_out 		= null,
				@va_Sequence_out	= null
	
	
		/*B lecture des disponibilite de l'equipement */
		select
			@vl_Desactive = desactive,
			@vl_HS = HS,
			@vl_Majeure = majeure,
			@vl_Mineure = mineure,
			@vl_Critique = critique,
			@vl_Inhibe = inhibe
		from HIS..EQT_DSP
		where equipement = @va_NumEqt_in and type = XDC_EQT_ECH and 
			horodate in (	select max(horodate) from HIS..EQT_DSP
					where equipement = @va_NumEqt_in and 
						type = XDC_EQT_ECH and
						horodate <= @va_Horodate_in )
	
#else
#ifdef CI
	/*! cas ou la procedure est creee au CI */

	/*A si la date donnee est anterieure d'un mois au moins */
 	/*A rechercher l'evenement dans la base HIS sinon dans  */
	if @va_Horodate_in < (select dateadd(month, -1, getdate ()))
	begin	
		exec @vl_Status = SQL_HIS.PRC..XZAT;08
			@va_Horodate_in, @va_NumEqt_in,	
			@va_DispoEchangeur_out	output, @va_Sequence_out	output,
			@va_NumEvt_out 		output, @va_CleEvt_out		output,
			@va_Priorite_out 	output, @va_DistanceEvt_out	output,
			@va_Action_out		output, @va_SiteAction_out	output
      		if @vl_Status = null
                        return XDC_PRC_INC
            	else return @vl_Status
	end
	else
	begin
		/*B lecture des infos sur le tunnel */
		select 
			@va_Sequence_out	= EXP..ACT_ECH.sequence,
			@va_NumEvt_out		= EXP..ACT_GEN.evenement,
			@va_CleEvt_out		= EXP..ACT_GEN.cle,
			@va_Priorite_out	= EXP..ACT_GEN.priorite,
			@va_DistanceEvt_out	= EXP..ACT_GEN.distance_evt,
			@va_Action_out		= EXP..ACT_GEN.numero,
			@va_SiteAction_out	= EXP..ACT_GEN.sit
		from EXP..ACT_ECH, EXP..ACT_GEN
		where	EXP..ACT_GEN.equipement = @va_NumEqt_in and 
			EXP..ACT_GEN.type = XDC_ACT_ECHANGEUR and
			EXP..ACT_ECH.actio = EXP..ACT_GEN.numero and 
			EXP..ACT_ECH.sit = EXP..ACT_GEN.sit and
			(EXP..ACT_GEN.heure_succes <= @va_Horodate_in OR EXP..ACT_GEN.heure_lancement <= @va_Horodate_in) and
			(EXP..ACT_GEN.heure_fin is null or 
			 EXP..ACT_GEN.heure_fin > @va_Horodate_in)
	
		select @vl_NbEnregistrements = @@rowcount
	
		if @vl_NbEnregistrements > 1
			/*! erreur dans la base */
			return XDC_NOK
		else if @vl_NbEnregistrements = 0
			/*! si l'echangeur n'est pas utilise alors positionner a null */
			/*! evt sinon probleme de conservation de la valeur de l'     */
			/*! appel precedent  */
			select	@va_NumEvt_out 		= null, 
				@va_CleEvt_out 		= null,
				@va_Sequence_out	= null
	
	
		/*B lecture des disponibilite de l'equipement */
		select
			@vl_Desactive = desactive,
			@vl_HS = HS,
			@vl_Majeure = majeure,
			@vl_Mineure = mineure,
			@vl_Critique = critique,
			@vl_Inhibe = inhibe
		from EXP..EQT_DSP
		where equipement = @va_NumEqt_in and type = XDC_EQT_ECH and 
			horodate in (	select max(horodate) from EXP..EQT_DSP
					where equipement = @va_NumEqt_in and 
						type = XDC_EQT_ECH and
						horodate <= @va_Horodate_in )
	end
#else
	/*! cas ou la procedure est creee sur le serveur d'un district */

	/*A lecture des infos sur le tunnel */
	select 
		@va_Sequence_out	= EXP..ACT_ECH.sequence,
		@va_NumEvt_out		= EXP..ACT_GEN.evenement,
		@va_CleEvt_out		= EXP..ACT_GEN.cle,
		@va_Priorite_out	= EXP..ACT_GEN.priorite,
		@va_DistanceEvt_out	= EXP..ACT_GEN.distance_evt,
		@va_Action_out		= EXP..ACT_GEN.numero,
		@va_SiteAction_out	= EXP..ACT_GEN.sit
	from EXP..ACT_ECH, EXP..ACT_GEN
	where	EXP..ACT_GEN.equipement = @va_NumEqt_in and 
		EXP..ACT_GEN.type = XDC_ACT_ECHANGEUR and
		EXP..ACT_ECH.actio = EXP..ACT_GEN.numero and 
		EXP..ACT_ECH.sit = EXP..ACT_GEN.sit and
		EXP..ACT_GEN.heure_succes <= @va_Horodate_in and
		(EXP..ACT_GEN.heure_fin is null or 
		 EXP..ACT_GEN.heure_fin > @va_Horodate_in)

	select @vl_NbEnregistrements = @@rowcount

	if @vl_NbEnregistrements > 1
		/*! erreur dans la base */
		return XDC_NOK
	else if @vl_NbEnregistrements = 0
		/*! si l'echangeur n'est pas utilise alors positionner a null */
		/*! evt sinon probleme de conservation de la valeur de l'     */
		/*! appel precedent  */
		select	@va_NumEvt_out 		= null, 
			@va_CleEvt_out 		= null,
			@va_Sequence_out	= null


	/*A lecture des disponibilite de l'equipement */
	select
		@vl_Desactive = desactive,
		@vl_HS = HS,
		@vl_Majeure = majeure,
		@vl_Mineure = mineure,
		@vl_Critique = critique,
		@vl_Inhibe = inhibe
	from EXP..EQT_DSP
	where equipement = @va_NumEqt_in and type = XDC_EQT_ECH and 
		horodate in (	select max(horodate) from EXP..EQT_DSP
				where equipement = @va_NumEqt_in and 
					type = XDC_EQT_ECH and
					horodate <= @va_Horodate_in )
#endif
#endif

        if @@rowcount != 1
		/*A erreur dans la base */
		return XDC_NOK

	/*A calcul de l'etat de l'equipement */
	select @va_DispoEchangeur_out = @vl_Desactive * XDC_EQT_DESACTIVE + 
			@vl_HS * XDC_EQT_HS +
	   		@vl_Majeure * XDC_EQT_MAJEUR + @vl_Critique * XDC_EQT_CRITIQUE +
	   		@vl_Inhibe * XDC_EQT_INHIBE + @vl_Mineure * XDC_EQT_MINEUR


	return XDC_OK
go
