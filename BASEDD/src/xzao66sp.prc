/*E*/
/* Fichier : $Id: xzao66sp.prc,v 1.6 2018/10/19 14:58:14 devgfi Exp $        $Revision: 1.6 $        $Date: 2018/10/19 14:58:14 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao66sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	16 Sep 1994	: Creation
* volcic	20 Oct 1994	: Suppr XDC_ECHEC et Ajout #ifdef CI (v1.2)
* JMG  		13/11/07	: ajout site de gestion DEM715
* LCL		05/07/17	: Modif PRA  1.4
* LCL		11/07/18	: correction syntaxe from  1.5
* JPL		18/10/18	: Nommage des colonnes sur 'insert'  1.6
* GGY		26/09/23	:	Ajout district CA (DEM483)
* GGY		17/10/23	: Correction si CFG vide (DEM483)
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao66sp.h" */

/* static char *version = "@(#)xzao66.c	1.1 23/18/94 : xzao66" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Ajout d'un serveur de terminaux dans la table EQUIPEMENT
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAO;66
   @va_Basedd_in        	char(3),
   @va_NomServeur_in		char(10),
   @va_NomMaitre_in		char(10),
   @va_Resultat_out		int output
   as 

/* 
*
* SP	XZAO66SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd	va_Basedd_in        		
* XDY_Nom	va_NomServeur_in
* XDY_NomEqt	va_NomMaitre_in			
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf	   va_Resultat_out	
*
* VALEUR RENDUE :
*
* Ajoute un serveur de terminaux en base
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
* Ajout ou update dans la table EQT_GEN (EQUIPEMENT), Suppression impossible
* ,mettre HS.
*
* MECANISMES :
*
------------------------------------------------------*/
declare
   
   /*A
   ** Definition des variables globales
   */

   @vl_Serveur		smallint,
   @vl_Maitre		smallint,
   @vl_Numero		smallint,
   @vl_Site			char(10)


   select @va_Resultat_out = XDC_LU
   
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_Basedd_in = NULL
   or @va_NomServeur_in = NULL 
   or @va_NomMaitre_in = NULL 
      
      return XDC_NOK
   
   else   
   begin

		/* Verification que le site est bien celui du CI ou CA */
		select  @vl_Site = @@servername
	   
		/* Si le site est CI ou CA */
		if @vl_Site=XDC_SQLCI or @vl_Site=XDC_SQLCA
		begin
      
      /*A
      **  Travail dans la base CFG
      */   
      
      if @va_Basedd_in = XDC_BASE_CFG
      begin 
         select @vl_Maitre = numero from CFG..EQT_GEN
            where nom  = @va_NomMaitre_in
              and type = XDC_EQT_MAC
              
         /*A
         * La machine maitre existe comme equipement
         */    
         
         if @vl_Maitre <> null
         begin
            
            select @vl_Serveur = numero from CFG..EQT_GEN
               where nom  = @va_NomServeur_in
                 and type = XDC_EQT_SRV
                        
          
            /*A
            * Le serveur de terminaux maitre n'existe pas comme equipement
            */
              
            if @vl_Serveur = null
            begin
         
               /*A
               **  Ajout du serveur dans la table EQT_GEN
	       */
               
               select @vl_Numero = max(numero) from CFG..EQT_GEN
                  where type = XDC_EQT_SRV
                  
               if @vl_Numero = null
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
                             site_gestion
                           )
               values ( XDC_EQT_SRV, @vl_Numero + 1, @vl_Maitre, XDC_MAI_MAC, null, null, null, @va_NomServeur_in, null)
               
               /*A
               ** Si erreur d'insertion
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
                  
            /*A
            * Le serveur de terminaux n'existe pas comme equipement
            */
            
            else
            
            begin
               
               update CFG..EQT_GEN
               set maitre      = @vl_Maitre,
                   type_maitre = XDC_MAI_MAC
               where numero  = @vl_Serveur
                 and type    = XDC_EQT_SRV
            
               /*A
               ** Si Erreur de modification
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
         
         /*A
         ** La machine maitre n'existe pas comme equipement
         */
         
         else
            
            /*A
            ** Affectation du parametre de sortie Resultat a XDC_ECHEC
            */
         
            select @va_Resultat_out = XDC_ECHEC
      end
      
#ifdef CI

      else
      
      /*A
      **  Travail dans la base CFT
      */   
      
      begin 
         
         select @vl_Maitre = numero from CFT..EQT_GEN
            where nom  = @va_NomMaitre_in
              and type = XDC_MAI_MAC
              
         /*A
         * La machine maitre existe comme equipement
         */    
         
         if @vl_Maitre <> null
         begin
            
            select @vl_Serveur = numero from CFT..EQT_GEN
               where nom  = @va_NomServeur_in
                 and type = XDC_EQT_SRV
                        
          
            /*A
            * Le serveur de terminaux n'existe pas comme equipement
            */
              
            if @vl_Serveur = null
            begin
         
               /*A
               **  Ajout du serveur dans la table EQT_GEN
	       */
               
               select @vl_Numero = max(numero) from CFT..EQT_GEN
                  where type = XDC_EQT_SRV
                  
               if @vl_Numero = null
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
                             site_gestion
                           )
               values ( XDC_EQT_SRV, @vl_Numero + 1, @vl_Maitre, XDC_MAI_MAC, null, null, null, @va_NomServeur_in, null)
               
               /*A
               ** Si erreur d'insertion
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
                  
            /*A
            * Le serveur de terminaux existe comme equipement
            */
            
            else
            
            begin
               
               update CFT..EQT_GEN
               set maitre      = @vl_Maitre,
                   type_maitre = XDC_MAI_MAC
               where numero    = @vl_Serveur
                 and type      = XDC_EQT_SRV
            
               /*A
               ** Si Erreur de modification
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
         
         /*A
         ** La machine maitre n'existe pas comme equipement
         */
         
         else
            
            /*A
            ** Affectation du parametre de sortie Resultat a XDC_ECHEC
            */
         
            select @va_Resultat_out = XDC_ECHEC
      end
#endif      
      
   end
   end
      
   return XDC_OK
         
go  
