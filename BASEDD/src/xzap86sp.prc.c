#include "/users/gesconf/migrazur/XDMICG/inc/xdc.h"
#include "/users/gesconf/migrazur/XDMICG/inc/xzaec.h"
#include "/users/gesconf/migrazur/XDMICG/inc/xzaac.h"
#include "/users/gesconf/migrazur/XDMICG/inc/xzacc.h"
#include "/users/gesconf/migrazur/XDMICG/inc/xzahc.h"
								/*E*/
/* Fichier : $Id: xzap86sp.prc,v 1.1 2021/06/03 07:11:43 pc2dpdy Exp $        Release : $Revision: 1.1 $        Date : $Date: 2021/06/03 07:11:43 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAP * FICHIER xzap86sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Plan d'Actions >
*
------------------------------------------------------
* HISTORIQUE :
*
* ABE	07/04/21	: Creatin PAU SONO DEM-SAE244
------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'xzap86' and type = 'P')
	drop procedure XZAP86
go


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Lit une proposition d'affichage PAU d'un plan d'actions
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

create procedure XZAP86

	@va_NumPA_in		smallint,
	@va_Ordre_in		smallint							
	
/*
* ARGUMENTS EN ENTREE :
*
* XDY_Mot 		va_NumPA_in
* XDY_Octet 		va_Ordre_in
								
*
* ARGUMENTS EN SORTIE :
* 			va_District_in
* 			va_Remarque_in
* XDY_Entier		va_PrioriteEvt_in
* XDY_Eqt		va_NoEqt_in
* 			va_Explication_in
* XDY_MessageSONO	va_Message_in
* XDY_Autoroute		va_Autoroute_in
* XDY_Sens		va_Sens_in
* XDY_Entier		va_PR_Debut_in
* XDY_Entier		va_PR_Fin_in
* 			va_Realisable_in
*
* CODE RETOUR : 
*
*  XDC_OK 
*  XDC_NOK
*
* CONDITION D'UTILISATION :
*
* FONCTION :
*
* Lit une proposition d'affichage PAU SONO d'un plan d'actions
*
* MECANISMES :
*
------------------------------------------------------*/


as 
  
#ifdef HIST
declare @vl_Status int

	exec @vl_Status = SQL_CI.PRC..XZAP86
	@va_NumPA_in,
	@va_Ordre_in

        if @vl_Status = null
                return XDC_PRC_INC
        else return @vl_Status
#else
   /*A
   ** Si une des variables d'entree est nulle alors retourne XDC_ARG_INV
   */
   declare 	@vl_priorite tinyint,
   		@vl_distance int,
   		@vl_evt int,
   		@vl_cle tinyint,
   		@vl_eqt smallint
   
   if @va_NumPA_in = null
   or @va_Ordre_in = null
  
      return XDC_ARG_INV
  
   else
   
   /*A
   ** Si les variables d'entree sont non nulles
   ** Lecture d'une Proposition PAU d'un plan d'action
   */
   
   begin
      																				
      select numero,
		ordre,
		district,
		equipement,
		priorite,
		remarque,
		explication,
		realisable,
		message,
		autoroute,
		sens,
		PR_deb,
		PR_fin,
		libelle	   	
      from EXP..SAD_SONO
   
        where numero = @va_NumPA_in and ordre = @va_Ordre_in 
   	
      return XDC_OK
      
   end   
         
#endif
go
