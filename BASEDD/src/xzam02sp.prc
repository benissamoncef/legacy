/*E*/
/*Fichier : $Id: xzam02sp.prc,v 1.2 2009/06/25 14:55:36 gesconf Exp $      Release : $Revision: 1.2 $        Date : $Date: 2009/06/25 14:55:36 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAM * FICHIER xzam02sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Interface Alarmes >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	04 Nov 1994	: Creation
* niepceron	24 Jun 2009	: type alarme tinyint -> smallint v1.2
------------------------------------------------------*/

/* fichiers inclus */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*   Lit une fiche alarme en base
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAM;02
   @va_TypeAlrm_in			smallint = null,
   @va_TypeEqt_in			tinyint = null,
   @va_Eqt_in				smallint = null,
   @va_HorodatePremApparition_out	datetime = null output,
   @va_HorodatePreDisparition_out	datetime = null output,
   @va_HorodateDernierChgt_out		datetime = null output,
   @va_NombreDApparitions_out		smallint = null output,
   @va_Etat_out				bit output,
   @va_Commentaitre_out			char(50) = ' ' output,
   @va_DemandeTravaux_out		int = null output,
   @va_NombreTotalApparitions_out	int = null output
   as 

/* 
*
* SP	XZAM02SP
*
* PARAMETRES EN ENTREE :
*
* XDY_TypeAlarme	va_NumFicheAlarme_in
* XDY_TypeEqt		va_TypeEqt_in
* XDY_Eqt		va_Eqt_in
*
* PARAMETRES EN SORTIE :
*
* XDY_Horodate		va_HorodatePremApparition_out
* XDY_Horodate		va_HorodatePreDisparition_out
* XDY_Horodate		va_HorodateDernierChgt_out
* XDY_Mot		va_NombreDApparitions_out
* XDY_Booleen		va_Etat_out
* XDY_Phrase		va_Commentaitre_out
* XDY_DT		va_DemandeTravaux_out
* XDY_Entier		va_NombreTotalApparitions_out
*
* VALEUR RENDUE :
*
* Lit une fiche alarme en base
*
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK
*  NumFicheAlarme inconnu
*
* CONDITIONS D'UTILISATION :
*
* FONCTION :
*
* select dans ALM_EQT, ALM_NBR
*
* MECANISMES :
*
------------------------------------------------------*/
declare
   
   /*A
   ** Declaration des variables locales
   */
   
   @vl_Alrm		smallint
   
     
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_TypeAlrm_in = null
   or @va_TypeEqt_in = null
   or @va_Eqt_in = null
      
      return ( XDC_ARG_INV )
   
   else   
   
   begin
   
   /*A
   ** Si les parametres sont non nuls
   */
         
      
      select @vl_Alrm = nombre_d_apparitions from EXP..ALM_EQT
        where type 		= @va_TypeAlrm_in
          and type_equipement 	= @va_TypeEqt_in
          and equipement	= @va_Eqt_in
          
      /*A
      ** La fiche alarme existe
      ** Recherche de la fiche alarme
      ** Affectation des parametres de sorties
      */
      
      if @vl_Alrm != null
      begin
      
         select @va_HorodatePremApparition_out 	= ALM.date_1er_apparition,
         	@va_HorodatePreDisparition_out	= ALM.date_1er_disparition,
         	@va_HorodateDernierChgt_out	= ALM.date_dernier_changement,
         	@va_NombreDApparitions_out	= ALM.nombre_d_apparitions,
         	@va_Etat_out			= ALM.etat,
         	@va_Commentaitre_out		= ALM.commentaire,
         	@va_DemandeTravaux_out		= ALM.demande_de_travaux,
         	@va_NombreTotalApparitions_out  = NBR.nombre_d_apparitions
         
         from  EXP..ALM_EQT ALM, EXP..ALM_NBR NBR
         
         where ALM.type 		= @va_TypeAlrm_in
           and ALM.type_equipement 	= @va_TypeEqt_in
           and ALM.equipement		= @va_Eqt_in
           and NBR.type 		= @va_TypeAlrm_in
           and NBR.type_equipement 	= @va_TypeEqt_in
           and NBR.equipement		= @va_Eqt_in
      end
          
      else
      
      /*A
      ** La fiche alarme n'existe pas
      ** return XDC_NOK
      */
         
         return ( XDC_NOK )
         
           
   end
      
   return ( XDC_OK )
         
go  
