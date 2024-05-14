/*E*/
/* Fichier : $Id: xzao491sp.prc,v 1.4 2021/10/07 09:25:04 pc2dpdy Exp $      Release : $Revision: 1.4 $        Date : $Date: 2021/10/07 09:25:04 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao491sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
* CGR   28/07/20        : Creation IMU 1.1 DEM-SAE155
* ABE	07/06/21	: Ajout trim pour les string DEM-SAE155 1.2
* ABE	21/06/21	: Ajout du param NomMachine pour filtrer DEM-SAE155 1.3
* ABE	07/10/21	: Fix type pour trouver nomero machine maitre de type IMU a type MAC DEM-SAE155 1.4
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao491sp.h" */

/* static char *version = "@(#)xzao491sp.prc	1.1 1.1 28/07/20: xzao491" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Liste des IMU pour une machine
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAO491' and type = 'P')
	drop procedure XZAO491
go

create procedure XZAO491
   @va_Basedd_in        	char(3),
   @va_NomMachine_in		char(10)
   as 

/* 
*
* SP	XZAO491SP
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
* Retourne la liste des IMU
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
* Retourne les parametres numero de IMU, nom du serveur, adresse ip et port
*
* MECANISMES :
*
*/

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
            ** Recherche de la liste des IMU
            */
            select distinct IMU.numero,
		   GEN.nom,
		   GEN.autoroute,
		   GEN.PR,
		   GEN.sens,
		   rtrim(IMU.adresse_end_point_OPCUA) as adresse_end_point_OPCUA,
		   rtrim(IMU.user_OPCUA) as user_OPCUA,
		   rtrim(IMU.password_OPCUA) as password_OPCUA,
		   rtrim(IMU.nom_BICE) as nom_BICE,
		   IMU.port_maitre,
		   IMU.sous_type,
		   rtrim(IMU.nom_machine) as nom_machine,
		   GEN.site_gestion								
           from     CFG..EQT_IMU IMU, CFG..EQT_GEN GEN
	   where GEN.type = XDC_EQT_IMU and IMU.numero = GEN.numero and GEN.maitre = @vl_Machine
	 end
         else 
	 begin
            /*A
            ** Recherche de la liste des IMU
            */
            select distinct IMU.numero,
		   GEN.nom,
		   GEN.autoroute,
		   GEN.PR,
		   GEN.sens,
		   rtrim(IMU.adresse_end_point_OPCUA) as adresse_end_point_OPCUA, 
		   rtrim(IMU.user_OPCUA) as user_OPCUA,
		   rtrim(IMU.password_OPCUA) as password_OPCUA,
		   rtrim(IMU.nom_BICE) as nom_BICE,
		   IMU.port_maitre,
		   IMU.sous_type,
		   rtrim(IMU.nom_machine) as nom_machine,
		   GEN.site_gestion								
           from     CFG..EQT_IMU IMU, CFG..EQT_GEN GEN
	   where GEN.type = XDC_EQT_IMU and IMU.numero = GEN.numero
	 end
      end
   end
      
#ifdef CI
   
   /*A
   **  Travail dans la base CFT
   */ 

   if @va_Basedd_in = XDC_BASE_CFT      
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
            ** Recherche de la liste des IMU
            */         
            select distinct IMU.numero,
		   GEN.nom,
		   GEN.autoroute,
		   GEN.PR,
		   GEN.sens,
		   IMU.adresse_end_point_OPCUA,
		   IMU.user_OPCUA,
		   IMU.password_OPCUA,
		   IMU.nom_BICE,
		   IMU.port_maitre,
		   IMU.sous_type,
		   IMU.nom_machine,
		   GEN.site_gestion

	    from     CFT..EQT_IMU IMU, CFT..EQT_GEN GEN
	    where GEN.type = XDC_EQT_IMU and IMU.numero = GEN.numero and GEN.maitre = @vl_Machine
	 end
         else
	 begin
            /*A
            ** Recherche de la liste des IMU
            */         
            select distinct IMU.numero,
		   GEN.nom,
		   GEN.autoroute,
		   GEN.PR,
		   GEN.sens,
		   IMU.adresse_end_point_OPCUA,
		   IMU.user_OPCUA,
		   IMU.password_OPCUA,
		   IMU.nom_BICE,
		   IMU.port_maitre,
		   IMU.sous_type,
		   IMU.nom_machine,
		   GEN.site_gestion

	    from     CFT..EQT_IMU IMU, CFT..EQT_GEN GEN
	    where GEN.type = XDC_EQT_IMU and IMU.numero = GEN.numero
	 end
   end							

#endif      
     
   return XDC_OK
         
go  
