/*E*/
/*  Fichier : $Id: xzao454sp.prc,v 1.2 2018/10/11 14:19:09 devgfi Exp $      Release : $Revision: 1.2 $        Date : $Date: 2018/10/11 14:19:09 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao454sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* 
*
------------------------------------------------------
* SEQUENCE D'APPEL : 
*
* SP	XZAO454SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Mot		Equipement_In
*
* PARAMETRES EN SORTIE :
*
* XDY_Octet	type_out	
*
* VALEUR RENDUE :
*
* Recherche le type de BAF
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
* gaborit	17 jan 95	: Creation
------------------------------------------------------*/


use PRC
go

if exists (select 1 from sysobjects where name = 'XZAO454' and type = 'P')
	drop procedure XZAO454
go


create procedure XZAO454
   @va_equipement_in	smallint=null,
   @va_type_out		tinyint=null output
   as
   
      select  @va_type_out=type
      from CFG..EQT_BAF
      where numero=@va_equipement_in
   return XDC_OK
go            
