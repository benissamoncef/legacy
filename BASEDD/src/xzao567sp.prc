/*E*/
/* Fichier : $Id: xzao567sp.prc,v 1.2 2018/01/09 11:59:44 pc2dpdy Exp $        Release : $Revision: 1.2 $        Date : $Date: 2018/01/09 11:59:44 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzao567sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* VR		13/01/12 : Creation (DEM/1016)
* LCL		05/07/17 : modif PRA
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao567sp.h" */

/* static char *version = "@(#)xzao567.c	1.1 23/18/94 : xzao567" ; */

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

if exists (select * from sysobjects where name = 'XZAO567' and type = 'P')
	drop procedure XZAO567
go

create procedure XZAO567
   @va_Basedd_in             char(3),
   @va_Resultat_out		int output
   as 

/* 
*
* SP    XZAO567SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd        va_Basedd_in
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
* Affichage des donnees de la table REG_VIRTUELLE
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* VR    13 Jan 2012	: Creation (DEM/1016)
* GGY	26/09/23 	: Ajout district CA (DEM483)
* GGY	17/10/23	: Correction si CFG vide (DEM483)
------------------------------------------------------*/
declare	@vl_Site	char(10)
	     
    select @va_Resultat_out = XDC_LU
   
    /*A
    ** Test des parametres d'entree :
    **   Si un des parametres d'entree est egal a null alors XDC_NOK   
    */
      
    if @va_Basedd_in           = null
        
		return XDC_ARG_INV
   
    else   
    begin
      
		/* Verification que le site est bien celui du CI ou CA */
		select  @vl_Site = @@servername
	   
		/* Si le site est CI ou CA */
		if @vl_Site=XDC_SQLCI or @vl_Site=XDC_SQLCA
		begin

            /*A
            **  Travail dans la base CFG
            */   
      
				if @va_Basedd_in = XDC_BASE_CFG
            
					begin
                        /*A
                        ** Lecture des donnees dans REG_VIRTUELLE
                        */

						select 
						numero, 
						station1,
						station2,
						station3,
						operateur1,
						operateur2,
						operateur3,
						inhibition
						from   CFG..REG_VIRTUELLE
                      
					end   

                       

#ifdef CI      
            else

             /*A
            **  Travail dans la base CFT
            */   
      
				if @va_Basedd_in = XDC_BASE_CFT
            
					begin
                        /*A
                        ** Lecture des donn�es dans REG_VIRTUELLE
                        */

						select 
						numero, 
						station1,
						station2,
						station3,
						operateur1,
						operateur2,
						operateur3,
						inhibition
						from   CFT..REG_VIRTUELLE
                      
					end   
  

#endif
      
	    end
    end         
     
    return XDC_OK
         
go  
