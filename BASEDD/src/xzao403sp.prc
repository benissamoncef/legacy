/*E*/
/* Fichier : $Id: xzao403sp.prc,v 1.9 2018/11/07 19:39:52 devgfi Exp $        $Revision: 1.9 $        $Date: 2018/11/07 19:39:52 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao403sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
* Mismer	06 Fev 1996	: Creation (DEM/1395)  1.1, 1.2
* JMG   	13/11/07	: ajout site de gestion DEM715  1.3
* XXX		xx/09/17	: Modif PRA  1.4
* JPL		01/10/17	: Degroupee  1.5
* JPL		01/10/17	: Colonne 'code_zone' de type smallint; parametre de code en accord (DEM 1236);
* 				     		maintien de la colonne 'code' POUR PHASE TRANSITOIRE  1.6
* JPL		15/11/17	: Suppression colonne 'code' (DEM 1236)  1.7
* JPL		17/10/18	: Nommage des colonnes sur 'insert'  1.8
* JPL		17/10/18	: Ajout du parametre 'supprime' (DEM 1305)  1.9
* GGY		26/09/23	: Ajout district CA (DEM483)
* GGY		17/10/23	: Correction si CFG vide (DEM483)
------------------------------------------------------*/

/* fichiers inclus */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Ajout d'un equipement zone temps de parcours dans la base
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

if exists (select 1 from sysobjects where name = 'XZAO403' and type = 'P')
	drop procedure XZAO403
go


create procedure XZAO403
   @va_Basedd_in        	char(3),
   @va_NomEqt_in		char(10),
   @va_Autoroute_in		char(4),
   @va_Maitre_in		char(10),
   @va_Site_in			char(2),
   @va_PR_in			T_PR,
   @va_Sens_in			T_SENS,
   @va_Libelle_in		char(25),
   @va_PR_Fin_in		T_PR,
   @va_Code_Zone_in		smallint,
   @va_TDP_habituel_in		smallint,
   @va_TDP_max_in		smallint,
   @va_Tolerance_haute_in	smallint,
   @va_Tolerance_basse_in	smallint,
   @va_Type_zone_in		bit,
   @va_Inhibition_in		bit,
   @va_Supprime_in		bit,
   @va_Resultat_out		int output
   as 

/* 
*
* SP	XZAO403SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd		va_Basedd_in
* XDY_NomEqt		va_NomEqt_in
* XDY_NomAuto		va_Autoroute_in
* XDY_NomEqt		va_Maitre_in
* XDY_NomSite		va_Site_in
* XDY_PR		va_PR_in
* XDY_Sens		va_Sens_in
* XDY_Nom		va_Libelle_in
* XDY_PR		va_PR_Fin_in
* XDY_Mot		va_Code_Zone_in
* XDY_Mot		va_TDP_habituel_in
* XDY_Mot		va_TDP_max_in
* XDY_Mot		va_Tolerance_haute_in
* XDY_Mot		va_Tolerance_basse_in
* XDY_Booleen		va_Type_zone_in
* XDY_Booleen		va_Inhibition_in
* XDY_Booleen		va_Supprime_in
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf	   	va_Resultat_out	
*
* VALEUR RENDUE :
*
* Ajout d'un equipement zone de parcours en base
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
* Insertion ou modification dans la table EQT_ZDP 
*
* MECANISMES :
*
------------------------------------------------------*/

declare
   
   /*A
   ** Definition des variables globales
   */

   @vl_Autoroute	tinyint,
   @vl_Site			tinyint,
   @vl_SiteNom		char(10),
   @vl_Maitre		smallint,
   @vl_NumEqt		smallint,
   @vl_Numero		smallint
   
   
   select @va_Resultat_out = XDC_ECHEC
   
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_Basedd_in = null  
   or @va_NomEqt_in = null 
   or @va_Autoroute_in = null 
   or @va_Maitre_in = null
   or @va_Site_in = null 
   or @va_PR_in = null 
   or @va_PR_Fin_in = null 
   or @va_Sens_in = null 
   or @va_Code_Zone_in = null
   or @va_Libelle_in = null
   or @va_TDP_habituel_in = null
   or @va_TDP_max_in = null
   or @va_Tolerance_haute_in = null
   or @va_Tolerance_basse_in = null
   or @va_Type_zone_in = null
   or @va_Inhibition_in = null
   or @va_Supprime_in = null
   
      return XDC_ARG_INV


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
                        
            select @vl_Site = numero from CFG..RES_DIS
              where code = @va_Site_in
                        
            select @vl_Maitre = numero from CFG..EQT_GEN
              where nom  = @va_Maitre_in
                and type = XDC_EQT_TDP
            
            /*A
            ** L'autoroute et le maitre et le site existent
            */
                     
            if @vl_Autoroute <> null and @vl_Maitre <> null and @vl_Site <> null
            begin
                
                select @vl_NumEqt = numero from CFG..EQT_GEN
                  where type = XDC_EQT_ZDP
                    and nom  = @va_NomEqt_in  
                
               /*A
               ** L'equipement ZDP n'existe pas dans EQT_GEN
               */
               
               if @vl_NumEqt = null
               begin
               
                  /*A
                  ** Insertion de l'equipement ZDP dans la table EQT_GEN
                  */
                  
                  select @vl_Numero = max(numero) from CFG..EQT_GEN
                    where type = XDC_EQT_ZDP

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
                  values ( XDC_EQT_ZDP,
                           @vl_Numero + 1,
                           @vl_Maitre,
                           XDC_EQT_TDP,
                           @vl_Autoroute,
                           @va_PR_in,
                           @va_Sens_in,
                           @va_NomEqt_in,
                           null,
                           @va_Supprime_in
                         )
                  
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
                  ** Insertion de la zone de parcours dans la table EQT_ZDP
                  */
                  
                  insert into CFG..EQT_ZDP
                         ( numero, sit, libelle, PR_fin, code_zone,
                  		TDP_habituel, TDP_max, tolerance_haute,
                  		tolerance_basse, type_zone, inhibition )
                  values ( @vl_Numero + 1, @vl_Site, @va_Libelle_in, @va_PR_Fin_in, @va_Code_Zone_in, 
                  		@va_TDP_habituel_in, @va_TDP_max_in, @va_Tolerance_haute_in, 
                  		@va_Tolerance_basse_in, @va_Type_zone_in, @va_Inhibition_in )
                  
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
               ** L'equipement ZDP existe dans EQT_GEN
               */
                     
               begin
               
                  /*A
                  ** Modification de l'equipement ZDP dans la table EQT_GEN
                  */
                  
                  update CFG..EQT_GEN
                  set maitre      = @vl_Maitre,
                      type_maitre = XDC_EQT_TDP,
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
                  ** Modification de l'equipement zone de parcours dans la table EQT_ZDP
                  */
                  
                  update CFG..EQT_ZDP
                  set sit     		= @vl_Site, 
                      libelle		= @va_Libelle_in, 
                      PR_fin		= @va_PR_Fin_in, 
                      code_zone		= @va_Code_Zone_in, 
                      TDP_habituel	= @va_TDP_habituel_in, 
                      TDP_max		= @va_TDP_max_in, 
                      tolerance_haute	= @va_Tolerance_haute_in, 
                      tolerance_basse	= @va_Tolerance_basse_in, 
                      type_zone		= @va_Type_zone_in, 
                      inhibition	= @va_Inhibition_in
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
                        
            select @vl_Site = numero from CFT..RES_DIS
              where code = @va_Site_in
                        
            select @vl_Maitre = numero from CFT..EQT_GEN
              where nom  = @va_Maitre_in
                and type = XDC_EQT_TDP
            
            /*A
            ** L'autoroute et le maitre et le site existent
            */
                     
            if @vl_Autoroute <> null and @vl_Maitre <> null and @vl_Site <> null
            begin
                
                select @vl_NumEqt = numero from CFT..EQT_GEN
                  where type = XDC_EQT_ZDP
                    and nom  = @va_NomEqt_in  
                
               /*A
               ** L'equipement ZDP n'existe pas dans EQT_GEN
               */
               
               if @vl_NumEqt = null
               begin
               
                  /*A
                  ** Insertion de l'equipement ZDP dans la table EQT_GEN
                  */
                  
                  select @vl_Numero = max(numero) from CFT..EQT_GEN
                    where type = XDC_EQT_ZDP

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
                  values ( XDC_EQT_ZDP,
                           @vl_Numero + 1,
                           @vl_Maitre,
                           XDC_EQT_TDP,
                           @vl_Autoroute,
                           @va_PR_in,
                           @va_Sens_in,
                           @va_NomEqt_in,
                           null,
                           @va_Supprime_in
                         )
                  
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
                  ** Insertion de la zone de parcours dans la table EQT_ZDP
                  */
                  
                  insert into CFT..EQT_ZDP
                         ( numero, sit, libelle, PR_fin, code_zone,
                  		TDP_habituel, TDP_max, tolerance_haute,
                  		tolerance_basse, type_zone, inhibition )
                  values ( @vl_Numero + 1, @vl_Site, @va_Libelle_in, @va_PR_Fin_in, @va_Code_Zone_in, 
                  		@va_TDP_habituel_in, @va_TDP_max_in, @va_Tolerance_haute_in, 
                  		@va_Tolerance_basse_in, @va_Type_zone_in, @va_Inhibition_in )
                  
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
               ** L'equipement ZDP existe dans EQT_GEN
               */
                     
               begin
               
                  /*A
                  ** Modification de l'equipement ZDP dans la table EQT_GEN
                  */
                  
                  update CFT..EQT_GEN
                  set maitre      = @vl_Maitre,
                      type_maitre = XDC_EQT_TDP,
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
                  ** Modification de l'equipement zone de parcours dans la table EQT_ZDP
                  */
                  
                  update CFT..EQT_ZDP
                  set sit     		= @vl_Site, 
                      libelle		= @va_Libelle_in, 
                      PR_fin		= @va_PR_Fin_in, 
                      code_zone		= @va_Code_Zone_in, 
                      TDP_habituel	= @va_TDP_habituel_in, 
                      TDP_max		= @va_TDP_max_in, 
                      tolerance_haute	= @va_Tolerance_haute_in, 
                      tolerance_basse	= @va_Tolerance_basse_in, 
                      type_zone		= @va_Type_zone_in, 
                      inhibition	= @va_Inhibition_in
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

   return XDC_OK
go  
