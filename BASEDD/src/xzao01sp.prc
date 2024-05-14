/*E*/
/*  Fichier : $Id: xzao01sp.prc,v 1.5 2010/10/12 09:09:42 gesconf Exp $      Release : $Revision: 1.5 $        Date : $Date: 2010/10/12 09:09:42 $
-------------------------------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
-------------------------------------------------------------------------------
* SOUS-SYSTEME  BASEDD
-------------------------------------------------------------------------------
* FICHIER xzao01sp.prc
-------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture de la configuration du reseau d'autoroutes
*
-------------------------------------------------------------------------------
* HISTORIQUE :
*
* B.G.  10/11/94 : Creation (v1.1)
* B.G.	24/10/95 : gestion du sens inverse A57 (1.2)
* JPL	05/11/08 : Gestion d'un itineraire multi-autoroutes 1.3
* JPL	05/11/08 : Table tempo. et resultats en un seul 'select' (bug applix) 1.4
* JPL	30/09/10 : Support indication extremites par (autoroute, PR) (DEM 930) 1.5
-----------------------------------------------------------------------------*/

/*X*/
/*-----------------------------------------------------------------------------
* SERVICE RENDU
*
* Recherche la liste des ouvrages d'art dont la hauteur est inferieure
* a une hauteur donnee sur l'itineraire dont les extremites amont et aval
* sont indiquees soit par un echangeur, soit par un couple (autoroute, PR).
*
* -----------------------------------------------------------------------------
* SEQUENCE D'APPEL
*
* SP	XZAO01_Gabarit
*
* PARAMETRES EN ENTREE :
*
* XDY_Mot		va_Hauteur_in
* XDY_Octet		va_AutorouteAmont_in
* XDY_Entier		va_PR_Amont_in
* XDY_Octet		va_AutorouteAval_in
* XDY_Entier		va_PR_Aval_in
* XDY_Mot		va_EchangeurAmont_in
* XDY_Mot		va_EchangeurAval_in
*
* PARAMETRES EN SORTIE :
*
* Aucun
*
* VALEUR RENDUE :
*
* XDY_Nom		va_NomOuvrage_out   |
* XDY_Mot		va_Hauteur_out      | dans un select
* XDY_PR		va_PR_out 	    |
* XDY_NomAuto		va_NomAutoroute_out |
*
* CODE RETOUR :
*
* XDC_OK
* XDC_NOK :		parametre(s) incorrect(s) (echangeur inconnu)
* XDC_ARG_INV :		parametre d'entree invalide (NULL)
* XDC_ACT_REFUSEE :	impossible (pas d'itineraire entre les echangeurs)
* <0 erreur sybase
*
* CONDITIONS D'UTILISATION :
*
* FONCTION :
*
* Decompose l'itineraire defini par les echangeurs amont et aval
* en une suite d'etapes situees sur un meme autoroute,
* et pour chacune de ces etapes recherche la liste des ouvrages d'art
* et des tunnels de hauteur inferieure a celle indiquee.
*
-----------------------------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAO01' and type = 'P')
	drop procedure XZAO01
go


create procedure XZAO01
	@va_Hauteur_in		smallint = null,
	@va_AutorouteAmont_in	T_AUTOROUTE = null,
	@va_PR_Amont_in		T_PR = null,
	@va_AutorouteAval_in	T_AUTOROUTE = null,
	@va_PR_Aval_in		T_PR = null,
	@va_EchangeurAmont_in	smallint = null,
	@va_EchangeurAval_in	smallint = null
as
	declare	@vl_autoroute1	tinyint,
		@vl_autoroute2	tinyint,
		@vl_PR1		int,
		@vl_PR2		int,
		@vl_nb_etapes	smallint,
		@vl_nom_auto	T_NOM,
		@vl_sens	tinyint,
		@vl_inverse	int,
		@vl_Status	int

	/*B Bug applix null */
	if @va_Hauteur_in = 0 select @va_Hauteur_in = null
	if @va_AutorouteAmont_in = 0 select @va_AutorouteAmont_in = null
	if @va_AutorouteAval_in = 0 select @va_AutorouteAval_in = null
	if @va_EchangeurAmont_in = 0 select @va_EchangeurAmont_in = null
	if @va_EchangeurAval_in = 0 select @va_EchangeurAval_in = null


	/*A
	** Determiner les parametres d'entree utilisables,
	** controler la validite des parametres obligatoires
	*/

	if (@va_AutorouteAmont_in <> null and @va_PR_Amont_in <> XDC_PR_INCONNU)
	begin
		select	@vl_autoroute1 = @va_AutorouteAmont_in,
			@vl_PR1 = @va_PR_Amont_in
	end

	if (@va_AutorouteAval_in <> null and @va_PR_Aval_in <> XDC_PR_INCONNU)
	begin
		select	@vl_autoroute2 = @va_AutorouteAval_in,
			@vl_PR2 = @va_PR_Aval_in
	end


	if	@va_Hauteur_in = null or
		(@vl_autoroute1 = null and @va_EchangeurAmont_in = null) or
		(@vl_autoroute2 = null and @va_EchangeurAval_in = null)

	return XDC_ARG_INV


	/*A
	** Rechercher l'autoroute et le PR des echangeurs si necessaire
	*/

	if @vl_autoroute1 = null
	begin
		select	@vl_autoroute1 = POR.autoroute,
			@vl_PR1 = ECH.PR
		from	CFG..RES_POR POR,
			CFG..RES_ECH ECH
		where	ECH.numero = @va_EchangeurAmont_in and
			ECH.portion = POR.numero
	end

	if @vl_autoroute2 = null
	begin
		select	@vl_autoroute2 = POR.autoroute,
			@vl_PR2 = ECH.PR
		from	CFG..RES_POR POR,
			CFG..RES_ECH ECH
		where	ECH.numero = @va_EchangeurAval_in and
			ECH.portion = POR.numero
	end

	if (@vl_autoroute1 = null or @vl_autoroute2 = null)
		return XDC_NOK


	/*A
	** Decomposer l'itineraire en etapes sur une meme autoroute
	*/

	exec @vl_Status = XZAO02 @vl_autoroute1,
	                         @vl_PR1,
	                         @vl_autoroute2,
	                         @vl_PR2,
	                         XDC_OUI,
	                         @vl_nb_etapes output
	if @vl_Status != XDC_OK
		return @vl_Status
	if @vl_nb_etapes = 0
		return XDC_ACT_REFUSEE


	/*A
	** Creer une table temporaire pour stocker les ouvrages
	*/

	create table #TEMPO_OUVRAGES (
		nom_ouvrage	char(25),
		gabarit		smallint,
		pr		int,
		nom_autoroute	char(4),
	)


	/*A
	** Parcourir l'itineraire etape par etape
	*/

	declare Pointeur_Etape cursor for
		select	autoroute, pr_debut, pr_fin
		  from LISTE_ETAPES
		 where spid = @@spid
		 order by etape
	open Pointeur_Etape
	fetch Pointeur_Etape into @vl_autoroute1, @vl_PR1, @vl_PR2

	while (@@sqlstatus = 0)
	begin
		/*A
		** Rechercher le nom de l'autoroute et si son sens est inverse
		*/

		select @vl_nom_auto = rtrim (nom)
		  from CFG..RES_AUT
		 where numero = @vl_autoroute1

		select @vl_inverse = inversee
		  from CFG..RES_AUT_INV
		 where numero = @vl_autoroute1


		/*A
		** Determiner le sens de parcours de cette etape
		*/

		if (@vl_inverse * @vl_PR1) > (@vl_inverse * @vl_PR2)
			select @vl_sens = XDC_SENS_NORD
		else
			select @vl_sens = XDC_SENS_SUD


		/*A
		** Rechercher les ouvrages d'art et les tunnels dans ce sens
		*/

		--print "Etape: sur %1!, de %2! a %3!, sens %4!", @vl_nom_auto, @vl_PR1, @vl_PR2, @vl_sens

		/*A Cas sens sud normal */

		if	@vl_sens=XDC_SENS_SUD and @vl_inverse=XDC_PR_NOR
		begin
			insert #TEMPO_OUVRAGES
			select	SUP.nom,SUP.gabarit_sens1,pr=SUP.PR,@vl_nom_auto
			from	CFG..RES_SUP SUP,
				CFG..RES_POR POR
			where	POR.autoroute=@vl_autoroute1 and
				POR.numero=SUP.portion and
				SUP.PR>@vl_PR1 and
				SUP.PR<@vl_PR2 and
				SUP.gabarit_sens1<=@va_Hauteur_in and
				SUP.gabarit_sens1<>0
			union
			select	SEG.nom_tunnel,SEG.gabarit,pr=SEG.PR_debut,@vl_nom_auto
			from	CFG..RES_SEG SEG,
				CFG..RES_POR POR
			where	POR.autoroute=@vl_autoroute1 and
				POR.numero=SEG.portion and
				SEG.sens=@vl_sens and
				SEG.PR_debut>@vl_PR1 and
				SEG.PR_debut<@vl_PR2 and
				SEG.tunnel=XDC_OUI and
				SEG.gabarit<=@va_Hauteur_in and
				SEG.gabarit<>0
			order by pr asc
		end

		/*A Cas sens nord normal */

		else
		if	@vl_sens=XDC_SENS_NORD and @vl_inverse=XDC_PR_NOR
		begin
			insert #TEMPO_OUVRAGES
			select	SUP.nom,SUP.gabarit_sens2,pr=SUP.PR,@vl_nom_auto
			from	CFG..RES_SUP SUP,
				CFG..RES_POR POR
			where	POR.autoroute=@vl_autoroute1 and
				POR.numero=SUP.portion and
				SUP.PR<@vl_PR1 and
				SUP.PR>@vl_PR2 and
				SUP.gabarit_sens2<=@va_Hauteur_in and
				SUP.gabarit_sens2<>0
			union
			select	SEG.nom_tunnel,SEG.gabarit,pr=SEG.PR_debut,@vl_nom_auto
			from	CFG..RES_SEG SEG,
				CFG..RES_POR POR
			where	POR.autoroute=@vl_autoroute1 and
				POR.numero=SEG.portion and
				SEG.sens=@vl_sens and
				SEG.PR_debut<@vl_PR1 and
				SEG.PR_debut>@vl_PR2 and
				SEG.tunnel=XDC_OUI and
				SEG.gabarit<=@va_Hauteur_in and
				SEG.gabarit<>0
			order by pr desc
		end

		/*A Cas sens sud inverse */

		else
		if	@vl_sens=XDC_SENS_SUD
		begin
			insert #TEMPO_OUVRAGES
			select	SUP.nom,SUP.gabarit_sens1,pr=SUP.PR,@vl_nom_auto
			from	CFG..RES_SUP SUP,
				CFG..RES_POR POR
			where	POR.autoroute=@vl_autoroute1 and
				POR.numero=SUP.portion and
				SUP.PR<@vl_PR1 and
				SUP.PR>@vl_PR2 and
				SUP.gabarit_sens1<=@va_Hauteur_in and
				SUP.gabarit_sens1<>0
			union
			select	SEG.nom_tunnel,SEG.gabarit,pr=SEG.PR_debut,@vl_nom_auto
			from	CFG..RES_SEG SEG,
				CFG..RES_POR POR
			where	POR.autoroute=@vl_autoroute1 and
				POR.numero=SEG.portion and
				SEG.sens=@vl_sens and
				SEG.PR_debut<@vl_PR1 and
				SEG.PR_debut>@vl_PR2 and
				SEG.tunnel=XDC_OUI and
				SEG.gabarit<=@va_Hauteur_in and
				SEG.gabarit<>0
			order by pr desc
		end

		/*A Cas sens nord inverse */

		else
		begin
			insert #TEMPO_OUVRAGES
			select	SUP.nom,SUP.gabarit_sens2,pr=SUP.PR,@vl_nom_auto
			from	CFG..RES_SUP SUP,
				CFG..RES_POR POR
			where	POR.autoroute=@vl_autoroute1 and
				POR.numero=SUP.portion and
				SUP.PR>@vl_PR1 and
				SUP.PR<@vl_PR2 and
				SUP.gabarit_sens2<=@va_Hauteur_in and
				SUP.gabarit_sens2<>0
			union
			select	SEG.nom_tunnel,SEG.gabarit,pr=SEG.PR_debut,@vl_nom_auto
			from	CFG..RES_SEG SEG,
				CFG..RES_POR POR
			where	POR.autoroute=@vl_autoroute1 and
				POR.numero=SEG.portion and
				SEG.sens=@vl_sens and
				SEG.PR_debut>@vl_PR1 and
				SEG.PR_debut<@vl_PR2 and
				SEG.tunnel=XDC_OUI and
				SEG.gabarit<=@va_Hauteur_in and
				SEG.gabarit<>0
			order by pr asc
		end

		fetch Pointeur_Etape into @vl_autoroute1, @vl_PR1, @vl_PR2
	end
	close Pointeur_Etape


	/*A
	** Lorsque les resultats sont obtenus,
	** supprimer les donnees d'itineraire
	** DE CE PROCESSUS dans la table interne
	*/

	delete LISTE_ETAPES
	 where spid = @@spid


	/*A
	** Retourner les ouvrages dans l'ordre ou il ont ete obtenus
	*/

	select * from #TEMPO_OUVRAGES

	return XDC_OK
go
