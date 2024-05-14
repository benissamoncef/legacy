/*E*/
/* Fichier : $Id: xzae566sp.prc,v 1.2 2018/06/20 14:53:09 devgfi Exp $      Release : $Revision: 1.2 $        Date : $Date: 2018/06/20 14:53:09 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae566sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* VR		13/02/12 : Creation (DEM/1016)  1.1
* JPL		20/06/18 : Suppression caracteres hors norme, genant le systeme  1.2
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzae566sp.h" */

/* static char *version = "@(#)xzae566.c	1.1 23/18/94 : xzae566" ; */

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

if exists (select * from sysobjects where name = 'XZAE566' and type = 'P')
	drop procedure XZAE566
go

	create procedure XZAE566
	   @va_Numero_in		int,
	   @va_Cle_in			tinyint
	   as 

/* 
*
* SP    XZAO553SP
*
* PARAMETRES EN ENTREE :
*
*  					va_Numero_in
* 					va_Cle_in
*
* PARAMETRES EN SORTIE :
*

* CODE RETOUR :
*
* 0    XDC_OK
* 1    XDC_NOK 
*
*
* FONCTION :
*
* Recherche des PMVA dans la zone de regulation
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* VR    13 Fev 2012    : Creation (DEM/1016)
------------------------------------------------------*/

   
    /*A
    ** Test des parametres d'entree :
    **   Si un des parametres d'entree est egal a null alors XDC_NOK   
    */

 
    if @va_Numero_in = null
	   
		return XDC_ARG_INV

    else   
    begin
        /*A
        ** Modification de l equipement
        */
		 
				Update EXP..FMC_GEN
				set fin = getdate(),
					zz = zz+1
				where numero = @va_Numero_in
				and   cle = @va_Cle_in									     
	end

    return XDC_OK
         
go  
