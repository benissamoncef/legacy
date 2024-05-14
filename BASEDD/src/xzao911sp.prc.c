#include "/users/gesconf/migrazur/XDMICG/inc/xdc.h"
#include "/users/gesconf/migrazur/XDMICG/inc/xzaec.h"
#include "/users/gesconf/migrazur/XDMICG/inc/xzaac.h"
#include "/users/gesconf/migrazur/XDMICG/inc/xzacc.h"
#include "/users/gesconf/migrazur/XDMICG/inc/xzahc.h"
#include "/users/gesconf/migrazur/XDMICG/inc/xzaoc.h"
/*E*/
/* Fichier : $Id: xzao911sp.prc,v 1.4 2021/04/12 06:06:56 gesconf Exp $        Release : $Revision: 1.4 $        Date : $Date: 2021/04/12 06:06:56 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzao911sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* VR		12/01/12 : Creation
* JPL		20/04/17 : Ajout trace de l'etat du controle de la zone RAU en base HIStorique (DEM 1225)  1.2
* PNI		12/04/21 : Supression appel SQL_HIS 1.3 SAE-263
-----------------------------------------------------*/ 

/* fichiers inclus */

/* #include "xzao911sp.h" */

/* static char *version = "$Id: xzao911sp.prc,v 1.4 2021/04/12 06:06:56 gesconf Exp $" ; */

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


if exists (select 1 from sysobjects where name = 'XZAO911' and type = 'P')
	drop procedure XZAO911
go

	create procedure XZAO911
	   @va_Zone_in		char(30),
	   @va_Etat_in		char(5)
	   as 

/* 
*
* SP    XZAO911SP
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

	   declare	@vl_libelle_zone	char(30)

    /*A
    ** Test des parametres d'entree :
    **   Si un des parametres d'entree est egal a null alors XDC_NOK   
    */
    if @va_Zone_in = null
	or @va_Etat_in = null
		return XDC_ARG_INV
    
		
	else 
       begin
    
                update EXP..BAS_RAU
                set    etat = @va_Etat_in
                where  zone = @va_Zone_in

                /*A
                ** Si pas d'enregistrement
                */

				if @@rowcount <> 1
				begin
					return XDC_NOK
				end
    end         


    /*B Tracer le basculement du controle de la zone RAU en base HIStorique */
    select @vl_libelle_zone = substring (libelle, 1, 30)  from EXP..BAS_RAU  where zone = @va_Zone_in
    exec PRC..XZAO913  @vl_libelle_zone, @va_Etat_in

    return XDC_OK
         
go  
