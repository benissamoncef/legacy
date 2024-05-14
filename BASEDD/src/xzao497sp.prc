/*E*/
/* Fichier : $Id: xzao497sp.prc,v 1.1 2021/05/03 13:07:44 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2021/05/03 13:07:44 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao497sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* CGR   07/10/20        : Creation 1.1 DEM-SAE155
------------------------------------------------------*/
/* fichiers inclus */
/* #include "xzao497sp.h" */
/* static char *version = "@(#)xzao497sp.prc    1.1 1.1 07/10/20: xzao497" ; */
/* declaration de fonctions internes */
/* definition de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU :
*
*  Liste des Messages pour un BICE donné, ou tous les Messages
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
use PRC
go
if exists (select * from sysobjects where name = 'XZAO497' and type = 'P')
        drop procedure XZAO497
go
create procedure XZAO497
	@va_Basedd_in           char(3),
	@va_IdBICE_in		tinyint
	as
/*
*
* SP    XZAO497SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd    va_Basedd_in
* XDY_Entier    va_IdBICE_in
*
* PARAMETRES EN SORTIE :
*
* VALEUR RENDUE :
*
* Retourne la liste des Messages du ou des BICE
*
* CODE RETOUR :
*
* 0     XDC_OK
* 1     XDC_NOK
*
* FONCTION :
*
* Retourne les parametres numero et libelle des Messages
*
------------------------------------------------------*/
/*A
** Test des parametres d'entree :
** Si un des parametres d'entree est egal a null ou le numero de BICE donne est invalide
** alors XDC_ARG_INV
*/
if @va_Basedd_in = null or @va_IdBICE_in = null or @va_IdBICE_in > 4
	return XDC_ARG_INV
else
	begin
	/*A
	**  Travail dans la base CFG
	*/
	if @va_Basedd_in = XDC_BASE_CFG
		begin
		if ( @va_IdBICE_in = 0 )
			begin
			/*A
			** Recherche de la liste de tous les Messages
			*/
			select distinct numero,
					libelle
			from CFG..EQT_IMU_MSG
			end
		
		else
			begin
			/*A
			** Recherche de la liste des Messages associes au BICE donne
			*/
			
			select distinct numero,
					libelle
			from CFG..EQT_IMU_MSG
			where numero = @va_IdBICE_in
			
			end
		end
#ifdef CI
	else
	
	/*A
	**  Travail dans la base CFT
	*/
	if ( @va_IdBICE_in = 0 )
		begin
		/*A
		** Recherche de la liste de tous les Messages
		*/
		select distinct numero,
				libelle
		from CFT..EQT_IMU_MSG
		end
	
	else
		begin
		
		/*A
		** Recherche de la liste des Messages associes au BICE donne
		*/
		
		select distinct numero,
				libelle
		from CFT..EQT_IMU_MSG
		where numero = @va_IdBICE_in
		
		end
#endif
	end
return XDC_OK
go

