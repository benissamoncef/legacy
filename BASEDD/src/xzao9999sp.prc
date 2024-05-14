#include "/users/pc2dpdy/migrazur/XDMICG/inc/xdc.h"
#include "/users/pc2dpdy/migrazur/XDMICG/inc/xzaec.h"
#include "/users/pc2dpdy/migrazur/XDMICG/inc/xzaac.h"
#include "/users/pc2dpdy/migrazur/XDMICG/inc/xzacc.h"
#include "/users/pc2dpdy/migrazur/XDMICG/inc/xzahc.h"
#include "/users/pc2dpdy/migrazur/XDMICG/inc/xzaoc.h"
/*E*/
/*------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao9999sp.prc
------------------------------------------------------
* HISTORIQUE :
*		
* LCL  27/01/2021 : Creation MOVIS PRA DEM-SAE93 1.0
------------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Indiquer si le serveur sur lequel on se trouve est un serveur de repli en mode PRA
* 
* -------------------------------------------------------
* Sequence d'appel
* SP	XZAO9999_ModePRA
* 
* Arguments en entree
* char(2)		Nom du site
* 
* Arguments en sortie
* XDY_Booleen		Mode PRA
* 
* Code retour								
* XDC_OK
* XDC_NOK
* XDC_ARG_INV		parametre invalide
* <0 erreur sybase 
* 
* Conditions d'utilisation
* 
* Fonction
* Ex :
* Mode nominal
* @va_Site_in = "CI" --> @va_pra_out = XDC_NOK
* @va_Site_in = "DS" --> @va_pra_out = XDC_NOK
* @va_Site_in = "DM" --> @va_pra_out = XDC_NOK
* Basculement PRA
* @va_Site_in = "CI" --> @va_pra_out = XDC_OK
* @va_Site_in = "DS" --> @va_pra_out = XDC_OK
* @va_Site_in = "DM" --> @va_pra_out = XDC_OK
---------------------------------------------------------*/
use PRC
go
		
if exists (select * from sysobjects where name = 'XZAO9999' and type = 'P')
	drop procedure XZAO9999
go				

create procedure XZAO9999
   @va_NomSite_in   	char(2), 
   @va_pra_out 		int output
as 
		
declare @vl_pra int,
	@vl_serveur_pra char(6),
	@vl_nb_instance int

	select @vl_pra = XDC_NOK
	select @va_pra_out = @vl_pra

	select @vl_serveur_pra=serveur_pra
	from CFG..RES_PRA
	where code=@va_NomSite_in

	select numero
	from CFG..RES_PRA
	where serveur_pra=@vl_serveur_pra

	select @vl_nb_instance = @@rowcount

	if @vl_nb_instance = 0
		return XDC_NOK
	else if @vl_nb_instance = 1
		select @vl_pra = XDC_NOK
	else if @vl_nb_instance = 2
		select @vl_pra = XDC_OK
	else
		return XDC_NOK

	select @va_pra_out = @vl_pra

	return XDC_OK
	

go
						
