/*E*/
/* Fichier : $Id: xzae563sp.prc,v 1.2 2019/01/17 18:21:15 pc2dpdy Exp $      Release : $Revision: 1.2 $        Date : $Date: 2019/01/17 18:21:15 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae563sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* VR		13/02/12 : Creation (DEM/1016)
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzae563sp.h" */

/* static char *version = "@(#)xzae563.c	1.1 23/18/94 : xzae563" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAE563' and type = 'P')
	drop procedure XZAE563
go

	create procedure XZAE563
	   @va_Horodate_in      datetime = null,
	   @va_Autoroute_in		tinyint,
	   @va_Sens_in			tinyint,
	   @va_Debut_in			int,
	   @va_Fin_in			int
	   as 

/* 
*
* SP    XZAO553SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd        va_Basedd_in
*  					va_Horodate_in
*  					va_Autoroute_in
* 					va_Sens_in
*  					va_Debut_in
*  					va_Fin_in
*
* PARAMETRES EN SORTIE :
*

* CODE RETOUR :
*
* 0    XDC_OK
* 1    XDC_NOK 
*
*
* FONCTION :
*
* 
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* VR    13 Fev 2012    : Creation (DEM/1016)
------------------------------------------------------*/
declare @vl_Status int
		
        /*A
        ** Verification que le serveur est bien celui du SDHIS
        */
 if @va_Horodate_in = null
	select @va_Horodate_in=getdate()

	if @va_Autoroute_in = null or @va_Sens_in = null
		or @va_Debut_in=null or @va_Fin_in = null
			return XDC_ARG_INV

#ifdef HIST
	select GEN.numero, GEN.cle, GEN.type, 
		HIS.PR , TRH.localisation_tete, vitesse, personnel
	from   HIS..FMC_GEN GEN,
	   HIS..FMC_HIS HIS,
	   HIS..FMC_TRH TRH
	where  GEN.numero = HIS.numero
	and	   GEN.cle = HIS.cle
	and	   HIS.autoroute = @va_Autoroute_in
	and	   HIS.sens = @va_Sens_in
	and    not (HIS.PR < @va_Debut_in or HIS.PR > @va_Fin_in)
	and	   GEN.debut <=  @va_Horodate_in
	and 	   TRH.numero = GEN.numero
	and	   TRH.cle = GEN.cle
	and	   TRH.horodate= HIS.horodate_validation
	and	   not (TRH.localisation_tete < @va_Debut_in or TRH.localisation_tete > @va_Fin_in)
	and	   GEN.fin = null
	and	   GEN.type in ( XZAEC_FMC_Travaux ,  XZAEC_FMC_ZoneStockage )
	and 	(HIS.VL = XDC_VOIE_SENS_NORMAL or HIS.VM1 = XDC_VOIE_SENS_NORMAL or HIS.VM2  = XDC_VOIE_SENS_NORMAL or HIS.VR = XDC_VOIE_SENS_NORMAL)
	and	horodate_validation in (select max(horodate_validation) from HIS..FMC_HIS HIS
						where HIS.numero = GEN.numero
						and HIS.cle = GEN.cle)
   union
	select GEN.numero, GEN.cle, GEN.type, 
		HIS.PR , BAS.localisation_PR_fin, vitesse,0
	from   HIS..FMC_GEN GEN,
	   HIS..FMC_HIS HIS,
	   HIS..FMC_BAS BAS
	where  GEN.numero = HIS.numero
	and	   GEN.cle = HIS.cle
	and	   HIS.autoroute = @va_Autoroute_in
	and	   HIS.sens = @va_Sens_in
	and    not (HIS.PR < @va_Debut_in or HIS.PR > @va_Fin_in)
	and	   GEN.debut <=  @va_Horodate_in
	and 	   BAS.numero = GEN.numero
	and	   BAS.cle = GEN.cle
	and	   BAS.zz= GEN.zz
	and	   not (BAS.localisation_PR_fin < @va_Debut_in or BAS.localisation_PR_fin > @va_Fin_in)
	and	   GEN.fin = null
	and	   GEN.type in ( XZAEC_FMC_BasculTravaux, XZAEC_FMC_Basculement)
	and	horodate_validation in (select max(horodate_validation) from HIS..FMC_HIS HIS
						where HIS.numero = GEN.numero
						and HIS.cle = GEN.cle)

#else
	/*A si la date donnee est anterieure d'un mois au moins */
 	/*A rechercher l'evenement dans la base HIS sinon dans  */
	if @va_Horodate_in < (select dateadd(month, -1, getdate ()))
	begin	
		exec @vl_Status = SQL_HIS.PRC..XZAE563
							   @va_Horodate_in,
							   @va_Autoroute_in,
							   @va_Sens_in,
							   @va_Debut_in,
							   @va_Fin_in
        if @vl_Status = null
                return XDC_PRC_INC
        else return @vl_Status
	end
	else
	begin
		select GEN.numero, GEN.cle, GEN.type, 
			HIS.PR , TRH.localisation_tete, vitesse, personnel
		from   EXP..FMC_GEN GEN,
		   EXP..FMC_HIS_DER HIS,
		   EXP..FMC_TRH TRH
		where  GEN.numero = HIS.numero
		and	   GEN.cle = HIS.cle
		and	   HIS.autoroute = @va_Autoroute_in
		and	   HIS.sens = @va_Sens_in
		and    not (HIS.PR < @va_Debut_in or HIS.PR > @va_Fin_in)
		and	   GEN.debut <=  @va_Horodate_in
		and 	   TRH.numero = GEN.numero
		and	   TRH.cle = GEN.cle
		and	   TRH.horodate= HIS.horodate_validation
		and	   not (TRH.localisation_tete < @va_Debut_in or TRH.localisation_tete > @va_Fin_in)
		and	   GEN.fin = null
		and	   GEN.type in ( XZAEC_FMC_Travaux ,  XZAEC_FMC_ZoneStockage )
		and 	(HIS.VL = XDC_VOIE_SENS_NORMAL or HIS.VM1 = XDC_VOIE_SENS_NORMAL or HIS.VM2  = XDC_VOIE_SENS_NORMAL or HIS.VR = XDC_VOIE_SENS_NORMAL)
		and	horodate_validation in (select max(horodate_validation) from EXP..FMC_HIS_DER HIS
							where HIS.numero = GEN.numero
							and HIS.cle = GEN.cle)
	   union
		select GEN.numero, GEN.cle, GEN.type, 
			HIS.PR , BAS.localisation_PR_fin, vitesse,0
		from   EXP..FMC_GEN GEN,
		   EXP..FMC_HIS_DER HIS,
		   EXP..FMC_BAS BAS
		where  GEN.numero = HIS.numero
		and	   GEN.cle = HIS.cle
		and	   HIS.autoroute = @va_Autoroute_in
		and	   HIS.sens = @va_Sens_in
		and    not (HIS.PR < @va_Debut_in or HIS.PR > @va_Fin_in)
		and	   GEN.debut <=  @va_Horodate_in
		and 	   BAS.numero = GEN.numero
		and	   BAS.cle = GEN.cle
		and	   BAS.zz= GEN.zz
		and	   not (BAS.localisation_PR_fin < @va_Debut_in or BAS.localisation_PR_fin > @va_Fin_in)
		and	   GEN.fin = null
		and	   GEN.type in ( XZAEC_FMC_BasculTravaux, XZAEC_FMC_Basculement)
		and	horodate_validation in (select max(horodate_validation) from EXP..FMC_HIS_DER HIS
							where HIS.numero = GEN.numero
							and HIS.cle = GEN.cle)
	end
#endif
    return XDC_OK
         
go  
