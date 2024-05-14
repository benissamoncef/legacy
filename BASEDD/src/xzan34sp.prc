/*E*/
/*Fichier : $Id: xzan34sp.prc,v 1.7 2020/11/09 14:14:02 devgtie Exp $      Release : $Revision: 1.7 $        Date : $Date: 2020/11/09 14:14:02 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAN * FICHIER xzan34sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Interface Annuaire >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	26 Oct 1994	: Creation
* gaborit	03 dec 1994	: modif (bouche les trous eventuels)
* Guilhou	03 dec 1996	: refonte de la gestion des astreintes (DEM/1306) 1.3
* Guilhou	21 jan 1997 : correction delete (DEM/1385) 1.4
* Guilhou	13 jan 1997	: suppression lignes inutiles 1.5
* LCL		04/07/17	: Modif PRA
* RGR		09/11/2020	: Correction Pb de compilation
* GGY		26/09/23	: Ajout district CA (DEM483)
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzan34sp.h" */

/* static char *version = "@(#)xzan34.c	1.1 27/10/94 : xzan34" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Supprime les fiches perimees
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAN;34
   as 

/* 
*
* SP	XZAN34SP
*
* PARAMETRES EN ENTREE :
*
* PARAMETRES EN SORTIE :
*
* VALEUR RENDUE :
*
* Supprime les fiches perimees et en cree de nouvelles pour j + 400 jours
* 
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK 
*
* CONDITIONS D'UTILISATION :
*
* Appele par timer
* 
* FONCTION :
*
* MECANISMES :
*
------------------------------------------------------*/
declare

      /*A
      ** Declaration des variables locales
      */
    
      @vl_Numero	smallint,
      @vl_Bidon		int,
      @vl_CptBoucle	int,
      @vl_Date		datetime,
      @vl_DateV		datetime,
      @vl_Site		tinyint
  
	/* Verification que le site est bien celui du CI ou CA */
	select  @vl_Site = numero
		from CFG..RES_DIS
		where serveur = @@servername and type = XDC_TYPEM_SD

	/* Si le site est CI ou CA */
	if @@rowcount = 1
	begin
		delete CFG..ANN_AST_CAL
			where date_fin <  dateadd(month, -1, getdate ())
   	end
    
	return ( XDC_OK )
     
go  
