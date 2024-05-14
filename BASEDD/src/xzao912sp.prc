/*E*/
/* Fichier : $Id: xzao912sp.prc,v 1.2 2018/10/11 13:52:53 devgfi Exp $        Release : $Revision: 1.2 $        Date : $Date: 2018/10/11 13:52:53 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzao912sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* VR		12/01/12 : Creation
-----------------------------------------------------*/ 

/* fichiers inclus */

/* #include "xzao912sp.h" */

/* static char *version = "$Id: xzao912sp.prc,v 1.2 2018/10/11 13:52:53 devgfi Exp $" ; */

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


if exists (select 1 from sysobjects where name = 'XZAO912' and type = 'P')
	drop procedure XZAO912
go

	create procedure XZAO912
	   @va_autoroute_in	tinyint,
	   @va_pr_in		T_PR,
	   @va_filtrage_out	bit output
	   as 

/* 
*
* SP    XZAO912SP
*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf       va_Resultat_out    

* CODE RETOUR :
*
* 0    XDC_OK
* 1    XDC_NOK 
*
*
* FONCTION :
*
* Mise a jour des données dans la table ZON_REG
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* VR    12 Jan 2012    : Creation (DEM/1016)
------------------------------------------------------*/

    /*A
    ** Test des parametres d'entree :
    **   Si un des parametres d'entree est egal a null alors XDC_NOK   
    */
    if @va_autoroute_in = null
	or @va_pr_in = null
		begin
		return XDC_ARG_INV
   end

   if exists (select * from EXP..BAS_RAU e, CFG..ZON_RAU c
   where  e.zone = c.zone
   and e.etat = "CI"
   and c.autoroute = @va_autoroute_in
   and c.debut <= @va_pr_in 
   and c.fin >= @va_pr_in )
   begin
   	select @va_filtrage_out = XDC_NON
   end
   else
   begin
   	select @va_filtrage_out = XDC_OUI
   end

    return XDC_OK
         
go  
