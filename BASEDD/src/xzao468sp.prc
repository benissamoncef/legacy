/*E*/
/* Fichier : $Id: xzao468sp.prc,v 1.1 2013/01/25 09:59:43 pc2dpdy Exp $        Release : $Revision: 1.1 $        Date : $Date: 2013/01/25 09:59:43 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao468sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao468sp.h" */

/* static char *version = "@(#)xzao468.c	1.1 23/18/94 : xzao468" ; */

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

if exists (select * from sysobjects where name = 'XZAO468' and type = 'P')
	drop procedure XZAO468
go	

create procedure XZAO468
   as 

/* 
*
* SP	XZAO468SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Entier	va_Basedd_in		
*
* PARAMETRES EN SORTIE :
*
* VALEUR RENDUE :
*
* Retourne la liste des PMVA pour temps de parcours 
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
* Retourne les parametres numero de PMVA, nom du LT, autoroute, PR, sens
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* VR	05 Juin 2012	: Creation (DEM/1014 PMA)
------------------------------------------------------*/
   
           /*A
           ** Recherche de la liste des PMVA en config pour temps de parcours
           */
           select distinct PMA.numero,
        	   	   GEN3.autoroute, 
        	   	   GEN3.PR, 
        	   	   GEN3.sens,
        	   	   GEN.nom,
			   GEN3.site_gestion
        
           from    CFG..EQT_GEN GEN, 
        	   CFG..EQT_GEN GEN2, 
        	   CFG..EQT_GEN GEN3, 
        	   CFG..EQT_PMVA PMA
          
             where GEN.type         = XDC_EQT_MAC
               and GEN2.type        = XDC_EQT_SRV
               and GEN2.type_maitre = XDC_MAI_SRV
               and GEN3.type        = XDC_EQT_PMVA
               and GEN3.type_maitre = XDC_EQT_SRV
               and GEN3.maitre      = GEN2.numero
               and GEN.numero       = GEN2.maitre
               and GEN2.numero      = GEN3.maitre
               and PMA.numero       = GEN3.numero
            
             order by GEN3.sens, GEN3.autoroute, GEN3.PR, PMA.numero   
      
   return XDC_OK
         
go  
