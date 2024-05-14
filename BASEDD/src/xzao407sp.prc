/*E*/
/* Fichier : $Id: xzao407sp.prc,v 1.7 2018/11/07 11:10:53 devgfi Exp $        $Revision: 1.7 $        $Date: 2018/11/07 11:10:53 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao407sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer	06 Fev 1996	: Creation (DEM/1395)
* JMG		13/11/07	: ajout site de gestion DEM715  1.3
* LCL		11/07/18	: correction syntaxe from  1.5
* JPL		19/10/18	: Nommage des colonnes sur 'insert'  1.6
* JPL		07/11/18	: Degroupee; Gestion du champ 'Supprime' (DEM 1305)  1.7
* GGY		26/09/23	: Ajout district CA (DEM483)
* GGY		17/10/23	: Correction si CFG vide (DEM483)
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao407sp.h" */

/* static char *version = "@(#)xzao407.c	1.1 20/09/94 : xzao407" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Ajout d'un equipement UGTP temps de parcours dans la base
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

if exists (select 1 from sysobjects where name = 'XZAO407' and type = 'P')
	drop procedure XZAO407
go


create procedure XZAO407
   @va_Basedd_in        	char(3),
   @va_NomEqt_in		char(10),
   @va_Autoroute_in		char(4),
   @va_Serveur_in		char(10),
   @va_PR_in			T_PR,
   @va_Sens_in			T_SENS,
   @va_Liaison_in		tinyint,
   @va_RGS_in			char(3),
   @va_Supprime_in		bit,
   @va_Resultat_out		int output
   as 

/* 
*
* SP	XZAO407SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd		va_Basedd_in
* XDY_NomEqt		va_NomEqt_in
* XDY_NomAuto		va_Autoroute_in
* XDY_NomEqt		va_Serveur_in
* XDY_PR		va_PR_in
* XDY_Sens		va_Sens_in
* XDY_Octet		va_Liaison_in
* XDY_Rgs		va_RGS_in
* XDY_Booleen		va_Supprime_in
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf	   	va_Resultat_out	
*
* VALEUR RENDUE :
*
* Ajout d'un equipement TDP (UGTP) en base
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
* Insertion ou modification dans la table EQT_TDP (UGTP)
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
   @vl_NumEqt		smallint,
   @vl_Numero		smallint,
   @vl_Site			char(10)
   
   
   select @va_Resultat_out = XDC_ECHEC
   
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_Basedd_in = null  
   or @va_NomEqt_in = null 
   or @va_Autoroute_in = null 
   or @va_Serveur_in = null
   or @va_PR_in = null 
   or @va_Sens_in = null 
   or @va_Liaison_in = null 
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
                        
            select @vl_Serveur = numero from CFG..EQT_GEN
              where nom  = @va_Serveur_in
                and type = XDC_EQT_SRV
            
            /*A
            ** L'autoroute et le serveur existent
            */
                     
            if @vl_Autoroute <> null and @vl_Serveur <> null
            begin
                
                select @vl_NumEqt = numero from CFG..EQT_GEN
                  where type = XDC_EQT_TDP
                    and nom  = @va_NomEqt_in  
                
               /*A
               ** L'equipement TDP (UGTP) n'existe pas dans EQT_GEN
               */
               
               if @vl_NumEqt = null
               begin
               
                  /*A
                  ** Insertion de l'equipement TDP (UGTP) dans la table EQT_GEN
                  */
                  
                  select @vl_Numero = max(numero) from CFG..EQT_GEN
                    where type = XDC_EQT_TDP

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
                  values ( XDC_EQT_TDP, @vl_Numero + 1, @vl_Serveur, XDC_EQT_SRV, @vl_Autoroute, @va_PR_in, @va_Sens_in, @va_NomEqt_in, null, @va_Supprime_in)
                  
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
                  ** Insertion de l'equipement TDP (UGTP) dans la table EQT_TDP
                  */
                  
                  insert into CFG..EQT_TDP
                  values ( @vl_Numero + 1, @va_RGS_in, @va_Liaison_in )
                  
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
               ** L'equipement TDP (UGTP) existe dans EQT_GEN
               */
                     
               begin
               
                  /*A
                  ** Modification de l'equipement TDP (UGTP) dans la table EQT_GEN
                  */
                  
                  update CFG..EQT_GEN
                  set maitre      = @vl_Serveur,
                      type_maitre = XDC_EQT_SRV,
                      autoroute   = @vl_Autoroute,
                      PR          = @va_PR_in,
                      sens        = @va_Sens_in,
                      eqt_supprime = @va_Supprime_in
                  where numero = @vl_NumEqt
                    and nom    = @va_NomEqt_in
               
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
                  ** Modification de l'equipement TDP (UGTP) dans la table EQT_TDP
                  */
                  
                  update CFG..EQT_TDP
                  set rgs     = @va_RGS_in,
                      liaison_serie    = @va_Liaison_in
                  where numero = @vl_NumEqt
               
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
                        
            select @vl_Serveur = numero from CFT..EQT_GEN
              where nom  = @va_Serveur_in
                and type = XDC_EQT_SRV
            
            /*A
            ** L'autoroute et le serveur existent
            */
                     
            if @vl_Autoroute <> null and @vl_Serveur <> null
            begin
                
                select @vl_NumEqt = numero from CFT..EQT_GEN
                  where type = XDC_EQT_TDP
                    and nom  = @va_NomEqt_in  
                
               /*A
               ** L'equipement TDP (UGTP) n'existe pas dans EQT_GEN
               */
               
               if @vl_NumEqt = null
               begin
               
                  /*A
                  ** Insertion de l'equipement TDP (UGTP) dans la table EQT_GEN
                  */
                  
                  select @vl_Numero = max(numero) from CFT..EQT_GEN
                    where type = XDC_EQT_TDP

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
                  values ( XDC_EQT_TDP, @vl_Numero + 1, @vl_Serveur, XDC_EQT_SRV, @vl_Autoroute, @va_PR_in, @va_Sens_in, @va_NomEqt_in, null, @va_Supprime_in)
                  
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
                  ** Insertion de l'equipement TDP (UGTP) dans la table EQT_TDP
                  */
                  
                  insert into CFT..EQT_TDP
                  values ( @vl_Numero + 1, @va_RGS_in, @va_Liaison_in )
                  
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
               ** L'equipement TDP (UGTP) existe dans EQT_GEN
               */
                     
               begin
               
                  /*A
                  ** Modification de l'equipement TDP (UGTP) dans la table EQT_GEN
                  */
                  
                  update CFT..EQT_GEN
                  set maitre      = @vl_Serveur,
                      type_maitre = XDC_EQT_SRV,
                      autoroute   = @vl_Autoroute,
                      PR          = @va_PR_in,
                      sens        = @va_Sens_in,
                      eqt_supprime = @va_Supprime_in
                  where numero = @vl_NumEqt
                    and nom    = @va_NomEqt_in
               
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
                  ** Modification de l'equipement TDP (UGTP) dans la table EQT_TDP
                  */
                  
                  update CFT..EQT_TDP
                  set rgs     = @va_RGS_in,
                      liaison_serie    = @va_Liaison_in
                  where numero = @vl_NumEqt
               
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
