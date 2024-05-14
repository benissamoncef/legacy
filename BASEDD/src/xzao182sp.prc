/*E*/
/* Fichier : $Id: xzao182sp.prc,v 1.7 2018/11/07 10:06:08 devgfi Exp $        $Revision: 1.7 $        $Date: 2018/11/07 10:06:08 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao182sp.prc
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
* JMG   	13/11/07	: ajout site de gestion DEM715  1.3
* LCL		11/07/18	: correction syntaxe from  1.5
* JPL		19/10/18	: Nommage des colonnes sur 'insert'  1.6
* JPL		07/11/18	: Degroupee; Gestion du champ 'Supprime' (DEM 1305)  1.7
* GGY		26/09/23	: Ajout district CA (DEM483)
* GGY		17/10/23	: Correction si CFG vide (DEM483)
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao182sp.h" */

/* static char *version = "@(#)xzao182.c	1.1 20/09/94 : xzao182" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Ajout les feux d'affectation de voies d'un tunnel dans la base
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

if exists (select 1 from sysobjects where name = 'XZAO182' and type = 'P')
	drop procedure XZAO182
go


create procedure XZAO182
   @va_Basedd_in        	char(3),
   @va_NomFAV_in		char(10),
   @va_Autoroute_in		char(4),
   @va_NomTube_in		char(10),
   @va_PR_in			T_PR,
   @va_Sens_in			T_SENS,
   @va_Voie_in			tinyint,
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
* SP	XZAO182SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd		va_Basedd_in
* XDY_NomEqt		va_NomFAV_in
* XDY_NomAuto		va_Autoroute_in
* XDY_NomEqt		va_NomTube_in
* XDY_PR		va_PR_in
* XDY_Sens		va_Sens_in
* XDY_Voie		va_Voie_in
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
* Ajoute les feux d'affectation de voies d'un tunnel en base
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
* Insertion ou modification dans la table EQT_FAV (EQUIPEMENT_FAVANGEUR)
*
* MECANISMES :
*
------------------------------------------------------*/
declare
   
   /*A
   ** Definition des variables globales
   */

   @vl_Autoroute	tinyint,
   @vl_Tube			smallint,
   @vl_NumFAV		smallint,
   @vl_Numero		smallint,
   @vl_Site			char(10)
   
   
   select @va_Resultat_out = XDC_ECHEC
   
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_Basedd_in = null  
   or @va_NomFAV_in = null 
   or @va_Autoroute_in = null 
   or @va_NomTube_in = null
   or @va_PR_in = null 
   or @va_Sens_in = null 
   or @va_Voie_in = null 
   or @va_RangComm_in = null 
   or @va_RangEtat_in = null  
   or @va_BitDiscordance_in = null 
   or @va_Supprime_in = null
   
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
                        
            select @vl_Tube = numero from CFG..EQT_GEN
              where nom  = @va_NomTube_in
                and type = XDC_EQT_TUB
            
            /*A
            ** L'autoroute et le serveur existent
            */
                     
            if @vl_Autoroute <> null and @vl_Tube <> null
            begin
             
               select @vl_NumFAV = numero from CFG..EQT_GEN
                where type = XDC_EQT_FAV
                  and nom  = @va_NomFAV_in
                
               /*A
               ** Les FAV n'existent pas dans EQT_GEN
               */
               
               if @vl_NumFAV = null
               begin
               
                  /*A
                  ** Insertion du demi echangeur dans la table EQT_GEN
                  */
                  
                  select @vl_Numero = max(numero) from CFG..EQT_GEN
                    where type = XDC_EQT_FAV
              
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
                  values ( XDC_EQT_FAV, @vl_Numero + 1, @vl_Tube, XDC_MAI_FAV, @vl_Autoroute, @va_PR_in, @va_Sens_in, @va_NomFAV_in, null, @va_Supprime_in)
                  
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
                  ** Insertion des FAV dans la table EQT_FAV
                  */
                  
                  insert into CFG..EQT_FAV
                  values ( @vl_Numero + 1, @va_Voie_in, @va_RangComm_in, @va_RangComm2_in, @va_RangEtat_in, @va_RangEtat2_in, @va_BitDiscordance_in,@va_BitDiscordance2_in )
                  
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
                  ** Modification des FAV dans la table EQT_GEN
                  */
                  
                  update CFG..EQT_GEN
                  set maitre      = @vl_Tube,
                      type_maitre = XDC_MAI_FAV,
                      autoroute   = @vl_Autoroute,
                      PR          = @va_PR_in,
                      eqt_supprime = @va_Supprime_in
                  where numero = @vl_NumFAV
                    and type   = XDC_EQT_FAV
                    and nom    = @va_NomFAV_in
               
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
                  ** Modification des FAV dans la table EQT_FAV
                  */
                  
                  update CFG..EQT_FAV
                  set voie       = @va_Voie_in,
                      rang_comm  = @va_RangComm_in,
                      rang_comm2 = @va_RangComm2_in,
                      rang_etat  = @va_RangEtat_in,
                      rang_etat2 = @va_RangEtat2_in,
                      bit_disc   = @va_BitDiscordance_in,
                      bit_disc2  = @va_BitDiscordance2_in
                  where numero = @vl_NumFAV
               
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
            ** Les feux d'affectation des voies du tunnel ou l'autoroute n'existe pas dans EQT_GEN et RES_AUT
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
                        
            select @vl_Tube = numero from CFT..EQT_GEN
              where nom  = @va_NomTube_in
                and type = XDC_EQT_TUB
            
            /*A
            ** L'autoroute et le serveur existent
            */
                     
            if @vl_Autoroute <> null and @vl_Tube <> null
            begin
             
               select @vl_NumFAV = numero from CFT..EQT_GEN
                where type = XDC_EQT_FAV
                  and nom  = @va_NomFAV_in
                
               /*A
               ** Les FAV n'existent pas dans EQT_GEN
               */
               
               if @vl_NumFAV = null
               begin
               
                  /*A
                  ** Insertion du demi echangeur dans la table EQT_GEN
                  */
                  
                  select @vl_Numero = max(numero) from CFT..EQT_GEN
                    where type = XDC_EQT_FAV
              
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
                  values ( XDC_EQT_FAV, @vl_Numero + 1, @vl_Tube, XDC_MAI_FAV, @vl_Autoroute, @va_PR_in, @va_Sens_in, @va_NomFAV_in, null, @va_Supprime_in)
                  
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
                  ** Insertion des FAV dans la table EQT_FAV
                  */
                  
                  insert into CFT..EQT_FAV
                  values ( @vl_Numero + 1, @va_Voie_in, @va_RangComm_in, @va_RangComm2_in, @va_RangEtat_in, @va_RangEtat2_in, @va_BitDiscordance_in,@va_BitDiscordance2_in )
                  
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
                  ** Modification des FAV dans la table EQT_GEN
                  */
                  
                  update CFT..EQT_GEN
                  set maitre      = @vl_Tube,
                      type_maitre = XDC_MAI_FAV,
                      autoroute   = @vl_Autoroute,
                      PR          = @va_PR_in,
                      eqt_supprime = @va_Supprime_in
                  where numero = @vl_NumFAV
                    and type   = XDC_EQT_FAV
                    and nom    = @va_NomFAV_in
               
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
                  ** Modification des FAV dans la table EQT_FAV
                  */
                  
                  update CFT..EQT_FAV
                  set voie       = @va_Voie_in,
                      rang_comm  = @va_RangComm_in,
                      rang_comm2 = @va_RangComm2_in,
                      rang_etat  = @va_RangEtat_in,
                      rang_etat2 = @va_RangEtat2_in,
                      bit_disc   = @va_BitDiscordance_in,
                      bit_disc2  = @va_BitDiscordance2_in
                  where numero = @vl_NumFAV
               
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
            ** Les feux d'affectation des voies du tunnel ou l'autoroute n'existe pas dans EQT_GEN et RES_AUT
            ** Affectation du parametre de sortie Resultat a XDC_ECHEC
            */
            
               select @va_Resultat_out = XDC_ECHEC
               
         end
         
#endif
         
      end
   end         
     
   return XDC_OK
         
go  
