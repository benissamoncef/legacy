/*E*/
/* Fichier : $Id: xzao495sp.prc,v 1.3 2021/10/19 09:32:17 pc2dpdy Exp $        $Revision: 1.3 $        $Date: 2021/10/19 09:32:17 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao495sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* CGR   05/10/20	: Creation 1.1 DEM-SAE155
* ABE	07/06/21	: Fix sortie sur ajout, maj DEM-SAE155 1.2
* ABE	19/10/21	: Fix insertion sur cle numero et id DEM-SAE155 1.3
* ABE	02/02/22	: Fix update sur la cle numero et id DEM-SAE155 1.4
* GGY	26/09/23	: Ajout district CA (DEM483)
* GGY	17/10/23	: Correction si CFG vide (DEM483)
------------------------------------------------------ */
/* fichiers inclus */
/* static char *version = "@(#)xzao495sp.prc    1.1 1.1 05/10/20 : xzao495" ; */
/* declaration de fonctions internes */
/* definition de fonctions externes */

/*X
------------------------------------------------------
* SERVICE RENDU :
* Ajout d'une Zone IMU  dans la base
*
* SEQUENCE D'APPEL :
* SP    XZAO495_Ajouter_Zon_IMU
*
* Arguments en entree
* XDY_Basedd     va_Basedd_in
* XDY_Eqt	 va_Numero_in
* XDY_Entier     va_Id_in
* XDY_Texte      va_Libelle_in
*
* Arguments en sortie
* XDC_Entier    va_Resultat_out
*
------------------------------------------------------
*/
use PRC
go
if exists (select 1 from sysobjects where name = 'XZAO495' and type = 'P')
        drop procedure XZAO495
go
create procedure XZAO495
	@va_Basedd_in                char(3),
	@va_Numero_in                smallint,
	@va_Id_in		     int,
	@va_Libelle_in               char(251),
	@va_Resultat_out             int output
	as
------------------------------------------------------*/
/*A
** Definition des variables globales
*/
declare @vl_IdZone              smallint,
		@vl_Site                char(10)

select @va_Resultat_out = XDC_LU
/*A
** Test des parametres d'entree :
**   Si un des parametres d'entree est egal a null alors XDC_NOK
*/
if @va_Basedd_in                = null
   or @va_Numero_in             = null
   or @va_Id_in          	= null
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
			
			select @vl_IdZone = id from CFG..EQT_IMU_ZON
			where id      = @va_Id_in
			and numero = @va_Numero_in
			/*A
			** La zone n existe pas dans la table EQT_IMU_ZON
			*/
			if @vl_IdZone = null
		
			begin
				
				/*A
				** Insertion de la Zone IMU dans la table EQT_IMU_ZON
				*/
				
				insert into CFG..EQT_IMU_ZON (numero,
							      id,
							      libelle)
				values ( @va_Numero_in,
					 @va_Id_in,
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
					commit tran
					return XDC_NOK
					
				end
				select @va_Resultat_out = XDC_AJOUT
			end
			else
			
			begin
				/*A
				** La zone existe deja dans la table
				*/
								
				/*A
				** Modification de la Zone dans la table EQT_IMU_ZON
				*/
				update CFG..EQT_IMU_ZON
				set libelle = @va_Libelle_in
				where id = @va_Id_in and			
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
					commit tran
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
		select @vl_IdZone = id from CFT..EQT_IMU_ZON
		where id      = @va_Id_in
		and numero = @va_Numero_in
		/*A
		** La Zone n existe pas dans la table EQT_IMU_ZON
		*/
		if @vl_IdZone = null
	
		begin
			
			/*A
			** Insertion de la Zone dans la table EQT_IMU_ZON
			*/
			insert into CFT..EQT_IMU_ZON (numero,
						      id,
						      libelle)
			
			values ( @va_Numero_in,
				 @va_Id_in,
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
				commit tran
				return XDC_NOK
			end
			select @va_Resultat_out = XDC_AJOUT
		end
		else
			/*A
			** La Zone existe deja dans la table
			*/
		begin
			/*A
			** Modification de la Zone dans la table EQT_IMU_ZON
			*/
			update CFT..EQT_IMU_ZON
			set	libelle = @va_Libelle_in
			where id = @va_Id_in and			
			 numero 	= @va_Numero_in
					
			/*A
			** Erreur de modification
			*/
			if @@rowcount <> 1
		
			begin
				/*A
				** Affectation du parametre de sortie Resultat a XDC_MODIF
				*/
				select @va_Resultat_out = XDC_MODIF
				commit tran
				return XDC_NOK
			end
			select @va_Resultat_out = XDC_MODIF
		end
	end
#endif
	commit tran
return XDC_OK
go

