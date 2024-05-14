/*E*/
/*Fichier : $Id: xzao410sp.prc,v 1.2 2007/12/13 10:46:10 pc2dpdy Exp $      Release : $Revision: 1.2 $        Date : $Date: 2007/12/13 10:46:10 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao410sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao410sp.h" */

/* static char *version = "@(#)xzao410.c	1.1 23/18/94 : xzao410" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Liste des PMV pour temps de parcours
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAO;410
   as 

/* 
*
* SP	XZAO410SP
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
* Retourne les parametres numero de PMV, nom du LT, autoroute, PR, sens
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* mismer	27 Fev 1997	: Creation (DEM/1395)
------------------------------------------------------*/
   
           /*A
           ** Recherche de la liste des PMV en config pour temps de parcours
           */
           select distinct PMV.numero,
        	   	   GEN3.autoroute, 
        	   	   GEN3.PR, 
        	   	   GEN3.sens,
        	   	   GEN.nom,
			   GEN3.site_gestion
        
           from    CFG..EQT_GEN GEN, 
        	   CFG..EQT_GEN GEN2, 
        	   CFG..EQT_GEN GEN3, 
        	   CFG..EQT_PMV PMV
          
             where GEN.type         = XDC_EQT_MAC
               and GEN2.type        = XDC_EQT_SRV
               and GEN2.type_maitre = XDC_MAI_SRV
               and GEN3.type        = XDC_EQT_PMV
               and GEN3.type_maitre = XDC_EQT_SRV
               and GEN3.maitre      = GEN2.numero
               and GEN.numero       = GEN2.maitre
               and GEN2.numero      = GEN3.maitre
               and PMV.numero       = GEN3.numero
            
             order by GEN3.sens, GEN3.autoroute, GEN3.PR, PMV.numero   
      
   return XDC_OK
         
go  
