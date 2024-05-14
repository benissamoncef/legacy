/*E*/
/* Fichier : $Id: xzao422sp.prc,v 1.4 2020/05/19 14:30:10 pc2dpdy Exp $        $Revision: 1.4 $        $Date: 2020/05/19 14:30:10 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao422sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	22 Jan 1998	: Creation
* JMG	13/11/07 : ajout site de gestion DEM715  1.2
* JPL	17/10/18 : Filtrage des equipements marques comme supprimes (DEM 1305)  1.3
* LCL	11/07/19 : passage PAL sur IP DEM1337 1.4
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao422sp.h" */

/* static char *version = "@(#)xzao422.c	1.1 23/18/94 : xzao422" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Liste des PAL connectees avec ses modules
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAO;422
   @va_NomMachine_in		char(10)
   as 

/* 
*
* SP	XZAO422SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Entier	va_Basedd_in		
* XDY_Machine	va_NomMachine_in		
*
* PARAMETRES EN SORTIE :
*
* VALEUR RENDUE :
*
* Retourne la liste des PAL connectes 
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
* MECANISMES :
*
------------------------------------------------------*/
declare
   
   /*A
   ** Declaration des variables locales
   */
   
   @vl_Machine		smallint
    
   if rtrim (@va_NomMachine_in) = NULL select @va_NomMachine_in=NULL

   if ( @va_NomMachine_in != NULL )
   begin
 
            /*A
            ** Recherche du numero de la machine argument
            */
         
            select @vl_Machine = numero from CFG..EQT_GEN
            where nom  = @va_NomMachine_in
                and type = XDC_EQT_MAC
                   
            /*A
            ** Recherche de la liste des PAL associes a un serveur
            */
         
            select 
			PAL.numero, 
            		PAL.type, 
         		GSRV.nom, 
         		PAL.liaison_serie, 
         		PAL.rgs,
			GPAL.site_gestion,
			PAL.adresse_ip,
			PAL.port
         
            from   CFG..EQT_GEN GMAC, 
         	CFG..EQT_GEN GSRV, 
         	CFG..EQT_GEN GPAL, 
         	CFG..EQT_PAL PAL
                 
            where GMAC.numero      = @vl_Machine
                and GMAC.type        = XDC_EQT_MAC
                and GSRV.type        = XDC_EQT_SRV
                and GSRV.maitre      = GMAC.numero
                and GSRV.type_maitre = XDC_MAI_SRV
                and GPAL.type	= XDC_EQT_PAL
                and GPAL.maitre      = GSRV.numero
                and GPAL.type_maitre = XDC_MAI_PAL
                and GPAL.eqt_supprime = XDC_FAUX
                and PAL.numero       = GPAL.numero
             
             order by PAL.numero

   end
   else
   begin

            /*A
            ** Recherche de la liste des PAL associes a plusieurs serveurs
            */
         
            select distinct PAL.numero, 
            		    PAL.type, 
         		    GSRV.nom, 
         		    PAL.liaison_serie, 
         		    PAL.rgs,
			    GPAL.site_gestion,
			    PAL.adresse_ip,
			    PAL.port
         
            from   CFG..EQT_GEN GMAC, 
         	CFG..EQT_GEN GSRV, 
         	CFG..EQT_GEN GPAL, 
         	CFG..EQT_PAL PAL 
                 
            where GMAC.type        = XDC_EQT_MAC
                and GSRV.type        = XDC_EQT_SRV
                and GSRV.maitre      = GMAC.numero
                and GSRV.type_maitre = XDC_MAI_SRV
                and GPAL.type = XDC_EQT_PAL
                and GPAL.maitre      = GSRV.numero
                and GPAL.type_maitre = XDC_MAI_PAL
                and GPAL.eqt_supprime = XDC_FAUX
                and PAL.numero       = GPAL.numero
             
             order by PAL.numero

   end
             
   return XDC_OK
         
go  
