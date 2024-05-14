/*E*/
/*  Fichier : $Id: vuem08sp.prc,v 1.17 2017/03/09 15:41:47 pc2dpdy Exp $        Release : $Revision: 1.17 $        Date : $Date: 2017/03/09 15:41:47 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER vuem08sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* vues metiers
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	19/07/95	: Creation
* C.T.	24/08/95	: Modif interface (V 1.2)
* C.T.	07/06/96	: Ajout retour du pictogramme (V 1.3)
* C.T.	17/09/96	: Remplacer le sens par le libelle sens (ANA48 V 1.4)
* V.L.	31/10/96	: Ajout des sens pour les affichages en gare (V 1.5)
* C.P.  13/11/97	: Modification requette pour un seul mois (V1.6)
* C.T.	24/02/98	: Modif suite pb default suite a passage en SQL 11.0.3.2 (V1.7)
* P.N.	05/02/99	: Modif interg. dem/1724 v1.8
* JMG	21/09/99	: action sur remorque : l'autoroute est directement dans la table
* JPL	29/09/10	: ... uniquement pour les actions sur remorque (et non sur fourgon) 1.10
* JPL	22/08/12	: Abstraction totale de FMC_HIS, constantes, controles,  format des dates  1.11
* JPL	24/08/12	: Support type PMVA; ajout colonnes identifiant la FMC (DEM 1049)  1.12
* JPL	18/09/12	: Date de fin incluse ; support filtre par secteurs (DEM 1049)  1.13
* JPL	25/09/12	: Precision de la duree en secondes  1.14
* JPL	18/07/16	: Generalisation de l'usage de table de sites temporaire; identique precedente  1.15
* JPL	19/07/16	: Support regions pour criteres geographiques; compatible precedente (DEM 1171)  1.16
* JMG   09/03/17 	: REGIONALISATION - DEM1220 1.17
* LCL	28/09/23 	: Adaptations CNA DEM-483	
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* SP	VUEM08_Liste_Affichage_PMV
* 
* Arguments en entree
* XDY_Horodate	va_Debut_in	: horodate de debut
* XDY_Horodate	va_Fin_in	: horodate de fin
* XDY_Octet	va_Type_in
* char(30)	va_Secteurs_in	: liste de secteurs de recherche
* XDY_Octet	va_Datex_in	: recherche d'evts. intergestionnaires
* 
* Arguments en sortie
*
* liste retournee
* XDY_Nom	district
* XDY_Nom	type_cause
* XDC_Entier	numero FMC
* XDC_Octet	cle FMC
* XDC_Horodate	date succes
* XDC_Horodate	date fin
* XDC_Entier	duree
* XDC_Autoroute	autoroute
* XDC_PR	PR
* XDY_Texte	sens
* XDY_Texte	message
* char(4)	picto
* 
* Service rendu
* Liste des affichages PMV, PMV sur fourgon ou remorque,
* sur une periode donnee, par secteur et tries chronologiquement.
* 
* Code retour
* XDC_OK
* XDC_ARG_INV	: parametres d'entree null ou site incorrect
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* 
* Fonction
-----------------------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'VUEM08' and type = 'P')
	drop procedure VUEM08
go


create procedure VUEM08
	@va_Debut_in		datetime	= null,
	@va_Fin_in		datetime	= null,
	@va_Type_in		tinyint,
	@va_Secteurs_in		char(30)	= null,
	@va_Datex_in		tinyint		= null
as

declare	@vl_NomSite		T_NOM,
	@vl_CodesSites		varchar(50)

	/*A controle des parametres d'entree */
	if (@va_Debut_in is null or @va_Debut_in = XDC_DATE_NULLE) or
	   (@va_Fin_in is null or @va_Fin_in = XDC_DATE_NULLE)
		return XDC_ARG_INV

	if @va_Type_in not in ( XDC_ACT_PMV, XDC_ACT_PMVA, XDC_ACT_PMV_REMORQUE, XDC_ACT_PMV_FOURGON, XDC_ACT_AFF_GARE )
		return XDC_ARG_INV


	/*A Analyser la liste des sites de recherche et renseigner une table temporaire */
	create table #LISTE_SITES (
		numero		tinyint		not null,
		nom		char(25)	not null
	)

	select @vl_CodesSites = ' ' + ltrim (rtrim (@va_Secteurs_in)) + ' '
	/*A Si tous les secteurs sont requis, prendre le resultat de XZAO30 sauf le CI */
	if upper (@vl_CodesSites) like "% TOUS %"
	begin
		insert #LISTE_SITES (numero, nom)
		select numero, nom from CFG..RES_DIS where type <> 0 and numero <> XDC_CI
	end
	else
	begin
		insert #LISTE_SITES (numero, nom)
		select numero, nom from CFG..RES_DIS where @vl_CodesSites like "% " + code + " %"
	end


	if @va_Type_in = XDC_ACT_PMV
	begin
		/*B liste des affichages PMV dans la periode */
		select	#LISTE_SITES.nom,
			HIS..FMC_GEN.numero,
			HIS..FMC_GEN.cle,
			CFG..TYP_FMC.nom,
			convert(char(10), heure_succes, 103) + ' ' + convert(char(8), heure_succes, 8),
			convert(char(10), heure_fin, 103) + ' ' + convert(char(8), heure_fin, 8),
			datediff(second, HIS..ACT_GEN.heure_succes, HIS..ACT_GEN.heure_fin),
			CFG..RES_AUT.nom,
			CFG..EQT_GEN.PR,
			CFG..LIB_PRE.libelle,
			premiere_ligne + '|' + deuxieme_ligne + '|' + troisieme_ligne + '|' +
			   premiere_ligne_alternat + '|' + deuxieme_ligne_alternat + '|' + troisieme_ligne_alternat,
			pictogramme

		from	HIS..ACT_GEN, CFG..EQT_GEN,  #LISTE_SITES,
			HIS..FMC_GEN,
			HIS..ACT_PMV,
			CFG..TYP_FMC, CFG..RES_AUT, CFG..LIB_PRE

		where	HIS..ACT_GEN.type = XDC_ACT_PMV and

			CFG..EQT_GEN.type = XDC_EQT_PMV and
			CFG..EQT_GEN.numero = HIS..ACT_GEN.equipement and
			CFG..EQT_GEN.site_gestion = #LISTE_SITES.numero and

			heure_succes >= @va_Debut_in and heure_succes <= @va_Fin_in and
			heure_fin is not null and

			HIS..FMC_GEN.numero = HIS..ACT_GEN.evenement and
			HIS..FMC_GEN.cle = HIS..ACT_GEN.cle and
		((@va_Datex_in=XDC_VRAI and HIS..FMC_GEN.datex=XDC_VRAI) or
		 (@va_Datex_in<>XDC_VRAI and HIS..FMC_GEN.datex<>XDC_VRAI)) and

			HIS..ACT_GEN.numero = HIS..ACT_PMV.actio and
			HIS..ACT_GEN.sit = HIS..ACT_PMV.sit and

			CFG..TYP_FMC.numero = HIS..FMC_GEN.type and
			CFG..RES_AUT.numero = CFG..EQT_GEN.autoroute and
			CFG..LIB_PRE.notyp = XDC_LIB_SENS_LOC  and  CFG..LIB_PRE.code = CFG..EQT_GEN.sens
		order by #LISTE_SITES.numero, heure_succes, heure_fin
			 --CFG..RES_AUT.nom, CFG..EQT_GEN.PR, CFG..EQT_GEN.sens
	end



	if @va_Type_in = XDC_ACT_PMVA
	begin
		/*B liste des affichages PMVA dans la periode */
		select	#LISTE_SITES.nom,
			HIS..FMC_GEN.numero,
			HIS..FMC_GEN.cle,
			CFG..TYP_FMC.nom,
			convert(char(10), heure_succes, 103) + ' ' + convert(char(8), heure_succes, 8),
			convert(char(10), heure_fin, 103) + ' ' + convert(char(8), heure_fin, 8),
			datediff(second, HIS..ACT_GEN.heure_succes, HIS..ACT_GEN.heure_fin),
			CFG..RES_AUT.nom,
			CFG..EQT_GEN.PR,
			CFG..LIB_PRE.libelle,
			premiere_ligne + '|' + deuxieme_ligne + '|' +
			   troisieme_ligne + '|' + quatrieme_ligne + '|' + cinquieme_ligne + '|' +
			   premiere_ligne_alternat + '|' + deuxieme_ligne_alternat + '|' +
			   troisieme_ligne_alternat + '|' + quatrieme_ligne_alternat + '|' + cinquieme_ligne_alternat

		from	HIS..ACT_GEN, CFG..EQT_GEN,  #LISTE_SITES,
			HIS..FMC_GEN,
			HIS..ACT_PMA,
			CFG..TYP_FMC, CFG..RES_AUT, CFG..LIB_PRE

		where	HIS..ACT_GEN.type = XDC_ACT_PMVA and

			CFG..EQT_GEN.type = XDC_EQT_PMVA and
			CFG..EQT_GEN.numero = HIS..ACT_GEN.equipement and
			CFG..EQT_GEN.site_gestion = #LISTE_SITES.numero and

			heure_succes >= @va_Debut_in and heure_succes <= @va_Fin_in and
			heure_fin is not null and

			HIS..FMC_GEN.numero = HIS..ACT_GEN.evenement and
			HIS..FMC_GEN.cle = HIS..ACT_GEN.cle and
		((@va_Datex_in=XDC_VRAI and HIS..FMC_GEN.datex=XDC_VRAI) or
		 (@va_Datex_in<>XDC_VRAI and HIS..FMC_GEN.datex<>XDC_VRAI)) and

			HIS..ACT_GEN.numero = HIS..ACT_PMA.actio and
			HIS..ACT_GEN.sit = HIS..ACT_PMA.sit and

			CFG..TYP_FMC.numero = HIS..FMC_GEN.type and
			CFG..RES_AUT.numero = CFG..EQT_GEN.autoroute and
			CFG..LIB_PRE.notyp = XDC_LIB_SENS_LOC  and  CFG..LIB_PRE.code = CFG..EQT_GEN.sens
		order by #LISTE_SITES.numero, heure_succes, heure_fin
			 --CFG..RES_AUT.nom, CFG..EQT_GEN.PR, CFG..EQT_GEN.sens
	end



	if @va_Type_in = XDC_ACT_PMV_FOURGON
	begin
		/*B liste des affichages PMV sur fourgon dans la periode */
		select 
			district = #LISTE_SITES.nom,
			HIS..FMC_GEN.numero,
			HIS..FMC_GEN.cle,
			nom_type = CFG..TYP_FMC.nom,
			heure_succes = convert(char(10), heure_succes, 103) + ' ' + convert(char(8), heure_succes, 8),
			heure_fin = convert(char(10), heure_fin, 103) + ' ' + convert(char(8), heure_fin, 8),
			duree = datediff(second, HIS..ACT_GEN.heure_succes, HIS..ACT_GEN.heure_fin),
			autoroute = CFG..RES_AUT.nom,
			HIS..ACT_FRG.PR,
			CFG..LIB_PRE.libelle,
			message

		from	HIS..ACT_GEN, HIS..FMC_GEN,
			HIS..ACT_FRG, CFG..RES_POR, /*HIS..FMC_HIS,*/  #LISTE_SITES,
			CFG..TYP_FMC, CFG..RES_AUT, CFG..LIB_PRE

		where	HIS..ACT_GEN.type = XDC_ACT_PMV_FOURGON and

			heure_succes >= @va_Debut_in and heure_succes <= @va_Fin_in and
			heure_fin is not null and

			HIS..FMC_GEN.numero = HIS..ACT_GEN.evenement and
			HIS..FMC_GEN.cle = HIS..ACT_GEN.cle and
		((@va_Datex_in=XDC_VRAI and HIS..FMC_GEN.datex=XDC_VRAI) or
		 (@va_Datex_in<>XDC_VRAI and HIS..FMC_GEN.datex<>XDC_VRAI)) and

			HIS..ACT_GEN.numero = HIS..ACT_FRG.actio and
			HIS..ACT_GEN.sit = HIS..ACT_FRG.sit and

			CFG..RES_POR.autoroute = HIS..ACT_FRG.autoroute and
			CFG..RES_POR.PR_debut <= HIS..ACT_FRG.PR and
			CFG..RES_POR.PR_fin > HIS..ACT_FRG.PR and
			CFG..RES_POR.district = #LISTE_SITES.numero and

			CFG..TYP_FMC.numero = HIS..FMC_GEN.type and
			CFG..RES_AUT.numero = HIS..ACT_FRG.autoroute and
			CFG..LIB_PRE.notyp = XDC_LIB_SENS_LOC  and  CFG..LIB_PRE.code = HIS..ACT_FRG.sens
		order by #LISTE_SITES.numero, heure_succes, heure_fin
	end



	if @va_Type_in = XDC_ACT_PMV_REMORQUE
	begin
		/*B liste des affichages PMV sur remorque dans la periode */
		select 
			district = #LISTE_SITES.nom,
			HIS..FMC_GEN.numero,
			HIS..FMC_GEN.cle,
			nom_type = CFG..TYP_FMC.nom,
			heure_succes = convert(char(10), heure_succes, 103) + ' ' + convert(char(8), heure_succes, 8),
			heure_fin = convert(char(10), heure_fin, 103) + ' ' + convert(char(8), heure_fin, 8),
			duree = datediff(second, HIS..ACT_GEN.heure_succes, HIS..ACT_GEN.heure_fin),
			autoroute = CFG..RES_AUT.nom,
			HIS..ACT_RMQ.PR,
			CFG..LIB_PRE.libelle,
			message

		from HIS..ACT_GEN, HIS..FMC_GEN,
			HIS..ACT_RMQ, CFG..RES_POR, /*HIS..FMC_HIS,*/  #LISTE_SITES,
			CFG..TYP_FMC, CFG..RES_AUT, CFG..LIB_PRE

		where	HIS..ACT_GEN.type = XDC_ACT_PMV_REMORQUE and

			heure_succes >= @va_Debut_in and heure_succes <= @va_Fin_in and
			heure_fin is not null and

			HIS..FMC_GEN.numero = HIS..ACT_GEN.evenement and
			HIS..FMC_GEN.cle = HIS..ACT_GEN.cle and
		((@va_Datex_in=XDC_VRAI and HIS..FMC_GEN.datex=XDC_VRAI) or
		 (@va_Datex_in<>XDC_VRAI and HIS..FMC_GEN.datex<>XDC_VRAI)) and

			HIS..ACT_GEN.numero = HIS..ACT_RMQ.actio and
			HIS..ACT_GEN.sit = HIS..ACT_RMQ.sit and

			CFG..RES_POR.autoroute = HIS..ACT_RMQ.autoroute and
			CFG..RES_POR.PR_debut <= HIS..ACT_RMQ.PR and
			CFG..RES_POR.PR_fin > HIS..ACT_RMQ.PR and
			CFG..RES_POR.district = #LISTE_SITES.numero and

			CFG..TYP_FMC.numero = HIS..FMC_GEN.type and
			CFG..RES_AUT.numero = HIS..ACT_RMQ.autoroute and
			CFG..LIB_PRE.notyp = XDC_LIB_SENS_LOC  and  CFG..LIB_PRE.code = HIS..ACT_RMQ.sens
		order by #LISTE_SITES.numero, heure_succes, heure_fin
	end



	if @va_Type_in = XDC_ACT_AFF_GARE
	begin
		/*B liste des affichages en gare */
		create table #RECAP_PMV (       num_district tinyint,
						district char(25),
						numero int,
						cle tinyint,
						debut datetime,
						fin datetime,
						duree int null,
						autoroute char(4) null,
						PR int null,
						sens tinyint default XDC_SENS_INCONNU,
						PR_sens1 int null,
						PR_sens2 int null,
						peage_entree_sens1 bit,
						peage_sortie_sens1 bit,
						peage_entree_sens2 bit,
						peage_sortie_sens2 bit,
						message char(250) null,
						nom_type char(25),
						gare  char(25) null )

		insert #RECAP_PMV(numero , cle , num_district, district, nom_type, debut, fin , duree,
				gare, autoroute, PR, PR_sens1, PR_sens2, peage_entree_sens1, peage_sortie_sens1,
				peage_entree_sens2, peage_sortie_sens2, message, sens)
		select 
			HIS..FMC_GEN.numero,
			HIS..FMC_GEN.cle,
			num_district = #LISTE_SITES.numero,
			district = #LISTE_SITES.nom,
			CFG..TYP_FMC.nom,
			heure_succes,
			heure_fin,
			datediff(second, HIS..ACT_GEN.heure_succes, HIS..ACT_GEN.heure_fin),
			nom_gare,
			CFG..RES_AUT.nom,
			CFG..RES_PEA.PRsens1,
			CFG..RES_PEA.PRsens1,
			CFG..RES_PEA.PRsens2,
			0,
			0,
			0,
			0,
			affichette,
			XDC_SENS_INCONNU

		from	HIS..ACT_GEN, HIS..FMC_GEN,
			HIS..ACT_GAR, CFG..RES_POR, CFG..RES_PEA,  #LISTE_SITES,
			CFG..TYP_FMC, CFG..RES_AUT

		where	HIS..ACT_GEN.type = XDC_ACT_AFF_GARE and

			heure_succes >= @va_Debut_in and heure_succes <= @va_Fin_in and
			heure_fin is not null and

			HIS..FMC_GEN.numero = HIS..ACT_GEN.evenement and
			HIS..FMC_GEN.cle = HIS..ACT_GEN.cle and
		((@va_Datex_in=XDC_VRAI and HIS..FMC_GEN.datex=XDC_VRAI) or
		 (@va_Datex_in<>XDC_VRAI and HIS..FMC_GEN.datex<>XDC_VRAI)) and

			HIS..ACT_GEN.numero = HIS..ACT_GAR.actio and
			HIS..ACT_GEN.sit = HIS..ACT_GAR.sit and

			CFG..RES_PEA.nom = HIS..ACT_GAR.nom_gare and
			CFG..RES_POR.numero = CFG..RES_PEA.portion and
			CFG..RES_POR.district = #LISTE_SITES.numero and

			CFG..TYP_FMC.numero = HIS..FMC_GEN.type and
			CFG..RES_AUT.numero = CFG..RES_POR.autoroute

		update #RECAP_PMV set PR = PR_sens2 
		where PR = 0

		/*B liste des affichages en gare d'echangeur dans la periode */
		insert #RECAP_PMV(numero, cle, num_district, district, nom_type, debut, fin , duree,
				gare, autoroute, PR, peage_entree_sens1, peage_sortie_sens1, 
				peage_entree_sens2, peage_sortie_sens2, message, sens )
		select 
			HIS..FMC_GEN.numero,
			HIS..FMC_GEN.cle,
			num_district = #LISTE_SITES.numero,
			district = #LISTE_SITES.nom,
			CFG..TYP_FMC.nom,
			heure_succes,
			heure_fin,
			datediff(second, HIS..ACT_GEN.heure_succes, HIS..ACT_GEN.heure_fin),
			nom_gare,
			CFG..RES_AUT.nom,
			CFG..RES_ECH.PR,
			CFG..RES_ECH.peage_entree_sens1,
			CFG..RES_ECH.peage_sortie_sens1,
			CFG..RES_ECH.peage_entree_sens2,
			CFG..RES_ECH.peage_sortie_sens2,
			affichette,
			XDC_SENS_INCONNU
		from	HIS..ACT_GEN, HIS..FMC_GEN,
			HIS..ACT_GAR, CFG..RES_POR, CFG..RES_ECH,  #LISTE_SITES,
			CFG..TYP_FMC, CFG..RES_AUT
		where	HIS..ACT_GEN.type = XDC_ACT_AFF_GARE and

			heure_succes >= @va_Debut_in and heure_succes <= @va_Fin_in and
			heure_fin is not null and

			HIS..FMC_GEN.numero = HIS..ACT_GEN.evenement and
			HIS..FMC_GEN.cle = HIS..ACT_GEN.cle and
		((@va_Datex_in=XDC_VRAI and HIS..FMC_GEN.datex=XDC_VRAI) or
		 (@va_Datex_in<>XDC_VRAI and HIS..FMC_GEN.datex<>XDC_VRAI)) and

			HIS..ACT_GEN.numero = HIS..ACT_GAR.actio and
			HIS..ACT_GEN.sit = HIS..ACT_GAR.sit and
			HIS..ACT_GAR.nom_gare not in (	select CFG..RES_ECH.nom from CFG..RES_PEA, CFG..RES_ECH
					where CFG..RES_PEA.nom = CFG..RES_ECH.nom) and

			CFG..RES_ECH.nom = HIS..ACT_GAR.nom_gare and
			CFG..RES_POR.numero = CFG..RES_ECH.portion and
			CFG..RES_POR.district = #LISTE_SITES.numero and

			CFG..TYP_FMC.numero = HIS..FMC_GEN.type and
			CFG..RES_AUT.numero = CFG..RES_POR.autoroute


		/*B si le sens est non precise on met le sens du peage */
		update #RECAP_PMV set sens = XDC_SENS_SUD
		where
		(PR_sens1 <> XDC_FAUX and PR_sens2 = XDC_FAUX)
		or ((peage_entree_sens1 = 1 or peage_sortie_sens1 = 1) and
		(peage_entree_sens2 = 0 and peage_sortie_sens2 = 0))


		update #RECAP_PMV set sens = XDC_SENS_NORD
		where
		(PR_sens2 <> XDC_FAUX and PR_sens1 = XDC_FAUX)
		or ((peage_entree_sens2 = 1 or peage_sortie_sens2 = 1) and
		(peage_entree_sens1 = 0 and peage_sortie_sens1 = 0))


		/*B On met le sens en accord avec le sens de l'evenement provoquant l'affichage */
		update #RECAP_PMV set sens = HIS..FMC_HIS.sens
		from HIS..FMC_HIS, #RECAP_PMV
		where #RECAP_PMV.sens = XDC_SENS_INCONNU and
		HIS..FMC_HIS.numero = #RECAP_PMV.numero and HIS..FMC_HIS.cle = #RECAP_PMV.cle


		/*A retourner la liste des infos */
		select 
			district,
			numero,
			cle,
			nom_type,
			convert(char(10), debut, 103) + ' ' + convert(char(8), debut, 8),
			convert(char(10), fin, 103) + ' ' + convert(char(8), fin, 8),
			duree,
			autoroute,
			PR,
			sens,
			message
		from #RECAP_PMV
		order by num_district, debut, fin
	end


	return XDC_OK
go
