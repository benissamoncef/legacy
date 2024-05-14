/*E*/
/* Fichier : $Id: xzao552sp.prc,v 1.4 2018/07/11 11:17:16 pc2dpdy Exp $        Release : $Revision: 1.4 $        Date : $Date: 2018/07/11 11:17:16 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzao552sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* VR		12/01/12	: Creation (DEM/1016)
* LCL		05/07/17	: Modif PRA
* LCL		11/07/18	: correction syntaxe from
* GGY		26/09/23	: Ajout district CA (DEM483)
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao552sp.h" */

/* static char *version = "@(#)xzao552.c	1.1 23/18/94 : xzao552" ; */

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

if exists (select * from sysobjects where name = 'XZAO552' and type = 'P')
	drop procedure XZAO552
go

create procedure XZAO552
   @va_Basedd_in        char(3),
   @va_Numero_in		smallint,
   @va_Libelle_in		char(60),
   @va_Nominal_in		bit,
   @va_Degrade_in		bit,
   @va_Niveau_in		smallint,
   @va_Inhibition_in	bit,
   @va_Zone_in			smallint,
   @va_Numero_out		smallint output
   as 

/* 
*
* SP    XZAO552SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd        va_Basedd_in
*  					va_Numero_in
* 					va_Libelle_in
*  					va_Nominal_in
*					va_Degrade_in
*					va_Niveau_in
*  					va_Inhibition_in
*  					va_Zone_in
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
* Mise a jour des donnees dans la table REG_SCE
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* VR    12 Jan 2012    : Creation (DEM/1016)
------------------------------------------------------*/
     /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_Basedd_in = null  
   or @va_Numero_in = null 
   or @va_Libelle_in = null 
   or @va_Nominal_in = null 
   or @va_Degrade_in = null 
   or @va_Niveau_in = null
   or @va_Inhibition_in = null 
   or @va_Zone_in = null
         return XDC_ARG_INV
   
   
    declare @vl_numero smallint,
	@vl_nb int,
	@vl_Site 	tinyint

    if @va_Basedd_in             = null
		return XDC_ARG_INV
		
  /*A
  ** Verification que le serveur est bien celui du CI
  */
      
  select @va_Numero_out = @va_Numero_in

	/* Verification du site */
	select  @vl_Site = numero
		from CFG..RES_DIS
		where serveur = @@servername

  /* PRA
  if @@servername = XDC_SQLDP
  */
  if @vl_Site = XDC_DP
  begin
            update CFG..REG_SCE
	           set    libelle = @va_Libelle_in,
			   nominal = @va_Nominal_in,
			   degrade = @va_Degrade_in,
			   niveau  = @va_Niveau_in,
			   inhibition = @va_Inhibition_in
	           where  numero = @va_Numero_in
		       and zone = @va_Zone_in
	
		   		if @@rowcount <> 1
				begin
					select @vl_numero = max(numero) from  CFG..REG_SCE
					if @vl_numero<>0
						select @vl_numero  = @vl_numero +1
					else
						select @vl_numero = 1
					select @va_Numero_out = @vl_numero
					insert into CFG..REG_SCE ( numero,
								   libelle,
								   nominal,
								   degrade,
								   niveau,
								   inhibition,
								   zone)
				        values ( @vl_numero,
						   @va_Libelle_in,
						   @va_Nominal_in,
						   @va_Degrade_in,
						   @va_Niveau_in,
						   @va_Inhibition_in,
						   @va_Zone_in)				
			
					if @@rowcount != 1
						return XDC_NOK
				end
	
				if @va_Nominal_in != 0
					begin
						update CFG..REG_SCE
						set nominal=0
						where zone = @va_Zone_in
						and numero !=  @va_Numero_in
					end
				else
					begin
						select @vl_nb = count(*)
						from CFG..REG_SCE
						where zone = @va_Zone_in
						and numero = @va_Numero_in
						and nominal  != 0
				
						if @vl_nb=0
						begin
							update CFG..REG_SCE
							set nominal =-1
							where zone = @va_Zone_in 
							and niveau = (select min(niveau) from CFG..REG_SCE
								where zone = @va_Zone_in)
						end
					end
	end
 /* PRA
 if @@servername = XDC_SQLCI
 */
 if @vl_Site = XDC_CI or @vl_Site = XDC_CA
  begin
   
	   /*A
	    **  Travail dans la base CFG
	    */  
		if @va_Basedd_in = XDC_BASE_CFG
		begin
	           update CFG..REG_SCE
	           set    libelle = @va_Libelle_in,
			   nominal = @va_Nominal_in,
			   degrade = @va_Degrade_in,
			   niveau  = @va_Niveau_in,
			   inhibition = @va_Inhibition_in
	           where  numero = @va_Numero_in
		       and zone = @va_Zone_in
	
		   		if @@rowcount <> 1
				begin
					select @vl_numero = max(numero) from  CFG..REG_SCE
					if @vl_numero<>0
						select @vl_numero  = @vl_numero +1
					else
						select @vl_numero = 1
					select @va_Numero_out = @vl_numero
					insert into CFG..REG_SCE ( numero,
								   libelle,
								   nominal,
								   degrade,
								   niveau,
								   inhibition,
								   zone)
				        values ( @vl_numero,
						   @va_Libelle_in,
						   @va_Nominal_in,
						   @va_Degrade_in,
						   @va_Niveau_in,
						   @va_Inhibition_in,
						   @va_Zone_in)				
			
					if @@rowcount != 1
						return XDC_NOK
				end
	
				if @va_Nominal_in != 0
					begin
						update CFG..REG_SCE
						set nominal=0
						where zone = @va_Zone_in
						and numero !=  @va_Numero_in
					end
				else
					begin
						select @vl_nb = count(*)
						from CFG..REG_SCE
						where zone = @va_Zone_in
						and numero = @va_Numero_in
						and nominal  != 0
				
						if @vl_nb=0
						begin
							update CFG..REG_SCE
							set nominal =-1
							where zone = @va_Zone_in 
							and niveau = (select min(niveau) from CFG..REG_SCE
								where zone = @va_Zone_in)
						end
					end
		end

#ifdef CI      
    else
	begin
	
	    /*A
	    **  Travail dans la base CFT
	    */   
	
		if @va_Basedd_in = XDC_BASE_CFT
	        
			begin
		           update CFT..REG_SCE
		           set    libelle = @va_Libelle_in,
				   nominal = @va_Nominal_in,
				   degrade = @va_Degrade_in,
				   niveau  = @va_Niveau_in,
				   inhibition = @va_Inhibition_in
		           where  numero = @va_Numero_in
			       and zone = @va_Zone_in
		
		   		   if @@rowcount <> 1
			
				   begin
						select @vl_numero = max(numero) from  CFT..REG_SCE
						if @vl_numero<>0
							select @vl_numero  = @vl_numero +1
						else
							select @vl_numero = 1
							
						select @va_Numero_out = @vl_numero
						
						insert into CFT..REG_SCE ( numero,
									   libelle,
									   nominal,
									   degrade,
									   niveau,
									   inhibition,
									   zone)
					        values ( @vl_numero,
							   @va_Libelle_in,
							   @va_Nominal_in,
							   @va_Degrade_in,
							   @va_Niveau_in,
							   @va_Inhibition_in,
							   @va_Zone_in)				
				
						if @@rowcount != 1
							return XDC_NOK
					end
		
					if @va_Nominal_in != 0
						begin
							update CFT..REG_SCE
							set nominal=0
							where zone = @va_Zone_in
							and numero !=  @va_Numero_in
						end
					else
						begin
							select @vl_nb = count(*)
							from CFT..REG_SCE
							where zone = @va_Zone_in
							and numero = @va_Numero_in
							and nominal  != 0
					
							if @vl_nb=0
								begin
									update CFT..REG_SCE
									set nominal =-1
									where zone = @va_Zone_in 
									and niveau = (select min(niveau) from CFT..REG_SCE
										where zone = @va_Zone_in)
								end
						end
			end
		end
#endif
      
end      
return XDC_OK
go  
