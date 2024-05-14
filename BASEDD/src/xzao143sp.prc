/*E*/
/* Fichier : $Id: xzao143sp.prc,v 1.7 2018/11/06 20:06:04 devgfi Exp $        $Revision: 1.7 $        $Date: 2018/11/06 20:06:04 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao143sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	19 Sep 1994	: Creation
* volcic	21 Oct 1994	: Suppr XDC_ECHEC et Ajout #ifdef CI (v1.2)
* JMG   	13/11/07	: ajout site de gestion DEM715  1.3
* LCL		11/07/18	: correction syntaxe from  1.5
* JPL		18/10/18	: Nommage des colonnes sur 'insert'  1.6
* JPL		06/11/18	: Degroupee; Gestion du champ 'Supprime' (DEM 1305)  1.7
* GGY		07/09/23	: Passage des RAU en IP : ajout de adresse_ip_esclave, port_ip_esclave, adresse_ip_maitre, port_ip_maitre	(DEM483)
* GGY		26/09/23	: Ajout district CA (DEM483)
* GGY		17/10/23	: Correction si CFG vide (DEM483)
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao143sp.h" */

/* static char *version = "@(#)xzao143.c	1.1 20/09/94 : xzao143" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Ajout d'une PIRAU dans la base
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

if exists (select 1 from sysobjects where name = 'XZAO143' and type = 'P')
	drop procedure XZAO143
go


create procedure XZAO143
	@va_Basedd_in        		char(3),
	@va_Nom_in					char(10),
	@va_NomServeur_in			char(10),
	@va_LiaisonMaitre_in		tinyint,
	@va_LiaisonEsclave_in		tinyint,
	@va_RGS_in					char(3),
	@va_Supprime_in				bit,
	@va_AdresseIpEsclave_in		char(20),
	@va_PortIpEsclave_in		int,
	@va_AdresseIpMaitre_in		char(20),
	@va_PortIpMaitre_in			int,
	@va_Resultat_out			int output
   as 

/* 
*
* SP	XZAO143SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd		va_Basedd_in
* XDY_NomEqt		va_Nom_in
* XDY_NomEqt		va_NomServeur_in
* XDY_Octet		va_LiaisonMaitre_in
* XDY_Octet		va_LiaisonEsclave_in
* XDY_Rgs		va_RGS_in
* XDY_Booleen		va_Supprime_in
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf	   va_Resultat_out	
*
* VALEUR RENDUE :
*
* Ajoute une PIRAU en base
* 
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK 
*
* CONDITIONS D'UTILISATION :
*
* Config OFF LINE
*
* FONCTION :
*
* Insertion ou modification dans la table EQT_PIRAU (PIRAU)
*
* MECANISMES :
*
------------------------------------------------------*/
declare
   
   /*A
   ** Definition des variables globales
   */

   @vl_Serveur		smallint,
   @vl_PIRAU		smallint,
   @vl_NumPIRAU		smallint,
   @vl_Numero		smallint,
   @vl_Site			char(10)
   
   select @va_Resultat_out = XDC_LU
            
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   	if @va_Basedd_in = null  
			or @va_Nom_in = null 
			or @va_NomServeur_in = null 
			or @va_LiaisonMaitre_in = null 
			or @va_LiaisonEsclave_in = null 
			or @va_RGS_in = null
			or @va_Supprime_in = null
			or @va_AdresseIpEsclave_in = null
			or @va_PortIpEsclave_in = null
			or @va_AdresseIpMaitre_in = null	
			or @va_PortIpMaitre_in = null
    	return XDC_ARG_INV
   
   else   
   begin
      
		/* Verification que le site est bien celui du CI ou CA */
		select  @vl_Site = @@servername
	   
		/* Si le site est CI ou CA */
		if @vl_Site=XDC_SQLCI or @vl_Site=XDC_SQLCA
		begin tran
      
      
         /*A
         **  Travail dans la base CFG
         */   
      
         if @va_Basedd_in = XDC_BASE_CFG
         begin
         
            /*A
            ** Verification de l'existence et recherche du serveur et de la PIRAU comme equipement
            */
         
            select @vl_Serveur = numero from CFG..EQT_GEN
              where type = XDC_EQT_SRV
              	and nom  = @va_NomServeur_in
            
            select @vl_PIRAU = numero from CFG..EQT_GEN
              where type = XDC_EQT_RAU
              	and nom  = @va_Nom_in
              
            /*A
            ** La PIRAU n'existe pas comme equipement
            */
            
            if @vl_PIRAU = null
            begin
               
               /*A
               ** Insertion du PI dans la table EQT_GEN
               */
               
               select @vl_Numero = max(numero) from CFG..EQT_GEN
                 where type = XDC_EQT_RAU
              
               if  @vl_Numero = null
                 select @vl_Numero = 0  
            
               insert into CFG..EQT_GEN (
                             type,
                             numero,
                             maitre,
                             type_maitre,
                             autoroute,
                             PR,
                             sens,
                             nom,
                             site_gestion,
                             eqt_supprime
                           )
               values ( XDC_EQT_RAU, @vl_Numero + 1, @vl_Serveur, XDC_MAI_RAU, null, null, null, @va_Nom_in, null, @va_Supprime_in)
                       
               /*A
               ** Erreur d'insertion
               */
                     
               if @@rowcount <> 1
               begin
                     
                  /*A
                  ** Affectation du parametre de sortie Resultat a XDC_AJOUT
                  */
         
                  select @va_Resultat_out = XDC_AJOUT
                        
                  return XDC_NOK
               end   
                        
               /*A
               ** Insertion du PI dans la table EQT_RAU
               */
               
               insert into CFG..EQT_RAU
               values ( @vl_Numero + 1, @va_RGS_in, @va_LiaisonMaitre_in, @va_LiaisonEsclave_in, 
			   			@va_AdresseIpEsclave_in, @va_PortIpEsclave_in, @va_AdresseIpMaitre_in, @va_PortIpMaitre_in)
                       
               /*A
               ** Erreur d'insertion
               */
                     
               if @@rowcount <> 1
               begin
                     
                  /*A
                  ** Affectation du parametre de sortie Resultat a XDC_AJOUT
                  */
         
                  select @va_Resultat_out = XDC_AJOUT
                        
                  return XDC_NOK
               end   
                        
               /*A
               ** Affectation du parametre de sortie Resultat a XDC_AJOUT
               */
         
               select @va_Resultat_out = XDC_AJOUT
                     
            end
                  
            else
                   
            /*A
            ** Le PIRAU existe deja dans EQT_GEN
            */
                  
            begin
           
               /*A
               ** Modification du PIRAU dans la table EQT_GEN
               */
                
               update CFG..EQT_GEN
    	       set maitre     = @vl_Serveur,
    	           type_maitre = XDC_MAI_RAU,
    	           eqt_supprime	= @va_Supprime_in
    	       where numero = @vl_PIRAU
    	         and type   = XDC_EQT_RAU
    	         and nom    = @va_Nom_in   
                       
               /*A
               ** Erreur de modification
               */
                     
               if @@rowcount <> 1
               begin
                  
                  /*A
                  ** Affectation du parametre de sortie Resultat a XDC_MODIF
                  */
         
                  select @va_Resultat_out = XDC_MODIF
                       
                  return XDC_NOK
               end
                     
               /*A
               ** Modification du PIRAU dans la table EQT_RAU
               */
                
               update CFG..EQT_RAU
    	       set liaison_serie_maitre		= @va_LiaisonMaitre_in,
    	           	liaison_serie_esclave	= @va_LiaisonEsclave_in, 
    	          	rgs              		= @va_RGS_in,
				   	adresse_ip_esclave		= @va_AdresseIpEsclave_in,
					port_ip_esclave			= @va_PortIpEsclave_in,
					adresse_ip_maitre		= @va_AdresseIpMaitre_in,
					port_ip_maitre			= @va_PortIpMaitre_in
    	       where numero = @vl_PIRAU
                       
               /*A
               ** Erreur de modification
               */
                     
               if @@rowcount <> 1
               begin
                  
                  /*A
                  ** Affectation du parametre de sortie Resultat a XDC_MODIF
                  */
         
                  select @va_Resultat_out = XDC_MODIF
                       
                  return XDC_NOK
               end
                     
               /*A
               ** Affectation du parametre de sortie Resultat a XDC_MODIF
               */
         
               select @va_Resultat_out = XDC_MODIF
                        
            end
         end
         
         else

#ifdef CI

         /*A
         **  Travail dans la base CFT
         */   
      
         begin
         
            /*A
            ** Verification de l'existence et recherche du serveur et de la PIRAU comme equipement
            */
         
            select @vl_Serveur = numero from CFT..EQT_GEN
              where type = XDC_EQT_SRV
              	and nom  = @va_NomServeur_in
            
            select @vl_PIRAU = numero from CFT..EQT_GEN
              where type = XDC_EQT_RAU
              	and nom  = @va_Nom_in
              
            /*A
            ** La PIRAU n'existe pas comme equipement
            */
            
            if @vl_PIRAU = null
            begin
               
               /*A
               ** Insertion du PI dans la table EQT_GEN
               */
               
               select @vl_Numero = max(numero) from CFT..EQT_GEN
                 where type = XDC_EQT_RAU
              
               if  @vl_Numero = null
                 select @vl_Numero = 0  
            
               insert into CFT..EQT_GEN (
                             type,
                             numero,
                             maitre,
                             type_maitre,
                             autoroute,
                             PR,
                             sens,
                             nom,
                             site_gestion,
                             eqt_supprime
                           )
               values ( XDC_EQT_RAU, @vl_Numero + 1, @vl_Serveur, XDC_MAI_RAU, null, null, null, @va_Nom_in, null, @va_Supprime_in)
                       
               /*A
               ** Erreur d'insertion
               */
                     
               if @@rowcount <> 1
               begin
                     
                  /*A
                  ** Affectation du parametre de sortie Resultat a XDC_AJOUT
                  */
         
                  select @va_Resultat_out = XDC_AJOUT
                        
                  return XDC_NOK
               end   
                        
               /*A
               ** Insertion du PIRAU dans la table EQT_RAU
               */
               
               insert into CFT..EQT_RAU
               values ( @vl_Numero + 1, @va_RGS_in, @va_LiaisonMaitre_in, @va_LiaisonEsclave_in, 
			   			@va_AdresseIpEsclave_in, @va_PortIpEsclave_in, @va_AdresseIpMaitre_in, @va_PortIpMaitre_in)
                       
               /*A
               ** Erreur d'insertion
               */
                     
               if @@rowcount <> 1
               begin
                     
                  /*A
                  ** Affectation du parametre de sortie Resultat a XDC_AJOUT
                  */
         
                  select @va_Resultat_out = XDC_AJOUT
                        
                  return XDC_NOK
               end   
                        
               /*A
               ** Affectation du parametre de sortie Resultat a XDC_AJOUT
               */
         
               select @va_Resultat_out = XDC_AJOUT
                     
            end
                  
            else
                   
            /*A
            ** Le PIRAU existe deja dans EQT_PIRAU
            */
                  
            begin
           
               /*A
               ** Modification du PIRAU dans la table EQT_GEN
               */
                
               update CFT..EQT_GEN
    	       set maitre     = @vl_Serveur,
    	           type_maitre = XDC_MAI_RAU,
    	           eqt_supprime	= @va_Supprime_in
    	       where numero = @vl_PIRAU
    	         and type   = XDC_EQT_RAU
    	         and nom    = @va_Nom_in   
                       
               /*A
               ** Erreur de modification
               */
                     
               if @@rowcount <> 1
               begin
                  
                  /*A
                  ** Affectation du parametre de sortie Resultat a XDC_MODIF
                  */
         
                  select @va_Resultat_out = XDC_MODIF
                       
                  return XDC_NOK
               end
                     
               /*A
               ** Modification du PIRAU dans la table EQT_PIRAU
               */
                
               update CFT..EQT_RAU
    	       set liaison_serie_maitre		= @va_LiaisonMaitre_in,
    	           	liaison_serie_esclave	= @va_LiaisonEsclave_in, 
    	           	rgs              		= @va_RGS_in,
				   	adresse_ip_esclave		= @va_AdresseIpEsclave_in,
					port_ip_esclave			= @va_PortIpEsclave_in,
					adresse_ip_maitre		= @va_AdresseIpMaitre_in,
					port_ip_maitre			= @va_PortIpMaitre_in
    	       where numero = @vl_PIRAU
                       
               /*A
               ** Erreur de modification
               */
                     
               if @@rowcount <> 1
               begin
                  
                  /*A
                  ** Affectation du parametre de sortie Resultat a XDC_MODIF
                  */
         
                  select @va_Resultat_out = XDC_MODIF
                       
                  return XDC_NOK
               end
                     
               /*A
               ** Affectation du parametre de sortie Resultat a XDC_MODIF
               */
         
               select @va_Resultat_out = XDC_MODIF
                        
            end
         end
             
#endif      
      
      
      commit tran
   end         
     
   return XDC_OK
         
go  
