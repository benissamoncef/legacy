/*E*/
/*  Fichier : $Id: xzac31sp.prc,v 1.10 2010/08/30 15:58:17 gesconf Exp $        Release : $Revision: 1.10 $        Date : $Date: 2010/08/30 15:58:17 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac31sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	08/09/94	: Creation
* C.T.	10/11/94	: Modif bugs applix (V 1.3)
* C.T.	14/11/94	: Modif recherche infos pour CI (V 1.4)
* C.T.	26/06/95	: Remplacer le numero interlocuteur par son nom (V 1.5)
* C.P.	08/06/98	: Modif separation EXP et HIS (V 1.6)
* Hilmarcher 05/05/04   : Modif pour debug proc v1.7
* JMG   21/11/07        : ajout envoi de mail DEM/718 1.9
* JPL	19/07/10	: Au CI ou sur secteur, si l'action est introuvable, executer sur HIS (DEM 941) 1.10
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Lecture d'une actio d'appel telephonique ou bip.
* 
* Sequence d'appel
* SP	XZAC31_Lire_Appel_Tel_Bip
* 
* Arguments en entree
* XDY_Entier	va_NumAction_in
* XDY_Octet	va_Site_in
* 
* Arguments en sortie
* XDY_Nom	va_Interlocuteur_out
* XDY_Tel	va_NumeroTalFax_out
* XDY_Octet	va_TypeAppel_out
* XDY_Horodate	va_HoroLancement_out
* XDY_Operateur	va_Operateur_out
* XDY_Horodate	va_HoroEchec_out
* XDY_Horodate	va_HoroSucces_out
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
* Select dans les tables ACTION et APPEL_TELEPHONE_BIP_OU_FAX
------------------------------------------------------*/

use PRC
go

create procedure XZAC;31
	@va_NumAction_in	int		= null,
	@va_Site_in		T_SITE		= null,
	@va_Interlocuteur_out	T_NOM		= null	output,
	@va_NumeroTelFax_out	char(100)	= null	output,
	@va_TypeAppel_out	tinyint		= null	output,
	@va_HoroLancement_out	datetime	= null	output,
	@va_Operateur_out	smallint	= null	output,
	@va_HoroEchec_out	datetime	= null	output,
	@va_HoroSucces_out	datetime	= null	output
as
	declare	@vl_NumInterlocuteur	smallint,
		@vl_Status		int

	/*! transfo en paramÉtres nulls bug applix */
	if @va_NumAction_in = 0 select @va_NumAction_in = null
	if @va_Site_in = 0 select @va_Site_in = null

	/*A Controle des parametres d'entree */
	if @va_NumAction_in = null or @va_Site_in = null
		return XDC_ARG_INV

	/*A lecture de l'action */

#ifdef HIST

	begin
		select
			@vl_NumInterlocuteur	= interlocuteur,
			@va_NumeroTelFax_out	= numero_telephonique,
			@va_TypeAppel_out	= telephone_ou_bip_ou_fax
		from HIS..ACT_APL
		where   actio = @va_NumAction_in and
			sit = @va_Site_in

		if @@rowcount = 0
			return XDC_NOK

		select
			@va_HoroLancement_out	= heure_lancement,
			@va_HoroEchec_out	= heure_echec,
			@va_Operateur_out	= operateur,
			@va_HoroSucces_out	= heure_succes
		from HIS..ACT_GEN 
		where	numero = @va_NumAction_in and 
			sit = @va_Site_in

		if @@rowcount = 0
			return XDC_NOK
	end

#else

	begin
		select
			@vl_NumInterlocuteur	= interlocuteur,
			@va_NumeroTelFax_out	= numero_telephonique,
			@va_TypeAppel_out	= telephone_ou_bip_ou_fax
		from EXP..ACT_APL
		where   actio = @va_NumAction_in and
			sit = @va_Site_in

		/*A si les infos ne sont pas trouvees     */
		/*A alors les rechercher dans la base HIS */
		if @@rowcount = 0
		begin
			exec @vl_Status = SQL_HIS.PRC..XZAC;31
						@va_NumAction_in,
						@va_Site_in,
						@va_Interlocuteur_out	output,
						@va_NumeroTelFax_out	output,
						@va_TypeAppel_out	output,
						@va_HoroLancement_out	output,
						@va_Operateur_out	output,
						@va_HoroEchec_out	output,
						@va_HoroSucces_out	output
			if @vl_Status = null
				return XDC_PRC_INC
			else return @vl_Status
		end

		select
			@va_HoroLancement_out	= heure_lancement,
			@va_HoroEchec_out	= heure_echec,
			@va_Operateur_out	= operateur,
			@va_HoroSucces_out	= heure_succes
		from EXP..ACT_GEN
		where	numero = @va_NumAction_in and 
			sit = @va_Site_in 

		if @@rowcount = 0
			return XDC_NOK
	end

#endif

	/*A recherche le nom de l'interlocuteur */
	select 	@va_Interlocuteur_out = nom
	from CFG..ANN_FIC
	where numero = @vl_NumInterlocuteur

	if @va_Interlocuteur_out is null
		select  @va_Interlocuteur_out = XDC_CHAINE_VIDE

	return XDC_OK
go
