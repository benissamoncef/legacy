/*E*/
/*Fichier : $Id: xzaa07sp.prc,v 1.1 1998/03/06 09:51:03 gaborit Exp $      Release : $Revision: 1.1 $        Date : $Date: 1998/03/06 09:51:03 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAA * FICHIER xzaa07sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Interface Alerte >
*
------------------------------------------------------
* EXPTORIQUE :
*
* Torregrossa	05 mar 1998	: creation
------------------------------------------------------*/

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*  retourner la liste des alertes du site local traitees
*  en l'absence de l'operateur depuis la derniere prise
*  poste
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAA;07
	@va_TypeAlerte_in	tinyint = null
  as 

/* 
*
* SP	XZAA07_ListeAlertesTraiteesParCI
*
* PARAMETRES EN ENTREE :
* va_TypeAlerte_in :	types d'alerte a rechercher
*
* PARAMETRES EN SORTIE :
*
* VALEUR RENDUE :
*
* Retourner la liste des alertes tritees du site par le CI
* 
* CODE RETOUR :
*
* XDC_OK
* XDC_NOK	: site non prevu
*
* CONDITIONS D'UTILISATION :
* sur PC niveau 2 ou 3 uniquement
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
    
   @vl_HorodatePrisePoste	datetime,
   @vl_HorodateFinPoste	datetime,
   @vl_NumFiche		int,
   @vl_Site		T_SITE


   if  @va_TypeAlerte_in = 0 select @va_TypeAlerte_in = null

   /* recherche le site local */
   select @vl_Site = numero 
   from CFG..RES_DIS
   where serveur = @@servername

   if @vl_Site is null
         return XDC_NOK
  

   /*A rechercher la derniere date de prise de poste en cours */
   select
	@vl_NumFiche = max(numero)
   from EXP..FMC_GEN
   where type = XZAEC_FMC_PosteOperateur and cle = @vl_Site

   select 
	@vl_HorodatePrisePoste = debut, 
	@vl_HorodateFinPoste = cloture 
   from EXP..FMC_GEN
   where numero = @vl_NumFiche and cle = @vl_Site and cloture is null

   if  @vl_HorodatePrisePoste is null
	select  @vl_HorodatePrisePoste = getdate()

   /*A rechercher la derniere date de fin de poste dans le cas un poste en cours */
   select
	@vl_NumFiche = max(numero)
   from EXP..FMC_GEN
   where type = XZAEC_FMC_PosteOperateur and cle = @vl_Site and numero < @vl_NumFiche

   if @vl_NumFiche is not null
   begin
   	select 
		@vl_HorodateFinPoste = cloture 
   	from EXP..FMC_GEN
   	where numero = @vl_NumFiche and cle = @vl_Site and cloture is not null
   end
   else	/* 1ere prise de poste : pas d'alertes */
	return XDC_OK

   /*A retourner la liste des alertes du site */
   select 	
	ALT.numero,
	ALT.horodate,
       	ALT.type,
       	TYP.nom,
       	EQT.numero,
       	EQT.type,
       	EQT.nom,
       	ALT.texte,
       	EQT.autoroute,
       	EQT.PR,
       	EQT.sens,
       	ALT.qualification,
	ALT.sit
   from CFG..EQT_GEN EQT, EXP..ALT_EXP ALT, CFG..TYP_ALT TYP
   where ALT.sit = @vl_Site and 
	(ALT.qualification = XDC_ALT_EXPLOITATION or
	ALT.qualification = XDC_ALT_VRAIE or
	ALT.qualification = XDC_ALT_FAUSSE or
	ALT.qualification = XDC_ALT_VRAIE_EVT) and
	horodate > @vl_HorodateFinPoste and
	horodate < @vl_HorodatePrisePoste and
	EQT.numero =ALT.equipement and
	EQT.type = ALT.type_equipement and
	TYP.numero = ALT.type and
	(@va_TypeAlerte_in is null or ALT.type = @va_TypeAlerte_in)
   order by horodate desc, EQT.autoroute,EQT.PR
         
       
   return ( XDC_OK )
go  
