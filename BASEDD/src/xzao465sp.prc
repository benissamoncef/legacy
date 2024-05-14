/*E*/
/* Fichier : $Id: xzao465sp.prc,v 1.2 2017/02/28 11:16:11 devgfi Exp $        Release : $Revision: 1.2 $        Date : $Date: 2017/02/28 11:16:11 $
--------------------------------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
--------------------------------------------------------------------------------
* SOUS-SYSTEME  BASEDD
--------------------------------------------------------------------------------
* MODULE XZAO * FICHIER xzao465sp.prc
--------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
* VR   13/08/12        : creation (DEM/1014 PMA)
* JPL	06/02/17 : Libelles sans espaces de fin mais non tronques (DEM 1215)  1.2
------------------------------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao465sp.h" */

/* static char *version = "@(#)xzao460sp.prc	1.0 1.0 08/11/11: xzao465" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X-----------------------------------------------------------------------------
* SERVICE RENDU : 
*
*  Retourne les numeros et libelles de destinations Temps de Parcours
*  configurees pour un PMVA donne.
*
--------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

if exists (select 1 from sysobjects where name = 'XZAO465' and type = 'P')
	drop procedure XZAO465
go


create procedure XZAO465
   @va_numero_PMA_in smallint,
   @va_Basedd_in     char(3),  
   @va_num_dest_proche_un_out smallint output,
   @va_num_dest_loin_un_out smallint output,
   @va_num_dest_proche_deux_out smallint output,
   @va_num_dest_loin_deux_out smallint output,  
   @va_nom_dest_proche_un_out varchar(15) output,
   @va_nom_dest_loin_un_out varchar(15) output,
   @va_nom_dest_proche_deux_out varchar(15) output,
   @va_nom_dest_loin_deux_out varchar(15) output
   
   as 

/* 
*
* SP	XZAO460SP
*
* PARAMETRES EN ENTREE :
*
* smallint va_numero_PMV_in		
* XDY_Entier va_Basedd_in	
* PARAMETRES EN SORTIE :
*
* 
* VALEUR RENDUE :
*
* Retourne la liste des destination ... du PMVA
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
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* VR	08/11/11	creation (DEM/1014)
------------------------------------------------------*/

    /*A
    ** Test des parametres d'entree :
    **   Si un des parametres d'entree est egal a null alors XDC_ARG_INV 
    */
      
   if @va_Basedd_in = null 
      return XDC_ARG_INV
   
   else   
   
   begin
   
	/*A
    **  Travail dans la base CFG
    */   
   
		if @va_Basedd_in = XDC_BASE_CFG
		begin  
	  
	/*A
    **  Recuperer le numero et le type des 4 destinations dans les parametres
    */
		
			select @va_num_dest_proche_un_out = numero_dest from CFG..DTP_PMA
			where numero_PMA = @va_numero_PMA_in
			and type_dest = 1
			
			select @va_num_dest_loin_un_out = numero_dest from CFG..DTP_PMA
			where numero_PMA = @va_numero_PMA_in
			and type_dest = 2
			
			select @va_num_dest_proche_deux_out = numero_dest from CFG..DTP_PMA
			where numero_PMA = @va_numero_PMA_in
			and type_dest = 4
			
			select @va_num_dest_loin_deux_out = numero_dest from CFG..DTP_PMA
			where numero_PMA = @va_numero_PMA_in
			and type_dest = 5
			
			/*A
			**  Recuperer le nom des 3 destinations dans les parametres
			*/	
				select @va_nom_dest_proche_un_out = rtrim (libelle_PMV) from CFG..RES_DTP
				where numero = @va_num_dest_proche_un_out
						
				select @va_nom_dest_loin_un_out = rtrim (libelle_PMV) from CFG..RES_DTP
				where numero = @va_num_dest_loin_un_out
				
				select @va_nom_dest_proche_deux_out = rtrim (libelle_PMV) from CFG..RES_DTP
				where numero = @va_num_dest_proche_deux_out
				
				select @va_nom_dest_loin_deux_out = rtrim (libelle_PMV) from CFG..RES_DTP
				where numero = @va_num_dest_loin_deux_out
		end
	end
	return XDC_OK		
go  
