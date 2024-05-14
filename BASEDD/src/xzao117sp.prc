/*E*/
/* Fichier : $Id: xzao117sp.prc,v 1.7 2019/05/21 11:08:30 pc2dpdy Exp $        $Revision: 1.7 $        $Date: 2019/05/21 11:08:30 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao117sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	07 Oct 1994	: Creation
* JMG		21/11/07	: ajout site de gestion DEM/715
* JMG		22/11/07	: ajout site de gestion DEM/715 1.3 1.4
* PNI 		07/07/09	: renommage de ST des secteurs DEM887 1.5
* JPL		16/10/18 	: Filtrage des equipements PAU marques comme supprimes (DEM 1305)  1.6
* LCL		07/05/19 	: Ajout type PAU DEM 1334 1.7
* GGY		07/09/23 	: Passage des RAU en IP : ajout de adresse_ip_esclave, port_ip_esclave, adresse_ip_maitre, port_ip_maitre	(DEM483)
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao117sp.h" */

/* static char *version = "@(#)xzao117.c	1.1 23/18/94 : xzao117" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Liste des Systemes RAU pour une machine
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAO;117
   @va_Basedd_in        	char(3),
   @va_NomMachine_in		char(10)
   as 

/* 
*
* SP	XZAO117SP
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
* Retourne la liste des Systemes RAU de la Machine 
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
* Retourne XZAOT_PIRAU et XZAOT_RAU
*
* MECANISMES :
*
------------------------------------------------------*/
declare
   
   /*A
   ** Declaration des variables locales
   */
   
   	@vl_Machine				smallint,
   	@vl_ST					smallint,
   	@vl_Numero				smallint,
   	@vl_Maitre 				tinyint,
   	@vl_Esclave 			tinyint,
   	@vl_Rgs 				char(3),
   	@vl_AdresseIpEsclave	char (20),
	@vl_PortIpEsclave		int,
	@vl_AdresseIpMaitre		char (20),
	@vl_PortIpMaitre		int
    
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_ARG_INV  
   */
      
   if @va_Basedd_in = null
   or @va_NomMachine_in = null
    
      return XDC_ARG_INV
   
   else   
   
   /*A
   **   Les parametres d'entree ne sont pas nuls  
   */
      
   begin
      
      /*A
      **  Travail dans la base CFG
      */   
      
      if @va_Basedd_in = XDC_BASE_CFG
      begin   
      
         /*A
         ** Recherche du numero de la machine argument
         */
         
         select @vl_Machine = numero from CFG..EQT_GEN
                 where nom  = @va_NomMachine_in
                   and type = XDC_EQT_MAC
                   
         /*A
         ** Recherche de la liste des PAU associes a une machine
         */
     
	if @va_NomMachine_in != "SPCI"
	begin

        select distinct GRAU.numero,
        		GSRV.nom, 
        		RAU.liaison_serie_maitre, 
        		RAU.liaison_serie_esclave, 
        		RAU.rgs,
				RAU.adresse_ip_esclave,
				RAU.port_ip_esclave,
				RAU.adresse_ip_maitre,
				RAU.port_ip_maitre,
        		PAU.numero,
        		PAU.code_PAU,
        		PAU.code_antenne,
			PAU.type_PAU
        
        from CFG..EQT_GEN GSRV, 
        	CFG..EQT_GEN GPAU, 
        	CFG..EQT_GEN GRAU, 
        	CFG..EQT_PAU PAU,
          	CFG..EQT_RAU RAU
          
          where GSRV.type         = XDC_EQT_SRV
            and GSRV.maitre       = @vl_Machine
            and GSRV.type_maitre  = XDC_MAI_SRV
            and GRAU.type         = XDC_EQT_RAU
            and GRAU.maitre       = GSRV.numero
            and GRAU.type_maitre  = XDC_MAI_RAU
            and RAU.numero        = GRAU.numero
            and GPAU.type         = XDC_EQT_PAU
            and GPAU.maitre       = GRAU.numero
            and GPAU.type_maitre  = XDC_MAI_PAU
            and GPAU.eqt_supprime = XDC_FAUX
            and PAU.numero        = GPAU.numero
            
          order by GSRV.nom, RAU.numero, PAU.numero   
       end
       else
       begin
       select @vl_ST=numero
       from CFG..EQT_GEN
       where type         = XDC_EQT_SRV
       and maitre       = @vl_Machine

       select @vl_Maitre=liaison_serie_maitre, 
		@vl_Esclave = liaison_serie_esclave,
		@vl_Rgs = rgs,
		@vl_Numero = RAU.numero,
		@vl_AdresseIpEsclave = RAU.adresse_ip_esclave,
		@vl_PortIpEsclave = RAU.port_ip_esclave,	
		@vl_AdresseIpMaitre	= RAU.adresse_ip_maitre,
		@vl_PortIpMaitre = RAU.port_ip_maitre
	from CFG..EQT_RAU RAU,  CFG..EQT_GEN GEN
	where GEN.type         = XDC_EQT_RAU
	and GEN.maitre       = @vl_ST
	and RAU.numero  = GEN.numero

       select distinct @vl_Numero,
                        GSRV.nom,
                        @vl_Maitre,
                        @vl_Esclave,
                        @vl_Rgs,
						@vl_AdresseIpEsclave,
						@vl_PortIpEsclave,
						@vl_AdresseIpMaitre,
						@vl_PortIpMaitre,
                        PAU.numero,
                        PAU.code_PAU,
                        PAU.code_antenne,
			PAU.type_PAU

        from CFG..EQT_GEN GSRV,
                CFG..EQT_GEN GPAU,
                CFG..EQT_GEN GRAU,
                CFG..EQT_PAU PAU,
                CFG..EQT_RAU RAU
	         
	where GSRV.type         = XDC_EQT_SRV
	and GSRV.maitre       = @vl_Machine
	and GSRV.type_maitre  = XDC_MAI_SRV
	and GRAU.type         = XDC_EQT_RAU
	and GRAU.maitre       in ( select numero
		from CFG..EQT_GEN where (nom = "STPR"  or nom = "STVD" or
					nom = "STVE") )
	and GRAU.type_maitre  = XDC_MAI_RAU
        and RAU.numero        = GRAU.numero
	and GPAU.type         = XDC_EQT_PAU
	and GPAU.maitre       = GRAU.numero
	and GPAU.type_maitre  = XDC_MAI_PAU
	and GPAU.eqt_supprime = XDC_FAUX
	and PAU.numero        = GPAU.numero
	
	order by GSRV.nom, RAU.numero, PAU.numero
	end

      end
      
#ifdef CI      
      
      else
      
      /*A
      **  Travail dans la base CFT
      */   
      
      begin   
      
         /*A
         ** Recherche du numero de la machine argument
         */
         
         select @vl_Machine = numero from CFT..EQT_GEN
                 where nom  = @va_NomMachine_in
                   and type = XDC_EQT_MAC
                   
         /*A
         ** Recherche de la liste des Pictos associes a plusieurs serveurs
         */
         
        select distinct GRAU.numero,
        		GSRV.nom, 
        		RAU.liaison_serie_maitre, 
        		RAU.liaison_serie_esclave, 
        		RAU.rgs,
				RAU.adresse_ip_esclave,
				RAU.port_ip_esclave,
				RAU.adresse_ip_maitre,
				RAU.port_ip_maitre,
        		PAU.numero,
        		PAU.code_PAU,
        		PAU.code_antenne,
			PAU.type_PAU
        
        from CFT..EQT_GEN GSRV, 
        	CFT..EQT_GEN GPAU, 
        	CFT..EQT_GEN GRAU, 
        	CFT..EQT_PAU PAU,
          	CFT..EQT_RAU RAU
          
          where GSRV.type         = XDC_EQT_SRV
            and GSRV.maitre       = @vl_Machine
            and GSRV.type_maitre  = XDC_MAI_SRV
            and GRAU.type         = XDC_EQT_RAU
            and GRAU.maitre       = GSRV.numero
            and GRAU.type_maitre  = XDC_MAI_RAU
            and RAU.numero        = GRAU.numero
            and GPAU.type         = XDC_EQT_PAU
            and GPAU.maitre       = GRAU.numero
            and GPAU.type_maitre  = XDC_MAI_PAU
            and GPAU.eqt_supprime = XDC_FAUX
            and PAU.numero        = GPAU.numero
            
          order by GSRV.nom, RAU.numero, PAU.numero   
            
                   
      end
#endif
      
   end
      
   return XDC_OK      
go  
