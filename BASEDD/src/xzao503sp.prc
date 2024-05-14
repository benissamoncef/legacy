/*E*/
/* Fichier : $Id: xzao503sp.prc,v 1.1 2021/06/03 07:08:54 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2021/06/03 07:08:54 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao503sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
* ABE	11/02/21 : Création SONO DEM-SAE244 1.1
* GGY	26/09/23 : Ajout district CA (DEM483)
* GGY	17/10/23 : Correction si CFG vide (DEM483)
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao503sp.h" */

/* static char *version = "@(#)xzao503sp.prc	1.1 1.1 28/07/20: xzao503" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Ajout d'une zone sonorisation dans la base
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAO503' and type = 'P')
	drop procedure XZAO503
go

create procedure XZAO503
   @va_Basedd_in        	char(3),
   @va_id_in			int,
   @va_libelle_in              	char(30),
   @va_autoroute_in 		tinyint, 
   @va_sens_in		 	tinyint,
   @va_PR_debut_in	  	T_PR,
   @va_PR_fin_in	 	T_PR,
   @va_Resultat_out             int output
   as 

/* 
*
* SP	XZAO503SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Entier	 va_Basedd_in		: Base visée
* XDY_Entier	 va_id_in		: Id de la zone
* XDY_Texte      libelle 		: Nom de la zone
* XDY_Autoroute	 autoroute_zone		: autoroute de la zone
* XDY_Sens	 sens_zone		: sens de la zone
* XDY_PR	 PR_debut_zone		: PR debut de la zone
* XDY_PR	 PR_fin_zone		: PR fin de la zone
*
* PARAMETRES EN SORTIE :
*
* XDY_Entier	Va_Resultat_out		: Résultat
*
* VALEUR RENDUE :
*
* Retourne le resultat de l'insertion en base
* 
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK 
*
* CONDITIONS D'UTILISATION :
*
* FONCTION :
*
* MECANISMES :
*
*/

/*A
** Definition des variables globales
*/
declare @vl_id             char(30),
  		@vl_Site			char(10)


/*A
** Test des parametres d'entree :
**   Si un des parametres d'entree est egal a null alors XDC_ARG_INV 
*/

if @va_Basedd_in = null or
  @va_libelle_in = null or
  @va_autoroute_in = null or 
  @va_sens_in = null or
  @va_PR_debut_in = null or
  @va_PR_fin_in = null or
	return XDC_ARG_INV

else   
begin

		/* Verification que le site est bien celui du CI ou CA */
		select  @vl_Site = @@servername
	   
		/* Si le site est CI ou CA */
		if @vl_Site=XDC_SQLCI or @vl_Site=XDC_SQLCA
		begin tran
		
		/*A
		**  Travail dans la base CFG
		*/
		
		if @va_Basedd_in = XDC_BASE_CFG
	
		begin
			
			select @vl_id = id from CFG..ZON_SONO
			where 	id = @va_id_in
			/*A
			** La zone n existe pas dans la table ZON..SONO
			*/
			if @vl_Id_Zone = null
		
			begin
				
				/*A
				** Insertion de la Zone SONO dans la table ZON_SONO
				*/
				
				insert into CFG..ZON_SONO (id,
							libelle,
							autoroute,
							sens,
							PR_debut,
							PR_fin)
				values (@va_id_in,
					@va_libelle_in,
					@va_autoroute_in,
					@va_sens_in,
					@va_PR_debut_in,
					@va_PR_fin_in)
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
			end
			else
			
			begin
				/*A
				** La zone existe deja dans la table
				*/
								
				/*A
				** Modification de la Zone dans la table ZON_SONO
				*/
				update CFG..ZON_SONO
				set libelle = @va_libelle_in,
					autoroute = @va_autoroute_in,
					sens = @va_sens_in,
					PR_debut = @va_PR_debut_in,
					PR_fin = @va_PR_fin_in
				where id = @va_id_in
				/*A
				** Erreur de modification
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
	end
#ifdef CI
	
	
	begin
		/*A
		**  Travail dans la base CFT
		*/
		select @vl_id = id from CFT..ZON_SONO
			where 	id = @va_id_in
			/*A
			** La zone n existe pas dans la table ZON_SONO
			*/
			if @vl_Id_Zone = null
	
		begin
			
		
			/*A
			** Insertion de la Zone SONO dans la table ZON_SONO
			*/
			
			insert into CFT..ZON_SONO (id,
						libelle,
						autoroute,
						sens,
						PR_debut,
						PR_fin)
			values (@va_id_in,
				@va_libelle_in,
				@va_autoroute_in,
				@va_sens_in,
				@va_PR_debut_in,
				@va_PR_fin_in)
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
		end
		else
		
		begin
			/*A
			** La zone existe deja dans la table
			*/
							
			/*A
			** Modification de la Zone dans la table ZON_SONO
			*/
			update CFT..ZON_SONO
			set libelle = @va_libelle_in,
				autoroute = @va_autoroute_in,
				sens = @va_sens_in,
				PR_debut = @va_PR_debut_in,
				PR_fin = @va_PR_fin_in
			where id = @va_id_in
			/*A
			** Erreur de modification
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
	commit



     
return XDC_OK         
go  
