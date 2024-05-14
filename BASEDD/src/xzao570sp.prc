/*E*/
/* Fichier : $Id: xzao570sp.prc,v 1.1 2012/07/04 11:33:36 pc2dpdy Exp $        Release : $Revision: 1.1 $        Date : $Date: 2012/07/04 11:33:36 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzao570sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* VR		01/02/12 : Creation (DEM/1016)
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao570sp.h" */

/* static char *version = "@(#)xzao570.c	1.1 23/18/94 : xzao570" ; */

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

if exists (select * from sysobjects where name = 'XZAO570' and type = 'P')
	drop procedure XZAO570
go

create procedure XZAO570
   @va_Basedd_in            char(3),
   @va_Simulation_in        tinyint,
   @va_TypeParam_in         tinyint
   as 

/* 
*
* SP    XZAO570SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd        va_Basedd_in
*
* PARAMETRES EN SORTIE :
*
*
* CODE RETOUR :
*
* 0    XDC_OK
* 1    XDC_NOK 
*
*
* FONCTION :
*
* Affichage des donnees de la table REG_PAR
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* VR    13 Jan 2012    : Creation (DEM/1016)
------------------------------------------------------*/

	     

   /*A
    ** Si ce n'est pas en mode de simulation alors on travail sur CFG
    */
	if @va_Simulation_in = 0 or @va_Basedd_in = XDC_BASE_CFG
	begin
		/*A
		** Test des parametres d'entree :
		**   Si le type de parametre d'entree est egal a 0 alors retourner tous les param dans REG_PAR
		*/
		if @va_TypeParam_in = 0    
			select numero, 
				nom,
				valeur, type
				from   CFG..REG_PAR
		else   
			/*A
			** Sinon on retourne la liste des param en fonction du type
			*/
			select numero, 
				nom,
				valeur, type
				from   CFG..REG_PAR
				where type = @va_TypeParam_in
	end           
#ifdef CI
	else if @va_Simulation_in = 1 or @va_Basedd_in = XDC_BASE_CFT
	begin
			/*A
			** Test des parametres d'entree :
			**   Si le type de parametre d'entree est egal a 0 alors retourner tous les param dans REG_PAR
			*/
			if @va_TypeParam_in = 0    
				select numero, 
					nom,
					valeur, type
				from   CFT..REG_PAR
			else   
				/*A
				** Sinon on retourne la liste des param en fonction du type
				*/
				select numero, 
					nom,
					valeur, type
					from   CFT..REG_PAR
					where type = @va_TypeParam_in

	end           
#endif
go  
