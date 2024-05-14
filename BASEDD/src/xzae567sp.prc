/*E*/
/*  Fichier : @(#)xzae567sp.prc	1.14      Release : 1.14        Date : 13/02/12
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae567sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* VR		13/02/12 : Creation (DEM/1016)
* JPL		20/06/18 : Suppression caracteres hors norme, genant le systeme  1.2
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzae567sp.h" */

/* static char *version = "@(#)xzae567.c	1.1 23/18/94 : xzae567" ; */

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

if exists (select * from sysobjects where name = 'XZAE567' and type = 'P')
	drop procedure XZAE567
go

	create procedure XZAE567
	   @va_zone_in		smallint
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
* Recherche des PMVA dans la zone de regulation
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* VR    13 Fev 2012    : Creation (DEM/1016)
------------------------------------------------------*/
 
	
    select GEN.numero, GEN.cle
    from EXP..FMC_GEN GEN, EXP..FMC_REG REG
    where GEN.numero = REG.numero
    and GEN.cle = REG.cle
    and REG.zone = @va_zone_in
    and GEN.fin is null
    and REG.sit = GEN.sit
    group by GEN.numero, GEN.cle
    order by GEN.numero desc
    return XDC_OK
         
go  
