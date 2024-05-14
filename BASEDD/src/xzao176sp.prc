/*E*/
/* Fichier : $Id: xzao176sp.prc,v 1.7 2018/10/19 15:08:32 devgfi Exp $        $Revision: 1.7 $        $Date: 2018/10/19 15:08:32 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao176sp.prc
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
* JMG   	13/11/07	: ajout site de gestion DEM715  1.4
* LCL		11/07/18	: correction syntaxe fro6
* JPL		19/10/18	: Nommage des colonnes sur 'insert'  1.7
* GGY		26/09/23	: Ajout district CA (DEM483)
* GGY		17/10/23	: Correction si CFG vide (DEM483)
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao176sp.h" */

/* static char *version = "@(#)xzao176.c	1.1 20/09/94 : xzao176" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Ajout d'un demi echangeur dans la base
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAO;176
   @va_Basedd_in        	char(3),
   @va_NomDemiEch_in		char(10),
   @va_Autoroute_in		char(4),
   @va_NomServeur_in		char(10),
   @va_PR_in			T_PR,
   @va_Sens_in			T_SENS,
   @va_AdresseEtat_in		smallint,
   @va_AdresseCommande_in	smallint,
   @va_AdresseDiscordance_in	smallint,
   @va_AdresseTelealarme_in	smallint,
   @va_Resultat_out		int output
   as 

/* 
*
* SP	XZAO176SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd		va_Basedd_in
* XDY_NomEqt		va_NomDemiEch_in
* XDY_NomAuto		va_Autoroute_in
* XDY_NomEqt		va_NomServeur_in
* XDY_PR		va_PR_in
* XDY_Sens		va_Sens_in
* XDY_Mot		va_AdresseEtat_in
* XDY_Mot		va_AdresseCommande_in
* XDY_Mot		va_AdresseDiscordance_in
* XDY_Mot		va_AdresseTelealarme_in
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf	   	va_Resultat_out	
*
* VALEUR RENDUE :
*
* Ajoute d'un demi echangeur en base
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
* Insertion ou modification dans la table EQT_ECH (EQUIPEMENT_ECHANGEUR)
*
* MECANISMES :
*
------------------------------------------------------*/
declare
   
   /*A
   ** Definition des variables globales
   */

   @vl_Autoroute	tinyint,
   @vl_Serveur		smallint,
   @vl_NumEch		smallint,
   @vl_Numero		smallint,
   @vl_Site			char(10)
   
   
   select @va_Resultat_out = XDC_LU
   
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_Basedd_in = null 
   or @va_NomDemiEch_in = null 
   or @va_Autoroute_in = null 
   or @va_NomServeur_in = null
   or @va_PR_in = null 
   or @va_Sens_in = null 
   or @va_AdresseEtat_in = null 
   or @va_AdresseCommande_in = null 
   or @va_AdresseDiscordance_in = null 
   or @va_AdresseTelealarme_in = null
   
      return XDC_ARG_INV
   
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
            ** Verification de l'existence de l'autoroute et du serveur comme equipement
            */
         
            select @vl_Autoroute = numero from CFG..RES_AUT
              where nom = @va_Autoroute_in
                        
            select @vl_Serveur = numero from CFG..EQT_GEN
              where nom  = @va_NomServeur_in
                and type = XDC_EQT_NIC
            
            /*A
            ** L'autoroute et la GTC Nice existent
            */
                     
            if @vl_Autoroute <> null and @vl_Serveur <> null
            begin
             
               select @vl_NumEch = numero from CFG..EQT_GEN
                where type = XDC_EQT_ECH
                  and nom  = @va_NomDemiEch_in
                
               /*A
               ** Le demi-echangeur n'existe pas dans EQT_GEN
               */
               
               if @vl_NumEch = null
               begin
               
                  /*A
                  ** Insertion du demi echangeur dans la table EQT_GEN
                  */
                  
                  select @vl_Numero = max(numero) from CFG..EQT_GEN
                    where type = XDC_EQT_ECH
              
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
                  values ( XDC_EQT_ECH, @vl_Numero + 1, @vl_Serveur, XDC_MAI_ECH, @vl_Autoroute, @va_PR_in, @va_Sens_in, @va_NomDemiEch_in , null)
                  
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
                  ** Insertion du demi echangeur dans la table EQT_ECH
                  */
                  
                  insert into CFG..EQT_ECH
                  values ( @vl_Numero + 1, @va_AdresseEtat_in, @va_AdresseCommande_in, @va_AdresseDiscordance_in, @va_AdresseTelealarme_in )
                  
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
               
               else
                
               /*A
               ** Le demi-echangeur existe dans EQT_GEN
               */
                     
               begin
               
                  /*A
                  ** Modification du demi echangeur dans la table EQT_GEN
                  */
                  
                  update CFG..EQT_GEN
                  set maitre      = @vl_Serveur,
                      type_maitre = XDC_MAI_ECH,
                      autoroute   = @vl_Autoroute,
                      PR          = @va_PR_in
                  where numero = @vl_NumEch
                    and type   = XDC_EQT_ECH
                    and nom    = @va_NomDemiEch_in
               
                  /*A
                  ** Si erreur de modification
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
                  ** Modification du demi echangeur dans la table EQT_ECH
                  */
                  
                  update CFG..EQT_ECH
                  set adresse_etat        = @va_AdresseEtat_in,
                      adresse_commande    = @va_AdresseCommande_in,
                      adresse_discordance = @va_AdresseDiscordance_in,
                      adresse_telealarme  = @va_AdresseTelealarme_in
                  where numero = @vl_NumEch
               
                  /*A
                  ** Si erreur de modification
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
                 
            /*A
            ** La GTC Nice ou l'autoroute n'existe pas dans EQT_GEN et RES_AUT
            ** Affectation du parametre de sortie Resultat a XDC_ECHEC
            */
            
               select @va_Resultat_out = XDC_ECHEC
               
         end
      
#ifdef CI      
      	 
      	 else
      
         /*A
         **  Travail dans la base CFG
         */   
      
         begin
         
            /*A
            ** Verification de l'existence de l'autoroute et du serveur comme equipement
            */
         
            select @vl_Autoroute = numero from CFT..RES_AUT
              where nom = @va_Autoroute_in
                        
            select @vl_Serveur = numero from CFT..EQT_GEN
              where nom  = @va_NomServeur_in
                and type = XDC_EQT_NIC
            
            /*A
            ** L'autoroute et le serveur existent
            */
                     
            if @vl_Autoroute <> null and @vl_Serveur <> null
            begin
             
               select @vl_NumEch = numero from CFT..EQT_GEN
                where type = XDC_EQT_ECH
                  and nom  = @va_NomDemiEch_in
                
               /*A
               ** Le demi-echangeur n'existe pas dans EQT_GEN
               */
               
               if @vl_NumEch = null
               begin
               
                  /*A
                  ** Insertion du demi echangeur dans la table EQT_GEN
                  */
                  
                  select @vl_Numero = max(numero) from CFT..EQT_GEN
                    where type = XDC_EQT_ECH
              
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
                  values ( XDC_EQT_ECH, @vl_Numero + 1, @vl_Serveur, XDC_MAI_ECH, @vl_Autoroute, @va_PR_in, @va_Sens_in, @va_NomDemiEch_in , null)
                  
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
                  ** Insertion du demi echangeur dans la table EQT_ECH
                  */
                  
                  insert into CFT..EQT_ECH
                  values ( @vl_Numero + 1, @va_AdresseEtat_in, @va_AdresseCommande_in, @va_AdresseDiscordance_in, @va_AdresseTelealarme_in )
                  
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
               
               else
                
               /*A
               ** Le demi-echangeur existe dans EQT_GEN
               */
                     
               begin
               
                  /*A
                  ** Modification du demi echangeur dans la table EQT_GEN
                  */
                  
                  update CFT..EQT_GEN
                  set maitre      = @vl_Serveur,
                      type_maitre = XDC_MAI_ECH,
                      autoroute   = @vl_Autoroute,
                      PR          = @va_PR_in
                  where numero = @vl_NumEch
                    and type   = XDC_EQT_ECH
                    and nom    = @va_NomDemiEch_in
               
                  /*A
                  ** Si erreur de modification
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
                  ** Modification du demi echangeur dans la table EQT_ECH
                  */
                  
                  update CFT..EQT_ECH
                  set adresse_etat        = @va_AdresseEtat_in,
                      adresse_commande    = @va_AdresseCommande_in,
                      adresse_discordance = @va_AdresseDiscordance_in,
                      adresse_telealarme  = @va_AdresseTelealarme_in
                  where numero = @vl_NumEch
               
                  /*A
                  ** Si erreur de modification
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
                 
            /*A
            ** Le serveur ou l'autoroute n'existe pas dans EQT_GEN et RES_AUT
            ** Affectation du parametre de sortie Resultat a XDC_ECHEC
            */
            
               select @va_Resultat_out = XDC_ECHEC
               
         end

#endif
      
      end
   end         
     
   return XDC_OK
         
go  
