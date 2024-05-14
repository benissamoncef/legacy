/*E*/
/* Fichier : $Id: xzao494sp.prc,v 1.1 2021/05/03 13:07:32 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2021/05/03 13:07:32 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao494sp.prc
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

/* #include "xzao494sp.h" */

/* static char *version = "@(#)xzao494sp.prc    1.1 1.1 28/07/20: xzao494" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU :
*
*  Liste des Zones pour un BICE donné, ou toutes les zones
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAO494' and type = 'P')
        drop procedure XZAO494
go

create procedure XZAO494
	@va_Basedd_in           char(3),
	@va_IdBICE_in		tinyint
	as

/*
*
* SP    XZAO494SP
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
* Retourne la liste des Zones du ou des BICE
*
* CODE RETOUR :
*
* 0     XDC_OK
* 1     XDC_NOK
*
* FONCTION :
*
* Retourne les parametres numero, id et libelle de Zone
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

		if @va_IdBICE_in = 0
			begin

			/*A
			** Recherche de la liste de toutes les Zones
			*/

			select distinct numero,
					id,
					libelle
			from CFG..EQT_IMU_ZON

			end
		
		else
			begin

			/*A
			** Recherche de la liste des Zones associees au BICE donne
			*/
			
			select distinct numero,
					id,
					libelle
			from CFG..EQT_IMU_ZON
			where numero = @va_IdBICE_in
			
			end
		end

#ifdef CI

	else
	
	/*A
	**  Travail dans la base CFT
	*/

	if @va_IdBICE_in = 0
		begin
	
		/*A
		** Recherche de la liste de toutes les Zones
		*/

		select distinct numero,
				id,
				libelle
		from CFT..EQT_IMU_ZON

		end
	
	else
		begin
		
		/*A
		** Recherche de la liste des Zones associees au BICE donne
		*/
		
		select distinct numero,
				id,
				libelle
		from CFT..EQT_IMU_ZON
		where numero = @va_IdBICE_in
		
		end

#endif
	end

return XDC_OK

go

