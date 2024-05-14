/*E*/
/* Fichier : $Id: xzao73sp.prc,v 1.8 2018/11/06 14:59:57 devgfi Exp $        $Revision: 1.8 $        $Date: 2018/11/06 14:59:57 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao73sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	19 Sep 1994	: Creation
*
* JMG   13/11/07	: ajout site de gestion DEM715  A.4
* LCL	05/07/17	: Modif PRA  1.5
* LCL	11/07/18	: correction syntaxe from  1.6
* JPL	18/10/18	: Nommage des colonnes sur 'insert'  1.7
* JPL	06/11/18	: Degroupee; Gestion du champ 'Supprime' (DEM 1305)  1.8
* GGY	26/09/23	:	Ajout district CA (DEM483)
* GGY	17/10/23	: Correction si CFG vide (DEM483)
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao73sp.h" */

/* static char *version = "@(#)xzao73.c	1.1 23/18/94 : xzao73" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Ajout d'une mini GTC dans la base
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

if exists (select 1 from sysobjects where name = 'XZAO73' and type = 'P')
	drop procedure XZAO73
go


create procedure XZAO73
   @va_Basedd_in        char(3),
   @va_NomMiniGTC_in	char(10),
   @va_NomServeur_in	char(10),
   @va_Liaison_in		tinyint,
   @va_Rgs_in			char(3),
   @va_AlarmeETOR_in	tinyint,
   @va_AlerteETOR_in	smallint,
   @va_NomETOR_in		char(10),
   @va_Supprime_in		bit,
   @va_Resultat_out		int output
   as 

/* 
*
* SP	XZAO73SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd		va_Basedd_in        		
* XDY_NomEqt    	va_NomMiniGTC_in
* XDY_NomEqt    	va_NomServeur_in
* XDY_Octet     	va_Liaison_in
* XDY_Rgs       	va_Rgs_in
* XDY_TypeAlarme	va_AlarmeETOR_in
* XDY_TypeAlerte	va_AlerteETOR_in
* XDY_NomEqt       	va_NomETOR_in
* XDY_Booleen		va_Supprime_in
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf	   va_Resultat_out	
*
* VALEUR RENDUE :
*
* Ajoute une mini GTC (ListeETOR = tableau[32] avce champs : alarme/alerte/rien
* et No d'alarme ou d'alerte en base
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
* Insertion ou modification dans les tables EQT_GTC, EQT_TOR et EQT_GEN (EQUIPEMENT)
*
* MECANISMES :
*
------------------------------------------------------*/
declare
   
   /*A
   ** Definition des variables globales
   */

   @vl_NumMiniGTC	smallint,
   @vl_Serveur		smallint,
   @vl_Numero		smallint,
   @vl_Num			smallint,
   @vl_NumGT		smallint,
   @vl_NumBit		tinyint,
   @vl_NumTOR		smallint,
   @vl_Site			char(10)
   
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_Basedd_in = null
   or @va_NomMiniGTC_in = null 
   or @va_NomServeur_in = null 
   or @va_Liaison_in = null 
   or @va_AlarmeETOR_in = null
   or @va_AlerteETOR_in	= null
   or @va_Rgs_in = null
   or @va_Supprime_in = null
   
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
         
         /*A
         ** Verification de l'existence du serveur
         */
         
         select @vl_Serveur = numero from CFG..EQT_GEN
           where nom 	= @va_NomServeur_in
             and type 	= XDC_EQT_SRV
         
         /*B
         ** Le serveur existe parmi les equipements
         */
         
         if @vl_Serveur != null
         begin
            
            /*A 
            ** Insertion ou Modification de la Mini GTC dans EQT_GTC
            */
            
            select @vl_NumMiniGTC = numero from CFG..EQT_GEN
              where nom 	= @va_NomMiniGTC_in
               and type 	= XDC_EQT_GTC
         
            /*B
            ** La Mini GTC n'existe pas parmi les equipements
            */
         
            if @vl_NumMiniGTC = null
            begin
              
               select @vl_Numero = max(numero) from CFG..EQT_GEN
                 where type = XDC_EQT_GTC
                 
               /*A 
               ** Insertion de la Mini GTC dans EQT_GEN
               */
               
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
                             site_gestion,
                             eqt_supprime
                           )
               values ( XDC_EQT_GTC, @vl_Numero + 1, @vl_Serveur, XDC_MAI_GTC, null, null, null, @va_NomMiniGTC_in , null, @va_Supprime_in)
               
               /*A 
               ** Insertion de la Mini GTC dans EQT_GTC
               */
               
               insert into CFG..EQT_GTC
               values ( @vl_Numero + 1, @va_Rgs_in, @va_Liaison_in )
               
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
               
               select @va_Resultat_out = XDC_AJOUT
                  
           end
           
           else
                
           begin
              
               /*A 
               ** Update de la Mini GTC dans EQT_GEN
               */
               
               update CFG..EQT_GEN
               set maitre 	=  @vl_Serveur, 
                   type_maitre	= XDC_MAI_GTC,
                   eqt_supprime	= @va_Supprime_in
               where type 	= XDC_EQT_GTC
                 and numero 	= @vl_NumMiniGTC
               
               /*A 
               ** Update de la Mini GTC dans EQT_GTC
               */
               
               update CFG..EQT_GTC
               set rgs 			= @va_Rgs_in,
                   liaison_serie 	= @va_Liaison_in
               where numero = @vl_NumMiniGTC 
               
               /*A
               ** Erreur d'update
               */
                     
               if @@rowcount <> 1
               begin
                     
                  /*A
                  ** Affectation du parametre de sortie Resultat a XDC_MODIF
                  */
         
                  select @va_Resultat_out = XDC_MODIF
                        
                  return XDC_NOK
               end
               
               select @va_Resultat_out = XDC_MODIF
                        
                  
            end
               
            select @vl_NumTOR = numero from CFG..EQT_GEN
              where nom 	= @va_NomETOR_in
               and type 	= XDC_EQT_TOR
         
            select @vl_NumGT = numero from CFG..EQT_GEN
                 where type 	= XDC_EQT_GTC
                   and nom 	= @va_NomMiniGTC_in
            
            /*B
            ** La ETOR n'existe pas parmi les equipements
            */
         
            if @vl_NumTOR = null
            begin
             
               select @vl_Num = max(numero) from CFG..EQT_GEN
                 where type = XDC_EQT_TOR
            
               /*A 
               ** Insertion des TOR dans EQT_GEN
               */
               
               if @vl_Num = null
                  select @vl_Num = 0
                  
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
               values ( XDC_EQT_TOR, @vl_Num + 1, @vl_NumGT, XDC_MAI_TOR, null, null, null, @va_NomETOR_in, null, XDC_FAUX)
               
               /*A 
               ** Insertion de la Mini GTC dans EQT_TOR
               */
               
               select @vl_NumBit = max(numero_de_bit) from CFG..EQT_TOR
                 where numero = @vl_Num + 1
            
               /*A 
               ** Insertion des TOR dans EQT_GEN
               */
               
               if @vl_NumBit = null
                  select @vl_NumBit = 0
                  
               insert into CFG..EQT_TOR
               values ( @vl_Num + 1, @vl_NumBit + 1, @va_AlerteETOR_in, @va_AlarmeETOR_in )
               
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
            
            begin
              
               /*A 
               ** Update de l'ETOR dans EQT_GEN
               */
               
               update CFG..EQT_GEN
               set maitre 	= @vl_NumGT, 
                   type_maitre	= XDC_MAI_TOR,
                   nom		= @va_NomETOR_in
               where type 	= XDC_EQT_TOR
                 and numero 	= @vl_NumTOR
               
               /*A 
               ** Update de l'ETOR dans EQT_TOR
               */
               
               update CFG..EQT_TOR
               set alerte 		= @va_AlerteETOR_in,
                   alarme 		= @va_AlarmeETOR_in
               where numero = @vl_NumTOR 
               
               /*A
               ** Erreur d'update
               */
                     
               if @@rowcount <> 1
               begin
                     
                  /*A
                  ** Affectation du parametre de sortie Resultat a XDC_MODIF
                  */
         
                  select @va_Resultat_out = XDC_MODIF
                        
                  return XDC_NOK
               end
               
               select @va_Resultat_out = XDC_MODIF
                        
                  
            end
                  
         end  
            
         else
         
            /*A
            ** Le serveur n'existe pas parmi les equipements
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
         
         /*A
         ** Verification de l'existence du serveur
         */
         
         select @vl_Serveur = numero from CFT..EQT_GEN
           where nom 	= @va_NomServeur_in
             and type 	= XDC_EQT_SRV
         
         /*B
         ** Le serveur existe parmi les equipements
         */
         
         if @vl_Serveur != null
         begin
            
            /*A 
            ** Insertion ou Modification de la Mini GTC dans EQT_GTC
            */
            
            select @vl_NumMiniGTC = numero from CFT..EQT_GEN
              where nom 	= @va_NomMiniGTC_in
               and type 	= XDC_EQT_GTC
         
            /*B
            ** La Mini GTC n'existe pas parmi les equipements
            */
         
            if @vl_NumMiniGTC = null
            begin
              
               select @vl_Numero = max(numero) from CFT..EQT_GEN
                 where type = XDC_EQT_GTC
                 
               /*A 
               ** Insertion de la Mini GTC dans EQT_GEN
               */
               
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
                             site_gestion,
                             eqt_supprime
                           )
               values ( XDC_EQT_GTC, @vl_Numero + 1, @vl_Serveur, XDC_MAI_GTC, null, null, null, @va_NomMiniGTC_in, null, @va_Supprime_in)
               
               /*A 
               ** Insertion de la Mini GTC dans EQT_GTC
               */
               
               insert into CFT..EQT_GTC
               values ( @vl_Numero + 1, @va_Rgs_in, @va_Liaison_in )
               
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
               
               select @va_Resultat_out = XDC_AJOUT
                  
           end
           
           else
                
           begin
              
               /*A 
               ** Update de la Mini GTC dans EQT_GEN
               */
               
               update CFT..EQT_GEN
               set maitre 	=  @vl_Serveur, 
                   type_maitre	= XDC_MAI_GTC,
                   eqt_supprime	= @va_Supprime_in
               where type 	= XDC_EQT_GTC
                 and numero 	= @vl_NumMiniGTC
               
               /*A 
               ** Update de la Mini GTC dans EQT_GTC
               */
               
               update CFT..EQT_GTC
               set rgs 			= @va_Rgs_in,
                   liaison_serie 	= @va_Liaison_in
               where numero = @vl_NumMiniGTC 
               
               /*A
               ** Erreur d'update
               */
                     
               if @@rowcount <> 1
               begin
                     
                  /*A
                  ** Affectation du parametre de sortie Resultat a XDC_MODIF
                  */
         
                  select @va_Resultat_out = XDC_MODIF
                        
                  return XDC_NOK
               end
               
               select @va_Resultat_out = XDC_MODIF
                        
                  
            end
               
            select @vl_NumTOR = numero from CFT..EQT_GEN
              where nom 	= @va_NomETOR_in
               and type 	= XDC_EQT_TOR
         
            select @vl_NumGT = numero from CFT..EQT_GEN
             where type 	= XDC_EQT_GTC
               and nom 	= @va_NomMiniGTC_in
            
            /*B
            ** La ETOR n'existe pas parmi les equipements
            */
         
            if @vl_NumTOR = null
            begin
             
               select @vl_Num = max(numero) from CFT..EQT_GEN
                 where type = XDC_EQT_TOR
            
               /*A 
               ** Insertion des TOR dans EQT_GEN
               */
               
               if @vl_Num = null
                  select @vl_Num = 0
                  
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
               values ( XDC_EQT_TOR, @vl_Num + 1, @vl_NumGT, XDC_MAI_TOR, null, null, null, @va_NomETOR_in, null, XDC_FAUX)
               
               /*A 
               ** Insertion de la Mini GTC dans EQT_TOR
               */
               
               select @vl_NumBit = max(numero_de_bit) from CFT..EQT_TOR
                 where numero = @vl_Num + 1
            
               /*A 
               ** Insertion des TOR dans EQT_GEN
               */
               
               if @vl_NumBit = null
                  select @vl_NumBit = 0
                  
               insert into CFT..EQT_TOR
               values ( @vl_Num + 1, @vl_NumBit + 1, @va_AlerteETOR_in, @va_AlarmeETOR_in )
               
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
            
            begin
              
               /*A 
               ** Update de l'ETOR dans EQT_GEN
               */
               
               update CFT..EQT_GEN
               set maitre 	= @vl_NumGT, 
                   type_maitre	= XDC_MAI_TOR,
                   nom		= @va_NomETOR_in
               where type 	= XDC_EQT_TOR
                 and numero 	= @vl_NumTOR
               
               /*A 
               ** Update de l'ETOR dans EQT_TOR
               */
               
               update CFT..EQT_TOR
               set alerte 		= @va_AlerteETOR_in,
                   alarme 		= @va_AlarmeETOR_in
               where numero = @vl_NumTOR 
               
               /*A
               ** Erreur d'update
               */
                     
               if @@rowcount <> 1
               begin
                     
                  /*A
                  ** Affectation du parametre de sortie Resultat a XDC_MODIF
                  */
         
                  select @va_Resultat_out = XDC_MODIF
                        
                  return XDC_NOK
               end
               
               select @va_Resultat_out = XDC_MODIF
                        
                  
            end
                  
         end  
            
         else
         
            /*A
            ** Le serveur n'existe pas parmi les equipements
            ** Affectation du parametre de sortie Resultat a XDC_ECHEC
            */
         
               select @va_Resultat_out = XDC_ECHEC
               
      end


#endif    
    
    end  
    
    else
      
      /*A
      ** Affectation du parametre de sortie Resultat a XDC_ECHEC
      */
         
      select @va_Resultat_out = XDC_ECHEC 
   
   end   
   return XDC_OK
         
go  
