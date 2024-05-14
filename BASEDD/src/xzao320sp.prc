/*E*/
/*  Fichier : $Id: xzao320sp.prc,v 1.2 2007/03/26 15:46:29 gesconf Exp $      Release : $Revision: 1.2 $        Date : $Date: 2007/03/26 15:46:29 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzao320sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* lecture et ecriture des descriptions des equipements 
* et du reseau
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	11/12/95	: Creation
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* SP	XZAO320_LISTE_VTL
* 
* Arguments en entree
* 
* Arguments en sortie
* 
* liste retournee :
* XDY_Mot	va_NumEqt
* XDY_Autoroute	va_Autoroute
* XDY_PR	va_PR
* XDY_Sens	va_Sens	: position de la station
* XDY_Sens	va_Sens	: sens point de mesure
* XDY_Vitesse	va_Vitesse

* Service rendu
* Recherche la liste des vitesses libres
* 
* Code retour
* XDC_OK
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* 
* Fonction
-----------------------------------------------------------------------*/

use PRC
go

create procedure XZAO;320
as
	create table #LISTE_STATIONS(numero smallint, autoroute tinyint null, PR int null, sens tinyint null, point_mesure tinyint null, VTL int null)

	/*A recherche les stations */
	insert  #LISTE_STATIONS (numero , autoroute , PR , sens , point_mesure)
	select
		numero,
		CFG..EQT_GEN.autoroute,
		CFG..EQT_GEN.PR,
		CFG..EQT_GEN.sens,
		CFG..RES_CHA.sens
	from CFG..EQT_GEN,  CFG..RES_CHA
	where CFG..EQT_GEN.type = XDC_EQT_RAD  and CFG..RES_CHA.station = numero

	/*A recherche les vitesses libres */
	/*
	update #LISTE_STATIONS set VTL = CFG..EQT_RAD_VTL.VTL
	from #LISTE_STATIONS, CFG..EQT_RAD_VTL
	where #LISTE_STATIONS.numero = CFG..EQT_RAD_VTL.station and*/
/*		#LISTE_STATIONS.point_mesure = CFG..EQT_RAD_VTL.sens

	select
		numero,
		autoroute,
		PR,
		sens,
		point_mesure,
		VTL
	from  #LISTE_STATIONS
	*/
	select
		numero,
		autoroute,
		PR, #LISTE_STATIONS.sens,
		point_mesure,
		CFG..EQT_RAD_VTL.VTL
	from  #LISTE_STATIONS, CFG..EQT_RAD_VTL
	where numero *= station and point_mesure *= CFG..EQT_RAD_VTL.sens

	return XDC_OK
go
