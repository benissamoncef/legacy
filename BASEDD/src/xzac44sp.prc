/*E*/
/*  Fichier : $Id: xzac44sp.prc,v 1.9 2010/08/30 15:58:22 gesconf Exp $        Release : $Revision: 1.9 $        Date : $Date: 2010/08/30 15:58:22 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac44sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	09/09/94	: Creation
* C.T.	20/09/94	: Modif retour du message tfm 
*                         suite a modif de table ACT_TXT_TFM
*                         modif interface (ajout site)
* C.T.	10/11/94	: Modif bugs applix (V 1.3)
* C.T.	14/11/94	: Modif recherche infos pour CI (V 1.4)
* C.T.	09/12/94	: Modif clause de lecture d'action (V 1.5)
*                         non teste
* B.G.	28/12/94	: Modif jointure 
* P.N.  05/11/96	: Ajout du niveau TFM (dem/1248) v1.7
* C.P.	10/06/98	: Separation EXP et HIS (V 1.8)
* JPL	15/07/10	: Au CI, si l'action est introuvable, executer sur HIS (DEM 941) 1.9
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Lecture de l'actio Message TFM
* 
* Sequence d'appel
* SP	XZAC44_Lire_ActionTFM
* 
* Arguments en entree
* XDY_Entier	va_NumeroAction_in
* XDY_Octet	va_Site_in
* 
* Arguments en sortie
* XDY_Eqt	va_NumEqt_out
* XDY_Horodate	va_HeureLancement_out		envoi a la radio
* XDY_Horodate	va_HeureLancementFin_out	envoi de la fin de message
* XDY_Horodate	va_HeureSucces_out		premiere diffusion
* XDY_Horodate	va_HeureFin_out			suppression du message
* XDY_Entier	va_NombreDiffusions_out
* char*		va_Message_out			liste de morceau de texte
* char*		va_Niveau_out			liste de morceau de texte
* 
* Code retour
* XDC_OK
* XDC_NOK	: actio non trouvee
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* le message de trafic FM est retourne dans une liste de 
* de morceau de texte ordonnee
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
* Select des tables ACTION et MESSAGE_TFM
------------------------------------------------------*/

use PRC
go

create procedure XZAC;44
	@va_NumeroAction_in		int		= null,
	@va_Site_in			tinyint		= null,
	@va_HeureLancement_out		datetime	= null		output,
	@va_HeureLancementFin_out	datetime	= null		output,
	@va_HeureSucces_out		datetime	= null		output,
	@va_HeureFin_out		datetime	= null		output,
	@va_NombreDiffusions_out	int		= null		output,
	@va_Niveau_out			tinyint		= null		output
as
	declare	@vl_Status	int

	/*! transfo en paramÉtres nulls bug applix */
	if @va_NumeroAction_in = 0 select @va_NumeroAction_in = null
	if @va_Site_in = 0 select @va_Site_in = null

	/*A controle des parametres obligatoires */
	if @va_NumeroAction_in = null or @va_Site_in = null
		return XDC_ARG_INV

	/*A lecture des infos sur une action TFM */

#ifdef HIST

	begin
		select	@va_HeureLancement_out		= heure_lancement,
			@va_HeureLancementFin_out	= heure_lancement_fin,
			@va_HeureSucces_out		= heure_succes,
			@va_HeureFin_out		= heure_fin
		from  HIS..ACT_GEN
		where 	numero = @va_NumeroAction_in and
	 		type = XDC_ACT_TFM and
	 		sit = @va_Site_in

		select  @va_NombreDiffusions_out	= nombre_diffusions,
			@va_Niveau_out	= niveau
		from  HIS..ACT_TFM
		where 	actio = @va_NumeroAction_in  and 
			sit = @va_Site_in

		if @@rowcount = 0
			return XDC_NOK

		/*B lecture du texte du TFM */
		select
			texte
		from  HIS..ACT_TXT_TFM
		where actio = @va_NumeroAction_in  and sit = @va_Site_in
		order by morceau
	end

	
#else

	/*! cas ou la procedure est creee au CI */

	select	@va_HeureLancement_out		= heure_lancement,
		@va_HeureLancementFin_out	= heure_lancement_fin,
		@va_HeureSucces_out		= heure_succes,
		@va_HeureFin_out		= heure_fin
	from  EXP..ACT_GEN
	where 	numero = @va_NumeroAction_in and
	 	type = XDC_ACT_TFM and
	 	sit = @va_Site_in

	/*A si les infos ne sont pas trouvees     */
 	/*A alors les rechercher dans la base HIS */
	if @@rowcount = 0
	begin
		exec @vl_Status = SQL_HIS.PRC..XZAC;44
					@va_NumeroAction_in,
					@va_Site_in,
					@va_HeureLancement_out		output,
					@va_HeureLancementFin_out	output,
					@va_HeureSucces_out		output,
					@va_HeureFin_out		output,
					@va_NombreDiffusions_out	output,
					@va_Niveau_out			output
		if @vl_Status = null
			return XDC_PRC_INC
		else return @vl_Status
	end


	select  @va_NombreDiffusions_out	= nombre_diffusions,
		@va_Niveau_out	= niveau
	from  EXP..ACT_TFM
	where 	actio = @va_NumeroAction_in  and 
		sit = @va_Site_in
	
	if @@rowcount = 0
		return XDC_NOK

	/*A lecture du texte du TFM */
	select
		texte
	from  EXP..ACT_TXT_TFM
	where actio = @va_NumeroAction_in  and sit = @va_Site_in
	order by morceau

#endif

	return XDC_OK
GO
