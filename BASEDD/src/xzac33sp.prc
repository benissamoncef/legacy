/*E*/
/*  Fichier : $Id: xzac33sp.prc,v 1.9 2010/09/02 09:48:13 gesconf Exp $      Release : $Revision: 1.9 $        Date : $Date: 2010/09/02 09:48:13 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac33sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	08/09/94	: Creation
* C.T.	10/11/94	: Modif bugs applix (V 1.3)
* C.T.	14/11/94	: Modif recherche infos pour CI (V 1.4)
* C.T.	09/12/94	: Modif clause de lecture d'action (V 1.5)
*                         non teste
* C.P.	10/06/98	: Separation EXP et HIS (V 1.6)
* JMG	21/09/99	: ajout autoroute 
* JPL	21/07/10	: Au CI ou sur secteur, si l'action est introuvable, executer sur HIS (DEM 941) 1.8
* JPL	01/09/10	: ... et retour compte-rendu OK si l'evenement est trouve 1.9
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Lecture d'une intervention de la patrouille Escota avec PMV sur fourgon
* 
* Sequence d'appel
* SP	XZAC33_Lire_PMV_Sur_Fourgon
* 
* Arguments en entree
* XDY_Entier		va_NumeroAction_in
* XDY_Octet		va_Site_in
* 
* Arguments en sortie
* XY_Horodate		va_HoroDebut_in
* XY_Horodate		va_HoroFin_in
* XDY_PR		va_PR_in
* XDY_Sens		va_Sens_in
* char(20)		va_Message_in
* XDY_Operateur		va_Operateur_in
* XDY_Autoroute		va_autoroute_in
*
* Code retour
* XDC_OK
* XDC_NOK	: actio non trouvee
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* La procedure stockee au CI recherche les informations 
* dans la base HIS si les informations n'existent pas dans
* EXP tandis qu'au niveau des districts la recherche s'effectue 
* dans la base EXP.
* Le code de recherche d'infos de la procedure au CI et aux districts 
* doit etre identique a l'exception de la base de recherche des infos
* (toute modif dans la creation de la procedure doit etre 
*  effectuee pour le CI et les districts)
* 
* 
* Fonction
* Select dans les tables ACTION et PMV_SUR_FOURGON
-----------------------------------------------------*/

use PRC
go

create procedure XZAC;33
	@va_NumeroAction_in	int		= null,
	@va_Site_in		T_SITE		= null,
	@va_HoroDebut_in	datetime	= null	output,
	@va_HoroFin_in		datetime	= null	output,
	@va_PR_in		T_PR		= null	output,
	@va_Sens_in		T_SENS		= null	output,
	@va_Message_in		char(20)	= null	output,
	@va_Operateur_in	T_OPERATEUR	= null	output,
	@va_Autoroute_in	T_AUTOROUTE 	= null  output
as
	declare	@vl_NbEnrg	int,
		@vl_Status	int

	/*! transfo en paramÉtres nulls bug applix */
	if @va_NumeroAction_in = 0 select @va_NumeroAction_in = null
	if @va_Site_in = 0 select @va_Site_in = null

	/*A Controle des parametres d'entree */
	if @va_NumeroAction_in = null or @va_Site_in = null
		return XDC_ARG_INV

	/*A lecture de l'action */

#ifdef HIST

 	/*A rechercher dans la base HIS */
	select
		@va_HoroDebut_in	= HIS..ACT_GEN.heure_lancement,
		@va_HoroFin_in		= HIS..ACT_GEN.heure_fin,
		@va_PR_in		= HIS..ACT_FRG.PR,
		@va_Sens_in		= HIS..ACT_FRG.sens,
		@va_Message_in		= HIS..ACT_FRG.message,
		@va_Operateur_in	= HIS..ACT_GEN.operateur,
		@va_Autoroute_in	= HIS..ACT_FRG.autoroute
	from 	HIS..ACT_GEN, HIS..ACT_FRG
	where 	HIS..ACT_GEN.numero = @va_NumeroAction_in and 
		HIS..ACT_GEN.type = XDC_ACT_PMV_FOURGON and
		HIS..ACT_GEN.sit = @va_Site_in and
		HIS..ACT_FRG.actio = @va_NumeroAction_in and 
		HIS..ACT_FRG.sit = @va_Site_in

	select @vl_NbEnrg = @@rowcount
	
#else

	select
		@va_HoroDebut_in	= EXP..ACT_GEN.heure_lancement,
		@va_HoroFin_in		= EXP..ACT_GEN.heure_fin,
		@va_PR_in		= EXP..ACT_FRG.PR,
		@va_Sens_in		= EXP..ACT_FRG.sens,
		@va_Message_in		= EXP..ACT_FRG.message,
		@va_Operateur_in	= EXP..ACT_GEN.operateur,
		@va_Autoroute_in	= EXP..ACT_FRG.autoroute
	from 	EXP..ACT_GEN, EXP..ACT_FRG
	where 	EXP..ACT_GEN.numero = @va_NumeroAction_in and 
		EXP..ACT_GEN.type = XDC_ACT_PMV_FOURGON and
		EXP..ACT_GEN.sit = @va_Site_in and
		EXP..ACT_FRG.actio = @va_NumeroAction_in and 
		EXP..ACT_FRG.sit = @va_Site_in

	select @vl_NbEnrg = @@rowcount

	/*A si les infos ne sont pas trouvees     */
	/*A alors les rechercher dans la base HIS */
	if @vl_NbEnrg = 0
	begin
		exec @vl_Status = SQL_HIS.PRC..XZAC;33
					@va_NumeroAction_in,
					@va_Site_in,
					@va_HoroDebut_in	output,
					@va_HoroFin_in		output,
					@va_PR_in		output,
					@va_Sens_in		output,
					@va_Message_in		output,
					@va_Operateur_in	output,
					@va_Autoroute_in	output
		if @vl_Status = null
			return XDC_PRC_INC
		else return @vl_Status
	end

#endif

	if @vl_NbEnrg != 1
		return XDC_NOK

	return XDC_OK
go
