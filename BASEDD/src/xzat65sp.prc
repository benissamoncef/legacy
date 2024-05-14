/*E*/
/*  Fichier : $Id: xzat65sp.prc,v 1.5 2019/03/12 12:40:18 devgfi Exp $        $Revision: 1.5 $        $Date: 2019/03/12 12:40:18 $
-------------------------------------------------------------------------------------------
* ESCOTA * PROJET MIGRAZUR
-------------------------------------------------------------------------------------------
* SOUS-SYSTEME : BASEDD
-------------------------------------------------------------------------------------------
* FICHIER xzat65sp.prc
-------------------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Liste des mises au neutre programmées des PMVA.
*
-------------------------------------------------------------------------------------------
* HISTORIQUE :
*
* JPL	27/02/13 : Creation (DEM 1061)  1.1
* JPL	06/03/13 : Ajout lignes alternat; ordre des lignes retournees  1.2
* JPL	20/03/13 : Ne pas retourner les mises au neutre dont la Fmc est cloturee  1.3
* JPL	21/07/17 : Support de l'indicateur de mode clignotant / alternat (DEM 1230)  1.4
* JPL	03/12/18 : Retour des données d'affichage horaire et discontinu (DEM 1312)  1.5
-----------------------------------------------------------------------------------------*/


/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU
*
*  Retourne la liste des programmations de mise au neutre des PMVA d'un site donne,
*  ou de tous les sites.
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL
*
* SP	XZAT65_Liste_Neutres_PMVA
*
* PARAMETRES EN ENTREE :
*
* XDY_Site	va_Site_in	: Numero du site
*
*
* PARAMETRES EN SORTIE :
*
* Aucun
*
* CODE RETOUR :
*
* XDC_OK
* XDC_NOK           
* XDC_ARG_INV       : parametre d'entree obligatoire manquant (Null) ou invalide
* <0                : erreur sybase
*
* CONDITIONS D'UTILISATION :
*
* FONCTION :
*
*  Retourne le contenu de la table PRG_PMA en filtrant les equipements
*  au moyen du numero de site.
*
-----------------------------------------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAT65' and type = 'P')
	drop procedure XZAT65
go


create procedure XZAT65
	@va_Site_in	T_SITE		= null
as

	/* Transformation des parametres d'entree nulls */
	if @va_Site_in = 0	select @va_Site_in = null


	select	FMC.numero,
		FMC.cle,
		FMC.type,

		PRG.numero,
		PRG.debut,
		PRG.fin,
		PRG.ligne_1,
		PRG.ligne_2,
		PRG.ligne_3,
		PRG.ligne_4,
		PRG.ligne_5,
		PRG.ligne_1_alternat,
		PRG.ligne_2_alternat,
		PRG.ligne_3_alternat,
		PRG.ligne_4_alternat,
		PRG.ligne_5_alternat,
		PRG.clignotement,
		PRG.type_jour,
		PRG.discontinu

	from	EXP..PRG_PMA PRG,
		CFG..EQT_GEN EQT,
		EXP..FMC_GEN FMC

	where	EQT.type = XDC_EQT_PMVA
	  and	EQT.numero = PRG.numero
	  and	( @va_Site_in is null  or  EQT.site_gestion = @va_Site_in )

	  and	FMC.numero = PRG.evenement
	  and	FMC.cle = PRG.cle
	  and	FMC.cloture is null

	order by EQT.site_gestion, FMC.numero, EQT.autoroute, EQT.PR desc

	return XDC_OK
go
