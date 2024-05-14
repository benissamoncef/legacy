/*E*/
/* Fichier : $Id: xzao82sp.prc,v 1.7 2021/03/08 13:41:51 pc2dpdy Exp $        $Revision: 1.7 $        $Date: 2021/03/08 13:41:51 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao82sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	06 Oct 1994	: Creation
* volcic	30 Nov 1994	: Ajout #ifdef CI (v1.2)
* JMG   13/11/07 : ajout site de gestion DEM715  1.3
* JPL	16/10/18 : Filtrage des equipements marques comme supprimes (DEM 1305)  1.4
* JPL	19/10/18 : Filtrage des analyseurs supprimes comme les DAI (DEM 1305)  1.5
* LCL	07/06/19 : DAI/IP DEM1333 1.6
* CGR	14/09/20 : Ajout deuxieme adresse IP DEM-SAE175 1.7
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao82sp.h" */

/* static char *version = "@(#)xzao82.c	1.1 23/18/94 : xzao82" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Liste des DAI pour une machine
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAO;82
   @va_Basedd_in        	char(3),
   @va_NomMachine_in		char(10)
   as 

/* 
*
* SP	XZAO82SP
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
* Retourne la liste des DAI de la Machine 
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
* Retourne les parametres numero de DAI, nom du ST, numero de liaison, adresse rgs
* numero de type de DAI
*
* MECANISMES :
*
------------------------------------------------------*/
declare
   
   /*A
   ** Declaration des variables locales
   */
   
   @vl_Machine		smallint
   
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_Basedd_in = null 
      return XDC_ARG_INV
   
   else   
   
   begin
      
      /*A
      **  Travail dans la base CFG
      */   
      
      if @va_Basedd_in = XDC_BASE_CFG
      begin   
      
         /*A
         ** Recherche du numero de la Machine passee en argument
         */
         
         select @vl_Machine = numero from CFG..EQT_GEN
                 where nom  = @va_NomMachine_in
                   and type = XDC_EQT_MAC
                   
         
         /*A
         ** Recherche de la DAI et des analyseurs associes pour plusieurs serveurs
         ** de terminaux
         */
         
         select distinct DAI.numero, 
         		GEN.nom, 
         		DAI.rgs, 
         		DAI.liaison_serie_maitre, 
         		DAI.liaison_serie_esclave, 
         		ANA.numero, 
         		ANA.numero_analyseur, 
         		ANA.nombre_de_voies ,
			GEN2.site_gestion,
			DAI.adresse_ip,
			DAI.port,
			DAI.adresse_ip_e,
			DAI.port_e,
			DAI.adresse2_ip,
			DAI.port2,
			DAI.adresse2_ip_e,
			DAI.port2_e
         from CFG..EQT_GEN GEN, CFG..EQT_GEN GEN2, CFG..EQT_GEN GEN3, CFG..EQT_DAI DAI, CFG..EQT_ANA ANA 
           
           where GEN.type         = XDC_EQT_SRV
              and GEN.maitre       = @vl_Machine
              and GEN.type_maitre  = XDC_MAI_SRV
              and GEN2.type        = XDC_EQT_DAI
              and GEN2.maitre      = GEN.numero
              and GEN2.type_maitre = XDC_MAI_DAI
              and GEN2.eqt_supprime = XDC_FAUX
              and DAI.numero       = GEN2.numero
              and GEN3.type        = XDC_EQT_ANA
              and GEN3.maitre      = DAI.numero
              and GEN3.type_maitre = XDC_MAI_ANA
              and GEN3.eqt_supprime = XDC_FAUX
              and ANA.numero	   = GEN3.numero
         
         order by GEN.nom, DAI.numero, GEN3.numero   
      
      end
      
#ifdef CI
      
      else
      
      /*A
      **  Travail dans la base CFT
      */   
      
      begin   
      
         /*A
         ** Recherche du numero de la Machine passee en argument
         */
         
         select @vl_Machine = numero from CFT..EQT_GEN
                 where nom  = @va_NomMachine_in
                   and type = XDC_EQT_MAC
                   
         
         /*A
         ** Recherche de la DAI et des analyseurs associes pour plusieurs serveurs
         ** de terminaux
         */
         
         select distinct DAI.numero, 
         		GEN.nom, 
         		DAI.rgs, 
         		DAI.liaison_serie_maitre, 
         		DAI.liaison_serie_esclave, 
         		ANA.numero, 
         		ANA.numero_analyseur, 
         		ANA.nombre_de_voies ,
			GEN2.site_gestion,
			DAI.adresse_ip,
			DAI.port,
			DAI.adresse_ip_e,
			DAI.port_e,
			DAI.adresse2_ip,
			DAI.port2,
			DAI.adresse2_ip_e,
			DAI.port2_e
         from CFT..EQT_GEN GEN, CFT..EQT_GEN GEN2, CFT..EQT_GEN GEN3, CFT..EQT_DAI DAI, CFT..EQT_ANA ANA 
           
           where GEN.type         = XDC_EQT_SRV
              and GEN.maitre       = @vl_Machine
              and GEN.type_maitre  = XDC_MAI_SRV
              and GEN2.type        = XDC_EQT_DAI
              and GEN2.maitre      = GEN.numero
              and GEN2.type_maitre = XDC_MAI_DAI
              and GEN2.eqt_supprime = XDC_FAUX
              and DAI.numero       = GEN2.numero
              and GEN3.type        = XDC_EQT_ANA
              and GEN3.maitre      = DAI.numero
              and GEN3.type_maitre = XDC_MAI_ANA
              and GEN3.eqt_supprime = XDC_FAUX
              and ANA.numero	   = GEN3.numero
         
         order by GEN.nom, DAI.numero, GEN3.numero   
      
      end
#endif            
      
            
   end
      
   return XDC_OK
         
go  
