/*E*/
/*Fichier : $Id: xzao413sp.prc,v 1.4 2000/12/11 14:44:46 gesconf Exp $     Release : $Revision: 1.4 $        Date :  $Date: 2000/12/11 14:44:46 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao413sp.prc
------------------------------------------------------
* HISTORIQUE :
*
* guilhou       10/12/97        : creation vide 1.1
* guilhou	10/12/97	: remplissage (dem/1534) 1.2
* Guilhou	20/02/98	: direction en majuscules 1.3
* Guilhou	20/09/00	: modif structure ZON_PMV 1.4
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao413sp.h" */

/* static char *version = " $Id: xzao413sp.prc,v 1.4 2000/12/11 14:44:46 gesconf Exp $: xzao413" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Liste des zones d'influence PMV
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAO413' and type = 'P')
	drop procedure XZAO413
go

create procedure XZAO413
   @va_Basedd_in                char(3)
   as 

   if @va_Basedd_in = null
      return XDC_ARG_INV

   else

   begin
      	/*A
      	**  Travail dans la base CFG
      	*/

      	if @va_Basedd_in = XDC_BASE_CFG
      	begin

		select 	numero_zone,
			numero_pmv,
			autoroute_pmv,
			sens_pmv,
			PR_pmv,
			site_pmv,
			autoroute_zone,
			sens_zone,
			PR_debut_zone,
			PR_fin_zone,
			upper(direction_zone),
			PR_1_pmv_amont,
			sens_1_pmv_amont,
			autoroute_1_pmv_amont,
			distance_1_pmv_amont,
			PR_1_pmv_amont_bis,
			sens_1_pmv_amont_bis,
			autoroute_1_pmv_amont_bis,
			distance_1_pmv_amont_bis,
			PR_1_pmv_amont_ter,
			sens_1_pmv_amont_ter,
			autoroute_1_pmv_amont_ter,
			distance_1_pmv_amont_ter
		from CFG..ZON_PMV
		where inhibition = 0
		order by autoroute_pmv, sens_pmv, PR_pmv
	end

#ifdef CI
	else
	begin
		select 	numero_zone,
			numero_pmv,
                        autoroute_pmv,
                        sens_pmv,
                        PR_pmv,
			site_pmv,
                        autoroute_zone,
                        sens_zone,
                        PR_debut_zone,
                        PR_fin_zone,
                        upper(direction_zone),
			PR_1_pmv_amont,
			sens_1_pmv_amont,
			autoroute_1_pmv_amont,
			distance_1_pmv_amont,
			PR_1_pmv_amont_bis,
			sens_1_pmv_amont_bis,
			autoroute_1_pmv_amont_bis,
			distance_1_pmv_amont_bis,
			PR_1_pmv_amont_ter,
			sens_1_pmv_amont_ter,
			autoroute_1_pmv_amont_ter,
			distance_1_pmv_amont_ter
                from CFT..ZON_PMV
		where inhibition = 0
                order by autoroute_pmv, sens_pmv, PR_pmv
	
	end
#endif
   end

   return XDC_OK
go 
	 
