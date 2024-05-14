/*E*/
/*Fichier : $Id: xzao4270sp.prc,v 1.2 2011/02/14 14:53:13 gesconf Exp $     Release : $Revision: 1.2 $        Date : $Date: 2011/02/14 14:53:13 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao4270sp.prc
------------------------------------------------------
* HISTORIQUE :
*
* guilhou       16/06/05       : creation 1.1
* JPL		14/02/11 : Ajout de la colonne "distance" (DEM 953)  1.2
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao4270sp.h" */

/* static char *version = " $Id: xzao4270sp.prc,v 1.2 2011/02/14 14:53:13 gesconf Exp $ $Revision: 1.2 $ $Date: 2011/02/14 14:53:13 $: xzao4270" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Liste des zones d'influence PMVA
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAO4270' and type = 'P')
	drop procedure XZAO4270
go


create procedure XZAO4270
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
			numero_pmva,
			autoroute_pmva,
			sens_pmva,
			PR_pmva,
			site_pmva,
			autoroute_zone,
			sens_zone,
			PR_debut_zone,
			PR_fin_zone,
			upper(direction_zone),
			type_zone,
                        distance
		from CFG..ZON_PMA
		where inhibition = 0
		order by autoroute_pmva, sens_pmva, PR_pmva
	end

#ifdef CI
	else
	begin
		select 	numero_zone,
			numero_pmva,
                        autoroute_pmva,
                        sens_pmva,
                        PR_pmva,
			site_pmva,
                        autoroute_zone,
                        sens_zone,
                        PR_debut_zone,
                        PR_fin_zone,
                        upper(direction_zone),
                        type_zone,
                        distance
                from CFT..ZON_PMA
		where inhibition = 0
                order by autoroute_pmva, sens_pmva, PR_pmva
	
	end
#endif
   end

   return XDC_OK
go 
	 
