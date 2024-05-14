/*E*/
/*Fichier : $Id: xzan30sp.prc,v 1.4 1997/01/21 11:42:52 verdier Exp $      Release : $Revision: 1.4 $        Date : $Date: 1997/01/21 11:42:52 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAN * FICHIER xzan30sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Annuaire >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	26 Oct 1994	: Creation
* gaborit	06 dec 1994	: modif arguments et traitement 1.2
* Guilhou	14 nov 1996	: refonte de la gestion des astreintes (DEM/1306) 1.3
* Guilhou	21 jan 1997	: correction recup horodates debut et fin (DEM/1384) 1.4
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzan30sp.h" */

/* static char *version = "@(#)xzan30.c	1.1 23/18/94 : xzan30" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Retourne la fiche annuaire associee a une competence pour une horodate
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAN;30
   @va_NumCompetence_in		smallint = null,
   @va_Horodate_in		datetime = null,
   @va_NumFicheAnnuaire_in	smallint = null,
   @va_HoroDebut_out		datetime = null output,
   @va_HoroFin_out		datetime = null output
   as 

/* 
*
* SP	XZAN30SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Mot		va_NumCompetence_in
* XDY_Horodate		va_Horodate_in
* XDY_Mot		va_NumFicheAnnuaire_in
*
* PARAMETRES EN SORTIE :
*
* XDY_Horodate		va_HoroDebut_out
* XDY_Horodate		va_HoroFin_out
*
* VALEUR RENDUE :
*
* Retourne la fiche annuaire associee a une competence pour une horodate
* 
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK
* Horodate invalide (non comprise entre aujourd'hui et aujourd'hui + 400 )
* NumCompetence inconnu
*
* CONDITIONS D'UTILISATION :
*
* FONCTION :
*
* MECANISMES :
*
------------------------------------------------------*/
declare

   /*A
   ** Declaration des variables locales
   */
    
   @vl_Heure	datetime,
   @vl_Date	datetime,
   @vl_DateTmp	datetime,
   @vl_HeureCar char(9)
       
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_Horodate_in = "1900"
   or @va_NumCompetence_in = 0
      
      return ( XDC_ARG_INV )
   
   else   
   
   /*A
   ** Si les parametres sont non nuls
   */
   begin
	if @va_NumFicheAnnuaire_in=0 select @va_NumFicheAnnuaire_in=null
	
	select @va_HoroDebut_out=@va_Horodate_in,
		@va_HoroFin_out=@va_Horodate_in
	
/*A recuperation de l'heure de debut d'astreinte */

	select @vl_Heure=heure_de_debut
	from CFG..ANN_AST_CMP
	where numero=@va_NumCompetence_in
	
	if @@rowcount=0 return XDC_ARG_INV
	
/*A recuperation de la date correspondante dans le calendrier */

	select @va_HoroFin_out=date_fin, @va_HoroDebut_out=date_debut
	from CFG..ANN_AST_CAL
	where   competence=@va_NumCompetence_in and
		interlocuteur=@va_NumFicheAnnuaire_in and
		date_debut <= @va_Horodate_in and
		date_fin >= @va_Horodate_in

	if @@rowcount=0 return XDC_OK -- personne dans le calendrier
   end
     
   return ( XDC_OK )
         
go  
