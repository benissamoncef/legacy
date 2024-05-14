/*E*/
/*  Fichier : $Id: xzao05sp.prc,v 1.1 1995/01/17 20:02:25 gaborit Exp $      Release : $Revision: 1.1 $        Date : $Date: 1995/01/17 20:02:25 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao05sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* 
*
------------------------------------------------------
* SEQUENCE D'APPEL : 
*
* SP	XZAO05SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Mot		Equipement_In
* XDY_Octet		Type_In
*
* PARAMETRES EN SORTIE :
*
* XDY_NomMachine	NomMachine_Out	
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
* gaborit	17 jan 95	: Creation
------------------------------------------------------*/


use PRC
go

create procedure XZAO;05
   @va_equipement_in	smallint=null,
   @va_type_in		tinyint=null,
   @va_machine_out	char(10)=null output
   as
   
   
   /* Definition des variables locales */
   
   declare @vl_maitre		smallint,
   	   @vl_typemaitre	tinyint
   
   /* boucle de recherche du maitre */
   while @va_equipement_in<>null and @va_type_in<>XDC_EQT_MAC
   begin
      select  @vl_maitre=maitre,@vl_typemaitre=type_maitre
      from CFG..EQT_GEN
      where numero=@va_equipement_in
        and type=@va_type_in
      if @@rowcount=1
         select @va_equipement_in=@vl_maitre,@va_type_in=@vl_typemaitre
      else
         select @va_equipement_in=null,@va_type_in=null
   end
   if @va_equipement_in<>null
      select @va_machine_out=nom
      from CFG..EQT_GEN
      where numero=@va_equipement_in
        and type=@va_type_in
   else
      select @va_machine_out=' '
   return XDC_OK
go            
