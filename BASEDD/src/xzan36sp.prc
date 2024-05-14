/*E*/
/*Fichier : $Id: xzan36sp.prc,v 1.1 1994/11/15 12:18:10 volcic Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/15 12:18:10 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAN * FICHIER xzan36sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Annuaire >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	26 Oct 1994	:	Creation
* GGY		22/09/23	:	Ajout CFT et va_Basedd_in (DEM483)
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzan36sp.h" */

/* static char *version = "@(#)xzan36.c	1.1 23/18/94 : xzan36" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Indique si un type d'astreinte existe ou non en base
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAN;36
	@va_Basedd_in				char(3)	= null,
	@va_NumTypeAstreinte_in		tinyint	= null,
	@va_Existence_out			bit		= XDC_FAUX output
	as 

/* 
*
* SP	XZAN36SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Octet		va_NumTypeAstreinte_in
*
* PARAMETRES EN SORTIE :
*
* XDY_Booleen		va_Existence_out
*
* VALEUR RENDUE :
*
* Indique si un type d'astreinte existe ou non en base
* 
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK
* NumTypeAstreinte inconnu
*
* CONDITIONS D'UTILISATION :
*
* FONCTION :
*
* MECANISMES :
*
------------------------------------------------------*/
declare

	/* Declaration des variables locales */
	@vl_Type	tinyint

	/* Test des parametres d'entree :
	**   Si un des parametres d'entree est egal a null alors XDC_NOK */
	if @va_NumTypeAstreinte_in	= null
		or @va_Basedd_in		= null
			return ( XDC_ARG_INV )

	else
	/* Si les parametres sont non nuls */
   	begin
		if @va_Basedd_in = XDC_BASE_CFG
		begin
			/* Travail dans la base CFG */
			select @va_Existence_out = XDC_FAUX

			select @vl_Type = numero from CFG..TYP_AST
					where numero = @va_NumTypeAstreinte_in

			if @@rowcount = 1

				select @va_Existence_out = XDC_VRAI

			else

				select @va_Existence_out = XDC_FAUX
		end

		else if @va_Basedd_in = XDC_BASE_CFT
		begin
			/* Travail dans la base CFT */
			select @va_Existence_out = XDC_FAUX

			select @vl_Type = numero from CFT..TYP_AST
					where numero = @va_NumTypeAstreinte_in

			if @@rowcount = 1
				select @va_Existence_out = XDC_VRAI
			else
				select @va_Existence_out = XDC_FAUX
		end

		else
		/* Erreur sur le nom de base */
			return ( XDC_NOK )
	end

	return ( XDC_OK )

go
