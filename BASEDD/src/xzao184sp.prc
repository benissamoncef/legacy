/*E*/
/* Fichier : $Id: xzao184sp.prc,v 1.10 2018/11/07 17:24:43 devgfi Exp $        $Revision: 1.10 $        $Date: 2018/11/07 17:24:43 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao184sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	19 Sep 1994	: Creation
* volcic	30 Nov 1994	: Ajout #ifdef CI (v1.2)
* volcic	05 Fev 1997	: Ajout colonne voie et inverse (DEM/1373) (v1.3)
* JMG   13/11/07        : ajout site de gestion DEM715  1.4
* JPL	11/10/18	: Restauration du code de test dans CFT au CI seulement  1.8
* JPL	19/10/18 : Nommage des colonnes sur 'insert'  1.9
* JPL	07/11/18 : Dégroupée; Gestion du champ 'Supprime' (DEM 1305)  1.10
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao184sp.h" */

/* static char *version = "$Id: xzao184sp.prc,v 1.10 2018/11/07 17:24:43 devgfi Exp $ : xzao184"; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Ajout d'un panneau GTC dans la base
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

if exists (select 1 from sysobjects where name = 'XZAO184' and type = 'P')
	drop procedure XZAO184
go


create procedure XZAO184
   @va_Basedd_in        	char(3),
   @va_NomPnGTC_in		char(10),
   @va_Autoroute_in		char(4),
   @va_NomMaitre_in		char(10),
   @va_PR_in			T_PR,
   @va_Sens_in			T_SENS,
   @va_Voie_in			T_VOIE,
   @va_Inverse_in		tinyint,
   @va_RangComm_in		tinyint,
   @va_RangComm2_in		tinyint,
   @va_RangEtat_in		tinyint,
   @va_RangEtat2_in		tinyint,
   @va_BitDiscordance_in	tinyint,
   @va_BitDiscordance2_in	tinyint,
   @va_Supprime_in		bit,
   @va_Resultat_out		int output
   as 

/* 
*
* SP	XZAO184SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd		va_Basedd_in
* XDY_NomEqt		va_NomPnGTC_in
* XDY_NomAuto		va_Autoroute_in
* XDY_NomEqt		va_NomMaitre_in
* XDY_PR		va_PR_in
* XDY_Sens		va_Sens_in
* XDY_Voie		va_Voie_in
* XDY_Octet		va_Inverse_in
* XDY_Octet		va_RangComm_in
* XDY_Octet		va_RangComm2_in
* XDY_Octet		va_RangEtat_in
* XDY_Octet		va_RangEtat2_in
* XDY_Octet		va_BitDiscordance_in
* XDY_Octet		va_BitDiscordance2_in
* XDY_Booleen		va_Supprime_in
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf	   	va_Resultat_out	
*
* VALEUR RENDUE :
*
* Ajoute d'un panneau GTC en base
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
* Insertion ou modification dans la table EQT_PAN (EQUIPEMENT_PANNEAU)
*
* MECANISMES :
*
------------------------------------------------------*/
declare
   
   /*A
   ** Definition des variables globales
   */

   @vl_Autoroute	tinyint,
   @vl_Maitre		smallint,
   @vl_NumPAN		smallint,
   @vl_TypeSrv		int,
   @vl_Type		int,
   @vl_Numero		smallint
   
   
   select @va_Resultat_out = XDC_ECHEC
   
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_Basedd_in = null  
   or @va_NomPnGTC_in = null 
   or @va_Autoroute_in = null 
   or @va_NomMaitre_in = null
   or @va_PR_in = null 
   or @va_Sens_in = null 
   or @va_Voie_in = null 
   or @va_Inverse_in = null  
   or @va_RangComm_in = null  
   or @va_RangEtat_in = null 
   or @va_BitDiscordance_in = null 
   or @va_Supprime_in = null
   
      return XDC_ARG_INV
   


      /*A
      ** Verification que le serveur est bien celui du CI
      */
      
      /*if @@servername = XDC_SQLCI */
      /* USINE GTIE if @@servername = XDC_SQLDN */
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
                        
            select @vl_Maitre = numero, @vl_TypeSrv = type from CFG..EQT_GEN
              where nom  = @va_NomMaitre_in
                and (type = XDC_EQT_TUB or type = XDC_EQT_ECH) 
            
            /*A
            ** L'autoroute et le serveur existent
            */
                     
            if @vl_Autoroute <> null and @vl_Maitre <> null
            begin
             
               if @vl_TypeSrv = XDC_EQT_TUB
                 select @vl_Type = XDC_MAI_PAN_TUB
               else  
                 select @vl_Type = XDC_MAI_PAN_ECH
                 
               select @vl_NumPAN = numero from CFG..EQT_GEN
                where type = XDC_EQT_PAN
                  and nom  = @va_NomPnGTC_in
                
               /*A
               ** Le panneau GTC n'existe pas dans EQT_GEN
               */
               
               if @vl_NumPAN = null
               begin
               
                  /*A
                  ** Insertion du demi echangeur dans la table EQT_GEN
                  */
                  
                  select @vl_Numero = max(numero) from CFG..EQT_GEN
                    where type = XDC_EQT_PAN
              
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
                  values ( XDC_EQT_PAN, @vl_Numero + 1, @vl_Maitre, @vl_Type, @vl_Autoroute, @va_PR_in, @va_Sens_in, @va_NomPnGTC_in, null, @va_Supprime_in )
                  
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
                  ** Insertion du panneau GTC dans la table EQT_PAN
                  */
                  
                  insert into CFG..EQT_PAN
                  values ( @vl_Numero + 1, @va_RangComm_in, @va_RangComm2_in, @va_RangEtat_in, @va_RangEtat2_in, @va_BitDiscordance_in,@va_BitDiscordance2_in, @va_Voie_in, @va_Inverse_in )
                  
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
                  set maitre      = @vl_Maitre,
                      type_maitre = @vl_Type,
                      autoroute   = @vl_Autoroute,
                      PR          = @va_PR_in,
                      eqt_supprime = @va_Supprime_in
                  where numero = @vl_NumPAN
                    and type   = XDC_EQT_PAN
                    and nom    = @va_NomPnGTC_in
               
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
                  ** Modification du panneau GTC dans la table EQT_PAN
                  */
                  
                  update CFG..EQT_PAN
                  set voie       = @va_Voie_in,
                      inverse    = @va_Inverse_in,
                      rang_comm  = @va_RangComm_in,
                      rang_comm2 = @va_RangComm2_in,
                      rang_etat  = @va_RangEtat_in,
                      rang_etat2 = @va_RangEtat2_in,
                      bit_disc   = @va_BitDiscordance_in,
                      bit_disc2  = @va_BitDiscordance2_in
                  where numero = @vl_NumPAN
               
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
            ** Le maitre ou l'autoroute n'existe pas dans EQT_GEN et RES_AUT
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
            ** Verification de l'existence de l'autoroute et du serveur comme equipement
            */
         
            select @vl_Autoroute = numero from CFT..RES_AUT
              where nom = @va_Autoroute_in
                        
            select @vl_Maitre = numero, @vl_TypeSrv = type from CFT..EQT_GEN
              where nom  = @va_NomMaitre_in
                and (type = XDC_EQT_TUB or type = XDC_EQT_ECH) 
            
            /*A
            ** L'autoroute et le serveur existent
            */
                     
            if @vl_Autoroute <> null and @vl_Maitre <> null
            begin
             
               if @vl_TypeSrv = XDC_EQT_TUB
                 select @vl_Type = XDC_MAI_PAN_TUB
               else  
                 select @vl_Type = XDC_MAI_PAN_ECH
                 
               select @vl_NumPAN = numero from CFT..EQT_GEN
                where type = XDC_EQT_PAN
                  and nom  = @va_NomPnGTC_in
                
               /*A
               ** Le panneau GTC n'existe pas dans EQT_GEN
               */
               
               if @vl_NumPAN = null
               begin
               
                  /*A
                  ** Insertion du demi echangeur dans la table EQT_GEN
                  */
                  
                  select @vl_Numero = max(numero) from CFT..EQT_GEN
                    where type = XDC_EQT_PAN
              
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
                  values ( XDC_EQT_PAN, @vl_Numero + 1, @vl_Maitre, @vl_Type, @vl_Autoroute, @va_PR_in, @va_Sens_in, @va_NomPnGTC_in , null, @va_Supprime_in)
                  
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
                  ** Insertion du panneau GTC dans la table EQT_PAN
                  */
                  
                  insert into CFT..EQT_PAN
                  values ( @vl_Numero + 1, @va_RangComm_in, @va_RangComm2_in, @va_RangEtat_in, @va_RangEtat2_in, @va_BitDiscordance_in,@va_BitDiscordance2_in, @va_Voie_in, @va_Inverse_in )
                  
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
                  set maitre      = @vl_Maitre,
                      type_maitre = @vl_Type,
                      autoroute   = @vl_Autoroute,
                      PR          = @va_PR_in,
                      eqt_supprime = @va_Supprime_in
                  where numero = @vl_NumPAN
                    and type   = XDC_EQT_PAN
                    and nom    = @va_NomPnGTC_in
               
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
                  ** Modification du panneau GTC dans la table EQT_PAN
                  */
                  
                  update CFT..EQT_PAN
                  set voie       = @va_Voie_in,
                      inverse    = @va_Inverse_in,
                      rang_comm  = @va_RangComm_in,
                      rang_comm2 = @va_RangComm2_in,
                      rang_etat  = @va_RangEtat_in,
                      rang_etat2 = @va_RangEtat2_in,
                      bit_disc   = @va_BitDiscordance_in,
                      bit_disc2  = @va_BitDiscordance2_in
                  where numero = @vl_NumPAN
               
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
            ** Le maitre ou l'autoroute n'existe pas dans EQT_GEN et RES_AUT
            ** Affectation du parametre de sortie Resultat a XDC_ECHEC
            */
            
               select @va_Resultat_out = XDC_ECHEC
               
         end

#endif
         
         
      end

   return XDC_OK
         
go  
