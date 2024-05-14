/*E*/
/* Fichier : $Id: xzaa30sp.prc,v 1.3 2020/11/03 16:09:51 pc2dpdy Exp $      Release : $Revision: 1.3 $        Date : $Date: 2020/11/03 16:09:51 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzaa30sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* VR		13/01/12 : Creation (DEM/1016)
* LCL		23/08/17 : Modif PRA
* LCL		17/05/20 : MOVIS correction d'erreur en gestion de conf DEM-SAE93
* ABK		26/09/23 : Ajout district CA (DEM-483)
* GGY		17/10/23 : Correction si CFG vide (DEM483)
* ------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzaa30sp.h" */

/* static char *version = "@(#)xzaa30.c	1.1 23/18/94 : xzaa30" ; */

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

if exists (select * from sysobjects where name = 'XZAA30' and type = 'P')
	drop procedure XZAA30
go

create procedure XZAA30
   @va_Basedd_in             char(3),
   @va_NumeroAlerte_in		 smallint
   as 

/* 
*
* SP    XZAA30SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd        va_Basedd_in
*					va_NumeroAlerte_in
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
* Affichage des donnees de la table ALT_REG
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* VR    13 Jan 2012    : Creation (DEM/1016)
------------------------------------------------------*/
declare
   
    /*A
    ** Definition des variables globales
    */
	@vl_Numero					smallint,
	@vl_Contexte				smallint,
	@vl_Scenario				smallint,
	@vl_IndicateurHoraire		smallint,
	@vl_Site					char(10)
   
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
                        ** Lecture des donnees dans ALT_REG
                        */

						select 
						@vl_Numero = numero, 
						@vl_Contexte = contexte,
						@vl_Scenario = scenario,
						@vl_IndicateurHoraire = indicateur_horaire
						from   CFG..ALT_REG
						where numero = @va_NumeroAlerte_in
					end   

                       

#ifdef CI      
            else

             /*A
            **  Travail dans la base CFT
            */   
      
				if @va_Basedd_in = XDC_BASE_CFT
            
					begin
                        /*A
                        ** Lecture des donnees dans ALT_REG
                        */

						select 
						@vl_Numero = numero, 
						@vl_Contexte = contexte,
						@vl_Scenario = scenario,
						@vl_IndicateurHoraire = indicateur_horaire
						from   CFT..ALT_REG
						where numero = @va_NumeroAlerte_in
                      
					end   
  

#endif
      
	    end
    end         
     
    return XDC_OK
         
go  
