/*Fichier : $Id: xzao510sp.prc
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao510sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------*/

/* fichiers inclus */
/* declaration de fonctions internes */
/* definition de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Ajout d'un batiment dans la base
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

if exists (select 1 from sysobjects where name = 'XZAO510' and type = 'P')
	drop procedure XZAO510
go

use PRC
go

create procedure XZAO510
   @va_Basedd_in        char(3),
   @va_Numero_in		tinyint,
   @va_NomBatiment_in	char(25),
   @va_Abreviation_in	char(15),
   @va_Autoroute_in		char(4),
   @va_Sens_in			tinyint,
   @va_PR_in			int,
   @va_Creation_in		bit,
   @va_Resultat_out		int output
   as 
/* 
*
* SP	XZAO510SP
*
* PARAMETRES EN ENTREE :
*
*	XDY_Basedd			va_Basedd_in
*	XZAOT_ConfBat		va_Batiment_in
*		XDY_Octet			Numero
*		XDY_Nom				NomBatiment
*		XDY_Abrev			Abreviation
*		XDY_NomAuto			Autoroute
*		XDY_PR				PR
*		XDY_Sens			Sens
*		XDY_Booleen			Creation
*
* PARAMETRES EN SORTIE :
*
*	XDY_ResConf			*va_Resultat_out
*
* VALEUR RENDUE :
*
* Ajoute un Batiment en base
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
* Insertion ou modification dans la table RES_BAT (Batiment)
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* GGY	16/02/24	: Creation (DEM-483)
*
------------------------------------------------------*/
declare

	/* Definition des variables globales */
	@vl_NumAutoroute	tinyint,
	@vl_Portion			smallint,
	@vl_NumBat			smallint,
	@vl_Numero			smallint,
	@vl_Site			char(10)

	select @va_Resultat_out = XDC_LU
   
	/* Test des parametres d'entree : Si un des parametres d'entree est egal a null alors XDC_NOK */
	if	@va_Basedd_in		= null	or @va_Numero_in	= null	or @va_NomBatiment_in	= null or
   		@va_Abreviation_in	= null	or @va_Autoroute_in = null	or @va_Sens_in			= null or
		@va_PR_in			= null	or @va_Creation_in	= null
			return XDC_ARG_INV
	else   
	begin
		/* Verification que le site est bien celui du CI ou CA */
		select @vl_Site = @@servername
			
		/* Si le site est CI ou CA */
		if @vl_Site=XDC_SQLCI or @vl_Site=XDC_SQLCA
		begin			
			/* Travail dans la base CFG */   
			if @va_Basedd_in = XDC_BASE_CFG
		 	begin
			    /* Verification de l'existence et recherche du numero de l'autoroute */
				select @vl_NumAutoroute = numero 
					from CFG..RES_AUT 
						where nom = @va_Autoroute_in

				select @vl_Portion = numero 
					from  CFG..RES_POR    
						where	autoroute = @vl_NumAutoroute and
								(PR_debut < @va_PR_in and PR_fin >= @va_PR_in )


		    	if @vl_Portion <> null
		    	begin

					/* Insertion ou Modification du batiment
					** Verification de l'existence et recherche du batiment dans RES_BAT */
					select @vl_NumBat = numero
						from CFG..RES_BAT
		    	    		where numero = @va_Numero_in

					/*A L'aire n'existe pas dans RES_BAT */
					if @vl_NumBat = null
					begin
						/* Creation positionne =>
						** Insertion du batiment dans la table RES_BAT */
						if @va_Creation_in = XDC_VRAI
						begin
							insert into CFG..RES_BAT values (	@va_Numero_in, @va_NomBatiment_in, @va_Abreviation_in, @vl_NumAutoroute,
																@va_PR_in, @va_Sens_in, @vl_Portion, XDC_FAUX )

							/* Erreur d'insertion */
							if @@rowcount <> 1
							begin
								/* Affectation du parametre de sortie Resultat a XDC_AJOUT */
								select @va_Resultat_out = XDC_AJOUT
								return XDC_NOK
							end

					      	/* Affectation du parametre de sortie Resultat a XDC_AJOUT */
							select @va_Resultat_out = XDC_AJOUT
					   end
					end

					else
					/* L'aire existe deja */
					begin
						/* Creation positionne =>
						** Modification du batiment dans la table RES_BAT */
						if @va_Creation_in = XDC_VRAI
						begin

							update CFG..RES_BAT
								set	nom         = @va_NomBatiment_in, 
									abreviation = @va_Abreviation_in,
									autoroute	= @vl_NumAutoroute,
									PR		    = @va_PR_in, 
									sens        = @va_Sens_in,
									portion		= @vl_Portion,
									supprime    = XDC_FAUX   
										where numero   = @vl_NumBat  

							/* Erreur de modification */
							if @@rowcount > 1
							begin
								/* Affectation du parametre de sortie Resultat a XDC_MODIF */
								select @va_Resultat_out = XDC_MODIF
								return XDC_NOK
							end

							/* Affectation du parametre de sortie Resultat a XDC_MODIF */
							select @va_Resultat_out = XDC_MODIF
						end

						else
						/* Creation non positionne =>
						** Marquage de suppression du batiment dans la table RES_BAT */
						begin
							update CFG..RES_BAT
								set supprime = XDC_VRAI   
									where numero = @vl_NumBat

							/* Erreur de suppression */
							if @@rowcount > 1
							begin
								/* Affectation du parametre de sortie Resultat a XDC_SUPPR */
								select @va_Resultat_out = XDC_SUPPR
								return XDC_NOK
							end

							/* Affectation du parametre de sortie Resultat a XDC_SUPPR */
							select @va_Resultat_out = XDC_SUPPR
						end
					end   
				end
				else
					/* L'autoroute n'existe pas
					**		Affectation du parametre de sortie Resultat a XDC_ECHEC */
					select @va_Resultat_out = XDC_ECHEC
			end	
#ifdef CI
			else
			/* Travail dans la base CFT */   
			begin
				/* Verification de l'existence et recherche du numero de l'autoroute */                
				select @vl_NumAutoroute = numero 
					from CFT..RES_AUT 
						where nom = @va_Autoroute_in

				select @vl_Portion = numero 
					from  CFT..RES_POR    
						where	autoroute = @vl_NumAutoroute and
								(PR_debut < @va_PR_in and PR_fin >= @va_PR_in )

				if @vl_Portion <> null
				begin
			
					/* Insertion ou Modification du batiment
					** 		Verification de l'existence et recherche du batiment dans RES_BAT */
					select @vl_NumBat = numero 
						from CFT..RES_BAT
							where numero = @va_Numero_in

					/* L'aire n'existe pas dans RES_BAT */
					if @vl_NumBat = null
					begin
						/* Creation positionne =>
						** Insertion du batiment dans la table RES_BAT */
						if @va_Creation_in = XDC_VRAI
						begin
							insert into CFT..RES_BAT values (	@va_Numero_in, @va_NomBatiment_in, @va_Abreviation_in, @vl_NumAutoroute,
																@va_PR_in, @va_Sens_in, @vl_Portion, XDC_FAUX )
					
							/* Erreur d'insertion */
							if @@rowcount <> 1
							begin
								/* Affectation du parametre de sortie Resultat a XDC_AJOUT */
								select @va_Resultat_out = XDC_AJOUT
								return XDC_NOK
							end

							/* Affectation du parametre de sortie Resultat a XDC_AJOUT */
							select @va_Resultat_out = XDC_AJOUT
						end
					end
					else
					/* L'aire existe deja */      
					begin
						/* Creation positionne =>
						** Modification du batiment dans la table RES_BAT */
						if @va_Creation_in = XDC_VRAI
						begin
							update CFT..RES_BAT
								set	nom         = @va_NomBatiment_in, 
									abreviation = @va_Abreviation_in,
									autoroute	= @vl_NumAutoroute,
									PR		    = @va_PR_in, 
									sens        = @va_Sens_in,
									portion		= @vl_Portion,
									supprime    = XDC_FAUX   
										where numero   = @vl_NumBat  

							/* Erreur de modification */
							if @@rowcount > 1
							begin
								/* Affectation du parametre de sortie Resultat a XDC_MODIF */
								select @va_Resultat_out = XDC_MODIF
								return XDC_NOK
							end

							/* Affectation du parametre de sortie Resultat a XDC_MODIF */
							select @va_Resultat_out = XDC_MODIF
						end
						else
						/* Creation non positionne =>
						** Marquage de suppression du batiment dans la table RES_BAT */
						begin
							update CFT..RES_BAT
								set supprime    = XDC_VRAI   
									where numero   = @vl_NumBat

							/* Erreur de suppression */
							if @@rowcount > 1
							begin
								/* Affectation du parametre de sortie Resultat a XDC_SUPPR */
								select @va_Resultat_out = XDC_SUPPR
								return XDC_NOK
							end

							/* Affectation du parametre de sortie Resultat a XDC_SUPPR */
							select @va_Resultat_out = XDC_SUPPR
						end
					end   
				end
				else
					/* L'autoroute n'existe pas
					** 		Affectation du parametre de sortie Resultat a XDC_ECHEC */
					select @va_Resultat_out = XDC_ECHEC
			end
#endif
		end

	end         

	return XDC_OK

go
