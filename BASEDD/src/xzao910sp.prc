/*E*/
/* Fichier : $Id: xzao910sp.prc,v 1.1 2016/09/29 15:42:46 pc2dpdy Exp $        Release : $Revision: 1.1 $        Date : $Date: 2016/09/29 15:42:46 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzao910sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* VR		13/01/12 : Creation (DEM/1016)
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao910sp.h" */

/* static char *version = "@(#)xzao910.c	1.1 23/18/94 : xzao910" ; */

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

if exists (select * from sysobjects where name = 'XZAO910' and type = 'P')
	drop procedure XZAO910
go

create procedure XZAO910
   @va_Basedd_in             char(3),
   @va_site_in			tinyint=null
   as 

/* 
*
* SP    XZAO555SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd        va_Basedd_in
*  
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf       va_Resultat_out
*
* CODE RETOUR :
*
* 0    XDC_OK
* 1    XDC_NOK 
*
*
* FONCTION :
*
* Affichage des donnees de la table ZON_REG
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* VR    13 Jan 2012    : Creation (DEM/1016)
------------------------------------------------------*/

   
    /*A
    ** Test des parametres d'entree :
    **   Si un des parametres d'entree est egal a null alors XDC_NOK   
    */
      
    if @va_site_in=0 select    @va_site_in=null
    if @va_Basedd_in             = null 
        
		return XDC_ARG_INV
   
    else   
    begin
      
                        /*A
                        ** Lecture des donnees dans BAS_RAU
                        */

	if @va_site_in=null
	begin
	select rtrim(zone), rtrim(libelle), rtrim(etat), sit
	from   EXP..BAS_RAU
	order by sit, zone
	end
	else
	begin
	select rtrim(zone), rtrim(libelle), rtrim(etat), sit
	from   EXP..BAS_RAU
	where sit = @va_site_in
	order by sit, zone
	end
    end   

    return XDC_OK
         
go  
