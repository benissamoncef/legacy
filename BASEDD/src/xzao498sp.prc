/*E*/
/* Fichier : $Id: xzao498sp.prc,v 1.2 2021/06/07 10:26:39 pc2dpdy Exp $        $Revision: 1.2 $        $Date: 2021/06/07 10:26:39 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao498sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* CGR   05/10/20 : Creation 1.1 DEM-SAE155
* ABE	07/06/21 : Fix sortie sur ajout, maj. Ajout numero_in dans le filtre DEM-SAE155 1.3
* GGY	26/09/23 : Ajout district CA (DEM483)
* GGY	17/10/23 : Correction si CFG vide (DEM483)
------------------------------------------------------ */
/* fichiers inclus */
/* static char *version = "@(#)xzao498sp.prc    1.1 1.1 05/10/20 : xzao498" ; */
/* declaration de fonctions internes */
/* definition de fonctions externes */

/*X
------------------------------------------------------
* SERVICE RENDU :
* Ajout d'un message  IMU  dans la base
*
* SEQUENCE D'APPEL :
* SP    XZAO498_Ajouter_Msg_IMU
*
* Arguments en entree
* XDY_Basedd     va_Basedd_in
* XDY_Eqt	 va_Numero_in
* XDY_Texte      va_Libelle_in
*
* Arguments en sortie
* XDC_Entier    va_Resultat_out
*
------------------------------------------------------
*/
use PRC
go
if exists (select 1 from sysobjects where name = 'XZAO498' and type = 'P')
        drop procedure XZAO498
go
create procedure XZAO498
	@va_Basedd_in                char(3),
	@va_Numero_in                int,
	@va_Libelle_in               char(251),
	@va_Resultat_out             int output
	as
------------------------------------------------------*/
/*A
** Definition des variables globales
*/
declare @vl_NomMessage          char(251),
		@vl_Site                char(10)

select @va_Resultat_out = XDC_LU
/*A
** Test des parametres d'entree :
**   Si un des parametres d'entree est egal a null alors XDC_NOK
*/
if @va_Basedd_in                = null
   or @va_Numero_in             = null
   or @va_Libelle_in            = null
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
			
			select @vl_NomMessage = libelle from CFG..EQT_IMU_MSG
			where libelle = @va_Libelle_in and
			      numero = @va_Numero_in 
			/*A
			** Le message  n existe pas dans la table EQT_IMU_MSG
			*/
			if @vl_NomMessage = null
				begin
				
				/*A
				** Insertion du message dans la table EQT_IMU_MSG
				*/
				
				insert into CFG..EQT_IMU_MSG (numero,
							      libelle)
				values ( @va_Numero_in,
					 @va_Libelle_in)
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
				select @va_Resultat_out = XDC_AJOUT
				end
			else
				/*A
				** Le message existe deja dans la table
				*/
				
				begin
				
				/*A
				** Modification du message dans la table EQT_IMU_MSG
				*/
				update CFG..EQT_IMU_MSG
				set numero = @va_Numero_in
				where libelle = @va_Libelle_in and
			      	      numero = @va_Numero_in 
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
				select @va_Resultat_out = XDC_MODIF
				end
				
			end
		
		end
	#ifdef CI
	
	
		begin
		/*A
		**  Travail dans la base CFT
		*/
		select @vl_NomMessage = libelle from CFT..EQT_IMU_MSG
		where libelle = @va_Libelle_in and
		      numero = @va_Numero_in
		/*A
		** Le message n existe pas dans la table EQT_IMU_MSG
		*/
		if @vl_NomMessage = null
			begin
			
			/*A
			** Insertion du message dans la table EQT_IMU_MSG
			*/
			insert into CFT..EQT_IMU_MSG (numero,
						      libelle)
			
			values ( @va_Numero_in,
				 @va_Libelle_in)
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
			select @va_Resultat_out = XDC_AJOUT
			end
		else
			/*A
			** Le message existe deja dans la table
			*/
			begin
			/*A
			** Modification du message dans la table EQT_IMU_MSG
			*/
			update CFT..EQT_IMU_MSG
			set numero 	= @va_Numero_in
			where libelle = @va_Libelle_in and
		              numero = @va_Numero_in
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
			select @va_Resultat_out = XDC_MODIF
		end
	#endif
	commit

return XDC_OK
go

