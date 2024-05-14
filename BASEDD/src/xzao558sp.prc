/*E*/
/* Fichier : $Id: xzao558sp.prc,v 1.1 2012/07/04 11:31:36 pc2dpdy Exp $        Release : $Revision: 1.1 $        Date : $Date: 2012/07/04 11:31:36 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzao558sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* VR		13/01/12 : Creation (DEM/1016)
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao558sp.h" */

/* static char *version = "@(#)xzao558.c	1.1 23/18/94 : xzao558" ; */

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

if exists (select * from sysobjects where name = 'XZAO558' and type = 'P')
	drop procedure XZAO558
go

create procedure XZAO558
   @va_Basedd_in             char(3),
   @va_NumZone_in			 int
   as 

/* 
*
* SP    XZAO555SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd        va_Basedd_in
*  					va_NumZone_in
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf       va_Resultat_out
*
* CODE RETOUR :
*
* 0    XDC_OK
* 1    XDC_NOK 
*
*
* FONCTION :
*
* Affichage des donnees de la table REG_SCE
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* VR    13 Jan 2012    : Creation (DEM/1016)
------------------------------------------------------*/
	   
   
    /*A
    ** Test des parametres d'entree :
    **   Si un des parametres d'entree est egal a null alors XDC_NOK   
    */
      
    if @va_Basedd_in             = null or
	   @va_NumZone_in          = null
        
		return XDC_ARG_INV
   
      
            /*A
            **  Travail dans la base CFG
            */   
      
				if @va_Basedd_in = XDC_BASE_CFG
            
					begin
                        /*A
                        ** Lecture des donnees dans REG_SCE
                        */
						select 
						numero, 
						libelle,
						niveau,
						nominal,
						degrade,	
						zone,
						inhibition
						from   CFG..REG_SCE
						where  zone = @va_NumZone_in
						order by niveau
					end   

                       

#ifdef CI      
            else
	begin
             /*A
            **  Travail dans la base CFT
            */   
      
				if @va_Basedd_in = XDC_BASE_CFT
            
					begin
                        /*A
                        ** Lecture des donnees dans REG_SCE
                        */

						select 
						numero, 
						libelle,
						niveau,
						nominal,
						degrade,	
						zone,inhibition
						from   CFT..REG_SCE
						where  zone = @va_NumZone_in
						order by niveau

					end   
  
	end
#endif
      
     
    return XDC_OK
         
go  
