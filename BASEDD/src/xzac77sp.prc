/*E*/
/*Fichier : $Id: xzac77sp.prc,v 1.2 2020/10/09 14:18:02 gesconf Exp $      Release : $Revision: 1.2 $        Date : $Date: 2020/10/09 14:18:02 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac77sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture message TFM
*
------------------------------------------------------
* HISTORIQUE :
*
* PNI	23/02/05	: Creation
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Lecture de l'actio Message TFM
* 
* Sequence d'appel
* SP	XZAC77_Lire_ActionTFM
* 
* Arguments en entree
* XDY_Entier	va_NumeroFMC_in
* XDY_Octet	va_Site_in
* 
* Arguments en sortie
* char*		va_Message_out			liste de morceau de texte
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
* Select des tables PLAN ACTION et MESSAGE_TFM
------------------------------------------------------*/

use PRC
go
if exists (select * from sysobjects where name = 'XZAC77' and type = 'P')
        drop procedure XZAC77
go

create procedure XZAC77
	@va_NumeroFMC_in		int		= null,
	@va_Site_in			tinyint		= null,
        @va_NumPA_in            	smallint	= null
as
declare	@vl_NumeroAction		int,
	@vl_Site			tinyint
	
#ifdef HIST	
	/*! transfo en paramÉtres nulls bug applix */
	if @va_NumeroFMC_in = 0 select @va_NumeroFMC_in = null
	if @va_Site_in = 0 select @va_Site_in = null

	/*A controle des parametres obligatoires */
	if @va_NumeroFMC_in = null or @va_Site_in = null
		return XDC_ARG_INV

	
	select @vl_NumeroAction=numero,
		@vl_Site=sit
	from HIS..ACT_GEN 
	where evenement=@va_NumeroFMC_in and cle=@va_Site_in and type=XDC_ACT_TFM 
	group by evenement,cle
	having numero=max(numero)


	/*A lecture du texte du TFM */
	select
		texte
	from  HIS..ACT_TXT_TFM
	where actio = @vl_NumeroAction  and sit = @vl_Site
	order by morceau
#else	
	if @va_NumPA_in != 0 and @va_NumPA_in != null
	begin
		select @va_NumeroFMC_in = evenement,
			@va_Site_in = cle
		from EXP..SAD_PLN
		where numero=@va_NumPA_in
	end
	/*! transfo en paramÉtres nulls bug applix */
	if @va_NumeroFMC_in = 0 select @va_NumeroFMC_in = null
	if @va_Site_in = 0 select @va_Site_in = null

	/*A controle des parametres obligatoires */
	if @va_NumeroFMC_in = null or @va_Site_in = null
		return XDC_ARG_INV

	
	select @vl_NumeroAction=numero,
		@vl_Site=sit
	from EXP..ACT_GEN 
	where evenement=@va_NumeroFMC_in and cle=@va_Site_in and type=XDC_ACT_TFM 
	group by evenement,cle
	having numero=max(numero)


	/*A lecture du texte du TFM */
	select
		texte
	from  EXP..ACT_TXT_TFM
	where actio = @vl_NumeroAction  and sit = @vl_Site
	order by morceau

	return XDC_OK
#endif
go
