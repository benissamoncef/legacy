/*E*/
/* Fichier : $Id: xzao553sp.prc,v 1.9 2018/11/08 10:49:51 devgfi Exp $        $Revision: 1.9 $        $Date: 2018/11/08 10:49:51 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzao553sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	05/07/12 : Creation (DEM/1016)
* JMG	05/07/12 : correction gestion des PR + site gestion (DEM/1016) 1.2
* PNI	26/11/12 : correction pour accepter vitesse amont a 100 pour la zone  A8-28-18N v1.3
* JMG   20/03/17 : correction CFT/CFG
* LCL	05/07/17 : Modif PRA  1.5
* LCL   11/07/18 : correction syntaxe from
* LCL   11/07/18 : correction syntaxe @vl_site  1.7
* JPL	17/10/18 : Ajout du parametre 'supprime' (DEM 1305)  1.9
* GGY	26/09/23 : Ajout district CA (DEM483)
* GGY	17/10/23 : Correction si CFG vide (DEM483)
-----------------------------------------------------*/ 

/* fichiers inclus */

/* #include "xzao553sp.h" */

/* static char *version = "@(#)xzao553.c	1.1 23/18/94 : xzao553" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


use PRC
go

if exists (select 1 from sysobjects where name = 'XZAO553' and type = 'P')
	drop procedure XZAO553
go


	create procedure XZAO553
	   @va_Basedd_in         char(3),
	   @va_Numero_in		smallint,
	   @va_Nom_in			char(50),
	   @va_Autoroute_in		char(5),
	   @va_Sens_in			smallint,
	   @va_Debut_in			int,
	   @va_Fin_in			int,
	   @va_Vitesse_amont_in	 smallint,
	   @va_Inhibition_in	bit,
	   @va_ModeHoraire_in	bit,
	   @va_NomServeur_in	char(10),
	   @va_Supprime_in	bit,
	   @va_Resultat_out		int output
	   as 

/* 
* SERVICE RENDU : 
*
*
* SEQUENCE D'APPEL :
*
* SP    XZAO553SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd		va_Basedd_in
*  		va_Numero_in
*  		va_Nom_in
* XDY_NomAuto		va_Autoroute_in
* XDY_Sens		va_Sens_in
* XDY_PR 		va_Debut_in
* XDY_PR 		va_Fin_in
*  		va_Vitesse_amont_in
* XDY_Booleen		va_Inhibition_in
* XDY_Booleen		va_ModeHoraire_in
* XDY_NomEqt		va_NomServeur_in
* XDY_Booleen		va_Supprime_in
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf       va_Resultat_out    

* CODE RETOUR :
*
* 0    XDC_OK
* 1    XDC_NOK 
*
*
* FONCTION :
*
* Mise a jour des donnees dans la table ZON_REG
*
* MECANISMES :
*
------------------------------------------------------*/

declare @vl_autoroute	int,
		@vl_zone 		int,
  		@vl_Serveur		smallint,
  		@vl_pr_zone_deb int,
  		@vl_pr_zone_fin int,
  		@vl_pr_aut_deb 	int,
  		@vl_pr_aut_fin 	int,
  		@vl_PR_aut 		int,
		@vl_site 		tinyint,
   		@vl_SiteNom		char(10),
		@vl_numero 		int

    select @va_Resultat_out = XDC_LU
   
    /*A
    ** Test des parametres d'entree :
    **   Si un des parametres d'entree est egal a null alors XDC_NOK   
    */
    if @va_Basedd_in = null
    or @va_Numero_in = null
	or @va_Nom_in = null
	or @va_Autoroute_in = null
	or @va_Sens_in = null
	or @va_Debut_in = null
	or @va_Fin_in = null
	or @va_Vitesse_amont_in = null
	or @va_Inhibition_in = null
	or @va_Supprime_in = null
		return XDC_ARG_INV
    
		
	else 
       begin
    
        /*A
        ** Verification que le serveur est bien celui du CI
        */
     
	/*vl_pr_zone_deb contient forcement le PR min*/
	if @va_Debut_in <= @va_Fin_in
	begin
		select @vl_pr_zone_deb = @va_Debut_in
		select @vl_pr_zone_fin = @va_Fin_in
	end
	else
	begin
		select @vl_pr_zone_fin = @va_Debut_in
		select @vl_pr_zone_deb = @va_Fin_in
	end


	if @va_Sens_in = XDC_SENS_NORD
	begin
		/*le PR dans EQT_GEN doit etre le PR max*/
		select @vl_pr_aut_deb = @vl_pr_zone_deb
		select @vl_pr_zone_deb = @vl_pr_zone_fin
		select @vl_pr_zone_fin = @vl_pr_aut_deb
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
			    ** Verification de la valeur de la vitesse
			    */ 
		    	if @va_Vitesse_amont_in != 130 and
		    		@va_Vitesse_amont_in != 110 and
		    		@va_Vitesse_amont_in != 100 and
		    		@va_Vitesse_amont_in != 90 and
		    		@va_Vitesse_amont_in != 70
		    			return XDC_ARG_INV
		    
			
				/*A
			    ** Verification de la valeur du sens
			    */ 
		    	if @va_Sens_in != XDC_SENS_NORD and
		    		@va_Sens_in != XDC_SENS_SUD and
		    		@va_Sens_in != XDC_SENS_SORTIE_NORD and
		    		@va_Sens_in != XDC_SENS_SORTIE_SUD and
					@va_Sens_in != XDC_SENS_ENTREE_NORD and
					@va_Sens_in != XDC_SENS_ENTREE_SUD
		    			return XDC_ARG_INV
						
						
				/*A
				** recherche du numero d'autoroute
				*/

				select @vl_autoroute = numero
				from CFG..RES_AUT
				where nom = @va_Autoroute_in

				if @@rowcount <> 1
				begin
					/*A
					** Affectation du parametre de sortie Resultat a XDC_MODIF
					*/

					select @va_Resultat_out = XDC_MODIF

					return XDC_NOK
				end
				
				/*A
				** Verification incompatibilite PR zone / PR autoroute
				*/
				/*A
				** PR debut zone
				*/
				select  @vl_site = district, @vl_numero = numero from CFG..RES_POR
				where autoroute = @vl_autoroute and
				      @va_Debut_in between PR_debut and PR_fin
			 	
				if @@rowcount < 1
				begin
					/*A
					** Affectation du parametre de sortie Resultat a XDC_MODIF
					*/

					select @va_Resultat_out = XDC_MODIF

					return XDC_NOK
				end

				/*A
				** PR fin zone
				*/
				select  numero from CFG..RES_POR
				where autoroute = @vl_autoroute and
				      @va_Fin_in between PR_debut and PR_fin
			 	
				if @@rowcount < 1
				begin
					/*A
					** Affectation du parametre de sortie Resultat a XDC_MODIF
					*/

					select @va_Resultat_out = XDC_MODIF

					return XDC_NOK
				end

				select @vl_Serveur = numero from CFG..EQT_GEN
      			where nom  = @va_NomServeur_in
       			and type = XDC_EQT_SRV
        
				if @@rowcount <> 1
				begin
					/*A
					** Affectation du parametre de sortie Resultat a XDC_MODIF
					*/

					select @va_Resultat_out = XDC_MODIF

					return XDC_NOK
				end

                /*A
                ** Modification de l equipement dans la table ZON_REG
                */

                update CFG..ZON_REG
                set    nom = @va_Nom_in,
					   autoroute = @vl_autoroute,
					   sens = @va_Sens_in,
					   debut = @va_Debut_in,
					   fin = @va_Fin_in,
					   vitesse_amont = @va_Vitesse_amont_in,
					   inhibition = @va_Inhibition_in,
					   mode_horaire = @va_ModeHoraire_in
                where  numero = @va_Numero_in

		update CFG..EQT_GEN
                set    type = XDC_EQT_ZONE_REGUL,
					   numero = @va_Numero_in,
					   maitre = @vl_Serveur,
					   type_maitre = XDC_MAI_PRV,
					   autoroute = @vl_autoroute,
					   PR = @vl_pr_zone_deb,
					   sens = @va_Sens_in,
					   nom = @va_Nom_in,
					   site_gestion = @vl_site,
					   eqt_supprime = @va_Supprime_in
                where  numero = @va_Numero_in
                and type = XDC_EQT_ZONE_REGUL
              
                /*A
                ** Si pas d'enregistrement
                */

				if @@rowcount <> 1
				begin

					/*A
					** Insertion dans la table ZON_REG
					*/
	
					insert into CFG..ZON_REG ( numero,
							   nom,
							   autoroute,
							   sens,
							   debut,
							   fin,
							   vitesse_amont,
							   inhibition,
							   mode_horaire)
					  values ( @va_Numero_in,
							   @va_Nom_in,
							   @vl_autoroute,
							   @va_Sens_in,
							   @va_Debut_in,
							   @va_Fin_in,
							   @va_Vitesse_amont_in,
							   @va_Inhibition_in,
							   @va_ModeHoraire_in)	
					
					insert into CFG..EQT_GEN   (type,
							numero,
							maitre,
							type_maitre,
							autoroute,
							PR,
							sens,
							nom, site_gestion,
							eqt_supprime
					      )
					values( XDC_EQT_ZONE_REGUL,
							@va_Numero_in,
							@vl_Serveur,
							XDC_MAI_PRV,
							@vl_autoroute,
							@vl_pr_zone_deb,
							@va_Sens_in,
							@va_Nom_in,@vl_site,
							@va_Supprime_in
					      )
					   
					   			
						/*A
						** Si erreur d'insertion
						*/

						if @@rowcount <> 1
						begin

							/*A
							** Affectation du parametre de sortie Resultat a XDC_MODIF
							*/

							select @va_Resultat_out = XDC_MODIF

							return XDC_NOK
						end   
					end   
				end   
#ifdef CI      
            else

            /*A
            **  Travail dans la base CFT
            */   

				if @va_Basedd_in = XDC_BASE_CFT
            
					begin

					/*A
					    ** Verification de la valeur de la vitesse
					    */ 
					    	if @va_Vitesse_amont_in != 130 and
		    					@va_Vitesse_amont_in != 110 and
					    		@va_Vitesse_amont_in != 100 and
					    		@va_Vitesse_amont_in != 90 and
					    		@va_Vitesse_amont_in != 70
					    			return XDC_ARG_INV
						
						/*A
						** recherche du numero d'autoroute
						*/

						select @vl_autoroute = numero
						from CFT..RES_AUT
						where nom = @va_Autoroute_in
		                
						if @@rowcount <> 1
						begin
							/*A
							** Affectation du parametre de sortie Resultat a XDC_MODIF
							*/

							select @va_Resultat_out = XDC_MODIF

							return XDC_NOK
						end

						/*A
						** Verification incompatibilite PR zone / PR autoroute
						*/
						/*A
						** PR debut zone
						*/
						select  @vl_site = district, @vl_numero = numero from CFT..RES_POR
						where autoroute = @vl_autoroute and
						      @va_Debut_in between PR_debut and PR_fin
					 	
						if @@rowcount <> 1
						begin
							/*A
							** Affectation du parametre de sortie Resultat a XDC_MODIF
							*/

							select @va_Resultat_out = XDC_MODIF

							return XDC_NOK
						end

						/*A
						** PR fin zone
						*/
						select  numero from CFT..RES_POR
						where autoroute = @vl_autoroute and
						      @va_Fin_in between PR_debut and PR_fin
					 	
						if @@rowcount < 1
						begin
							/*A
							** Affectation du parametre de sortie Resultat a XDC_MODIF
							*/

							select @va_Resultat_out = XDC_MODIF

							return XDC_NOK
						end

						select @vl_Serveur = numero from CFT..EQT_GEN
              			where nom  = @va_NomServeur_in
               			and type = XDC_EQT_SRV
                
						if @@rowcount < 1
						begin
							/*A
							** Affectation du parametre de sortie Resultat a XDC_MODIF
							*/

							select @va_Resultat_out = XDC_MODIF

							return XDC_NOK
						end

                        /*A
                        ** Modification de l equipement dans la table ZON_REG
                        */

                        update CFT..ZON_REG
                        set    nom = @va_Nom_in,
							   autoroute = @vl_autoroute,
							   sens = @va_Sens_in,
							   debut = @va_Debut_in,
							   fin = @va_Fin_in,
							   vitesse_amont = @va_Vitesse_amont_in,
							   inhibition = @va_Inhibition_in,
							   mode_horaire = @va_ModeHoraire_in
                        where  numero = @va_Numero_in

						update CFT..EQT_GEN
                        set    type = XDC_EQT_ZONE_REGUL,
							   numero = @va_Numero_in,
							   maitre = @vl_Serveur,
							   type_maitre = XDC_MAI_PRV,
							   autoroute = @vl_autoroute,
							   PR = @vl_pr_zone_deb,
							   sens = @va_Sens_in,
							   nom = @va_Nom_in,
							   site_gestion = @vl_site,
							   eqt_supprime = @va_Supprime_in
                        where  numero = @va_Numero_in
                        and type = XDC_EQT_ZONE_REGUL
                        
                        /*A
                        ** Si pas d'enregistrement
                        */

						if @@rowcount <> 1
						begin

							/*A
							** Insertion dans la table ZON_REG
							*/

				
							insert into CFT..ZON_REG ( numero,
													   nom,
													   autoroute,
													   sens,
													   debut,
													   fin,
													   vitesse_amont,
													   inhibition,
													   mode_horaire)
											  values ( @va_Numero_in,
													   @va_Nom_in,
													   @vl_autoroute,
													   @va_Sens_in,
													   @va_Debut_in,
													   @va_Fin_in,
													   @va_Vitesse_amont_in,
													   @va_Inhibition_in,
													   @va_ModeHoraire_in)		
													   
								insert into CFT..EQT_GEN   (type,
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
												values( XDC_EQT_ZONE_REGUL,
														@va_Numero_in,
														@vl_Serveur,
														XDC_MAI_PRV,
														@vl_autoroute,
														@vl_pr_zone_deb,
														@va_Sens_in,
														@va_Nom_in,@vl_site,
														@va_Supprime_in
												      )
								/*A
								** Si erreur d'insertion
								*/

								if @@rowcount <> 1
								begin

									/*A
									** Affectation du parametre de sortie Resultat a XDC_MODIF
									*/

									select @va_Resultat_out = XDC_MODIF

									return XDC_NOK
								end   
						end   
					end   
#endif
      
	    end
    end         
     
    return XDC_OK
         
go  
