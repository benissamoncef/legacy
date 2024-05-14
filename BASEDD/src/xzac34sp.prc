/*E*/
/*  Fichier : $Id: xzac34sp.prc,v 1.9 2010/08/30 15:58:18 gesconf Exp $      Release : $Revision: 1.9 $        Date : $Date: 2010/08/30 15:58:18 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac34sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	07/09/94	: Creation
* C.T.	13/09/94	: Modif parametre en entree
*                         (HoroLancFin remplace par HoroFin
*                          et HoroSucces supprime)
* C.T.	20/09/94	: Modif taille message (non reteste)
* C.T.	10/11/94	: Modif bugs applix (V 1.3)
* C.T.	14/11/94	: Modif recherche infos pour CI (V 1.4)
* C.T.	09/12/94	: Modif clause de lecture d'action (V 1.5)
*                         non teste
* C.T.	24/01/95	: Modif retour : heure_lancement_fin en 
*                         heure_fin (V 1.6)
* C.P.	10/06/98	: Separation EXP et HIS (V 1.7)
* JMG	21/09/99	: ajout autoroute en sortie 
* JPL	21/07/10	: Au CI ou sur secteur, si l'action est introuvable, executer sur HIS (DEM 941) 1.9
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* SP	XZAC34_Lire_PMV_Mobile
* 
* Arguments en entree
* XDY_Entier		va_NumAction_in
* XDY_Octet		va_Site_in
* 
* Arguments en sortie
* XDY_PR		va_PR_out
* XDY_Sens		va_Sens_out
* char(48)		va_Message_out
* XY_Horodate		va_HoroDebut_out
* XY_Horodate		va_HoroFin_out
* XDY_Operateur		va_Operateur_out
* XDY_Autoroute		va_Autoroute_out
*
* Service rendu
* Lecture d'une actio sur un PMV sur un mobile
* 
* Code retour
* XDC_OK
* XDC_NOK	: actio non trouvee, ou insertion impossible
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
* Select dans les tables ACTION, PMV_MOBILE
----------------------------------------------*/


use PRC
go

create procedure XZAC;34
	@va_NumAction_in	int		= null,
	@va_Site_in		T_SITE		= null,
	@va_PR_out		T_PR		= null	output,
	@va_Sens_out		T_SENS		= null	output,
	@va_Message_out		char(48)	= null	output,
	@va_HoroDebut_out	datetime	= null	output,
	@va_HoroFin_out		datetime	= null	output,
	@va_Operateur_out	T_OPERATEUR	= null	output,
	@va_Autoroute_out	T_AUTOROUTE	= null	output
as
	declare	@vl_Status	int

	/*! transfo en paramÉtres nulls bug applix */
	if @va_NumAction_in = 0 select @va_NumAction_in = null
	if @va_Site_in = 0 select @va_Site_in = null

	/*A Controle des parametres d'entree */
	if @va_NumAction_in = null or @va_Site_in = null
		return XDC_ARG_INV

	/*A lecture de l'action */

#ifdef HIST

	select
		@va_HoroDebut_out	= heure_lancement,
		@va_HoroFin_out		= heure_fin,
		@va_Operateur_out       = operateur
	from    HIS..ACT_GEN
	where   numero = @va_NumAction_in and
		sit = @va_Site_in

	if @@rowcount != 1
		return XDC_NOK

	select		
		@va_PR_out		= PR,
		@va_Sens_out		= sens,
		@va_Message_out		= message,
		@va_Autoroute_out	= autoroute
	from 	HIS..ACT_RMQ
	where 	actio = @va_NumAction_in and 
		sit = @va_Site_in
	
#else

	select
		@va_HoroDebut_out	= heure_lancement,
		@va_HoroFin_out		= heure_fin,
		@va_Operateur_out	= operateur
	from    EXP..ACT_GEN
	where   numero = @va_NumAction_in and
		sit = @va_Site_in

	/*A si les infos ne sont pas trouvees     */
	/*A alors les rechercher dans la base HIS */
	if @@rowcount = 0
	begin
		exec @vl_Status = SQL_HIS.PRC..XZAC;34
					@va_NumAction_in,
					@va_Site_in,
					@va_PR_out		output,
					@va_Sens_out		output,
					@va_Message_out		output,
					@va_HoroDebut_out	output,
					@va_HoroFin_out		output,
					@va_Operateur_out	output,
					@va_Autoroute_out	output
		if @vl_Status = null
			return XDC_PRC_INC
		else return @vl_Status
	end


	select
		@va_PR_out		= PR,
		@va_Sens_out		= sens,
		@va_Message_out		= message,
		@va_Autoroute_out	= autoroute
	from 	EXP..ACT_RMQ
	where 	actio = @va_NumAction_in and 
		sit = @va_Site_in

#endif

	if @@rowcount != 1
		return XDC_NOK

	return XDC_OK
go
