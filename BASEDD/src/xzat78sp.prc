/*E*/
/*  Fichier : $Id: xzat78sp.prc,v 1.1 2018/03/26 18:35:45 devgfi Exp $        $Revision: 1.1 $        $Date: 2018/03/26 18:35:45 $
-------------------------------------------------------------------------------------------
* ESCOTA  *  PROJET MIGRAZUR
-------------------------------------------------------------------------------------------
* SOUS-SYSTEME : BASEDD
-------------------------------------------------------------------------------------------
* FICHIER xzat78sp.prc
-------------------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Mise au neutre programmée a la date courante d'un PMVA SAGA.
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
*  Retourne les donnees de mise au neutre programmee d'un PMVA SAGA donne
*  a l'horodate courante.
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL
*
* SP	XZAT78_Neutre_Programme_PMVA_SAGA
*
* PARAMETRES EN ENTREE :
*
* XDY_Eqt	va_NumEqt_in	: Numero du PMVA SAGA
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
*  Si une mise au neutre programmee existe pour le PMVA SAGA a la date courante,
*  retourne les donnees et les dates de la FMC attachee.
*
-----------------------------------------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAT78' and type = 'P')
	drop procedure XZAT78
go


create procedure XZAT78
	@va_NumEqt_in	T_EQUIPEMENT	= null
as

	/* Transformation des parametres d'entree nulls */
	if @va_NumEqt_in = 0	select @va_NumEqt_in = null


	select	NUMERO_EQT	= PRG.numero,
				0,

		NUMERO_FMC	= FMC.numero,
		CLE_FMC		= FMC.cle,
		TYPE_FMC	= FMC.type,
		DEBUT_FMC	= FMC.debut,
		FIN_FMC		= FMC.fin,

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
		EXP..FMC_GEN FMC

	where	PRG.numero = @va_NumEqt_in
	  and	getdate () between PRG.debut and PRG.fin
	  and	FMC.numero = PRG.evenement  and  FMC.cle = PRG.cle

	return XDC_OK
go
