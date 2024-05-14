/*E*/
/*  Fichier : $Id: xzac30sp.prc,v 1.10 2010/08/30 15:58:17 gesconf Exp $        Release : $Revision: 1.10 $        Date : $Date: 2010/08/30 15:58:17 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac30sp.prc
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
* C.T.	26/06/95	: Remplacer le numero interlocuteur par son nom (V 1.5)
* B.N.  29/08/96	: Utilisation de HIS à la place de EXP pour recherche > 1 mois (DEM 1159) (V1.6-1.7)
* C.P.	08/06/98	: Modif separation EXP et HIS (V 1.8)
* JMG   21/11/07        : ajout envoi de mail DEM/718 1.9
* JPL	19/07/10	: Au CI ou sur secteur, si l'action est introuvable, executer sur HIS (DEM 941) 1.10
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Lecture d'une actio d'envoi d'un fax
* 
* Sequence d'appel
* SP	XZAC30_Lire_Fax
* 
* Arguments en entree
* XDY_Entier		va_NumAction_in
* XDY_Octet		va_Site_in
* 
* Arguments en sortie
* XDY_Nom		va_Interlocuteur_out
* XDY_Tel		va_NumeroTalFax_out
* XDY_Horodate		va_HoroLancement_out
* XDY_Operateur		va_Operateur_out
* XDY_Horodate		va_HeureSucces_out
* XDY_Horodate		va_HeureEchec_out
* 
* Code retour
* XDC_OK
* XDC_NOK	: actio non trouvee
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* Le texte du fax est retourne sous forme de liste de morceaux de texte ordonnes
* 
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
* Recuperer de la table TEXTE_FAX le texte du fax par morceau et le reconstituer
* Appeler XZAC31
----------------------------------------------------*/

use PRC
go

create procedure XZAC;30
	@va_NumAction_in	int		= null,
	@va_Site_in		T_SITE		= null,
	@va_Interlocuteur_out	T_NOM		= null	output,
	@va_NumeroTelFax_out	char(100)	= null	output,
	@va_HoroLancement_out	datetime	= null	output,
	@va_Operateur_out	T_OPERATEUR	= null	output,
	@va_HoroSucces_out	datetime	= null	output,
	@va_HoroEchec_out	datetime	= null	output
as
	declare @vl_Status int, @vl_TypeAppel tinyint, @vl_NumActionFax int

	/*! transfo en paramÉtres nulls bug applix */
	if @va_NumAction_in = 0 select @va_NumAction_in = null
	if @va_Site_in = 0 select @va_Site_in = null

	/*A controle des parametres obligatoires */
	if @va_NumAction_in = null or @va_Site_in = null
		return XDC_ARG_INV

	/*A lecture de l'actio generique et du complement d'appel de fax */

#ifdef HIST

	select @vl_NumActionFax = action_fax
	from HIS..ACT_APL
	where actio = @va_NumAction_in and sit = @va_Site_in

	if @vl_NumActionFax is null
		select @vl_NumActionFax = @va_NumAction_in

	exec @vl_Status = XZAC;31 	@va_NumAction_in, @va_Site_in, 
					@va_Interlocuteur_out	output,
					@va_NumeroTelFax_out	output,
					@vl_TypeAppel		output,
					@va_HoroLancement_out	output,
					@va_Operateur_out	output,
					@va_HoroEchec_out	output,
					@va_HoroSucces_out	output
	if @vl_Status = null
		return XDC_PRC_INC
	else if @vl_Status != XDC_OK
		return @vl_Status

	/*A lecture du texte du fax par morceau */
 	/*A rechercher dans la base HIS */
	select texte from HIS..ACT_FAX
	where actio = @vl_NumActionFax and sit = @va_Site_in
	order by morceau
		
#else

	/*A recherche si l'action est liee a une autre action fax */
	select @vl_NumActionFax = action_fax
	from EXP..ACT_APL
	where actio = @va_NumAction_in and sit = @va_Site_in

	/*A si les infos ne sont pas trouvees     */
	/*A alors les rechercher dans la base HIS */
	if @@rowcount = 0
	begin
		exec @vl_Status = SQL_HIS.PRC..XZAC;30
					@va_NumAction_in,
					@va_Site_in,
					@va_Interlocuteur_out	output,
					@va_NumeroTelFax_out	output,
					@va_HoroLancement_out	output,
					@va_Operateur_out	output,
					@va_HoroSucces_out	output,
					@va_HoroEchec_out	output
		if @vl_Status = null
			return XDC_PRC_INC
		else return @vl_Status
	end

	if @vl_NumActionFax is null
		select @vl_NumActionFax = @va_NumAction_in


	exec @vl_Status = XZAC;31	@va_NumAction_in, @va_Site_in, 
					@va_Interlocuteur_out	output,
					@va_NumeroTelFax_out	output,
					@vl_TypeAppel		output,
					@va_HoroLancement_out	output,
					@va_Operateur_out	output,
					@va_HoroEchec_out	output,
					@va_HoroSucces_out	output
	if @vl_Status = null
		return XDC_PRC_INC
	else if @vl_Status != XDC_OK
		return @vl_Status

	/*A lecture du texte du fax par morceau */
	select texte from EXP..ACT_FAX
	where actio = @vl_NumActionFax and sit = @va_Site_in
	order by morceau

#endif

	return XDC_OK
go
