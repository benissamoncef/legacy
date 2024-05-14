/*E*/
/*  Fichier : $Id: xzat75sp.prc,v 1.1 2018/03/26 18:35:18 devgfi Exp $        $Revision: 1.1 $        $Date: 2018/03/26 18:35:18 $
-------------------------------------------------------------------------------------------
* ESCOTA  *  PROJET MIGRAZUR
-------------------------------------------------------------------------------------------
* SOUS-SYSTEME : BASEDD
-------------------------------------------------------------------------------------------
* FICHIER xzat75sp.prc
-------------------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Liste des mises au neutre programmées des PMVA SAGA.
*
-------------------------------------------------------------------------------------------
* HISTORIQUE :
*
* JPL	26/03/18 : Creation (DEM 1278)  1.1
-----------------------------------------------------------------------------------------*/


/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU
*
*  Retourne la liste des programmations de mise au neutre des PMVA SAGA.
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL
*
* SP	XZAT75_Liste_Neutres_PMVA_SAGA
*
* PARAMETRES EN ENTREE :
*
* Aucun
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
*  Retourne le contenu de la table PRG_PMA_SAGA.
*
-----------------------------------------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAT75' and type = 'P')
	drop procedure XZAT75
go


create procedure XZAT75
as
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
		PRG.clignotement

	from	EXP..PRG_PMA_SAGA PRG,
		CFG..EQT_GEN EQT,
		EXP..FMC_GEN FMC

	where	EQT.type = XDC_EQT_SAGA
	  and	EQT.numero = PRG.numero

	  and	FMC.numero = PRG.evenement
	  and	FMC.cle = PRG.cle
	  and	FMC.cloture is null

	order by FMC.numero, EQT.autoroute, EQT.PR desc

	return XDC_OK
go
