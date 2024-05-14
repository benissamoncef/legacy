/*E*/
/*  Fichier : $Id: xzao00sp.prc,v 1.1 1995/01/17 20:02:25 claudel Exp $      Release : $Revision: 1.1 $        Date : $Date: 2023/10/03 20:02:25 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao00sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* 
*
------------------------------------------------------
* SEQUENCE D'APPEL : 
*
* SP	XZAO00SP
*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
* XDY_Mot       va_site_out
* XDY_NomSite	va_nom_site_out	
*
* VALEUR RENDUE :
*
* Recherche la machine qui pilote un equipement
* 
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK 
*
* CONDITIONS D'UTILISATION :
*
* FONCTION :
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* LCL	03/10/23	: Creation
------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAO00' and type = 'P')
	drop procedure XZAO00
go

create procedure XZAO00
    @va_site_out		smallint	output,
    @va_nom_site_out	char(3)		output
as

    /* recherche du site principal */
    select @va_site_out = numero, @va_nom_site_out = code 
	    from CFG..RES_DIS
    		where type = XDC_TYPEM_SD

    if @@rowcount = 1
        return XDC_OK
    else
        return XDC_NOK

go            
