/*E*/
/* Fichier : $Id: xzao505sp.prc,v 1.1 2021/09/15 13:50:00 pc2dpdy Exp $        Release : $Revision: 1.1 $        Date : $Date: 2021/09/15 13:50:00 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzao505sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Recherche des equipements impactes par une perturbation
*
------------------------------------------------------
* HISTORIQUE :
*
* ABE	24/05/21	creation IMU 1.1 DEM-SAE155
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
*
* Retourne le numéro d'IMU correspondant au nom du BICE
*
* -------------------------------------------------------
* Sequence d'appel
* SP	XZAO505_IMU_amont
* 
* Arguments en entree
* XDY_Texte		va_NomBICE_in
* 
* Arguments en sortie
* XDY_Eqt		va_NumIMU_out
*
* Code retour
* XDC_OK
* XDC_NOK
* <0 erreur sybase 
* 
* Conditions d'utilisation
* 
---------------------------------------------------------*/

use PRC
go

if exists (select 1 from sysobjects where name = 'XZAO505' and type = 'P')
	drop procedure XZAO505
go

create procedure XZAO505
	@va_NomBICE_in	char(11),
	@va_NumEqt_out	int=null output
as

/* Cherche le numéro de l'équipement correspondant au nom BICE*/
select @va_NumEqt_out = numero from CFG..EQT_IMU
where nom_BICE = @va_NomBICE_in

return XDC_OK
												
go
					
