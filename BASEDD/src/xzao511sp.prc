/*E*/
/*Fichier : $Id: xzao511sp.prc,v 1.1 2007/03/26 15:49:54 gesconf Exp $      Release : $Revision: 1.1 $       Date : $Date: 2007/03/26 15:49:54 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao511sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao511sp.h" */

/* static char *version = "$Id: xzao511sp.prc,v 1.1 2007/03/26 15:49:54 gesconf Exp $ $Revision: 1.1 $ $Date: 2007/03/26 15:49:54 $ : xzao511" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Liste des PMVA pour temps de parcours
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAO511' and type = 'P')
	drop procedure XZAO511
go

create procedure XZAO511
   as 

/* 
*
* SP	XZAO511SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Entier	va_Basedd_in		
*
* PARAMETRES EN SORTIE :
*
* VALEUR RENDUE :
*
* Retourne la liste des PMV pour temps de parcours 
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
* Retourne les parametres numero de PMVA et liste des destinations possibles
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* mismer	27 Fev 1997	: Creation (DEM/1417)
------------------------------------------------------*/
   
           /*A
           ** Recherche de la liste des PMV en config pour temps de parcours
           */
           select distinct PMV.numero,
           		   GEN.sens,
        	   	   RES.numero, 
        	   	   RES.libelle_PMV
        
           from    CFG..EQT_GEN GEN,
           	   CFG..EQT_GEN GEN2,
           	   CFG..EQT_ZDP ZDP,
           	   CFG..RES_DTP RES,
        	   CFG..EQT_PMVA PMV,
        	   CFG..EQT_GEN GEN3,
        	   CFG..EQT_ZDP ZDP2
          
             where GEN.type         =  XDC_EQT_PMVA
               and PMV.numero	    =  GEN.numero
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
            
             order by PMV.numero, RES.PR   
      

   return XDC_OK
         
go  
