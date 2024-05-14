/*E*/
/*Fichier :  $Id: xzao622sp.prc,v 1.3 2018/07/11 10:54:06 pc2dpdy Exp $      Release : $Revision: 1.3 $        Date : $Date: 2018/07/11 10:54:06 $
------------------------------------------------------
* ASTEK *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao622sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao622sp.h" */

/* static char *version = "@(#)xzao622.c    1.1 05/09/07 : xzao622" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Ajout d'une sequence autorisee d'un equipement du 
*  domaine VENTILATION dans la base
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAO;622
	@va_Basedd_in             char(3),
	@va_NomSequence_in        T_NOM,
	@va_NomVEN_in             T_NOM,
	@va_Adresse_in            smallint,
	@va_BitComm_in            tinyint,
	@va_BitDiscordance_in     tinyint,
	@va_Creation_in           bit,
	@va_Resultat_out          int output
	as 

/* 
*
* SP    XZAO622SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd         va_Basedd_in
* XDY_Nom            va_NomSequence_in
* XDY_Nom            va_NomVEN_in
* XDY_Mot            va_Adresse_in
* XDY_Octet          va_BitComm_in
* XDY_Octet          va_BitDiscordance_in
* XDY_Booleen        @va_Creation_in    
* XDY_Booleen        va_Creation_in
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf        va_Resultat_out    
*
* VALEUR RENDUE :
*
* Ajoute une sequence autorisee d'un equipement 
* du domaine VENTILATION en base
* 
* CODE RETOUR :
*
* 0    XDC_OK
* 1    XDC_NOK 
*
* CONDITIONS D'UTILISATION :
*
* Config OFF LINE
*
* FONCTION :
*
* Insertion ou modification dans la table EQT_SEQ_GTC_VEN (EQUIPEMENT_SEQ_VEN)
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* AAZ	05 Sep 2007	: Creation
* LCL	05/07/17	: modif PRA
* LCL	11/07/18	: correction syntaxe from
* GGY	26/09/23	: Ajout district CA (DEM483)
* GGY	17/10/23	: Correction si CFG vide (DEM483)
------------------------------------------------------*/
declare

	/*A
	** Definition des variables globales
	*/

	@vl_NumVEN         	smallint,
	@vl_NumeroVEN      	smallint,
	@vl_NumSeq         	tinyint,
	@vl_NumeroSeq      	tinyint,
	@vl_Site			char(10)


	select @va_Resultat_out = XDC_ECHEC

	/*A
	** Test des parametres d'entree :
	**   Si un des parametres d'entree est egal a null alors XDC_NOK   
	*/

	if @va_Basedd_in         = null or
	   @va_NomSequence_in    = null or
	   @va_NomSequence_in    = null or
	   @va_NomVEN_in         = null or
	   @va_Adresse_in        = null or
	   @va_BitComm_in        = null or
	   @va_BitDiscordance_in = null or
	   @va_Creation_in       = null 

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
				** Verification de l'existence de la sequence d'un equipement VENTILATION dans EQT_SEQ_GTC_VEN
				** et de l'equipement VENTILATION dans EQT_GTC_VEN
				*/

				select @vl_NumSeq = numero
				from   CFG..EQT_SEQ_GTC_VEN
				where  nom        = @va_NomSequence_in

				/*A
				** La sequence d'un equipement VENTILATION existe dans EQT_SEQ_GTC_VEN
				*/

				if @vl_NumSeq <> null
				begin

					select @vl_NumVEN = numero
					from   CFG..EQT_GTC_VEN
					where  numero in (select numero 
					                  from   CFG..EQT_GEN
					                  where  nom  = @va_NomVEN_in and
					                         type = XDC_EQT_VEN)

					/*A
					** L'equipement VENTILATION existe dans EQT_GTC_VEN
					*/

					if @vl_NumVEN <> null
					begin

						select @vl_NumeroVEN = equipement,
						       @vl_NumeroSeq = sequence
						from   CFG..EQT_SQA_GTC_VEN
						where  equipement    = @vl_NumVEN and
						       sequence      = @vl_NumSeq

						/*A
						** La sequence autorisee de l'equipement du domaine VENTILATION
						** n'existe pas dans EQT_SQA_GTC_VEN
						*/

						if @vl_NumeroVEN = null and @vl_NumeroSeq = null
						begin

							/*A
							** Creation positionne =>
							** Insertion de la sequence autorisee d'un equipement du domaine VENTILATION
							** dans la table EQT_SQA_GTC_VEN
							*/

							if @va_Creation_in = XDC_VRAI
							begin

								insert into CFG..EQT_SQA_GTC_VEN ( type,
								                                   equipement,
								                                   sequence )
								                          values ( XDC_EQT_VEN,
								                                   @vl_NumVEN, 
								                                   @vl_NumSeq )

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
								** Creation non positionne =>
								** Affectation du parametre de sortie Resultat a XDC_ECHEC
								*/

								select @va_Resultat_out = XDC_ECHEC

						end

						else

						/*A
						** La sequence autorisee de l'equipement VENTILATION existe dans EQT_SQA_GTC_VEN
						*/

						begin

							if @va_Creation_in = XDC_VRAI

							/*A 
							** Creation non positionne =>
							** Modification de la sequence autorisee d'un equipement du domaine VENTILATION
							** dans la table EQT_SQA_GTC_VEN
							*/

							begin
								
								/* Pas de champ � mettre � jour dans la GTC r�nov�e */
								/*
								update CFG..EQT_SQA_GTC_VEN
								set    adresse    = @va_Adresse_in,
								       bit_comm   = @va_BitComm_in,
								       bit_disc   = @va_BitDiscordance_in
								where  equipement = @vl_NumVEN and
								       sequence   = @vl_NumSeq

								//A
								// Si erreur de modification
								//

								if @@rowcount <> 1
								begin

									//A
									// Affectation du parametre de sortie Resultat a XDC_MODIF
									//

									select @va_Resultat_out = XDC_MODIF

									return XDC_NOK
								end
								*/

								/*A
								** Affectation du parametre de sortie Resultat a XDC_MODIF
								*/
         
								select @va_Resultat_out = XDC_MODIF
							end

						else       

						/*A
						** Creation non positionne =>
						** Suppression de la sequence autorisee d'un equipement VENTILATION dans la table EQT_SQA_GTC_VEN
						*/

						begin

							delete CFG..EQT_SQA_GTC_VEN
							where  type       = XDC_EQT_VEN and
							       equipement = @vl_NumVEN  and
							       sequence   = @vl_NumSeq     

							/*A
							** Si erreur de suppression
							*/

							if @@rowcount <> 1
							begin

								/*A
								** Affectation du parametre de sortie Resultat a XDC_SUPPR
								*/

								select @va_Resultat_out = XDC_SUPPR

								return XDC_NOK
							end

							/*A
							** Affectation du parametre de sortie Resultat a XDC_SUPPR
							*/

							select @va_Resultat_out = XDC_SUPPR
						end
					end
				end   

				else

					/*A
					** L'equipement VENTILATION n'existe pas 
					** Affectation du parametre de sortie Resultat a XDC_ECHEC
					*/

					select @va_Resultat_out = XDC_ECHEC
			end

			else

				/*A
				** La sequence du domaine VENTILATION n'existe pas 
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
			** Verification de l'existence de la sequence d'un equipement VENTILATION dans EQT_SEQ_GTC_VEN
			** et de l'equipement VENTILATION dans EQT_GTC_VEN
			*/

			select @vl_NumSeq = numero
			from   CFT..EQT_SEQ_GTC_VEN
			where  nom = @va_NomSequence_in

			/*A
			** La sequence d'un equipement VENTILATION existe dans EQT_SEQ_GTC_VEN
			*/

			if @vl_NumSeq <> null
			begin

				select @vl_NumVEN = numero
				from   CFT..EQT_GTC_VEN
				where  numero in ( select numero
				                   from   CFT..EQT_GEN
				                   where  nom  = @va_NomVEN_in and
				                          type = XDC_EQT_VEN )

				/*A
				** L'equipement VENTILATION existe dans EQT_GTC_VEN
				*/

				if @vl_NumVEN <> null
				begin

					select @vl_NumeroVEN = equipement,
					       @vl_NumeroSeq = sequence
					from   CFT..EQT_SQA_GTC_VEN
					where  equipement    = @vl_NumVEN and
					       sequence      = @vl_NumSeq

					/*A
					** La sequence autorisee de l'equipement VENTILATION n'existe pas dans EQT_SQA_GTC_VEN
					*/

					if @vl_NumeroVEN = null and
					   @vl_NumeroSeq = null
					begin

						/*A
						** Creation positionne =>
						** Insertion de la sequence autorisee d'un equipement VENTILATION dans la table EQT_SQA_GTC_VEN
						*/

						if @va_Creation_in = XDC_VRAI
						begin

							insert into CFT..EQT_SQA_GTC_VEN ( type,
							                                   equipement,
							                                   sequence )
							                          values ( XDC_EQT_VEN,
							                                   @vl_NumVEN,
							                                   @vl_NumSeq )

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
							** Creation non positionne =>
							** Affectation du parametre de sortie Resultat a XDC_ECHEC
							*/

							select @va_Resultat_out = XDC_ECHEC

					end

					else

					/*A
					** La sequence autorisee de l'equipement VENTILATION existe dans EQT_SQA_GTC_VEN
					*/

					begin

						if @va_Creation_in = XDC_VRAI

						/*A 
						** Creation non positionne =>
						** Modification de la sequence autorisee d'un equipement VENTILATION dans la table EQT_SQA_GTC_VEN
						*/

						begin

							/* Pas de champ � mettre � jour dans la GTC r�nov�e */
							/*

							update CFT..EQT_SQA_GTC_VEN
							set    adresse    = @va_Adresse_in,
							       bit_comm   = @va_BitComm_in,
							       bit_disc   = @va_BitDiscordance_in
							where  equipement = @vl_NumVEN and
							       sequence   = @vl_NumSeq   

							//A
							// Si erreur de modification
							//

							if @@rowcount <> 1
							begin

								//A
								// Affectation du parametre de sortie Resultat a XDC_MODIF
								//

								select @va_Resultat_out = XDC_MODIF

								return XDC_NOK
							end
							*/

							/*A
							** Affectation du parametre de sortie Resultat a XDC_MODIF
							*/

							select @va_Resultat_out = XDC_MODIF
						end

						else

						/*A
						** Creation non positionne =>
						** Suppression de la sequence autorisee d'un equipement VENTILATION
						** dans la table EQT_SQA_GTC_VEN
						*/

						begin

							delete CFT..EQT_SQA_GTC_VEN
							where  type       = XDC_EQT_VEN and
							       equipement = @vl_NumVEN and
							       sequence   = @vl_NumSeq     

							/*A
							** Si erreur de suppression
							*/

							if @@rowcount <> 1
							begin

								/*A
								** Affectation du parametre de sortie Resultat a XDC_SUPPR
								*/

								select @va_Resultat_out = XDC_SUPPR

								return XDC_NOK
							end

							/*A
							** Affectation du parametre de sortie Resultat a XDC_SUPPR
							*/

							select @va_Resultat_out = XDC_SUPPR
						end
					end
				end   

				else

					/*A
					** L'equipement VENTILATION n'existe pas 
					** Affectation du parametre de sortie Resultat a XDC_ECHEC
					*/

					select @va_Resultat_out = XDC_ECHEC
				end

				else

					/*A
					** La sequence de l'equipement VENTILATION n'existe pas 
					** Affectation du parametre de sortie Resultat a XDC_ECHEC
					*/

					select @va_Resultat_out = XDC_ECHEC

			end 
#endif      

		end
	end         

	return XDC_OK
         
go  
