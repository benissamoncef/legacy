/*E*/
/* Fichier : $Id: xzao566sp.prc,v 1.1 2012/07/04 11:33:02 pc2dpdy Exp $        Release : $Revision: 1.1 $        Date : $Date: 2012/07/04 11:33:02 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzao566sp.prc
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

/* static char *version = "@(#)xzao566.c	1.1 23/18/94 : xzao566" ; */

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

if exists (select * from sysobjects where name = 'XZAO566' and type = 'P')
	drop procedure XZAO566
go

	create procedure XZAO566

	   
	   as 

/* 
*
* SP    XZAO562SP
*
* PARAMETRES EN ENTREE :
*
* 
*
* PARAMETRES EN SORTIE :
*
* 
* CODE RETOUR :
*
* 0    XDC_OK
* 1    XDC_NOK 
*
*
* FONCTION :
*
* Transfert du contenu de la table REG_SCE de CFT dans CFG
* Transfert du contenu de la table REG_SCE_CMD de CFT dans CFG
* Transfert du contenu de la table REG_CON de CFT dans CFG
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
        Suppression des enregistrements des Tables REG_SCE_CMD 			REG_SCE REG_CON et REG_ASS dans CFG */
	delete from CFG..REG_SCE_CMD
	delete from CFG..REG_SCE
	delete from CFG..REG_CON
	delete from CFG..REG_ASS

	/*A
        Duplication des donnees des tables REG_SCE_CMD REG_SCE 			REG_CON et REG_ASS de CFT dans CFG */
	insert into CFG..REG_SCE_CMD
		select * from CFT..REG_SCE_CMD

	insert into CFG..REG_SCE
		select * from CFT..REG_SCE

	insert into CFG..REG_CON
		select * from CFT..REG_CON

	insert into CFG..REG_ASS
		select * from CFT..REG_ASS

	commit tran

    return XDC_OK
         
go  
