/*E*/
/* Fichier : $Id: xzao550sp.prc,v 1.1 2012/07/04 11:30:09 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2012/07/04 11:30:09 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzao550sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* VR		12/01/12 : Creation (DEM/1016)
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao550sp.h" */

/* static char *version = "@(#)xzao550.c	1.1 23/18/94 : xzao550" ; */

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

if exists (select * from sysobjects where name = 'XZAO550' and type = 'P')
	drop procedure XZAO550
go

create procedure XZAO550
   @va_Basedd_in             char(3),
   @va_Numero_in	smallint,
   @va_Type_in		tinyint,
   @va_Valeur_in	int,
   @va_Resultat_out		int output
   as 

/* 
*
* SP    XZAO550SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd        va_Basedd_in
*  					va_Numero_in
*  					va_Type_in
* 					va_Valeur_in
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf       va_Resultat_out    

* CODE RETOUR :
*
* 0    XDC_OK
* 1    XDC_NOK 
*
*
* FONCTION :
*
* Mise a jour des donnees dans la table REG_PAR
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* VR    12 Jan 2012    : Creation (DEM/1016)
------------------------------------------------------*/
declare
   
    /*A
    ** Definition des variables globales
    */

    @vl_Numero        smallint
   
   
    select @va_Resultat_out = XDC_LU
   
    /*A
    ** Test des parametres d'entree :
    **   Si un des parametres d'entree est egal a null alors XDC_NOK   
    */
      
    if @va_Basedd_in             = null or
	   @va_Numero_in             = null or 
       @va_Type_in		         = null or 
       @va_Valeur_in	         = null 
        
		return XDC_ARG_INV
   
	if @va_Basedd_in = XDC_BASE_CFG
	begin
             update CFG..REG_PAR
             set    type = @va_Type_in,
		   valeur = @va_Valeur_in
             where  numero = @va_Numero_in

             if @@rowcount <> 1
             begin
                  select @va_Resultat_out = XDC_MODIF
                  return XDC_NOK
    	     end   
	end      
#ifdef CI      
    else
    begin
          update CFT..REG_PAR
          set    type = @va_Type_in,
		   valeur = @va_Valeur_in
          where  numero = @va_Numero_in

          if @@rowcount <> 1
          begin
               select @va_Resultat_out = XDC_MODIF
                return XDC_NOK
	  end  

    end
#endif
      
     
    return XDC_OK
         
go  
