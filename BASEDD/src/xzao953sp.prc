/*E*/
/* Fichier : $Id: xzao953sp.prc,v 1.2 2017/10/02 15:19:38 pc2dpdy Exp $        Release : $Revision: 1.2 $        Date : $Date: 2017/10/02 15:19:38 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzao953sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	26/06/17 convergence 1.1
* LCL	05/07/17	: Modif PRA
-----------------------------------------------------*/ 

/* fichiers inclus */

/* #include "xzao953sp.h" */

/* static char *version = "@(#)xzao953.c	1.1 23/18/94 : xzao953" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/


use PRC
go


if exists (select * from sysobjects where name = 'XZAO953' and type = 'P')
	drop procedure XZAO953
go

	create procedure XZAO953
	   @va_Basedd_in         char(3),
	   @va_Numero_in		smallint,
	   @va_Autoroute_in		char(5),
	   @va_Sens_in			smallint,
	   @va_Debut_in			int,
	   @va_Fin_in			int,
	   @va_Resultat_out		int output
	   as 

/* 
*
* SP    XZAO953SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd        va_Basedd_in
*  					va_Numero_in
*  					va_Nom_in
*  					va_Autoroute_in
* 					va_Sens_in
*  					va_Debut_in
*  					va_Fin_in
*  					va_Vitesse_amont_in
*  					va_Inhibition_in
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
* Mise a jour des donnees dans la table ZON_URB
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* VR    12 Jan 2012	: Creation (DEM/1016)
* GGY	26/09/23	: Ajout district CA (DEM483)
* GGY	17/10/23	: Correction si CFG vide (DEM483)
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
	or @va_Autoroute_in = null
	or @va_Sens_in = null
	or @va_Debut_in = null
	or @va_Fin_in = null
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

                /*A
                ** Modification de l equipement dans la table ZON_URB
                */

                update CFG..ZON_URB
                set   autoroute = @vl_autoroute,
					   sens = @va_Sens_in,
					   debut = @va_Debut_in,
					   fin = @va_Fin_in
                where  numero = @va_Numero_in

                /*A
                ** Si pas d'enregistrement
                */

				if @@rowcount <> 1
				begin

					/*A
					** Insertion dans la table ZON_URB
					*/
	
					insert into CFG..ZON_URB ( numero,
							   autoroute,
							   sens,
							   debut,
							   fin)
					  values ( @va_Numero_in,
							   @vl_autoroute,
							   @va_Sens_in,
							   @va_Debut_in,
							   @va_Fin_in)
					
					   			
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

                        /*A
                        ** Modification de l equipement dans la table ZON_URB
                        */

                        update CFT..ZON_URB
                        set    autoroute = @vl_autoroute,
							   sens = @va_Sens_in,
							   debut = @va_Debut_in,
							   fin = @va_Fin_in
                        where  numero = @va_Numero_in

                        /*A
                        ** Si pas d'enregistrement
                        */

						if @@rowcount <> 1
						begin

							/*A
							** Insertion dans la table ZON_URB
							*/

				
			insert into CFT..ZON_URB ( numero,
			   autoroute,
			   sens,
			   debut,
			   fin)
				  values ( @va_Numero_in,
				   @vl_autoroute,
				   @va_Sens_in,
				   @va_Debut_in,
				   @va_Fin_in)
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
