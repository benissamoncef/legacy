/*E*/
/*  Fichier : $Id: xzae80sp.prc,v 1.8 2007/05/03 17:31:09 gesconf Exp $      Release : $Revision: 1.8 $        Date : $Date: 2007/05/03 17:31:09 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae80sp.prc
------------------------------------------------------
* DESCRIPTION DE LA PROCEDURE :
*
* retourne l'intitule d'un evenement
* pour la synthèse d'un evt ou operation
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.     	14/10/94 : Creation	V 1.1 
* C.T.     	14/02/95 : Modif format 	V 1.2 
*                          Site remplace par numero site
* C.T.     	17/02/95 : Modif longueur ligne 1.4 
* C.T.     	02/10/95 : Modif libelle sens pour A57 1.5
* B.G.		02/11/95 : Retour a la version 1.4 suite a gestion A57 1.6
* JPL		17/04/07 : Libelle "2 sens" pour Meteo, Inondation (DEM 638) 1.7
* JPL		03/05/07 : Libelle "2 sens" pour Traitement Chaussees (DEM 644) 1.8
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Retourne l'intitule d'une fiche main courante 
* 
* Sequence d'appel
* SP	XZAE80_Intitule_FMC
* 
* Arguments en entree
* XDY_Entier	va_NumEvt_in
* XDY_Site	va_CleEvt_in
* XDC_Mot	va_NumType_in
* XDC_Nom	va_Type_in
* XDY_Autoroute	va_Autoroute_in
* XDC_PR	va_PR_in
* XDC_Sens	va_Sens_in
* 
* Arguments en sortie
* char(60)	va_Intitule_out 
* 
* Code retour
* XDC_OK	
* XDC_NOK	evenement pas trouve
* 
* Conditions d'utilisation
* Cette procedure est utilisée par la procedure XZAE02
* Elle est interne a la base de données.
* 
* Fonction
* format de l'intitule : N%4s %2s %10s %4s pr %3s.%3s, %11s
* (NumEvt Site Type Autoroute pr Kilometres, Sens)
* 
-------------------------------------------------------*/

use PRC
go

drop procedure XZAE80
go


create procedure XZAE80
	@va_NumEvt_in		int		= null,
	@va_CleEvt_in		T_SITE		= null,
	@va_NumType_in		smallint	= null,
	@va_Type_in		char(10)	= null,
	@va_Autoroute_in	T_AUTOROUTE	= null,
	@va_PR_in		T_PR		= null,
	@va_Sens_in		T_SENS		= null,
	@va_Intitule_out	char(60)	= null	output
as
	declare @vl_NomAutoroute char(4), @vl_PR char(7), 
		@vl_NomSens char(21), @vl_ChaineNumEvt varchar(20), 
		@vl_ChaineFixe char(20),
		@vl_CaractDebut int, @vl_PR_float float, @vl_Longueur int

	/* intialisation */
	select @va_Intitule_out = null

	/*A creation de l'intitule avec l'evenement et son type */
	if @va_NumEvt_in != null and @va_CleEvt_in != null
	begin
		/* conversion du numero d'evenement en chaine de caracteres */
		select @vl_ChaineNumEvt = convert(char(20), @va_NumEvt_in)

		/* calcul la position du premier caractere a prendre */
		select @vl_Longueur = char_length(@vl_ChaineNumEvt),
			@vl_ChaineFixe = @vl_ChaineNumEvt
		if @vl_Longueur < 4
			select @vl_CaractDebut = 1
		else	select @vl_CaractDebut = @vl_Longueur - 3

		/* ajout l'evenement et son type dans l'intitule */
		select @va_Intitule_out = 'N' + 
				substring(@vl_ChaineFixe, @vl_CaractDebut, 4) +
				' ' + convert(char(2), @va_CleEvt_in) + ' ' + @va_Type_in
	end

	if @va_Autoroute_in != null 
	begin
		/* recherche le nom de l'autoroute */
		select @vl_NomAutoroute = nom
		from CFG..RES_AUT
		where numero = @va_Autoroute_in

		if @@rowcount != 1
			return XDC_NOK

		/* position en km */
		if @va_PR_in != null
		begin
			select @vl_PR_float = @va_PR_in
			select @vl_PR = str(@vl_PR_float/1000, 7, 3)
		end

		/* libelle du sens : nord ou sud */
		if @va_Sens_in = XDC_SENS_SORTIE_NORD
			select @vl_NomSens = XDC_LIB_SENS_SORTIE_NORD
		else if @va_Sens_in = XDC_SENS_SORTIE_SUD
			select @vl_NomSens = XDC_LIB_SENS_SORTIE_SUD
		else if @va_Sens_in = XDC_SENS_ENTREE_NORD
			select @vl_NomSens = XDC_LIB_SENS_ENTREE_NORD
		else if @va_Sens_in = XDC_SENS_ENTREE_SUD
			select @vl_NomSens = XDC_LIB_SENS_ENTREE_SUD
		else if @va_Sens_in = XDC_SENS_NORD
			select @vl_NomSens = XDC_LIB_SENS_NORD
		else if @va_Sens_in = XDC_SENS_SUD
			select @vl_NomSens = XDC_LIB_SENS_SUD
		else if (@va_NumType_in = XZAEC_FMC_Meteo  or
			 @va_NumType_in = XZAEC_FMC_Inondation  or
			 @va_NumType_in = XZAEC_FMC_TraitChaussees)
			select @vl_NomSens = XDC_LIB_2_SENS
		else	select @vl_NomSens = XDC_LIB_SENS_NON_PREC

		/*A ajout È l'intitule la localisation de l'evenement */
		select @va_Intitule_out = rtrim (@va_Intitule_out) + ' ' + 
					  @vl_NomAutoroute + ' pr ' +
					  @vl_PR + ', ' + @vl_NomSens
	end

	if @va_Intitule_out = null
		return XDC_ARG_INV

	return XDC_OK
go

