/*E*/
/*  Fichier : $Id: xzao11sp.prc,v 1.1 1995/01/17 14:13:41 gaborit Exp $      Release : $Revision: 1.1 $        Date : $Date: 1995/01/17 14:13:41 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao11sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* SEQUENCE D'APPEL : 
*
* SP	XZAO11SP_ListePlans
*
* PARAMETRES EN ENTREE :
*
* XDY_Octet		NumEchSortie_in (null=tous les plans)
*
* PARAMETRES EN SORTIE : (dans un select)
*
* XDY_Mot		NumPlan_out
* XDY_Nom		NomPlan_out
* XDY_Octet		NumEchSortie_out
* XDY_Octet		NumEchReinj_out
*
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
* cree un plan de secours
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* gaborit	04 jan 1995	: Creation (1.1)
------------------------------------------------------*/

 
use PRC

go

create procedure XZAO;11
   @va_EchSor_in	tinyint
   as
   
   /* modif bug applix */
   
   if @va_EchSor_in=0 select @va_EchSor_in=null
   
   select numero,
          nom,
          echangeur_sortie,
          echangeur_reinjection
   from   CFG..PLN_SCR
   where  echangeur_sortie=@va_EchSor_in
      or  @va_EchSor_in=null
   order by echangeur_sortie, echangeur_reinjection
   
   return XDC_OK
go            



