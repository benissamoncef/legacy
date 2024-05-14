/*E*/
/* Fichier : $Id: xzao480sp.prc,v 1.1 2013/01/25 10:00:38 pc2dpdy Exp $        Release : $Revision: 1.1 $        Date : $Date: 2013/01/25 10:00:38 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao480sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
* VR   04/06/12      : Creation (DEM/1014 PMA)
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao480sp.h" */

/* static char *version = "@(#)xzao480sp.prc	1.0 1.0 08/11/11: xzao480" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAO480' and type = 'P')
	drop procedure XZAO480
go	

create procedure XZAO480
   @va_numero_PMA_in smallint,
   @va_Basedd_in     char(3),
   @va_num_dest_un_proche_out smallint output,
   @va_num_dest_un_loin_out smallint output,
   @va_num_dest_deux_proche_out smallint output,
   @va_num_dest_deux_loin_out smallint output,
   
   @va_nom_dest_un_proche_out varchar(10) output,
   @va_nom_dest_un_loin_out varchar(10) output,
   @va_nom_dest_deux_proche_out varchar(10) output,
   @va_nom_dest_deux_loin_out varchar(10) output
   
   as 

/* 
*
* SP	XZAO480SP
*
* PARAMETRES EN ENTREE :
*
* smallint va_numero_PMA_in		
* XDY_Entier va_Basedd_in	
* PARAMETRES EN SORTIE :
* 
* va_num_dest_un_proche_out
* va_num_dest_un_loin_out
* va_num_dest_deux_proche_out,
* va_num_dest_deux_loin_out
* va_nom_dest_un_proche_out
* va_nom_dest_un_loin_out
* va_nom_dest_deux_proche_out
* va_nom_dest_deux_loin_out
*
* VALEUR RENDUE :
*
* Retourne la liste des destination ... du PMA
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
* VR   04/06/12      : Creation (DEM/PMA)
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

			select @va_num_dest_un_proche_out = numero_dest from CFG..DTP_PMA 
			where numero_PMA = @va_numero_PMA_in
			and type_dest = 1
			if (@@rowcount < 1)
			
			select @va_num_dest_un_loin_out = numero_dest from CFG..DTP_PMA 
			where numero_PMA = @va_numero_PMA_in
			and type_dest = 2
			if (@@rowcount < 1)
			
			select @va_num_dest_deux_proche_out = numero_dest from CFG..DTP_PMA 
			where numero_PMA = @va_numero_PMA_in
			and type_dest = 3
			if (@@rowcount < 1)
			
			select @va_num_dest_deux_loin_out = numero_dest from CFG..DTP_PMA 
			where numero_PMA = @va_numero_PMA_in
			and type_dest = 4
			if (@@rowcount < 1)
			
			/*A
			**  Recuperer le nom des 4 destinations dans les parametres
			*/	

				select @va_nom_dest_un_proche_out = libelle_PMV from CFG..RES_DTP
				where numero = @va_num_dest_un_proche_out
				
				select @va_nom_dest_un_loin_out = libelle_PMV from CFG..RES_DTP
				where numero = @va_num_dest_un_loin_out
				
				select @va_nom_dest_deux_proche_out = libelle_PMV from CFG..RES_DTP
				where numero = @va_num_dest_deux_proche_out
				
				select @va_nom_dest_deux_loin_out = libelle_PMV from CFG..RES_DTP
				where numero = @va_num_dest_deux_loin_out

	end
	return XDC_OK		
go  
