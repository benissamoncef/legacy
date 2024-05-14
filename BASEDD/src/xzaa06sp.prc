/*E*/
/*Fichier : $Id: xzaa06sp.prc,v 1.6 2011/04/04 15:37:47 pc2dpdy Exp $      Release : $Revision: 1.6 $        Date : $Date: 2011/04/04 15:37:47 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAA * FICHIER xzaa06sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Interface Alerte >
*
------------------------------------------------------
* EXPTORIQUE :
*
* Torregrossa	04 Sep 1995	: creation
* C.P.		07/01/98	: Correction gestion multi-sites dans 
*				  les procs (1.2)
* Torregrossa	07 Jan 1998	: modif interface proc pour PCNIVEAU2 (1532) V1.3
* Niepceron	07 Jul 1998	: modif separation EXP/HIS (1696) V1.'
* Niepceron     24/11/1998	: ajout sit CI dem/1724 V1.5
* JMG		24/03/11 : alertes localisees V1.6
------------------------------------------------------*/

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*  retourner la liste des alertes du site local
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAA;06
	@va_TypeAlerte_in	tinyint = null,
	@va_TypeSite_in		tinyint = null
   as 

/* 
*
* SP	XZAA06_ListeAlertes
*
* PARAMETRES EN ENTREE :
* va_TypeAlerte_in :	types d'alerte a rechercher
*
* PARAMETRES EN SORTIE :
*
* VALEUR RENDUE :
*
* Retourner la liste des alertes du site
* 
* CODE RETOUR :
*
* XDC_OK
* XDC_NOK	: site non prevu
*
* CONDITIONS D'UTILISATION :
*
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
    
   @vl_Site		T_SITE,
   @vl_Creation		tinyint,
   @vl_Status int


   if  @va_TypeAlerte_in = 0 select @va_TypeAlerte_in = null
   if @va_TypeSite_in = 0 select @va_TypeSite_in = null

#ifdef HIST
	exec @vl_Status = SQL_CI.PRC..XZAA;06
	@va_TypeAlerte_in,
	@va_TypeSite_in	

        if @vl_Status = null
                return XDC_PRC_INC
        else return @vl_Status
#else

   /* recherche le site local */
   select @vl_Site = numero 
   from CFG..RES_DIS
   where serveur = @@servername

   if @vl_Site is null
         return XDC_NOK
  
   select @vl_Creation = XDC_VRAI

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
	@vl_Creation,
	ALT.sit,
       LOC.autoroute,
        LOC.PR,
        LOC.sens
   from CFG..EQT_GEN EQT, EXP..ALT_EXP ALT, CFG..TYP_ALT TYP, EXP..ALT_LOC LOC
   where (@va_TypeSite_in is not null or ALT.sit = @vl_Site) and 
	ALT.qualification != XDC_ALT_EXPLOITATION and
	ALT.qualification != XDC_ALT_VRAIE and
	ALT.qualification != XDC_ALT_FAUSSE and
	ALT.qualification != XDC_ALT_VRAIE_EVT and
	ALT.qualification != XDC_ALT_VRAIE_FUSION_FMC_SAE_EXT and
	EQT.numero =ALT.equipement and
	EQT.type = ALT.type_equipement and
	ALT.numero *= LOC.numero and
	ALT.sit *= LOC.sit and
	TYP.numero = ALT.type and
	(@va_TypeAlerte_in is null or ALT.type = @va_TypeAlerte_in) and
	(@va_TypeSite_in is null or ALT.sit in (select numero from CFG..RES_DIS where type = @va_TypeSite_in or type = XDC_TYPE_PCCI))
   order by horodate desc, EQT.autoroute,EQT.PR
         
       
   return ( XDC_OK )
#endif
go  
