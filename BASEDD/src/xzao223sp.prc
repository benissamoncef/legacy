/*E*/
/*Fichier : $Id: xzao223sp.prc,v 1.1 2007/02/08 17:32:51 gesconf Exp $      Release : $Revision: 1.1 $        Date : $Date: 2007/02/08 17:32:51 $
-------------------------------------------------------------------------------
* ESCOTA *  PROJET PASTRE
-------------------------------------------------------------------------------
* SOUS-SYSTEME  BASEDD
-------------------------------------------------------------------------------
* MODULE XZAO * FICHIER xzao223sp.prc
-------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
-----------------------------------------------------------------------------*/



/*X----------------------------------------------------------------------------
* SERVICE RENDU :
*
*  Liste des Autoroutes pour un disctrict donné.
*
-------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

drop procedure  XZAO223
go

create procedure XZAO223
   @va_District_in        	tinyint
   as

/*
*
* SP	XZAO223SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Entier	va_District_in
*
* PARAMETRES EN SORTIE :
*
* VALEUR RENDUE :
*
*  Liste des Autoroutes du district de numéro va_District_in.
*
*  Remarque : si va_District_in = numéro CI, la liste retournée est vide.
*
*
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK
*
* CONDITIONS D'UTILISATION :
*
* FONCTION :
*
* MECANISMES :
*
-------------------------------------------------------------------------------
* HISTORIQUE :
*
* JPL	07/02/2007	: Creation
-----------------------------------------------------------------------------*/

   /*A
   ** Test des paramètres d'entrée :
   **   Si un des paramètres d'entrée est égal a null alors XDC_NOK
   */

   if @va_District_in = null

      return XDC_ARG_INV

   else

   begin

      /*A
      ** Recherche des autoroutes (et leurs informations complémentaires)
      ** dont une portion se situe dans le district
      **               ou l'a pour district de recouvrement
      */

      select distinct CFG..RES_AUT.numero, CFG..RES_AUT.nom, CFG..RES_AUT_INV.inversee, CFG..RES_AUT_INV.escota
      from CFG..RES_POR, CFG..RES_AUT, CFG..RES_AUT_INV
      where (CFG..RES_POR.district = @va_District_in  or
             CFG..RES_POR.district_recouvrement = @va_District_in)
        and CFG..RES_POR.autoroute = CFG..RES_AUT.numero
        and CFG..RES_POR.autoroute = CFG..RES_AUT_INV.numero

   end

   return XDC_OK

go
