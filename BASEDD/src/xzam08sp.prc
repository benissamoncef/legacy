/*E*/
/*Fichier : $Id: xzam08sp.prc,v 1.2 2018/06/27 15:52:45 devgfi Exp $      Release : $Revision: 1.2 $        Date : $Date: 2018/06/27 15:52:45 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAM * FICHIER xzam08sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Interface Alarmes >
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	14/09/16 creation FACTIS 1.1
* JPL	27/06/18 : Ajout directive 'drop procedure'  1.2
------------------------------------------------------*/

/* fichiers inclus */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*   Cree une nouvelle fiche alarme, ou met a jour celle existant deja
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

if exists (select 1 from sysobjects where name = 'XZAM08' and type = 'P')
	drop procedure XZAM08
go


create procedure XZAM08
   @va_Type_in		smallint = null,
   @va_TypeEqt_in	tinyint = null,
   @va_Eqt_in		smallint = null
   as 

/* 
*
* SP	XZAM07SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Horodate		va_Horodate_in
* XDY_TypeEqt		va_TypeEqt_in
* XDY_Eqt		va_Eqt_in
* XDY_TypeAlarme	va_Type_in
* XDY_Booleen		va_Etat_in
*
* PARAMETRES EN SORTIE :
*
* VALEUR RENDUE :
*
* Cree une nouvelle fiche alarme, ou met a jour celle existant deja (Alarme Manuelle)
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
* recherche dans la table ALARME s'il existe deja une fiche alarme pour le meme
* equipement et le meme type d'alarme.
*
* Si c'est le cas, met a jour cette fiche (etat, horodate dernier changement, horodate
* premiere disparition, nombre dapparitions), sinon, cree une nouvelle fiche
*
* Met a jour l'indisponibilite de l'equipement en fonction du type d'alarme (majeure,
* mineure, critique) et de la desactivation eventuelle + appel ATRP18 (XZIS02)
*
* Incremente le compteur globlal d'alarmes de l'equipement
*
* MECANISMES :
*
------------------------------------------------------*/
declare
   
   /*A
   ** Declaration des variables locales
   */
   
   @vl_Site 		tinyint, 
   @vl_Status	 	int
   
     
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_TypeEqt_in = null
   or @va_Eqt_in = null
   or @va_Type_in = null
      
      return ( XDC_ARG_INV )
   
   else   
   
   begin
   
   /*A
   ** Si les parametres sont non nuls
   */
         
     delete from EXP..ALM_IHM
     where type = @va_Type_in
     and type_equipement = @va_TypeEqt_in 
     and equipement = @va_Eqt_in
      
      
   end
      
   return ( XDC_OK )
         
go  
