/*E*/
/*  Fichier : $Id: xzac37sp.prc,v 1.9 2010/09/02 09:46:39 gesconf Exp $      Release : $Revision: 1.9 $        Date : $Date: 2010/09/02 09:46:39 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac37sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	08/09/94	: Creation
* C.T.	13/09/94	: Modif parametre en entree
*                         (HoroLancFin remplace par HoroFin)
* C.T.	10/11/94	: Modif bugs applix (V 1.3)
* C.T.	14/11/94	: Modif recherche infos pour CI (V 1.4)
* C.T.	09/12/94	: Modif clause de lecture d'action (V 1.5)
*                         non teste
* C.T.	14/03/95	: Modif longueur champ affichette applix (V 1.6)
* C.P.	10/06/98	: Separation EXP et HIS (V 1.7)
* JPL	21/07/10	: Au CI ou sur secteur, si l'action est introuvable, executer sur HIS (DEM 941) 1.8
* JPL	01/09/10	: ... et retour compte-rendu OK si l'evenement est trouve 1.9
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Lecture d'une actio d'affichage dans une gare de peage
* 
* Sequence d'appel
* SP	XZAC37_Lire_Affichage_Gare
* 
* Arguments en entree
* XDY_Entier		va_NumeroAction_in
* XDY_Octet		va_Site_in
* 
* Arguments en sortie
* XDY_Nom		va_NomGare_out
* XDY_Horodate		va_HoroAffDebut_out
* XDY_Horodate		va_HoroAffFin_out
* char(250)		va_Affichette_out
* XY_Horodate		va_HoroLancement_out
* XY_Horodate		va_HoroFin_out
* XDY_Operateur		va_Operateur_out
* XY_Horodate		va_HoroSucces_out
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
* Select dans les tables ACTION, AFFICHAGE_EN_GARE
-----------------------------------------------------------------------*/

use PRC
go

create procedure XZAC;37
	@va_NumeroAction_in	int		= null,
	@va_Site_in		T_SITE		= null,
	@va_NomGare_out		T_NOM		= null	output,
	@va_HoroAffDebut_out	datetime	= null	output,
	@va_HoroAffFin_out	datetime	= null	output,
	@va_Affichette_out	char(250)	= null	output,
	@va_HoroLancement_out	datetime	= null	output,
	@va_HoroFin_out		datetime	= null	output,
	@va_Operateur_out	T_OPERATEUR	= null	output,
	@va_HoroSucces_out	datetime	= null	output
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

	select
		@va_HoroAffDebut_out	= HIS..ACT_GAR.horodate_debut,
		@va_HoroAffFin_out	= HIS..ACT_GAR.horodate_fin,
		@va_HoroLancement_out	= HIS..ACT_GEN.heure_lancement,
		@va_HoroFin_out		= HIS..ACT_GEN.heure_fin,
		@va_HoroSucces_out	= HIS..ACT_GEN.heure_succes,
		@va_NomGare_out		= HIS..ACT_GAR.nom_gare,
		@va_Affichette_out	= HIS..ACT_GAR.affichette,
		@va_Operateur_out	= HIS..ACT_GEN.operateur
	from 	HIS..ACT_GEN, HIS..ACT_GAR
	where 	HIS..ACT_GEN.numero = @va_NumeroAction_in and 
		HIS..ACT_GEN.type = XDC_ACT_AFF_GARE and
		HIS..ACT_GEN.sit = @va_Site_in and
		HIS..ACT_GAR.actio = @va_NumeroAction_in and 
		HIS..ACT_GAR.sit = @va_Site_in

	select @vl_NbEnrg = @@rowcount

#else

	select
		@va_HoroAffDebut_out	= EXP..ACT_GAR.horodate_debut,
		@va_HoroAffFin_out	= EXP..ACT_GAR.horodate_fin,
		@va_HoroLancement_out	= EXP..ACT_GEN.heure_lancement,
		@va_HoroFin_out		= EXP..ACT_GEN.heure_fin,
		@va_HoroSucces_out	= EXP..ACT_GEN.heure_succes,
		@va_NomGare_out		= EXP..ACT_GAR.nom_gare,
		@va_Affichette_out	= EXP..ACT_GAR.affichette,
		@va_Operateur_out	= EXP..ACT_GEN.operateur
	from 	EXP..ACT_GEN, EXP..ACT_GAR
	where 	EXP..ACT_GEN.numero = @va_NumeroAction_in and 
		EXP..ACT_GEN.type = XDC_ACT_AFF_GARE and
		EXP..ACT_GEN.sit = @va_Site_in and
		EXP..ACT_GAR.actio = @va_NumeroAction_in and 
		EXP..ACT_GAR.sit = @va_Site_in

	select @vl_NbEnrg = @@rowcount

	/*A si les infos ne sont pas trouvees     */
	/*A alors les rechercher dans la base HIS */
	if @vl_NbEnrg = 0
	begin
		exec @vl_Status = SQL_HIS.PRC..XZAC;37
					@va_NumeroAction_in,
					@va_Site_in,
					@va_NomGare_out		output,
					@va_HoroAffDebut_out	output,
					@va_HoroAffFin_out	output,
					@va_Affichette_out	output,
					@va_HoroLancement_out	output,
					@va_HoroFin_out		output,
					@va_Operateur_out	output,
					@va_HoroSucces_out	output
		if @vl_Status = null
			return XDC_PRC_INC
		else return @vl_Status
	end

#endif

	if @vl_NbEnrg != 1
		return XDC_NOK

	return XDC_OK
go
