/*E*/
/*Fichier : $Id: xzae202sp.prc,v 1.3 2021/07/15 14:12:42 pc2dpdy Exp $     Release : $Revision: 1.3 $        Date :  $Date: 2021/07/15 14:12:42 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAE * FICHIER xzae202sp.prc
------------------------------------------------------
* HISTORIQUE :
*
* ABE       21/10/20	: creation SAE-DEM130 1.1
* ABE	    08/01/21	: Ajout filtrage FMC basÃ© sur xzae185 DEM-SAE130 1.2
* ABE	    15/07/21	: Ajout tempo pour le dev pb fmc pas présente dans la base à la création 1.3
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzae202sp.h" */

/* static char *version = " $Id: xzae202sp.prc,v 1.3 2021/07/15 14:12:42 pc2dpdy Exp $: xzae202" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  retourne le type de FMC et la cause pour un numero et un type
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go
				
if exists (select * from sysobjects where name = 'XZAE202' and type = 'P')
	drop procedure XZAE202
go		

create procedure XZAE202

   @va_numero_in	int	= null,
   @va_cle_in		tinyint = null,
   @va_numero_cause_out	int 	= null	output,
   @va_cle_cause_out	tinyint	= null	output,
   @va_type_out		tinyint = 0 output

as

/* Attente 3 seconde en mod DEV car la base est moins rapide */
#if defined (DEV)
	WAITFOR DELAY '00:00:00:300'											
#endif

	if @va_numero_in is null
		return XDC_ARG_INV
		
	if @va_cle_in is null
		return XDC_ARG_INV

	select 	@va_type_out=type,
		@va_numero_cause_out=cause,
		@va_cle_cause_out=cle_cause
	from EXP..FMC_GEN
	where 
		numero=@va_numero_in and
		cle=@va_cle_in
		and origine_creation not like '%ESSAI%'
		and origine_creation not like '%EXERCICE%'
		and datex = XDC_FAUX
		and type <> XZAEC_FMC_TeteBouchon
		and type <> XZAEC_FMC_PosteOperateur

   return XDC_OK
go 
