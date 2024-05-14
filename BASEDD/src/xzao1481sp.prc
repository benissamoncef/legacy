/*E*/
/*Fichier : %W%      Release : %I%        Date : %G%
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao148sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao148sp.h" */

/* static char *version = "%W%  %G% : xzao148" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Liste des portions d'un district
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAO1481' and type = 'P')
	drop procedure XZAO1481
go

create procedure XZAO1481
	@va_Basedd_in       	char(3),
	@va_NomDistrict_in 	char(25)	
as 

/* 
*
* SP	XZAO148SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Entier	va_Basedd_in			
* XDY_Nom	va_NomDistrict_in
*
* PARAMETRES EN SORTIE :
*
* VALEUR RENDUE :
*
*  Liste des portions d'un district
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
* Retourne  XZAOT_Portion
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	14/02/21012 : creation pour flash V2 
* LCL 06/02/2018  : Modifications suite a REGIO
* LCL 26/09/2023  : Gestion dynamique des districts DEM-483
* LCL	28/09/23 	: Adaptations CNA DEM-483	
------------------------------------------------------*/
 
declare @vl_numero int

/*A
** Test des parametres d'entree :
**   Si un des parametres d'entree est egal a null alors XDC_NOK   
*/
    
if rtrim ( @va_Basedd_in ) 		= null	select @va_Basedd_in = null
if rtrim ( @va_NomDistrict_in ) = null 	select @va_NomDistrict_in = null


if @va_Basedd_in = null
or @va_NomDistrict_in = null 
    return XDC_ARG_INV
else   

/*A
** Les parametres d'entree sont non nuls
*/

begin
  
	/*A
	**  Travail dans la base CFG
	*/   
	
	if @va_Basedd_in = XDC_BASE_CFG
	begin
      
		/*A
		** Recherche des portions du district
		*/        
		select @vl_numero = numero
		from CFG..RES_DIS
		where nom = @va_NomDistrict_in

		if @vl_numero = XDC_VC
		begin
			select POR.numero, POR.autoroute, POR.PR_debut, POR.PR_fin, POR.district_recouvrement
		  		from CFG..RES_POR POR, CFG..RES_DIS DIS, CFG..RES_DIS DISR
		    		where POR.district = DIS.numero 
		      			and DIS.numero    in (XDC_VC,XDC_DP) 
		    				group by DIS.nom, POR.numero   
		end
    	else if @vl_numero = XDC_DP
    	begin
			select POR.numero, POR.autoroute, POR.PR_debut, POR.PR_fin, POR.district_recouvrement
				from CFG..RES_POR POR, CFG..RES_DIS DIS, CFG..RES_DIS DISR
					where POR.district = DIS.numero
						and POR.numero != 67
						and POR.numero != 22
						and DIS.numero    in (XDC_VC,XDC_DP)
							group by DIS.nom, POR.numero
      		union
      		select POR.numero, 3, 72790, 79590, POR.district_recouvrement
				from CFG..RES_POR POR, CFG..RES_DIS DIS, CFG..RES_DIS DISR
					where POR.district = DIS.numero
						and DIS.numero      in (XDC_VC,XDC_DP)
						and POR.numero = 67
        					group by DIS.nom, POR.numero
      		union
      		select POR.numero, 3, 79590, 80790, 25 
				from CFG..RES_POR POR, CFG..RES_DIS DIS, CFG..RES_DIS DISR
					where POR.district = DIS.numero
						and DIS.numero      in (XDC_VC,XDC_DP)
						and POR.numero = 22
							group by DIS.nom, POR.numero
    	end
    	else
    	/*if @vl_numero = XDC_CA : Tous les sites autres que ESCOTA */    
    	begin
      		select POR.numero, POR.autoroute, POR.PR_debut, POR.PR_fin, POR.district_recouvrement
      			from CFG..RES_POR POR, CFG..RES_DIS DIS, CFG..RES_DIS DISR
        			where POR.district = DIS.numero
          				and DIS.numero = @vl_numero
        					group by DIS.nom, POR.numero
    	end
	end
	else
#ifdef CI
	begin
  	/*A
  	**  Travail dans la base CFT
  	*/   
  
		if @va_Basedd_in = XDC_BASE_CFT
		begin
		/*A
		** Recherche des portions du district
		*/
			select @vl_numero = numero
				from CFT..RES_DIS
					where nom = @va_NomDistrict_in

			if @vl_numero in (XDC_CI,XDC_DP,XDC_VC)
			begin
				select POR.numero, POR.autoroute, POR.PR_debut, POR.PR_fin, POR.district_recouvrement
					from CFT..RES_POR POR, CFT..RES_DIS DIS, CFT..RES_DIS DISR
						where POR.district = DIS.numero
							and DIS.numero  = @vl_numero
							and POR.numero != 67
							and POR.numero != 22
								group by DIS.nom, POR.numero
			union
				select POR.numero, 3, 72790, 79590, POR.district_recouvrement
					from CFG..RES_POR POR, CFG..RES_DIS DIS, CFG..RES_DIS DISR
						where POR.district = DIS.numero
							and DIS.numero = @vl_numero
							and POR.numero = 67
								group by DIS.nom, POR.numero
			union
				select POR.numero, 3, 79590, 80790, 25
					from CFG..RES_POR POR, CFG..RES_DIS DIS, CFG..RES_DIS DISR
						where POR.district = DIS.numero
							and DIS.numero      in (XDC_VC,XDC_DP)
							and POR.numero = 22
								group by DIS.nom, POR.numero
			end
			else 
			begin
			/*if @vl_numero = XDC_CA : Tous les sites autres que ESCOTA */    
				select POR.numero, POR.autoroute, POR.PR_debut, POR.PR_fin, POR.district_recouvrement
					from CFT..RES_POR POR, CFT..RES_DIS DIS, CFT..RES_DIS DISR
						where POR.district = DIS.numero
							and DIS.numero = @vl_numero
								group by DIS.nom, POR.numero   
			end
		end

	return XDC_OK
		
	end
#endif

return (XDC_NOK)

end

return XDC_OK

go
