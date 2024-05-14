/*E*/
/* Fichier : $Id: xzae562sp.prc,v 1.2 2015/09/01 08:53:45 pc2dpdy Exp $      Release : $Revision: 1.2 $        Date : $Date: 2015/09/01 08:53:45 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae562sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* VR		17/02/12 : Creation (DEM/1016)
* JMG		03/08/15 : accident et vehFeu sur BAU aussi lot 22 DEM131 1.é
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzae562sp.h" */

/* static char *version = "@(#)xzae562.c	1.1 23/18/94 : xzae562" ; */

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

if exists (select * from sysobjects where name = 'XZAE562' and type = 'P')
	drop procedure XZAE562
go

	create procedure XZAE562
	   @va_Horodate_in      datetime,
	   @va_Autoroute_in		smallint,
	   @va_Sens_in			smallint,
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
* XDY_ResConf       va_Resultat_out    

* CODE RETOUR :
*
* 0    XDC_OK
* 1    XDC_NOK 
*
*
* FONCTION :
*
* Recherche les PRV dans la zone de chantier et le premier PRV en amont du chantier.
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* VR    17 Fev 2012    : Creation (DEM/1016)
------------------------------------------------------*/
declare @vl_Status int,
	@vl_min int, @vl_max int
		
        /*A
        ** Verification que le serveur est bien celui du SDHIS
        */
		 if @va_Horodate_in = null
			select @va_Horodate_in=getdate()

		 if @va_Autoroute_in = null or @va_Sens_in = null
			or @va_Debut_in=null or @va_Fin_in = null
				return XDC_ARG_INV

	select @vl_max = @va_Debut_in, @vl_min = @va_Fin_in
	if  @va_Debut_in < @va_Fin_in
	begin
		select @vl_min = @va_Debut_in, @vl_max = @va_Fin_in
	end
	
#ifdef HIST

       /*A
        ** Recherche evenements sur HIS
        */

		select GEN.numero, GEN.cle, GEN.type, HIS.confirme_ou_signale, HIS.VL, HIS.VM1, HIS.VM2, HIS.VR, HIS.VL_I, HIS.VM1_I, HIS.VM2_I, HIS.VR_I,
					REG.qualification
		from   HIS..FMC_GEN GEN,
		CFG..REG_TYP REG,
		HIS..FMC_HIS HIS
		where  GEN.numero = HIS.numero
		and	   GEN.cle = HIS.cle
		and	   HIS.autoroute = @va_Autoroute_in
		and	   HIS.sens = @va_Sens_in
		and    	   not (HIS.PR > @vl_max or HIS.PR <= @vl_min)
		and	   GEN.debut <= @va_Horodate_in
		and	   GEN.fin = null
		and	   GEN.type = REG.type	
		and	( (GEN.type = XZAEC_FMC_Accident) or
			  (GEN.type = XZAEC_FMC_VehFeu) or 
			  (HIS.VL != XDC_VOIE_INCONNUE and HIS.VL != XDC_VOIE_SENS_NORMAL and HIS.VL != XDC_VOIE_INEXISTANTE)  or
			  (HIS.VM1 != XDC_VOIE_INCONNUE and HIS.VM1 != XDC_VOIE_SENS_NORMAL and HIS.VM1 != XDC_VOIE_INEXISTANTE)  or
			  (HIS.VM2 != XDC_VOIE_INCONNUE and HIS.VM2 !=XDC_VOIE_SENS_NORMAL and HIS.VM2 !=XDC_VOIE_INEXISTANTE)  or
			  (HIS.VL != XDC_VOIE_INCONNUE and HIS.VR !=XDC_VOIE_SENS_NORMAL and HIS.VL !=XDC_VOIE_INEXISTANTE)  )
	and	HIS.horodate_validation in (select max(horodate_validation)
			from HIS..FMC_GEN GEN,
		CFG..REG_TYP REG,
		HIS..FMC_HIS HIS
		where  GEN.numero = HIS.numero
		and	   GEN.cle = HIS.cle
		and	   HIS.autoroute = @va_Autoroute_in
		and	   HIS.sens = @va_Sens_in
		and    	   not (HIS.PR > @vl_max or HIS.PR <= @vl_min)
		and	   GEN.debut <= @va_Horodate_in
		and		HIS.horodate_validation <= @va_Horodate_in
		and	   GEN.fin = null
		and	   GEN.type = REG.type	
		and	( (GEN.type = XZAEC_FMC_Accident) or
			  (GEN.type = XZAEC_FMC_VehFeu) or
			  (HIS.VL != XDC_VOIE_INCONNUE and HIS.VL != XDC_VOIE_SENS_NORMAL and HIS.VL != XDC_VOIE_INEXISTANTE)  or
			  (HIS.VM1 != XDC_VOIE_INCONNUE and HIS.VM1 != XDC_VOIE_SENS_NORMAL and HIS.VM1 != XDC_VOIE_INEXISTANTE)  or
			  (HIS.VM2 != XDC_VOIE_INCONNUE and HIS.VM2 !=XDC_VOIE_SENS_NORMAL and HIS.VM2 !=XDC_VOIE_INEXISTANTE)  or
			  (HIS.VL != XDC_VOIE_INCONNUE and HIS.VR !=XDC_VOIE_SENS_NORMAL and HIS.VL !=XDC_VOIE_INEXISTANTE)  )
			)

#else
	
	/*A si la date donnee est anterieure d'un mois au moins */
 	/*A rechercher l'evenement dans la base HIS sinon dans  */
	if @va_Horodate_in < (select dateadd(month, -1, getdate ()))
		begin	
			exec @vl_Status = SQL_HIS.PRC..XZAE562
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
	
	       /*A
	        ** Recherche evenements sur les autres serveurs
	         */
			select GEN.numero, GEN.cle, GEN.type, HIS.confirme_ou_signale, HIS.VL, HIS.VM1, HIS.VM2, HIS.VR, HIS.VL_I, HIS.VM1_I, HIS.VM2_I, HIS.VR_I,
						REG.qualification
			from   EXP..FMC_GEN GEN,
			CFG..REG_TYP REG,
			EXP..FMC_HIS_DER HIS
			where  GEN.numero = HIS.numero
			and	   GEN.cle = HIS.cle
			and	   HIS.autoroute = @va_Autoroute_in
			and	   HIS.sens = @va_Sens_in
			and    	   not (HIS.PR > @vl_max or HIS.PR <= @vl_min)
			and	   GEN.debut <= @va_Horodate_in
			and	   GEN.fin = null
			and	   GEN.type = REG.type	
			and	( (GEN.type = XZAEC_FMC_Accident) or
				  (GEN.type = XZAEC_FMC_VehFeu) or
				  (HIS.VL != XDC_VOIE_INCONNUE and HIS.VL != XDC_VOIE_SENS_NORMAL and HIS.VL != XDC_VOIE_INEXISTANTE)  or
				  (HIS.VM1 != XDC_VOIE_INCONNUE and HIS.VM1 != XDC_VOIE_SENS_NORMAL and HIS.VM1 != XDC_VOIE_INEXISTANTE)  or
				  (HIS.VM2 != XDC_VOIE_INCONNUE and HIS.VM2 !=XDC_VOIE_SENS_NORMAL and HIS.VM2 !=XDC_VOIE_INEXISTANTE)  or
				  (HIS.VL != XDC_VOIE_INCONNUE and HIS.VR !=XDC_VOIE_SENS_NORMAL and HIS.VL !=XDC_VOIE_INEXISTANTE)  )
		end
#endif
      
    return XDC_OK
         
go  								
