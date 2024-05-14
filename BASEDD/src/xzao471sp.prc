/*E*/
/*  Fichier : $Id: xzao471sp.prc,v 1.1 2016/11/23 20:25:00 devgfi Exp $        Release : $Revision: 1.1 $        Date : $Date: 2016/11/23 20:25:00 $
-------------------------------------------------------------------------------
* ESCOTA *  PROJET MIGRAZUR
-------------------------------------------------------------------------------
* SOUS-SYSTEME  BASEDD
-------------------------------------------------------------------------------
* FICHIER xzao471sp.prc
-------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Determination de la limite de vitesse dans une zone delimitee par deux PR.
*
-------------------------------------------------------------------------------
* HISTORIQUE :
*
* JPL	23/11/16 : Creation (DEM 1193) 1.1
-----------------------------------------------------------------------------*/

/*X*/
/*-----------------------------------------------------------------------------
* SERVICE RENDU
*
*  Determine la vitesse limite autorisee dans une zone situee entre deux PR
*  sur une autoroute et dans un sens donnes.
*  Si la zone est reduite a un point, retourne la limite de vitesse a partir
*  de ce point.
*
* -----------------------------------------------------------------------------
* SEQUENCE D'APPEL
*
* SP	XZAO471_Limite_Vitesse_Zone
*
* PARAMETRES EN ENTREE :
*
* XDY_Octet	va_Autoroute_in		: identifiant de l'autoroute
* XDY_Entier	va_PR_Debut_in		: PR de debut de zone
* XDY_Entier	va_PR_Fin_in		: PR de fin de zone
* XDY_Octet	va_Sens_in		: indicateur du sens de circulation.
*
* PARAMETRES EN SORTIE :
*
* XDY_Octet	va_Vitesse_out		: Valeur de la limite de vitesse dans la zone.
*
* CODE RETOUR :
*
* XDC_OK
* XDC_ARG_INV       : parametre d'entree obligatoire manquant (Null) ou invalide
* <0                : erreur Sybase
*
* CONDITIONS D'UTILISATION :
*
*  Pas de contrainte sur l'ordre des PR par rapport au sens de circulation.
*  Si l'un des PR est null alors l'autre est considere comme point de depart.
*
* FONCTION :
*
*  Si la zone n'est pas reduite a un point, determination du minimum
*  de la vitesse autorisee definie par les panneaux de police situes
*  dans la zone et dans le sens indique.
*
*  Puis prise en compte de la consigne de vitesse definie par le panneau
*  de police le plus proche situe en amont ou au tout debut de la zone.
*
-----------------------------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAO471' and type = 'P')
	drop procedure XZAO471
go


create procedure XZAO471
	@va_Autoroute_in	T_AUTOROUTE,
	@va_PR_Debut_in		T_PR,
	@va_PR_Fin_in		T_PR,
	@va_Sens_in		T_SENS,
	@va_Vitesse_out		tinyint output
as

declare
	@vl_PR_Min		T_PR,
	@vl_PR_Max		T_PR,
	@vl_Sens_PRs		smallint,
	@vl_Vitesse_Zone	tinyint,
	@vl_Vitesse_Amont	tinyint

	/*A Limite de vitesse par defaut, en l'absence de consigne dans la zone et en amont */
	select @va_Vitesse_out = 130


	/*A
	** Verifier la validite des parametres d'entree obligatoires
	*/

	select @vl_Sens_PRs = inversee from CFG..RES_AUT_INV where numero = @va_Autoroute_in
	if @@rowcount = 0
		return XDC_ARG_INV

	if @va_Sens_in not in ( XDC_SENS_1, XDC_SENS_2 )
		return XDC_ARG_INV

	if ( ( @va_PR_Debut_in < 0 )  or  ( @va_PR_Fin_in < 0 ) or
	     ( @va_PR_Debut_in is null  and  @va_PR_Fin_in is null ) )
		return XDC_ARG_INV


	/*A
	** Ordonner les PRs, en les identifiant si l'un est manquant
	*/

	select @vl_PR_Min = @va_PR_Debut_in, @vl_PR_Max = @va_PR_Fin_in

	if @vl_PR_Min is null
		select @vl_PR_Min = @vl_PR_Max
	else if @vl_PR_Max is null
		select @vl_PR_Max = @vl_PR_Min
	else if @vl_PR_Min > @vl_PR_Max
		select @vl_PR_Min = @va_PR_Fin_in, @vl_PR_Max = @va_PR_Debut_in

	--select "PRs : " , @vl_PR_Min, @vl_PR_Max


	/*A
	** Determiner la vitesse autorisee en amont de la zone ou du lieu specifie
	*/

	if ( @va_Sens_in = XDC_SENS_1  and  @vl_Sens_PRs = XDC_PR_NOR )  or
	   ( @va_Sens_in = XDC_SENS_2  and  @vl_Sens_PRs = XDC_PR_INV )
	begin
		/*B Dans le sens des PR croissants, choisir le panneau de plus grand PR avant le PR minimal de la zone */
		select	@vl_Vitesse_Amont = vitesse
		from	CFG..RES_POR POR, CFG..RES_POL POL
		where	POR.autoroute = @va_Autoroute_in  and  POL.sens = @va_Sens_in
		  and	POL.portion = POR.numero
		  and	POL.PR <= @vl_PR_Min

		having	POL.PR = max ( POL.PR )
		  and	POR.autoroute = @va_Autoroute_in  and  POL.sens = @va_Sens_in
		  and	POL.portion = POR.numero
	end
	else
	begin
		/*B Dans le sens des PR decroissants, choisir le panneau de plus petit PR apres le PR maximal de la zone */
		select	@vl_Vitesse_Amont = vitesse
		from	CFG..RES_POR POR, CFG..RES_POL POL
		where	POR.autoroute = @va_Autoroute_in  and  POL.sens = @va_Sens_in
		  and	POL.portion = POR.numero
		  and	POL.PR >= @vl_PR_Max

		having	POL.PR = min ( POL.PR )
		  and	POR.autoroute = @va_Autoroute_in  and  POL.sens = @va_Sens_in
		  and	POL.portion = POR.numero
	end

	/*B Retourner la limite de vitesse amont si elle est plus restrictive */
	if @vl_Vitesse_Amont < @va_Vitesse_out
		select @va_Vitesse_out = @vl_Vitesse_Amont


	/*A
	** Si la zone n'est pas reduite a un point alors determiner la vitesse limite dans cette zone
	*/

	if @vl_PR_Min <> @vl_PR_Max
	begin
		select @vl_Vitesse_Zone = min ( POL.vitesse )
		from	CFG..RES_POR POR, CFG..RES_POL POL
		where	POR.autoroute = @va_Autoroute_in  and  POL.sens = @va_Sens_in
		  and	POL.portion = POR.numero
		  and	POL.PR between ( @vl_PR_Min + 1 ) and ( @vl_PR_Max - 1 )

		/*B Retourner la limite de vitesse dans la zone si elle est plus restrictive */
		if @vl_Vitesse_Zone < @va_Vitesse_out
			select @va_Vitesse_out = @vl_Vitesse_Zone
	end

	return XDC_OK
go
