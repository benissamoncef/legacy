/*E*/
/* Fichier : $Id: xzao187sp.prc,v 1.6 2018/11/07 10:42:17 devgfi Exp $        $Revision: 1.6 $        $Date: 2018/11/07 10:42:17 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao187sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer	04 Nov 1996	: Creation (DEM/1232)
* JMG 		13/11/07	: ajout site de gestion DEM715  1.2
* LCL		11/07/18	: correction syntaxe from  1.4
* JPL		19/10/18 	: Nommage des colonnes sur 'insert'  1.5
* JPL		07/11/18 	: Degroupee; Gestion du champ 'Supprime' (DEM 1305)  1.6
* GGY		17/10/23	: Correction si CFG vide (DEM483)
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao187sp.h" */

/* static char *version = "@(#)xzao187.c	1.1 20/09/94 : xzao187" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Ajout d'un equipement generique LCR dans la base
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

if exists (select 1 from sysobjects where name = 'XZAO187' and type = 'P')
	drop procedure XZAO187
go


create procedure XZAO187
   @va_Basedd_in        	char(3),
   @va_NomEqt_in		char(10),
   @va_Autoroute_in		char(4),
   @va_Serveur_in		char(10),
   @va_PR_in			T_PR,
   @va_Sens_in			T_SENS,
   @va_Type_in			tinyint,
   @va_LiaisonMaitre_in		tinyint,
   @va_LiaisonEsclave_in	tinyint,
   @va_RGS_in			char(3),
   @va_Code_in			tinyint,
   @va_NomSite_in		char(2),
   @va_Supprime_in		bit,
   @va_Resultat_out		int output
   as 

/* 
*
* SP	XZAO187SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd		va_Basedd_in
* XDY_NomEqt		va_NomEqt_in
* XDY_NomAuto		va_Autoroute_in
* XDY_NomEqt		va_Serveur_in
* XDY_PR		va_PR_in
* XDY_Sens		va_Sens_in
* XDY_Octet		va_Type_in
* XDY_Octet		va_LiaisonMaitre_in
* XDY_Octet		va_LiaisonEsclave_in
* XDY_Rgs		va_RGS_in
* XDY_Octet		va_Code_in
* XDY_NomSite		va_NomSite_in
* XDY_Booleen		va_Supprime_in
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf	   	va_Resultat_out	
*
* VALEUR RENDUE :
*
* Ajout d'un equipement generique LCR en base
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
* Insertion ou modification dans la table EQT_LCR (Equipement generique LCR)
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
   @vl_Site			tinyint,
   @vl_SiteNom		char(10)
   
   
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
   or @va_Type_in = null 
   or @va_LiaisonMaitre_in = null 
   or @va_LiaisonEsclave_in = null 
   or @va_Code_in = null 
   or @va_NomSite_in = null
   or @va_Supprime_in = null
   
      return XDC_ARG_INV
   
   else   
   begin
      
		/* Verification que le site est bien celui du CI ou CA */
		select  @vl_SiteNom = @@servername
	   
		/* Si le site est CI ou CA */
		if @vl_SiteNom=XDC_SQLCI or @vl_SiteNom=XDC_SQLCA
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
            
	   select @vl_Site = numero from CFG..RES_DIS
	   where code = @va_NomSite_in

	       if @vl_Site = NULL
		       return XDC_ARG_INV

            /*A
            ** L'autoroute et le serveur existent
            */
                     
            if @vl_Autoroute <> null and @vl_Serveur <> null
            begin
                
                select @vl_NumEqt = numero from CFG..EQT_GEN
                  where type = @va_Type_in
                    and nom  = @va_NomEqt_in  
                
               /*A
               ** L'equipement generique LCR n'existe pas dans EQT_GEN
               */
               
               if @vl_NumEqt = null
               begin
               
                  /*A
                  ** Insertion de l'equipement generique LCR dans la table EQT_GEN
                  */
                  
                  select @vl_Numero = max(numero) from CFG..EQT_LCR

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
                  values ( @va_Type_in, @vl_Numero + 1, @vl_Serveur, XDC_EQT_SRV, @vl_Autoroute, @va_PR_in, @va_Sens_in, @va_NomEqt_in, @vl_Site, @va_Supprime_in)
                  
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
                  ** Insertion de l'equipement generique LCR dans la table EQT_LCR
                  */
                  
                  insert into CFG..EQT_LCR
                  values ( @vl_Numero + 1, @va_Type_in, @va_RGS_in, @va_LiaisonMaitre_in, @va_LiaisonEsclave_in, @va_Code_in )
                  
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
               ** L'equipement generique LCR existe dans EQT_GEN
               */
                     
               begin
               
                  /*A
                  ** Modification de l'equipement generique LCR dans la table EQT_GEN
                  */
                  
                  update CFG..EQT_GEN
                  set maitre      = @vl_Serveur,
                      type_maitre = XDC_EQT_SRV,
                      autoroute   = @vl_Autoroute,
                      PR          = @va_PR_in,
                      sens        = @va_Sens_in,
		      site_gestion = @vl_Site,
		      eqt_supprime = @va_Supprime_in
                  where numero = @vl_NumEqt
                    and type   = @va_Type_in
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
                  ** Modification de l'equipement generique LCR dans la table EQT_LCR
                  */
                  
                  update CFG..EQT_LCR
                  set type    = @va_Type_in,
                      rgs     = @va_RGS_in,
                      liaison_serie_maitre    = @va_LiaisonMaitre_in,
                      liaison_serie_esclave   = @va_LiaisonEsclave_in,
                      code    = @va_Code_in
                  where numero = @vl_NumEqt
                     and type  = @va_Type_in
               
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
            
	 select @vl_Site = numero from CFT..RES_DIS
		 where code = @va_NomSite_in

	     if @vl_Site = NULL
		     return XDC_ARG_INV

            /*A
            ** L'autoroute et le serveur existent
            */
                     
            if @vl_Autoroute <> null and @vl_Serveur <> null
            begin
                
                select @vl_NumEqt = numero from CFT..EQT_GEN
                  where type = @va_Type_in 
                    and nom  = @va_NomEqt_in  
                
               /*A
               ** L'equipement generique LCR n'existe pas dans EQT_GEN
               */
               
               if @vl_NumEqt = null
               begin
               
                  /*A
                  ** Insertion de l'equipement generique LCR dans la table EQT_GEN
                  */
                  
                  select @vl_Numero = max(numero) from CFT..EQT_LCR
              
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
                  values ( @va_Type_in, @vl_Numero + 1, @vl_Serveur, XDC_EQT_SRV, @vl_Autoroute, @va_PR_in, @va_Sens_in, @va_NomEqt_in, @vl_Site, @va_Supprime_in)
                  
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
                  ** Insertion de l'equipement generique LCR dans la table EQT_LCR
                  */
                  
                  insert into CFT..EQT_LCR
                  values ( @vl_Numero + 1, @va_Type_in, @va_RGS_in, @va_LiaisonMaitre_in, @va_LiaisonEsclave_in, @va_Code_in )
                  
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
               ** L'equipement generique LCR existe dans EQT_GEN
               */
                     
               begin
               
                  /*A
                  ** Modification de l'equipement generique LCR dans la table EQT_GEN
                  */
                  
                  update CFT..EQT_GEN
                  set maitre      = @vl_Serveur,
                      type_maitre = XDC_EQT_SRV,
                      autoroute   = @vl_Autoroute,
                      PR          = @va_PR_in,
                      sens        = @va_Sens_in,
		      site_gestion = @vl_Site,
		      eqt_supprime = @va_Supprime_in
                  where numero = @vl_NumEqt
                    and type   = @va_Type_in
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
                  ** Modification de l'equipement generique LCR dans la table EQT_LCR
                  */
                  
                  update CFT..EQT_LCR
                  set type    = @va_Type_in,
                      rgs     = @va_RGS_in,
                      liaison_serie_maitre    = @va_LiaisonMaitre_in,
                      liaison_serie_esclave   = @va_LiaisonEsclave_in,
                      code    = @va_Code_in
                  where numero = @vl_NumEqt
                     and type  = @va_Type_in
               
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
