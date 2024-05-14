/*E*/
/* Fichier : $Id: xzao502sp.prc,v 1.1 2021/06/03 07:08:36 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2021/06/03 07:08:36 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao502sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
* ABE	11/02/21 : Création SONO DEM-SAE244 1.1
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao502sp.h" */

/* static char *version = "@(#)xzao502sp.prc	1.1 1.1 28/07/20: xzao502" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Lecture des zone de sonorisation
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAO502' and type = 'P')
	drop procedure XZAO502
go

create procedure XZAO502
	   @va_Basedd_in                char(3)
   as 

/* 
*
* SP	XZAO502SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Entier	va_Basedd_in	
*	
*
* PARAMETRES EN SORTIE :
*
* VALEUR RENDUE :
*
* Retourne les infos de la zone de sonoristaion
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
* Retourne les parametres id, libelle, autoroute, sens, pr debut et pr fin
*
* MECANISMES :
*
*/
      
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_ARG_INV 
   */
      
   if @va_Basedd_in = null 
      return XDC_ARG_INV
   
   else   
   
   begin
      
      /*A
      **  Travail dans la base CFG
      */   
      
      if @va_Basedd_in = XDC_BASE_CFG
      begin
            /*A
            ** Recherche de la liste des RAU
            */
           select distinct id,
			libelle,
    			autoroute,
    			sens,
    			PR_debut,
   			PR_fin
			   
           from     CFG..ZON_SONO
      end
   end
      
#ifdef CI
   
   /*A
   **  Travail dans la base CFT
   */ 

   if @va_Basedd_in = XDC_BASE_CFT      
   begin
         /*A
         ** Recherche de la liste des RAU
         */
         
         select distinct id,
			libelle,
    			autoroute,
    			sens,
    			PR_debut,
   			PR_fin

	from     CFT..ZON_SONO
   end

#endif      
     
   return XDC_OK
         
go  
