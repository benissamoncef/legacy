/*E*/
/* Fichier : %W%        Release : %I%        Date : %G%
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao730sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
* JMG   20/03/17 : creation DEM1191
* LCL	05/07/17 : Modif PRA
* JPL	15/10/18 : Correction syntaxe  1.4
* JPL	17/10/18 : Nommage des colonnes sur 'insert'  1.5
* JPL	06/02/19 : Appel de XZAO67 degroupee  1.6
* GGY	26/09/23 : Ajout district CA (DEM483)
* GGY	17/10/23 : Correction si CFG vide (DEM483)
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao730sp.h" */

/* static char *version = "$Id: xzao730sp.prc,v 1.6 2019/02/06 14:15:39 devgfi Exp $ $Revision: 1.6 $ $Date: 2019/02/06 14:15:39 $" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Ajout d'un eqt SAGA dans la base
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

if exists (select 1 from sysobjects where name = 'XZAO730' and type = 'P')
	drop procedure XZAO730
go

create procedure XZAO730
   @va_Basedd_in        	char(3),
   @va_NomEqtSAGA_in		char(10),
   @va_NomAutoroute_in		char(4),
   @va_PR_in			int,
   @va_Sens_in			tinyint,
   @va_TypeSAGA_in		tinyint,
   @va_Instance_in		char(30),
   @va_FelsCode_in		char(3),
   @va_EqtCode_in		char(3),
   @va_CodeCmdMessage_in	char(3),
   @va_CodeCmdLuminosite_in	char(3),
   @va_CodeCmdPicto_in		char(3),
   @va_CodeCmdHeure_in		char(3),
   @va_NomSite_in               char(2),
   @va_Alarme_in               char(50),
   @va_Resultat_out		int output
   as 

/* 
*
* SP	XZAO730SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd	va_Basedd_in        		
* XDY_NomEqt    va_NomEqtSAGA_in
* XDY_Nom       va_NomAutoroute_in
* XDY_PR        va_PR_in
* XDY_Sens      va_Sens_in
* XDY_Octet     va_TypeSAGA_in
*  ...
* XDY_NomSite   va_NomSite_in
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf	   va_Resultat_out	
*
* VALEUR RENDUE :
*
* Ajoute un equipement SAGA en base
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
* Insertion ou modification dans la table EQT_SAGA
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	09/06/05: Creation
------------------------------------------------------*/
declare
   
   /*A
   ** Definition des variables globales
   */

   @vl_Autoroute	tinyint,
   @vl_Machine		smallint,
   @vl_NumEqtSAGA	smallint,
   @vl_Numero		smallint,
   @vl_Site			tinyint,
   @vl_SiteNom		char(10),
   @vl_Serveur		smallint,
   @vl_cr 			int,
   @vl_flag_radt	tinyint
   
   select @va_Resultat_out = XDC_LU
   
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_Basedd_in = null
   or @va_NomEqtSAGA_in = null 
   or @va_NomAutoroute_in = null 
   or @va_PR_in = null 
   or @va_Sens_in = null 
   or @va_TypeSAGA_in = null 
   or @va_Instance_in = null 
   or @va_FelsCode_in = null 
   or @va_EqtCode_in = null 
   or @va_CodeCmdMessage_in = null 
   or @va_CodeCmdLuminosite_in = null 
   or @va_CodeCmdPicto_in = null 
   or @va_CodeCmdHeure_in = null 
   or @va_NomSite_in = null
      return XDC_ARG_INV
   else   
 
 begin
      
    select @vl_flag_radt=0
    /* si eqt saga RADT*/
    if @va_TypeSAGA_in = XDC_SAGA_RADT
    begin
	select @vl_flag_radt=1
	/*on cree en realite un eqt de type RADT dans le SAE*/
	exec @vl_cr = PRC..XZAO67 @va_Basedd_in, @va_NomEqtSAGA_in,
			@va_NomAutoroute_in, @va_PR_in, @va_Sens_in, 
			"STDP",0,"xxx",  " "," ",0, @va_NomSite_in, 
			@va_Resultat_out output
	if @vl_cr <> XDC_OK
		return @vl_cr
    end

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
         ** Mise a jour de la table EQT_GEN 
         */
         
         /*B
         ** Verification de l'existence et recherche du numero de l'autoroute ainsi
         ** que du serveur associe au eqt SAGA
         */
         
         select @vl_Autoroute = numero from CFG..RES_AUT
           where nom = @va_NomAutoroute_in
           
       select @vl_Site = numero from CFG..RES_DIS
       where code = @va_NomSite_in

	   if @vl_Site = NULL
	   return XDC_ARG_INV

	 select @vl_Serveur = numero from CFG..EQT_GEN where nom = "SPDP"
         /*A
         ** l'autoroute et le serveur associe au eqt SAGA existent
         */
         
         if @vl_Autoroute <> null and @vl_Serveur <> null
         begin
         
            /*A 
            ** Insertion ou Modification du eqt SAGA
            */
                          
                  /*B
                  ** Verification de l'existence et Recherche de l'equipement eqt SAGA
                  */
         
		  /*si on traite un eqt RADT */
		  if @vl_flag_radt = 1
		  begin
			/*on recupere son numero deja traite plus haut via XZAO67*/
			select @vl_NumEqtSAGA = numero from CFG..EQT_GEN
			where type  =XDC_EQT_RAD
			and nom   = @va_NomEqtSAGA_in

		  	select @vl_Numero = numero
		  	from CFG..EQT_SAGA
		  	where numero = @vl_NumEqtSAGA and type_saga = XDC_SAGA_RADT

			if @@rowcount <>1
				select @vl_Numero = 0
	
		  	if @vl_Numero = 0
		  	begin
				/*insertion dans EQT_SAGA*/
                     		insert into CFG..EQT_SAGA (numero, type_saga, instance, fels_code, eqt_code, 
					code_cmd_message, code_cmd_luminosite, code_cmd_picto, code_cmd_heure, alarme)
                     		values ( @vl_NumEqtSAGA, @va_TypeSAGA_in, @va_Instance_in, @va_FelsCode_in, @va_EqtCode_in, 
					@va_CodeCmdMessage_in, @va_CodeCmdLuminosite_in, 
						@va_CodeCmdPicto_in, @va_CodeCmdHeure_in, @va_Alarme_in)
		  	end
			else
			begin
                    		update CFG..EQT_SAGA
    	            		set type_saga     	= @va_TypeSAGA_in,
    	                		instance 	= @va_Instance_in,
					fels_code	= @va_FelsCode_in,
					eqt_code 	= @va_EqtCode_in,
					code_cmd_message = @va_CodeCmdMessage_in,
					code_cmd_luminosite  = @va_CodeCmdLuminosite_in,
					code_cmd_picto = @va_CodeCmdPicto_in,
					code_cmd_heure = @va_CodeCmdHeure_in,
					alarme = @va_Alarme_in
                    		where numero = @vl_NumEqtSAGA and type_saga= @va_TypeSAGA_in
			end
                     	if @@rowcount <> 1
                     	begin
                        	select @va_Resultat_out = XDC_AJOUT
                        	return XDC_NOK
                     	end

			return XDC_OK
		  end

		/* traitement d un eqt hors RADT SAGA*/
                  select @vl_NumEqtSAGA = numero from CFG..EQT_GEN
                    where type	= XDC_EQT_SAGA
                      and nom	= @va_NomEqtSAGA_in

              
                  /*A
                  ** Le eqt SAGA n'existe pas
                  */
         
                  if @vl_NumEqtSAGA = null
                  begin
            
                     /*A
                     ** Insertion dans la table equipement du nouveau eqt SAGA
                     */
            
                     select @vl_Numero = max(numero) from CFG..EQT_GEN
                       where type = XDC_EQT_SAGA
              
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
                     values ( XDC_EQT_SAGA,
                              @vl_Numero + 1,
                              @vl_Serveur,
                              XDC_EQT_MAC,
                              @vl_Autoroute,
                              @va_PR_in,
                              @va_Sens_in,
                              @va_NomEqtSAGA_in,
                              @vl_Site
                            )
                     /*A
                     ** Erreur d'insertion
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
                     ** Insertion dans la table eqt SAGA du nouveau eqt SAGA
                     */
            
                     insert into CFG..EQT_SAGA (numero, type_saga, instance, fels_code, eqt_code, code_cmd_message, code_cmd_luminosite, code_cmd_picto, code_cmd_heure, alarme)
                     values ( @vl_Numero + 1, @va_TypeSAGA_in, @va_Instance_in, @va_FelsCode_in, @va_EqtCode_in, @va_CodeCmdMessage_in, @va_CodeCmdLuminosite_in, @va_CodeCmdPicto_in, @va_CodeCmdHeure_in, @va_Alarme_in)
                     /*A
                     ** Erreur d'insertion
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
                  end /* Fin Le PMVA n'existe pas */
                  
                  else
         
                  /*A
                  ** Le PMVA existe deja
                  */ 
              
                  begin
           
                    /*A
                    ** Modification dans la table du PMVA
                    */
            
                    update CFG..EQT_GEN
    	            set maitre      = @vl_Serveur,
    	                type_maitre = XDC_EQT_MAC,
                        autoroute   = @vl_Autoroute,
                        PR          = @va_PR_in,
                        sens        = @va_Sens_in,
                        nom         = @va_NomEqtSAGA_in,
			site_gestion = @vl_Site
                    where numero = @vl_NumEqtSAGA
                      and type   = XDC_EQT_SAGA 
    
                    /*A
                    ** Erreur de modificaion
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
                    ** Modification dans la table du PMVA
                    */
            
                    update CFG..EQT_SAGA
    	            set type_saga     	= @va_TypeSAGA_in,
    	                instance 	= @va_Instance_in,
			fels_code	= @va_FelsCode_in,
			eqt_code 	= @va_EqtCode_in,
			code_cmd_message = @va_CodeCmdMessage_in,
			code_cmd_luminosite  = @va_CodeCmdLuminosite_in,
			code_cmd_picto = @va_CodeCmdPicto_in,
			code_cmd_heure = @va_CodeCmdHeure_in,
			alarme = @va_Alarme_in
                    where numero = @vl_NumEqtSAGA and type_saga= @va_TypeSAGA_in
    
                    /*A
                    ** Erreur de modificaion
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
               
                 end /* Fin Le PMVA existe deja */
         

         
 
         
         end /* Fin test sur autoroute et serveur */
         else   
         /*A
         ** L'autoroute ou le serveur n'existe pas
         ** Affectation du parametre de sortie Resultat a XDC_ECHEC
         */
         begin
         
           select @va_Resultat_out = XDC_ECHEC
	   return XDC_OK
	 end
      
      end /* Fin test base CFG */

#ifdef CI
      
       else   
      /*A
      **  Travail dans la base CFT
      */   
      
      begin
         
         /*A
         ** Mise a jour de la table EQT_GEN 
         */
         
         /*B
         ** Verification de l'existence et recherche du numero de l'autoroute ainsi
         ** que du serveur associe au PMVA
         */
         
         select @vl_Autoroute = numero from CFT..RES_AUT
           where nom = @va_NomAutoroute_in
           
	 select @vl_Serveur = numero from CFT..EQT_GEN where nom = "SPDP"
          
	     select @vl_Site = numero from CFT..RES_DIS
	     where code = @va_NomSite_in

	 if @vl_Site = NULL
	 return XDC_ARG_INV

         /*A
         ** l'autoroute et le serveur associe au PMVA existent
         */
         
         if @vl_Autoroute <> null and @vl_Serveur <> null
         begin
         
            /*A 
            ** Insertion ou Modification du PMVA
            */
            
            /*B
            ** Verification de l'existence et recherche du type PMVA
            */

		  /*si on traite un eqt RADT */
		  if @vl_flag_radt = 1
		  begin
			/*on recupere son numero deja traite plus haut via XZAO67*/
			select @vl_NumEqtSAGA = numero from CFT..EQT_GEN
			where type  =XDC_EQT_RAD
			and nom   = @va_NomEqtSAGA_in

			/*select @vl_NumEqtSAGA*/
		  	select @vl_Numero = numero
		  	from CFT..EQT_SAGA
		  	where numero = @vl_NumEqtSAGA and type_saga=XDC_SAGA_RADT

			if @@rowcount <>1
				select @vl_Numero = 0

			/* select @vl_Numero*/

		  	if @vl_Numero = 0
		  	begin
				select "insert"
				/*insertion dans EQT_SAGA*/
                     		insert into CFT..EQT_SAGA (numero, type_saga, instance, fels_code, eqt_code, 
					code_cmd_message, code_cmd_luminosite, code_cmd_picto, code_cmd_heure, alarme)
                     		values ( @vl_NumEqtSAGA, @va_TypeSAGA_in, @va_Instance_in, @va_FelsCode_in, @va_EqtCode_in, 
					@va_CodeCmdMessage_in, @va_CodeCmdLuminosite_in, 
						@va_CodeCmdPicto_in, @va_CodeCmdHeure_in, @va_Alarme_in)
		  	end
			else
			begin
                    		update CFT..EQT_SAGA
    	            		set type_saga     	= @va_TypeSAGA_in,
    	                		instance 	= @va_Instance_in,
					fels_code	= @va_FelsCode_in,
					eqt_code 	= @va_EqtCode_in,
					code_cmd_message = @va_CodeCmdMessage_in,
					code_cmd_luminosite  = @va_CodeCmdLuminosite_in,
					code_cmd_picto = @va_CodeCmdPicto_in,
					code_cmd_heure = @va_CodeCmdHeure_in,
					alarme = @va_Alarme_in
                    		where numero = @vl_NumEqtSAGA
				and type_saga = @va_TypeSAGA_in
			end
                     	if @@rowcount <> 1
                     	begin
                        	select @va_Resultat_out = XDC_AJOUT
                        	return XDC_NOK
                     	end

			select @va_Resultat_out = XDC_AJOUT
			return XDC_OK
		  end

              
                  /*B
                  ** Verification de l'existence et Recherche de l'equipement PMVA
                  */
         
                  select @vl_NumEqtSAGA = numero from CFT..EQT_GEN
                    where type	= XDC_EQT_SAGA
                      and nom	= @va_NomEqtSAGA_in
         
                  /*A
                  ** Le PMVA n'existe pas
                  */
         
                  if @vl_NumEqtSAGA  is null
                  begin
            
                     /*A
                     ** Insertion dans la table equipement du nouveau PMVA
                     */
            
                     select @vl_Numero = max(numero) from CFT..EQT_GEN
                       where type = XDC_EQT_SAGA
              
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
                     values ( XDC_EQT_SAGA,
                              @vl_Numero + 1,
                              @vl_Serveur,
                              XDC_EQT_MAC,
                              @vl_Autoroute,
                              @va_PR_in,
                              @va_Sens_in,
                              @va_NomEqtSAGA_in,
                              @vl_Site
                            )
            
                     /*A
                     ** Erreur d'insertion
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
                     ** Insertion dans la table PMVA du nouveau PMVA
                     */
            
                     insert into CFT..EQT_SAGA (numero, type_saga, instance, fels_code, eqt_code, code_cmd_message, code_cmd_luminosite, code_cmd_picto, code_cmd_heure, alarme)
                     values ( @vl_Numero + 1, @va_TypeSAGA_in, @va_Instance_in, @va_FelsCode_in, @va_EqtCode_in, @va_CodeCmdMessage_in, @va_CodeCmdLuminosite_in, @va_CodeCmdPicto_in, @va_CodeCmdHeure_in, @va_Alarme_in)
            
                     /*A
                     ** Erreur d'insertion
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
                  end /* Fin Le PMVA n'existe pas */
                  
                  else
         
                  /*A
                  ** Le PMVA existe deja
                  */ 
              
                  begin
           
                    /*A
                    ** Modification dans la table du PMVA
                    */
            
                    update CFT..EQT_GEN
    	            set maitre      = @vl_Serveur,
    	                type_maitre = XDC_EQT_MAC,
                        autoroute   = @vl_Autoroute,
                        PR          = @va_PR_in,
                        sens        = @va_Sens_in,
                        nom         = @va_NomEqtSAGA_in,
			site_gestion = @vl_Site
                    where numero = @vl_NumEqtSAGA
                      and type   = XDC_EQT_SAGA 
    
                    /*A
                    ** Erreur de modificaion
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
                    ** Modification dans la table du PMVA
                    */
            
                    update CFT..EQT_SAGA
    	            set type_saga     	= @va_TypeSAGA_in,
    	                instance 	= @va_Instance_in,
			fels_code	= @va_FelsCode_in,
			eqt_code 	= @va_EqtCode_in,
			code_cmd_message = @va_CodeCmdMessage_in,
			code_cmd_luminosite  = @va_CodeCmdLuminosite_in,
			code_cmd_picto = @va_CodeCmdPicto_in,
			code_cmd_heure = @va_CodeCmdHeure_in,
			alarme = @va_Alarme_in
                    where numero = @vl_NumEqtSAGA and type_saga = @va_TypeSAGA_in
    
                    /*A
                    ** Erreur de modificaion
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
               
                 end /* Fin Le PMVA existe deja */
         
         

         
         end /* Fin test sur autoroute et serveur */
         else   
         /*A
         ** L'autoroute ou le serveur n'existe pas
         ** Affectation du parametre de sortie Resultat a XDC_ECHEC
         */
         begin
           select @va_Resultat_out = XDC_ECHEC
	   return XDC_OK
	 end
         
      end /* fin test base CFT */
#endif
       
    end /* fin verification base CI */
  end  /* fin arguments valides */ 
  return XDC_OK
         
go  
