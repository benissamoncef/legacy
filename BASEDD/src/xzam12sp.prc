/*E*/
/*X Fichier : $Id: xzam12sp.prc,v 1.2 2018/06/27 15:52:37 devgfi Exp $        $Revision: 1.2 $        $Date: 2018/06/27 15:52:37 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAM * FICHIER xzam12sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Interface Alarmes >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	03 Nov 1994	: Creation
* JPL	27/06/18 : Ajout directive 'drop procedure'  1.2
------------------------------------------------------*/

/* fichiers inclus */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*  retourne le nb d alarmes critiques en cours
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

if exists (select 1 from sysobjects where name = 'XZAM12' and type = 'P')
	drop procedure XZAM12
go


create procedure XZAM12
   @va_nombre_out		int output
   as 

/* 
*
* SP	XZAM01SP
*
* PARAMETRES EN ENTREE :
*
* PARAMETRES EN SORTIE :
*
* VALEUR RENDUE :
*
* Cree une nouvelle fiche alarme, ou met a jour celle existant deja
*
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK
* 6	XDC_ACT_REFUSEE : pas de creation sur equipement HS
*
* CONDITIONS D'UTILISATION :
*
* FONCTION :
*
* MECANISMES :
*
------------------------------------------------------*/
   
			select @va_nombre_out = count(*) from CFG..TYP_ALM, EXP..ALM_EQT
			where CFG..TYP_ALM.numero = EXP..ALM_EQT.type
			and etat = XDC_VRAI
			and critique = XDC_VRAI
 return (XDC_OK)

go

