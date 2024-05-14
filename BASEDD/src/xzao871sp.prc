/*E*/
/* Fichier : $Id: xzao871sp.prc,v 1.1 2018/04/19 14:08:42 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2018/04/19 14:08:42 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao871sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
* LCL   02/03/18        : Creation controleurs de feux CTRL_FEUX 1.1 DEM1284
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao871sp.h" */

/* static char *version = "@(#)xzao871sp.prc	1.2 1.2 11/19/07: xzao871" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Liste des CFE pour une machine
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAO871' and type = 'P')
	drop procedure XZAO871
go

create procedure XZAO871
   @va_Basedd_in        	char(3),
   @va_NomMachine_in		char(10)
   as 

/* 
*
* SP	XZAO871SP
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
* Retourne la liste des PMV de la Machine 
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
* Retourne les parametres numero de CFE, nom du serveur, adresse ip et port
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* LCL	02/03/18	creation controleurs de feux CTRL_FEUX 1.1
------------------------------------------------------*/
declare
   
   /*A
   ** Declaration des variables locales
   */
   
   @vl_Machine		smallint
   
   
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_ARG_INV 
   */
      
   if @va_Basedd_in = null 
      return XDC_ARG_INV
   
   else   
   
   begin

      if rtrim (@va_NomMachine_in) = NULL select @va_NomMachine_in=NULL
      
      /*A
      **  Travail dans la base CFG
      */   
      
      if @va_Basedd_in = XDC_BASE_CFG
      begin   
      
         if ( @va_NomMachine_in != NULL )
         begin
 
            /*A
            ** Recherche du numero de la machine argument
            */
         
            select @vl_Machine = numero from CFG..EQT_GEN
              where nom  = @va_NomMachine_in
                and type = XDC_EQT_MAC
                   
            /*A
            ** Recherche de la liste des CFE associes a plusieurs serveurs
            */
         
           select distinct CFE.numero,
			   CFE.adresse_ip,
			   CFE.port,
        	   	   GEN.nom, 
			   GEN2.site_gestion
           from    CFG..EQT_GEN GEN, 
        	   CFG..EQT_GEN GEN2, 
        	   CFG..EQT_CFE CFE
             where GEN.type         = XDC_EQT_SRV
               and GEN.maitre       = @vl_Machine
               and GEN.type_maitre  = XDC_MAI_SRV
               and GEN2.type        = XDC_EQT_CFE
               and GEN2.maitre      = GEN.numero
               and GEN2.type_maitre = XDC_MAI_CFE
               and CFE.numero       = GEN2.numero
             order by CFE.numero, GEN.nom
            
         end
         else
         begin
            /*A
            ** Recherche de la liste des CFE associes a plusieurs serveurs
            */
           select distinct CFE.numero,
			   CFE.adresse_ip,
			   CFE.port,
        	   	   GEN.nom, 
			   GEN2.site_gestion
           from    CFG..EQT_GEN GEN, 
        	   CFG..EQT_GEN GEN2, 
        	   CFG..EQT_CFE CFE
             where GEN.type         = XDC_EQT_SRV
               and GEN.type_maitre  = XDC_MAI_SRV
               and GEN2.type        = XDC_EQT_CFE
               and GEN2.maitre      = GEN.numero
               and GEN2.type_maitre = XDC_MAI_CFE
               and CFE.numero       = GEN2.numero
             order by CFE.numero, GEN.nom
         end
      end
      
#ifdef CI

      else
      
      /*A
      **  Travail dans la base CFT
      */   
      
      begin   
      
         if ( @va_NomMachine_in != NULL )
         begin
            /*A
            ** Recherche du numero de la machine argument
            */
         
            select @vl_Machine = numero from CFT..EQT_GEN
                 where nom  = @va_NomMachine_in
                   and type = XDC_EQT_MAC
                   
            /*A
            ** Recherche de la liste des CFE associes a plusieurs serveurs
            */
         
           select distinct CFE.numero,
	   		   CFE.adresse_ip,
			   CFE.port,
        	   	   GEN.nom, 
		 	   GEN2.site_gestion
           from    CFT..EQT_GEN GEN, 
        	   CFT..EQT_GEN GEN2, 
        	   CFT..EQT_CFE CFE
             where GEN.type         = XDC_EQT_SRV
               and GEN.maitre       = @vl_Machine
               and GEN.type_maitre  = XDC_MAI_SRV
               and GEN2.type        = XDC_EQT_CFE
               and GEN2.maitre      = GEN.numero
               and GEN2.type_maitre = XDC_MAI_CFE
               and CFE.numero       = GEN2.numero
             order by GEN.nom, CFE.numero
                   
         end
         else
         begin
            /*A
            ** Recherche de la liste des PMV associes a plusieurs serveurs
            */
         
           select distinct CFE.numero,
	   		   CFE.adresse_ip,
			   CFE.port,
        	   	   GEN.nom, 
			   GEN2.site_gestion 
           from    CFT..EQT_GEN GEN, 
        	   CFT..EQT_GEN GEN2, 
        	   CFT..EQT_CFE CFE
             where GEN.type         = XDC_EQT_SRV
               and GEN.type_maitre  = XDC_MAI_SRV
               and GEN2.type        = XDC_EQT_CFE
               and GEN2.maitre      = GEN.numero
               and GEN2.type_maitre = XDC_MAI_CFE
               and CFE.numero       = GEN2.numero
             order by GEN.nom, CFE.numero
                   
         end
      end

#endif      
   end
      
   return XDC_OK
         
go  
