/*E*/
/* Fichier : $Id: xzao861sp.prc,v 1.2 2019/04/23 11:08:50 pc2dpdy Exp $      Release : $Revision: 1.2 $        Date : $Date: 2019/04/23 11:08:50 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao861sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
* LCL   02/03/18        : Creation controleurs de feux CTRL_FEUX 1.1 DEM1284
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao861sp.h" */

/* static char *version = "@(#)xzao861sp.prc	1.2 1.2 11/19/07: xzao861" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Liste des CAP pour une machine
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAO861' and type = 'P')
	drop procedure XZAO861
go

create procedure XZAO861
   @va_Basedd_in        	char(3),
   @va_NomMachine_in		char(10),
   @va_Type_in			int
   as 

/* 
*
* SP	XZAO861SP
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
* Retourne les parametres numero de CAP, nom du serveur, adresse ip et port
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
            ** Recherche de la liste des CAP associes a plusieurs serveurs
            */
         
           select distinct CAP.numero,
	   		   GEN.nom,
			   AUT.nom,
			   GEN.PR,
			   GEN.sens,
			   CAP.type,
			   rtrim(CAP.rgs),
			   rtrim(CAP.adresse_ip),
			   CAP.port_maitre,
			   CAP.port_esclave,
			   CAP.point_caracteristique,
			   GEN2.nom,
			   GEN.site_gestion
           from    CFG..EQT_GEN GEN, 
        	   CFG..EQT_GEN GEN2, 
        	   CFG..EQT_CAP CAP,
		   CFG..RES_AUT AUT
             where GEN2.type         = XDC_EQT_SRV
               and  GEN.type_maitre  = XDC_MAI_CAP
               and GEN.type        = XDC_EQT_CAP
               and GEN2.numero      = GEN.maitre
               and GEN2.type_maitre = XDC_MAI_SRV
               and CAP.numero       = GEN.numero
	       and CAP.type	    = @va_Type_in
	       and AUT.numero	    = GEN.autoroute
	       and GEN2.maitre = @vl_Machine
             order by CAP.numero, GEN.nom
            
         end
         else
         begin
            /*A
            ** Recherche de la liste des CAP associes a plusieurs serveurs
            */
           select distinct CAP.numero,
	   		   GEN.nom,
			   AUT.nom,
			   GEN.PR,
			   GEN.sens,
			   CAP.type,
			   CAP.rgs,
			   CAP.adresse_ip,
			   CAP.port_maitre,
			   CAP.port_esclave,
			   CAP.point_caracteristique,
			   GEN2.nom,
			   GEN.site_gestion
			   
           from    CFG..EQT_GEN GEN, 
        	   CFG..EQT_GEN GEN2, 
        	   CFG..EQT_CAP CAP,
		   CFG..RES_AUT AUT
             where GEN2.type         = XDC_EQT_SRV
               and  GEN.type_maitre  = XDC_MAI_CAP
               and GEN.type        = XDC_EQT_CAP
               and GEN2.numero      = GEN.maitre
               and GEN2.type_maitre = XDC_MAI_SRV
               and CAP.numero       = GEN.numero
	       and CAP.type	    = @va_Type_in
	       and AUT.numero	    = GEN.autoroute
             order by CAP.numero, GEN.nom
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
            ** Recherche de la liste des CAP associes a plusieurs serveurs
            */
         
           select distinct CAP.numero,
	   		   GEN.nom,
			   AUT.nom,
			   GEN.PR,
			   GEN.sens,
			   CAP.type,
			   CAP.rgs,
			   CAP.adresse_ip,
			   CAP.port_maitre,
			   CAP.port_esclave,
			   CAP.point_caracteristique,
			   GEN2.nom,
			   GEN2.site_gestion
           from    CFT..EQT_GEN GEN, 
        	   CFT..EQT_GEN GEN2, 
        	   CFT..EQT_CAP CAP,
		   CFT..RES_AUT AUT
             where GEN.type         = XDC_EQT_SRV
               and GEN.maitre       = @vl_Machine
               and GEN.type_maitre  = XDC_MAI_SRV
               and GEN2.type        = XDC_EQT_CAP
               and GEN2.maitre      = GEN.numero
               and GEN2.type_maitre = XDC_MAI_CAP
               and CAP.numero       = GEN2.numero
	       and CAP.type	    = @va_Type_in
	       and AUT.numero	    = GEN.autoroute
             order by GEN.nom, CAP.numero
                   
         end
         else
         begin
            /*A
            ** Recherche de la liste des PMV associes a plusieurs serveurs
            */
         
           select distinct CAP.numero,
	   		   GEN.nom,
			   AUT.nom,
			   GEN.PR,
			   GEN.sens,
			   CAP.type,
			   CAP.rgs,
			   CAP.adresse_ip,
			   CAP.port_maitre,
			   CAP.port_esclave,
			   CAP.point_caracteristique,
			   GEN2.nom,
			   GEN2.site_gestion
           from    CFT..EQT_GEN GEN, 
        	   CFT..EQT_GEN GEN2, 
        	   CFT..EQT_CAP CAP,
		   CFT..RES_AUT AUT
             where GEN.type         = XDC_EQT_SRV
               and GEN.type_maitre  = XDC_MAI_SRV
               and GEN2.type        = XDC_EQT_CAP
               and GEN2.maitre      = GEN.numero
               and GEN2.type_maitre = XDC_MAI_CAP
               and CAP.numero       = GEN2.numero
	       and CAP.type	    = @va_Type_in
	       and AUT.numero	    = GEN.autoroute
             order by GEN.nom, CAP.numero
                   
         end
      end

#endif      
   end
      
   return XDC_OK
         
go  
