/*E*/
/* Fichier : $Id: xzao577sp.prc,v 1.2 2018/10/11 14:52:56 devgtie Exp $      Release : $Revision: 1.2 $        Date : $Date: 2018/10/11 14:52:56 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER XZAO577sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* RG    11 Oct 2018    : DEM1307 - Retrait des PRV Barriere Pleine Voie
* VR		13/02/12 : Creation (DEM/1016)
------------------------------------------------------*/

/* fichiers inclus */

/* #include "XZAO577sp.h" */

/* static char *version = "@(#)XZAO577.c	1.1 23/18/94 : XZAO577" ; */

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

if exists (select * from sysobjects where name = 'XZAO577' and type = 'P')
	drop procedure XZAO577
go

create procedure XZAO577
	   @va_Autoroute_in		tinyint,
	   @va_Sens_in			tinyint,
	   @va_Debut_in			int,
	   @va_Fin_in			int
	   as 

/* 
*
* SP    XZAO577SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd        va_Basedd_in
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
* RG    11 Oct 2018    : DEM1307 - Retrait des PRV Barriere Pleine Voie
* VR    16 Fev 2012    : Creation (DEM/1016)
------------------------------------------------------*/

declare @vl_pr_min int, @vl_pr_max int
	
if @va_Autoroute_in = null or
   @va_Debut_in = null or
   @va_Fin_in = null or
   @va_Sens_in = null 
 return XDC_ARG_INV

if @va_Debut_in <= @va_Fin_in
begin
  select @vl_pr_min = @va_Debut_in
  select @vl_pr_max = @va_Fin_in
end
else
begin
  select @vl_pr_max = @va_Debut_in
  select @vl_pr_min = @va_Fin_in	
end
	

/* Ramene le sens sur section courante s'il s'agit d'une sortie ou d'une entree */
if @va_Sens_in = XDC_SENS_SORTIE_NORD or
   @va_Sens_in = XDC_SENS_ENTREE_NORD
 select  @va_Sens_in = XDC_SENS_NORD

if @va_Sens_in=XDC_SENS_SORTIE_SUD or
   @va_Sens_in=XDC_SENS_ENTREE_SUD
  select  @va_Sens_in=XDC_SENS_SUD
        
if @va_Sens_in<>XDC_SENS_NORD and @va_Sens_in<>XDC_SENS_SUD
  return XDC_OK

select distinct EQT.numero,
       DSP.sit,
       @va_Autoroute_in,
       EQT.PR,
       @va_Sens_in
from CFG..EQT_GEN EQT,
     CFG..EQT_PRV PRV, 
     EXP..EQT_DSP DSP
 where EQT.type = XDC_EQT_PRV                                and
       EQT.autoroute = @va_Autoroute_in                      and
       (EQT.sens = XDC_SENS_SUD or EQT.sens = XDC_SENS_NORD) and
       EQT.PR >= @vl_pr_min                                  and
       EQT.PR <= @vl_pr_max                                  and
       PRV.type <> XDC_PRV_BPV                               and     -- DEM1307 - 11/10/2018 - RG
       PRV.numero = EQT.numero                           and
       DSP.equipement = EQT.numero                           and
       DSP.type = XDC_EQT_PRV                                and
       DSP.dernier = 1                                       and     -- derniere dispo connue
       DSP.inhibe = 0                                        and     -- non inhibe
       DSP.desactive = 0                                     and     -- non desactive
       DSP.majeure = 0                                       and     -- en panne
       DSP.HS=0                                                      -- non HS

    return XDC_OK
         
go  
