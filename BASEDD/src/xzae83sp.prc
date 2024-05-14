/*E*/
/*  Fichier : $Id: xzae83sp.prc,v 1.5 2011/10/05 15:53:19 gesconf Exp $      Release : $Revision: 1.5 $        Date : $Date: 2011/10/05 15:53:19 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae83sp.prc
------------------------------------------------------
* DESCRIPTION DE LA PROCEDURE :
*
* retourne l'intitule evenement et la localisation
* pour la synthèse d'un evt ou operation
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.     	17/11/95 : Creation	V 1.1 
* C.T.     	30/01/96 : Numero sur 6 digits V 1.2
* JPL		17/04/07 : Libelle "2 sens" pour Meteo, Inondation (DEM 638) 1.3
* JPL		03/05/07 : Libelle "2 sens" pour Traitement Chaussees (DEM 644) 1.4
* JPL		23/09/11 : Initalisations, test sens invalide, code de retour  1.5
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Retourne l'intitule et la localisation d'une fiche main courante 
* 
* Sequence d'appel
* SP	XZAE83_Intitule_2_lignes_FMC
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
* char(60)	va_localisation_out
* 
* Code retour
* XDC_OK	
* XDC_NOK	autoroute inconnue ou sens de circulation invalide
* 
* Conditions d'utilisation
* Cette procedure est une sous-procedure utilisablee par toute autre
* procedure. Elle est interne a la base de données.
* 
* Fonction
*   Si un numero d'evenement est fournie, construit son intitule
*   de format	N%6s %2s %25s          (NumEvt Site Type )
*
*   Si l'autoroute est fournie, construit le libelle de localisation
*   de format	%4s pr %3s.%3s, %s     (Autoroute pr Kilometres, Sens)
* 
-------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAE83' and type = 'P')
	drop procedure XZAE83
go


create procedure XZAE83
	@va_NumEvt_in		int		= null,
	@va_CleEvt_in		T_SITE		= null,
	@va_NumType_in		smallint	= null,
	@va_Type_in		T_NOM		= null,
	@va_Autoroute_in	T_AUTOROUTE	= null,
	@va_PR_in		T_PR		= null,
	@va_Sens_in		T_SENS		= null,
	@va_Intitule_out	char(60)	= null	output,
	@va_Localisation_out	char(60)	= null	output
as
	declare @vl_NomAutoroute char(4), @vl_PR char(7), @vl_NomSens char(21),
		@vl_ChaineNumEvt varchar(20), @vl_ChaineFixe char(20),
		@vl_CaractDebut int, @vl_Longueur int,
		@vl_PR_float float

	/* Intialisations */
	/* ... fait par les declarations */


	/*A creation de l'intitule avec l'evenement et son type */
	if @va_NumEvt_in != null and @va_CleEvt_in != null
	begin
		/* conversion du numero d'evenement en chaine de caracteres */
		select @vl_ChaineNumEvt = convert(char(20), @va_NumEvt_in)

		/* calcul la position du premier caractere a prendre */
		select @vl_Longueur = char_length(@vl_ChaineNumEvt),
			@vl_ChaineFixe = @vl_ChaineNumEvt
		if @vl_Longueur < XZAEC_NB_DIGITS_LIB_FMC
			select @vl_CaractDebut = 1
		else	select @vl_CaractDebut = @vl_Longueur - (XZAEC_NB_DIGITS_LIB_FMC - 1)

		/* ajout l'evenement et son type dans l'intitule */
		select @va_Intitule_out = 'N' + 
				substring(@vl_ChaineFixe, @vl_CaractDebut, XZAEC_NB_DIGITS_LIB_FMC) +
				' ' + convert(char(2), @va_CleEvt_in) + ' ' + @va_Type_in 
	end


	/* Construction du libelle de localisation par PR et sens de circulation */
	if @va_Autoroute_in != null 
	begin
		/* recherche le nom de l'autoroute */
		select @vl_NomAutoroute = nom
		from CFG..RES_AUT
		where numero = @va_Autoroute_in

		if @@rowcount != 1
			return XDC_NOK

		/* PR en km */
		if @va_PR_in != null
		begin
			select @vl_PR_float = @va_PR_in
			select @vl_PR = str(@vl_PR_float/1000, 7, 3)
		end

		/* Libelle du sens incluant le cas "2 sens" */
		if @va_Sens_in = XDC_SENS_NORD
			select @vl_NomSens = XDC_LIB_SENS_NORD
		else if @va_Sens_in = XDC_SENS_SUD
			select @vl_NomSens = XDC_LIB_SENS_SUD
		else if @va_Sens_in = XDC_SENS_SORTIE_NORD
			select @vl_NomSens = XDC_LIB_SENS_SORTIE_NORD
		else if @va_Sens_in = XDC_SENS_SORTIE_SUD
			select @vl_NomSens = XDC_LIB_SENS_SORTIE_SUD
		else if @va_Sens_in = XDC_SENS_ENTREE_NORD
			select @vl_NomSens = XDC_LIB_SENS_ENTREE_NORD
		else if @va_Sens_in = XDC_SENS_ENTREE_SUD
			select @vl_NomSens = XDC_LIB_SENS_ENTREE_SUD
		else if @va_Sens_in = XDC_SENS_INCONNU
		begin
			if (@va_NumType_in = XZAEC_FMC_Meteo  or
			    @va_NumType_in = XZAEC_FMC_Inondation  or
			    @va_NumType_in = XZAEC_FMC_TraitChaussees)
				select @vl_NomSens = XDC_LIB_2_SENS
			else
				select @vl_NomSens = XDC_LIB_SENS_NON_PREC
		end
		else
			return XDC_NOK

		select @va_Localisation_out = @vl_NomAutoroute + ' pr ' +
		                                 @vl_PR + ', ' + @vl_NomSens
	end

	return XDC_OK
go
