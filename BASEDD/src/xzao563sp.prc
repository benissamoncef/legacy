/*E*/
/* Fichier : $Id: xzao563sp.prc,v 1.1 2012/07/04 11:32:25 pc2dpdy Exp $        Release : $Revision: 1.1 $        Date : $Date: 2012/07/04 11:32:25 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzao563sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* VR		17/01/12 : Creation (DEM/1016)
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao563sp.h" */

/* static char *version = "@(#)xzao563.c	1.1 23/18/94 : xzao563" ; */

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

if exists (select * from sysobjects where name = 'XZAO563' and type = 'P')
	drop procedure XZAO563
go

create procedure XZAO563
   @va_Basedd_in             char(3),
   @va_NumeroZone_in		 smallint
   as 

/* 
*
* SP    XZAO563SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd        va_Basedd_in
*					va_NumeroAlerte_in
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
* Affichage de la liste des donnees de la table REG_COM
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* VR    13 Jan 2012    : Creation (DEM/1016)
------------------------------------------------------*/

   
    /*A
    ** Test des parametres d'entree :
    **   Si un des parametres d'entree est egal a null alors XDC_NOK   
    */
      
    if @va_Basedd_in           = null
        
		return XDC_ARG_INV
   
	if @va_Basedd_in = XDC_BASE_CFG
            
					begin
                        /*A
                        ** Lecture des donnees dans REG_COM
                        */

						select CPT.station, AUT.nom, CHA.sens, CHA.PR, zone, GEN.nom
						from   CFG..REG_COM CPT,
							CFG..EQT_GEN GEN,
							CFG..RES_CHA CHA,
							CFG..RES_AUT AUT
						where zone = @va_NumeroZone_in
						and GEN.type = XDC_EQT_RAD 
						and GEN.numero = CPT.station
						and CPT.station = GEN.numero
						and CHA.station=CPT.station
						and AUT.numero = GEN.autoroute	
						and CHA.sens = CPT.sens
					end   

                       

#ifdef CI      
            else
	begin
		if @va_Basedd_in = XDC_BASE_CFT
            
					begin
                        /*A
                        ** Lecture des donnees dans REG_COM
                        */


						select CPT.station, AUT.nom, CHA.sens, CHA.PR, zone, GEN.nom
						from   CFT..REG_COM CPT,
							CFT..EQT_GEN GEN,
							CFT..RES_CHA CHA,
							CFT..RES_AUT AUT
						where zone = @va_NumeroZone_in
						and GEN.type = XDC_EQT_RAD 
						and GEN.numero = CPT.station
						and CPT.station = GEN.numero
						and CHA.station=CPT.station
						and AUT.numero = GEN.autoroute	
						and CHA.sens = CPT.sens
                      
					end   
  
	end
#endif
      
    return XDC_OK
         
go  
