/*E*/
/* Fichier : $Id: xzao4271sp.prc,v 1.1 2012/07/04 11:29:36 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2012/07/04 11:29:36 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao4271sp.prc
------------------------------------------------------
* HISTORIQUE :
*
* VR	03/01/12	creation (DEM/1016)
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao4271sp.h" */

/* static char *version = " @(#)xzao4271sp.prc	1.2 1.2 02/14/11: xzao4271" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Liste des zones d'influence PRV
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAO4271' and type = 'P')
	drop procedure XZAO4271
go


create procedure XZAO4271
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
			numero_prv,
			autoroute_prv,
			sens_prv,
			PR_prv,
			site_prv,
			autoroute_zone,
			sens_zone,
			PR_debut_zone,
			PR_fin_zone,
			upper(direction_zone),
			type_zone,
                        distance
		from CFG..ZON_PRV
		where inhibition = 0
		order by autoroute_prv, sens_prv, PR_prv
	end

#ifdef CI
	else
	begin
		select 	numero_zone,
			numero_prv,
                        autoroute_prv,
                        sens_prv,
                        PR_prv,
			site_prv,
                        autoroute_zone,
                        sens_zone,
                        PR_debut_zone,
                        PR_fin_zone,
                        upper(direction_zone),
                        type_zone,
                        distance
                from CFT..ZON_PRV
		where inhibition = 0
                order by autoroute_prv, sens_prv, PR_prv
	
	end
#endif
   end

   return XDC_OK
go 
	 
