/*E*/
/*Fichier : $Id: xzao74bsp.prc,v 1.5 2020/11/03 17:04:18 pc2dpdy Exp $      Release : $Revision: 1.5 $        Date : $Date: 2020/11/03 17:04:18 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao74bsp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao74sp.h" */

/* static char *version = "@(#)xzao74.c	1.1 23/18/94 : xzao74" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Liste des types PMV
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAO74' and type = 'P')
	drop procedure XZAO74
go

create procedure XZAO74
   @va_Basedd_in        	char(3)
   as 

/* 
*
* SP	XZAO74SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Entier	va_Basedd_in		
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf	   va_Resultat_out	
*
* VALEUR RENDUE :
*
* Retourne la liste des types de PMV
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
* Retourne les parametres numero de type, nombre de lignes, nombre de caracteres
* presence flash, presence temperature
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	06 Oct 1994	: Creation
* volcic	09 Nov 1994	: Ajout #ifdef CI (v1.2)
* LCL		O2/10/2018	: Suppression SAGA DEM1306 1.3 
* JPC    15/03/2023	: Conversion en degroupee
------------------------------------------------------*/
   
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_ARG_INV   
   */
      
   if @va_Basedd_in = null 
      
      return XDC_ARG_INV
   
   else   
   
   /*A
   ** Les parametres d'entree ne sont pas nuls
   */
   
   begin
      
      /*A
      **  Travail dans la base CFG
      */   
      
      if @va_Basedd_in = XDC_BASE_CFG
         
         select numero, lignes, caracteres, flash, temperature, picto from CFG..TYP_PMV   
      
#ifdef CI

      else
      
      /*A
      ** Travail dans la base CFT
      */
         
         select numero, lignes, caracteres, flash, temperature, picto from CFT..TYP_PMV   
            
#endif

   end
      
   return XDC_OK
         
go  
