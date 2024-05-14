/*E*/
/* Fichier : $Id: xzao555sp.prc,v 1.3 2018/07/11 11:19:25 pc2dpdy Exp $        Release : $Revision: 1.3 $        Date : $Date: 2018/07/11 11:19:25 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzao555sp.prc
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
* LCL           11/07/18        : correction syntaxe from
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao555sp.h" */

/* static char *version = "@(#)xzao555.c	1.1 23/18/94 : xzao555" ; */

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

if exists (select * from sysobjects where name = 'XZAO555' and type = 'P')
	drop procedure XZAO555
go

create procedure XZAO555
   @va_Basedd_in             char(3),
   @va_TypeParam_in			 tinyint,
   @va_Resultat_out		int output
   as 

/* 
*
* SP    XZAO555SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd        va_Basedd_in
*  					va_TypeParam_in
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf       va_Resultat_out
*					numero
*					nom
*					valeur
*
* CODE RETOUR :
*
* 0    XDC_OK
* 1    XDC_NOK 
*
*
* FONCTION :
*
* Affichage des donnees de la table REG_PAR
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* VR    13 Jan 2012 : Creation (DEM/1016)
* GGY	26/09/23 	: Ajout district CA (DEM483)
* GGY	17/10/23	: Correction si CFG vide (DEM483)
------------------------------------------------------*/
declare
	@vl_Site	char(10)

    select @va_Resultat_out = XDC_LU
   
    /*A
    ** Test des parametres d'entree :
    **   Si un des parametres d'entree est egal a null alors XDC_NOK   
    */
      
    if @va_Basedd_in             = null or
	   @va_TypeParam_in          = null
        
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
                        ** Lecture des donnees dans REG_PAR
                        */

						select 
						numero, 
						type, 
						nom, 
						valeur
						from   CFG..REG_PAR
						where  type = @va_TypeParam_in
                      
					end   

                       

#ifdef CI      
            else

             /*A
            **  Travail dans la base CFT
            */   
      
				if @va_Basedd_in = XDC_BASE_CFT
            
					begin
                        /*A
                        ** Lecture des donnees dans REG_PAR
                        */

						select 
						numero, 
						type, 
						nom, 
						valeur
						from   CFT..REG_PAR
						where  type = @va_TypeParam_in

					end   
  

#endif
      
	    end
    end         
     
    return XDC_OK
         
go  
