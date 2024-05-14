/*E*/
/* Fichier : $Id: xzao180sp.prc,v 1.10 2018/11/06 21:40:02 devgfi Exp $        $Revision: 1.10 $        $Date: 2018/11/06 21:40:02 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao180sp.prc
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
* volcic	25 Jan 1994	: Modif insertion (v1.3)
* volcic	27 Jan 1994	: Modif insertion (v1.4)
* mismer	24 Avr 1997	: Ajout colonne alerte + type gabarit,vitesse et niche (v1.5) (DEM/1421)
* JMG   	13/11/07	: ajout site de gestion DEM715  1.6
* LCL		11/07/18	: correction syntaxe from  1.8
* JPL		19/10/18	: Nommage des colonnes sur 'insert'  1.9
* JPL		06/11/18	: Degroupee; Gestion du champ 'Supprime' (DEM 1305)  1.10
* GGY		26/09/23	: Ajout district CA (DEM483)
* GGY		17/10/23	: Correction si CFG vide (DEM483)
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao180sp.h" */

/* static char *version = "@(#)xzao180.c	1.1 20/09/94 : xzao180" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Ajout d'un capteur de tunnel dans la base
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

if exists (select 1 from sysobjects where name = 'XZAO180' and type = 'P')
	drop procedure XZAO180
go


create procedure XZAO180
   @va_Basedd_in        	char(3),
   @va_NomCapteur_in		char(10),
   @va_Autoroute_in		char(4),
   @va_NomTube_in		char(10),
   @va_PR_in			T_PR,
   @va_Sens_in			T_SENS,
   @va_Type_in			tinyint,
   @va_NumBit_in		tinyint,
   @va_NumAlerte_in		tinyint,
   @va_Supprime_in		bit,
   @va_Resultat_out		int output
   as 

/* 
*
* SP	XZAO180SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd		va_Basedd_in
* XDY_NomEqt		va_NomCapteur_in
* XDY_NomAuto		va_Autoroute_in
* XDY_NomEqt		va_NomTube_in
* XDY_PR		va_PR_in
* XDY_Sens		va_Sens_in
* XDY_Octet		va_Type_in
* XDY_Octet		va_NumBit_in
* XDY_Octet		va_NumAlerte_in
* XDY_Booleen		va_Supprime_in
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf	   	va_Resultat_out	
*
* VALEUR RENDUE :
*
* Ajoute d'un capteur de tunnel en base
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
* Insertion ou modification dans la table EQT_CPT (CAPTEUR_TUNNEL)
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
   @vl_NumCapteur	smallint,
   @vl_Numero		smallint,
   @vl_Site			char(10)
   
   
   select @va_Resultat_out = XDC_ECHEC
   
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_Basedd_in = null  
   or @va_NomCapteur_in = null 
   or @va_Autoroute_in = null 
   or @va_NomTube_in = null
   or @va_PR_in = null 
   or @va_Sens_in = null 
   or @va_Type_in = null 
   or @va_NumBit_in = null 
   or @va_NumAlerte_in = null 
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
                
                select @vl_NumCapteur = numero from CFG..EQT_GEN
                  where ( type = XDC_EQT_LIT
                    or  type = XDC_EQT_EXT
		    or  type = XDC_EQT_OPA
		    or  type = XDC_EQT_CCO
		    or  type = XDC_EQT_GAB
		    or  type = XDC_EQT_VIT
		    or  type = XDC_EQT_NICHE )
                    and nom  = @va_NomCapteur_in  
                
               /*A
               ** Le capteur n'existe pas dans EQT_GEN
               */
               
               if @vl_NumCapteur = null
               begin
               
                  /*A
                  ** Insertion du capteur de tunnel dans la table EQT_GEN
                  */
                  
                  select @vl_Numero = max(numero) from CFG..EQT_GEN
                    where type = XDC_EQT_LIT
                      or  type = XDC_EQT_EXT
		      or  type = XDC_EQT_OPA
		      or  type = XDC_EQT_CCO
		      or  type = XDC_EQT_GAB
		      or  type = XDC_EQT_VIT
		      or  type = XDC_EQT_NICHE

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
                  values ( @va_Type_in, @vl_Numero + 1, @vl_Tube, XDC_MAI_CPT, @vl_Autoroute, @va_PR_in, @va_Sens_in, @va_NomCapteur_in, null, @va_Supprime_in)
                  
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
                  ** Insertion du capteur de tunnel dans la table EQT_CPT
                  */
                  
                  insert into CFG..EQT_CPT
                  values ( @vl_Numero + 1, @va_Type_in, @va_NumBit_in, @va_NumAlerte_in )
                  
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
               ** Le capteur du tunnel existe dans EQT_GEN
               */
                     
               begin
               
                  /*A
                  ** Modification du capteur de tunnel dans la table EQT_GEN
                  */
                  
                  update CFG..EQT_GEN
                  set maitre      = @vl_Tube,
                      type_maitre = XDC_MAI_CPT,
                      autoroute   = @vl_Autoroute,
                      PR          = @va_PR_in,
                      eqt_supprime = @va_Supprime_in
                  where numero = @vl_NumCapteur
                    and type   = @va_Type_in
                    and nom    = @va_NomCapteur_in
               
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
                  ** Modification du capteur de tunnel dans la table EQT_CPT
                  */
                  
                  update CFG..EQT_CPT
                  set type       = @va_Type_in,
                      num_bit    = @va_NumBit_in,
                      num_alerte = @va_NumAlerte_in
                  where numero = @vl_NumCapteur
               
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
                
                select @vl_NumCapteur = numero from CFT..EQT_GEN
                  where ( type = XDC_EQT_LIT
		    or  type = XDC_EQT_EXT
		    or  type = XDC_EQT_OPA
		    or  type = XDC_EQT_CCO
		    or  type = XDC_EQT_GAB
		    or  type = XDC_EQT_VIT
		    or  type = XDC_EQT_NICHE )
                    and nom  = @va_NomCapteur_in  
                
               /*A
               ** Le capteur n'existe pas dans EQT_GEN
               */
               
               if @vl_NumCapteur = null
               begin
               
                  /*A
                  ** Insertion du capteur de tunnel dans la table EQT_GEN
                  */
                  
                  select @vl_Numero = max(numero) from CFT..EQT_GEN
                    where type = XDC_EQT_LIT
		      or  type = XDC_EQT_EXT
		      or  type = XDC_EQT_OPA
		      or  type = XDC_EQT_CCO
		      or  type = XDC_EQT_GAB
		      or  type = XDC_EQT_VIT
		      or  type = XDC_EQT_NICHE
              
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
                  values ( @va_Type_in, @vl_Numero + 1, @vl_Tube, XDC_MAI_CPT, @vl_Autoroute, @va_PR_in, @va_Sens_in, @va_NomCapteur_in, null, @va_Supprime_in)
                  
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
                  ** Insertion du tunnel dans la table EQT_CPT
                  */
                  
                  insert into CFT..EQT_CPT
                  values ( @vl_Numero + 1, @va_Type_in, @va_NumBit_in, @va_NumAlerte_in )
                  
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
               ** Le capteur du tunnel existe dans EQT_GEN
               */
                     
               begin
               
                  /*A
                  ** Modification du capteur de tunnel dans la table EQT_GEN
                  */
                  
                  update CFT..EQT_GEN
                  set maitre      = @vl_Tube,
                      type_maitre = XDC_MAI_CPT,
                      autoroute   = @vl_Autoroute,
                      PR          = @va_PR_in,
                      eqt_supprime = @va_Supprime_in
                  where numero = @vl_NumCapteur
                    and type   = @va_Type_in
                    and nom    = @va_NomCapteur_in
               
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
                  ** Modification du capteur de tunnel dans la table EQT_CPT
                  */
                  
                  update CFT..EQT_CPT
                  set type       = @va_Type_in,
                      num_bit    = @va_NumBit_in,
                      num_alerte = @va_NumAlerte_in
                  where numero = @vl_NumCapteur
               
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
