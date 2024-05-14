/*E*/
/* Fichier : $Id: xzao565sp.prc,v 1.1 2012/07/04 11:32:53 pc2dpdy Exp $        Release : $Revision: 1.1 $        Date : $Date: 2012/07/04 11:32:53 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzao565sp.prc
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

/* static char *version = "@(#)xzao565.c	1.1 23/18/94 : xzao565" ; */

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

if exists (select * from sysobjects where name = 'XZAO565' and type = 'P')
	drop procedure XZAO565
go

	create procedure XZAO565

	   /*@va_Basedd_in         	     char(3),
	   @va_Resultat_out		int output*/
	   
	   as 

/* 
*
* SP    XZAO565SP
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
* Transfert du contenu de la table REG_COM de CFT dans CFG
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* VR    16 Jan 2012    : Creation (DEM/1016)
------------------------------------------------------*/
      

	begin tran
	/*A
        Suppression des enregistrements de la Table REG_COM dans CFG */
	delete CFG..REG_COM
	
	/*A
        Duplication des donnees de la table REG_COM de CFT dans CFG */
	insert into CFG..REG_COM
		select * from CFT..REG_COM

	commit tran

    return XDC_OK
         
go  

