/*E*/
/* Fichier : $Id: xzao556sp.prc,v 1.2 2018/10/11 12:30:55 devgfi Exp $        Release : $Revision: 1.2 $        Date : $Date: 2018/10/11 12:30:55 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzao556sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* VR	13/01/12 : Creation (DEM/1016)
* ABE	31/05/21 : PRV s IP DEM-SAE283 1.2
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao556sp.h" */

/* static char *version = "@(#)xzao556.c	1.1 23/18/94 : xzao556" ; */

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

if exists (select 1 from sysobjects where name = 'XZAO556' and type = 'P')
	drop procedure XZAO556
go

create procedure XZAO556
   @va_Basedd_in             char(3),
   @va_NumeroZone_in		smallint
   as 

/* 
*
* SP    XZAO556SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd        va_Basedd_in
*  					va_NumeroZone_in
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf       va_Resultat_out
*
* CODE RETOUR :
*
* 0    XDC_OK
* 1    XDC_NOK 
*
*
* FONCTION :
*
* Affichage des donnees de la table EQT_PRV
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* VR    13 Jan 2012    : Creation (DEM/1016)
------------------------------------------------------*/
declare @vl_sens tinyint
   
    /*A
    ** Test des parametres d'entree :
    **   Si un des parametres d'entree est egal a null alors XDC_NOK   
    */
      
    if @va_Basedd_in             = null 
        
		return XDC_ARG_INV
 
 if @va_NumeroZone_in = -1
	select @va_NumeroZone_in = null

	if @va_NumeroZone_in = null
		select @vl_sens = XDC_SENS_SUD
	else
		select @vl_sens = sens
		from CFG..ZON_REG
		where numero = @va_NumeroZone_in

	if @va_Basedd_in = XDC_BASE_CFG
	begin
		if @vl_sens = XDC_SENS_SUD
		begin
			if @va_NumeroZone_in = null
				select PRV.numero, AUT.nom, GEN.PR, GEN.sens, PRV.vitesse_nominale, PRV.critique, PRV.type,  PRV.zone, PRV.bretelle_entree, PRV.rappel_nominal, PRV.adresse_ip, PRV.port
				from   CFG..EQT_PRV PRV,
					   CFG..EQT_GEN GEN,
					   EXP..EQT_DSP DSP,
					   CFG..RES_AUT AUT
				where  GEN.type = XDC_EQT_PRV
				and	   GEN.numero = PRV.numero
				and	   GEN.numero = DSP.equipement
				and	   DSP.inhibe = 0
				and		DSP.dernier = 1
				and	DSP.type = XDC_EQT_PRV
				and	DSP.equipement = PRV.numero
				and	   AUT.numero = GEN.autoroute
				order by GEN.PR asc
			else
				select PRV.numero, AUT.nom, GEN.PR, GEN.sens, PRV.vitesse_nominale, PRV.critique, PRV.type,  PRV.zone, PRV.bretelle_entree, PRV.rappel_nominal, PRV.adresse_ip, PRV.port
				from   CFG..EQT_PRV PRV,
					   CFG..EQT_GEN GEN,
					   EXP..EQT_DSP DSP,
					   CFG..RES_AUT AUT
				where  PRV.zone = @va_NumeroZone_in 
				and	   GEN.type = XDC_EQT_PRV
				and	   GEN.numero = PRV.numero
				and	   GEN.numero = DSP.equipement
				and	   DSP.inhibe = 0
				and		DSP.dernier = 1
				and	DSP.type = XDC_EQT_PRV
				and	DSP.equipement = PRV.numero
				and	   AUT.numero = GEN.autoroute
				order by GEN.PR asc
        	end 
		else
		begin
			if @va_NumeroZone_in = null
				select PRV.numero, AUT.nom, GEN.PR, GEN.sens, PRV.vitesse_nominale, PRV.critique, PRV.type,  PRV.zone, PRV.bretelle_entree, PRV.rappel_nominal, PRV.adresse_ip, PRV.port
				from   CFG..EQT_PRV PRV,
					   CFG..EQT_GEN GEN,
					   EXP..EQT_DSP DSP,
					   CFG..RES_AUT AUT 
				where  GEN.type = XDC_EQT_PRV
				and	   GEN.numero = PRV.numero
				and	   GEN.numero = DSP.equipement
				and	   DSP.inhibe = 0
				and		DSP.dernier = 1
				and	DSP.type = XDC_EQT_PRV
				and	DSP.equipement = PRV.numero
				and	   AUT.numero = GEN.autoroute
				order by GEN.PR desc
			else
				select PRV.numero, AUT.nom, GEN.PR, GEN.sens, PRV.vitesse_nominale, PRV.critique, PRV.type,  PRV.zone, PRV.bretelle_entree, PRV.rappel_nominal, PRV.adresse_ip, PRV.port
				from   CFG..EQT_PRV PRV,
					   CFG..EQT_GEN GEN,
					   EXP..EQT_DSP DSP,
					   CFG..RES_AUT AUT 
				where  PRV.zone = @va_NumeroZone_in 
				and	   GEN.type = XDC_EQT_PRV
				and	   GEN.numero = PRV.numero
				and	   GEN.numero = DSP.equipement
				and	   DSP.inhibe = 0
				and		DSP.dernier = 1
				and	DSP.type = XDC_EQT_PRV
				and	DSP.equipement = PRV.numero
				and	   AUT.numero = GEN.autoroute
				order by GEN.PR desc
       		end 
	end   
#ifdef CI      
    else if @va_Basedd_in = XDC_BASE_CFT
	begin
		select @vl_sens = sens 
		from CFT..ZON_REG
		where numero = @va_NumeroZone_in

		if @vl_sens = XDC_SENS_SUD
		begin
			select PRV.numero, AUT.nom, GEN.PR, GEN.sens, PRV.vitesse_nominale, PRV.critique, PRV.type,  PRV.zone, PRV.bretelle_entree, PRV.rappel_nominal, PRV.adresse_ip, PRV.port
			from   CFT..EQT_PRV PRV,
		   		CFT..EQT_GEN GEN,
		   		EXP..EQT_DSP DSP,
				CFT..RES_AUT AUT
			where  PRV.zone = @va_NumeroZone_in 
			and	   GEN.type = XDC_EQT_PRV
			and	   GEN.numero = PRV.numero
			and	   GEN.type = DSP.type
			and	   GEN.numero = DSP.equipement
			and	   DSP.dernier=1
			and	   DSP.inhibe = 0
			and		AUT.numero = GEN.autoroute
			order by GEN.PR asc
		end
		else
		begin
			select PRV.numero, AUT.nom, GEN.PR, GEN.sens, PRV.vitesse_nominale, PRV.critique, PRV.type,  PRV.zone, PRV.bretelle_entree, PRV.rappel_nominal, PRV.adresse_ip, PRV.port
			from   CFT..EQT_PRV PRV,
		   		CFT..EQT_GEN GEN,
		   		EXP..EQT_DSP DSP,
				CFT..RES_AUT AUT
			where  PRV.zone = @va_NumeroZone_in 
			and	   GEN.type = XDC_EQT_PRV
			and	   GEN.numero = PRV.numero
			and	   GEN.type = DSP.type
			and	   GEN.numero = DSP.equipement
			and	   DSP.dernier = 1
			and	   DSP.inhibe = 0
			and		AUT.numero = GEN.autoroute
			order by GEN.PR desc
		end
	end   
 

#endif
      
    return XDC_OK
         
go  
