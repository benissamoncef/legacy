/*E*/
/*Fichier : $Id: xzao456sp.prc,v 1.5 2018/07/11 11:30:52 pc2dpdy Exp $     Release : $Revision: 1.5 $        Date : $Date: 2018/07/11 11:30:52 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao456sp.prc
------------------------------------------------------
* HISTORIQUE :
* PNI	29/01/2008 : Suppression du crit�re de site pour l'ajout/modif d'une zone v1.3 DEM743
* LCL	05/07/2017 : Modif PRA
* LCL           11/07/18        : correction syntaxe from
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao456sp.h" */

/* static char *version = " $Id: xzao456sp.prc,v 1.5 2018/07/11 11:30:52 pc2dpdy Exp $ $Revision: 1.5 $ $Date: 2018/07/11 11:30:52 $: xzao456" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Ajout d'une zone d'influence BAD dans la base
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go
drop procedure XZAO456
go

create procedure XZAO456
   @va_Basedd_in        	char(3),
   @va_AutorouteBAD_in		char(5),
   @va_SensBAD_in		tinyint,
   @va_PrBAD_in			T_PR,
   @va_SiteBAD_in		char(2),
   @va_AutorouteZone_in		char(5),
   @va_SensZone_in		tinyint,
   @va_PrDebutZone_in		T_PR,
   @va_PrFinZone_in		T_PR,
   @va_Inhibition_in		bit,
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
* Insertion ou modification dans la table ZON_BAD 
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* Guilhou	16/06/05 : creation 
* GGY		26/09/23 : Ajout district CA (DEM483)
* GGY		17/10/23 : Correction si CFG vide (DEM483)
------------------------------------------------------*/
declare
   
   @vl_Autoroute        tinyint,
   @vl_AutorouteZone    tinyint,
   @vl_Site             tinyint,
   @vl_SiteNom			char(10),
   @vl_Maitre           smallint,
   @vl_NumEqt           smallint,
   @vl_Numero           smallint

   select @va_Resultat_out = XDC_ECHEC
   
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_Basedd_in = null  
   or @va_AutorouteBAD_in = null 
   or @va_SensBAD_in = null
   or @va_PrBAD_in = null 
   or @va_SiteBAD_in = null 
   or @va_AutorouteZone_in = null 
   or @va_SensZone_in = null 
   or @va_PrDebutZone_in = null
   or @va_PrFinZone_in = null
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
            ** Verification de l'existence de l'autoroute et du BAD comme equipement
            */
         
	    select @vl_Autoroute = null
            select @vl_Autoroute = numero from CFG..RES_AUT
            where nom = @va_AutorouteBAD_in
                        
	    select @vl_AutorouteZone = null
	    select @vl_AutorouteZone = numero from CFG..RES_AUT
	    where nom = @va_AutorouteZone_in

	    select @vl_Site = null
            select @vl_Site = numero from CFG..RES_DIS
            where code = @va_SiteBAD_in
                        
	    select @vl_Numero = null
            select @vl_Numero = numero from CFG..EQT_GEN
            where autoroute = @vl_Autoroute
		and PR = @va_PrBAD_in
		and sens = @va_SensBAD_in
                and type = XDC_EQT_BAD
            
            /*A
            ** L'autoroute et le BAD et le site existent
            */
                     
            if @vl_Autoroute <> null and @vl_AutorouteZone <> null and @vl_Numero <> null and @vl_Site <> null
            begin
                
                select @vl_NumEqt = numero_zone from CFG..ZON_BAD
                where numero_bad =  @vl_Numero and 
			autoroute_bad = @vl_Autoroute and
			sens_bad = @va_SensBAD_in and
			PR_bad = @va_PrBAD_in and 
			autoroute_zone = @vl_AutorouteZone and
			sens_zone = @va_SensZone_in and
			PR_debut_zone = @va_PrDebutZone_in and
			PR_fin_zone = @va_PrFinZone_in
			
               /*A
               ** La zone d'influence n'existe pas dans ZON_BAD
               */
               
               if @vl_NumEqt = null
               begin
               
                  /*A
                  ** Insertion de la zone d'influence dans la table ZON_BAD
                  */
                  
                  select @vl_NumEqt = max(numero_zone) from CFG..ZON_BAD

                  if @vl_NumEqt = null
                    select @vl_NumEqt = 0  
            
                  insert into CFG..ZON_BAD(numero_zone,numero_bad,autoroute_bad,sens_bad,PR_bad,
			site_bad,autoroute_zone,sens_zone,PR_debut_zone,PR_fin_zone,inhibition)
                  values ( @vl_NumEqt + 1, 
                 	   @vl_Numero,
			   @vl_Autoroute,
			   @va_SensBAD_in,
			   @va_PrBAD_in,
			   @vl_Site,
			   @vl_AutorouteZone,
			   @va_SensZone_in,
			   @va_PrDebutZone_in,
			   @va_PrFinZone_in,
			   @va_Inhibition_in)

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
		* la zone d'influence existe dans ZON_BAD	
               */
                     
               begin
               
                  /*A
                  ** Modification de la zone d'influence dans la table ZON_BAD
                  */
                  
                  update CFG..ZON_BAD
                  set  site_bad= @vl_Site,
		      PR_debut_zone	= @va_PrDebutZone_in, 
		      PR_fin_zone 	= @va_PrFinZone_in,
                      inhibition	= @va_Inhibition_in
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
            ** Verification de l'existence de l'autoroute et du BAD comme equipement
            */
         
	    select @vl_Autoroute = null
            select @vl_Autoroute = numero from CFT..RES_AUT
              where nom = @va_AutorouteBAD_in
                        
	    select @vl_AutorouteZone = null
	    select @vl_AutorouteZone = numero from CFT..RES_AUT
	      where nom = @va_AutorouteZone_in

	    select @vl_Site = null
            select @vl_Site = numero from CFT..RES_DIS
              where code = @va_SiteBAD_in
                        
	    select @vl_Numero = null
            select @vl_Numero = numero from CFT..EQT_GEN
              where autoroute  = @vl_Autoroute
		and PR = @va_PrBAD_in
		and sens = @va_SensBAD_in
                and type = XDC_EQT_BAD
            
            /*A
            ** L'autoroute et le BAD et le site existent
            */
                     
            if @vl_Autoroute <> null and @vl_AutorouteZone <> null and @vl_Numero <> null and @vl_Site <> null
            begin
                
                select @vl_NumEqt = numero_zone from CFT..ZON_BAD
                  where numero_bad =  @vl_Numero and 
			autoroute_bad = @vl_Autoroute and
			sens_bad = @va_SensBAD_in and
			PR_bad = @va_PrBAD_in and 
			autoroute_zone = @vl_AutorouteZone and
			sens_zone = @va_SensZone_in  and
			PR_debut_zone = @va_PrDebutZone_in and
			PR_fin_zone = @va_PrFinZone_in
			
               /*A
               ** La zone d'influence n'existe pas dans ZON_BAD
               */
               
               if @vl_NumEqt = null
               begin
               
                  /*A
                  ** Insertion de la zone d'influence dans la table ZON_BAD
                  */
                  
                  select @vl_NumEqt = max(numero_zone) from CFT..ZON_BAD

                  if @vl_NumEqt = null
                    select @vl_NumEqt = 0  
            
                  insert into CFT..ZON_BAD(numero_zone,numero_bad,autoroute_bad,sens_bad,PR_bad,site_bad,
			autoroute_zone,sens_zone,PR_debut_zone,PR_fin_zone,inhibition)
                  values ( @vl_NumEqt + 1, 
                 	   @vl_Numero,
			   @vl_Autoroute,
			   @va_SensBAD_in,
			   @va_PrBAD_in,
			   @vl_Site,
			   @vl_AutorouteZone,
			   @va_SensZone_in,
			   @va_PrDebutZone_in,
			   @va_PrFinZone_in,
			   @va_Inhibition_in)

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
		* la zone d'influence existe dans ZON_BAD	
               */
                     
               begin
               
                  /*A
                  ** Modification de la zone d'influence dans la table ZON_BAD
                  */
                  
                  update CFT..ZON_BAD
                  set  site_bad= @vl_Site,
		      PR_debut_zone	= @va_PrDebutZone_in, 
		      PR_fin_zone 	= @va_PrFinZone_in,
                      inhibition	= @va_Inhibition_in
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
