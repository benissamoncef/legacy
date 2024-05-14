/*E*/
/*Fichier : $Id: xzao816sp.prc,v 1.2 2018/01/09 15:20:36 pc2dpdy Exp $      Release : $Revision: 1.2 $        Date : $Date: 2018/01/09 15:20:36 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao816sp.prc
------------------------------------------------------
------------------------------------------------------
* HISTORIQUE :
* JMG   20/03/17        creation DEM1191
* LCL	05/07/17	: Modif PRA
* PNI 22/03/23 : Correction virgule presente en fin de select
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao816sp.h" */

/* static char *version = " $Id: xzao816sp.prc,v 1.2 2018/01/09 15:20:36 pc2dpdy Exp $: xzao816" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Ajout d'une zone d'influence dans la base
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAO816' and type = 'P')
	drop procedure XZAO816
go


create procedure XZAO816
   @va_Basedd_in        	char(3),
   @va_AutoroutePMV_in		char(5),
   @va_SensPMV_in		tinyint,
   @va_PrPMV_in			T_PR,
   @va_SitePMV_in		char(2),
   @va_AutorouteZone_in		char(5),
   @va_SensZone_in		tinyint,
   @va_PrDebutZone_in		T_PR,
   @va_PrFinZone_in		T_PR,
   @va_DirectionZone_in		char(15),
   @va_Inhibition_in		bit,
   @va_pr_1_pmv_amont	T_PR,
   @va_sens_1_pmv_amont	tinyint,
   @va_autoroute_1_pmv_amont char(5),
   @va_distance_1_pmv_amont	int,
   @va_pr_1_pmv_amont_bis	T_PR,
   @va_sens_1_pmv_amont_bis	tinyint,
   @va_autoroute_1_pmv_amont_bis char(5),
   @va_distance_1_pmv_amont_bis	int,
   @va_pr_1_pmv_amont_ter	T_PR,
   @va_sens_1_pmv_amont_ter	tinyint,
   @va_autoroute_1_pmv_amont_ter char(5),
   @va_distance_1_pmv_amont_ter	int,
   @va_Resultat_out		int output
   as 

/* 
*
* SP	XZAO403SP
*
* PARAMETRES EN ENTREE :
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf	   	va_Resultat_out	
*
* VALEUR RENDUE :
*
* Ajout d'une zone d'influence en base
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
* Insertion ou modification dans la table ZON_PMV 
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* Guilhou	22/12/97	: creation (dem/1534)
* C.T.		22/01/98	: correction erreur de compilation V1.2
* GGY		26/09/23	: Ajout district CA (DEM483)
* GGY		17/10/23	: Correction si CFG vide (DEM483)
------------------------------------------------------*/
declare
   
   @vl_Autoroute        tinyint,
   @vl_AutorouteZone    tinyint,
   @vl_Site             tinyint,
   @vl_SiteNom			char(10),
   @vl_Maitre           smallint,
   @vl_NumEqt           smallint,
   @vl_Numero           smallint,
   @vl_autoroute_1_pmv_amont tinyint,
   @vl_autoroute_1_pmv_amont_bis tinyint,
   @vl_autoroute_1_pmv_amont_ter tinyint

   select @va_Resultat_out = XDC_ECHEC
   
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_Basedd_in = null  
   or @va_AutoroutePMV_in = null 
   or @va_SensPMV_in = null
   or @va_PrPMV_in = null 
   or @va_SitePMV_in = null 
   or @va_AutorouteZone_in = null 
   or @va_SensZone_in = null 
   or @va_PrDebutZone_in = null
   or @va_PrFinZone_in = null
   or @va_DirectionZone_in = null
   or @va_Inhibition_in = null
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
            ** Verification de l'existence de l'autoroute et du PMV comme equipement
            */
         
	    select @vl_Autoroute = null
            select @vl_Autoroute = numero from CFG..RES_AUT
            where nom = @va_AutoroutePMV_in
                        
	    select @vl_AutorouteZone = null
	    select @vl_AutorouteZone = numero from CFG..RES_AUT
	    where nom = @va_AutorouteZone_in

	    select @vl_autoroute_1_pmv_amont = null
	    select @vl_autoroute_1_pmv_amont = numero from CFG..RES_AUT
	    where nom = @va_autoroute_1_pmv_amont

	    select @vl_autoroute_1_pmv_amont_bis = null
	    select @vl_autoroute_1_pmv_amont_bis = numero from CFG..RES_AUT
	    where nom = @va_autoroute_1_pmv_amont_bis

	    select @vl_autoroute_1_pmv_amont_ter = null
	    select @vl_autoroute_1_pmv_amont_ter = numero from CFG..RES_AUT
	    where nom = @va_autoroute_1_pmv_amont_ter

	    select @vl_Site = null
            select @vl_Site = numero from CFG..RES_DIS
            where code = @va_SitePMV_in
                        
	    select @vl_Numero = null
            select @vl_Numero = numero from CFG..EQT_GEN
            where autoroute = @vl_Autoroute
		and PR = @va_PrPMV_in
		and sens = @va_SensPMV_in
                and type = XDC_EQT_SAGA
		and numero in (select numero from CFG..EQT_SAGA
			    where type_saga = XDC_SAGA_PMV_1_18 or
            type_saga= XDC_SAGA_PMV_2_18 or
            type_saga = XDC_SAGA_PMV_2_15 or
            type_saga = XDC_SAGA_PMV_3_15 or
            type_saga = XDC_SAGA_PMV_2_18_P or
            type_saga = XDC_SAGA_PMV_3_15_P)
            
            /*A
            ** L'autoroute et le PMV et le site existent
            */
                     
            if @vl_Autoroute <> null and @vl_AutorouteZone <> null and @vl_Numero <> null and @vl_Site <> null
            begin
                
                select @vl_NumEqt = numero_zone from CFG..ZON_PMV_SAGA
                where numero_pmv =  @vl_Numero and 
			autoroute_pmv = @vl_Autoroute and
			sens_pmv = @va_SensPMV_in and
			PR_pmv = @va_PrPMV_in and 
			autoroute_zone = @vl_AutorouteZone and
			sens_zone = @va_SensZone_in and
			PR_debut_zone = @va_PrDebutZone_in and
			PR_fin_zone = @va_PrFinZone_in
               /*A
               ** La zone d'influence n'existe pas dans ZON_PMV_SAGA
               */
               
               if @vl_NumEqt = null
               begin
               
                  /*A
                  ** Insertion de la zone d'influence dans la table ZON_PMV
                  */
                  
                  select @vl_NumEqt = max(numero_zone) from CFG..ZON_PMV_SAGA

                  if @vl_NumEqt = null
                    select @vl_NumEqt = 0  
            
                  insert into CFG..ZON_PMV_SAGA(numero_zone,numero_pmv,autoroute_pmv,sens_pmv,PR_pmv,
			site_pmv,autoroute_zone,sens_zone,PR_debut_zone,PR_fin_zone,direction_zone,inhibition,
			PR_1_pmv_amont,sens_1_pmv_amont,autoroute_1_pmv_amont,distance_1_pmv_amont,
			PR_1_pmv_amont_bis,sens_1_pmv_amont_bis,autoroute_1_pmv_amont_bis,distance_1_pmv_amont_bis,
			PR_1_pmv_amont_ter,sens_1_pmv_amont_ter,autoroute_1_pmv_amont_ter,distance_1_pmv_amont_ter)
                  values ( @vl_NumEqt + 1, 
                 	   @vl_Numero,
			   @vl_Autoroute,
			   @va_SensPMV_in,
			   @va_PrPMV_in,
			   @vl_Site,
			   @vl_AutorouteZone,
			   @va_SensZone_in,
			   @va_PrDebutZone_in,
			   @va_PrFinZone_in,
			   @va_DirectionZone_in,
			   @va_Inhibition_in,
			   @va_pr_1_pmv_amont,
			   @va_sens_1_pmv_amont,
			   @vl_autoroute_1_pmv_amont,
			   @va_distance_1_pmv_amont,
			   @va_pr_1_pmv_amont_bis,
			   @va_sens_1_pmv_amont_bis,
			   @vl_autoroute_1_pmv_amont_bis,
			   @va_distance_1_pmv_amont_bis,
			   @va_pr_1_pmv_amont_ter,
			   @va_sens_1_pmv_amont_ter,
			   @vl_autoroute_1_pmv_amont_ter,
			   @va_distance_1_pmv_amont_ter)

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
		* la zone d'influence existe dans ZON_PMV	
               */
                     
               begin
               
                  /*A
                  ** Modification de la zone d'influence dans la table ZON_PMV
                  */
                  
                  update CFG..ZON_PMV_SAGA
                  set  site_pmv= @vl_Site,
		      PR_debut_zone	= @va_PrDebutZone_in, 
		      PR_fin_zone 	= @va_PrFinZone_in,
		      direction_zone 	= @va_DirectionZone_in,
                      inhibition	= @va_Inhibition_in,
		      PR_1_pmv_amont    = @va_pr_1_pmv_amont,
		      sens_1_pmv_amont  = @va_sens_1_pmv_amont,
		      autoroute_1_pmv_amont = @vl_autoroute_1_pmv_amont,
		      distance_1_pmv_amont = @va_distance_1_pmv_amont,
		      PR_1_pmv_amont_bis    = @va_pr_1_pmv_amont_bis,
		      sens_1_pmv_amont_bis  = @va_sens_1_pmv_amont_bis,
		      autoroute_1_pmv_amont_bis = @vl_autoroute_1_pmv_amont_bis,
		      distance_1_pmv_amont_bis = @va_distance_1_pmv_amont_bis,
		      PR_1_pmv_amont_ter    = @va_pr_1_pmv_amont_ter,
		      sens_1_pmv_amont_ter  = @va_sens_1_pmv_amont_ter,
		      autoroute_1_pmv_amont_ter = @vl_autoroute_1_pmv_amont_ter,
		      distance_1_pmv_amont_ter = @va_distance_1_pmv_amont_ter
                  where numero_zone = @vl_NumEqt
               
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
            ** Le serveur ou l'autoroute n'existe pas 
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
            ** Verification de l'existence de l'autoroute et du PMV comme equipement
            */
         
	    select @vl_Autoroute = null
            select @vl_Autoroute = numero from CFT..RES_AUT
              where nom = @va_AutoroutePMV_in
                        
	    select @vl_AutorouteZone = null
	    select @vl_AutorouteZone = numero from CFT..RES_AUT
	      where nom = @va_AutorouteZone_in

	    select @vl_autoroute_1_pmv_amont = null
	    select @vl_autoroute_1_pmv_amont = numero from CFG..RES_AUT
	    where nom = @va_autoroute_1_pmv_amont

	    select @vl_autoroute_1_pmv_amont_bis = null
	    select @vl_autoroute_1_pmv_amont_bis = numero from CFG..RES_AUT
	    where nom = @va_autoroute_1_pmv_amont_bis

	    select @vl_autoroute_1_pmv_amont_ter = null
	    select @vl_autoroute_1_pmv_amont_ter = numero from CFG..RES_AUT
	    where nom = @va_autoroute_1_pmv_amont_ter

	    select @vl_Site = null
            select @vl_Site = numero from CFT..RES_DIS
              where code = @va_SitePMV_in
                        
	    select @vl_Numero = null
            select @vl_Numero = numero from CFT..EQT_GEN
              where autoroute  = @vl_Autoroute
		and PR = @va_PrPMV_in
		and sens = @va_SensPMV_in
                and type = XDC_EQT_SAGA
		and numero in (select numero from CFT..EQT_SAGA
            where type_saga = XDC_SAGA_PMV_1_18 or
            type_saga= XDC_SAGA_PMV_2_18 or
            type_saga = XDC_SAGA_PMV_2_15 or
            type_saga = XDC_SAGA_PMV_3_15 or
            type_saga = XDC_SAGA_PMV_2_18_P or
            type_saga = XDC_SAGA_PMV_3_15_P)

            
            /*A
            ** L'autoroute et le PMV et le site existent
            */
                     
            if @vl_Autoroute <> null and @vl_AutorouteZone <> null and @vl_Numero <> null and @vl_Site <> null
            begin
                
                select @vl_NumEqt = numero_zone from CFT..ZON_PMV_SAGA
                  where numero_pmv =  @vl_Numero and 
			autoroute_pmv = @vl_Autoroute and
			sens_pmv = @va_SensPMV_in and
			PR_pmv = @va_PrPMV_in and 
			autoroute_zone = @vl_AutorouteZone and
			sens_zone = @va_SensZone_in  and
			PR_debut_zone = @va_PrDebutZone_in and
			PR_fin_zone = @va_PrFinZone_in
               /*A
               ** La zone d'influence n'existe pas dans ZON_PMV
               */
               
               if @vl_NumEqt = null
               begin
               
                  /*A
                  ** Insertion de la zone d'influence dans la table ZON_PMV
                  */
                  
                  select @vl_NumEqt = max(numero_zone) from CFT..ZON_PMV_SAGA

                  if @vl_NumEqt = null
                    select @vl_NumEqt = 0  
            
                  insert into CFT..ZON_PMV_SAGA(numero_zone,numero_pmv,autoroute_pmv,sens_pmv,PR_pmv,site_pmv,
			autoroute_zone,sens_zone,PR_debut_zone,PR_fin_zone,direction_zone,inhibition,
			PR_1_pmv_amont,sens_1_pmv_amont,autoroute_1_pmv_amont,distance_1_pmv_amont,
			PR_1_pmv_amont_bis,sens_1_pmv_amont_bis,autoroute_1_pmv_amont_bis,distance_1_pmv_amont_bis,
			PR_1_pmv_amont_ter,sens_1_pmv_amont_ter,autoroute_1_pmv_amont_ter,distance_1_pmv_amont_ter)
                  values ( @vl_NumEqt + 1, 
                 	   @vl_Numero,
			   @vl_Autoroute,
			   @va_SensPMV_in,
			   @va_PrPMV_in,
			   @vl_Site,
			   @vl_AutorouteZone,
			   @va_SensZone_in,
			   @va_PrDebutZone_in,
			   @va_PrFinZone_in,
			   @va_DirectionZone_in,
			   @va_Inhibition_in,
			   @va_pr_1_pmv_amont,
			   @va_sens_1_pmv_amont,
			   @vl_autoroute_1_pmv_amont,
			   @va_distance_1_pmv_amont,
			   @va_pr_1_pmv_amont_bis,
			   @va_sens_1_pmv_amont_bis,
			   @vl_autoroute_1_pmv_amont_bis,
			   @va_distance_1_pmv_amont_bis,
			   @va_pr_1_pmv_amont_ter,
			   @va_sens_1_pmv_amont_ter,
			   @vl_autoroute_1_pmv_amont_ter,
			   @va_distance_1_pmv_amont_ter)

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
		* la zone d'influence existe dans ZON_PMV	
               */
                     
               begin
               
                  /*A
                  ** Modification de la zone d'influence dans la table ZON_PMV
                  */
                  
                  update CFT..ZON_PMV_SAGA
                  set  site_pmv= @vl_Site,
		      PR_debut_zone	= @va_PrDebutZone_in, 
		      PR_fin_zone 	= @va_PrFinZone_in,
		      direction_zone 	= @va_DirectionZone_in,
                      inhibition	= @va_Inhibition_in,
		      PR_1_pmv_amont    = @va_pr_1_pmv_amont,
		      sens_1_pmv_amont  = @va_sens_1_pmv_amont,
		      autoroute_1_pmv_amont = @vl_autoroute_1_pmv_amont,
		      distance_1_pmv_amont = @va_distance_1_pmv_amont,
		      PR_1_pmv_amont_bis    = @va_pr_1_pmv_amont_bis,
		      sens_1_pmv_amont_bis  = @va_sens_1_pmv_amont_bis,
		      autoroute_1_pmv_amont_bis = @vl_autoroute_1_pmv_amont_bis,
		      distance_1_pmv_amont_bis = @va_distance_1_pmv_amont_bis,
		      PR_1_pmv_amont_ter    = @va_pr_1_pmv_amont_ter,
		      sens_1_pmv_amont_ter  = @va_sens_1_pmv_amont_ter,
		      autoroute_1_pmv_amont_ter = @vl_autoroute_1_pmv_amont_ter,
		      distance_1_pmv_amont_ter = @va_distance_1_pmv_amont_ter
                  where numero_zone = @vl_NumEqt
               
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
            ** Le serveur ou l'autoroute n'existe pas 
            ** Affectation du parametre de sortie Resultat a XDC_ECHEC
            */
            
               select @va_Resultat_out = XDC_ECHEC
               
         end

#endif
      
      end
   end         
     
   return XDC_OK
         
go  
