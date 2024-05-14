/*E*/
/* Fichier : $Id: xzao571sp.prc,v 1.1 2012/07/04 11:33:49 pc2dpdy Exp $        Release : $Revision: 1.1 $        Date : $Date: 2012/07/04 11:33:49 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzao571sp.prc
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

/* #include "xzao571sp.h" */

/* static char *version = "@(#)xzao571.c	1.1 23/18/94 : xzao571" ; */

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

if exists (select * from sysobjects where name = 'XZAO571' and type = 'P')
	drop procedure XZAO571
go

	create procedure XZAO571
	   @va_Basedd_in         	     char(3),
	   @va_NumeroScen_in		     smallint,
	   @va_numZone_in		     smallint
	   
	   as 

	   
/* 
*
* SP    XZAO571SP
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
* Suppression d'une condition dans REG_CON
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* MG    25 Avril 2012    : Creation (DEM/1016)
------------------------------------------------------*/
   
  if @va_Basedd_in             = null or
	 @va_NumeroScen_in         = null or
	@va_numZone_in 		= null
        return XDC_ARG_INV
		
       
  if @va_Basedd_in = XDC_BASE_CFG
  begin
  
	delete CFG..REG_ASS where
		scenario = @va_NumeroScen_in and
		zone = @va_numZone_in

	delete CFG..REG_SCE_CMD where
		scenario = @va_NumeroScen_in

	delete CFG..REG_SCE where
		numero = @va_NumeroScen_in and
		zone = @va_numZone_in

	return XDC_OK 
  end
		
	#ifdef  CI 
            else
		begin
			delete CFT..REG_ASS where
				scenario = @va_NumeroScen_in and
				zone = @va_numZone_in

			delete CFT..REG_SCE_CMD where
				scenario = @va_NumeroScen_in

			delete CFT..REG_SCE where
				numero = @va_NumeroScen_in and
				zone = @va_numZone_in
				
			return XDC_OK
            end
	#endif
      
    return XDC_OK
         
go  
