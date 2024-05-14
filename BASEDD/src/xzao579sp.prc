/*E*/
/* Fichier : $Id: xzao579sp.prc,v 1.1 2012/07/04 11:34:54 pc2dpdy Exp $        Release : $Revision: 1.1 $        Date : $Date: 2012/07/04 11:34:54 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzao579sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* MG		25 Avril 2012 : Creation (DEM/1016)
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao579sp.h" */

/* static char *version = "@(#)xzao579.c	1.1 23/18/94 : xzao579" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAO579' and type = 'P')
	drop procedure XZAO579
go

	create procedure XZAO579
	   @va_Basedd_in         	     char(3),
	   @va_NumeroCondition_in		 smallint,
	   @va_NumeroScen_in		     smallint
	   
	   as 

	   
/* 
*
* SP    XZAO579SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd        va_Basedd_in
*
* PARAMETRES EN SORTIE :
*
*     

* CODE RETOUR :
*
* 0    XDC_OK
* 1    XDC_NOK 
*
*
* FONCTION :
*
* Suppression d'une condition dans REG_CONDITION
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* MG    25 Avril 2012    : Creation (DEM/1016)
------------------------------------------------------*/
   
  if @va_Basedd_in             = null or
     @va_NumeroCondition_in    = null or
	 @va_NumeroScen_in         = null
        return XDC_ARG_INV
		
       
  if @va_Basedd_in = XDC_BASE_CFG
  begin
  
	delete CFG..REG_CON where
		numero = @va_NumeroCondition_in and 
		scenario = @va_NumeroScen_in
	return XDC_OK 
  end
		
	#ifdef  CI 
            else
			begin
			delete CFT..REG_CON where
				numero = @va_NumeroCondition_in and 
				scenario = @va_NumeroScen_in
			return XDC_OK
            end
	#endif
      
    return XDC_OK
         
go  
