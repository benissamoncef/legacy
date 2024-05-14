/*E*/
/*Fichier : @(#)xzai502sp.prc	1.4      Release : 1.4        Date : 03/19/99
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAI * FICHIER xzai502sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Interface Info TFM >
*
------------------------------------------------------
* HISTORIQUE :
*
* F. lizot      12 Fev 2004     : Creation
* F.Lizot  	27/06/2005 	:  Correction SAGA DEM473 1.2
------------------------------------------------------*/


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*  Acquittement d'un message TFM de la table TFM_DTX (message TFM DATEX sans FMC)
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAI502' and type = 'P')
	drop procedure XZAI502
go

create procedure XZAI502
   @va_code_partenaire_in   char(10) = null,
   @va_reference_datex_in   char(50) = null,
   @va_Horodate_in          datetime = null
   as 

/* 
*
* SP	XZAI502SP
*
* PARAMETRES EN ENTREE :
*
* va_code_partenaire_in   code du partenaire qui a emis le message
* va_reference_datex_in   reference datex associee
* va_Horodate_in          heure de diffusion
*
* PARAMETRES EN SORTIE :
*
* VALEUR RENDUE :
*
* Incremente le nombre de diffusions du message
* Met a jour l'heure de diffusion
* Positionne supprime a 1
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
------------------------------------------------------*/

   if exists ( select * from EXP..TFM_DTX where code_partenaire=@va_code_partenaire_in and reference_datex= @va_reference_datex_in)
   begin
      update EXP..TFM_DTX 
      set nombre_diffusions=nombre_diffusions+1, 
          heure_diffusion=@va_Horodate_in,
          supprime=1
      where code_partenaire=@va_code_partenaire_in and reference_datex= @va_reference_datex_in
   end
   else
      return XDC_NOK
   
   return ( XDC_OK )
         
go  
