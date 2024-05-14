/*E*/
/*Fichier : @(#)xzao460sp.prc	1.0     Release : 1.0       Date : 08/11/11
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao460sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
* VR   08/11/11        : ajout destination (DEM/1014)
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao460sp.h" */

/* static char *version = "@(#)xzao460sp.prc	1.0 1.0 08/11/11: xzao460" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

drop procedure XZAO460
go

create procedure XZAO460
   @va_numero_PMV_in smallint,
   @va_Basedd_in     char(3),
   @va_num_dest_proche_out smallint output,
   @va_num_dest_lointaine_out smallint output,
   @va_num_dest_noeud_out smallint output,
   
   @va_nom_dest_proche_out varchar(10) output,
   @va_nom_dest_lointaine_out varchar(10) output,
   @va_nom_dest_noeud_out varchar(10) output
   
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
* smallint va_num_dest_proche_out
* varchar(10) va_nom_dest_proche_out
* smallint va_num_dest_lointaine_out
* varchar(10) va_nom_dest_lointaine_out
* smallint va_num_dest_noeud_out
* varchar(10) va_nom_dest_noeud_out
* 
* VALEUR RENDUE :
*
* Retourne la liste des destination ... du PMV
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
* VR	08/11/11	creation
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
    **  Récupérer le numero et le type des 3 destinations dans les paramètres
    */
		
			select @va_num_dest_proche_out = numero_dest from CFG..DTP_PMV 
			where numero_PMV = @va_numero_PMV_in
			and type_dest = 1
			if (@@rowcount < 1)
			
			select @va_num_dest_lointaine_out = numero_dest from CFG..DTP_PMV 
			where numero_PMV = @va_numero_PMV_in
			and type_dest = 2
			if (@@rowcount < 1)
			
			select @va_num_dest_noeud_out = numero_dest from CFG..DTP_PMV 
			where numero_PMV = @va_numero_PMV_in
			and type_dest = 3
			if (@@rowcount < 1)
			
			/*A
			**  Récupérer le nom des 3 destinations dans les paramètres
			*/	
				select @va_nom_dest_proche_out = libelle_PMV from CFG..RES_DTP
				where numero = @va_num_dest_proche_out
				
				
				select @va_nom_dest_lointaine_out = libelle_PMV from CFG..RES_DTP
				where numero = @va_num_dest_lointaine_out
				
				select @va_nom_dest_noeud_out = libelle_PMV from CFG..RES_DTP
				where numero = @va_num_dest_noeud_out
		end
	end
	return XDC_OK		
go  
