/*E*/
/* Fichier : $Id: xzao2571sp.prc,v 1.6 2023/08/21 18:39:51 devgfi Exp $        $Revision: 1.6 $        $Date: 2023/08/21 18:39:51 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao2571sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* ABK  21/08/23   : Creation (DEM/483)
------------------------------------------------------ */

/* fichiers inclus */

/* #include "xzao2571sp.h" */

/* static char *version = "@(#)xzao2571sp.prc	1.2 1.2 11/19/07 : xzao2571" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */

use PRC
go

if exists (select 1 from sysobjects where name = 'XZAO2571' and type = 'P')
	drop procedure XZAO2571
go

create procedure XZAO2571
   @va_Basedd_in    	char(3),
   @va_NomPIC_in    	char(10),
   @va_Autoroute_in 	char(4),
   @va_PR_in        	int,
   @va_Sens_in      	tinyint,
   @va_NomServeur_in	char(10),
   @va_Liaison_in   	tinyint,
   @va_Rgs_in       	char(3),
   @va_NomSite_in   	char(2),
   @va_Supprime_in		bit,
   @va_Adresse_IP_in	char(20),
   @va_Port_in			int,
   @va_Resultat_out		int output
   as 

declare @vl_Autoroute tinyint,
        @vl_AutoMod   tinyint,
        @vl_Type      tinyint,
        @vl_Serveur   smallint,
        @vl_NumPIC    smallint,
        @vl_Numero    smallint,
        @vl_Num       smallint,
        @vl_Site      tinyint,
        @vl_pr_deb    integer,
        @vl_pr_fin    integer,
        @vl_sens      smallint
   
   select @va_Resultat_out = XDC_LU
   
	/*
	** Test des parametres d'entree :
	** Si un des parametres d'entree est egal a null alors XDC_NOK   
	*/
	if @va_Basedd_in             = null
		or @va_NomPIC_in          = null
		or @va_Autoroute_in       = null 
		or @va_PR_in              = null
		or @va_Sens_in            = null
		or @va_NomServeur_in      = null
		or @va_Liaison_in         = null
		or @va_Rgs_in             = null
		or @va_NomSite_in         = null
		or @va_Supprime_in        = null
			return XDC_ARG_INV
  
  	else		
    begin

	/* Verification que le serveur est bien celui du CI */
	select  @vl_Site = numero
	from CFG..RES_DIS
	where serveur = @@servername

	/* if @@servername = XDC_SQLCI */
	if @vl_Site = XDC_CI
	begin tran
	
		/* Travail dans la base CFG */
		if @va_Basedd_in = XDC_BASE_CFG
		begin
	
			/* Verification de l'existence et recherche de l'autoroute et de la PIC comme equipement */
			select @vl_Autoroute = numero 
				from CFG..RES_AUT
					where nom = @va_Autoroute_in
	      
			select @vl_Serveur = numero 
				from CFG..EQT_GEN
					where	nom  = @va_NomServeur_in and
							type = XDC_EQT_SRV

			select @vl_Site = numero 
				from CFG..RES_DIS
					where code = @va_NomSite_in

			if @vl_Site = NULL
				return XDC_ARG_INV	     

			/* Existence de l'autoroute et du serveur */
			if	@vl_Autoroute != null and 
				@vl_Serveur   != null /*and @vl_Type != null*/
			begin

				select @vl_NumPIC = numero 
					from CFG..EQT_GEN
						where	type = XDC_EQT_PIC and 
								nom  = @va_NomPIC_in 

				/* La PIC n'existe pas comme equipement */
				if @vl_NumPIC = null
				begin

					/* Insertion du nouveau picto dans la table EQT_GEN et EQT_PIC */
					select @vl_Numero = max(numero) 
						from CFG..EQT_GEN
							where type = XDC_EQT_PIC

					if @vl_Numero = null
						select @vl_Numero = 0  
	            
					insert into CFG..EQT_GEN (	type,
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
						values ( XDC_EQT_PIC, 
						         @vl_Numero + 1, 
						         @vl_Serveur, 
						         XDC_MAI_PIC, 
						         @vl_Autoroute, 
						         @va_PR_in, 
						         @va_Sens_in, 
						         @va_NomPIC_in,
						         @vl_Site,
						         @va_Supprime_in)

					if @@rowcount <> 1
					begin
						/* Affectation du parametre de sortie Resultat a XDC_AJOUT */
						select @va_Resultat_out = XDC_AJOUT
							return XDC_NOK
	       			end   
	                  
					insert into CFG..EQT_PIC (	numero, 
												liaison_serie,
												rgs,
												adresse_ip,
												port )
						values (	@vl_Numero + 1, 
									@va_Liaison_in, 
									@va_Rgs_in ,
									@va_Adresse_IP_in,
									@va_Port_in)

					/* Erreur d'insertion */
					if @@rowcount <> 1
					begin
						/* Affectation du parametre de sortie Resultat a XDC_AJOUT */
						select @va_Resultat_out = XDC_AJOUT 
							return XDC_NOK
					end   
				end
				else
	        	/* La PIC existe comme equipement */
	        	begin

					/*select @va_NumPIC_out = @vl_NumPIC*/
					/* Modification de la PIC dans la table EQT_GEN et EQT_PIC */
					update CFG..EQT_GEN
						set maitre       = @vl_Serveur,
							type_maitre  = XDC_MAI_PIC,
							autoroute    = @vl_Autoroute,
							PR           = @va_PR_in,
							sens         = @va_Sens_in,
							site_gestion = @vl_Site,
							eqt_supprime	= @va_Supprime_in
								where	numero = @vl_NumPIC and 
										type   = XDC_EQT_PIC    

					/* Erreur de modification */
					if @@rowcount <> 1
					begin
					/* Affectation du parametre de sortie Resultat a XDC_MODIF */
					select @va_Resultat_out = XDC_MODIF   
						return XDC_NOK
					end   

					update CFG..EQT_PIC
						set liaison_serie    = @va_Liaison_in,
							rgs              = @va_Rgs_in,
							adresse_ip = @va_Adresse_IP_in,
							port = @va_Port_in
								where numero = @vl_NumPIC   

					/* Erreur de modification */
					if @@rowcount <> 1
					begin
						/* Affectation du parametre de sortie Resultat a XDC_MODIF */
						select @va_Resultat_out = XDC_MODIF
							return XDC_NOK
					end 
				end
			end
			else
				/* L'autoroute ou le serveur n'existe pas
				** Affectation du parametre de sortie Resultat a XDC_ECHEC
				*/
				select @va_Resultat_out = XDC_ECHEC
	    end

#ifdef CI
         
		else
		begin
         
			/* Travail dans la base CFT */   
			/* Verification de l'existence et recherche de l'autoroute et de la PIC comme equipement */
		
			select @vl_Autoroute = numero 
				from CFT..RES_AUT
					where nom = @va_Autoroute_in

			select @vl_Serveur = numero 
				from CFT..EQT_GEN
					where	nom  = @va_NomServeur_in and
			    			type = XDC_EQT_SRV

			select @vl_Site = numero 
				from CFT..RES_DIS
			    	where code = @va_NomSite_in

			if @vl_Site = NULL
				return XDC_ARG_INV

			/* Existence de l'autoroute et du serveur */
			if @vl_Autoroute != null and @vl_Serveur != null /*and @vl_Type != null*/
			begin
               
				select @vl_NumPIC = numero 
					from CFT..EQT_GEN
						where type = XDC_EQT_PIC and 
						      nom  = @va_NomPIC_in 
                   
				/* La PIC n'existe pas comme equipement */
				if @vl_NumPIC = null
				begin
                  
				/* Insertion de la PIC dans la table EQT_GEN et EQT_PIC */
				select @vl_Numero = max(numero) 
					from CFT..EQT_GEN
						where type = XDC_EQT_PIC
                    
				if @vl_Numero = null
					select @vl_Numero = 0  
                  
				insert into CFT..EQT_GEN (	type,
											numero,
											maitre,
											type_maitre,
											autoroute,
											PR,
											sens,
											nom,
											site_gestion,
											eqt_supprime)
					values (	XDC_EQT_PIC, 
								@vl_Numero + 1, 
								@vl_Serveur, 
								XDC_MAI_PIC, 
								@vl_Autoroute, 
								@va_PR_in, 
								@va_Sens_in, 
								@va_NomPIC_in , 
								@vl_Site,
								@va_Supprime_in)
                  
				if @@rowcount <> 1
				begin
					/* Affectation du parametre de sortie Resultat a XDC_AJOUT */
					select @va_Resultat_out = XDC_AJOUT         
						return XDC_NOK
             	end   
                        
				insert into CFT..EQT_PIC (	numero, 
											liaison_serie,
											rgs, 
											adresse_ip,
											port)
					values (	@vl_Numero + 1, 
								@va_Liaison_in, 
								@va_Rgs_in ,
								@va_Adresse_IP_in,
								@va_Port_in)
                  
				/* Erreur d'insertion */                     
				if @@rowcount <> 1
				begin
					/* Affectation du parametre de sortie Resultat a XDC_AJOUT */
					select @va_Resultat_out = XDC_AJOUT
						return XDC_NOK
				end
			end
			else
				/* La PIC existe comme equipement */
        	begin
               
				/*select @va_NumPIC_out = @vl_NumPIC*/

				/* Modification de la PIC dans la table EQT_GEN et EQT_PIC */
				update CFT..EQT_GEN
					set maitre       = @vl_Serveur,
						type_maitre  = XDC_MAI_PIC,
						autoroute    = @vl_Autoroute,
						PR           = @va_PR_in,
						sens         = @va_Sens_in,
						site_gestion = @vl_Site,
						eqt_supprime = @va_Supprime_in
						where	numero = @vl_NumPIC and 
				     			type   = XDC_EQT_PIC    

				/* Erreur de modification */    
				if @@rowcount <> 1
				begin
					/* Affectation du parametre de sortie Resultat a XDC_MODIF */
					select @va_Resultat_out = XDC_MODIF
						return XDC_NOK
				end   

				update CFT..EQT_PIC
					set liaison_serie    = @va_Liaison_in,
					    rgs              = @va_Rgs_in,
					    adresse_ip = @va_Adresse_IP_in,
					    port = @va_Port_in
						where numero = @vl_NumPIC   

				/* Erreur de modification */
				if @@rowcount <> 1
				begin
					/* Affectation du parametre de sortie Resultat a XDC_MODIF */
					select @va_Resultat_out = XDC_MODIF    
						return XDC_NOK
				end               
			end
		end
		else
			
			/* L'autoroute ou le serveur n'existe pas
				Affectation du parametre de sortie Resultat a XDC_ECHEC */
			select @va_Resultat_out = XDC_ECHEC

	end

#endif

	commit
	end
	return XDC_OK

go
