/*E*/
/*Fichier : @(#)xzao611sp.prc	1.1      Release : 1.1       Date : 01/03/12
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao611sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao611sp.h" */

/* static char *version = "@(#)xzao611sp.prc	1.1 1.1 03/26/07 : xzao611" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Liste des PRV pour temps de parcours
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go


if exists (select * from sysobjects where name = 'XZAO611' and type = 'P')
	drop procedure XZAO611
go

create procedure XZAO611
   as 

/* 
*
* SP	XZAO611SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Entier	va_Basedd_in		
*
* PARAMETRES EN SORTIE :
*
* VALEUR RENDUE :
*
* Retourne la liste des PRV pour temps de parcours 
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
* Retourne les parametres numero de PRV et liste des destinations possibles
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* VR	01 Mars	2012	: Creation (DEM/1016)
------------------------------------------------------*/
   
           /*A
           ** Recherche de la liste des PRV en config pour temps de parcours
           */
           select distinct PRV.numero,
           		   GEN.sens,
        	   	   RES.numero, 
        	   	   RES.libelle_PMV
        
           from    CFG..EQT_GEN GEN,
           	   CFG..EQT_GEN GEN2,
           	   CFG..EQT_ZDP ZDP,
           	   CFG..RES_DTP RES,
        	   CFG..EQT_PRV PRV,
        	   CFG..EQT_GEN GEN3,
        	   CFG..EQT_ZDP ZDP2
          
             where GEN.type         =  XDC_EQT_PRV
               and PRV.numero	    =  GEN.numero
               and GEN2.type  	    =  XDC_EQT_ZDP
               and ZDP.numero	    =  GEN2.numero
               and ZDP.inhibition   =  0
               and GEN.autoroute    =  GEN2.autoroute
               and GEN.sens	    =  GEN2.sens
               and GEN2.PR 	    <= GEN.PR 
               and GEN.PR 	    <= ZDP.PR_fin 
               and GEN3.type  	    =  XDC_EQT_ZDP
               and GEN.autoroute    =  GEN3.autoroute
               and GEN.sens	    =  GEN3.sens
               and ZDP2.numero	    =  GEN3.numero
               and ZDP2.inhibition  =  0
               and GEN3.PR 	    <= RES.PR 
               and RES.PR 	    <= ZDP2.PR_fin
               and RES.sens	    =  GEN.sens
               and RES.inhibition   =  0
               and ((GEN.sens = XDC_SENS_SUD and GEN.PR < RES.PR) or
                    (GEN.sens = XDC_SENS_NORD and GEN.PR > RES.PR))  
            
             order by PRV.numero, RES.PR   
      

   return XDC_OK
         
go  
