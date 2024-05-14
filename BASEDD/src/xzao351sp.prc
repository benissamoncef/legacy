/*E*/
/*Fichier : $Id: xzao351sp.prc,v 1.3 2018/07/11 11:23:44 pc2dpdy Exp $      Release : $Revision: 1.3 $        Date : $Date: 2018/07/11 11:23:44 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao351sp.prc
------------------------------------------------------
------------------------------------------------------
* HISTORIQUE :
* JMG	01/01/08	creation
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao351sp.h" */

/* static char *version = " $Id: xzao351sp.prc,v 1.3 2018/07/11 11:23:44 pc2dpdy Exp $: xzao351" ; */

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
drop procedure XZAO351
go


create procedure XZAO351
   @va_Basedd_in        	char(3),
   @va_AutorouteTUN_in		char(5),
   @va_SensTUN_in		tinyint,
   @va_PrTUN_in			T_PR,
   @va_AutorouteZone_in		char(5),
   @va_SensZone_in		tinyint,
   @va_PrDebutZone_in		T_PR,
   @va_PrFinZone_in		T_PR,
   @va_TypeZone_in		tinyint,
   @va_GroupeTUN_in		char(3),
   @va_PRPSTT_in		T_PR,
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
* Insertion ou modification dans la table ZON_TUN 
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* Guilhou	22/12/97	: creation (dem/1534)
* C.T.		22/01/98	: correction erreur de compilation V1.2
* LCL		11/07/18	: correction syntaxe from
* GGY		26/09/23	: Ajout district CA (DEM483)
* GGY		17/10/23	: Correction si CFG vide (DEM483)
------------------------------------------------------*/
declare
   
   @vl_Autoroute        tinyint,
   @vl_AutorouteZone    tinyint,
   @vl_Maitre           smallint,
   @vl_NumEqt           smallint,
   @vl_Numero           smallint,
   @vl_Site				char(10)

   select @va_Resultat_out = XDC_ECHEC
   
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_Basedd_in = null  
   or @va_AutorouteTUN_in = null 
   or @va_SensTUN_in = null
   or @va_PrTUN_in = null 
   or @va_AutorouteZone_in = null 
   or @va_SensZone_in = null 
   or @va_PrDebutZone_in = null
   or @va_PrFinZone_in = null
   or @va_PRPSTT_in = null
   or @va_Inhibition_in = null
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
            ** Verification de l'existence de l'autoroute et du TUN comme equipement
            */
         
	    select @vl_Autoroute = null
            select @vl_Autoroute = numero from CFG..RES_AUT
            where nom = @va_AutorouteTUN_in
                        
	    select @vl_AutorouteZone = null
	    select @vl_AutorouteZone = numero from CFG..RES_AUT
	    where nom = @va_AutorouteZone_in

	    select @vl_Numero = null
            select @vl_Numero = numero from CFG..EQT_GEN
            where autoroute = @vl_Autoroute
		and PR = @va_PrTUN_in
		and sens = @va_SensTUN_in
                and type = XDC_EQT_TUB
            
            /*A
            ** L'autoroute et le TUN et le site existent
            */
                     
            if @vl_Autoroute <> null and @vl_AutorouteZone <> null and @vl_Numero <> null 
            begin
                
                select @vl_NumEqt = numero_zone from CFG..ZON_TUN
                where numero_tube =  @vl_Numero and 
			autoroute= @vl_AutorouteZone and
			sens= @va_SensZone_in and
			PR_debut= @va_PrDebutZone_in and
			PR_fin= @va_PrFinZone_in
               /*A
               ** La zone d'influence n'existe pas dans ZON_TUN
               */
               
               if @vl_NumEqt = null
               begin
               
                  /*A
                  ** Insertion de la zone d'influence dans la table ZON_TUN
                  */
                  
                  select @vl_NumEqt = max(numero_zone) from CFG..ZON_TUN

                  if @vl_NumEqt = null
                    select @vl_NumEqt = 0  
            
                  insert into CFG..ZON_TUN(numero_zone,numero_tube,type_zone,groupe,
			autoroute,sens,PR_debut,PR_fin,inhibition, PR_PSTT)
                  values ( @vl_NumEqt + 1, 
                 	   @vl_Numero,
			   @va_TypeZone_in,
			   @va_GroupeTUN_in,
			   @vl_AutorouteZone,
			   @va_SensZone_in,
			   @va_PrDebutZone_in,
			   @va_PrFinZone_in,
			   @va_Inhibition_in,
			   @va_PRPSTT_in)

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
		* la zone d'influence existe dans ZON_TUN	
               */
                     
               begin
               
                  /*A
                  ** Modification de la zone d'influence dans la table ZON_TUN
                  */
                  
                  update CFG..ZON_TUN
                  set  PR_debut= @va_PrDebutZone_in, 
		      PR_fin= @va_PrFinZone_in,
                      inhibition	= @va_Inhibition_in,
		      type_zone = @va_TypeZone_in,
		      PR_PSTT = @va_PRPSTT_in,
		      groupe = @va_GroupeTUN_in
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
            ** Verification de l'existence de l'autoroute et du TUN comme equipement
            */
         
	    select @vl_Autoroute = null
            select @vl_Autoroute = numero from CFT..RES_AUT
              where nom = @va_AutorouteTUN_in
                        
	    select @vl_AutorouteZone = null
	    select @vl_AutorouteZone = numero from CFT..RES_AUT
	      where nom = @va_AutorouteZone_in

	    select @vl_Numero = null
            select @vl_Numero = numero from CFT..EQT_GEN
              where autoroute  = @vl_Autoroute
		and PR = @va_PrTUN_in
		and sens = @va_SensTUN_in
                and type = XDC_EQT_TUB
            
            /*A
            ** L'autoroute et le TUN et le site existent
            */
                     
            if @vl_Autoroute <> null and @vl_AutorouteZone <> null and @vl_Numero <> null 
            begin
                
                select @vl_NumEqt = numero_zone from CFT..ZON_TUN
                  where numero_tube =  @vl_Numero and 
			autoroute= @vl_AutorouteZone and
			sens= @va_SensZone_in  and
			PR_debut= @va_PrDebutZone_in and
			PR_fin= @va_PrFinZone_in
               /*A
               ** La zone d'influence n'existe pas dans ZON_TUN
               */
               
               if @vl_NumEqt = null
               begin
               
                  /*A
                  ** Insertion de la zone d'influence dans la table ZON_TUN
                  */
                  
                  select @vl_NumEqt = max(numero_zone) from CFT..ZON_TUN

                  if @vl_NumEqt = null
                    select @vl_NumEqt = 0  
            
                  insert into CFT..ZON_TUN(numero_zone,numero_tube,type_zone,groupe,
			autoroute,sens,PR_debut,PR_fin,inhibition,PR_PSTT)
                  values ( @vl_NumEqt + 1, 
                 	   @vl_Numero,
			   @va_TypeZone_in,
			   @va_GroupeTUN_in,
			   @vl_AutorouteZone,
			   @va_SensZone_in,
			   @va_PrDebutZone_in,
			   @va_PrFinZone_in,
			   @va_Inhibition_in,
			   @va_PRPSTT_in)

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
		* la zone d'influence existe dans ZON_TUN	
               */
                     
               begin
               
                  /*A
                  ** Modification de la zone d'influence dans la table ZON_TUN
                  */
                  
                  update CFT..ZON_TUN
                  set  PR_debut	= @va_PrDebutZone_in, 
		      PR_fin 	= @va_PrFinZone_in,
                      inhibition	= @va_Inhibition_in,
		      type_zone = @va_TypeZone_in,
		      PR_PSTT = @va_PRPSTT_in,
		      groupe = @va_GroupeTUN_in
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
