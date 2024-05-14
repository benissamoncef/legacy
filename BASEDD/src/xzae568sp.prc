/*E*/
/*  Fichier : @(#)xzae568sp.prc	1.14      Release : 1.14        Date : 13/02/12
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae568sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* VR		13/02/12 : Cr�ation (DEM/1016)
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzae568sp.h" */

/* static char *version = "@(#)xzae568.c	1.1 23/18/94 : xzae568" ; */

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

if exists (select * from sysobjects where name = 'XZAE568' and type = 'P')
	drop procedure XZAE568
go

	create procedure XZAE568
	   @va_numero_saga_in		char(10)
	   as 

/* 
*
* SP    XZAO553SP
*
* PARAMETRES EN ENTREE :
*
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
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* VR    13 Fev 2012    : Creation (DEM/1016)
------------------------------------------------------*/
 
	
    select GEN.numero, GEN.cle
    from EXP..FMC_GEN GEN
    where rtrim(GEN.com_fmc_init) = rtrim(@va_numero_saga_in) /*and poste_enrichisseur is not null*/
    order by GEN.type
    return XDC_OK
         
go  
