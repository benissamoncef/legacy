/*E*/
/*Fichier : $Id: xzao781sp.prc,v 1.1 2017/03/31 17:49:51 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2017/03/31 17:49:51 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao781sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
* JMG   20/03/17        creation DEM1191
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao781sp.h" */

/* static char *version = "@(#)xzao781.c	1.1 23/18/94 : xzao781" ; */

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

drop procedure XZAO781
go

create procedure XZAO781
   @va_Basedd_in        	char(3)
   as 

/* 
*
* SP	XZAO781SP
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
* JMG	08/06/16	 creation 1.1
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
         
         select numero, 
		type_saga,
		instance,
		fels_code,
		eqt_code,
		code_cmd_message,
		code_cmd_luminosite,
		code_cmd_heure
	 from CFG..EQT_SAGA   
      
#ifdef CI

      else
      
      /*A
      ** Travail dans la base CFT
      */
         
         select numero, 
		type_saga,
		instance,
		fels_code,
		eqt_code,
		code_cmd_message,
		code_cmd_luminosite,
		code_cmd_heure
	 from CFT..EQT_SAGA   
      
            
#endif

   end
      
   return XDC_OK
         
go  
