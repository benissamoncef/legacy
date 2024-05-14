/*E*/
/*  Fichier : $Id: xzat70sp.prc,v 1.2 2018/04/05 10:13:45 devgfi Exp $        $Revision: 1.2 $        $Date: 2018/04/05 10:13:45 $
-------------------------------------------------------------------------------------------
* ESCOTA  *  PROJET MIGRAZUR
-------------------------------------------------------------------------------------------
* SOUS-SYSTEME : BASEDD
-------------------------------------------------------------------------------------------
* FICHIER xzat70sp.prc
-------------------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Liste des mises au neutre programmées des PMV SAGA.
*
-------------------------------------------------------------------------------------------
* HISTORIQUE :
*
* JPL	26/03/18 : Creation (DEM 1278)  1.1
* JPL	30/03/18 : Ajout du parametre de site  1.2
-----------------------------------------------------------------------------------------*/


/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU
*
*  Retourne la liste des programmations de mise au neutre des PMV SAGA.
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL
*
* SP	XZAT70_Liste_Neutres_PMV_SAGA
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
*  Retourne le contenu de la table PRG_PMV_SAGA en filtrant les equipements
*  au moyen du numero de site.
*
-----------------------------------------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAT70' and type = 'P')
	drop procedure XZAT70
go


create procedure XZAT70
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
		PRG.ligne_1_alternat,
		PRG.ligne_2_alternat,
		PRG.ligne_3_alternat,
		PRG.pictogramme,
		PRG.bandeau,
		PRG.clignotement

	from	EXP..PRG_PMV_SAGA PRG,
		CFG..EQT_GEN EQT,
		EXP..FMC_GEN FMC

	where	EQT.type = XDC_EQT_SAGA
	  and	EQT.numero = PRG.numero
	  and	( @va_Site_in is null  or  EQT.site_gestion = @va_Site_in )

	  and	FMC.numero = PRG.evenement
	  and	FMC.cle = PRG.cle
	  and	FMC.cloture is null

	order by EQT.site_gestion, FMC.numero, EQT.autoroute, EQT.PR desc

	return XDC_OK
go
