/*E*/
/* Fichier : $Id: xzao562sp.prc,v 1.1 2012/07/04 11:32:17 pc2dpdy Exp $        Release : $Revision: 1.1 $        Date : $Date: 2012/07/04 11:32:17 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzao562sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* VR		16/01/12 : Creation (DEM/1016)
------------------------------------------------------*/

/* fichiers inclus */

/* static char *version = "@(#)xzao562.c	1.1 23/18/94 : xzao562" ; */

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

if exists (select * from sysobjects where name = 'XZAO562' and type = 'P')
	drop procedure XZAO562
go

	create procedure XZAO562
	   
	   as 

/* 
*
* SP    XZAO562SP
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
* Transfert du contenu de la table REG_PAR de CFT dans CFG
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* VR    16 Jan 2012    : Creation (DEM/1016)
------------------------------------------------------*/

	
   
    /*A
    ** Test des parametres d'entree :
    **   Si un des parametres d'entree est egal a null alors XDC_NOK   
    */
      
    /*if @va_Basedd_in             = null
		return XDC_ARG_INV*/
   
	begin tran
	/*A
        Suppression des enregistrements de la Table REG_PAR dans CFG */
	delete CFG..REG_PAR

	/*A
        Duplication des donnees de la table REG_PAR de CFT dans CFG */
	insert into CFG..REG_PAR
		select * from CFT..REG_PAR

	commit tran

    return XDC_OK
         
go  
