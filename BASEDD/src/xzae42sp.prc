/*E*/
/*  Fichier : $Id: xzae42sp.prc,v 1.9 2020/09/14 07:10:10 pc2dpdy Exp $      Release : $Revision: 1.9 $        Date : $Date: 2020/09/14 07:10:10 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae42sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture de fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	30/08/94	: Creation
* C.T.  10/11/94	: Modif bugs applix V 1.4
* C.T.	14/11/94	: Modif recherche des infos au CI (V 1.5)
* C.T.	09/10/95	: Suppression du code d'erreur sur plan de secours manquant (V 1.6)
* C.P.	17/06/98	: Modif separation EXP et HIS (V 1.7)
* JMG	16/09/08	: ajout PR en sortie 1.8 DEM/835
* ABE	11/09/20	: DEM-SAE130 Datex2 Récuperation du code matiere avec le curseur (pb récuperation par asql_ctparam_char_output)	V1.9
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Retourne les champs d'une fiche main courante delestage 
* 
* Sequence d'appel
* SP	XZAE42_Lire_Fiche_Delestage
* 
* Arguments en entree
* XDY_Entier		NumEvt
* XDY_Octet		CleEvt
* XDY_Horodate		Horodate : par defaut heure courante (non utilisee)
* 
* Arguments en sortie
* XDY_Nom		va_NomPlan_out
* XDY_Mot		va_NumPlan_out
* XDY_Nom		va_Hierarchie_out
* 
* Code retour
* XDC_OK
* XDC_NOK     : pas trouve de complement de fiche 
* XDC_ARG_INV : argument en entree null
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
* Select dans la table DELESTAGE
* where numero evenement and horodate
------------------------------------------------------*/

use PRC
go

create procedure XZAE;42
	@va_NumEvt_in		int = null,
	@va_CleEvt_in		tinyint = null,
	@va_Horodate_in		datetime = null, -- par defaut heure courante
	@va_NomPlan_out		T_NOM = null output,
	@va_NumPlan_out		smallint = null output,
	@va_Hierarchie_out	char(25) = null output,
	@va_PR_out		int = null output,
	@va_PR2_out              int = null output
as
	declare @vl_Status int,
		@vl_PR int
	
	/*! transfo en paramÉtres nulls bug applix */
	if @va_NumEvt_in = 0 select @va_NumEvt_in = null
	if @va_CleEvt_in = 0 select @va_CleEvt_in = null
	if @va_Horodate_in = XDC_DATE_NULLE select @va_Horodate_in = null

	/*A Controle les arguments en entree obligatoire */
	if @va_NumEvt_in = null or @va_CleEvt_in = null
		return XDC_ARG_INV

#ifdef HIST
		select
			@va_NumPlan_out		= plan_associe,
			@va_Hierarchie_out	= responsable
		from HIS..FMC_DEL
		where numero = @va_NumEvt_in and cle = @va_CleEvt_in
	
#else
#ifdef CI
	/*! cas ou la procedure est creee au CI */

	/*A si la date donnee est anterieure d'un mois au moins */
 	/*A rechercher l'evenement dans la base HIS sinon dans  */
	/*A la base EXP (au niveau du CI)                       */
	if @va_Horodate_in < (select dateadd(month, -1, getdate ()))
		/*B Lecture des informations de l'evenement */
		begin
			exec @vl_Status = SQL_HIS.PRC..XZAE;42
				@va_NumEvt_in, @va_CleEvt_in, @va_Horodate_in, @va_NomPlan_out output, @va_NumPlan_out output, @va_Hierarchie_out output
 			if @vl_Status = null
                        return XDC_PRC_INC
                	else return @vl_Status
		end
	else
		/*B Lecture des informations de l'evenement */
		select
			@va_NumPlan_out		= plan_associe,
			@va_Hierarchie_out	= responsable
		from EXP..FMC_DEL
		where numero = @va_NumEvt_in and cle = @va_CleEvt_in

#else
	/*! cas ou la procedure est creee sur le serveur d'un district */

	/*A Lecture des informations de l'evenement */
	select
		@va_NumPlan_out		= EXP..FMC_DEL.plan_associe,
		@va_Hierarchie_out	= EXP..FMC_DEL.responsable
	from EXP..FMC_DEL
	where numero = @va_NumEvt_in and cle = @va_CleEvt_in

#endif
#endif

	if @@rowcount = 0
		return XDC_NOK

	/*A recherche du nom du plan de secours */
	if @va_NumPlan_out is not null
		select @va_NomPlan_out = nom from CFG..PLN_SCR where numero = @va_NumPlan_out
		select nom from CFG..PLN_SCR where numero = @va_NumPlan_out

	/*recuperation du PR de reinjection*/
	exec PRC..XZAE161 @va_NumEvt_in,@va_CleEvt_in,
		       @va_PR2_out output,
		      @va_PR_out output

	return XDC_OK
go
